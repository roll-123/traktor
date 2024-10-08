/*
 * TRAKTOR
 * Copyright (c) 2022-2023 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "Core/Ref.h"
#include "Shape/Editor/Bake/IProbe.h"

namespace traktor::drawing
{

class IImageFilter;
class Image;

}

namespace traktor::shape
{
	
class IblProbe : public IProbe
{
	T_RTTI_CLASS;

public:
	IblProbe() = default;

	explicit IblProbe(const drawing::Image* radiance);

	virtual Color4f sampleRadiance(const Vector4& direction) const override final;

	const drawing::Image* getRadianceImage() const { return m_radiance; }

	virtual void serialize(ISerializer& s) override final;

private:
	Ref< const drawing::Image > m_radiance;
};
	
}
