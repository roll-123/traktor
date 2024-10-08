/*
 * TRAKTOR
 * Copyright (c) 2022 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "Render/ITexture.h"

// import/export mechanism.
#undef T_DLLCLASS
#if defined(T_RENDER_EXPORT)
#	define T_DLLCLASS T_DLLEXPORT
#else
#	define T_DLLCLASS T_DLLIMPORT
#endif

namespace traktor::render
{

/*! Render target set.
 * \ingroup Render
 *
 * The content valid flag is automatically reset when
 * the content of the render target has been reset
 * on the device.
 */
class T_DLLCLASS IRenderTargetSet : public Object
{
	T_RTTI_CLASS;

public:
	/*! Destroy render targets. */
	virtual void destroy() = 0;

	/*! Get width of render targets. */
	virtual int32_t getWidth() const = 0;

	/*! Get height of render targets. */
	virtual int32_t getHeight() const = 0;

	/*! Get color target texture. */
	virtual ITexture* getColorTexture(int32_t index) const = 0;

	/*! Get depth target texture. */
	virtual ITexture* getDepthTexture() const = 0;

	/*! Read back color target into system memory.
	 *
	 * \note
	 * This is a very slow operation and is only
	 * designed to be for screen shots etc.
	 *
	 * \param index Color target index.
	 * \param buffer System memory buffer; target is copied into
	 *               this buffer so it's up to the caller to ensure
	 *               it large enough.
	 * \return True if successfully copied.
	 */
	virtual bool read(int32_t index, void* buffer) const = 0;

	/*! Set debug name of render target set. */
	virtual void setDebugName(const wchar_t* name) {}
};

}
