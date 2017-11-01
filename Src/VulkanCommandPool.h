#pragma once

#include <vulkan/vulkan.h>

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
	private:
		bool Init(VulkanDevice* device, VkCommandPoolCreateFlags flags, uint32_t queueFamilyIndex);
	private:
		VkCommandPool m_pool;
	};
}