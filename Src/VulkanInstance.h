#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

namespace yzl
{
	class VulkanInstance
	{
	public:
		VkInstance m_vkInstance;

	public:
		VulkanInstance(std::string appName, std::string engineName, const std::vector<char const *>& extensions);
		~VulkanInstance();

	public:
		bool EnumeratePhysicalDevices();
		bool CheckExtensions(const std::vector<char const *>& extensions);
	private:
		bool Init(std::string& appName, std::string& engineName, const std::vector<char const *>& extensions);
	private:
		std::vector<VkPhysicalDevice> m_physicalDevices;
	};
}
