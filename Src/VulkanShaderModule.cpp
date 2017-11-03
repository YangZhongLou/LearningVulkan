#include "VulkanShaderModule.h"
#include "Utils.h"
#include "VulkanDevice.h"


namespace yzl
{
	VulkanShaderModule::VulkanShaderModule(VulkanDevice * device, std::vector<unsigned char> const & sourceCode)
	{
		Init(device, sourceCode);
	}

	VulkanShaderModule::~VulkanShaderModule()
	{
		if (VK_NULL_HANDLE != m_shaderModule) 
		{
			vkDestroyShaderModule(m_device->GetDevice(), m_shaderModule, nullptr);
			m_shaderModule = VK_NULL_HANDLE;
		}
	}

	bool VulkanShaderModule::Init(VulkanDevice * device, std::vector<unsigned char> const & sourceCode)
	{
		m_device = device;
		VkShaderModuleCreateInfo shaderModuleCreateInfo = 
		{
			VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,             
			nullptr,                                                 
			0,                                                       
			sourceCode.size(),                                       
			reinterpret_cast<uint32_t const *>(sourceCode.data())    
		};

		VkResult result = vkCreateShaderModule(m_device->GetDevice(), &shaderModuleCreateInfo, nullptr, &m_shaderModule);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not create a shader module." << std::endl;
			return false;
		}
		return true;
	}
}
