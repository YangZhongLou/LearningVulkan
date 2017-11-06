#include "VulkanCommandPool.h"
#include "../Utils.h"
#include "VulkanDevice.h"


namespace yzl
{

	VulkanCommandPool::VulkanCommandPool(VulkanDevice * device, VkCommandPoolCreateFlags flags, uint32_t queueFamilyIndex)
	{
		Init(device, flags, queueFamilyIndex);
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
		if (VK_NULL_HANDLE != m_pool) 
		{
			ReleaseCommandBuffers();
			vkDestroyCommandPool(m_device->GetDevice(), m_pool, nullptr);
			m_pool = VK_NULL_HANDLE;
		}
	}

	bool VulkanCommandPool::Reset(VulkanDevice * device, bool releaseResources)
	{
		VkResult result = vkResetCommandPool(device->GetDevice(), m_pool, releaseResources ? VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT : 0);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Error occurred during command pool reset." << std::endl;
			return false;
		}

		return true;
	}

	bool VulkanCommandPool::AllocateCommandBuffers(VulkanDevice * device, VulkanCommandPool * pool, VkCommandBufferLevel level, uint32_t count)
	{
		VkCommandBufferAllocateInfo commandBufferAllocateInfo = 
		{
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

	bool VulkanCommandPool::ReleaseCommandBuffers()
	{
		if (m_buffers.size() > 0) 
		{
			vkFreeCommandBuffers(m_device->GetDevice(), m_pool, static_cast<uint32_t>(m_buffers.size()), m_buffers.data());
			m_buffers.clear();
		}
		return true;
	}

	bool VulkanCommandPool::Init(VulkanDevice * device, VkCommandPoolCreateFlags flags, uint32_t queueFamilyIndex)
	{
		m_device = device;
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