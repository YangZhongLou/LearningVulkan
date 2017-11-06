
#include "Utils.h"
#include <iostream>

namespace yzl
{
	bool LoadVulkanLib(LIBRARY_TYPE & vulkanLibrary)
	{
#if defined _WIN32
		vulkanLibrary = LoadLibrary("vulkan-1.dll");
#elif defined __linux
		vulkanLibrary = dlopen("libvulkan.so.1", RTLD_NOW);
#endif

		if (vulkanLibrary == nullptr) {
			std::cout << "Error, LoadVulkanLib" << std::endl;
			return false;
		}
		return true;
	}

	void UnloadVulkanLib(LIBRARY_TYPE & vulkanLibrary)
	{
		if (nullptr != vulkanLibrary) {
#if defined _WIN32
			FreeLibrary(vulkanLibrary);
#elif defined __linux
			dlclose(vulkanLibrary);
#endif
			vulkanLibrary = nullptr;
		}
	}

	bool LoadVulkanFunctions(LIBRARY_TYPE& vulkanLibrary)
	{
#if defined _WIN32
#define LoadFunction GetProcAddress
#elif defined __linux
#define LoadFunction dlsym
#endif

#define EXPORTED_VULKAN_FUNCTION( name )                              \
    name = (PFN_##name)LoadFunction( vulkanLibrary, #name );         \
    if( name == nullptr ) {                                           \
      std::cout << "Could not load exported Vulkan function named: "  \
        #name << std::endl;                                           \
      return false;                                                   \
    }

#include "Vulkan/ListOfVulkanFunctions.inl"

		return true;
	}

	bool LoadGlobalLevelFunctions() {
#define GLOBAL_LEVEL_VULKAN_FUNCTION( name )                              \
    name = (PFN_##name)vkGetInstanceProcAddr( nullptr, #name );           \
    if( name == nullptr ) {                                               \
      std::cout << "Could not load global level Vulkan function named: "  \
        #name << std::endl;                                               \
      return false;                                                       \
    }

#include "Vulkan/ListOfVulkanFunctions.inl"

		return true;
	}

	bool LoadInstanceLevelFunctions(VkInstance instance,
		std::vector<char const *> const & enabledExtensions)
	{
#define INSTANCE_LEVEL_VULKAN_FUNCTION( name )                                  \
    name = (PFN_##name)vkGetInstanceProcAddr( instance, #name );                \
    if( name == nullptr ) {                                                     \
      std::cout << "Could not load instance-level Vulkan function named: "      \
        #name << std::endl;                                                     \
      return false;                                                             \
    }

		// Load instance-level functions from enabled extensions
#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension )        \
    for( auto & enabledExtension : enabledExtensions ) {                      \
      if( std::string( enabledExtension ) == std::string( extension ) ) {      \
        name = (PFN_##name)vkGetInstanceProcAddr( instance, #name );            \
        if( name == nullptr ) {                                                 \
          std::cout << "Could not load instance-level Vulkan function named: "  \
            #name << std::endl;                                                 \
          return false;                                                         \
        }                                                                       \
      }                                                                         \
    }

#include "Vulkan/ListOfVulkanFunctions.inl"

		return true;
	}

	bool LoadDeviceLevelFunctions(VkDevice logicalDevice,
		std::vector<char const *> const & enabledExtensions) {

#define DEVICE_LEVEL_VULKAN_FUNCTION( name )                                    \
    name = (PFN_##name)vkGetDeviceProcAddr( logicalDevice, #name );            \
    if( name == nullptr ) {                                                     \
      std::cout << "Could not load device-level Vulkan function named: "        \
        #name << std::endl;                                                     \
      return false;                                                             \
    }

		// Load device-level functions from enabled extensions
#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension )          \
    for( auto & enabled_extension : enabledExtensions ) {                      \
      if( std::string( enabled_extension ) == std::string( extension ) ) {      \
        name = (PFN_##name)vkGetDeviceProcAddr( logicalDevice, #name );        \
        if( name == nullptr ) {                                                 \
          std::cout << "Could not load device-level Vulkan function named: "    \
            #name << std::endl;                                                 \
          return false;                                                         \
        }                                                                       \
      }                                                                         \
    }

#include "Vulkan/ListOfVulkanFunctions.inl"

		return true;
	}
}