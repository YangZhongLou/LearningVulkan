#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <map>

namespace yzl
{
	class VulkanPhysicalDevice;
	class VulkanInstance;
	class VulkanQueue;
	struct SemaphoreInfo;

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
		bool SynchronizeCommandBuffers(VulkanQueue* firstQueue,
			std::vector<SemaphoreInfo*>  firstWaitsemaphoreInfos,
			std::vector<VkCommandBuffer> firstCommandBuffers,
			std::vector<SemaphoreInfo*>  syncSemaphoreInfos,
			VulkanQueue* secondQueue,
			std::vector<VkCommandBuffer> secondCommandBuffers,
			std::vector<VkSemaphore> secondSignalSemaphores,
			VkFence secondFence);
		VulkanPhysicalDevice* GetVulkanPhysicalDevice() { return m_vulkanPhysicalDevice; }
		bool WaitForFences(std::vector<VkFence> const & fences, VkBool32 waitForAll, uint64_t timeout);
		bool WaitIdle();
		bool ResetFences(std::vector<VkFence> const & fences);
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