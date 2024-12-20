/*
 * TRAKTOR
 * Copyright (c) 2022-2024 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "Core/Misc/String.h"
#include "Core/Settings/PropertyGroup.h"
#include "Core/Settings/PropertyInteger.h"
#include "Core/Settings/PropertyString.h"
#include "I18N/Text.h"
#include "Sound/IAudioDriver.h"
#include "Sound/Editor/SoundSettingsPage.h"
#include "Ui/Container.h"
#include "Ui/Edit.h"
#include "Ui/NumericEditValidator.h"
#include "Ui/Static.h"
#include "Ui/TableLayout.h"
#include "Ui/DropDown.h"

namespace traktor::sound
{

T_IMPLEMENT_RTTI_FACTORY_CLASS(L"traktor.sound.SoundSettingsPage", 0, SoundSettingsPage, editor::ISettingsPage)

bool SoundSettingsPage::create(ui::Container* parent, const PropertyGroup* originalSettings, PropertyGroup* settings, const std::list< ui::Command >& shortcutCommands)
{
	Ref< ui::Container > container = new ui::Container();
	if (!container->create(parent, ui::WsNone, new ui::TableLayout(L"*,100%", L"*", 0_ut, 4_ut)))
		return false;

	Ref< ui::Static > staticSounder = new ui::Static();
	staticSounder->create(container, i18n::Text(L"EDITOR_SETTINGS_SOUND_DRIVER_TYPE"));

	m_dropAudioDriver = new ui::DropDown();
	m_dropAudioDriver->create(container);

	Ref< ui::Static > staticVirtualChannels = new ui::Static();
	staticVirtualChannels->create(container, i18n::Text(L"EDITOR_SETTINGS_SOUND_VIRTUAL_CHANNELS"));

	m_editVirtualChannels = new ui::Edit();
	m_editVirtualChannels->create(container, L"", ui::WsNone, new ui::NumericEditValidator(false, 1, 256));

	Ref< ui::Static > staticSampleRate = new ui::Static();
	staticSampleRate->create(container, i18n::Text(L"EDITOR_SETTINGS_SOUND_SAMPLE_RATE"));

	m_dropSampleRate = new ui::DropDown();
	m_dropSampleRate->create(container);
	m_dropSampleRate->add(L"11025");
	m_dropSampleRate->add(L"22050");
	m_dropSampleRate->add(L"44100");
	m_dropSampleRate->add(L"48000");

	Ref< ui::Static > staticBitsPerSample = new ui::Static();
	staticBitsPerSample->create(container, i18n::Text(L"EDITOR_SETTINGS_SOUND_BITS_PER_SAMPLE"));

	m_dropBitsPerSample = new ui::DropDown();
	m_dropBitsPerSample->create(container);
	m_dropBitsPerSample->add(L"8");
	m_dropBitsPerSample->add(L"16");

	Ref< ui::Static > staticHwChannels = new ui::Static();
	staticHwChannels->create(container, i18n::Text(L"EDITOR_SETTINGS_SOUND_HW_CHANNELS"));

	m_dropHwChannels = new ui::DropDown();
	m_dropHwChannels->create(container);
	m_dropHwChannels->add(L"1");
	m_dropHwChannels->add(L"2");
	m_dropHwChannels->add(L"2.1");
	m_dropHwChannels->add(L"4.0");
	m_dropHwChannels->add(L"4.1");
	m_dropHwChannels->add(L"5.1");
	m_dropHwChannels->add(L"6.1");
	m_dropHwChannels->add(L"7.1");

	Ref< ui::Static > staticFrameSamples = new ui::Static();
	staticFrameSamples->create(container, i18n::Text(L"EDITOR_SETTINGS_SOUND_FRAME_SAMPLES"));

	m_editFrameSamples = new ui::Edit();
	m_editFrameSamples->create(container, L"", ui::WsNone, new ui::NumericEditValidator(false, 16, 4096));

	Ref< ui::Static > staticMixerFrames = new ui::Static();
	staticMixerFrames->create(container, i18n::Text(L"EDITOR_SETTINGS_SOUND_MIXER_FRAMES"));

	m_editMixerFrames = new ui::Edit();
	m_editMixerFrames->create(container, L"", ui::WsNone, new ui::NumericEditValidator(false, 1, 16));

	std::wstring soundDriverType = settings->getProperty< std::wstring >(L"Editor.AudioDriver");
	for (const auto type : type_of< sound::IAudioDriver >().findAllOf(false))
	{
		std::wstring name = type->getName();
		int32_t index = m_dropAudioDriver->add(name);
		if (name == soundDriverType)
			m_dropAudioDriver->select(index);
	}

	m_editVirtualChannels->setText(toString(settings->getProperty< int32_t >(L"Editor.SoundVirtualChannels")));

	int32_t sampleRate = settings->getProperty< int32_t >(L"Editor.SoundSampleRate");
	switch (sampleRate)
	{
	case 11025:
		m_dropSampleRate->select(0);
		break;
	case 22050:
		m_dropSampleRate->select(1);
		break;
	case 44100:
		m_dropSampleRate->select(2);
		break;
	case 48000:
		m_dropSampleRate->select(3);
		break;
	}

	int32_t bitsPerSample = settings->getProperty< int32_t >(L"Editor.SoundBitsPerSample");
	if (bitsPerSample == 8)
		m_dropBitsPerSample->select(0);
	else if (bitsPerSample == 16)
		m_dropBitsPerSample->select(1);

	m_dropHwChannels->select(settings->getProperty< int32_t >(L"Editor.SoundHwChannels") - 1);

	m_editFrameSamples->setText(toString(settings->getProperty< int32_t >(L"Editor.SoundFrameSamples")));
	m_editMixerFrames->setText(toString(settings->getProperty< int32_t >(L"Editor.SoundMixerFrames")));

	parent->setText(i18n::Text(L"EDITOR_SETTINGS_SOUND"));
	return true;
}

void SoundSettingsPage::destroy()
{
}

bool SoundSettingsPage::apply(PropertyGroup* settings)
{
	settings->setProperty< PropertyString >(L"Editor.AudioDriver", m_dropAudioDriver->getSelectedItem());
	settings->setProperty< PropertyInteger >(L"Editor.SoundVirtualChannels", parseString< int32_t >(m_editVirtualChannels->getText()));

	switch (m_dropSampleRate->getSelected())
	{
	case 0:
		settings->setProperty< PropertyInteger >(L"Editor.SoundSampleRate", 11025);
		break;
	case 1:
		settings->setProperty< PropertyInteger >(L"Editor.SoundSampleRate", 22050);
		break;
	case 2:
		settings->setProperty< PropertyInteger >(L"Editor.SoundSampleRate", 44100);
		break;
	case 3:
		settings->setProperty< PropertyInteger >(L"Editor.SoundSampleRate", 48000);
		break;
	}

	switch (m_dropBitsPerSample->getSelected())
	{
	case 0:
		settings->setProperty< PropertyInteger >(L"Editor.SoundBitsPerSample", 8);
		break;
	case 1:
		settings->setProperty< PropertyInteger >(L"Editor.SoundBitsPerSample", 16);
		break;
	}

	settings->setProperty< PropertyInteger >(L"Editor.SoundHwChannels", m_dropHwChannels->getSelected() + 1);

	settings->setProperty< PropertyInteger >(L"Editor.SoundFrameSamples", parseString< int32_t >(m_editFrameSamples->getText()));
	settings->setProperty< PropertyInteger >(L"Editor.SoundMixerFrames", parseString< int32_t >(m_editMixerFrames->getText()));

	return true;
}

}
