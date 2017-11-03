#pragma once

#include <vulkan/vulkan.h>

namespace yzl
{
	class VulkanDevice;

	struct BufferTransition {
		VkBuffer        buffer;
		VkAccessFlags   currentAccess;
		VkAccessFlags   newAccess;
		uint32_t        currentQueueFamily;
		uint32_t        newQueueFamily;
	};

	class VulkanBuffer
	{
	public:
		VulkanBuffer(VulkanDevice* device, VkDeviceSize size,VkBufferUsageFlags usage);
		~VulkanBuffer();
	public:
		VkBuffer GetBuffer() const { return m_buffer; }
		bool CreateView(VkFormat format,
			VkDeviceSize memoryOffset,
			VkDeviceSize memoryRangeVkBufferView, 
			VkBufferView& bufferView);
	private:
		bool Init(VulkanDevice* device, VkDeviceSize size, VkBufferUsageFlags usage);
	private:
		VulkanDevice* m_device;
		VkBufferView m_bufferView;
		VkBuffer m_buffer;
	};
}