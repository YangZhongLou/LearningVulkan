#pragma once

#include <vulkan/vulkan.h>

namespace yzl
{
	class VulkanDevice;

	class VulkanBufferView
	{
	public:
		VulkanBufferView(VulkanDevice* device,
			VkBuffer       buffer,
			VkFormat       format,
			VkDeviceSize   memoryOffset,
			VkDeviceSize   memoryRange);
		~VulkanBufferView();
	private:
		bool Init(VulkanDevice* device,
			VkBuffer       buffer,
			VkFormat       format,
			VkDeviceSize   memoryOffset,
			VkDeviceSize   memoryRange);
	private:
		VkBufferView m_bufferView;
	};
}