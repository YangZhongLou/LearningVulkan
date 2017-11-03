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
		if (m_bufferView != VK_NULL_HANDLE)
		{
			vkDestroyBufferView(m_device->GetDevice(), m_bufferView, nullptr);
			m_bufferView = VK_NULL_HANDLE;
		}

		if (m_buffer != VK_NULL_HANDLE) 
		{
			vkDestroyBuffer(m_device->GetDevice(), m_buffer, nullptr);
			m_buffer = VK_NULL_HANDLE;
		}
	}

	bool VulkanBuffer::CreateView(VkFormat format, VkDeviceSize memoryOffset, VkDeviceSize memoryRange, VkBufferView & bufferView)
	{
		if (m_bufferView != VK_NULL_HANDLE)
		{
			bufferView = m_bufferView;
			return true;
		}

		VkBufferViewCreateInfo bufferViewCreateInfo =
		{
			VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO,
			nullptr,
			0,
			m_buffer,
			format,
			memoryOffset,
			memoryRange
		};

		VkResult result = vkCreateBufferView(m_device->GetDevice(), &bufferViewCreateInfo, nullptr, &bufferView);
		if (VK_SUCCESS != result)
		{
			std::cout << "Could not creat buffer view." << std::endl;
			return false;
		}

		m_bufferView = bufferView;

		return true;
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