/*
 * TRAKTOR
 * Copyright (c) 2022-2024 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "Core/Math/Range.h"
#include "Spray/Source.h"

// import/export mechanism.
#undef T_DLLCLASS
#if defined(T_SPRAY_EXPORT)
#	define T_DLLCLASS T_DLLEXPORT
#else
#	define T_DLLCLASS T_DLLIMPORT
#endif

namespace traktor::spray
{

/*! Point particle source.
 * \ingroup Spray
 */
class T_DLLCLASS VirtualSource : public Source
{
	T_RTTI_CLASS;

public:
	explicit VirtualSource(
		float constantRate,
		float velocityRate,
		const Range< float >& velocity,
		const Range< float >& orientation,
		const Range< float >& angularVelocity,
		const Range< float >& age,
		const Range< float >& mass,
		const Range< float >& size
	);

	virtual void emit(
		Context& context,
		const Transform& transform,
		const Vector4& deltaMotion,
		uint32_t emitCount,
		EmitterInstanceCPU& emitterInstance
	) const override final;

	const Range< float >& getVelocity() const { return m_velocity; }

	const Range< float >& getOrientation() const { return m_orientation; }

	const Range< float >& getAngularVelocity() const { return m_angularVelocity; }

	const Range< float >& getAge() const { return m_age; }

	const Range< float >& getMass() const { return m_mass; }

	const Range< float >& getSize() const { return m_size; }

private:
	Range< float > m_velocity;
	Range< float > m_orientation;
	Range< float > m_angularVelocity;
	Range< float > m_age;
	Range< float > m_mass;
	Range< float > m_size;
};

}
