/*
 * TRAKTOR
 * Copyright (c) 2022-2024 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "Core/Serialization/AttributeDirection.h"
#include "Core/Serialization/AttributePoint.h"
#include "Core/Serialization/ISerializer.h"
#include "Core/Serialization/Member.h"
#include "Core/Serialization/MemberComposite.h"
#include "Spray/Sources/QuadSource.h"
#include "Spray/Sources/QuadSourceData.h"

namespace traktor::spray
{
	namespace
	{

const resource::Id< render::Shader > c_shaderQuadSource(L"{97043C88-124A-D14F-8B42-CF975518E081}");

	}

T_IMPLEMENT_RTTI_FACTORY_CLASS(L"traktor.spray.QuadSourceData", 0, QuadSourceData, SourceData)

QuadSourceData::QuadSourceData()
:	SourceData()
,	m_center(0.0f, 0.0f, 0.0f, 1.0f)
,	m_axis1(1.0f, 0.0f, 0.0f, 0.0f)
,	m_axis2(0.0f, 0.0f, 1.0f, 0.0f)
,	m_normal(0.0f, 1.0f, 0.0f, 0.0f)
,	m_velocity(0.0f, 0.0f)
,	m_orientation(0.0f, 2.0f * PI)
,	m_angularVelocity(0.0f, 0.0f)
,	m_age(1.0f, 1.0f)
,	m_mass(1.0f, 1.0f)
,	m_size(1.0f, 1.0f)
{
}

resource::Id< render::Shader > QuadSourceData::getShader() const
{
	return c_shaderQuadSource;
}

Ref< const Source > QuadSourceData::createSource(resource::IResourceManager* resourceManager) const
{
	return new QuadSource(
		getConstantRate(),
		getVelocityRate(),
		m_center,
		m_axis1,
		m_axis2,
		m_normal,
		m_velocity,
		m_orientation,
		m_angularVelocity,
		m_age,
		m_mass,
		m_size
	);
}

void QuadSourceData::serialize(ISerializer& s)
{
	SourceData::serialize(s);

	s >> Member< Vector4 >(L"center", m_center, AttributePoint());
	s >> Member< Vector4 >(L"axis1", m_axis1, AttributeDirection());
	s >> Member< Vector4 >(L"axis2", m_axis2, AttributeDirection());
	s >> Member< Vector4 >(L"normal", m_normal, AttributeDirection());
	s >> MemberComposite< Range< float > >(L"velocity", m_velocity);
	s >> MemberComposite< Range< float > >(L"orientation", m_orientation);
	s >> MemberComposite< Range< float > >(L"angularVelocity", m_angularVelocity);
	s >> MemberComposite< Range< float > >(L"age", m_age);
	s >> MemberComposite< Range< float > >(L"mass", m_mass);
	s >> MemberComposite< Range< float > >(L"size", m_size);
}

}
