#include "VulkanRenderPass.h"
#include "VulkanDevice.h"
#include "../Utils.h"

namespace yzl
{
	VulkanRenderPass::VulkanRenderPass(VulkanDevice * device,
		std::vector<VkAttachmentDescription> const & attachmentsDescriptions,
		std::vector<SubpassParameters> const & subpassParameters,
		std::vector<VkSubpassDependency> const & subpassDependencies)
	{
		Init(device, attachmentsDescriptions, subpassParameters, subpassDependencies);
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
		if (VK_NULL_HANDLE != m_renderPass) 
		{
			vkDestroyRenderPass(m_device->GetDevice(), m_renderPass, nullptr);
			m_renderPass = VK_NULL_HANDLE;
		}
	}

	static void SpecifySubpassDescriptions(std::vector<SubpassParameters> const & subpassParameters,
		std::vector<VkSubpassDescription> & subpassDescriptions) {
		subpassDescriptions.clear();

		for (auto & subpassDescription : subpassParameters) 
		{
			subpassDescriptions.push_back({
				0,                                                                   
				subpassDescription.pipelineType,                                     
				static_cast<uint32_t>(subpassDescription.inputAttachments.size()),   
				subpassDescription.inputAttachments.data(),                          
				static_cast<uint32_t>(subpassDescription.colorAttachments.size()),   
				subpassDescription.colorAttachments.data(),                          
				subpassDescription.resolveAttachments.data(),                        
				subpassDescription.depthStencilAttachment,                           
				static_cast<uint32_t>(subpassDescription.preserveAttachments.size()),
				subpassDescription.preserveAttachments.data()                        
			});
		}
	}

	bool VulkanRenderPass::Init(VulkanDevice * device, 
		std::vector<VkAttachmentDescription> const & attachmentsDescriptions, 
		std::vector<SubpassParameters> const & subpassParameters, 
		std::vector<VkSubpassDependency> const & subpassDependencies)
	{
		m_device = device;

		std::vector<VkSubpassDescription> subpassDescriptions;
		SpecifySubpassDescriptions(subpassParameters, subpassDescriptions);

		VkRenderPassCreateInfo renderPassCreateInfo = {
			VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,                // VkStructureType                    sType
			nullptr,                                                  // const void                       * pNext
			0,                                                        // VkRenderPassCreateFlags            flags
			static_cast<uint32_t>(attachmentsDescriptions.size()),   // uint32_t                           attachmentCount
			attachmentsDescriptions.data(),                          // const VkAttachmentDescription    * pAttachments
			static_cast<uint32_t>(subpassDescriptions.size()),       // uint32_t                           subpassCount
			subpassDescriptions.data(),                              // const VkSubpassDescription       * pSubpasses
			static_cast<uint32_t>(subpassDependencies.size()),       // uint32_t                           dependencyCount
			subpassDependencies.data()                               // const VkSubpassDependency        * pDependencies
		};

		VkResult result = vkCreateRenderPass(m_device->GetDevice(), &renderPassCreateInfo, nullptr, &m_renderPass);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not create a render pass." << std::endl;
			return false;
		}
		return true;
	}
}