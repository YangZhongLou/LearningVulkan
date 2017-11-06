#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace yzl
{
	class VulkanSurface;
	class VulkanDevice;

	class VulkanSwapchain
	{
	public:
		VulkanSwapchain(VulkanDevice* device, VulkanSurface* surface);
		~VulkanSwapchain();
	public:
		const std::vector<VkImage>& GetImages(VkDevice device);
		bool CheckImages(VkDevice device);
	private:
		bool Init(VulkanDevice* device, VulkanSurface* surface);
	private:
		VulkanDevice* m_device;
		VkSwapchainKHR m_swapchain;
		VkSwapchainKHR m_oldSwapchain;
		std::vector<VkImage> m_images;
	};
}