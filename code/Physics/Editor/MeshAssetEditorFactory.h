#pragma once

#include "Editor/IObjectEditorFactory.h"

// import/export mechanism.
#undef T_DLLCLASS
#if defined(T_PHYSICS_EDITOR_EXPORT)
#	define T_DLLCLASS T_DLLEXPORT
#else
#	define T_DLLCLASS T_DLLIMPORT
#endif

namespace traktor
{
	namespace physics
	{

class T_DLLCLASS MeshAssetEditorFactory : public editor::IObjectEditorFactory
{
	T_RTTI_CLASS;

public:
	virtual const TypeInfoSet getEditableTypes() const override final;

	virtual bool needOutputResources(const TypeInfo& typeInfo, std::set< Guid >& outDependencies) const override final;

	virtual Ref< editor::IObjectEditor > createObjectEditor(editor::IEditor* editor) const override final;

	virtual void getCommands(std::list< ui::Command >& outCommands) const override final;

	virtual Ref< ISerializable > cloneAsset(const ISerializable* asset) const override final;
};

	}
}
