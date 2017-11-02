#include "VulkanSemaphore.h"
#include "VulkanDevice.h"
#include "Utils.h"

namespace yzl
{
	VulkanSemaphore::VulkanSemaphore(VulkanDevice * device)
	{
		Init(device);
	}

	VulkanSemaphore::~VulkanSemaphore()
	{
		if (VK_NULL_HANDLE != m_semaphore) 
		{
			vkDestroySemaphore(m_device->GetDevice(), m_semaphore, nullptr);
			m_semaphore = VK_NULL_HANDLE;
		}
	}

	bool VulkanSemaphore::Init(VulkanDevice * device)
	{
		m_device = device;
		VkSemaphoreCreateInfo semaphoreCreateInfo = 
		{
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,    
			nullptr,                                    
			0                                           
		};

		VkResult result = vkCreateSemaphore(device->GetDevice(), &semaphoreCreateInfo, nullptr, &m_semaphore);
		if (VK_SUCCESS != result) {
			std::cout << "Could not create a semaphore." << std::endl;
			return false;
		}

		return true;
	}

}