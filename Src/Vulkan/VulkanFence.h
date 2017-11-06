#pragma once

#include <vulkan/vulkan.h>

namespace yzl
{
	class VulkanDevice;

	class VulkanFence
	{
	public:
		VulkanFence(VulkanDevice * device, bool signaled);
		~VulkanFence();
	private:
		bool Init(VulkanDevice* device, bool signaled);
	private:
		VulkanDevice* m_device;
		VkFence m_fence;
	};
}