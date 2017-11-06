#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace yzl
{
	class VulkanDevice;

	struct ImageDescriptorInfo 
	{
		VkDescriptorSet                     targetDescriptorSet;
		uint32_t                            targetDescriptorBinding;
		uint32_t                            targetArrayElement;
		VkDescriptorType                    targetDescriptorType;
		std::vector<VkDescriptorImageInfo>  imageInfos;
	};

	struct BufferDescriptorInfo 
	{
		VkDescriptorSet                     targetDescriptorSet;
		uint32_t                            targetDescriptorBinding;
		uint32_t                            targetArrayElement;
		VkDescriptorType                    targetDescriptorType;
		std::vector<VkDescriptorBufferInfo> bufferInfos;
	};

	struct TexelBufferDescriptorInfo 
	{
		VkDescriptorSet                     targetDescriptorSet;
		uint32_t                            targetArrayElement;
		VkDescriptorType                    targetDescriptorType;
		uint32_t                            targetDescriptorBinding;
		std::vector<VkBufferView>           texelBufferViews;
	};

	struct CopyDescriptorInfo 
	{
		VkDescriptorSet     targetDescriptorSet;
		uint32_t            targetDescriptorBinding;
		uint32_t            targetArrayElement;
		VkDescriptorSet     sourceDescriptorSet;
		uint32_t            sourceDescriptorBinding;
		uint32_t            sourceArrayElement;
		uint32_t            descriptorCount;
	};


	class VulkanDescriptorPool
	{
	public:
		VulkanDescriptorPool(VulkanDevice * device,
			bool freeIndividualSets,
			uint32_t maxSetsCount,
			std::vector<VkDescriptorPoolSize> const & descriptorTypes);
		~VulkanDescriptorPool();
	public:
		bool AllocateDescriptorSets(std::vector<VkDescriptorSetLayout> const & descriptorSetLayouts);
		void UpdateDescriptorSets(std::vector<ImageDescriptorInfo> const & imageDescriptorInfos,
			std::vector<BufferDescriptorInfo> const & bufferDescriptorInfos,
			std::vector<TexelBufferDescriptorInfo> const & texelBufferDescriptorInfos,
			std::vector<CopyDescriptorInfo> const & copyDescriptorInfos);
		bool Reset();
	private:
		bool Init(VulkanDevice * device,
			bool freeIndividualSets,
			uint32_t maxSetsCount,
			std::vector<VkDescriptorPoolSize> const & descriptorTypes);
	private:
		VulkanDevice * m_device;
		VkDescriptorPool m_pool;
		std::vector<VkDescriptorSet> m_descriptorSets;
	};
}