#pragma once

#include <vulkan/vulkan.h>

namespace yzl
{
	class VulkanDevice;

	class VulkanSampler
	{
	public:
		VulkanSampler(VulkanDevice* device,
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
			bool unnormalizedCoords);
		~VulkanSampler();
	private:
		bool Init(VulkanDevice* device,
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
			bool unnormalizedCoords);
	private:
		VulkanDevice* m_device;
		VkSampler m_sampler;
	};
}