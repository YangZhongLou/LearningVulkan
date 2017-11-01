#include "VulkanCommandOperator.h"
#include "Utils.h"

namespace yzl
{
	bool VulkanCommandOperator::BeginRecord(VkCommandBuffer buffer, VkCommandBufferUsageFlags usage, VkCommandBufferInheritanceInfo * secondaryInfo)
	{
		VkCommandBufferBeginInfo commandBufferBeginInfo = {
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,   
			nullptr,                                       
			usage,                                         
			secondaryInfo
		};

		VkResult result = vkBeginCommandBuffer(buffer, &commandBufferBeginInfo);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not begin command buffer recording operation." << std::endl;
			return false;
		}
		return true;
	}

	bool EndRecord(VkCommandBuffer buffer)
	{
		VkResult result = vkEndCommandBuffer(buffer);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Error occurred during command buffer recording." << std::endl;
			return false;
		}
		return true;
	}
}