/*
 * TRAKTOR
 * Copyright (c) 2022 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "Editor/IPipelineCache.h"

// import/export mechanism.
#undef T_DLLCLASS
#if defined(T_EDITOR_EXPORT)
#	define T_DLLCLASS T_DLLEXPORT
#else
#	define T_DLLCLASS T_DLLIMPORT
#endif

namespace traktor::editor
{

class T_DLLCLASS FilePipelineCache : public IPipelineCache
{
	T_RTTI_CLASS;

public:
	virtual bool create(const PropertyGroup* settings) override final;

	virtual void destroy() override final;

	virtual Ref< IStream > get(const Guid& guid, const PipelineDependencyHash& hash) override final;

	virtual Ref< IStream > put(const Guid& guid, const PipelineDependencyHash& hash) override final;

	virtual bool commit(const Guid& guid, const PipelineDependencyHash& hash) override final;

	virtual Ref< IStream > get(const Key& key) override final;

	virtual Ref< IStream > put(const Key& key) override final;
	
	virtual void getInformation(OutputStream& os) override final;

private:
	bool m_accessRead = true;
	bool m_accessWrite = true;
	std::wstring m_path;
	uint32_t m_hits = 0;
	uint32_t m_misses = 0;
};

}
