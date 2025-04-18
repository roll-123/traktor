/*
 * TRAKTOR
 * Copyright (c) 2022 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <algorithm>
#include "Core/Io/BitReader.h"
#include "Core/Io/IStream.h"

namespace traktor
{

T_IMPLEMENT_RTTI_CLASS(L"traktor.BitReader", BitReader, Object)

BitReader::BitReader(IStream* stream)
:	m_stream(stream)
,	m_data(0)
,	m_cnt(0)
,	m_eos(false)
{
}

bool BitReader::readBit()
{
	if (m_cnt <= 0)
	{
		if (m_stream->read((void*)&m_data, 1) != 1)
		{
			m_eos = true;
			return false;
		}
		m_cnt = 8;
	}

	uint8_t bit = 1 << --m_cnt;
	return bool((m_data & bit) != 0);
}

uint32_t BitReader::readUnsigned(int nbits)
{
	uint32_t o = 0, b = 1 << (nbits - 1);
	for (int i = 0; i < nbits; ++i)
	{
		if (m_cnt <= 0)
		{
			if (m_stream->read((void*)&m_data, 1) != 1)
			{
				m_eos = true;
				return 0;
			}
			m_cnt = 8;
		}

		uint8_t bit = 1 << --m_cnt;
		if ((m_data & bit) != 0)
			o |= b;

		b >>= 1;
	}
	return o;
}

int32_t BitReader::readSigned(int nbits)
{
	uint32_t u = readUnsigned(nbits);
	if (eos())
		return 0;

	uint32_t msb = 1 << (nbits - 1);
	if (u & msb)
		u |= ~(msb - 1);

	return *(int32_t*)(&u);
}

int8_t BitReader::readInt8()
{
	alignByte();

	int8_t u;
	if (m_stream->read(&u, sizeof(u)) != sizeof(u))
	{
		m_eos = true;
		return 0;
	}

	return u;
}

uint8_t BitReader::readUInt8()
{
	alignByte();

	uint8_t u;
	if (m_stream->read(&u, sizeof(u)) != sizeof(u))
	{
		m_eos = true;
		return 0;
	}

	return u;
}

int16_t BitReader::readInt16()
{
	alignByte();

	int16_t u;
	if (m_stream->read(&u, sizeof(u)) != sizeof(u))
	{
		m_eos = true;
		return 0;
	}

#if defined(T_BIG_ENDIAN)
	swap8in32(u);
#endif

	return u;
}

uint16_t BitReader::readUInt16()
{
	alignByte();

	uint16_t u;
	if (m_stream->read(&u, sizeof(u)) != sizeof(u))
	{
		m_eos = true;
		return 0;
	}

#if defined(T_BIG_ENDIAN)
	swap8in32(u);
#endif

	return u;
}

int32_t BitReader::readInt32()
{
	alignByte();

	int32_t u;
	if (m_stream->read(&u, sizeof(u)) != sizeof(u))
	{
		m_eos = true;
		return 0;
	}

#if defined(T_BIG_ENDIAN)
	swap8in32(u);
#endif

	return u;
}

uint32_t BitReader::readUInt32()
{
	alignByte();

	uint32_t u;
	if (m_stream->read(&u, sizeof(u)) != sizeof(u))
	{
		m_eos = true;
		return 0;
	}

#if defined(T_BIG_ENDIAN)
	swap8in32(u);
#endif

	return u;
}

void BitReader::alignByte()
{
	m_cnt = 0;
}

uint32_t BitReader::tell() const
{
	return (int32_t(m_stream->tell() - 1) << 3) + (8 - m_cnt);
}

void BitReader::skip(uint32_t nbits)
{
	uint32_t t = std::min< uint32_t >(nbits, m_cnt);

	nbits -= t;
	m_cnt -= t;

	if (nbits > 0)
	{
		int nbytes = nbits >> 3;
		m_stream->seek(IStream::SeekCurrent, nbytes);

		nbits &= 7;
		while (nbits-- > 0)
			readBit();
	}
}

bool BitReader::eos() const
{
	return m_eos;
}

Ref< IStream > BitReader::getStream()
{
	return m_stream;
}

}
