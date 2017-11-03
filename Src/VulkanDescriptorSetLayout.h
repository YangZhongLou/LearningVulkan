#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace yzl
{
	class VulkanDevice;

	class VulkanDescriptorSetLayout
	{
	public:
		VulkanDescriptorSetLayout(VulkanDevice* device,
			std::vector<VkDescriptorSetLayoutBinding> const & bindings);
		~VulkanDescriptorSetLayout();
	private:
		bool Init(VulkanDevice* device,
			std::vector<VkDescriptorSetLayoutBinding> const & bindings);
	private:
		VulkanDevice* m_device;
		VkDescriptorSetLayout m_descriptorSetLayout;
	};
}