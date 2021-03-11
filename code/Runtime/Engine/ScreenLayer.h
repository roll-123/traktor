#pragma once

#include <functional>
#include "Resource/Proxy.h"
#include "Runtime/Engine/Layer.h"

// import/export mechanism.
#undef T_DLLCLASS
#if defined(T_RUNTIME_EXPORT)
#	define T_DLLCLASS T_DLLEXPORT
#else
#	define T_DLLCLASS T_DLLIMPORT
#endif

namespace traktor
{
	namespace render
	{

class ProgramParameters;
class ScreenRenderer;
class Shader;

	}

	namespace runtime
	{

/*!
 * \ingroup Runtime
 */
class T_DLLCLASS ScreenLayer : public Layer
{
	T_RTTI_CLASS;

public:
	ScreenLayer(
		Stage* stage,
		const std::wstring& name,
		bool permitTransition,
		IEnvironment* environment,
		const resource::Proxy< render::Shader >& shader
	);

	virtual void destroy() override;

	virtual void transition(Layer* fromLayer) override final;

	virtual void prepare(const UpdateInfo& info) override final;

	virtual void update(const UpdateInfo& info) override final;

	virtual void setup(const UpdateInfo& info, render::RenderGraph& renderGraph) override final;

	virtual void preReconfigured() override final;

	virtual void postReconfigured() override final;

	virtual void suspend() override final;

	virtual void resume() override final;

	void setParameterCallback(const std::function< void(render::ProgramParameters*) >& parameterCallback) { m_parameterCallback = parameterCallback; }

	const std::function< void(render::ProgramParameters*) >& getParameterCallback() const { return m_parameterCallback; }

private:
	resource::Proxy< render::Shader > m_shader;
	Ref< render::ScreenRenderer > m_screenRenderer;
	std::function< void(render::ProgramParameters*) > m_parameterCallback;
};

	}
}