/*
 * TRAKTOR
 * Copyright (c) 2022 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "World/SMProj/UniformShadowProjection.h"

namespace traktor::world
{

T_IMPLEMENT_RTTI_CLASS(L"traktor.world.UniformShadowProjection", UniformShadowProjection, IShadowProjection)

UniformShadowProjection::UniformShadowProjection(uint32_t realShadowMapSize)
:	m_realShadowMapSize(float(realShadowMapSize))
{
}

void UniformShadowProjection::calculate(
	const Matrix44& viewInverse,
	const Vector4& lightPosition,
	const Vector4& lightDirection,
	const Frustum& viewFrustum,
	float shadowFarZ,
	bool quantizeProjection,
	Matrix44& outLightView,
	Matrix44& outLightProjection,
	Frustum& outShadowFrustum
) const
{
	// Calculate light axises.
	Vector4 lightAxisX, lightAxisY, lightAxisZ;

	if (quantizeProjection)
	{
		lightAxisZ = -lightDirection.normalized();
		lightAxisX = cross(lightAxisZ, Vector4(0.0f, 1.0f, 0.0f, 0.0f)).normalized();
		lightAxisY = cross(lightAxisX, lightAxisZ).normalized();
	}
	else
	{
		lightAxisZ = -lightDirection.normalized();
		lightAxisX = cross(viewInverse.axisZ(), lightAxisZ).normalized();
		lightAxisY = cross(lightAxisX, lightAxisZ).normalized();
	}

	const Matrix44 lightView(
		lightAxisX,
		lightAxisY,
		lightAxisZ,
		Vector4::origo()
	);

	const Matrix44 viewToLight = lightView.inverse() * viewInverse;

	// Calculate bounding box of view frustum in light space.
	Aabb3 viewFrustumBox;
	for (int i = 0; i < 8; ++i)
	{
		const Vector4 lightCorner = viewToLight * viewFrustum.corners[i];
		viewFrustumBox.contain(lightCorner);
	}

	// Update light view matrix with bounding box centered.
	const Vector4 center = viewFrustumBox.getCenter();
	const Vector4 extent = viewFrustumBox.getExtent() * Vector4(2.0f, 2.0f, 1.0f, 0.0f);

	const float c_extentStep = 8.0f;

	const Scalar ex = quantizeProjection ? Scalar(std::ceil(extent.x() / c_extentStep) * c_extentStep) : extent.x();
	const Scalar ey = quantizeProjection ? Scalar(std::ceil(extent.y() / c_extentStep) * c_extentStep) : extent.y();

	const Scalar smx = ex / Scalar(m_realShadowMapSize);
	const Scalar smy = ey / Scalar(m_realShadowMapSize);

	const Scalar cx = quantizeProjection ? Scalar(std::floor(center.x() / smx) * smx) : center.x();
	const Scalar cy = quantizeProjection ? Scalar(std::floor(center.y() / smy) * smy) : center.y();

	// Calculate world center of view frustum's bounding box.
	const Vector4 worldCenter = (
		lightAxisX * cx +
		lightAxisY * cy +
		lightAxisZ * center.z()
	).xyz1();

	const Scalar lightDistance = Scalar(shadowFarZ) * 2.0_simd;

	outLightView = Matrix44(
		lightAxisX,
		lightAxisY,
		lightAxisZ,
		worldCenter - lightAxisZ * (lightDistance + extent.z())
	);

	outLightView = outLightView.inverse();

	outLightProjection = orthoLh(
		ex,
		ey,
		0.0f,
		lightDistance + extent.z() * 2.0_simd
	);

	outShadowFrustum.buildOrtho(
		ex,
		ey,
		0.0f,
		lightDistance + extent.z() * 2.0_simd
	);

	// Add part of view frustum to shadow frustum.
	const Matrix44 view2Light = outLightView * viewInverse;
	for (uint32_t i = 0; i < viewFrustum.planes.size(); ++i)
	{
		const Plane viewFrustumPlane = view2Light * viewFrustum.planes[i];
		if (viewFrustumPlane.normal().z() <= 0.0f)
			outShadowFrustum.planes.push_back(viewFrustumPlane);
	}
}

}
