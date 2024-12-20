/*
 * TRAKTOR
 * Copyright (c) 2022-2023 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "Core/Guid.h"
#include "Core/Math/Vector4.h"
#include "Core/Serialization/ISerializable.h"

// import/export mechanism.
#undef T_DLLCLASS
#if defined(T_SHAPE_EDITOR_EXPORT)
#	define T_DLLCLASS T_DLLEXPORT
#else
#	define T_DLLCLASS T_DLLIMPORT
#endif

namespace traktor::shape
{

class T_DLLCLASS BakeConfiguration : public ISerializable
{
	T_RTTI_CLASS;

public:
	bool getEnableLightmaps() const { return m_enableLightmaps; }

	uint32_t getPrimarySampleCount() const { return m_primarySampleCount; }

	uint32_t getSecondarySampleCount() const { return m_secondarySampleCount; }

	uint32_t getShadowSampleCount() const { return m_shadowSampleCount; }

	float getMaxPathDistance() const { return m_maxPathDistance; }

	float getPointLightShadowRadius() const { return m_pointLightShadowRadius; }

	float getLumelDensity() const { return m_lumelDensity; }

	Vector4 getIrradianceGridDensity() const { return m_irradianceGridDensity; }

	int32_t getMinimumLightMapSize() const { return m_minimumLightMapSize; }

	int32_t getMaximumLightMapSize() const { return m_maximumLightMapSize; }

	bool getEnableDenoise() const { return m_enableDenoise; }

	float getAnalyticalLightAttenuation() const { return m_analyticalLightAttenuation; }

	float getAmbientOcclusionFactor() const { return m_ambientOcclusionFactor; }

	uint32_t calculateModelRelevanteHash() const;

	virtual void serialize(ISerializer& s) override final;

private:
	bool m_enableLightmaps = true;
	uint32_t m_primarySampleCount = 100;
	uint32_t m_secondarySampleCount = 50;
	uint32_t m_shadowSampleCount = 50;
	float m_maxPathDistance = 1.0f;
	float m_pointLightShadowRadius = 0.1f;
	float m_lumelDensity = 16.0f;
	Vector4 m_irradianceGridDensity = Vector4(0.1f, 0.1f, 0.1f, 0.0f);
	int32_t m_minimumLightMapSize = 16;
	int32_t m_maximumLightMapSize = 1024;
	bool m_enableDenoise = true;
	float m_analyticalLightAttenuation = 1.0f;
	float m_ambientOcclusionFactor = 0.5f;
};

}
