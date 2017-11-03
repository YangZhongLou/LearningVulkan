#include "VulkanFrameBuffer.h"
#include "VulkanDevice.h"
#include "Utils.h"

namespace yzl
{
	VulkanFrameBuffer::VulkanFrameBuffer(VulkanDevice * device, 
		VkRenderPass renderPass, 
		std::vector<VkImageView> const & attachments, 
		uint32_t width, uint32_t height, uint32_t layers)
	{
		Init(device, renderPass, attachments, width, height, layers);
	}

	VulkanFrameBuffer::~VulkanFrameBuffer()
	{
		if (VK_NULL_HANDLE != m_framebuffer) 
		{
			vkDestroyFramebuffer(m_device->GetDevice(), m_framebuffer, nullptr);
			m_framebuffer = VK_NULL_HANDLE;
		}
	}

	bool VulkanFrameBuffer::Init(VulkanDevice * device, 
		VkRenderPass renderPass, 
		std::vector<VkImageView> const & attachments, 
		uint32_t width, uint32_t height, uint32_t layers)
	{
		m_device = device;

		VkFramebufferCreateInfo framebufferCreateInfo = 
		{
			VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,  
			nullptr,                                    
			0,                                          
			renderPass,
			static_cast<uint32_t>(attachments.size()),  
			attachments.data(),                         
			width,                                      
			height,                                     
			layers                                      
		};

		VkResult result = vkCreateFramebuffer(device->GetDevice(), &framebufferCreateInfo, nullptr, &m_framebuffer);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not create a framebuffer." << std::endl;
			return false;
		}
		return true;
	}
}