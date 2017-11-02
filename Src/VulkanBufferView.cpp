#include "VulkanBufferView.h"
#include "VulkanDevice.h"
#include "Utils.h"

namespace yzl
{
	VulkanBufferView::VulkanBufferView(VulkanDevice * device, VkBuffer buffer, VkFormat format, VkDeviceSize memoryOffset, VkDeviceSize memoryRange)
	{
		Init(device, buffer, format, memoryOffset, memoryRange);
	}

	VulkanBufferView::~VulkanBufferView()
	{
	}

	bool VulkanBufferView::Init(VulkanDevice * device, VkBuffer buffer, VkFormat format, VkDeviceSize memoryOffset, VkDeviceSize memoryRange)
	{
		VkBufferViewCreateInfo bufferViewCreateInfo = 
		{
			VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO,   
			nullptr,                                     
			0,                                           
			buffer,                                      
			format,                                      
			memoryOffset,
			memoryRange
		};

		VkResult result = vkCreateBufferView(device->GetDevice(), &bufferViewCreateInfo, nullptr, &m_bufferView);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not creat buffer view." << std::endl;
			return false;
		}

		return true;
	}
}