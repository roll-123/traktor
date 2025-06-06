/*
 * TRAKTOR
 * Copyright (c) 2022-2025 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "Render/Image2/DirectionalBlurData.h"

#include "Core/Math/Const.h"
#include "Core/Math/Random.h"
#include "Core/Serialization/AttributeRange.h"
#include "Core/Serialization/ISerializer.h"
#include "Core/Serialization/Member.h"
#include "Core/Serialization/MemberEnum.h"
#include "Render/Image2/DirectionalBlur.h"
#include "Render/Shader.h"
#include "Resource/IResourceManager.h"

namespace traktor::render
{
namespace
{

Random s_random;

}

T_IMPLEMENT_RTTI_FACTORY_CLASS(L"traktor.render.DirectionalBlurData", 0, DirectionalBlurData, ImagePassStepData)

DirectionalBlurData::DirectionalBlurData()
	: m_blurType(BlurType::Gaussian)
	, m_direction(1.0f, 0.0f)
	, m_taps(15)
{
}

Ref< const ImagePassStep > DirectionalBlurData::createInstance(resource::IResourceManager* resourceManager, IRenderSystem* /*renderSystem*/) const
{
	Ref< DirectionalBlur > instance = new DirectionalBlur();

	// Bind shader.
	if (!resourceManager->bind(m_shader, instance->m_shader))
		return nullptr;

	// Get handles of sources.
	setSourceHandles(instance);

	float totalWeight = 0.0f;

	// Calculate kernel offset and weights.
	instance->m_gaussianOffsetWeights.resize(m_taps);
	if (m_blurType == BlurType::Gaussian)
	{
		float sigma = m_taps / 4.73f;

		// Iterate to prevent under or over sigma.
		for (int32_t i = 0; i < 10; ++i)
		{
			const float x = m_taps / 2.0f;
			const float a = 1.0f / sqrtf(TWO_PI * sigma * sigma);
			const float weight = a * std::exp(-((x * x) / (2.0f * sigma * sigma)));
			if (weight > 0.01f)
				sigma -= 0.1f;
			else if (weight < 0.001f)
				sigma += 0.01f;
			else
				break;
		}

		const float a = 1.0f / sqrtf(TWO_PI * sigma * sigma);
		for (int32_t i = 0; i < m_taps; ++i)
		{
			const float x = i - m_taps / 2.0f;
			const float weight = a * std::exp(-((x * x) / (2.0f * sigma * sigma)));
			totalWeight += weight;

			instance->m_gaussianOffsetWeights[i].set(
				i - (m_taps - 1.0f) / 2.0f,
				weight,
				0.0f,
				0.0f);
		}
	}
	else if (m_blurType == BlurType::Sine)
	{
		const float step = 1.0f / (m_taps - 1.0f);
		const float angleMin = (PI * step) / 2.0f;
		const float angleMax = PI - angleMin;

		for (int i = 0; i < m_taps; ++i)
		{
			const float phi = (float(i) * step) * (angleMax - angleMin) + angleMin;
			const float weight = sinf(phi);
			totalWeight += weight;

			instance->m_gaussianOffsetWeights[i].set(
				i - (m_taps - 1.0f) / 2.0f,
				weight,
				0.0f,
				0.0f);
		}
	}
	else if (m_blurType == BlurType::Box)
	{
		for (int32_t i = 0; i < m_taps; ++i)
			instance->m_gaussianOffsetWeights[i].set(
				i - (m_taps - 1.0f) / 2.0f,
				1.0f,
				0.0f,
				0.0f);
		totalWeight = float(m_taps);
	}
	else if (m_blurType == BlurType::Box2D)
	{
		for (int32_t i = 0; i < m_taps; ++i)
		{
			const float x = s_random.nextFloat() * 2.0f - 1.0f;
			const float y = s_random.nextFloat() * 2.0f - 1.0f;
			instance->m_gaussianOffsetWeights[i].set(
				x,
				y,
				0.0f,
				0.0f);
		}
	}
	else if (m_blurType == BlurType::Circle2D)
	{
		for (int32_t i = 0; i < m_taps;)
		{
			const float x = s_random.nextFloat() * 2.0f - 1.0f;
			const float y = s_random.nextFloat() * 2.0f - 1.0f;
			if (std::sqrt(x * x + y * y) <= 1.0f)
			{
				instance->m_gaussianOffsetWeights[i].set(
					x,
					y,
					0.0f,
					0.0f);
				++i;
			}
		}
	}

	if (totalWeight > FUZZY_EPSILON)
	{
		const Vector4 invWeight(1.0f, 1.0f / totalWeight, 1.0f, 1.0f);
		for (int32_t i = 0; i < m_taps; ++i)
			instance->m_gaussianOffsetWeights[i] *= invWeight;
	}

	instance->m_direction = Vector4(
		m_direction.x,
		m_direction.y,
		0.0f,
		0.0f);

	return instance;
}

void DirectionalBlurData::serialize(ISerializer& s)
{
	ImagePassStepData::serialize(s);

	s >> MemberEnumByValue< BlurType >(L"blurType", m_blurType);
	s >> Member< Vector2 >(L"direction", m_direction);
	s >> Member< int32_t >(L"taps", m_taps, AttributeRange(0, 32));
}

}
