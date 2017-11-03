#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace yzl
{
	struct BufferTransition;
	struct ImageTransition;

	class VulkanCommandBuffer
	{
	public:
		VulkanCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferUsageFlags usage, VkCommandBufferInheritanceInfo * secondary_command_buffer_info);
		~VulkanCommandBuffer();
	public:
		bool Reset(bool releaseResources);
	private:
		bool Begin(VkCommandBufferBeginInfo& commandBufferBeginInfo);
		bool End();
		bool AddBarrier(VkPipelineStageFlags generatingStages,
			VkPipelineStageFlags consumingStages,
			std::vector<BufferTransition> bufferTransitions);
		bool AddBarrier(VkPipelineStageFlags generatingStages,
			VkPipelineStageFlags consumingStages,
			std::vector<ImageTransition> transitions);

		void CopyBuffer(VkBuffer sourceBuffer, VkBuffer destinationBuffer, std::vector<VkBufferCopy> regions);
		void CopyBuffer(VkBuffer sourceBuffer, VkImage destinationImage, VkImageLayout imageLayout, std::vector<VkBufferImageCopy> regions);
		void CopyBuffer(VkImage sourceImage, VkBuffer destinationBuffer, VkImageLayout imageLayout, std::vector<VkBufferImageCopy> regions);

	private:
		VkCommandBuffer m_commandBuffer;
	};
}