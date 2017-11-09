#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace yzl
{
	class VulkanDevice;

	class VulkanCommandPool
	{
	public:
		VulkanCommandPool(VulkanDevice* device, VkCommandPoolCreateFlags flags, uint32_t queueFamilyIndex);
		~VulkanCommandPool();
	public:
		const VkCommandPool GetPool() { return m_pool; }
		VkCommandBuffer GetCommandBuffer(uint32_t i) { return m_buffers[i]; }
		bool AllocateCommandBuffers(VkCommandBufferLevel level, uint32_t count);
		bool Reset(VulkanDevice* device, bool releaseResources);
	private:
		bool Init(VulkanDevice* device, VkCommandPoolCreateFlags flags, uint32_t queueFamilyIndex);
		bool ReleaseCommandBuffers();
	private:
		VulkanDevice* m_device;
		VkCommandPool m_pool;
		std::vector<VkCommandBuffer> m_buffers;
	};
}