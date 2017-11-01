#pragma once

#include "Singleton.hpp"
#include <vulkan/vulkan.h>

namespace yzl
{
	Singleton(VulkanCommandOperator)
public:
	bool BeginRecord(VkCommandBuffer buffer, VkCommandBufferUsageFlags usage, VkCommandBufferInheritanceInfo * secondaryInfo);
	bool EndRecord(VkCommandBuffer buffer);

	bool Reset(VkCommandBuffer command_buffer, bool releaseResources);
	};

#define vkCommandOperator VulkanCommandOperator::GetInstance()
}