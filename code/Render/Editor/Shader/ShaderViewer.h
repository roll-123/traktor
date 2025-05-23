/*
 * TRAKTOR
 * Copyright (c) 2022-2025 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "Ui/Container.h"

#include <map>

namespace traktor
{

class Job;

}

namespace traktor::editor
{

class IEditor;

}

namespace traktor::ui
{

class CheckBox;
class DropDown;
class SyntaxRichEdit;
class Tab;
class ToolBarButtonClickEvent;

}

namespace traktor::render
{

class IProgramCompiler;
class ShaderGraph;

class ShaderViewer : public ui::Container
{
	T_RTTI_CLASS;

public:
	ShaderViewer(editor::IEditor* editor);

	virtual void destroy() override final;

	bool create(ui::Widget* parent);

	void reflect(const ShaderGraph* shaderGraph);

	bool handleCommand(const ui::Command& command);

private:
	struct CombinationInfo
	{
		uint32_t mask;
		uint32_t value;
		std::wstring shaders[4];
	};

	struct TechniqueInfo
	{
		AlignedVector< std::wstring > parameters;
		std::vector< CombinationInfo > combinations;
	};

	editor::IEditor* m_editor;
	Ref< ui::DropDown > m_dropCompiler;
	Ref< ui::DropDown > m_dropTechniques;
	Ref< ui::DropDown > m_dropCombinations;
	Ref< ui::DropDown > m_dropLanguages;
	Ref< ui::CheckBox > m_checkRelaxed;
	Ref< ui::Tab > m_tab;
	Ref< ui::SyntaxRichEdit > m_shaderEditors[3];
	Ref< ShaderGraph > m_pendingShaderGraph;
	Ref< ShaderGraph > m_lastShaderGraph;
	Ref< Job > m_reflectJob;
	std::map< std::wstring, TechniqueInfo > m_techniques;

	// These members are updated by reflection job thus
	// do not access while job is pending.
	std::map< std::wstring, TechniqueInfo > m_reflectedTechniques;

	void updateTechniques();

	void updateCombinations();

	void updateShaders();

	void eventToolBarClick(ui::ToolBarButtonClickEvent* event);

	void eventTimer(ui::TimerEvent* event);

	void jobReflect(Ref< ShaderGraph > shaderGraph, Ref< const IProgramCompiler > compiler);
};

}
