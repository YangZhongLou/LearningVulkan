#include "VulkanDescriptorPool.h"
#include "VulkanDevice.h"
#include "../Utils.h"


namespace yzl
{
	VulkanDescriptorPool::VulkanDescriptorPool(VulkanDevice * device, bool freeIndividualSets, uint32_t maxSetsCount, std::vector<VkDescriptorPoolSize> const & descriptorTypes)
	{
		Init(device, freeIndividualSets, maxSetsCount, descriptorTypes);
	}

	VulkanDescriptorPool::~VulkanDescriptorPool()
	{
		if (m_descriptorSets.size() > 0) 
		{
			VkResult result = vkFreeDescriptorSets(m_device->GetDevice(), m_pool, static_cast<uint32_t>(m_descriptorSets.size()), m_descriptorSets.data());
			m_descriptorSets.clear();
			if (VK_SUCCESS != result) 
			{
				std::cout << "Error occurred during freeing descriptor sets." << std::endl;
			}
		}

		if (VK_NULL_HANDLE != m_pool)
		{
			vkDestroyDescriptorPool(m_device->GetDevice(), m_pool, nullptr);
			m_pool = VK_NULL_HANDLE;
		}
	}

	bool VulkanDescriptorPool::AllocateDescriptorSets(std::vector<VkDescriptorSetLayout> const & descriptorSetLayouts)
	{
		if (descriptorSetLayouts.size() > 0) 
		{
			VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = 
			{
				VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,         
				nullptr,
				m_pool,                                       
				static_cast<uint32_t>(descriptorSetLayouts.size()),
				descriptorSetLayouts.data()
			};

			m_descriptorSets.resize(descriptorSetLayouts.size());

			VkResult result = vkAllocateDescriptorSets(m_device->GetDevice(), &descriptorSetAllocateInfo, m_descriptorSets.data());
			if (VK_SUCCESS != result) 
			{
				std::cout << "Could not allocate descriptor sets." << std::endl;
				return false;
			}

			return true;
		}

		return false;
	}

	void VulkanDescriptorPool::UpdateDescriptorSets(std::vector<ImageDescriptorInfo> const & imageDescriptorInfos, 
		std::vector<BufferDescriptorInfo> const & bufferDescriptorInfos, 
		std::vector<TexelBufferDescriptorInfo> const & texelBufferDescriptorInfos, 
		std::vector<CopyDescriptorInfo> const & copyDescriptorInfos)
	{
		std::vector<VkWriteDescriptorSet> writeDescriptors;
		std::vector<VkCopyDescriptorSet> copyDescriptors;

		for (auto & imageDescriptor : imageDescriptorInfos) 
		{
			writeDescriptors.push_back({
				VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,                              
				nullptr,                                                             
				imageDescriptor.targetDescriptorSet,                                
				imageDescriptor.targetDescriptorBinding,                            
				imageDescriptor.targetArrayElement,
				static_cast<uint32_t>(imageDescriptor.imageInfos.size()),
				imageDescriptor.targetDescriptorType,
				imageDescriptor.imageInfos.data(),
				nullptr,                                                             
				nullptr                                                              
			});
		}

		for (auto & bufferDescriptor : bufferDescriptorInfos) 
		{
			writeDescriptors.push_back({
				VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,                              
				nullptr,                                                             
				bufferDescriptor.targetDescriptorSet,                               
				bufferDescriptor.targetDescriptorBinding,                           
				bufferDescriptor.targetArrayElement,                                
				static_cast<uint32_t>(bufferDescriptor.bufferInfos.size()),         
				bufferDescriptor.targetDescriptorType,                              
				nullptr,                                                             
				bufferDescriptor.bufferInfos.data(),                                
				nullptr                                                              
			});
		}

		for (auto & texelBufferDescriptor : texelBufferDescriptorInfos) 
		{
			writeDescriptors.push_back({
				VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,                               
				nullptr,                                                              
				texelBufferDescriptor.targetDescriptorSet,
				texelBufferDescriptor.targetDescriptorBinding,
				texelBufferDescriptor.targetArrayElement,
				static_cast<uint32_t>(texelBufferDescriptor.texelBufferViews.size()),
				texelBufferDescriptor.targetDescriptorType,                         
				nullptr,                                                              
				nullptr,                                                              
				texelBufferDescriptor.texelBufferViews.data()                       
			});
		}

		for (auto & copyDescriptor : copyDescriptorInfos) {
			copyDescriptors.push_back({
				VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET,                             
				nullptr,                                                           
				copyDescriptor.sourceDescriptorSet,                               
				copyDescriptor.sourceDescriptorBinding,                           
				copyDescriptor.sourceArrayElement,                                
				copyDescriptor.targetDescriptorSet,
				copyDescriptor.targetDescriptorBinding,
				copyDescriptor.targetArrayElement,
				copyDescriptor.descriptorCount                                    
			});
		}

		vkUpdateDescriptorSets(m_device->GetDevice(), static_cast<uint32_t>(writeDescriptors.size()), writeDescriptors.data(), static_cast<uint32_t>(copyDescriptors.size()), copyDescriptors.data());
	}

	bool VulkanDescriptorPool::Reset()
	{
		VkResult result = vkResetDescriptorPool(m_device->GetDevice(), m_pool, 0);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Error occurred during descriptor pool reset." << std::endl;
			return false;
		}
		return true;
	}

	bool VulkanDescriptorPool::Init(VulkanDevice * device, bool freeIndividualSets, uint32_t maxSetsCount, 
		std::vector<VkDescriptorPoolSize> const & descriptorTypes)
	{
		m_device = device;
		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = 
		{
			VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,              
			nullptr,                                                    
			freeIndividualSets ?
			VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT : 0u,
			maxSetsCount,
			static_cast<uint32_t>(descriptorTypes.size()),
			descriptorTypes.data()
		};

		VkResult result = vkCreateDescriptorPool(device->GetDevice(), &descriptorPoolCreateInfo, nullptr, &m_pool);
		if (VK_SUCCESS != result) 
		{
			std::cout << "Could not create a descriptor pool." << std::endl;
			return false;
		}
		return true;
	}

}
