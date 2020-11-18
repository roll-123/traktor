#include "Core/Config.h"
#include "Render/Vulkan/Buffer.h"

namespace traktor
{
    namespace render
    {

bool Buffer::create(VmaAllocator allocator, uint32_t bufferSize, uint32_t usageBits, bool cpuAccess, bool gpuAccess)
{
	T_ASSERT(m_allocator == 0);

	VkBufferCreateInfo bci = {};
	bci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bci.size = bufferSize;
	bci.usage = usageBits;
	bci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VmaAllocationCreateInfo aci = {};
    if (cpuAccess && gpuAccess)
	    aci.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
    else if (!cpuAccess && gpuAccess)
        aci.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    else if (cpuAccess && !gpuAccess)
        aci.usage = VMA_MEMORY_USAGE_CPU_ONLY;
    else
        return false;

	if (vmaCreateBuffer(allocator, &bci, &aci, &m_buffer, &m_allocation, nullptr) != VK_SUCCESS)
		return false;

	m_allocator = allocator;
    return true;
}

void Buffer::destroy()
{
	if (m_allocator != 0)
	{
		vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);
		m_allocator = 0;
		m_allocation = 0;
		m_buffer = 0;
	}
}

void* Buffer::lock()
{
	void* ptr = nullptr;
	if (vmaMapMemory(m_allocator, m_allocation, &ptr) == VK_SUCCESS)
	{
		m_locked = true;
		return ptr;
	}
	else
		return nullptr;    
}

void Buffer::unlock()
{
	if (m_locked)
	{
		vmaUnmapMemory(m_allocator, m_allocation);
		m_locked = false;
	}
}

    }
}