#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace yzl
{
	class VulkanDevice;

	class VulkanPipelineCache
	{
	public:
		VulkanPipelineCache(VulkanDevice * device, std::vector<unsigned char> const & cacheData);
		~VulkanPipelineCache();
	public:
		bool GetData(std::vector<unsigned char> & cacheData);
		bool MergePipelineCaches(std::vector<VkPipelineCache> const & sourcePipelineCaches);
		bool CreateGraphicsPipelines(std::vector<VkGraphicsPipelineCreateInfo> const & graphicsPipelineCreateInfos,
			std::vector<VkPipeline> & graphicsPipelines);
		bool CreateComputePipelines(std::vector<VkComputePipelineCreateInfo> const &computePipelineCreateInfos,
			std::vector<VkPipeline> & computePipelines);

		void DestroyPipeline(VkPipeline & pipeline);
	private:
		bool Init(VulkanDevice * device, std::vector<unsigned char> const & cacheData);
	private:
		VulkanDevice * m_device;
		VkPipelineCache m_pipelineCache;
	};
}