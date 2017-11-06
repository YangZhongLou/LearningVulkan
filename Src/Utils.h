#pragma once

#include "Define.h"
#include "Vulkan/VulkanFunctions.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>

namespace yzl
{
	bool LoadVulkanLib(LIBRARY_TYPE& vulkanLibrary);
	void UnloadVulkanLib(LIBRARY_TYPE & vulkanLibrary);

	bool LoadVulkanFunctions(LIBRARY_TYPE& vulkanLibrary);
	bool LoadGlobalLevelFunctions();
	bool LoadInstanceLevelFunctions(VkInstance instance,
		std::vector<char const *> const & enabledExtensions);

	bool LoadDeviceLevelFunctions(VkDevice logicalDevice,
		std::vector<char const *> const & enabledExtensions);

#define SAFE_DELETE(x) if(x) delete x;
#define SAFE_DELETE_N(x) if(x) delete [] x;
}