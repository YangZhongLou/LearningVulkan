#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace yzl
{
	class VulkanDevice;
	class VulkanCommandPool;

	class VulkanCommandBuffers
	{
	public:
		VulkanCommandBuffers(VulkanDevice* device, VulkanCommandPool* pool, VkCommandBufferLevel level, uint32_t count);
		~VulkanCommandBuffers();

	private:
		bool Init(VulkanDevice* device, VulkanCommandPool* pool, VkCommandBufferLevel level, uint32_t count);
	private:
		VulkanDevice* m_device;
		VulkanCommandPool* m_pool;
		std::vector<VkCommandBuffer> m_buffers;
	};
}