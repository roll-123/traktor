/*
 * TRAKTOR
 * Copyright (c) 2022-2024 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "Ui/Canvas.h"
#include "Ui/StyleSheet.h"
#include "Ui/Sequencer/Sequence.h"
#include "Ui/Sequencer/SequencerControl.h"
#include "Ui/Sequencer/Track.h"

namespace traktor::ui
{

T_IMPLEMENT_RTTI_CLASS(L"traktor.ui.Track", Track, Key)

Track::Track(int32_t start, int32_t end, bool movable)
:	m_start(start)
,	m_end(end)
,	m_movable(movable)
{
}

void Track::setStart(int32_t start)
{
	m_start = start;
}

int32_t Track::getStart() const
{
	return m_start;
}

void Track::setEnd(int32_t end)
{
	m_end = end;
}

int32_t Track::getEnd() const
{
	return m_end;
}

void Track::move(int32_t offset)
{
	if (m_movable)
	{
		m_start += offset;
		m_end += offset;
	}
}

void Track::getRect(SequencerControl* sequencer, const Sequence* sequence, const Rect& rcClient, Rect& outRect) const
{
	outRect.left = sequence->clientFromTime(m_start);
	outRect.top = rcClient.top;
	outRect.right = sequence->clientFromTime(m_end);
	outRect.bottom = rcClient.bottom - 1;
}

void Track::paint(SequencerControl* sequencer, ui::Canvas& canvas, const Sequence* sequence, const Rect& rcClient, int scrollOffset)
{
	const StyleSheet* ss = sequencer->getStyleSheet();

	int32_t x1 = sequence->clientFromTime(m_start) - scrollOffset;
	int32_t x2 = sequence->clientFromTime(m_end) - scrollOffset;
	Rect rc(rcClient.left + x1, rcClient.top, rcClient.left + x2, rcClient.bottom - 1);
	bool selected = (sequence->getSelectedKey() == this);

	canvas.setBackground(ss->getColor(this, selected ? L"background-color-selected" : L"background-color"));
	canvas.fillRect(rc);

	canvas.setForeground(ss->getColor(this, L"color"));
	canvas.drawRect(rc);
}

}
