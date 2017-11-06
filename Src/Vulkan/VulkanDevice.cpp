
#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanCommandPool.h"
#include "VulkanExtensions.h"
#include "VulkanSemaphore.h"
#include "VulkanQueue.h"
#include "../Utils.h"
#include <iostream>


namespace yzl
{
	VulkanDevice::VulkanDevice(VulkanInstance* vkInstance, std::vector<VkPhysicalDevice>& physicalDevices,
		std::vector<char const *> const & desiredExtensions)
	{
		SelectPhysicalDevice(vkInstance, physicalDevices, desiredExtensions);

		AllocateCommandPool(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, m_graphicsQueue->GetFamilyIndex());
	}

	VulkanDevice::~VulkanDevice()
	{
		SAFE_DELETE(m_commandPool);
		SAFE_DELETE(m_vulkanPhysicalDevice);
		SAFE_DELETE(m_graphicsQueue);
		SAFE_DELETE(m_computeQueue);
		vkDestroyDevice(m_device, NULL);
	}

	static bool CreateLogicalDevice(VkPhysicalDevice physicalDevice,
		std::vector<QueueInfo>            queueInfos,
		std::vector<char const *> const & desiredExtensions,
		VkPhysicalDeviceFeatures        * desiredFeatures,
		VkDevice                        & logicalDevice) {
		std::vector<VkExtensionProperties> availableExtensions;

		vkExtensions.EnumerateDeviceExtension(physicalDevice);

		for (auto & extension : desiredExtensions) {
			if (!vkExtensions.IsExtensionSupported(extension)) {
				std::cout << "Extension named '" << extension << "' is not supported by a physical device." << std::endl;
				return false;
			}
		}

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		for (auto & info : queueInfos) {
			queueCreateInfos.push_back({
				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,       
				nullptr,                                          
				0,                                                
				info.familyIndex,                                 
				static_cast<uint32_t>(info.priorities.size()),    
				info.priorities.data()
			});
		};

		VkDeviceCreateInfo deviceCreateInfo = {
			VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,               
			nullptr,                                            
			0,                                                  
			static_cast<uint32_t>(queueCreateInfos.size()),
			queueCreateInfos.data(),
			0,                                                  
			nullptr,                                            
			static_cast<uint32_t>(desiredExtensions.size()),   
			desiredExtensions.data(),                          
			desiredFeatures
		};

		VkResult result = vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice);
		if ((result != VK_SUCCESS) ||
			(logicalDevice == VK_NULL_HANDLE)) {
			std::cout << "Could not create logical device." << std::endl;
			return false;
		}

		return true;
	}

	bool VulkanDevice::SynchronizeCommandBuffers(VulkanQueue * firstQueue, 
		std::vector<SemaphoreInfo*> firstWaitsemaphoreInfos,
		std::vector<VkCommandBuffer> firstCommandBuffers,
		std::vector<SemaphoreInfo*> syncSemaphoreInfos,
		VulkanQueue * secondQueue,
		std::vector<VkCommandBuffer> secondCommandBuffers,
		std::vector<VkSemaphore> secondSignalSemaphores, 
		VkFence secondFence)
	{
		std::vector<VkSemaphore> firstSignalSemaphores;
		for (auto & info : syncSemaphoreInfos) 
		{
			firstSignalSemaphores.emplace_back(info->semaphore);
		}

		if (!firstQueue->Submit(firstWaitsemaphoreInfos, firstCommandBuffers, firstSignalSemaphores, VK_NULL_HANDLE)) 
		{
			return false;
		}

		if (!secondQueue->Submit(syncSemaphoreInfos, secondCommandBuffers, secondSignalSemaphores, secondFence)) 
		{
			return false;
		}
		return true;
	}

	bool VulkanDevice::WaitForFences(std::vector<VkFence> const & fences, VkBool32 waitForAll, uint64_t timeout)
	{
		if (fences.size() > 0) 
		{
			VkResult result = vkWaitForFences(m_device, static_cast<uint32_t>(fences.size()), fences.data(), waitForAll, timeout);
			if (VK_SUCCESS != result) {
				std::cout << "Waiting on fence failed." << std::endl;
				return false;
			}
			return true;
		}
		return false;
	}

	bool VulkanDevice::WaitIdle()
	{
		VkResult result = vkDeviceWaitIdle(m_device);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Waiting on a device failed." << std::endl;
			return false;
		}
		return true;
	}

	bool VulkanDevice::ResetFences(std::vector<VkFence> const & fences)
	{
		if (fences.size() > 0) 
		{
			VkResult result = vkResetFences(m_device, static_cast<uint32_t>(fences.size()), fences.data());
			if (VK_SUCCESS != result) 
			{
				std::cout << "Error occurred when tried to reset fences." << std::endl;
				return false;
			}
			return VK_SUCCESS == result;
		}

		return false;
	}

	bool VulkanDevice::AllocateCommandPool(VkCommandPoolCreateFlags flags, uint32_t queueFamilyIndex)
	{
		m_commandPool = new VulkanCommandPool(this, flags, queueFamilyIndex);
	}

	bool VulkanDevice::SelectPhysicalDevice(const VulkanInstance * vkInstance, const std::vector<VkPhysicalDevice>& physicalDevices,
			std::vector<char const *> const & desiredExtensions)
	{
		for (auto& device : physicalDevices) 
		{
			VulkanPhysicalDevice physicalDevice(device);
			VkPhysicalDeviceFeatures deviceFeatures = physicalDevice.GetFeatures();
			VkPhysicalDeviceProperties deviceProperties = physicalDevice.GetProperties();

			if (!deviceFeatures.geometryShader) 
			{
				continue;
			}
			else 
			{
				deviceFeatures = {};
				deviceFeatures.geometryShader = VK_TRUE;
			}

			uint32_t graphicsQueueFamilyIndex = physicalDevice.GetCapabilitiesFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
			if (graphicsQueueFamilyIndex == -1) 
			{
				continue;
			}

			uint32_t computeQueueFamilyIndex = physicalDevice.GetCapabilitiesFamilyIndex(VK_QUEUE_COMPUTE_BIT);
			if (computeQueueFamilyIndex == -1)
			{
				continue;
			}

			std::vector<QueueInfo> requestedQueues = { { graphicsQueueFamilyIndex,{ 1.0f } } };
			if (graphicsQueueFamilyIndex != computeQueueFamilyIndex) {
				requestedQueues.push_back({ computeQueueFamilyIndex,{ 1.0f } });
			}

			/* Create logical device*/
			if (!CreateLogicalDevice(device, requestedQueues, desiredExtensions, &deviceFeatures, m_device)) 
			{
				continue;
			}
			else 
			{
				if (!LoadDeviceLevelFunctions(m_device, {})) 
				{
					return false;
				}

				m_vulkanPhysicalDevice = new VulkanPhysicalDevice(device);
				VkQueue graphicsQueue;
				VkQueue computeQueue;
				vkGetDeviceQueue(m_device, graphicsQueueFamilyIndex, 0, &graphicsQueue);
				vkGetDeviceQueue(m_device, graphicsQueueFamilyIndex, 0, &computeQueue);

				m_graphicsQueue = new VulkanQueue(this, graphicsQueue, graphicsQueueFamilyIndex);
				m_computeQueue = new VulkanQueue(this, graphicsQueue, computeQueueFamilyIndex);
				return true;
			}
		}

		return false;
	}
}