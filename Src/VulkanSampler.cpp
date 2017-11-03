#include "VulkanSampler.h"
#include "VulkanDevice.h"
#include "Utils.h"

namespace yzl
{
	VulkanSampler::VulkanSampler(VulkanDevice * device, 
		VkFilter magFilter, 
		VkFilter minFilter, 
		VkSamplerMipmapMode mipmapMode, 
		VkSamplerAddressMode uAddressMode, 
		VkSamplerAddressMode vAddressMode, 
		VkSamplerAddressMode wAddressMode, 
		float lodBias, 
		bool anisotropyEnable, 
		float maxAnisotropy, 
		bool compareEnable, 
		VkCompareOp compareOperator, 
		float minLod, 
		float maxLod, 
		VkBorderColor borderColor, 
		bool unnormalizedCoords)
	{
		Init(device, magFilter, minFilter, mipmapMode, uAddressMode, vAddressMode, wAddressMode, lodBias,
			anisotropyEnable, maxAnisotropy, compareEnable, compareOperator, minLod, maxLod,
			borderColor, unnormalizedCoords);
	}

	VulkanSampler::~VulkanSampler()
	{
		if (VK_NULL_HANDLE != m_sampler) 
		{
			vkDestroySampler(m_device->GetDevice(), m_sampler, nullptr);
			m_sampler = VK_NULL_HANDLE;
		}
	}

	bool VulkanSampler::Init(VulkanDevice * device, 
		VkFilter magFilter, 
		VkFilter minFilter, 
		VkSamplerMipmapMode mipmapMode, 
		VkSamplerAddressMode uAddressMode, 
		VkSamplerAddressMode vAddressMode, 
		VkSamplerAddressMode wAddressMode, 
		float lodBias, 
		bool anisotropyEnable, 
		float maxAnisotropy, 
		bool compareEnable, 
		VkCompareOp compareOperator, 
		float minLod, 
		float maxLod, 
		VkBorderColor borderColor, 
		bool unnormalizedCoords)
	{
		m_device = device;
		VkSamplerCreateInfo samplerCreateInfo = 
		{
			VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,  
			nullptr,                                
			0,                                      
			magFilter,                              
			minFilter,                              
			mipmapMode,                             
			uAddressMode,                           
			vAddressMode,                           
			wAddressMode,                           
			lodBias,                             
			anisotropyEnable,                    
			maxAnisotropy,                       
			compareEnable,                       
			compareOperator,                     
			minLod,                              
			maxLod,                              
			borderColor,                         
			unnormalizedCoords                   
		};

		VkResult result = vkCreateSampler(device->GetDevice(), &samplerCreateInfo, nullptr, &m_sampler);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not create sampler." << std::endl;
			return false;
		}

		m_device = device;
		return true;
	}
}