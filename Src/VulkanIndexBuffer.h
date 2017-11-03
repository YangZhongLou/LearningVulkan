#pragma once

#include <vulkan/vulkan.h>

namespace yzl
{
	class VulkanBuffer;

	class VulkanIndexBuffer
	{
	public:
		VulkanIndexBuffer(VulkanBuffer * buffer,
			VkDeviceSize memoryOffset,
			VkIndexType indexType);
		~VulkanIndexBuffer();
	public:
		VulkanBuffer * GetBuffer() const { return m_buffer; }
		VkDeviceSize GetMemoryOffset() const { return m_memoryOffset; }
		VkIndexType GetIndexType() const { return m_indexType; }
	private:
		VulkanBuffer * m_buffer;
		VkDeviceSize m_memoryOffset;
		VkIndexType m_indexType;
	};
}