#include "VulkanBuffer.h"
#include "VulkanDevice.h"
#include "Utils.h"

namespace yzl
{
	VulkanBuffer::VulkanBuffer(VulkanDevice * device, VkDeviceSize size, VkBufferUsageFlags usage)
	{
		Init(device, size, usage);
	}

	VulkanBuffer::~VulkanBuffer()
	{

	}

	bool VulkanBuffer::Init(VulkanDevice * device, VkDeviceSize size, VkBufferUsageFlags usage)
	{
		VkBufferCreateInfo bufferCreateInfo = {
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,   
			nullptr,                                
			0,                                      
			size,                                   
			usage,                                  
			VK_SHARING_MODE_EXCLUSIVE,              
			0,                                      
			nullptr                                 
		};

		VkResult result = vkCreateBuffer(device->GetDevice(), &bufferCreateInfo, nullptr, &m_buffer);
		if (VK_SUCCESS != result) {
			std::cout << "Could not create a buffer." << std::endl;
			return false;
		}

		return true;
	}

}