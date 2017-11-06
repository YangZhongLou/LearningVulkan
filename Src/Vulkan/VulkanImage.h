#pragma once

#include <vulkan/vulkan.h>

namespace yzl
{
	class VulkanDevice;

	struct ImageTransition 
	{
		VkImage             image;
		VkAccessFlags       currentAccess;
		VkAccessFlags       newAccess;
		VkImageLayout       currentLayout;
		VkImageLayout       newLayout;
		uint32_t            currentQueueFamily;
		uint32_t            newQueueFamily;
		VkImageAspectFlags  aspect;
	};

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
	public:
		VkImage GetImage() const { return m_image; }
		bool CreateView(
			VkImageViewType viewType,
			VkFormat format,
			VkImageAspectFlags aspect, 
			VkImageView& imageView);
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
		VulkanDevice* m_device;
		VkImage m_image;
		VkImageView m_imageView;
	};
}