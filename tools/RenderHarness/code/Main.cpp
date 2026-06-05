/*
 * TRAKTOR
 * Copyright (c) 2026 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// RenderHarness
//
// Headless per-part mesh render harness for the GearUp engine port. Walks a
// source database, finds every traktor.mesh.MeshAsset, loads its source model
// (.lwo/.fbx) through the CPU ModelRasterizer and writes a framed PNG plus a
// log line per asset. No GPU / render system / window involved, so it runs
// truly headless and side-steps the editor's Vulkan-viewport flakiness.
//
// It deliberately renders the *source* mesh (one PNG per MeshAsset), which
// needs no cooked output database and directly exercises the restored LWO
// importer. True per-entity (part) composition is a later hop.

#include "Core/Io/FileOutputStream.h"
#include "Core/Io/FileSystem.h"
#include "Core/Io/IStream.h"
#include "Core/Io/Utf8Encoding.h"
#include "Core/Log/Log.h"
#include "Core/Log/LogRedirectTarget.h"
#include "Core/Log/LogStreamTarget.h"
#include "Core/Math/Aabb3.h"
#include "Core/Math/Color4f.h"
#include "Core/Math/Const.h"
#include "Core/Math/Matrix44.h"
#include "Core/Math/Vector4.h"
#include "Core/Misc/CommandLine.h"
#include "Core/Misc/String.h"
#include "Core/Misc/TString.h"
#include "Core/Rtti/TypeInfo.h"
#include "Database/Database.h"
#include "Database/Group.h"
#include "Database/Instance.h"
#include "Database/Local/LocalDatabase.h"
#include "Drawing/Image.h"
#include "Drawing/PixelFormat.h"
#include "Mesh/Editor/MeshAsset.h"
#include "Model/Model.h"
#include "Model/ModelCache.h"
#include "Model/ModelRasterizer.h"
#include "Model/Operations/Triangulate.h"

using namespace traktor;

namespace
{

/*! Recursively gather every MeshAsset instance below a group. */
void collectMeshAssets(db::Group* group, RefArray< db::Instance >& outInstances)
{
	RefArray< db::Instance > childInstances;
	group->getChildInstances(childInstances);
	for (auto instance : childInstances)
	{
		const TypeInfo* primaryType = instance->getPrimaryType();
		if (primaryType != nullptr && is_type_of(type_of< mesh::MeshAsset >(), *primaryType))
			outInstances.push_back(instance);
	}

	RefArray< db::Group > childGroups;
	group->getChildGroups(childGroups);
	for (auto childGroup : childGroups)
		collectMeshAssets(childGroup, outInstances);
}

/*! Turn a database instance path into a safe file name. */
std::wstring sanitize(const std::wstring& path)
{
	std::wstring r = path;
	for (auto& c : r)
	{
		if (c == L'/' || c == L'\\' || c == L':' || c == L'*' || c == L'?' ||
			c == L'"' || c == L'<' || c == L'>' || c == L'|' || c == L' ')
			c = L'_';
	}
	// Trim leading separators so we don't emit "__foo".
	while (!r.empty() && r.front() == L'_')
		r.erase(r.begin());
	return r;
}

}

int main(int argc, const char** argv)
{
	CommandLine cmdLine(argc, argv);

	// Force the local database provider to link so the connection string can
	// resolve "traktor.db.LocalDatabase".
	T_FORCE_LINK_REF(db::LocalDatabase);

	traktor::log::info << L"RenderHarness; built '" << mbstows(__TIME__) << L" - " << mbstows(__DATE__) << L"'" << Endl;

	if (cmdLine.hasOption(L'h', L"help"))
	{
		traktor::log::info << L"Usage: RenderHarness (options)" << Endl;
		traktor::log::info << L"  -db=<connection>   Source database (default data/Source local)" << Endl;
		traktor::log::info << L"  -assets=<path>     Asset root for relative model paths (default data/Assets)" << Endl;
		traktor::log::info << L"  -cache=<path>      Model cache directory (default data/Temp/ModelCache)" << Endl;
		traktor::log::info << L"  -out=<path>        Output directory for PNGs (default _render)" << Endl;
		traktor::log::info << L"  -size=<n>          Image size in pixels, square (default 512)" << Endl;
		traktor::log::info << L"  -angle=<deg>       Yaw angle in degrees (default 30)" << Endl;
		traktor::log::info << L"  -filter=<substr>   Only render instances whose path contains <substr>" << Endl;
		return 0;
	}

	const std::wstring dbConnection = cmdLine.hasOption(L"db") ?
		cmdLine.getOption(L"db").getString() :
		L"provider=traktor.db.LocalDatabase;groupPath=data/Source;binary=false";
	const std::wstring assetPath = cmdLine.hasOption(L"assets") ? cmdLine.getOption(L"assets").getString() : L"data/Assets";
	const std::wstring cachePath = cmdLine.hasOption(L"cache") ? cmdLine.getOption(L"cache").getString() : L"data/Temp/ModelCache";
	const std::wstring outPath = cmdLine.hasOption(L"out") ? cmdLine.getOption(L"out").getString() : L"_render";
	const int32_t size = cmdLine.hasOption(L"size") ? cmdLine.getOption(L"size").getInteger() : 512;
	const float angleDeg = cmdLine.hasOption(L"angle") ? (float)cmdLine.getOption(L"angle").getInteger() : 30.0f;
	const std::wstring filter = cmdLine.hasOption(L"filter") ? cmdLine.getOption(L"filter").getString() : L"";

	if (!FileSystem::getInstance().makeAllDirectories(outPath))
	{
		traktor::log::error << L"Unable to create output directory \"" << outPath << L"\"." << Endl;
		return 1;
	}

	// Tee the log into <out>/render.log.
	Ref< IStream > logFile = FileSystem::getInstance().open(outPath + L"/render.log", File::FmWrite);
	if (logFile)
	{
		Ref< LogStreamTarget > logStreamTarget = new LogStreamTarget(new FileOutputStream(logFile, new Utf8Encoding()));
		traktor::log::info   .setGlobalTarget(new LogRedirectTarget(logStreamTarget, traktor::log::info   .getGlobalTarget()));
		traktor::log::warning.setGlobalTarget(new LogRedirectTarget(logStreamTarget, traktor::log::warning.getGlobalTarget()));
		traktor::log::error  .setGlobalTarget(new LogRedirectTarget(logStreamTarget, traktor::log::error  .getGlobalTarget()));
	}

	// Open source database.
	Ref< db::Database > database = new db::Database();
	if (!database->open(dbConnection))
	{
		traktor::log::error << L"Unable to open database \"" << dbConnection << L"\"." << Endl;
		return 2;
	}

	// Enumerate every mesh asset.
	RefArray< db::Instance > meshInstances;
	collectMeshAssets(database->getRootGroup(), meshInstances);
	traktor::log::info << L"Found " << (int32_t)meshInstances.size() << L" mesh asset(s)." << Endl;

	int32_t ok = 0, failed = 0, skipped = 0;
	for (auto instance : meshInstances)
	{
		const std::wstring path = instance->getPath();
		if (!filter.empty() && path.find(filter) == std::wstring::npos)
		{
			++skipped;
			continue;
		}

		Ref< const mesh::MeshAsset > asset = instance->getObject< mesh::MeshAsset >();
		if (!asset)
		{
			traktor::log::error << L"FAIL " << path << L" : unable to deserialize MeshAsset." << Endl;
			++failed;
			continue;
		}

		// Resolve and load the source model through the (uncooked) model cache.
		const Path fileName = FileSystem::getInstance().getAbsolutePath(Path(assetPath), Path(asset->getFileName()));
		Ref< model::Model > model = model::ModelCache::getInstance().getMutable(cachePath, fileName, asset->getImportFilter());
		if (!model)
		{
			traktor::log::error << L"FAIL " << path << L" : unable to read model \"" << asset->getFileName().getPathName() << L"\"." << Endl;
			++failed;
			continue;
		}

		if (!model->apply(model::Triangulate()))
		{
			traktor::log::error << L"FAIL " << path << L" : triangulate failed." << Endl;
			++failed;
			continue;
		}

		// Frame the model: centre it, normalise by its XY extent, push back along Z.
		// (Mirrors mesh::MeshAssetRasterizer so output matches the editor preview.)
		const Aabb3 boundingBox = model->getBoundingBox();
		const Scalar maxExtent = (boundingBox.getExtent() * Vector4(1.0f, 1.0f, 0.0f, 0.0f)).max();
		if (maxExtent <= 0.0_simd)
		{
			traktor::log::error << L"FAIL " << path << L" : degenerate bounding box (no renderable geometry)." << Endl;
			++failed;
			continue;
		}
		const Scalar invMaxExtent = 1.0_simd / maxExtent;
		const Matrix44 modelView =
			translate(0.0f, 0.0f, 2.5f) *
			scale(invMaxExtent, invMaxExtent, invMaxExtent) *
			rotateY(deg2rad(angleDeg)) *
			translate(-boundingBox.getCenter());

		Ref< drawing::Image > image = new drawing::Image(drawing::PixelFormat::getR8G8B8A8(), size, size);
		image->clear(Color4f(0.10f, 0.10f, 0.12f, 1.0f));

		if (!model::ModelRasterizer().generate(model, modelView, image))
		{
			traktor::log::error << L"FAIL " << path << L" : rasterize failed." << Endl;
			++failed;
			continue;
		}

		const std::wstring outFile = outPath + L"/" + sanitize(path) + L".png";
		if (!image->save(outFile))
		{
			traktor::log::error << L"FAIL " << path << L" : unable to save \"" << outFile << L"\"." << Endl;
			++failed;
			continue;
		}

		traktor::log::info << L"OK   " << path
			<< L" (verts=" << (int32_t)model->getPositions().size()
			<< L", polys=" << (int32_t)model->getPolygons().size()
			<< L") -> " << outFile << Endl;
		++ok;
	}

	traktor::log::info << L"Done. " << ok << L" ok, " << failed << L" failed, " << skipped << L" skipped." << Endl;

	database->close();

	if (logFile)
		logFile->close();

	return failed > 0 ? 1 : 0;
}
