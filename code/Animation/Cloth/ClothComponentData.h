/*
 * TRAKTOR
 * Copyright (c) 2022-2024 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "Core/Ref.h"
#include "Core/Containers/AlignedVector.h"
#include "Core/Math/Vector4.h"
#include "Resource/Id.h"
#include "World/IEntityComponentData.h"

// import/export mechanism.
#undef T_DLLCLASS
#if defined(T_ANIMATION_EXPORT)
#	define T_DLLCLASS T_DLLEXPORT
#else
#	define T_DLLCLASS T_DLLIMPORT
#endif

namespace traktor::render
{

class IRenderSystem;
class Shader;

}

namespace traktor::resource
{

class IResourceManager;

}

namespace traktor::animation
{

class Cloth;
class ClothComponent;

/*!
 * \ingroup Animation
 */
class T_DLLCLASS ClothComponentData : public world::IEntityComponentData
{
	T_RTTI_CLASS;

public:
	struct Anchor
	{
		std::wstring jointName;
		Vector4 jointOffset = Vector4::zero();
		uint32_t x = 0;
		uint32_t y = 0;

		void serialize(ISerializer& s);
	};

	Ref< ClothComponent > createComponent(
		resource::IResourceManager* resourceManager,
		render::IRenderSystem* renderSystem
	) const;

	virtual int32_t getOrdinal() const override final;

	virtual void setTransform(const world::EntityData* owner, const Transform& transform) override final;

	virtual void serialize(ISerializer& s) override final;

	const resource::Id< Cloth >& getCloth() const { return m_cloth; }

	const resource::Id< render::Shader >& getShader() const { return m_shader; }

private:
	resource::Id< Cloth > m_cloth;
	resource::Id< render::Shader > m_shader;
	float m_jointRadius = 0.1f;
	AlignedVector< Anchor > m_anchors;
	uint32_t m_solverIterations = 4;
	float m_damping = 0.01f;
};

}
