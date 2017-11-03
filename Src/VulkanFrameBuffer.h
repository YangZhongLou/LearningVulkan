#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace yzl
{
	class VulkanDevice;

	class VulkanFrameBuffer
	{
	public:
		VulkanFrameBuffer(VulkanDevice * device,
			VkRenderPass renderPass,
			std::vector<VkImageView> const & attachments,
			uint32_t width,
			uint32_t height,
			uint32_t layers);
		~VulkanFrameBuffer();
	private:
		bool Init(VulkanDevice * device,
			VkRenderPass renderPass,
			std::vector<VkImageView> const & attachments,
			uint32_t width,
			uint32_t height,
			uint32_t layers);
	private:
		VulkanDevice * m_device;
		VkFramebuffer m_framebuffer;
	};
}