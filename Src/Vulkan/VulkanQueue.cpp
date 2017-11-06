#include "VulkanQueue.h"
#include "../Utils.h"
#include "VulkanSemaphore.h"
#include "VulkanDevice.h"

namespace yzl
{
	VulkanQueue::VulkanQueue(VulkanDevice * device, VkQueue queue)
	{
		m_device = device;
		m_queue = queue;
	}

	VulkanQueue::~VulkanQueue()
	{
		m_device = VK_NULL_HANDLE;
		m_queue = VK_NULL_HANDLE;
	}

	bool VulkanQueue::Submit(std::vector<SemaphoreInfo*>& semaphoreInfos, std::vector<VkCommandBuffer>& commandBuffers, std::vector<VkSemaphore>& signalSemaphores, VkFence fence)
	{
		std::vector<VkSemaphore>          semaphores;
		std::vector<VkPipelineStageFlags> semaphoreStages;

		for (auto & info : semaphoreInfos) {
			semaphores.emplace_back(info->semaphore);
			semaphoreStages.emplace_back(info->waitingStage);
		}

		VkSubmitInfo submitInfo = {
			VK_STRUCTURE_TYPE_SUBMIT_INFO,                 
			nullptr,                                       
			static_cast<uint32_t>(semaphores.size()),
			semaphores.data(),                     
			semaphoreStages.data(),                        
			static_cast<uint32_t>(commandBuffers.size()),  
			commandBuffers.data(),                         
			static_cast<uint32_t>(signalSemaphores.size()),
			signalSemaphores.data()                        
		};

		VkResult result = vkQueueSubmit(m_queue, 1, &submitInfo, fence);
		if (VK_SUCCESS != result)
		{
			std::cout << "Error occurred during command buffer submission." << std::endl;
			return false;
		}

		return true;
	}

	bool VulkanQueue::IsFininshed(std::vector<SemaphoreInfo*>& semaphoreInfos, std::vector<VkCommandBuffer>& commandBuffers, std::vector<VkSemaphore>& signalSemaphores, VkFence fence, uint64_t timeout)
	{
		if (!Submit(semaphoreInfos, commandBuffers, signalSemaphores, fence))
		{
			return false;
		}

		return m_device->WaitForFences({ fence }, VK_FALSE, timeout);
	}

	bool VulkanQueue::WaitIdle()
	{
		VkResult result = vkQueueWaitIdle(m_queue);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Waiting for all operations submitted to queue failed." << std::endl;
			return false;
		}
		return true;
	}
}