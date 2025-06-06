/*
 * TRAKTOR
 * Copyright (c) 2022-2025 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "Core/Containers/AlignedVector.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector4.h"
#include "Render/Frame/RenderGraphTypes.h"

// import/export mechanism.
#undef T_DLLCLASS
#if defined(T_WORLD_EXPORT)
#	define T_DLLCLASS T_DLLEXPORT
#else
#	define T_DLLCLASS T_DLLIMPORT
#endif

namespace traktor::render
{

class IAccelerationStructure;

}

namespace traktor::world
{

class FogComponent;
class IEntityRenderer;
class IrradianceGrid;
class LightComponent;
class ProbeComponent;

static constexpr int32_t MaxSliceCount = 4;
static constexpr int32_t MaxLightShadowCount = 2;
static constexpr int32_t MaxLightsPerCluster = 16;
static constexpr int32_t ClusterDimXY = 16;
static constexpr int32_t ClusterDimZ = 32;

enum class Cancel
{
	Immediate = 0,
	End = 1
};

enum class Quality
{
	Disabled = 0,
	Low = 1,
	Medium = 2,
	High = 3,
	Ultra = 4,
	Last = 5
};

enum class Projection
{
	Orthographic = 0,
	Perspective = 1
};

enum class LightType
{
	Disabled = 0,
	Directional = 1,
	Point = 2,
	Spot = 3
};

/*!
 */
struct T_DLLCLASS EntityState
{
	bool visible = true;
	bool dynamic = false;
	bool locked = false;

	const static EntityState None;
	const static EntityState Visible;
	const static EntityState Dynamic;
	const static EntityState Locked;
	const static EntityState All;
};

/*! Update parameters. */
struct UpdateParams
{
	Object* contextObject = nullptr; /*!< Update context object; is Stage instance during runtime. */
	double totalTime = 0.0;			 /*!< Total time since first update. */
	double alternateTime = 0.0;		 /*!< Alternative absolute time. */
	double deltaTime = 0.0;			 /*!< Delta time since last update. */
};

/*!
 */
struct GatherView
{
	struct Renderable
	{
		IEntityRenderer* renderer = nullptr;
		Object* renderable = nullptr;
		EntityState state;
	};

	AlignedVector< Renderable > renderables;
	AlignedVector< const LightComponent* > lights;
	AlignedVector< const ProbeComponent* > probes;
	const LightComponent* cascadingDirectionalLight = nullptr;
	const FogComponent* fog = nullptr;
	const IrradianceGrid* irradianceGrid = nullptr;
	const render::IAccelerationStructure* rtWorldTopLevel = nullptr;
};

/*!
 */
struct DoubleBufferedTarget
{
	render::RGTargetSet previous;
	render::RGTargetSet current;
};

/*!
 */
struct DoubleBufferedBuffer
{
	render::RGBuffer previous;
	render::RGBuffer current;
};

/*! */
#pragma pack(1)

struct HWRT_Material
{
	float albedo[3];
	float emissive;
	float normal[3];
	int32_t albedoMap;
	float texCoord[2];
	uint8_t __pad__[8]; // 40
};

#pragma pack()

/*! */
Vector2 jitter(int32_t count);

}
