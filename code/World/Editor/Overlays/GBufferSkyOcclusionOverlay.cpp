/*
 * TRAKTOR
 * Copyright (c) 2022 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "Render/IRenderTargetSet.h"
#include "Render/ScreenRenderer.h"
#include "Render/Shader.h"
#include "Render/Context/RenderContext.h"
#include "Render/Frame/RenderGraph.h"
#include "Resource/IResourceManager.h"
#include "World/Editor/Overlays/GBufferSkyOcclusionOverlay.h"

namespace traktor::world
{
	namespace
	{

const resource::Id< render::Shader > c_debugShader(Guid(L"{949B3C96-0196-F24E-B36E-98DD504BCE9D}"));
const render::Handle c_handleDebugTechnique(L"Occlusion");
const render::Handle c_handleDebugAlpha(L"Scene_DebugAlpha");
const render::Handle c_handleDebugTexture(L"Scene_DebugTexture");

render::handle_t findTargetByName(const render::RenderGraph& renderGraph, const wchar_t* name)
{
	for (const auto& tm : renderGraph.getTargets())
	{
		if (wcscmp(tm.second.name, name) == 0)
			return tm.first;
	}
	return 0;
}

	}

T_IMPLEMENT_RTTI_FACTORY_CLASS(L"traktor.world.GBufferSkyOcclusionOverlay", 0, GBufferSkyOcclusionOverlay, IDebugOverlay)

bool GBufferSkyOcclusionOverlay::create(resource::IResourceManager* resourceManager)
{
	if (!resourceManager->bind(c_debugShader, m_shader))
		return false;

	return true;
}

void GBufferSkyOcclusionOverlay::setup(render::RenderGraph& renderGraph, render::ScreenRenderer* screenRenderer, IWorldRenderer* worldRenderer, const WorldRenderView& worldRenderView, float alpha) const
{
	render::handle_t gbufferId = findTargetByName(renderGraph, L"GBuffer");
	if (!gbufferId)
		return;

	Ref< render::RenderPass > rp = new render::RenderPass(L"GBuffer sky occlusion overlay");
	rp->setOutput(0, render::TfColor, render::TfColor);
	rp->addInput(gbufferId);
	rp->addBuild([=](const render::RenderGraph& renderGraph, render::RenderContext* renderContext) {
		auto gbufferTargetSet = renderGraph.getTargetSet(gbufferId);
		if (!gbufferTargetSet || gbufferTargetSet->getColorTexture(3) == nullptr)
			return;

		const render::Shader::Permutation perm(c_handleDebugTechnique);

		auto pp = renderContext->alloc< render::ProgramParameters >();
		pp->beginParameters(renderContext);
		pp->setFloatParameter(c_handleDebugAlpha, alpha);
		pp->setTextureParameter(c_handleDebugTexture, gbufferTargetSet->getColorTexture(3));
		pp->endParameters(renderContext);

		screenRenderer->draw(renderContext, m_shader, perm, pp);
	});
	renderGraph.addPass(rp);
}

}