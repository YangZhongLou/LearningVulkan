#include "VkRenderer.h"
#include "VulkanInstance.h"
#include "Utils.h"

namespace yzl
{
	VkRenderer::VkRenderer()
	{
		Init();
	}

	void VkRenderer::Init()
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

		m_vkInstance = new VulkanInstance("Render", "Render", extensions);
	}

	VkRenderer::~VkRenderer()
	{
		UnloadVulkanLib(m_vkLibrary);
		SAFE_DELETE(m_vkInstance);
	}

	void VkRenderer::Submit()
	{

	}

	void VkRenderer::Flush()
	{

	}
}