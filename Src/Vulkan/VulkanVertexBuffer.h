#pragma once

#include <vulkan/vulkan.h>

namespace yzl
{
	class VulkanBuffer;

	class VulkanVertexBuffer
	{
	public:
		VulkanVertexBuffer(VulkanBuffer* buffer, VkDeviceSize memoryOffset);
		~VulkanVertexBuffer();
	public:
		VulkanBuffer* GetBuffer() const { return m_buffer; }
		VkDeviceSize GetMemoryOffset() const { return m_memoryOffset; }
	private:
		VulkanBuffer* m_buffer;
		VkDeviceSize  m_memoryOffset;
	};
}