#include "VulkanDeviceMemory.h"
#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "VulkanPhysicalDevice.h"
#include "Utils.h"

namespace yzl
{
	VulkanDeviceMemory::VulkanDeviceMemory(VulkanDevice * device, VulkanBuffer * buffer, VkMemoryPropertyFlagBits memoryProperties)
	{
		Init(device, buffer, memoryProperties);
	}

	VulkanDeviceMemory::~VulkanDeviceMemory()
	{
	}

	bool VulkanDeviceMemory::Init(VulkanDevice * device, VulkanBuffer * buffer, VkMemoryPropertyFlagBits memoryProperties)
	{
		VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
		vkGetPhysicalDeviceMemoryProperties(device->GetVulkanPhysicalDevice()->GetDevice(), &physicalDeviceMemoryProperties);

		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(device->GetDevice(), buffer->GetBuffer(), &memoryRequirements);

		m_deviceMemory = VK_NULL_HANDLE;
		for (uint32_t type = 0; type < physicalDeviceMemoryProperties.memoryTypeCount; ++type) 
		{
			if ((memoryRequirements.memoryTypeBits & (1 << type)) &&
				((physicalDeviceMemoryProperties.memoryTypes[type].propertyFlags & memoryProperties) == memoryProperties)) 
			{

				VkMemoryAllocateInfo bufferMemoryAllocateInfo = 
				{
					VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,  
					nullptr,                                 
					memoryRequirements.size,                 
					type                                     
				};

				VkResult result = vkAllocateMemory(device->GetDevice(), &bufferMemoryAllocateInfo, nullptr, &m_deviceMemory);
				if (VK_SUCCESS == result) 
				{
					break;
				}
			}
		}

		if (VK_NULL_HANDLE == m_deviceMemory) {
			std::cout << "Could not allocate memory for a buffer." << std::endl;
			return false;
		}

		VkResult result = vkBindBufferMemory(device->GetDevice(), buffer->GetBuffer(), m_deviceMemory, 0);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not bind memory object to a buffer." << std::endl;
			return false;
		}
		return true;
	}
}


