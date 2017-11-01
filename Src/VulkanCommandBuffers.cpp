#include "VulkanDevice.h"
#include "VulkanCommandBuffers.h"
#include "VulkanCommandPool.h"
#include "Utils.h"


namespace yzl
{

	VulkanCommandBuffers::VulkanCommandBuffers(VulkanDevice * device, VulkanCommandPool * pool, VkCommandBufferLevel level, uint32_t count)
	{
		Init(device, pool, level, count);
	}

	VulkanCommandBuffers::~VulkanCommandBuffers()
	{
	}

	bool VulkanCommandBuffers::Init(VulkanDevice * device, VulkanCommandPool * pool, VkCommandBufferLevel level, uint32_t count)
	{
		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,  
			nullptr,                                         
			pool->GetPool(),
			level,                                           
			count                                            
		};

		m_buffers.resize(count);
		VkResult result = vkAllocateCommandBuffers(device->GetDevice(), &commandBufferAllocateInfo, m_buffers.data());
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not allocate command buffers." << std::endl;
			return false;
		}
		return true;
	}

}

