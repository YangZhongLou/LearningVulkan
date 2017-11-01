
#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanExtensions.h"
#include "Utils.h"
#include <iostream>


namespace yzl
{
	VulkanDevice::VulkanDevice(VulkanInstance* vkInstance, std::vector<VkPhysicalDevice>& physicalDevices,
		std::vector<char const *> const & desiredExtensions)
	{
		Init(vkInstance, physicalDevices, desiredExtensions);
	}

	VulkanDevice::~VulkanDevice()
	{
		SAFE_DELETE(m_vulkanPhysicalDevice);
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

	bool VulkanDevice::Init(const VulkanInstance * vkInstance, const std::vector<VkPhysicalDevice>& physicalDevices, 
			std::vector<char const *> const & desiredExtensions)
	{
		for (auto& device : physicalDevices) {
			VulkanPhysicalDevice physicalDevice(device);
			VkPhysicalDeviceFeatures deviceFeatures = physicalDevice.GetFeatures();
			VkPhysicalDeviceProperties deviceProperties = physicalDevice.GetProperties();

			if (!deviceFeatures.geometryShader) {
				continue;
			}
			else {
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
			if (!CreateLogicalDevice(device, requestedQueues, desiredExtensions, &deviceFeatures, m_device)) {
				continue;
			}
			else {
				if (!LoadDeviceLevelFunctions(m_device, {})) {
					return false;
				}

				m_vulkanPhysicalDevice = new VulkanPhysicalDevice(device);
				vkGetDeviceQueue(m_device, graphicsQueueFamilyIndex, 0, &m_graphicsQueue);
				vkGetDeviceQueue(m_device, graphicsQueueFamilyIndex, 0, &m_computeQueue);
				return true;
			}
		}

		return false;
	}
}