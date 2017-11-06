#include "../Utils.h"
#include "VulkanDevice.h"
#include "VulkanPipelineLayout.h"

namespace yzl
{
	VulkanPipelineLayout::VulkanPipelineLayout(VulkanDevice * device, std::vector<VkDescriptorSetLayout> const & descriptorSetLayouts, std::vector<VkPushConstantRange> const & pushConstantRanges)
	{
		Init(device, descriptorSetLayouts, pushConstantRanges);
	}

	VulkanPipelineLayout::~VulkanPipelineLayout()
	{
		if (VK_NULL_HANDLE != m_pipelineLayout) 
		{
			vkDestroyPipelineLayout(m_device->GetDevice(), m_pipelineLayout, nullptr);
			m_pipelineLayout = VK_NULL_HANDLE;
		}
	}

	bool VulkanPipelineLayout::Init(VulkanDevice * device, 
		std::vector<VkDescriptorSetLayout> const & descriptorSetLayouts, 
		std::vector<VkPushConstantRange> const & pushConstantRanges)
	{
		m_device = device;

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = 
		{
			VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,          
			nullptr,                                                
			0,                                                      
			static_cast<uint32_t>(descriptorSetLayouts.size()),
			descriptorSetLayouts.data(),
			static_cast<uint32_t>(pushConstantRanges.size()),
			pushConstantRanges.data()
		};

		VkResult result = vkCreatePipelineLayout(m_device->GetDevice(), &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout);

		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not create pipeline layout." << std::endl;
			return false;
		}
		return true;
	}
}