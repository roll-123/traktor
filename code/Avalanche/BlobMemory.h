/*
 * TRAKTOR
 * Copyright (c) 2022 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "Avalanche/IBlob.h"

// import/export mechanism.
#undef T_DLLCLASS
#if defined(T_AVALANCHE_EXPORT)
#	define T_DLLCLASS T_DLLEXPORT
#else
#	define T_DLLCLASS T_DLLIMPORT
#endif

namespace traktor
{

class ChunkMemory;

}

namespace traktor::avalanche
{

class T_DLLCLASS BlobMemory : public IBlob
{
	T_RTTI_CLASS;

public:
	BlobMemory();

	virtual int64_t size() const override final;

	virtual Ref< IStream > append() override final;

	virtual Ref< IStream > read() const override final;

	virtual bool remove() override final;

	virtual bool touch() override final;

	virtual DateTime lastAccessed() const override final;

private:
	Ref< ChunkMemory > m_memory;
	mutable DateTime m_lastAccessed;
};

}
