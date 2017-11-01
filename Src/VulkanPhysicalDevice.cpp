#include "VulkanPhysicalDevice.h"
#include <iostream>

namespace yzl
{
	VulkanPhysicalDevice::VulkanPhysicalDevice(VkPhysicalDevice physicalDevice)
	{
		m_physicalDevice = physicalDevice;
		m_isDirty = true;
	}

	VulkanPhysicalDevice::~VulkanPhysicalDevice()
	{}

	bool VulkanPhysicalDevice::CheckQueueFamilyProperties()
	{
		GetQueueFamilyProperties();
		return m_queueFamilyProperties.size() > 0;
	}

	std::vector<VkQueueFamilyProperties>& VulkanPhysicalDevice::GetQueueFamilyProperties()
	{
		if (!m_isDirty)
			return m_queueFamilyProperties;

		uint32_t queueFamiliesCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamiliesCount, nullptr);
		if (queueFamiliesCount == 0) {
			std::cout << "Could not get the number of queue families." << std::endl;
			m_queueFamilyProperties.resize(0);
			return m_queueFamilyProperties;
		}

		m_queueFamilyProperties.resize(queueFamiliesCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamiliesCount, m_queueFamilyProperties.data());
		if (queueFamiliesCount == 0) {
			std::cout << "Could not acquire properties of queue families." << std::endl;
			m_queueFamilyProperties.resize(0);
			return m_queueFamilyProperties;
		}

		return m_queueFamilyProperties;
	}

	bool VulkanPhysicalDevice::GetSupportSurfaceInfo(VkSurfaceKHR presentationSurface, uint32_t & queueFamilyIndex)
	{
		if (!CheckQueueFamilyProperties())
		{
			return false;
		}
	
		for (uint32_t index = 0; index < static_cast<uint32_t>(m_queueFamilyProperties.size()); ++index) {
			VkBool32 supported = VK_FALSE;
			VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, index, presentationSurface, &supported);
			if ((VK_SUCCESS == result) &&
				(VK_TRUE == supported))
			{
				queueFamilyIndex = index;
				return true;
			}
		}
		return false;
	}

	uint32_t VulkanPhysicalDevice::GetCapabilitiesFamilyIndex(VkQueueFlags capabilities)
	{
		GetQueueFamilyProperties();
		if (m_queueFamilyProperties.size() == 0)
		{
			return -1;
		}

		for (uint32_t index = 0; index < static_cast<uint32_t>(m_queueFamilyProperties.size()); ++index)
		{
			if ((m_queueFamilyProperties[index].queueCount > 0) &&
				(m_queueFamilyProperties[index].queueFlags & capabilities))
			{
				return index;
			}
		}

		return -1;
	}
}