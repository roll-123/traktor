/*
 * TRAKTOR
 * Copyright (c) 2024-2025 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "World/Editor/Overlays/HiZOverlay.h"

#include "Render/Context/RenderContext.h"
#include "Render/Frame/RenderGraph.h"
#include "Render/IRenderTargetSet.h"
#include "Render/ScreenRenderer.h"
#include "Render/Shader.h"
#include "Resource/IResourceManager.h"

namespace traktor::world
{
namespace
{

const resource::Id< render::Shader > c_debugShader(Guid(L"{949B3C96-0196-F24E-B36E-98DD504BCE9D}"));
const render::Handle c_handleDebugTechnique(L"HiZ");
const render::Handle c_handleDebugAlpha(L"Scene_DebugAlpha");
const render::Handle c_handleDebugMip(L"Scene_DebugMip");
const render::Handle c_handleDebugTexture(L"Scene_DebugTexture");

render::RGTexture findTextureByName(const render::RenderGraph& renderGraph, const wchar_t* name)
{
	for (const auto& tm : renderGraph.getTextures())
		if (wcscmp(tm.second.name, name) == 0)
			return tm.first;
	return render::RGTexture::Invalid;
}

}

T_IMPLEMENT_RTTI_FACTORY_CLASS(L"traktor.world.HiZOverlay", 0, HiZOverlay, BaseOverlay)

bool HiZOverlay::create(resource::IResourceManager* resourceManager)
{
	if (!BaseOverlay::create(resourceManager))
		return false;

	if (!resourceManager->bind(c_debugShader, m_shader))
		return false;

	return true;
}

void HiZOverlay::setup(render::RenderGraph& renderGraph, render::ScreenRenderer* screenRenderer, World* world, IWorldRenderer* worldRenderer, const WorldRenderView& worldRenderView, float alpha, float mip) const
{
	const render::RGTexture hiZId = findTextureByName(renderGraph, L"HiZ");
	if (hiZId == render::RGTexture::Invalid)
	{
		BaseOverlay::setup(renderGraph, screenRenderer, world, worldRenderer, worldRenderView, alpha, mip);
		return;
	}

	Ref< render::RenderPass > rp = new render::RenderPass(L"Hi-Z overlay");
	rp->setOutput(render::RGTargetSet::Output, render::TfColor, render::TfColor);
	rp->addInput(hiZId);
	rp->addBuild([=, this](const render::RenderGraph& renderGraph, render::RenderContext* renderContext) {
		auto hiZ = renderGraph.getTexture(hiZId);
		if (!hiZ)
			return;

		const render::Shader::Permutation perm(c_handleDebugTechnique);

		auto pp = renderContext->alloc< render::ProgramParameters >();
		pp->beginParameters(renderContext);
		pp->setFloatParameter(c_handleDebugAlpha, alpha);
		pp->setFloatParameter(c_handleDebugMip, mip);
		pp->setTextureParameter(c_handleDebugTexture, hiZ);
		pp->endParameters(renderContext);

		screenRenderer->draw(renderContext, m_shader, perm, pp);
	});
	renderGraph.addPass(rp);
}

}
