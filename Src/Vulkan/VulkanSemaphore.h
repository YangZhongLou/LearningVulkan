#pragma once

#include <vulkan/vulkan.h>

namespace yzl
{
	class VulkanDevice;
	typedef struct SemaphoreInfo
	{
		VkSemaphore           semaphore;
		VkPipelineStageFlags  waitingStage;
	}SemaphoreInfo;

	class VulkanSemaphore
	{
	public:
		VulkanSemaphore(VulkanDevice* device);
		~VulkanSemaphore();
	public:
		bool Init(VulkanDevice* device);
	private:
		VulkanDevice* m_device;
		VkSemaphore m_semaphore;
	};
}