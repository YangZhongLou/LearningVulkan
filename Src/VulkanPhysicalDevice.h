#pragma once

#include "VulkanFunctions.h"
#include <vector>

namespace yzl
{
	class VulkanPhysicalDevice
	{
	public:
		VulkanPhysicalDevice(VkPhysicalDevice physicalDevice);
		~VulkanPhysicalDevice();
	public:
		VkPhysicalDevice GetDevice() { return m_physicalDevice; }

		const VkPhysicalDeviceFeatures& GetFeatures() 
		{
			if (m_isDirty)
			{
				vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_features);
				m_isDirty = false;
				return m_features;
			}
			else
			{
				return m_features;
			}
		}

		const VkFormatProperties& GetFormatProperties(VkFormat format)
		{
			if (m_isDirty)
			{
				m_isDirty = false;
				vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &m_formatProperties);
				return m_formatProperties;
			}
			else
			{
				m_formatProperties;
			}
		}

		const VkPhysicalDeviceProperties& GetProperties() 
		{
			if (m_isDirty)
			{
				vkGetPhysicalDeviceProperties(m_physicalDevice, &m_properties);
				m_isDirty = false;
				return m_properties;
			}
			else
			{
				return m_properties;
			}
		}

		void EnableDirty()
		{
			m_isDirty = true;
		}

		bool CheckQueueFamilyProperties();
		std::vector<VkQueueFamilyProperties>& GetQueueFamilyProperties();

		bool GetSupportSurfaceInfo(VkSurfaceKHR  presentationSurface, uint32_t& queueFamilyIndex);

		/*
		* return, -1 means failure
		*/
		uint32_t GetCapabilitiesFamilyIndex(VkQueueFlags capabilities);
	private:
		VkPhysicalDevice m_physicalDevice;
		/* refine this */
		bool m_isDirty;
		VkPhysicalDeviceFeatures m_features;
		VkPhysicalDeviceProperties m_properties;
		VkFormatProperties m_formatProperties;
		std::vector<VkQueueFamilyProperties> m_queueFamilyProperties;
	};
}