/*
 * TRAKTOR
 * Copyright (c) 2024 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "Render/Vrfy/BufferViewVrfy.h"

namespace traktor::render
{

T_IMPLEMENT_RTTI_CLASS(L"traktor.render.BufferViewVrfy", BufferViewVrfy, IBufferView)

BufferViewVrfy::BufferViewVrfy(const BufferVrfy* buffer)
:	m_buffer(buffer)
{
}

}
