/*
 * TRAKTOR
 * Copyright (c) 2022-2024 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "Editor/IPipeline.h"

// import/export mechanism.
#undef T_DLLCLASS
#if defined(T_EDITOR_EXPORT)
#	define T_DLLCLASS T_DLLEXPORT
#else
#	define T_DLLCLASS T_DLLIMPORT
#endif

namespace traktor::editor
{

/*! Default pipeline.
 * \ingroup Editor
 *
 * This pipeline is used for every source instance
 * which hasn't a dedicated pipeline.
 * 
 * By default this pipeline doesn't request
 * caching, override shouldCache if caching should be preferred.
 *
 * It copies the source instance to the
 * output database without any modification.
 */
class T_DLLCLASS DefaultPipeline : public IPipeline
{
	T_RTTI_CLASS;

public:
	virtual bool create(const IPipelineSettings* settings, db::Database* database) override;

	virtual void destroy() override;

	virtual TypeInfoSet getAssetTypes() const override;

	virtual bool shouldCache() const override;

	virtual uint32_t hashAsset(const ISerializable* sourceAsset) const override;

	virtual bool buildDependencies(
		IPipelineDepends* pipelineDepends,
		const db::Instance* sourceInstance,
		const ISerializable* sourceAsset,
		const std::wstring& outputPath,
		const Guid& outputGuid
	) const override;

	virtual bool buildOutput(
		IPipelineBuilder* pipelineBuilder,
		const PipelineDependencySet* dependencySet,
		const PipelineDependency* dependency,
		const db::Instance* sourceInstance,
		const ISerializable* sourceAsset,
		const std::wstring& outputPath,
		const Guid& outputGuid,
		const Object* buildParams,
		uint32_t reason
	) const override;

	virtual Ref< ISerializable > buildProduct(
		IPipelineBuilder* pipelineBuilder,
		const db::Instance* sourceInstance,
		const ISerializable* sourceAsset,
		const Object* buildParams
	) const override;
};

}
