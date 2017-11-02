#pragma once

#include <vulkan/vulkan.h>

namespace yzl
{
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
	private:
		VkCommandBuffer m_commandBuffer;
	};
}