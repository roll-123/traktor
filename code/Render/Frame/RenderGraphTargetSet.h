/*
 * TRAKTOR
 * Copyright (c) 2025 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "Core/Object.h"
#include "Core/Ref.h"

namespace traktor::render
{

class IRenderTargetSet;

class RenderGraphTargetSet : public Object
{
public:
	explicit RenderGraphTargetSet(IRenderTargetSet* readTargetSet, IRenderTargetSet* writeTargetSet);

	void destroy();

	void swap();

	IRenderTargetSet* getReadTargetSet() const { return m_readTargetSet; }

	IRenderTargetSet* getWriteTargetSet() const { return m_writeTargetSet; }

private:
	Ref< IRenderTargetSet > m_readTargetSet;
	Ref< IRenderTargetSet > m_writeTargetSet;
};

}
