#pragma once

#include <vulkan/vulkan.h>
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

	struct FrameResources 
	{
		VulkanCommandBuffer * commandBuffer;
		VulkanSemaphore * imageAcquiredSemaphore;
		VulkanSemaphore * readyToPresentSemaphore;
		VulkanFence * drawingFinishedFence;
		VkImageView depthAttachment;
		VulkanFrameBuffer * framebuffer;
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
		const VulkanDevice * GetDevice() const;

		void CreatePresentSurface(WindowParameters windowParameters, 
			VkImageUsageFlags imageUsage,
			VkSurfaceTransformFlagBitsKHR surfaceTransform,
			VkPresentModeKHR presentMode,
			VkSurfaceFormatKHR surfaceFormat);
		void PrepareResources();

		void CreateSwapChain(VkImageUsageFlags swapchain_image_usage,
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
	};
}