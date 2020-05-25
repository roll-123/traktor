#include "Core/Misc/SafeDestroy.h"
#include "Render/Vrfy/Error.h"
#include "Render/Vrfy/VolumeTextureVrfy.h"

namespace traktor
{
	namespace render
	{

T_IMPLEMENT_RTTI_CLASS(L"traktor.render.VolumeTextureVrfy", VolumeTextureVrfy, IVolumeTexture)

VolumeTextureVrfy::VolumeTextureVrfy(IVolumeTexture* texture)
:	m_texture(texture)
{
}

void VolumeTextureVrfy::destroy()
{
	T_CAPTURE_ASSERT (m_texture, L"Volume texture already destroyed.");
	safeDestroy(m_texture);
}

ITexture* VolumeTextureVrfy::resolve()
{
	T_CAPTURE_ASSERT (m_texture, L"Volume texture destroyed.");
	return this;
}

int32_t VolumeTextureVrfy::getMips() const
{
	T_CAPTURE_ASSERT (m_texture, L"Volume texture destroyed.");
	return m_texture ? m_texture->getMips() : 0;
}

int32_t VolumeTextureVrfy::getWidth() const
{
	T_CAPTURE_ASSERT (m_texture, L"Volume texture destroyed.");
	return m_texture ? m_texture->getWidth() : 0;
}

int32_t VolumeTextureVrfy::getHeight() const
{
	T_CAPTURE_ASSERT (m_texture, L"Volume texture destroyed.");
	return m_texture ? m_texture->getHeight() : 0;
}

int32_t VolumeTextureVrfy::getDepth() const
{
	T_CAPTURE_ASSERT (m_texture, L"Volume texture destroyed.");
	return m_texture ? m_texture->getDepth() : 0;
}

	}
}