#include "VulkanCommandPool.h"
#include "Utils.h"
#include "VulkanDevice.h"


namespace yzl
{

	VulkanCommandPool::VulkanCommandPool(VulkanDevice * device, VkCommandPoolCreateFlags flags, uint32_t queueFamilyIndex)
	{
		Init(device, flags, queueFamilyIndex);
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
	}

	bool VulkanCommandPool::Init(VulkanDevice * device, VkCommandPoolCreateFlags flags, uint32_t queueFamilyIndex)
	{
		VkCommandPoolCreateInfo commandPoolCreateInfo =
		{
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			nullptr,
			flags,
			queueFamilyIndex
		};

		VkResult result = vkCreateCommandPool(device->GetDevice(), &commandPoolCreateInfo, nullptr, &m_pool);
		if (VK_SUCCESS != result)
		{
			std::cout << "Could not create command pool." << std::endl;
			return false;
		}

		return true;
	}


}