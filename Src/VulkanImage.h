#pragma once

#include <vulkan/vulkan.h>

namespace yzl
{
	class VulkanDevice;

	class VulkanImage
	{
	public:
		VulkanImage(VulkanDevice*	device,
			VkImageType             type,
			VkFormat                format,
			VkExtent3D              size,
			uint32_t                numMipmaps,
			uint32_t                numLayers,
			VkSampleCountFlagBits   samples,
			VkImageUsageFlags       usageScenarios,
			bool                    cubemap);
		~VulkanImage();
	private:
		bool Init(VulkanDevice*		device,
			VkImageType             type,
			VkFormat                format,
			VkExtent3D              size,
			uint32_t                numMipmaps,
			uint32_t                numLayers,
			VkSampleCountFlagBits   samples,
			VkImageUsageFlags       usageScenarios,
			bool                    cubemap);
	private:
		VkImage m_image;
	};
}