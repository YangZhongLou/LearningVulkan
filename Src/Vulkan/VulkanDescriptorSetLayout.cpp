#include "VulkanDescriptorSetLayout.h"
#include "VulkanDevice.h"
#include "../Utils.h"

namespace yzl
{
	VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(VulkanDevice * device, 
		std::vector<VkDescriptorSetLayoutBinding> const & bindings)
	{
		Init(device, bindings);
	}

	VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
	{
		if (VK_NULL_HANDLE != m_descriptorSetLayout) 
		{
			vkDestroyDescriptorSetLayout(m_device->GetDevice(), m_descriptorSetLayout, nullptr);
			m_descriptorSetLayout = VK_NULL_HANDLE;
		}
	}

	bool VulkanDescriptorSetLayout::Init(VulkanDevice * device, std::vector<VkDescriptorSetLayoutBinding> const & bindings)
	{
		m_device = device;
		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo =
		{
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			nullptr,
			0,
			static_cast<uint32_t>(bindings.size()),
			bindings.data()
		};

		VkResult result = vkCreateDescriptorSetLayout(device->GetDevice(), &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not create a layout for descriptor sets." << std::endl;
			return false;
		}
		return true;
	}
}