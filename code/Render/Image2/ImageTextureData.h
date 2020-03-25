#pragma once

#include <string>
#include "Core/Serialization/ISerializable.h"
#include "Resource/Id.h"

// import/export mechanism.
#undef T_DLLCLASS
#if defined(T_RENDER_EXPORT)
#	define T_DLLCLASS T_DLLEXPORT
#else
#	define T_DLLCLASS T_DLLIMPORT
#endif

namespace traktor
{
	namespace resource
	{

class IResourceManager;

	}

	namespace render
	{

class ImageTexture;
class ITexture;

/*!
 * \ingroup Render
 */
class T_DLLCLASS ImageTextureData : public ISerializable
{
	T_RTTI_CLASS;

public:
	Ref< const ImageTexture > createInstance(resource::IResourceManager* resourceManager) const;

	virtual void serialize(ISerializer& s) override final;

private:
	friend class ImageGraphPipeline;

	std::wstring m_textureId;
    resource::Id< ITexture > m_texture;
};

	}
}