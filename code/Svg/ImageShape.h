/*
 * TRAKTOR
 * Copyright (c) 2022 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "Svg/Shape.h"

// import/export mechanism.
#undef T_DLLCLASS
#if defined(T_SVG_EXPORT)
#	define T_DLLCLASS T_DLLEXPORT
#else
#	define T_DLLCLASS T_DLLIMPORT
#endif

namespace traktor::drawing
{

class Image;

}

namespace traktor::svg
{

/*! SVG image shape.
 * \ingroup SVG
 */
class T_DLLCLASS ImageShape : public Shape
{
	T_RTTI_CLASS;

public:
	explicit ImageShape(const Vector2& position, const Vector2& size, const drawing::Image* image);

	const Vector2& getPosition() const;

	const Vector2& getSize() const;

	const drawing::Image* getImage() const;

	virtual Aabb2 getBoundingBox() const override;

private:
	Vector2 m_position;
	Vector2 m_size;
	Ref< const drawing::Image > m_image;
};

}
