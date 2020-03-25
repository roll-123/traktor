#pragma once

#include "Scene/Editor/IComponentEditorFactory.h"

namespace traktor
{
	namespace animation
	{

/*!
 * \ingroup Animation
 */
class PathComponentEditorFactory : public scene::IComponentEditorFactory
{
	T_RTTI_CLASS;

public:
	virtual const TypeInfoSet getComponentDataTypes() const override final;

	virtual Ref< scene::IComponentEditor > createComponentEditor(scene::SceneEditorContext* context, scene::EntityAdapter* entityAdapter, world::IEntityComponentData* componentData) const override final;
};

	}
}
