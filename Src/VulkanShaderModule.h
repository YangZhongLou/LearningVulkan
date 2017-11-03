#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace yzl
{
	class VulkanDevice;

	class VulkanShaderModule
	{
	public:
		VulkanShaderModule(VulkanDevice * device,
			std::vector<unsigned char> const & sourceCode);
		~VulkanShaderModule();
	private:
		bool Init(VulkanDevice * device,
			std::vector<unsigned char> const & sourceCode);
	private:
		VulkanDevice * m_device;
		VkShaderModule m_shaderModule;
	};
}