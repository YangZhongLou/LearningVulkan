#include "VulkanRenderer.h"
#include "VulkanInstance.h"
#include "../Utils.h"
#include "../WindowParameters.h"
#include "VulkanSurface.h"
#include "VulkanSwapChain.h"
#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanCommandPool.h"

namespace yzl
{
	VulkanRenderer::VulkanRenderer(WindowParameters windowParams)
	{
		m_framesCount = 3;
		Init();
		CreatePresentSurface(windowParams,
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
			VK_PRESENT_MODE_MAILBOX_KHR,
			{ VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR });;

	}

	void VulkanRenderer::Init()
	{
		LoadVulkanLib(m_vkLibrary);
		LoadVulkanFunctions(m_vkLibrary);
		LoadGlobalLevelFunctions();

		std::vector<char const *> extensions;
		extensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
		extensions.emplace_back(
#ifdef VK_USE_PLATFORM_WIN32_KHR
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME

#elif defined VK_USE_PLATFORM_XCB_KHR
			VK_KHR_XCB_SURFACE_EXTENSION_NAME

#elif defined VK_USE_PLATFORM_XLIB_KHR
			VK_KHR_XLIB_SURFACE_EXTENSION_NAME
#endif
		);

		m_instance = new VulkanInstance("Render", "Render", extensions);
	}

	VulkanRenderer::~VulkanRenderer()
	{
		UnloadVulkanLib(m_vkLibrary);
		SAFE_DELETE(m_instance);
		SAFE_DELETE(m_surface);
		SAFE_DELETE(m_swapchain);
	}

	void VulkanRenderer::Submit()
	{

	}

	void VulkanRenderer::Flush()
	{

	}

	const VulkanDevice * VulkanRenderer::GetDevice() const
	{
		return m_instance->GetDevice();
	}

	void VulkanRenderer::CreatePresentSurface(WindowParameters windowParameters, 
		VkImageUsageFlags imageUsage, 
		VkSurfaceTransformFlagBitsKHR surfaceTransform, 
		VkPresentModeKHR presentMode, 
		VkSurfaceFormatKHR surfaceFormat)
	{
		m_surface = new VulkanSurface(m_instance->GetInstance(), &windowParameters);
		m_surface->GetCapabilities(m_instance->GetDevice()->GetVulkanPhysicalDevice()->GetDevice());
		if (!m_surface->CheckImageCount())
		{
			std::cout << "Error, VkRenderer::CreatePresentSurface, m_surface->CheckImageCount()" << std::endl;
		}

		if (!m_surface->CheckImageExtent())
		{
			std::cout << "Error, VkRenderer::CreatePresentSurface, m_surface->CheckImageExtent()" << std::endl;
		}


		if (!m_surface->CheckImageUsage(imageUsage))
		{
			std::cout << "Error, VkRenderer::CreatePresentSurface, m_surface->CheckImageUsage()" << std::endl;
		}

		if (!m_surface->CheckSurfaceTransform(surfaceTransform))
		{
			std::cout << "Error, VkRenderer::CreatePresentSurface, m_surface->CheckSurfaceTransform()" << std::endl;
		}

		if (!m_surface->CheckPresentMode(m_instance->GetDevice()->GetVulkanPhysicalDevice()->GetDevice(), presentMode))
		{
			std::cout << "Error, VkRenderer::CreatePresentSurface, m_surface->CheckPresentMode()" << std::endl;
		}

		if (!m_surface->CheckFormat(m_instance->GetDevice()->GetVulkanPhysicalDevice()->GetDevice(), surfaceFormat))
		{
			std::cout << "Error, VkRenderer::CreatePresentSurface, m_surface->CheckFormat()" << std::endl;
		}
	}

	void VulkanRenderer::PrepareResources()
	{
		for (uint32_t i = 0; i < m_framesCount; ++i)
		{
			/*
			std::vector<VkCommandBuffer> command_buffer;
			VkDestroyer<VkSemaphore> image_acquired_semaphore(LogicalDevice);
			VkDestroyer<VkSemaphore> ready_to_present_semaphore(LogicalDevice);
			VkDestroyer<VkFence> drawing_finished_fence(LogicalDevice);
			VkDestroyer<VkImageView> depth_attachment(LogicalDevice);

			GetDevice()->GetCommandPool()->AllocateCommandBuffers(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
			if (!AllocateCommandBuffers(*LogicalDevice, *CommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1, command_buffer)) {
				return false;
			}
			if (!CreateSemaphore(*LogicalDevice, *image_acquired_semaphore)) {
				return false;
			}
			if (!CreateSemaphore(*LogicalDevice, *ready_to_present_semaphore)) {
				return false;
			}
			if (!CreateFence(*LogicalDevice, true, *drawing_finished_fence)) {
				return false;
			}

			FrameResources.push_back({
				command_buffer[0],
				std::move(image_acquired_semaphore),
				std::move(ready_to_present_semaphore),
				std::move(drawing_finished_fence),
				std::move(depth_attachment),
				{}
			});*/
		}
	}

	void VulkanRenderer::CreateSwapChain(VkImageUsageFlags swapchain_image_usage, bool useDepth, VkImageUsageFlags depthAttachmentUsage)
	{
		m_swapchain = new VulkanSwapchain(m_instance->GetDevice(), m_surface);
	}
}