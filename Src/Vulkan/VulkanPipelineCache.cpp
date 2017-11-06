#include "../Utils.h"
#include "VulkanDevice.h"
#include "VulkanPipelineCache.h"


namespace yzl
{
	VulkanPipelineCache::VulkanPipelineCache(VulkanDevice * device, std::vector<unsigned char> const & cacheData)
	{
	}

	VulkanPipelineCache::~VulkanPipelineCache()
	{
		if (VK_NULL_HANDLE != m_pipelineCache) 
		{
			vkDestroyPipelineCache(m_device->GetDevice(), m_pipelineCache, nullptr);
			m_pipelineCache = VK_NULL_HANDLE;
		}
	}

	bool VulkanPipelineCache::GetData(std::vector<unsigned char>& cacheData)
	{
		size_t dataSize = 0;
		VkResult result = vkGetPipelineCacheData(m_device->GetDevice(), m_pipelineCache, &dataSize, nullptr);
		if ((VK_SUCCESS != result) || (0 == dataSize)) 
		{
			std::cout << "Could not get the size of the pipeline cache." << std::endl;
			return false;
		}

		cacheData.resize(dataSize);

		result = vkGetPipelineCacheData(m_device->GetDevice(), m_pipelineCache, &dataSize, cacheData.data());
		if ((VK_SUCCESS != result) || (0 == dataSize)) 
		{
			std::cout << "Could not acquire pipeline cache data." << std::endl;
			return false;
		}

		return true;
	}

	bool VulkanPipelineCache::MergePipelineCaches(std::vector<VkPipelineCache> const & sourcePipelineCaches)
	{
		if (sourcePipelineCaches.size() > 0)
		{
			VkResult result = vkMergePipelineCaches(m_device->GetDevice(), m_pipelineCache, static_cast<uint32_t>(sourcePipelineCaches.size()), sourcePipelineCaches.data());
			if (VK_SUCCESS != result) 
			{
				std::cout << "Could not merge pipeline cache objects." << std::endl;
				return false;
			}
			return true;
		}

		return false;
	}

	bool VulkanPipelineCache::CreateGraphicsPipelines(std::vector<VkGraphicsPipelineCreateInfo> const & graphicsPipelineCreateInfos, 
		std::vector<VkPipeline>& graphicsPipelines)
	{
		if (graphicsPipelineCreateInfos.size() > 0)
		{
			graphicsPipelines.resize(graphicsPipelineCreateInfos.size());
			VkResult result = vkCreateGraphicsPipelines(m_device->GetDevice(), m_pipelineCache, static_cast<uint32_t>(graphicsPipelineCreateInfos.size()), graphicsPipelineCreateInfos.data(), nullptr, graphicsPipelines.data());
			if (VK_SUCCESS != result) 
			{
				std::cout << "Could not create a graphics pipeline." << std::endl;
				return false;
			}
			return true;
		}

		return false;
	}

	bool VulkanPipelineCache::CreateComputePipelines(std::vector<VkComputePipelineCreateInfo> const & computePipelineCreateInfos, std::vector<VkPipeline>& computePipelines)
	{
		if (computePipelineCreateInfos.size() > 0)
		{
			computePipelines.resize(computePipelineCreateInfos.size());
			VkResult result = vkCreateComputePipelines(m_device->GetDevice(), m_pipelineCache, static_cast<uint32_t>(computePipelineCreateInfos.size()), computePipelineCreateInfos.data(), nullptr, computePipelines.data());
			if (VK_SUCCESS != result)
			{
				std::cout << "Could not create a graphics pipeline." << std::endl;
				return false;
			}
			return true;
		}

		return false;
	}

	void VulkanPipelineCache::DestroyPipeline(VkPipeline & pipeline)
	{
		if (VK_NULL_HANDLE != pipeline) 
		{
			vkDestroyPipeline(m_device->GetDevice(), pipeline, nullptr);
			pipeline = VK_NULL_HANDLE;
		}
	}

	bool VulkanPipelineCache::Init(VulkanDevice * device, std::vector<unsigned char> const & cacheData)
	{
		m_device = device;
		VkPipelineCacheCreateInfo pipelineCacheCreateInfo = 
		{
			VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,   
			nullptr,                                        
			0,                                              
			static_cast<uint32_t>(cacheData.size()),
			cacheData.data()
		};

		VkResult result = vkCreatePipelineCache(m_device->GetDevice(), &pipelineCacheCreateInfo, nullptr, &m_pipelineCache);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not create pipeline cache." << std::endl;
			return false;
		}

		return true;
	}
}

