#include "VulkanInstance.h"
#include "VulkanExtensions.h"
#include "VulkanPhysicalDevice.h"
#include <iostream>
#include "Utils.h"

namespace yzl
{
	VulkanInstance::VulkanInstance(std::string appName, std::string engineName, const std::vector<char const *>& extensions)
	{
		Init(appName, engineName, extensions);
	}

	VulkanInstance::~VulkanInstance()
	{
		vkDestroyInstance(m_vkInstance, nullptr);
		m_vkInstance = VK_NULL_HANDLE;
	}

	bool VulkanInstance::Init(std::string& appName, std::string& engineName, const std::vector<char const *>& extensions)
	{
		CheckExtensions(extensions);

		VkApplicationInfo applicationInfo = {
			VK_STRUCTURE_TYPE_APPLICATION_INFO,
			nullptr,
			appName.c_str(),
			VK_MAKE_VERSION(1, 0, 0),
			engineName.c_str(),
			VK_MAKE_VERSION(1, 0, 0),
			VK_MAKE_VERSION(1, 0, 0)
		};

		VkInstanceCreateInfo instanceCreateInfo = {
			VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			nullptr,
			0,
			&applicationInfo,
			0,
			nullptr,
			static_cast<uint32_t>(extensions.size()),
			extensions.data(),
		};

		VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_vkInstance);
		if ((result != VK_SUCCESS) ||
			(m_vkInstance == VK_NULL_HANDLE)) {
			std::cout << "Could not create Vulkan instance." << std::endl;
		}

		LoadInstanceLevelFunctions(m_vkInstance, extensions);

		return true;
	}

	bool VulkanInstance::EnumeratePhysicalDevices()
	{
		if (m_physicalDevices.size() > 0)
		{
			return true;
		}

		uint32_t devicesCount = 0;
		VkResult result = VK_SUCCESS;

		result = vkEnumeratePhysicalDevices(m_vkInstance, &devicesCount, nullptr);
		if ((result != VK_SUCCESS) ||
			(devicesCount == 0)) {
			std::cout << "Could not get the number of available physical devices." << std::endl;
			return false;
		}

		m_physicalDevices.resize(devicesCount);
		result = vkEnumeratePhysicalDevices(m_vkInstance, &devicesCount, m_physicalDevices.data());
		if ((result != VK_SUCCESS) ||
			(devicesCount == 0)) {
			std::cout << "Could not enumerate physical devices." << std::endl;
			return false;
		}

		return true;
	}


	bool VulkanInstance::CheckExtensions(const std::vector<char const *>& extensions)
	{
		for (auto extension : extensions) {
			if (!vkExtensions.IsExtensionSupported(extension)) {
				std::cout << "Extension named '" << extension << "' is not supported by an Instance object." << std::endl;
				return false;
			}
		}

		return true;
	}
}