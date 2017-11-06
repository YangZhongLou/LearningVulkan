#pragma once

#include "../Renderer.h"
#include "../Define.h"

namespace yzl
{
	class VulkanInstance;

	class VkRenderer : public Renderer
	{
	public:
		VkRenderer();
		~VkRenderer();

	public:
		void Submit() override;
		void Flush() override;

	private:
		void Init();
		void PresentImage();
		bool SubmitCommandBuffersToQueue();
	private:
		LIBRARY_TYPE m_vkLibrary;
		VulkanInstance* m_vkInstance;
	};
}