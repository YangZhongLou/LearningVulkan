#include "VulkanFence.h"
#include "VulkanDevice.h"
#include "../Utils.h"

namespace yzl
{
	VulkanFence::VulkanFence(VulkanDevice * device, bool signaled)
	{
		m_device = device;
	}

	VulkanFence::~VulkanFence()
	{
		if (VK_NULL_HANDLE != m_fence) 
		{
			vkDestroyFence(m_device->GetDevice(), m_fence, nullptr);
			m_fence = VK_NULL_HANDLE;
		}
	}

	bool VulkanFence::Init(VulkanDevice * device, bool signaled)
	{
		VkFenceCreateInfo fenceCreateInfo = 
		{
			VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,          // VkStructureType        sType
			nullptr,                                      // const void           * pNext
			signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0u, // VkFenceCreateFlags     flags
		};

		VkResult result = vkCreateFence(device->GetDevice(), &fenceCreateInfo, nullptr, &m_fence);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not create a fence." << std::endl;
			return false;
		}

		return true;
	}

}

