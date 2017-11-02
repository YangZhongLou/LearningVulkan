#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace yzl
{
	struct BufferTransition;

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
	private:
		VkCommandBuffer m_commandBuffer;
	};
}