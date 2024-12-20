/*
 * TRAKTOR
 * Copyright (c) 2022 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "Core/Serialization/ISerializer.h"
#include "Core/Serialization/Member.h"
#include "Drawing/ImageInfo.h"

namespace traktor::drawing
{

T_IMPLEMENT_RTTI_FACTORY_CLASS(L"traktor.drawing.ImageInfo", 0, ImageInfo, ISerializable)

void ImageInfo::setAuthor(const std::wstring& author)
{
	m_author = author;
}

const std::wstring& ImageInfo::getAuthor() const
{
	return m_author;
}

void ImageInfo::setCopyright(const std::wstring& copyright)
{
	m_copyright = copyright;
}

const std::wstring& ImageInfo::getCopyright() const
{
	return m_copyright;
}

void ImageInfo::setFormat(const std::wstring& format)
{
	m_format = format;
}

const std::wstring& ImageInfo::getFormat() const
{
	return m_format;
}

void ImageInfo::setGamma(float gamma)
{
	m_gamma = gamma;
}

float ImageInfo::getGamma() const
{
	return m_gamma;
}

void ImageInfo::serialize(ISerializer& s)
{
	s >> Member< std::wstring >(L"author", m_author);
	s >> Member< std::wstring >(L"copyright", m_copyright);
	s >> Member< std::wstring >(L"format", m_format);
	s >> Member< float >(L"gamma", m_gamma);
}

}
