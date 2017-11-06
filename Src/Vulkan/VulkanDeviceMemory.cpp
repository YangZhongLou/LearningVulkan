#include "VulkanDeviceMemory.h"
#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanImage.h"
#include "../Utils.h"

namespace yzl
{
	VulkanDeviceMemory::VulkanDeviceMemory(VulkanDevice * device)
	{
		m_device = device;
	}

	VulkanDeviceMemory::~VulkanDeviceMemory()
	{
		if (m_deviceMemory != VK_NULL_HANDLE)
		{
			vkFreeMemory(m_device->GetDevice(), m_deviceMemory, nullptr);
			m_deviceMemory = VK_NULL_HANDLE;
		}
	}

	bool VulkanDeviceMemory::Bind(VulkanBuffer * buffer, VkMemoryPropertyFlagBits memoryProperties)
	{
		VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
		vkGetPhysicalDeviceMemoryProperties(m_device->GetVulkanPhysicalDevice()->GetDevice(), &physicalDeviceMemoryProperties);

		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(m_device->GetDevice(), buffer->GetBuffer(), &memoryRequirements);

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

				VkResult result = vkAllocateMemory(m_device->GetDevice(), &bufferMemoryAllocateInfo, nullptr, &m_deviceMemory);
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

		VkResult result = vkBindBufferMemory(m_device->GetDevice(), buffer->GetBuffer(), m_deviceMemory, 0);
		if (VK_SUCCESS != result)
		{
			std::cout << "Could not bind memory object to a buffer." << std::endl;
			return false;
		}
		return true;
	}
	bool VulkanDeviceMemory::Bind(VulkanImage * image, VkMemoryPropertyFlagBits memoryProperties)
	{
		VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
		vkGetPhysicalDeviceMemoryProperties(m_device->GetVulkanPhysicalDevice()->GetDevice(), &physicalDeviceMemoryProperties);

		VkMemoryRequirements memoryRequirements;
		vkGetImageMemoryRequirements(m_device->GetDevice(), image->GetImage(), &memoryRequirements);

		m_deviceMemory = VK_NULL_HANDLE;
		for (uint32_t type = 0; type < physicalDeviceMemoryProperties.memoryTypeCount; ++type) 
		{
			if ((memoryRequirements.memoryTypeBits & (1 << type)) &&
				((physicalDeviceMemoryProperties.memoryTypes[type].propertyFlags & memoryProperties) == memoryProperties)) 
			{

				VkMemoryAllocateInfo image_memory_allocate_info = {
					VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,  
					nullptr,                                 
					memoryRequirements.size,
					type                                     
				};

				VkResult result = vkAllocateMemory(m_device->GetDevice(), &image_memory_allocate_info, nullptr, &m_deviceMemory);
				if (VK_SUCCESS == result) {
					break;
				}
			}
		}

		if (VK_NULL_HANDLE == m_deviceMemory) 
		{
			std::cout << "Could not allocate memory for an image." << std::endl;
			return false;
		}

		VkResult result = vkBindImageMemory(m_device->GetDevice(), image->GetImage(), m_deviceMemory, 0);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not bind memory object to an image." << std::endl;
			return false;
		}

		return true;
	}

	bool VulkanDeviceMemory::Flush(VkDeviceSize offset, VkDeviceSize dataSize, void * data, bool unmap, void ** pointer)
	{
		VkResult result;
		void * localPointer;
		result = vkMapMemory(m_device->GetDevice(), m_deviceMemory, offset, dataSize, 0, &localPointer);
		if (VK_SUCCESS != result) {
			std::cout << "Could not map memory object." << std::endl;
			return false;
		}

		std::memcpy(localPointer, data, dataSize);

		std::vector<VkMappedMemoryRange> memoryRanges = 
		{
			{
				VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE, 
				nullptr,                               
				m_deviceMemory,                        
				offset,                                
				VK_WHOLE_SIZE                          
			}
		};

		vkFlushMappedMemoryRanges(m_device->GetDevice(), static_cast<uint32_t>(memoryRanges.size()), memoryRanges.data());
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not flush mapped memory." << std::endl;
			return false;
		}

		if (unmap) 
		{
			vkUnmapMemory(m_device->GetDevice(), m_deviceMemory);
		}
		else if (nullptr != pointer) 
		{
			*pointer = localPointer;
		}

		return true;
	}
}


