#pragma once

#include "../Define.h"
#include "../Renderer.h"
#include <vulkan/vulkan.h>

namespace yzl
{
	class VulkanSurface;
	struct WindowParameters;
	class VulkanInstance;
	
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
	private:
		VulkanInstance * m_vkInstance;
		VulkanSurface * m_surface;
	};
}