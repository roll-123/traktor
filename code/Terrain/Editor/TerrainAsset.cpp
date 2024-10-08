/*
 * TRAKTOR
 * Copyright (c) 2022 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "Core/Serialization/ISerializer.h"
#include "Core/Serialization/MemberStl.h"
#include "Heightfield/Heightfield.h"
#include "Render/Shader.h"
#include "Resource/Member.h"
#include "Terrain/Editor/TerrainAsset.h"

namespace traktor::terrain
{

T_IMPLEMENT_RTTI_EDIT_CLASS(L"traktor.terrain.TerrainAsset", 6, TerrainAsset, ISerializable)

void TerrainAsset::serialize(ISerializer& s)
{
	T_ASSERT(s.getVersion() >= 6);

	s >> Member< uint32_t >(L"detailSkip", m_detailSkip);
	s >> Member< uint32_t >(L"patchDim", m_patchDim);
	s >> resource::Member< hf::Heightfield >(L"heightfield", m_heightfield);
	s >> resource::Member< render::Shader >(L"surfaceShader", m_surfaceShader);
}

}
