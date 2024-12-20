/*
 * TRAKTOR
 * Copyright (c) 2022-2024 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "Shape/Editor/EntityRenderer.h"
#include "Shape/Editor/Solid/SolidComponent.h"
#include "Shape/Editor/Spline/SplineComponent.h"

namespace traktor::shape
{

T_IMPLEMENT_RTTI_FACTORY_CLASS(L"traktor.shape.EntityRenderer", 0, EntityRenderer, world::IEntityRenderer)

bool EntityRenderer::initialize(const ObjectStore& objectStore)
{
	return true;
}

const TypeInfoSet EntityRenderer::getRenderableTypes() const
{
	return makeTypeInfoSet<
		SolidComponent,
		SplineComponent
	>();
}

void EntityRenderer::setup(
	const world::WorldSetupContext& context,
	const world::WorldRenderView& worldRenderView,
	Object* renderable
)
{
}

void EntityRenderer::setup(
	const world::WorldSetupContext& context
)
{
}

void EntityRenderer::build(
	const world::WorldBuildContext& context,
	const world::WorldRenderView& worldRenderView,
	const world::IWorldRenderPass& worldRenderPass,
	Object* renderable
)
{
	if (auto solidComponent = dynamic_type_cast< SolidComponent* >(renderable))
	{
		solidComponent->build(
			context,
			worldRenderView,
			worldRenderPass
		);
	}
	else if (auto splineComponent = dynamic_type_cast< SplineComponent* >(renderable))
	{
		splineComponent->build(
			context,
			worldRenderView,
			worldRenderPass
		);
	}
}

void EntityRenderer::build(
	const world::WorldBuildContext& context,
	const world::WorldRenderView& worldRenderView,
	const world::IWorldRenderPass& worldRenderPass
)
{
}

}
