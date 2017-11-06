
#include "VulkanSurface.h"
#include "../WindowParameters.h"
#include "../Utils.h"

namespace yzl
{
	VulkanSurface::VulkanSurface(VkInstance instance, WindowParameters* winParams)
	{
		memset(&m_capabilities, 0, sizeof(VkSurfaceCapabilitiesKHR));
		Init(instance, winParams);
	}

	VulkanSurface::~VulkanSurface()
	{
		if (m_surface) 
		{
			vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
			m_surface = VK_NULL_HANDLE;
		}
	}

	bool VulkanSurface::CheckImageCount()
	{
		m_imageCount = m_capabilities.minImageCount + 1;
		if ((m_capabilities.maxImageCount > 0) &&
			(m_imageCount > m_capabilities.maxImageCount)) 
		{
			m_imageCount = m_capabilities.maxImageCount;
		}
		return true;
	}

	bool VulkanSurface::CheckImageExtent()
	{
		if (0xFFFFFFFF == m_capabilities.currentExtent.width) 
		{
			m_imageExtent = { 640, 480 };

			if (m_imageExtent.width < m_capabilities.minImageExtent.width) 
			{
				m_imageExtent.width = m_capabilities.minImageExtent.width;
			}
			else if (m_imageExtent.width > m_capabilities.maxImageExtent.width) 
			{
				m_imageExtent.width = m_capabilities.maxImageExtent.width;
			}

			if (m_imageExtent.height < m_capabilities.minImageExtent.height) 
			{
				m_imageExtent.height = m_capabilities.minImageExtent.height;
			}
			else if (m_imageExtent.height > m_capabilities.maxImageExtent.height) 
			{
				m_imageExtent.height = m_capabilities.maxImageExtent.height;
			}
		}
		else 
		{
			m_imageExtent = m_capabilities.currentExtent;
		}
		return true;
	}

	bool VulkanSurface::CheckImageUsage(VkImageUsageFlags usage)
	{
		m_imageUsage = usage & m_capabilities.supportedUsageFlags;

		return usage == m_imageUsage;
	}

	VkSurfaceTransformFlagBitsKHR VulkanSurface::GetSurfaceTransform()
	{
		return m_surfaceTransform;
	}

	bool VulkanSurface::CheckSurfaceTransform(VkSurfaceTransformFlagBitsKHR surfaceTransform)
	{
		if (m_capabilities.supportedTransforms & surfaceTransform)
		{
			m_surfaceTransform = surfaceTransform;
		}
		else
		{
			m_surfaceTransform = m_capabilities.currentTransform;
		}
		return true;
	}

	bool VulkanSurface::CheckPresentMode(const VkPhysicalDevice physicalDevice, VkPresentModeKHR desiredMode)
	{
		uint32_t presentModesCount = 0;
		VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_surface, &presentModesCount, nullptr);
		if ((VK_SUCCESS != result) ||
			(0 == presentModesCount)) 
		{
			std::cout << "Could not get the number of supported present modes." << std::endl;
			m_presentMode = VK_PRESENT_MODE_MAX_ENUM_KHR;
			return false;
		}

		std::vector<VkPresentModeKHR> presentModes(presentModesCount);
		result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_surface, &presentModesCount, presentModes.data());
		if ((VK_SUCCESS != result) ||
			(0 == presentModesCount)) 
		{
			std::cout << "Could not enumerate present modes." << std::endl;
			m_presentMode = VK_PRESENT_MODE_MAX_ENUM_KHR;
			return false;
		}

		for (auto& mode : presentModes) 
		{
			if (mode == desiredMode) 
			{
				m_presentMode = desiredMode;
				return true;
			}
		}

		std::cout << "Desired present mode is not supported. Selecting default FIFO mode." << std::endl;
		for (auto& mode : presentModes) 
		{
			if (mode == VK_PRESENT_MODE_FIFO_KHR) 
			{
				m_presentMode = VK_PRESENT_MODE_FIFO_KHR;
				return true;
			}
		}

		std::cout << "VK_PRESENT_MODE_FIFO_KHR is not supported though it's mandatory for all drivers!" << std::endl;
		m_presentMode = VK_PRESENT_MODE_MAX_ENUM_KHR;
		return false;
	}

	bool VulkanSurface::CheckFormat(const VkPhysicalDevice physicalDevice, VkSurfaceFormatKHR desiredSurfaceFormat)
	{
		uint32_t formatsCount = 0;
		m_format = { VK_FORMAT_MAX_ENUM, VK_COLOR_SPACE_MAX_ENUM_KHR };

		VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_surface, &formatsCount, nullptr);
		if ((VK_SUCCESS != result) ||
			(0 == formatsCount)) {
			std::cout << "Could not get the number of supported surface formats." << std::endl;
			return false;
		}

		std::vector<VkSurfaceFormatKHR> surfaceFormats(formatsCount);
		result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_surface, &formatsCount, surfaceFormats.data());
		if ((VK_SUCCESS != result) ||
			(0 == formatsCount)) {
			std::cout << "Could not enumerate supported surface formats." << std::endl;
			return false;
		}

		if ((1 == surfaceFormats.size()) &&
			(VK_FORMAT_UNDEFINED == surfaceFormats[0].format)) {
			m_format.format = desiredSurfaceFormat.format;
			m_format.colorSpace = desiredSurfaceFormat.colorSpace;
			return true;
		}

		for (auto & oneSurfaceFormat : surfaceFormats) {
			if ((desiredSurfaceFormat.format == oneSurfaceFormat.format) &&
				(desiredSurfaceFormat.colorSpace == oneSurfaceFormat.colorSpace)) {
				m_format.format = desiredSurfaceFormat.format;
				m_format.colorSpace = desiredSurfaceFormat.colorSpace;
				return true;
			}
		}

		for (auto & oneSurfaceFormat : surfaceFormats) {
			if ((desiredSurfaceFormat.format == oneSurfaceFormat.format)) {
				std::cout << "Desired combination of format and colorspace is not supported. Selecting other colorspace." << std::endl;
				m_format.format = desiredSurfaceFormat.format;
				m_format.colorSpace = desiredSurfaceFormat.colorSpace;
				return true;
			}
		}

		m_format.format = desiredSurfaceFormat.format;
		m_format.colorSpace = desiredSurfaceFormat.colorSpace;
		std::cout << "Desired format is not supported. Selecting available format - colorspace combination." << std::endl;
		return true;
	}

	const VkSurfaceCapabilitiesKHR& VulkanSurface::GetCapabilities(const VkPhysicalDevice physicalDevice)
	{
		if (!m_isDirty)
		{
			return m_capabilities;
		}

		VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_surface, &m_capabilities);
		if (VK_SUCCESS != result) {
			std::cout << "Could not get the capabilities of a presentation surface." << std::endl;
			return m_capabilities;
		}

		m_isDirty = false;
		return m_capabilities;
	}

	bool VulkanSurface::Init(const VkInstance instance, const WindowParameters* winParams)
	{
		m_instance = instance;
#ifdef VK_USE_PLATFORM_WIN32_KHR
		VkWin32SurfaceCreateInfoKHR surface_create_info = {
			VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			winParams->hinstance,
			winParams->hwnd,
		};

		VkResult result = vkCreateWin32SurfaceKHR(instance, &surface_create_info, nullptr, &m_surface);
#endif

		if ((VK_SUCCESS != result) ||
			(VK_NULL_HANDLE == m_surface)) {
			std::cout << "Could not create presentation surface." << std::endl;
			return false;
		}

		return true;
	}


}