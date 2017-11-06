#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

namespace yzl
{
	class VulkanDevice;
	class VulkanPhysicalDevice;

	class VulkanInstance
	{
	public:
		VulkanInstance(std::string appName, std::string engineName, const std::vector<char const *>& extensions);
		~VulkanInstance();

	public:
		VkInstance GetInstance() const { return m_vkInstance; }
		bool EnumeratePhysicalDevices();
		bool CheckExtensions(const std::vector<char const *>& extensions);
		VulkanDevice* GetDevice() { return m_device; }
		bool SelectDevice(std::vector<char const *> const & desiredExtensions);
	private:
		bool Init(std::string& appName, std::string& engineName, const std::vector<char const *>& extensions);
	private:
		VkInstance m_vkInstance;
		std::vector<VkPhysicalDevice> m_physicalDevices;
		VulkanDevice* m_device;
	};
}
