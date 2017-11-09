#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "../Define.h"
#include "../Renderer.h"

namespace yzl
{
	struct WindowParameters;
	class VulkanSurface;
	class VulkanInstance;
	class VulkanSwapchain;
	class VulkanCommandBuffer;
	class VulkanSemaphore;
	class VulkanFence;
	class VulkanFrameBuffer;
	class VulkanDevice;
	class VulkanImage;
	class VulkanDeviceMemory;

	struct FrameResource 
	{
		VkCommandBuffer commandBuffer;
		VulkanSemaphore * imageAcquiredSemaphore;
		VulkanSemaphore * readyToPresentSemaphore;
		VulkanFence * drawingFinishedFence;
		VkImageView depthAttachment;
		VulkanFrameBuffer * framebuffer;

		~FrameResource();
	};
	
	class VulkanRenderer : public Renderer
	{
	public:
		VulkanRenderer(WindowParameters windowParams);
		~VulkanRenderer();
	public:
		void Submit() override;
		void Flush() override;
	public:
		VulkanDevice * GetDevice();

		void CreatePresentSurface(WindowParameters windowParameters, 
			VkImageUsageFlags imageUsage,
			VkSurfaceTransformFlagBitsKHR surfaceTransform,
			VkPresentModeKHR presentMode,
			VkSurfaceFormatKHR surfaceFormat);
		void PrepareResources();

		void CreateSwapChain(VkImageUsageFlags swapchainImageUsage,
			bool useDepth,
			VkImageUsageFlags depthAttachmentUsage);
	private:
		void Init();
		void PresentImage();
		bool SubmitCommandBuffersToQueue();
	private:
		LIBRARY_TYPE m_vkLibrary;
	private:
		VulkanInstance * m_instance;
		VulkanSurface * m_surface;
		VulkanSwapchain * m_swapchain;

		/* refine this */
		uint32_t m_framesCount;
		std::vector<FrameResource> m_frameResources;

		std::vector<VulkanImage*> m_depthImages;
		std::vector<VulkanDeviceMemory*> m_depthImagesMemory;
	};
}