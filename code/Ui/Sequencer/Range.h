/*
 * TRAKTOR
 * Copyright (c) 2022-2024 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "Ui/Sequencer/Key.h"

// import/export mechanism.
#undef T_DLLCLASS
#if defined(T_UI_EXPORT)
#	define T_DLLCLASS T_DLLEXPORT
#else
#	define T_DLLCLASS T_DLLIMPORT
#endif

namespace traktor::ui
{

/*! Sequencer range.
 * \ingroup UI
 */
class T_DLLCLASS Range : public Key
{
	T_RTTI_CLASS;

public:
	explicit Range(int32_t start, int32_t end, bool movable);

	void setStart(int32_t start);

	int32_t getStart() const;

	void setEnd(int32_t end);

	int32_t getEnd() const;

	virtual void move(int offset) override;

	virtual void getRect(SequencerControl* sequencer, const Sequence* sequence, const Rect& rcClient, Rect& outRect) const override;

	virtual void paint(SequencerControl* sequencer, ui::Canvas& canvas, const Sequence* sequence, const Rect& rcClient, int scrollOffset) override;

private:
	int32_t m_start;
	int32_t m_end;
	bool m_movable;
};

}
