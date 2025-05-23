/*
 * TRAKTOR
 * Copyright (c) 2022 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <vector>
#include "Core/Memory/Alloc.h"
#include "Core/Memory/BlockAllocator.h"
#include "Core/Misc/Align.h"
#include "Jungle/State/VectorValue.h"

namespace traktor::jungle
{
	namespace
	{

template < typename ValueType >
class ValueAllocator
{
public:
	enum { ValuesPerBlock = 4096 };

	void* alloc()
	{
		void* ptr = 0;
		for (std::vector< BlockAllocator* >::iterator i = m_allocators.begin(); i != m_allocators.end(); ++i)
		{
			BlockAllocator* allocator = *i;
			if ((ptr = allocator->alloc()) != 0)
				return ptr;
		}

		// No more space in block allocators; create a new block allocator.
		void* top = Alloc::acquireAlign(ValuesPerBlock * sizeof(ValueType), alignOf< ValueType >(), T_FILE_LINE);
		T_FATAL_ASSERT_M (top, L"Out of memory");

		BlockAllocator* allocator = new BlockAllocator(top, ValuesPerBlock, sizeof(ValueType));
		T_FATAL_ASSERT_M (allocator, L"Out of memory");

		m_allocators.push_back(allocator);
		return allocator->alloc();
	}

	void free(void* ptr)
	{
		for (std::vector< BlockAllocator* >::iterator i = m_allocators.begin(); i != m_allocators.end(); ++i)
		{
			BlockAllocator* allocator = *i;
			if (allocator->free(ptr))
				return;
		}

		T_FATAL_ERROR;
	}

private:
	std::vector< BlockAllocator* > m_allocators;
};

ValueAllocator< VectorValue > s_valueAllocator;

	}

T_IMPLEMENT_RTTI_CLASS(L"traktor.jungle.VectorValue", VectorValue, IValue)

void* VectorValue::operator new (size_t size)
{
	T_ASSERT(size == sizeof(VectorValue));
	return s_valueAllocator.alloc();
}

void VectorValue::operator delete (void* ptr)
{
	s_valueAllocator.free(ptr);
}

}
