#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace yzl
{
	struct SubpassParameters 
	{
		VkPipelineBindPoint                  pipelineType;
		std::vector<VkAttachmentReference>   inputAttachments;
		std::vector<VkAttachmentReference>   colorAttachments;
		std::vector<VkAttachmentReference>   resolveAttachments;
		VkAttachmentReference const        * depthStencilAttachment;
		std::vector<uint32_t>                preserveAttachments;
	};

	class VulkanDevice;

	class VulkanRenderPass
	{
	public:
		VulkanRenderPass(VulkanDevice * device,
			std::vector<VkAttachmentDescription> const & attachmentsDescriptions,
			std::vector<SubpassParameters> const & subpassParameters,
			std::vector<VkSubpassDependency> const & subpassDependencies);
		~VulkanRenderPass();
	public:
		VkRenderPass GetRenderPass() const { return m_renderPass; }
	private:
		bool Init(VulkanDevice * device,
			std::vector<VkAttachmentDescription> const & attachmentsDescriptions,
			std::vector<SubpassParameters> const & subpassParameters,
			std::vector<VkSubpassDependency> const & subpassDependencies);
	private:
		VulkanDevice * m_device;
		VkRenderPass m_renderPass;
	};
}