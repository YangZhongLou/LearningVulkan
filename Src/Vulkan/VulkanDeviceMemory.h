#pragma once

#include <vulkan/vulkan.h>

namespace yzl
{
	class VulkanDevice;
	class VulkanBuffer;
	class VulkanImage;

	class VulkanDeviceMemory
	{
	public:
		VulkanDeviceMemory(VulkanDevice* device);
		~VulkanDeviceMemory();
	public:
		bool Bind(VulkanBuffer* buffer, VkMemoryPropertyFlagBits  memoryProperties);
		bool Bind(VulkanImage* image, VkMemoryPropertyFlagBits memoryProperties);
		bool Flush(VkDeviceSize offset,
			VkDeviceSize data_size,
			void * data,
			bool unmap,
			void * * pointer);
	private:
		VulkanDevice* m_device;
		VkDeviceMemory m_deviceMemory;
	};
}