/*
 * TRAKTOR
 * Copyright (c) 2022-2024 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "Core/Guid.h"
#include "Core/Serialization/AttributeRange.h"
#include "Core/Serialization/ISerializer.h"
#include "Core/Serialization/MemberRefArray.h"
#include "Sound/Sound.h"
#include "Sound/Resound/IGrainData.h"
#include "Sound/Editor/SoundCategory.h"
#include "Sound/Editor/Resound/BankAsset.h"

namespace traktor::sound
{

T_IMPLEMENT_RTTI_EDIT_CLASS(L"traktor.sound.BankAsset", 3, BankAsset, ISerializable)

void BankAsset::addGrain(IGrainData* grain)
{
	m_grains.push_back(grain);
}

void BankAsset::removeGrain(IGrainData* grain)
{
	m_grains.remove(grain);
}

const RefArray< IGrainData >& BankAsset::getGrains() const
{
	return m_grains;
}

void BankAsset::serialize(ISerializer& s)
{
	if (s.getVersion() >= 1)
		s >> Member< Guid >(L"category", m_category, AttributeType(type_of< SoundCategory >()));

	if (s.getVersion() >= 2)
		s >> Member< float >(L"presence", m_presence, AttributeRange(0.0f));

	if (s.getVersion() >= 3)
		s >> Member< float >(L"presenceRate", m_presenceRate, AttributeRange(0.0f));

	s >> MemberRefArray< IGrainData >(L"grains", m_grains);
}

}
