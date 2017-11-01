
#include "VulkanSwapchain.h"
#include "VulkanSurface.h"
#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"
#include "Utils.h"


namespace yzl
{

	VulkanSwapchain::VulkanSwapchain(VulkanDevice* device, VulkanSurface* surface)
	{
		Init(device, surface);
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		if (m_swapchain)
		{
			vkDestroySwapchainKHR(m_device->GetDevice(), m_swapchain, nullptr);
			m_swapchain = VK_NULL_HANDLE;
		}
	}

	const std::vector<VkImage>& VulkanSwapchain::GetImages(VkDevice device)
	{
		return m_images;
	}

	bool VulkanSwapchain::CheckImages(VkDevice device)
	{
		uint32_t imagesCount = 0;
		VkResult result = vkGetSwapchainImagesKHR(device, m_swapchain, &imagesCount, nullptr);
		if ((VK_SUCCESS != result) ||
			(0 == imagesCount)) {
			std::cout << "Could not get the number of swapchain images." << std::endl;
			return false;
		}

		m_images.resize(imagesCount);
		result = vkGetSwapchainImagesKHR(device, m_swapchain, &imagesCount, m_images.data());
		if ((VK_SUCCESS != result) ||
			(0 == imagesCount)) {
			std::cout << "Could not enumerate swapchain images." << std::endl;
			return false;
		}

		return true;
	}

	bool VulkanSwapchain::Init(VulkanDevice* device, VulkanSurface* surface)
	{
		m_device = device;
		auto physicalDevice = device->GetVulkanPhysicalDevice()->GetDevice();
		const VkSurfaceCapabilitiesKHR& surfaceCapabilities = surface->GetCapabilities(physicalDevice);

		VkSwapchainCreateInfoKHR swapchainCreateInfo = {
			VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR, 
			nullptr,                                     
			0,                                           
			surface->GetSurface(),                       
			surface->GetImageCount(),
			surface->GetFormat().format,
			surface->GetFormat().colorSpace,
			surface->GetImageExtent(),
			1,                                           
			surface->GetImageUsage(),
			VK_SHARING_MODE_EXCLUSIVE,                   
			0,                                           
			nullptr,                                     
			surface->GetSurfaceTransform(),
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,           
			surface->GetPresentMode(),
			VK_TRUE,                                     
			m_oldSwapchain                                
		};

		VkResult result = vkCreateSwapchainKHR(device->GetDevice(), &swapchainCreateInfo, nullptr, &m_swapchain);
		if ((VK_SUCCESS != result) ||
			(VK_NULL_HANDLE == m_swapchain)) {
			std::cout << "Could not create a swapchain." << std::endl;
			return false;
		}

		if (VK_NULL_HANDLE != m_oldSwapchain) {
			vkDestroySwapchainKHR(device->GetDevice(), m_oldSwapchain, nullptr);
			m_oldSwapchain = VK_NULL_HANDLE;
		}

		return true;
	}
}


