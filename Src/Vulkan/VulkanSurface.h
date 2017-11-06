#pragma once

#include <vulkan/vulkan.h>

namespace yzl
{
	struct WindowParameters;

	class VulkanSurface
	{
	public:
		VulkanSurface(VkInstance instance, WindowParameters* winParams);
		~VulkanSurface();
	public:
		const VkSurfaceKHR GetSurface() const { return m_surface; }

		uint32_t GetImageCount() { return m_imageCount; }
		bool CheckImageCount();

		VkExtent2D GetImageExtent() { return m_imageExtent; }
		bool CheckImageExtent();

		VkImageUsageFlags GetImageUsage() { return m_imageUsage; }
		bool CheckImageUsage(VkImageUsageFlags usage);

		VkSurfaceTransformFlagBitsKHR GetSurfaceTransform();
		bool CheckSurfaceTransform(VkSurfaceTransformFlagBitsKHR surfaceTransform);

		VkPresentModeKHR GetPresentMode() const { return m_presentMode; }
		bool CheckPresentMode(const VkPhysicalDevice physicalDevice, VkPresentModeKHR desiredMode);

		VkSurfaceFormatKHR GetFormat() const { return m_format; }
		bool CheckFormat(const VkPhysicalDevice physicalDevice, VkSurfaceFormatKHR  desiredSurfaceFormat);

		void EnableDirty() { m_isDirty = true; }
		const VkSurfaceCapabilitiesKHR& GetCapabilities(const VkPhysicalDevice physicalDevice);
	private:
		bool Init(const VkInstance instance, const WindowParameters* winParams);
	private:
		VkSurfaceKHR m_surface;
		VkPresentModeKHR m_presentMode;
		VkSurfaceFormatKHR m_format;
		VkSurfaceTransformFlagBitsKHR m_surfaceTransform;
		VkImageUsageFlags m_imageUsage;
		VkExtent2D m_imageExtent;
		uint32_t m_imageCount;
		bool m_isDirty;
		VkInstance m_instance;
		VkSurfaceCapabilitiesKHR m_capabilities;
	};
}