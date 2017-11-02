#include "VulkanCommandBuffer.h"
#include "VulkanBuffer.h"
#include "Utils.h"

namespace yzl
{
	VulkanCommandBuffer::VulkanCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferUsageFlags usage, VkCommandBufferInheritanceInfo * secondary_command_buffer_info)
	{
		VkCommandBufferBeginInfo commandBufferBeginInfo = {
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,    // VkStructureType                        sType
			nullptr,                                        // const void                           * pNext
			usage,                                          // VkCommandBufferUsageFlags              flags
			secondary_command_buffer_info                   // const VkCommandBufferInheritanceInfo * pInheritanceInfo
		};

		m_commandBuffer = commandBuffer;
		Begin(commandBufferBeginInfo);
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
		End();
		m_commandBuffer = VK_NULL_HANDLE;
	}

	bool VulkanCommandBuffer::Reset(bool releaseResources)
	{
		VkResult result = vkResetCommandBuffer(m_commandBuffer, releaseResources ? VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT : 0);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Error occurred during command buffer reset." << std::endl;
			return false;
		}
		return true;
	}

	bool VulkanCommandBuffer::Begin(VkCommandBufferBeginInfo& commandBufferBeginInfo)
	{
		VkResult result = vkBeginCommandBuffer(m_commandBuffer, &commandBufferBeginInfo);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not begin command buffer recording operation." << std::endl;
			return false;
		}
		return true;
	}

	bool VulkanCommandBuffer::End()
	{
		VkResult result = vkEndCommandBuffer(m_commandBuffer);
		if (VK_SUCCESS != result)
		{
			std::cout << "Error occurred during command buffer recording." << std::endl;
			return false;
		}
		return true;
	}

	bool VulkanCommandBuffer::AddBarrier(VkPipelineStageFlags generatingStages, VkPipelineStageFlags consumingStages, std::vector<BufferTransition> bufferTransitions)
	{
		std::vector<VkBufferMemoryBarrier> bufferMemoryBarriers;

		for (auto & transition : bufferTransitions) 
		{
			bufferMemoryBarriers.push_back(
			{
				VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,   
				nullptr,                                   
				transition.currentAccess,           
				transition.newAccess,               
				transition.currentQueueFamily,      
				transition.newQueueFamily,          
				transition.buffer,                  
				0,                                         
				VK_WHOLE_SIZE                              
			});
		}

		if (bufferMemoryBarriers.size() > 0) {
			vkCmdPipelineBarrier(m_commandBuffer, generatingStages, consumingStages, 0, 0, nullptr, static_cast<uint32_t>(bufferMemoryBarriers.size()), bufferMemoryBarriers.data(), 0, nullptr);
		}

		return true;
	}
}

