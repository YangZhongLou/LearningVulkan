#include "VulkanIndexBuffer.h"

namespace yzl
{
	VulkanIndexBuffer::VulkanIndexBuffer(VulkanBuffer * buffer, VkDeviceSize memoryOffset, VkIndexType indexType)
	{
		m_buffer = buffer;
		m_memoryOffset = memoryOffset;
		m_indexType = indexType;
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
	}
}