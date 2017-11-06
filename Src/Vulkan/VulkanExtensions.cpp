
#include "VulkanExtensions.h"
#include "../Utils.h"
#include <iostream>

namespace yzl
{
	VulkanExtensions::~VulkanExtensions(){}

	bool VulkanExtensions::EnumerateInstanceExtension()
	{
		uint32_t extensionsCount = 0;
		VkResult result = VK_SUCCESS;

		result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);
		if ((result != VK_SUCCESS) ||
			(extensionsCount == 0)) {
			std::cout << "Could not get the number of instance extensions." << std::endl;
			return false;
		}

		m_extensions.resize(extensionsCount);
		result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, m_extensions.data());
		if ((result != VK_SUCCESS) ||
			(extensionsCount == 0)) {
			std::cout << "Could not enumerate instance extensions." << std::endl;
			return false;
		}

		return true;
	}

	bool VulkanExtensions::EnumerateDeviceExtension(VkPhysicalDevice& physicalDevice)
	{
		uint32_t extensionsCount = 0;
		VkResult result = VK_SUCCESS;

		result = vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, nullptr);
		if ((result != VK_SUCCESS) ||
			(extensionsCount == 0)) {
			std::cout << "Could not get the number of device extensions." << std::endl;
			return false;
		}

		m_extensions.resize(extensionsCount);
		result = vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionsCount, m_extensions.data());
		if ((result != VK_SUCCESS) ||
			(extensionsCount == 0)) {
			std::cout << "Could not enumerate device extensions." << std::endl;
			return false;
		}

		return true;
	}

	bool VulkanExtensions::IsExtensionSupported(const std::string& extension)
	{
		for (auto e : m_extensions)
		{
			if (strstr(e.extensionName, extension.c_str()))
			{
				return true;
			}
		}

		return false;
	}
}