#include "VulkanImage.h"
#include "VulkanDevice.h"
#include "../Utils.h"

namespace yzl
{
	VulkanImage::VulkanImage(VulkanDevice * device, VkImageType type, VkFormat format, VkExtent3D size, uint32_t numMipmaps, uint32_t numLayers, VkSampleCountFlagBits samples, VkImageUsageFlags usageScenarios, bool cubemap)
	{
		m_imageView = VK_NULL_HANDLE;
		Init(device, type, format, size, numMipmaps, numLayers, samples, usageScenarios, cubemap);
	}

	VulkanImage::~VulkanImage()
	{
		if (m_imageView != VK_NULL_HANDLE)
		{
			vkDestroyImageView(m_device->GetDevice(), m_imageView, nullptr);
			m_imageView = VK_NULL_HANDLE;
		}

		if (m_image != VK_NULL_HANDLE)
		{
			vkDestroyImage(m_device->GetDevice(), m_image, nullptr);
			m_image = VK_NULL_HANDLE;
		}
	}

	bool VulkanImage::CreateView(VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspect, VkImageView & imageView)
	{
		if (m_imageView != VK_NULL_HANDLE)
		{
			imageView = m_imageView;
			return true;
		}

		VkImageViewCreateInfo imageViewCreateInfo = 
		{
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,   
			nullptr,                                    
			0,                                          
			m_image,                                      
			viewType,
			format,                                     
			{                                           
				VK_COMPONENT_SWIZZLE_IDENTITY,          
				VK_COMPONENT_SWIZZLE_IDENTITY,          
				VK_COMPONENT_SWIZZLE_IDENTITY,          
				VK_COMPONENT_SWIZZLE_IDENTITY           
			},
			{                                           
				aspect,                                 
				0,                                      
				VK_REMAINING_MIP_LEVELS,                
				0,                                      
				VK_REMAINING_ARRAY_LAYERS               
			}
		};

		VkResult result = vkCreateImageView(m_device->GetDevice(), &imageViewCreateInfo, nullptr, &imageView);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not create an image view." << std::endl;
			return false;
		}

		m_imageView = imageView;
		return true;
	}

	bool VulkanImage::Init(VulkanDevice * device,
		VkImageType type, 
		VkFormat format, 
		VkExtent3D size, 
		uint32_t numMipmaps, 
		uint32_t numLayers, 
		VkSampleCountFlagBits samples, 
		VkImageUsageFlags usageScenarios, 
		bool cubemap)
	{
		m_device = device;

		VkImageCreateInfo imageCreateInfo = {
			VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,                
			nullptr,                                            
			cubemap ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0u, 
			type,                                               
			format,                                             
			size,                                               
			numMipmaps,                                    
			cubemap ? 6 * numLayers : numLayers,           
			samples,                                       
			VK_IMAGE_TILING_OPTIMAL,                       
			usageScenarios,                                
			VK_SHARING_MODE_EXCLUSIVE,                          
			0,                                                  
			nullptr,                                            
			VK_IMAGE_LAYOUT_UNDEFINED                           
		};

		VkResult result = vkCreateImage(device->GetDevice(), &imageCreateInfo, nullptr, &m_image);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not create an image." << std::endl;
			return false;
		}

		return true;
	}


}
