/*
 * TRAKTOR
 * Copyright (c) 2022 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "Model/IModelOperation.h"

// import/export mechanism.
#undef T_DLLCLASS
#if defined(T_MODEL_EXPORT)
#	define T_DLLCLASS T_DLLEXPORT
#else
#	define T_DLLCLASS T_DLLIMPORT
#endif

namespace traktor
{
	namespace model
	{

/*!
 * \ingroup Model
 */
class T_DLLCLASS Quantize : public IModelOperation
{
	T_RTTI_CLASS;

public:
	explicit Quantize(float step);

	explicit Quantize(const Vector4& step);

protected:
	virtual bool apply(Model& model) const override final;

private:
	Vector4 m_step;
};

	}
}

