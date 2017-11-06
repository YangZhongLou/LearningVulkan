#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace yzl
{
	class VulkanDevice;

	class VulkanPipelineLayout
	{
	public:
		VulkanPipelineLayout(VulkanDevice * device,
			std::vector<VkDescriptorSetLayout> const & descriptorSetLayouts,
			std::vector<VkPushConstantRange> const & pushConstantRanges);
		~VulkanPipelineLayout();
	private:
		bool Init(VulkanDevice * device,
			std::vector<VkDescriptorSetLayout> const & descriptorSetLayouts,
			std::vector<VkPushConstantRange> const & pushConstantRanges);
	private:
		VulkanDevice * m_device;
		VkPipelineLayout m_pipelineLayout;
	};
}