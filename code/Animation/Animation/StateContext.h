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

// import/export mechanism.
#undef T_DLLCLASS
#if defined(T_ANIMATION_EXPORT)
#	define T_DLLCLASS T_DLLEXPORT
#else
#	define T_DLLCLASS T_DLLIMPORT
#endif

namespace traktor::animation
{

/*! State evaluation context.
 * \ingroup Animation
 */
class T_DLLCLASS StateContext : public Object
{
	T_RTTI_CLASS;

public:
	void setTime(float time) { m_time = time; }

	float getTime() const { return m_time; }

	void setDuration(float duration) { m_duration = duration; }

	float getDuration() const { return m_duration; }

private:
	float m_time = 0.0f;
	float m_duration = 0.0f;
};

}
