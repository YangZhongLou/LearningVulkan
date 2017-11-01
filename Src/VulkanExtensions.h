#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include "Singleton.hpp"

namespace yzl
{
	Singleton(VulkanExtensions)
		public:
			bool IsExtensionSupported(const std::string& extension);
			bool EnumerateInstanceExtension();
			bool EnumerateDeviceExtension(VkPhysicalDevice& physicalDevice);
		private:
			std::vector<VkExtensionProperties> m_extensions;
	};

#define vkExtensions VulkanExtensions::GetInstance()
}