/*
 * TRAKTOR
 * Copyright (c) 2022 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "Scene/Editor/Events/PostFrameEvent.h"

namespace traktor::scene
{

T_IMPLEMENT_RTTI_CLASS(L"traktor.scene.PostFrameEvent", PostFrameEvent, ui::Event)

PostFrameEvent::PostFrameEvent(ui::EventSubject* sender)
:	ui::Event(sender)
{
}

}
