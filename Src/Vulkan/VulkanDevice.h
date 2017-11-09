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
	class VulkanCommandPool;

	struct QueueInfo
	{
		uint32_t familyIndex;
		std::vector<float> priorities;
	};

	class VulkanDevice
	{
	public:
		VulkanDevice(VulkanInstance* vkInstance, std::vector<VkPhysicalDevice>& physicalDevices, 
			std::vector<char const *> const & desiredExtensions);
		~VulkanDevice();
	public:
		VulkanCommandPool * GetCommandPool() { return m_commandPool; }
		VkDevice GetDevice() { return m_device; }
		bool SynchronizeCommandBuffers(VulkanQueue * firstQueue,
			std::vector<SemaphoreInfo*>  firstWaitsemaphoreInfos,
			std::vector<VkCommandBuffer> firstCommandBuffers,
			std::vector<SemaphoreInfo*>  syncSemaphoreInfos,
			VulkanQueue * secondQueue,
			std::vector<VkCommandBuffer> secondCommandBuffers,
			std::vector<VkSemaphore> secondSignalSemaphores,
			VkFence secondFence);
		VulkanPhysicalDevice* GetVulkanPhysicalDevice() { return m_vulkanPhysicalDevice; }
		bool WaitForFences(std::vector<VkFence> const & fences, VkBool32 waitForAll, uint64_t timeout);
		bool WaitIdle();
		bool ResetFences(std::vector<VkFence> const & fences);

		VulkanQueue * GetGraphicsQueue() const { return m_graphicsQueue; }
		VulkanQueue * GetComputeQueue() const { return m_computeQueue; }

	private:
		bool SelectPhysicalDevice(const VulkanInstance* vkInstance, const std::vector<VkPhysicalDevice>& physicalDevices,
				std::vector<char const *> const & desiredExtensions);
		bool AllocateCommandPool(VkCommandPoolCreateFlags flags, uint32_t queueFamilyIndex);
	private:
		VulkanPhysicalDevice * m_vulkanPhysicalDevice;
		VkDevice m_device;
		VulkanQueue * m_graphicsQueue;
		VulkanQueue * m_computeQueue;
		VulkanCommandPool * m_commandPool;
	};
}