#include "VulkanVertexBuffer.h"

namespace yzl
{
	VulkanVertexBuffer::VulkanVertexBuffer(VulkanBuffer * buffer, VkDeviceSize memoryOffset)
	{
		m_buffer = buffer;
		m_memoryOffset = memoryOffset;
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		m_buffer = VK_NULL_HANDLE;
	}
}