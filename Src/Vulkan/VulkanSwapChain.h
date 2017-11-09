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
		const std::vector<VkImage>& GetImages();
		const std::vector<VkImageView>& GetImagesView();
		bool QuerySwapchainImages();
		VulkanSurface * GetVulkanSurface() const { return m_surface; }
	private:
		bool Init(VulkanDevice* device, VulkanSurface* surface);
	private:
		VulkanDevice* m_device;
		VulkanSurface* m_surface;
		VkSwapchainKHR m_swapchain;
		VkSwapchainKHR m_oldSwapchain;
		std::vector<VkImage> m_images;
		std::vector<VkImageView> m_imagesView;
	};
}