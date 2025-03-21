/*
 * TRAKTOR
 * Copyright (c) 2022 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "Editor/IEditorTool.h"

namespace traktor::ui
{

class CloseEvent;

}

namespace traktor::script
{

class ScriptDebuggerDialog;

class ScriptDebuggerTool : public editor::IEditorTool
{
	T_RTTI_CLASS;

public:
	virtual std::wstring getDescription() const override final;

	virtual Ref< ui::IBitmap > getIcon() const override final;

	virtual bool needOutputResources(std::set< Guid >& outDependencies) const override final;

	virtual bool launch(ui::Widget* parent, editor::IEditor* editor, const PropertyGroup* param) override final;

private:
	Ref< ScriptDebuggerDialog > m_debuggerDialog;

	void eventCloseDialog(ui::CloseEvent* event);
};

}
