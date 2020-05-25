#include "Render/IRenderTargetSet.h"
#include "Render/ScreenRenderer.h"
#include "Render/Shader.h"
#include "Render/Context/RenderContext.h"
#include "Render/Frame/RenderGraph.h"
#include "Resource/IResourceManager.h"
#include "World/Editor/Overlays/GBufferNormalsOverlay.h"

namespace traktor
{
    namespace world
    {
        namespace
        {

const resource::Id< render::Shader > c_debugShader(Guid(L"{949B3C96-0196-F24E-B36E-98DD504BCE9D}"));
const render::Handle c_handleDebugTechnique(L"Normals");
const render::Handle c_handleDebugAlpha(L"Scene_DebugAlpha");
const render::Handle c_handleDebugTexture(L"Scene_DebugTexture");

        }

T_IMPLEMENT_RTTI_FACTORY_CLASS(L"traktor.world.GBufferNormalsOverlay", 0, GBufferNormalsOverlay, IDebugOverlay)

bool GBufferNormalsOverlay::create(resource::IResourceManager* resourceManager)
{
    if (!resourceManager->bind(c_debugShader, m_shader))
        return false;

    return true;
}

void GBufferNormalsOverlay::setup(render::RenderGraph& renderGraph, render::ScreenRenderer* screenRenderer, IWorldRenderer* worldRenderer, const WorldRenderView& worldRenderView, float alpha) const
{
	render::handle_t gbufferId = renderGraph.findTargetByName(L"GBuffer");
	if (!gbufferId)
		return;

	Ref< render::RenderPass > rp = new render::RenderPass(L"GBuffer normals overlay");
	rp->setOutput(0);
	rp->addInput(gbufferId);
	rp->addBuild([=](const render::RenderGraph& renderGraph, render::RenderContext* renderContext) {
		auto gbufferTargetSet = renderGraph.getTargetSet(gbufferId);
		if (!gbufferTargetSet)
			return;

		const render::Shader::Permutation perm(c_handleDebugTechnique);

		auto pp = renderContext->alloc< render::ProgramParameters >();
		pp->beginParameters(renderContext);
		pp->setFloatParameter(c_handleDebugAlpha, alpha);
		pp->setTextureParameter(c_handleDebugTexture, gbufferTargetSet->getColorTexture(1));
		pp->endParameters(renderContext);

		screenRenderer->draw(renderContext, m_shader, perm, pp);
	});
	renderGraph.addPass(rp);
}

    }
}