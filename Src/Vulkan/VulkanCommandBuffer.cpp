#include "VulkanCommandBuffer.h"
#include "VulkanBuffer.h"
#include "VulkanImage.h"
#include "VulkanRenderPass.h"
#include "VulkanIndexBuffer.h"
#include "VulkanVertexBuffer.h"
#include "../Utils.h"

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

	void VulkanCommandBuffer::BeginRenderPass(VulkanRenderPass* renderPass, 
		VkFramebuffer framebuffer,
		VkRect2D renderArea,
		std::vector<VkClearValue> const & clearValues,
		VkSubpassContents subpassContents)
	{
		VkRenderPassBeginInfo renderPassBeginInfo = 
		{
			VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,    
			nullptr,                                     
			renderPass->GetRenderPass(),
			framebuffer,                                 
			renderArea,
			static_cast<uint32_t>(clearValues.size()),
			clearValues.data()
		};

		vkCmdBeginRenderPass(m_commandBuffer, &renderPassBeginInfo, subpassContents);
	}

	void VulkanCommandBuffer::NextSubPass(VkSubpassContents subpassContents)
	{
		vkCmdNextSubpass(m_commandBuffer, subpassContents);
	}

	void VulkanCommandBuffer::EndRenderPass()
	{
		vkCmdEndRenderPass(m_commandBuffer);
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

	bool VulkanCommandBuffer::AddBarrier(VkPipelineStageFlags generatingStages, VkPipelineStageFlags consumingStages, std::vector<ImageTransition> transitions)
	{
		std::vector<VkImageMemoryBarrier> imageMemoryBarriers;
		for (auto & transition : transitions) 
		{
			imageMemoryBarriers.push_back({
				VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,  
				nullptr,                          
				transition.currentAccess,         
				transition.newAccess,             
				transition.currentLayout,         
				transition.newLayout,             
				transition.currentQueueFamily,    
				transition.newQueueFamily,        
				transition.image,                 
				{                                 
					transition.aspect,            
					0,                                   
					VK_REMAINING_MIP_LEVELS,             
					0,                                   
					VK_REMAINING_ARRAY_LAYERS            
				}
			});
		}

		if (imageMemoryBarriers.size() > 0) 
		{
			vkCmdPipelineBarrier(m_commandBuffer, generatingStages, consumingStages, 0, 0, nullptr, 0, nullptr, static_cast<uint32_t>(imageMemoryBarriers.size()), imageMemoryBarriers.data());
		}

		return true;
	}

	void VulkanCommandBuffer::CopyBuffer(VkBuffer sourceBuffer, VkBuffer destinationBuffer, std::vector<VkBufferCopy> regions)
	{
		if (regions.size() > 0) 
		{
			vkCmdCopyBuffer(m_commandBuffer, sourceBuffer, destinationBuffer, static_cast<uint32_t>(regions.size()), regions.data());
		}
	}

	void VulkanCommandBuffer::CopyBuffer(VkBuffer sourceBuffer, VkImage destinationImage, VkImageLayout imageLayout, std::vector<VkBufferImageCopy> regions)
	{
		if (regions.size() > 0) 
		{
			vkCmdCopyBufferToImage(m_commandBuffer, sourceBuffer, destinationImage, imageLayout, static_cast<uint32_t>(regions.size()), regions.data());
		}
	}

	void VulkanCommandBuffer::CopyBuffer(VkImage sourceImage, VkBuffer destinationBuffer, VkImageLayout imageLayout, std::vector<VkBufferImageCopy> regions)
	{
		if (regions.size() > 0) 
		{
			vkCmdCopyImageToBuffer(m_commandBuffer, sourceImage, imageLayout, destinationBuffer, static_cast<uint32_t>(regions.size()), regions.data());
		}
	}

	void VulkanCommandBuffer::SetViewport(uint32_t firstViewport, std::vector<VkViewport> const & viewports)
	{
		vkCmdSetViewport(m_commandBuffer, firstViewport, static_cast<uint32_t>(viewports.size()), viewports.data());
	}

	void VulkanCommandBuffer::SetScissor(uint32_t firstScissor, std::vector<VkRect2D> const & scissors)
	{
		vkCmdSetScissor(m_commandBuffer, firstScissor, static_cast<uint32_t>(scissors.size()), scissors.data());
	}

	void VulkanCommandBuffer::SetLineWidth(float lineWidth)
	{
		vkCmdSetLineWidth(m_commandBuffer, lineWidth);
	}

	void VulkanCommandBuffer::SetDepthBias(float constantFactor, float clamp, float slopeFactor)
	{
		vkCmdSetDepthBias(m_commandBuffer, constantFactor, clamp, slopeFactor);
	}

	void VulkanCommandBuffer::SetBlendConstants(std::array<float, 4> const & blendConstants)
	{
		vkCmdSetBlendConstants(m_commandBuffer, blendConstants.data());
	}

	void VulkanCommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
	{
		vkCmdDraw(m_commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void VulkanCommandBuffer::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance)
	{
		vkCmdDrawIndexed(m_commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}

	void VulkanCommandBuffer::Dispatch(uint32_t xSize, uint32_t ySize, uint32_t zSize)
	{
		vkCmdDispatch(m_commandBuffer, xSize, ySize, zSize);
	}

	void VulkanCommandBuffer::ExecuteSecondaryCommands(std::vector<VkCommandBuffer> const & secondaryCommandBuffers)
	{
		if (secondaryCommandBuffers.size() > 0)
		{
			vkCmdExecuteCommands(m_commandBuffer, static_cast<uint32_t>(secondaryCommandBuffers.size()), secondaryCommandBuffers.data());
		}
	}

	void VulkanCommandBuffer::PushConstants(VkPipelineLayout pipelineLayout, VkShaderStageFlags pipelineStages, uint32_t offset, uint32_t size, void * data)
	{
		vkCmdPushConstants(m_commandBuffer, pipelineLayout, pipelineStages, offset, size, data);
	}

	void VulkanCommandBuffer::BindPipeline(VkPipelineBindPoint pipelineType, VkPipeline pipeline)
	{
		vkCmdBindPipeline(m_commandBuffer, pipelineType, pipeline);
	}

	void VulkanCommandBuffer::BindDescriptorSets(VkPipelineBindPoint pipelineType,
			VkPipelineLayout pipelineLayout, 
			uint32_t indexForFirstSet, 
			std::vector<VkDescriptorSet> const & descriptorSets, 
			std::vector<uint32_t> const & dynamicOffsets)
	{
		vkCmdBindDescriptorSets(m_commandBuffer, pipelineType, pipelineLayout, indexForFirstSet,
			static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(),
			static_cast<uint32_t>(dynamicOffsets.size()), dynamicOffsets.data());
	}

	void VulkanCommandBuffer::BindVertexBuffers(uint32_t firstBinding, std::vector<VulkanVertexBuffer> const & vertexBuffers)
	{
		if (vertexBuffers.size() > 0)
		{
			std::vector<VkBuffer> buffers;
			std::vector<VkDeviceSize> offsets;

			for (auto & vertexBuffer : vertexBuffers) 
			{
				buffers.push_back(vertexBuffer.GetBuffer()->GetBuffer());
				offsets.push_back(vertexBuffer.GetMemoryOffset());
			}

			vkCmdBindVertexBuffers(m_commandBuffer, firstBinding, static_cast<uint32_t>(vertexBuffers.size()), buffers.data(), offsets.data());
		}
	}

	void VulkanCommandBuffer::BindIndexBuffer(VulkanIndexBuffer & indexBuffer)
	{
		vkCmdBindIndexBuffer(m_commandBuffer, indexBuffer.GetBuffer()->GetBuffer(), indexBuffer.GetMemoryOffset(), indexBuffer.GetIndexType());
	}

	void VulkanCommandBuffer::ClearColorImage(VkImage image, 
		VkImageLayout imageLayout, 
		std::vector<VkImageSubresourceRange> const & imageSubresourceRanges, 
		VkClearColorValue & clearColor)
	{
		vkCmdClearColorImage(m_commandBuffer, image, imageLayout, &clearColor, static_cast<uint32_t>(imageSubresourceRanges.size()), imageSubresourceRanges.data());
	}

	void VulkanCommandBuffer::ClearDepthStencilImage(VkImage image, 
		VkImageLayout imageLayout, 
		std::vector<VkImageSubresourceRange> const & imageSubresourceRanges, 
		VkClearDepthStencilValue & clearValue)
	{
		vkCmdClearDepthStencilImage(m_commandBuffer, image, imageLayout, &clearValue, static_cast<uint32_t>(imageSubresourceRanges.size()), imageSubresourceRanges.data());
	}

	void VulkanCommandBuffer::ClearRenderPassAttachments(std::vector<VkClearAttachment> const & attachments, 
		std::vector<VkClearRect> const & rects)
	{
		vkCmdClearAttachments(m_commandBuffer, static_cast<uint32_t>(attachments.size()), attachments.data(), static_cast<uint32_t>(rects.size()), rects.data());
	}
}

