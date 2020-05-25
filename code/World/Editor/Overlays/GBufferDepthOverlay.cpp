#include "Render/IRenderTargetSet.h"
#include "Render/ScreenRenderer.h"
#include "Render/Shader.h"
#include "Render/Context/RenderContext.h"
#include "Render/Frame/RenderGraph.h"
#include "Resource/IResourceManager.h"
#include "World/WorldRenderView.h"
#include "World/Editor/Overlays/GBufferDepthOverlay.h"

namespace traktor
{
    namespace world
    {
        namespace
        {

const resource::Id< render::Shader > c_debugShader(Guid(L"{949B3C96-0196-F24E-B36E-98DD504BCE9D}"));
const render::Handle c_handleDebugTechnique(L"ViewDepth");
const render::Handle c_handleDebugAlpha(L"Scene_DebugAlpha");
const render::Handle c_handleDebugTexture(L"Scene_DebugTexture");
const render::Handle c_handleViewDistance(L"Scene_ViewDistance");

        }

T_IMPLEMENT_RTTI_FACTORY_CLASS(L"traktor.world.GBufferDepthOverlay", 0, GBufferDepthOverlay, IDebugOverlay)

bool GBufferDepthOverlay::create(resource::IResourceManager* resourceManager)
{
    if (!resourceManager->bind(c_debugShader, m_shader))
        return false;

    return true;
}

void GBufferDepthOverlay::setup(render::RenderGraph& renderGraph, render::ScreenRenderer* screenRenderer, IWorldRenderer* worldRenderer, const WorldRenderView& worldRenderView, float alpha) const
{
	render::handle_t gbufferId = renderGraph.findTargetByName(L"GBuffer");
	if (!gbufferId)
		return;

	float nearZ = worldRenderView.getViewFrustum().getNearZ();
	float farZ = worldRenderView.getViewFrustum().getFarZ();

	Ref< render::RenderPass > rp = new render::RenderPass(L"GBuffer depth overlay");
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
		pp->setVectorParameter(c_handleViewDistance, Vector4(nearZ, farZ, 0.0f, 0.0f));
		pp->setTextureParameter(c_handleDebugTexture, gbufferTargetSet->getColorTexture(0));
		pp->endParameters(renderContext);

		screenRenderer->draw(renderContext, m_shader, perm, pp);
	});
	renderGraph.addPass(rp);
}

    }
}