
#include "VulkanSwapchain.h"
#include "VulkanSurface.h"
#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"
#include "../Utils.h"

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

	const std::vector<VkImage>& VulkanSwapchain::GetImages()
	{
		return m_images;
	}

	const std::vector<VkImageView>& VulkanSwapchain::GetImagesView()
	{
		if (m_imagesView.size() > 0)
		{
			return m_imagesView;
		}

		if (m_images.size() == 0)
		{
			QuerySwapchainImages();
		}

		for (std::size_t i = 0; i < m_images.size(); ++i)
		{
			VkImageView imageView;
			VkImageViewCreateInfo image_view_create_info = {
				VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,  
				nullptr,                                   
				0,                                         
				m_images[i],
				VK_IMAGE_VIEW_TYPE_2D,
				m_surface->GetFormat().format,
				{                                          
					VK_COMPONENT_SWIZZLE_IDENTITY,         
					VK_COMPONENT_SWIZZLE_IDENTITY,         
					VK_COMPONENT_SWIZZLE_IDENTITY,         
					VK_COMPONENT_SWIZZLE_IDENTITY          
				},
				{                                          
					VK_IMAGE_ASPECT_COLOR_BIT,
					0,                                     
					VK_REMAINING_MIP_LEVELS,               
					0,                                     
					VK_REMAINING_ARRAY_LAYERS              
				}
			};

			VkResult result = vkCreateImageView(m_device->GetDevice(), &image_view_create_info, nullptr, &imageView);
			if (VK_SUCCESS != result) 
			{
				std::cout << "Could not create an image view." << std::endl;
			}
			m_imagesView.push_back(imageView);
		}

		return m_imagesView;
	}

	bool VulkanSwapchain::QuerySwapchainImages()
	{
		uint32_t imagesCount = 0;
		VkResult result = vkGetSwapchainImagesKHR(m_device->GetDevice(), m_swapchain, &imagesCount, nullptr);
		if ((VK_SUCCESS != result) ||
			(0 == imagesCount)) 
		{
			std::cout << "Could not get the number of swapchain images." << std::endl;
			return false;
		}

		m_images.resize(imagesCount);
		result = vkGetSwapchainImagesKHR(m_device->GetDevice(), m_swapchain, &imagesCount, m_images.data());
		if ((VK_SUCCESS != result) ||
			(0 == imagesCount)) 
		{
			std::cout << "Could not enumerate swapchain images." << std::endl;
			return false;
		}

		return true;
	}

	bool VulkanSwapchain::Init(VulkanDevice* device, VulkanSurface* surface)
	{
		m_surface = surface;
		m_device = device;
		auto physicalDevice = device->GetVulkanPhysicalDevice()->GetDevice();
		const VkSurfaceCapabilitiesKHR& surfaceCapabilities = surface->GetCapabilities(physicalDevice);

		VkSwapchainCreateInfoKHR swapchainCreateInfo = 
		{
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
			(VK_NULL_HANDLE == m_swapchain)) 
		{
			std::cout << "Could not create a swapchain." << std::endl;
			return false;
		}

		if (VK_NULL_HANDLE != m_oldSwapchain) 
		{
			vkDestroySwapchainKHR(device->GetDevice(), m_oldSwapchain, nullptr);
			m_oldSwapchain = VK_NULL_HANDLE;
		}

		return true;
	}
}


