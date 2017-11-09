#include "VulkanRenderer.h"
#include "VulkanInstance.h"
#include "../Utils.h"
#include "../WindowParameters.h"
#include "VulkanSurface.h"
#include "VulkanSwapChain.h"
#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanCommandPool.h"
#include "VulkanSemaphore.h"
#include "VulkanFence.h"
#include "VulkanImage.h"
#include "VulkanFrameBuffer.h"
#include "VulkanDeviceMemory.h"
#include "VulkanBuffer.h"

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
		SAFE_DELETE(m_instance);
		SAFE_DELETE(m_surface);
		SAFE_DELETE(m_swapchain);
		if (m_depthImages.size() > 0)
		{
			for (auto depthImage : m_depthImages)
			{
				SAFE_DELETE(depthImage);
			}

			for (auto depthImageMemory : m_depthImagesMemory)
			{
				SAFE_DELETE(depthImageMemory);
			}
		}

		SAFE_DELETE(m_vertexBuffer);
		SAFE_DELETE(m_vertexBufferMemory);

		UnloadVulkanLib(m_vkLibrary);
	}

	void VulkanRenderer::Submit()
	{

	}

	void VulkanRenderer::Flush()
	{

	}

	VulkanDevice * VulkanRenderer::GetDevice()
	{
		return m_instance->GetDevice();
	}

	void VulkanRenderer::CreateVertexBuffer(VkDeviceSize size)
	{
		m_vertexBuffer = new VulkanBuffer(GetDevice(), size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
		m_vertexBufferMemory = new VulkanDeviceMemory(GetDevice());
		m_vertexBufferMemory->Bind(m_vertexBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
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
		auto commandPool = GetDevice()->GetCommandPool();
		commandPool->AllocateCommandBuffers(VK_COMMAND_BUFFER_LEVEL_PRIMARY, m_framesCount);

		for (uint32_t i = 0; i < m_framesCount; ++i)
		{
			VulkanSemaphore * imageAcquiredSemaphore = new VulkanSemaphore(GetDevice());
			VulkanSemaphore * readyToPresentSemaphore = new VulkanSemaphore(GetDevice());
			VulkanFence * drawingFinishedFence = new VulkanFence(GetDevice(), true);
			VkImageView depthAttachment = VK_NULL_HANDLE;;

			m_frameResources.push_back({
				commandPool->GetCommandBuffer(i),
				imageAcquiredSemaphore,
				readyToPresentSemaphore,
				drawingFinishedFence,
				depthAttachment,
				{} });
		}
	}

	void VulkanRenderer::CreateSwapChain(VkImageUsageFlags swapchainImageUsage, bool useDepth, VkImageUsageFlags depthAttachmentUsage)
	{
		m_swapchain = new VulkanSwapchain(m_instance->GetDevice(), m_surface);

		auto imageExtent = m_swapchain->GetVulkanSurface()->GetImageExtent();
		if (useDepth)
		{
			for (uint32_t i = 0; i < m_framesCount; ++i)
			{
				VulkanImage * image = new VulkanImage(GetDevice(), 
					VK_IMAGE_TYPE_2D, 
					VK_FORMAT_D16_UNORM, 
					{ imageExtent.width, imageExtent.height, 1 },
					1,
					1,
					VK_SAMPLE_COUNT_1_BIT,
					VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
					false);

				VulkanDeviceMemory * memory = new VulkanDeviceMemory(GetDevice());
				memory->Bind(image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
				image->CreateView(VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_D16_UNORM, VK_IMAGE_ASPECT_DEPTH_BIT, m_frameResources[i].depthAttachment);

				m_depthImages.emplace_back(image);
				m_depthImagesMemory.emplace_back(memory);
			}
		}
	}

	FrameResource::~FrameResource()
	{
		SAFE_DELETE(imageAcquiredSemaphore);
		SAFE_DELETE(readyToPresentSemaphore);
		SAFE_DELETE(drawingFinishedFence);
		SAFE_DELETE(framebuffer);
	}
}