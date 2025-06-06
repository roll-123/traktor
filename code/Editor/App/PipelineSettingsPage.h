/*
 * TRAKTOR
 * Copyright (c) 2022 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "Core/Ref.h"
#include "Editor/ISettingsPage.h"

namespace traktor
{
	namespace ui
	{

class ButtonClickEvent;
class CheckBox;
class Edit;

	}

	namespace editor
	{

/*! Pipeline settings pages.
 * \ingroup Editor
 */
class PipelineSettingsPage : public ISettingsPage
{
	T_RTTI_CLASS;

public:
	virtual bool create(ui::Container* parent, const PropertyGroup* originalSettings, PropertyGroup* settings, const std::list< ui::Command >& shortcutCommands) override final;

	virtual void destroy() override final;

	virtual bool apply(PropertyGroup* settings) override final;

private:
	Ref< ui::CheckBox > m_checkVerbose;
	Ref< ui::CheckBox > m_checkDependsThreads;
	Ref< ui::CheckBox > m_checkUseAvalanche;
	Ref< ui::Edit > m_editAvalancheHost;
	Ref< ui::Edit > m_editAvalanchePort;
	Ref< ui::CheckBox > m_checkAvalancheRead;
	Ref< ui::CheckBox > m_checkAvalancheWrite;
	Ref< ui::CheckBox > m_checkUseFileCache;
	Ref< ui::Edit > m_editFileCachePath;
	Ref< ui::CheckBox > m_checkFileCacheRead;
	Ref< ui::CheckBox > m_checkFileCacheWrite;
	Ref< ui::Edit > m_editInstanceCachePath;
	Ref< ui::Edit > m_editModelCachePath;
	Ref< ui::Edit > m_editAssetPath;

	void eventUseCacheClick(ui::ButtonClickEvent* event);
};

	}
}

