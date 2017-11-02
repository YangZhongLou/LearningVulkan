#pragma once

#include <vulkan/vulkan.h>

namespace yzl
{
	class VulkanDevice;
	class VulkanBuffer;

	class VulkanDeviceMemory
	{
	public:
		VulkanDeviceMemory(VulkanDevice* device, VulkanBuffer* buffer, VkMemoryPropertyFlagBits  memoryProperties);
		~VulkanDeviceMemory();
	private:
		bool Init(VulkanDevice* device, VulkanBuffer* buffer, VkMemoryPropertyFlagBits  memoryProperties);
	private:
		VkDeviceMemory m_deviceMemory;
	};
}