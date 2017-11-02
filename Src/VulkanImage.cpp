#include "VulkanImage.h"
#include "VulkanDevice.h"
#include "Utils.h"

namespace yzl
{
	VulkanImage::VulkanImage(VulkanDevice * device, VkImageType type, VkFormat format, VkExtent3D size, uint32_t numMipmaps, uint32_t numLayers, VkSampleCountFlagBits samples, VkImageUsageFlags usageScenarios, bool cubemap)
	{

	}

	VulkanImage::~VulkanImage()
	{
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
		VkImageCreateInfo image_create_info = {
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

		VkResult result = vkCreateImage(device->GetDevice(), &image_create_info, nullptr, &m_image);
		if (VK_SUCCESS != result) {
			std::cout << "Could not create an image." << std::endl;
			return false;
		}

		return true;
	}


}
