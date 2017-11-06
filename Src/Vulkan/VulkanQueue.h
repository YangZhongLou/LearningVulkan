#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace yzl
{
	struct SemaphoreInfo;
	class VulkanDevice;

	class VulkanQueue
	{
	public:
		VulkanQueue(VulkanDevice* device, VkQueue queue, uint32_t familyIndex);
		~VulkanQueue();
	public:
		uint32_t GetFamilyIndex() const { return m_familyIndex; }
		bool Submit(std::vector<SemaphoreInfo*>& semaphoreInfos,
			std::vector<VkCommandBuffer>& commandBuffers,
			std::vector<VkSemaphore>& signalSemaphores,
			VkFence fence);

		bool IsFininshed(std::vector<SemaphoreInfo*>& semaphoreInfos,
			std::vector<VkCommandBuffer>& commandBuffers,
			std::vector<VkSemaphore>& signalSemaphores,
			VkFence fence, uint64_t timeout);
		bool WaitIdle();
	private:
		VulkanDevice* m_device;
		VkQueue m_queue;
		uint32_t m_familyIndex;
	};
}