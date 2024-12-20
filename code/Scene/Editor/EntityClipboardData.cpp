/*
 * TRAKTOR
 * Copyright (c) 2022 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "Core/Serialization/ISerializer.h"
#include "Core/Serialization/MemberRefArray.h"
#include "Scene/Editor/EntityClipboardData.h"
#include "World/EntityData.h"

namespace traktor::scene
{

T_IMPLEMENT_RTTI_FACTORY_CLASS(L"traktor.scene.EntityClipboardData", 0, EntityClipboardData, ISerializable)

void EntityClipboardData::addEntityData(world::EntityData* entityData)
{
	m_entityData.push_back(entityData);
}

const RefArray< world::EntityData >& EntityClipboardData::getEntityData() const
{
	return m_entityData;
}

void EntityClipboardData::serialize(ISerializer& s)
{
	s >> MemberRefArray< world::EntityData >(L"entityData", m_entityData);
}

}
