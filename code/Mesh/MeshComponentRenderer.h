/*
 * TRAKTOR
 * Copyright (c) 2022-2024 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "World/IEntityRenderer.h"

// import/export mechanism.
#undef T_DLLCLASS
#if defined(T_MESH_EXPORT)
#	define T_DLLCLASS T_DLLEXPORT
#else
#	define T_DLLCLASS T_DLLIMPORT
#endif

namespace traktor::mesh
{

class MeshComponent;

/*! Mesh component renderer.
 * \ingroup Mesh
 */
class T_DLLCLASS MeshComponentRenderer : public world::IEntityRenderer
{
	T_RTTI_CLASS;

public:
	virtual bool initialize(const ObjectStore& objectStore) override final;

	virtual const TypeInfoSet getRenderableTypes() const override;

	virtual void setup(
		const world::WorldSetupContext& context,
		const world::WorldRenderView& worldRenderView,
		Object* renderable
	) override final;

	virtual void setup(
		const world::WorldSetupContext& context
	) override final;

	virtual void build(
		const world::WorldBuildContext& context,
		const world::WorldRenderView& worldRenderView,
		const world::IWorldRenderPass& worldRenderPass,
		Object* renderable
	) override final;

	virtual void build(
		const world::WorldBuildContext& context,
		const world::WorldRenderView& worldRenderView,
		const world::IWorldRenderPass& worldRenderPass
	) override final;
};

}
