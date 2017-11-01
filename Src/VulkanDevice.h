#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <map>

namespace yzl
{
	class VulkanPhysicalDevice;
	class VulkanInstance;

	typedef struct QueueInfo
	{
		uint32_t           familyIndex;
		std::vector<float> priorities;
	}QueueInfo;

	class VulkanDevice
	{
	public:
		VulkanDevice(VulkanInstance* vkInstance, std::vector<VkPhysicalDevice>& physicalDevices, 
			std::vector<char const *> const & desiredExtensions);
		~VulkanDevice();
	public:
		VkDevice GetDevice() { return m_device; }
		VulkanPhysicalDevice* GetVulkanPhysicalDevice() { return m_vulkanPhysicalDevice; }
	private:
		bool Init(const VulkanInstance* vkInstance, const std::vector<VkPhysicalDevice>& physicalDevices,
				std::vector<char const *> const & desiredExtensions);
	private:
		VulkanPhysicalDevice* m_vulkanPhysicalDevice;
		VkDevice m_device;
		VkQueue m_graphicsQueue;
		VkQueue m_computeQueue;
	};
}