/*
 * TRAKTOR
 * Copyright (c) 2022 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "Core/Object.h"
#include "Core/Ref.h"
#include "Core/Math/Color4f.h"

namespace traktor
{

class Transform;

}

namespace traktor::drawing
{

class Image;

}

namespace traktor::model
{

class Model;

}

namespace traktor::render
{

class SHCoeffs;

}

namespace traktor::shape
{

struct Light;
class BakeConfiguration;
class GBuffer;
class IProbe;

/*! Ray tracer interface.
 * \ingroup Illuminate
 */
class IRayTracer : public Object
{
	T_RTTI_CLASS;

public:
	virtual bool create(const BakeConfiguration* configuration) = 0;

	virtual void destroy() = 0;

	virtual void addEnvironment(const IProbe* environment) = 0;

	virtual void addLight(const Light& light) = 0;

	virtual void addModel(const model::Model* model, const Transform& transform) = 0;

	virtual void commit() = 0;

	virtual Ref< render::SHCoeffs > traceProbe(const Vector4& position, const Vector4& size) const = 0;

	virtual void traceLightmap(const model::Model* model, const GBuffer* gbuffer, drawing::Image* lightmapDiffuse, const int32_t region[4]) const = 0;

	virtual Color4f traceRay(const Vector4& position, const Vector4& direction) const = 0;
};

}
