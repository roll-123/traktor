#include "Core/Math/Polar.h"

namespace traktor
{

T_MATH_INLINE Polar::Polar(float phi_, float theta_)
:	phi(phi_)
,	theta(theta_)
{
}

T_MATH_INLINE Polar Polar::fromUnitCartesian(const Vector4& unit)
{
	float phi = acosf(unit.y());
	float theta = atan2f(unit.z(), unit.x());
	return Polar(phi, theta >= 0.0f ? theta : theta + TWO_PI);
}

T_MATH_INLINE Vector4 Polar::toUnitCartesian() const
{
	return Vector4(
		sinf(phi) * cosf(theta),
		cosf(phi),
		sinf(phi) * sinf(theta)
	);
}

}