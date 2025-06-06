/*
 * TRAKTOR
 * Copyright (c) 2022-2025 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "Render/Editor/IProgramCompiler.h"

// import/export mechanism.
#undef T_DLLCLASS
#if defined(T_RENDER_VULKAN_EDITOR_EXPORT)
#	define T_DLLCLASS T_DLLEXPORT
#else
#	define T_DLLCLASS T_DLLIMPORT
#endif

namespace traktor::render
{

/*! Vulkan program compiler.
 * \ingroup Render
 */
class T_DLLCLASS ProgramCompilerVk : public IProgramCompiler
{
	T_RTTI_CLASS;

public:
	virtual bool create(IProgramCompiler* embedded) override final;

	virtual const wchar_t* getRendererSignature() const override final;

	virtual Ref< ProgramResource > compile(
		const ShaderGraph* shaderGraph,
		const ShaderModule* shaderModule,
		const PropertyGroup* settings,
		const std::wstring& name,
		std::list< Error >& outErrors) const override final;

	virtual bool generate(
		const ShaderGraph* shaderGraph,
		const ShaderModule* shaderModule,
		const PropertyGroup* settings,
		const std::wstring& name,
		Output& output) const override final;
};

}
