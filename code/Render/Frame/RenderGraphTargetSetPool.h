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
#include "Core/Object.h"
#include "Core/RefArray.h"
#include "Render/Frame/RenderGraphTypes.h"

namespace traktor::render
{

class IRenderSystem;
class RenderGraphTargetSet;

/*!
 * \ingroup Render
 */
class RenderGraphTargetSetPool : public Object
{
	T_RTTI_CLASS;

public:
	explicit RenderGraphTargetSetPool(IRenderSystem* renderSystem);

	void destroy();

	/*! Acquire target from pool.
	 *
	 * \param name Name of target, useful for debugging etc.
	 * \param targetSetDesc Description of target required.
	 * \param sharedDepthStencilTargetSet Optional shared depth/stencil target set.
	 * \param sharedPrimaryDepthStencilTargetSet Optional shared depth/stencil with primary target set.
	 * \param referenceWidth Reference width of target required.
	 * \param referenceHeight Reference height of target required.
	 * \param persistentHandle Persistent handle; used to track persistent targets in pool, 0 means not persistent target.
	 */
	RenderGraphTargetSet* acquire(
		const wchar_t* name,
		const RenderGraphTargetSetDesc& targetSetDesc,
		IRenderTargetSet* sharedDepthStencilTargetSet,
		bool sharedPrimaryDepthStencilTargetSet,
		int32_t referenceWidth,
		int32_t referenceHeight,
		uint32_t multiSample,
		bool doubleBuffered,
		uint32_t persistentHandle);

	/*! */
	void release(Ref< RenderGraphTargetSet >& targetSet);

	/*! */
	void cleanup();

private:
	struct Target
	{
		Ref< RenderGraphTargetSet > rts;
		int32_t unused;
	};

	struct Pool
	{
		// Pool identification.
		RenderTargetSetCreateDesc rtscd;
		Ref< IRenderTargetSet > sharedDepthStencilTargetSet;
		uint32_t persistentHandle;

		// Pool targets.
		AlignedVector< Target > free;
		RefArray< RenderGraphTargetSet > acquired;
	};

	Ref< IRenderSystem > m_renderSystem;
	AlignedVector< Pool > m_pool;
};

}
