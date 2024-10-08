/*
 * TRAKTOR
 * Copyright (c) 2022 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "Core/Misc/TString.h"
#include "Ui/Win32/UtilitiesWin32.h"
#include "Ui/Win32/Window.h"

namespace traktor::ui
{
	namespace
	{

const struct { uint32_t keyCode; VirtualKey vkey; } c_translateTable[] =
{
	{ VK_TAB, VkTab },
	{ VK_CLEAR, VkClear },
	{ VK_RETURN, VkReturn },
	{ VK_SHIFT, VkShift },
	{ VK_CONTROL, VkControl },
	{ VK_MENU, VkMenu },
	{ VK_PAUSE, VkPause },
	{ VK_CAPITAL, VkCapital },
	{ VK_ESCAPE, VkEscape },
	{ VK_SPACE, VkSpace },
	{ VK_END, VkEnd },
	{ VK_HOME, VkHome },
	{ VK_PRIOR, VkPageUp },
	{ VK_NEXT, VkPageDown },
	{ VK_LEFT, VkLeft },
	{ VK_UP, VkUp },
	{ VK_RIGHT, VkRight },
	{ VK_DOWN, VkDown },
	{ VK_SELECT, VkSelect },
	{ VK_PRINT, VkPrint },
	{ VK_EXECUTE, VkExecute },
	{ VK_SNAPSHOT, VkSnapshot },
	{ VK_INSERT, VkInsert },
	{ VK_DELETE, VkDelete },
	{ VK_BACK, VkBackSpace },
	{ VK_HELP, VkHelp },
	{ VK_NUMPAD0, VkNumPad0 },
	{ VK_NUMPAD1, VkNumPad1 },
	{ VK_NUMPAD2, VkNumPad2 },
	{ VK_NUMPAD3, VkNumPad3 },
	{ VK_NUMPAD4, VkNumPad4 },
	{ VK_NUMPAD5, VkNumPad5 },
	{ VK_NUMPAD6, VkNumPad6 },
	{ VK_NUMPAD7, VkNumPad7 },
	{ VK_NUMPAD8, VkNumPad8 },
	{ VK_NUMPAD9, VkNumPad9 },
	{ VK_MULTIPLY, VkMultiply },
	{ VK_ADD, VkAdd },
	{ VK_SEPARATOR, VkSeparator },
	{ VK_SUBTRACT, VkSubtract },
	{ VK_DECIMAL, VkDecimal },
	{ VK_DIVIDE, VkDivide },
	{ VK_F1, VkF1 },
	{ VK_F2, VkF2 },
	{ VK_F3, VkF3 },
	{ VK_F4, VkF4 },
	{ VK_F5, VkF5 },
	{ VK_F6, VkF6 },
	{ VK_F7, VkF7 },
	{ VK_F8, VkF8 },
	{ VK_F9, VkF9 },
	{ VK_F10, VkF10 },
	{ VK_F11, VkF11 },
	{ VK_F12, VkF12 },
	{ VK_NUMLOCK, VkNumLock },
	{ VK_SCROLL, VkScroll }
};

	}

VirtualKey translateToVirtualKey(uint32_t keyCode)
{
	for (int32_t i = 0; i < sizeof_array(c_translateTable); ++i)
	{
		if (c_translateTable[i].keyCode == keyCode)
			return c_translateTable[i].vkey;
	}
	if ((keyCode >= '0' && keyCode <= '9') || (keyCode >= 'A' && keyCode <= 'Z'))
		return (VirtualKey)keyCode;
	return VkNull;
}

uint32_t translateToKeyCode(VirtualKey vk)
{
	if (vk >= VkA && vk <= VkZ)
		return (uint32_t)vk;
	if (vk >= Vk0 && vk <= Vk9)
		return (uint32_t)vk;
	for (int32_t i = 0; i < sizeof_array(c_translateTable); ++i)
	{
		if (c_translateTable[i].vkey == vk)
			return c_translateTable[i].keyCode;
	}
	return 0;
}

Font logFontToFont(const LOGFONT& lf)
{
	return Font(
		lf.lfFaceName,
		Unit(abs(lf.lfHeight)),
		(bool)(lf.lfWeight == FW_BOLD),
		(bool)(lf.lfItalic == TRUE),
		(bool)(lf.lfUnderline == TRUE)
	);
}

LOGFONT fontToLogFont(const Font& fnt)
{
	LOGFONT lf = {};
	lf.lfHeight = -fnt.getSize().get();
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = fnt.isBold() ? FW_BOLD : FW_NORMAL;
	lf.lfItalic = fnt.isItalic() ? TRUE : FALSE;
	lf.lfUnderline = fnt.isUnderline() ? TRUE : FALSE;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	_tcscpy_s(lf.lfFaceName, LF_FACESIZE, wstots(fnt.getFace()).c_str());
	return lf;
}

}
