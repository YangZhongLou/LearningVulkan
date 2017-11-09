#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <array>

namespace yzl
{
	struct BufferTransition;
	struct ImageTransition;

	class VulkanRenderPass;
	class VulkanVertexBuffer;
	class VulkanIndexBuffer;

	class VulkanCommandBuffer
	{
	public:
		VulkanCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferUsageFlags usage, VkCommandBufferInheritanceInfo * secondaryCommandBufferInfo);
		~VulkanCommandBuffer();
	public:
		bool Reset(bool releaseResources);
	private:
		bool Begin(VkCommandBufferBeginInfo& commandBufferBeginInfo);
		bool End();

		void BeginRenderPass(VulkanRenderPass* renderPass,
			VkFramebuffer framebuffer,
			VkRect2D renderArea,
			std::vector<VkClearValue> const & clearValues,
			VkSubpassContents subpassContents);
		
		void NextSubPass(VkSubpassContents subpassContents);

		void EndRenderPass();

		bool AddBarrier(VkPipelineStageFlags generatingStages,
			VkPipelineStageFlags consumingStages,
			std::vector<BufferTransition> bufferTransitions);
		bool AddBarrier(VkPipelineStageFlags generatingStages,
			VkPipelineStageFlags consumingStages,
			std::vector<ImageTransition> transitions);

		void CopyBuffer(VkBuffer sourceBuffer, VkBuffer destinationBuffer, std::vector<VkBufferCopy> regions);
		void CopyBuffer(VkBuffer sourceBuffer, VkImage destinationImage, VkImageLayout imageLayout, std::vector<VkBufferImageCopy> regions);
		void CopyBuffer(VkImage sourceImage, VkBuffer destinationBuffer, VkImageLayout imageLayout, std::vector<VkBufferImageCopy> regions);

		void SetViewport(uint32_t firstViewport,
			std::vector<VkViewport> const & viewports);
		void SetScissor(uint32_t firstScissor,
			std::vector<VkRect2D> const & scissors);
		void SetLineWidth(float lineWidth);
		void SetDepthBias(float constantFactor,
			float clamp,
			float slopeFactor);
		void SetBlendConstants(std::array<float, 4> const & blendConstants);

		void Draw(uint32_t vertexCount,
			uint32_t instanceCount,
			uint32_t firstVertex,
			uint32_t firstInstance);
		void DrawIndexed(uint32_t indexCount,
			uint32_t instanceCount,
			uint32_t firstIndex,
			uint32_t vertexOffset,
			uint32_t firstInstance);
		void Dispatch(uint32_t xSize,
			uint32_t ySize,
			uint32_t zSize);

		void ExecuteSecondaryCommands(std::vector<VkCommandBuffer> const & secondaryCommandBuffers);

		void PushConstants(VkPipelineLayout pipelineLayout,
			VkShaderStageFlags pipelineStages,
			uint32_t offset,
			uint32_t size,
			void * data);

		void BindPipeline(VkPipelineBindPoint pipelineType,
			VkPipeline pipeline);
		void BindDescriptorSets(VkPipelineBindPoint pipelineType,
			VkPipelineLayout pipelineLayout,
			uint32_t indexForFirstSet,
			std::vector<VkDescriptorSet> const & descriptorSets,
			std::vector<uint32_t> const & dynamicOffsets);
		void BindVertexBuffers(uint32_t firstBinding,
			std::vector<VulkanVertexBuffer> const & vertexBuffers);
		void BindIndexBuffer(VulkanIndexBuffer& indexBuffer);

		void ClearColorImage(VkImage image,
			VkImageLayout imageLayout,
			std::vector<VkImageSubresourceRange> const & imageSubresourceRanges,
			VkClearColorValue & clearColor);

		void ClearDepthStencilImage(VkImage image,
			VkImageLayout imageLayout,
			std::vector<VkImageSubresourceRange> const & imageSubresourceRanges,
			VkClearDepthStencilValue & clearValue);

		void ClearRenderPassAttachments(std::vector<VkClearAttachment> const & attachments,
			std::vector<VkClearRect> const       & rects);
	private:
		VkCommandBuffer m_commandBuffer;
	};
}