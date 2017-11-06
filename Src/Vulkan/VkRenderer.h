#pragma once

#include "../Define.h"
#include <vulkan/vulkan.h>
#include "../Renderer.h"

namespace yzl
{
	class VulkanSurface;
	struct WindowParameters;
	
	class VkRenderer : public Renderer
	{
	public:
		VkRenderer();
		~VkRenderer();
	public:
		void Submit() override;
		void Flush() override;
	public:
		void CreatePresentSurface(WindowParameters windowParameters);
	private:
		void Init();
		void PresentImage();
		bool SubmitCommandBuffersToQueue();
	private:
		LIBRARY_TYPE m_vkLibrary;
		VulkanInstance* m_vkInstance;
	private:
		VulkanSurface* m_surface;
	};
}