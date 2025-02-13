/*
 * TRAKTOR
 * Copyright (c) 2022 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "Core/Io/BitReader.h"
#include "Core/Io/MemoryStream.h"
#include "Core/Test/CaseBitReader.h"

namespace traktor::test
{

T_IMPLEMENT_RTTI_FACTORY_CLASS(L"traktor.test.CaseBitReader", 0, CaseBitReader, Case)

void CaseBitReader::run()
{
	{
		uint8_t d[] = { 0xa5 };

		MemoryStream ms(d, sizeof(d), true, false);
		BitReader br(&ms);

		CASE_ASSERT_EQUAL(br.readUnsigned(4), 0xa);
		CASE_ASSERT_EQUAL(br.readUnsigned(4), 0x5);
	}
	{
		uint8_t d[] = { 0xb6 };

		MemoryStream ms(d, sizeof(d), true, false);
		BitReader br(&ms);

		CASE_ASSERT_EQUAL(br.readUnsigned(3), 5);
		CASE_ASSERT_EQUAL(br.readUnsigned(5), 22);
	}
	{
		uint8_t d[] = { 0xb6, 0x00 };

		MemoryStream ms(d, sizeof(d), true, false);
		BitReader br(&ms);

		CASE_ASSERT_EQUAL(br.readUnsigned(3), 5);
		CASE_ASSERT_EQUAL(br.readUnsigned(6), 44);
	}
	{
		uint8_t d[] = { 0xa6, 0xc0 };

		MemoryStream ms(d, sizeof(d), true, false);
		BitReader br(&ms);

		CASE_ASSERT_EQUAL(br.readUnsigned(10), 667);
	}
}

}
