#pragma once

#include "Core/Ref.h"
#include "Resource/Id.h"
#include "Shape/Editor/Spline/SplineLayerComponentData.h"

// import/export mechanism.
#undef T_DLLCLASS
#if defined(T_SHAPE_EDITOR_EXPORT)
#	define T_DLLCLASS T_DLLEXPORT
#else
#	define T_DLLCLASS T_DLLIMPORT
#endif

namespace traktor
{
	namespace render
	{

class Shader;

	}

	namespace shape
	{

/*!
 * \ingroup Shape
 */
class T_DLLCLASS ExtrudeShapeLayerData : public SplineLayerComponentData
{
	T_RTTI_CLASS;

public:
	ExtrudeShapeLayerData();

	virtual Ref< SplineLayerComponent > createComponent(const world::IEntityBuilder* builder, resource::IResourceManager* resourceManager, render::IRenderSystem* renderSystem) const override final;

	virtual void serialize(ISerializer& s) override final;

	const resource::Id< render::Shader >& getShader() const { return m_shader; }

private:
	resource::Id< render::Shader > m_shader;
	bool m_automaticOrientation;
	float m_detail;
};

	}
}