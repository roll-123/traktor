#pragma once

#include <vector>
#include "Core/Ref.h"
#include "Resource/Id.h"
#include "World/IEntityComponentData.h"

// import/export mechanism.
#undef T_DLLCLASS
#if defined(T_ANIMATION_EXPORT)
#	define T_DLLCLASS T_DLLEXPORT
#else
#	define T_DLLCLASS T_DLLIMPORT
#endif

namespace traktor
{
	namespace render
	{

class IRenderSystem;
class Shader;

	}

	namespace resource
	{

class IResourceManager;

	}

	namespace animation
	{

class ClothComponent;

/*! \brief
 * \ingroup Animation
 */
class T_DLLCLASS ClothComponentData : public world::IEntityComponentData
{
	T_RTTI_CLASS;

public:
	struct Anchor
	{
		uint32_t x;
		uint32_t y;

		void serialize(ISerializer& s);
	};

	ClothComponentData();

	Ref< ClothComponent > createComponent(
		resource::IResourceManager* resourceManager,
		render::IRenderSystem* renderSystem
	) const;

	virtual void serialize(ISerializer& s) override final;

	const resource::Id< render::Shader >& getShader() const { return m_shader; }

private:
	resource::Id< render::Shader > m_shader;
	uint32_t m_resolutionX;
	uint32_t m_resolutionY;
	float m_scale;
	std::vector< Anchor > m_anchors;
	uint32_t m_solverIterations;
	float m_damping;
};

	}
}
