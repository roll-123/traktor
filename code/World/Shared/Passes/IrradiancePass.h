/*
 * TRAKTOR
 * Copyright (c) 2024 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "Core/Object.h"
#include "Render/Types.h"
#include "Resource/Proxy.h"

namespace traktor::render
{

class Buffer;
class ImageGraph;
class IRenderSystem;
class RenderGraph;
class ScreenRenderer;

}

namespace traktor::world
{

struct WorldCreateDesc;

class WorldEntityRenderers;
class WorldRenderView;

/*!
 */
class IrradiancePass : public Object
{
	T_RTTI_CLASS;

public:
	bool create(resource::IResourceManager* resourceManager, render::IRenderSystem* renderSystem, const WorldCreateDesc& desc);

	render::handle_t setup(
		const WorldRenderView& worldRenderView,
		const GatherView& gatheredView,
		const render::Buffer* lightSBuffer,
		bool needJitter,
		uint32_t frameCount,
		render::RenderGraph& renderGraph,
		render::handle_t gbufferTargetSetId,
		render::handle_t outputTargetSetId
	) const;

private:
	Ref< render::ScreenRenderer > m_screenRenderer;
	resource::Proxy< render::ImageGraph > m_irradiance;
};

}