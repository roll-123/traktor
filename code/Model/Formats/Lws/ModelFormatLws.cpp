#include <map>
#include "Core/Io/File.h"
#include "Core/Io/FileSystem.h"
#include "Core/Io/IStream.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/Transform.h"
#include "Core/Math/Vector4.h"
#include "Core/Misc/String.h"
#include "Model/Joint.h"
#include "Model/Model.h"
#include "Model/Formats/Lws/LwsDocument.h"
#include "Model/Formats/Lws/LwsGroup.h"
#include "Model/Formats/Lws/LwsValue.h"
#include "Model/Formats/Lws/ModelFormatLws.h"

namespace traktor
{
	namespace model
	{

T_IMPLEMENT_RTTI_FACTORY_CLASS(L"traktor.model.ModelFormatLws", 0, ModelFormatLws, ModelFormat)

void ModelFormatLws::getExtensions(std::wstring& outDescription, std::vector< std::wstring >& outExtensions) const
{
	outDescription = L"LightWave Scene";
	outExtensions.push_back(L"lws");
}

bool ModelFormatLws::supportFormat(const std::wstring& extension) const
{
	const std::wstring ext = L"lws";
	return compareIgnoreCase(extension, ext) == 0;
}

Ref< Model > ModelFormatLws::read(const Path& filePath, const std::wstring& filter) const
{
	Ref< IStream > stream = FileSystem::getInstance().open(filePath, File::FmRead);
	if (!stream)
		return nullptr;

	Ref< LwsDocument > document = LwsDocument::parse(stream);
	stream->close();
	if (!document)
		return nullptr;

	LwsGroup* rootGroup = document->getRootGroup();
	if (!rootGroup)
		return nullptr;

	Ref< Model > model = new Model();

	// Adapted verbatim from the 2017 SkeletonFormatLws, building model joints
	// (a geometry-less jointed model) instead of an animation skeleton; the
	// SkeletonPipeline converts model joints -> animation joints downstream.
	std::map< std::wstring, int32_t > nameCount;
	std::map< int32_t, int32_t > mm;	// LWS bone id -> model joint index
	int32_t id = 0;

	Joint current;
	bool have = false;

	float channels[8] = { 0.0f };

	const uint32_t count = rootGroup->getCount();
	for (uint32_t i = 0; i < count; ++i)
	{
		const LwsValue* value = dynamic_type_cast< const LwsValue* >(rootGroup->get(i));
		if (!value)
			continue;

		if (value->getName() == L"AddBone" && value->getCount() >= 1)
		{
			if (have)
				mm[id] = (int32_t)model->addJoint(current);

			current = Joint();
			have = true;

			id = value->getInteger(0);
		}

		if (have && value->getName() == L"BoneName" && value->getCount() >= 1)
		{
			current.setName(value->getString(0));
		}

		if (have && value->getName() == L"BoneWeightMapName" && value->getCount() >= 1)
		{
			std::wstring jointName = value->getString(0);

			const int32_t jointNameIndex = nameCount[jointName]++;
			if (jointNameIndex > 0)
				jointName += L"_" + toString(jointNameIndex);

			current.setName(jointName);
		}

		if (have && value->getName() == L"Channel")
		{
			const int32_t channelIndex = value->getInteger(0);
			if (channelIndex >= 0 && channelIndex < (int32_t)sizeof_array(channels))
			{
				if (i + 1 >= count)
					return nullptr;

				const LwsGroup* env = dynamic_type_cast< const LwsGroup* >(rootGroup->get(i + 1));
				if (!env)
					return nullptr;

				const LwsValue* key = dynamic_type_cast< const LwsValue* >(env->find(L"Key"));
				if (key)
					channels[channelIndex] = key->getFloat(0);
			}
		}

		if (have && value->getName() == L"ParentItem" && value->getCount() >= 1)
		{
			const int32_t parent = value->getInteger(0);

			std::map< int32_t, int32_t >::const_iterator it = mm.find(parent);
			if (it != mm.end())
				current.setParent((uint32_t)it->second);

			const float h = channels[3 + 0];
			const float p = channels[3 + 1];
			const float b = channels[3 + 2];

			const Transform T(
				Vector4(channels[0], channels[1], channels[2], 0.0f),
				Quaternion::fromEulerAngles(0.0f, 0.0f, b) *
				Quaternion::fromEulerAngles(0.0f, p, 0.0f) *
				Quaternion::fromEulerAngles(h, 0.0f, 0.0f)
			);
			current.setTransform(T);

			mm[id] = (int32_t)model->addJoint(current);
			have = false;
		}
	}

	if (have)
		model->addJoint(current);

	return model;
}

bool ModelFormatLws::write(const Path& filePath, const Model* model) const
{
	return false;
}

	}
}
