/**
 * Copyright (c) 2021-2022 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License")
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * DO NOT EDIT: This file is generated.
 */

#ifndef VULKAN_PROFILES_HPP_
#define VULKAN_PROFILES_HPP_ 1

#define VPAPI_ATTR inline

#include <vulkan/vulkan.h>
#include <cstddef>
#include <cstring>
#include <cstdint>
#include <cmath>
#include <vector>
#include <algorithm>

#if defined(VK_VERSION_1_0) && \
    defined(VK_EXT_swapchain_colorspace) && \
    defined(VK_GOOGLE_display_timing) && \
    defined(VK_KHR_android_surface) && \
    defined(VK_KHR_dedicated_allocation) && \
    defined(VK_KHR_descriptor_update_template) && \
    defined(VK_KHR_external_fence) && \
    defined(VK_KHR_external_fence_capabilities) && \
    defined(VK_KHR_external_fence_fd) && \
    defined(VK_KHR_external_memory) && \
    defined(VK_KHR_external_memory_capabilities) && \
    defined(VK_KHR_external_semaphore) && \
    defined(VK_KHR_external_semaphore_capabilities) && \
    defined(VK_KHR_external_semaphore_fd) && \
    defined(VK_KHR_get_memory_requirements2) && \
    defined(VK_KHR_get_physical_device_properties2) && \
    defined(VK_KHR_get_surface_capabilities2) && \
    defined(VK_KHR_incremental_present) && \
    defined(VK_KHR_maintenance1) && \
    defined(VK_KHR_storage_buffer_storage_class) && \
    defined(VK_KHR_surface) && \
    defined(VK_KHR_swapchain) && \
    defined(VK_KHR_variable_pointers)
#define VP_ANDROID_baseline_2021 1
#define VP_ANDROID_BASELINE_2021_NAME "VP_ANDROID_baseline_2021"
#define VP_ANDROID_BASELINE_2021_SPEC_VERSION 1
#define VP_ANDROID_BASELINE_2021_MIN_API_VERSION VK_MAKE_VERSION(1, 0, 68)
#endif

#if defined(VK_VERSION_1_3) && \
    defined(VK_KHR_global_priority)
#define VP_KHR_roadmap_2022 1
#define VP_KHR_ROADMAP_2022_NAME "VP_KHR_roadmap_2022"
#define VP_KHR_ROADMAP_2022_SPEC_VERSION 1
#define VP_KHR_ROADMAP_2022_MIN_API_VERSION VK_MAKE_VERSION(1, 3, 204)
#endif

#if defined(VK_VERSION_1_1) && \
    defined(VK_EXT_descriptor_indexing) && \
    defined(VK_EXT_host_query_reset) && \
    defined(VK_EXT_inline_uniform_block) && \
    defined(VK_EXT_memory_budget) && \
    defined(VK_EXT_robustness2) && \
    defined(VK_EXT_scalar_block_layout) && \
    defined(VK_EXT_subgroup_size_control) && \
    defined(VK_EXT_texel_buffer_alignment) && \
    defined(VK_EXT_vertex_attribute_divisor) && \
    defined(VK_KHR_16bit_storage) && \
    defined(VK_KHR_8bit_storage) && \
    defined(VK_KHR_bind_memory2) && \
    defined(VK_KHR_create_renderpass2) && \
    defined(VK_KHR_dedicated_allocation) && \
    defined(VK_KHR_depth_stencil_resolve) && \
    defined(VK_KHR_descriptor_update_template) && \
    defined(VK_KHR_device_group) && \
    defined(VK_KHR_driver_properties) && \
    defined(VK_KHR_external_fence) && \
    defined(VK_KHR_external_memory) && \
    defined(VK_KHR_external_semaphore) && \
    defined(VK_KHR_get_memory_requirements2) && \
    defined(VK_KHR_image_format_list) && \
    defined(VK_KHR_maintenance1) && \
    defined(VK_KHR_maintenance2) && \
    defined(VK_KHR_maintenance3) && \
    defined(VK_KHR_multiview) && \
    defined(VK_KHR_relaxed_block_layout) && \
    defined(VK_KHR_sampler_mirror_clamp_to_edge) && \
    defined(VK_KHR_shader_draw_parameters) && \
    defined(VK_KHR_shader_float16_int8) && \
    defined(VK_KHR_storage_buffer_storage_class) && \
    defined(VK_KHR_swapchain) && \
    defined(VK_KHR_swapchain_mutable_format) && \
    defined(VK_KHR_timeline_semaphore) && \
    defined(VK_KHR_uniform_buffer_standard_layout) && \
    defined(VK_KHR_variable_pointers)
#define VP_LUNARG_desktop_baseline_2022 1
#define VP_LUNARG_DESKTOP_BASELINE_2022_NAME "VP_LUNARG_desktop_baseline_2022"
#define VP_LUNARG_DESKTOP_BASELINE_2022_SPEC_VERSION 1
#define VP_LUNARG_DESKTOP_BASELINE_2022_MIN_API_VERSION VK_MAKE_VERSION(1, 1, 139)
#endif

#if defined(VK_VERSION_1_1) && \
    defined(VK_EXT_descriptor_indexing) && \
    defined(VK_EXT_host_query_reset) && \
    defined(VK_EXT_inline_uniform_block) && \
    defined(VK_EXT_memory_budget) && \
    defined(VK_EXT_robustness2) && \
    defined(VK_EXT_scalar_block_layout) && \
    defined(VK_EXT_subgroup_size_control) && \
    defined(VK_EXT_texel_buffer_alignment) && \
    defined(VK_EXT_vertex_attribute_divisor) && \
    defined(VK_KHR_16bit_storage) && \
    defined(VK_KHR_8bit_storage) && \
    defined(VK_KHR_bind_memory2) && \
    defined(VK_KHR_create_renderpass2) && \
    defined(VK_KHR_dedicated_allocation) && \
    defined(VK_KHR_depth_stencil_resolve) && \
    defined(VK_KHR_descriptor_update_template) && \
    defined(VK_KHR_device_group) && \
    defined(VK_KHR_driver_properties) && \
    defined(VK_KHR_external_fence) && \
    defined(VK_KHR_external_memory) && \
    defined(VK_KHR_external_semaphore) && \
    defined(VK_KHR_get_memory_requirements2) && \
    defined(VK_KHR_image_format_list) && \
    defined(VK_KHR_maintenance1) && \
    defined(VK_KHR_maintenance2) && \
    defined(VK_KHR_maintenance3) && \
    defined(VK_KHR_multiview) && \
    defined(VK_KHR_portability_enumeration) && \
    defined(VK_KHR_portability_subset) && \
    defined(VK_KHR_relaxed_block_layout) && \
    defined(VK_KHR_sampler_mirror_clamp_to_edge) && \
    defined(VK_KHR_shader_draw_parameters) && \
    defined(VK_KHR_shader_float16_int8) && \
    defined(VK_KHR_storage_buffer_storage_class) && \
    defined(VK_KHR_swapchain) && \
    defined(VK_KHR_swapchain_mutable_format) && \
    defined(VK_KHR_timeline_semaphore) && \
    defined(VK_KHR_uniform_buffer_standard_layout) && \
    defined(VK_KHR_variable_pointers)
#define VP_LUNARG_desktop_portability_2022 1
#define VP_LUNARG_DESKTOP_PORTABILITY_2022_NAME "VP_LUNARG_desktop_portability_2022"
#define VP_LUNARG_DESKTOP_PORTABILITY_2022_SPEC_VERSION 1
#define VP_LUNARG_DESKTOP_PORTABILITY_2022_MIN_API_VERSION VK_MAKE_VERSION(1, 1, 208)
#endif

#define VP_MAX_PROFILE_NAME_SIZE 256U

typedef struct VpProfileProperties {
    char        profileName[VP_MAX_PROFILE_NAME_SIZE];
    uint32_t    specVersion;
} VpProfileProperties;

typedef enum VpInstanceCreateFlagBits {
    // Default behavior:
    // - profile extensions are used (application must not specify extensions)

    // Merge application provided extension list and profile extension list
    VP_INSTANCE_CREATE_MERGE_EXTENSIONS_BIT = 0x00000001,

    // Use application provided extension list
    VP_INSTANCE_CREATE_OVERRIDE_EXTENSIONS_BIT = 0x00000002,

    VP_INSTANCE_CREATE_FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
} VpInstanceCreateFlagBits;
typedef VkFlags VpInstanceCreateFlags;

typedef struct VpInstanceCreateInfo {
    const VkInstanceCreateInfo* pCreateInfo;
    const VpProfileProperties*  pProfile;
    VpInstanceCreateFlags       flags;
} VpInstanceCreateInfo;

typedef enum VpDeviceCreateFlagBits {
    // Default behavior:
    // - profile extensions are used (application must not specify extensions)
    // - profile feature structures are used (application must not specify any of them) extended
    //   with any other application provided struct that isn't defined by the profile

    // Merge application provided extension list and profile extension list
    VP_DEVICE_CREATE_MERGE_EXTENSIONS_BIT = 0x00000001,

    // Use application provided extension list
    VP_DEVICE_CREATE_OVERRIDE_EXTENSIONS_BIT = 0x00000002,

    // Merge application provided versions of feature structures with the profile features
    // Currently unsupported, but is considered for future inclusion in which case the
    // default behavior could potentially be changed to merging as the currently defined
    // default behavior is forward-compatible with that
    // VP_DEVICE_CREATE_MERGE_FEATURES_BIT = 0x00000004,

    // Use application provided versions of feature structures but use the profile feature
    // structures when the application doesn't provide one (robust access disable flags are
    // ignored if the application overrides the corresponding feature structures)
    VP_DEVICE_CREATE_OVERRIDE_FEATURES_BIT = 0x00000008,

    // Only use application provided feature structures, don't add any profile specific
    // feature structures (robust access disable flags are ignored in this case and only the
    // application provided structures are used)
    VP_DEVICE_CREATE_OVERRIDE_ALL_FEATURES_BIT = 0x00000010,

    VP_DEVICE_CREATE_DISABLE_ROBUST_BUFFER_ACCESS_BIT = 0x00000020,
    VP_DEVICE_CREATE_DISABLE_ROBUST_IMAGE_ACCESS_BIT = 0x00000040,
    VP_DEVICE_CREATE_DISABLE_ROBUST_ACCESS =
        VP_DEVICE_CREATE_DISABLE_ROBUST_BUFFER_ACCESS_BIT | VP_DEVICE_CREATE_DISABLE_ROBUST_IMAGE_ACCESS_BIT,

    VP_DEVICE_CREATE_FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
} VpDeviceCreateFlagBits;
typedef VkFlags VpDeviceCreateFlags;

typedef struct VpDeviceCreateInfo {
    const VkDeviceCreateInfo*   pCreateInfo;
    const VpProfileProperties*  pProfile;
    VpDeviceCreateFlags         flags;
} VpDeviceCreateInfo;

// Query the list of available profiles in the library
VPAPI_ATTR VkResult vpGetProfiles(uint32_t *pPropertyCount, VpProfileProperties *pProperties);

// List the recommended fallback profiles of a profile
VPAPI_ATTR VkResult vpGetProfileFallbacks(const VpProfileProperties *pProfile, uint32_t *pPropertyCount, VpProfileProperties *pProperties);

// Check whether a profile is supported at the instance level
VPAPI_ATTR VkResult vpGetInstanceProfileSupport(const char *pLayerName, const VpProfileProperties *pProfile, VkBool32 *pSupported);

// Create a VkInstance with the profile instance extensions enabled
VPAPI_ATTR VkResult vpCreateInstance(const VpInstanceCreateInfo *pCreateInfo,
                                     const VkAllocationCallbacks *pAllocator, VkInstance *pInstance);

// Check whether a profile is supported by the physical device
VPAPI_ATTR VkResult vpGetPhysicalDeviceProfileSupport(VkInstance instance, VkPhysicalDevice physicalDevice,
                                                      const VpProfileProperties *pProfile, VkBool32 *pSupported);

// Create a VkDevice with the profile features and device extensions enabled
VPAPI_ATTR VkResult vpCreateDevice(VkPhysicalDevice physicalDevice, const VpDeviceCreateInfo *pCreateInfo,
                                   const VkAllocationCallbacks *pAllocator, VkDevice *pDevice);

// Query the list of instance extensions of a profile
VPAPI_ATTR VkResult vpGetProfileInstanceExtensionProperties(const VpProfileProperties *pProfile, uint32_t *pPropertyCount,
                                                            VkExtensionProperties *pProperties);

// Query the list of device extensions of a profile
VPAPI_ATTR VkResult vpGetProfileDeviceExtensionProperties(const VpProfileProperties *pProfile, uint32_t *pPropertyCount,
                                                          VkExtensionProperties *pProperties);

// Fill the feature structures with the requirements of a profile
VPAPI_ATTR void vpGetProfileFeatures(const VpProfileProperties *pProfile, void *pNext);

// Query the list of feature structure types specified by the profile
VPAPI_ATTR VkResult vpGetProfileFeatureStructureTypes(const VpProfileProperties *pProfile, uint32_t *pStructureTypeCount,
                                                      VkStructureType *pStructureTypes);

// Fill the property structures with the requirements of a profile
VPAPI_ATTR void vpGetProfileProperties(const VpProfileProperties *pProfile, void *pNext);

// Query the list of property structure types specified by the profile
VPAPI_ATTR VkResult vpGetProfilePropertyStructureTypes(const VpProfileProperties *pProfile, uint32_t *pStructureTypeCount,
                                                       VkStructureType *pStructureTypes);

// Query the requirements of queue families by a profile
VPAPI_ATTR VkResult vpGetProfileQueueFamilyProperties(const VpProfileProperties *pProfile, uint32_t *pPropertyCount,
                                                      VkQueueFamilyProperties2KHR *pProperties);

// Query the list of query family structure types specified by the profile
VPAPI_ATTR VkResult vpGetProfileQueueFamilyStructureTypes(const VpProfileProperties *pProfile, uint32_t *pStructureTypeCount,
                                                          VkStructureType *pStructureTypes);

// Query the list of formats with specified requirements by a profile
VPAPI_ATTR VkResult vpGetProfileFormats(const VpProfileProperties *pProfile, uint32_t *pFormatCount, VkFormat *pFormats);

// Query the requirements of a format for a profile
VPAPI_ATTR void vpGetProfileFormatProperties(const VpProfileProperties *pProfile, VkFormat format, void *pNext);

// Query the list of format structure types specified by the profile
VPAPI_ATTR VkResult vpGetProfileFormatStructureTypes(const VpProfileProperties *pProfile, uint32_t *pStructureTypeCount,
                                                     VkStructureType *pStructureTypes);

namespace detail {


VPAPI_ATTR bool isMultiple(double source, double multiple) {
    double mod = std::fmod(source, multiple);
    return std::abs(mod) < 0.0001; 
}

VPAPI_ATTR bool isPowerOfTwo(double source) {
    double mod = std::fmod(source, 1.0);
    if (std::abs(mod) >= 0.0001) return false;

    std::uint64_t value = static_cast<std::uint64_t>(std::abs(source));
    return !(value & (value - static_cast<std::uint64_t>(1)));
}

using PFN_vpStructFiller = void(*)(VkBaseOutStructure* p);
using PFN_vpStructComparator = bool(*)(VkBaseOutStructure* p);
using PFN_vpStructChainerCb =  void(*)(VkBaseOutStructure* p, void* pUser);
using PFN_vpStructChainer = void(*)(VkBaseOutStructure* p, void* pUser, PFN_vpStructChainerCb pfnCb);

struct VpFeatureDesc {
    PFN_vpStructFiller              pfnFiller;
    PFN_vpStructComparator          pfnComparator;
    PFN_vpStructChainer             pfnChainer;
};

struct VpPropertyDesc {
    PFN_vpStructFiller              pfnFiller;
    PFN_vpStructComparator          pfnComparator;
    PFN_vpStructChainer             pfnChainer;
};

struct VpQueueFamilyDesc {
    PFN_vpStructFiller              pfnFiller;
    PFN_vpStructComparator          pfnComparator;
};

struct VpFormatDesc {
    VkFormat                        format;
    PFN_vpStructFiller              pfnFiller;
    PFN_vpStructComparator          pfnComparator;
};

struct VpStructChainerDesc {
    PFN_vpStructChainer             pfnFeature;
    PFN_vpStructChainer             pfnProperty;
    PFN_vpStructChainer             pfnQueueFamily;
    PFN_vpStructChainer             pfnFormat;
};

struct VpProfileDesc {
    VpProfileProperties             props;
    uint32_t                        minApiVersion;

    const VkExtensionProperties*    pInstanceExtensions;
    uint32_t                        instanceExtensionCount;

    const VkExtensionProperties*    pDeviceExtensions;
    uint32_t                        deviceExtensionCount;

    const VpProfileProperties*      pFallbacks;
    uint32_t                        fallbackCount;

    const VkStructureType*          pFeatureStructTypes;
    uint32_t                        featureStructTypeCount;
    VpFeatureDesc                   feature;

    const VkStructureType*          pPropertyStructTypes;
    uint32_t                        propertyStructTypeCount;
    VpPropertyDesc                  property;

    const VkStructureType*          pQueueFamilyStructTypes;
    uint32_t                        queueFamilyStructTypeCount;
    const VpQueueFamilyDesc*        pQueueFamilies;
    uint32_t                        queueFamilyCount;

    const VkStructureType*          pFormatStructTypes;
    uint32_t                        formatStructTypeCount;
    const VpFormatDesc*             pFormats;
    uint32_t                        formatCount;

    VpStructChainerDesc             chainers;
};

template <typename T>
VPAPI_ATTR bool vpCheckFlags(const T& actual, const uint64_t expected) {
    return (actual & expected) == expected;
}

#ifdef VP_ANDROID_baseline_2021
namespace VP_ANDROID_BASELINE_2021 {

static const VkExtensionProperties instanceExtensions[] = {
    VkExtensionProperties{ VK_EXT_SWAPCHAIN_COLOR_SPACE_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_ANDROID_SURFACE_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_EXTERNAL_FENCE_CAPABILITIES_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_SURFACE_EXTENSION_NAME, 1 },
};

static const VkExtensionProperties deviceExtensions[] = {
    VkExtensionProperties{ VK_GOOGLE_DISPLAY_TIMING_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_DESCRIPTOR_UPDATE_TEMPLATE_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_EXTERNAL_FENCE_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_EXTERNAL_FENCE_FD_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_EXTERNAL_SEMAPHORE_FD_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_INCREMENTAL_PRESENT_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_MAINTENANCE_1_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_STORAGE_BUFFER_STORAGE_CLASS_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_SWAPCHAIN_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_VARIABLE_POINTERS_EXTENSION_NAME, 1 },
};

static const VkStructureType featureStructTypes[] = {
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR,
};

static const VkStructureType propertyStructTypes[] = {
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR,
};

static const VkStructureType formatStructTypes[] = {
    VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR,
    VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_3_KHR,
};

static const VpFeatureDesc featureDesc = {
    [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR: {
                    VkPhysicalDeviceFeatures2KHR* s = static_cast<VkPhysicalDeviceFeatures2KHR*>(static_cast<void*>(p));
                    s->features.depthBiasClamp = VK_TRUE;
                    s->features.fragmentStoresAndAtomics = VK_TRUE;
                    s->features.fullDrawIndexUint32 = VK_TRUE;
                    s->features.imageCubeArray = VK_TRUE;
                    s->features.independentBlend = VK_TRUE;
                    s->features.robustBufferAccess = VK_TRUE;
                    s->features.sampleRateShading = VK_TRUE;
                    s->features.shaderImageGatherExtended = VK_TRUE;
                    s->features.shaderSampledImageArrayDynamicIndexing = VK_TRUE;
                    s->features.shaderStorageImageArrayDynamicIndexing = VK_TRUE;
                    s->features.shaderUniformBufferArrayDynamicIndexing = VK_TRUE;
                    s->features.textureCompressionASTC_LDR = VK_TRUE;
                    s->features.textureCompressionETC2 = VK_TRUE;
                } break;
                default: break;
            }
    },
    [](VkBaseOutStructure* p) -> bool {
        bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR: {
                    VkPhysicalDeviceFeatures2KHR* s = static_cast<VkPhysicalDeviceFeatures2KHR*>(static_cast<void*>(p));
                    ret = ret && (s->features.depthBiasClamp == VK_TRUE);
                    ret = ret && (s->features.fragmentStoresAndAtomics == VK_TRUE);
                    ret = ret && (s->features.fullDrawIndexUint32 == VK_TRUE);
                    ret = ret && (s->features.imageCubeArray == VK_TRUE);
                    ret = ret && (s->features.independentBlend == VK_TRUE);
                    ret = ret && (s->features.robustBufferAccess == VK_TRUE);
                    ret = ret && (s->features.sampleRateShading == VK_TRUE);
                    ret = ret && (s->features.shaderImageGatherExtended == VK_TRUE);
                    ret = ret && (s->features.shaderSampledImageArrayDynamicIndexing == VK_TRUE);
                    ret = ret && (s->features.shaderStorageImageArrayDynamicIndexing == VK_TRUE);
                    ret = ret && (s->features.shaderUniformBufferArrayDynamicIndexing == VK_TRUE);
                    ret = ret && (s->features.textureCompressionASTC_LDR == VK_TRUE);
                    ret = ret && (s->features.textureCompressionETC2 == VK_TRUE);
                } break;
                default: break;
            }
        return ret;
    }
};

static const VpPropertyDesc propertyDesc = {
    [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR: {
                    VkPhysicalDeviceProperties2KHR* s = static_cast<VkPhysicalDeviceProperties2KHR*>(static_cast<void*>(p));
                    s->properties.limits.discreteQueuePriorities = 2;
                    s->properties.limits.framebufferColorSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.framebufferDepthSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.framebufferNoAttachmentsSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.framebufferStencilSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.maxBoundDescriptorSets = 4;
                    s->properties.limits.maxColorAttachments = 4;
                    s->properties.limits.maxComputeSharedMemorySize = 16384;
                    s->properties.limits.maxComputeWorkGroupCount[0] = 65535;
                    s->properties.limits.maxComputeWorkGroupCount[1] = 65535;
                    s->properties.limits.maxComputeWorkGroupCount[2] = 65535;
                    s->properties.limits.maxComputeWorkGroupInvocations = 128;
                    s->properties.limits.maxComputeWorkGroupSize[0] = 128;
                    s->properties.limits.maxComputeWorkGroupSize[1] = 128;
                    s->properties.limits.maxComputeWorkGroupSize[2] = 64;
                    s->properties.limits.maxDescriptorSetInputAttachments = 4;
                    s->properties.limits.maxDescriptorSetSampledImages = 48;
                    s->properties.limits.maxDescriptorSetSamplers = 48;
                    s->properties.limits.maxDescriptorSetStorageBuffers = 24;
                    s->properties.limits.maxDescriptorSetStorageBuffersDynamic = 4;
                    s->properties.limits.maxDescriptorSetStorageImages = 12;
                    s->properties.limits.maxDescriptorSetUniformBuffers = 36;
                    s->properties.limits.maxDescriptorSetUniformBuffersDynamic = 8;
                    s->properties.limits.maxDrawIndexedIndexValue = 4294967295;
                    s->properties.limits.maxDrawIndirectCount = 1;
                    s->properties.limits.maxFragmentCombinedOutputResources = 8;
                    s->properties.limits.maxFragmentInputComponents = 64;
                    s->properties.limits.maxFragmentOutputAttachments = 4;
                    s->properties.limits.maxFramebufferHeight = 4096;
                    s->properties.limits.maxFramebufferLayers = 256;
                    s->properties.limits.maxFramebufferWidth = 4096;
                    s->properties.limits.maxImageArrayLayers = 256;
                    s->properties.limits.maxImageDimension1D = 4096;
                    s->properties.limits.maxImageDimension2D = 4096;
                    s->properties.limits.maxImageDimension3D = 512;
                    s->properties.limits.maxImageDimensionCube = 4096;
                    s->properties.limits.maxInterpolationOffset = 0.4375f;
                    s->properties.limits.maxMemoryAllocationCount = 4096;
                    s->properties.limits.maxPerStageDescriptorInputAttachments = 4;
                    s->properties.limits.maxPerStageDescriptorSampledImages = 16;
                    s->properties.limits.maxPerStageDescriptorSamplers = 16;
                    s->properties.limits.maxPerStageDescriptorStorageBuffers = 4;
                    s->properties.limits.maxPerStageDescriptorStorageImages = 4;
                    s->properties.limits.maxPerStageDescriptorUniformBuffers = 12;
                    s->properties.limits.maxPerStageResources = 44;
                    s->properties.limits.maxPushConstantsSize = 128;
                    s->properties.limits.maxSampleMaskWords = 1;
                    s->properties.limits.maxSamplerAllocationCount = 4000;
                    s->properties.limits.maxSamplerAnisotropy = 1.0f;
                    s->properties.limits.maxSamplerLodBias = 2.0f;
                    s->properties.limits.maxStorageBufferRange = 134217728;
                    s->properties.limits.maxTexelBufferElements = 65536;
                    s->properties.limits.maxTexelGatherOffset = 7;
                    s->properties.limits.maxTexelOffset = 7;
                    s->properties.limits.maxUniformBufferRange = 16384;
                    s->properties.limits.maxVertexInputAttributeOffset = 2047;
                    s->properties.limits.maxVertexInputAttributes = 16;
                    s->properties.limits.maxVertexInputBindingStride = 2048;
                    s->properties.limits.maxVertexInputBindings = 16;
                    s->properties.limits.maxVertexOutputComponents = 64;
                    s->properties.limits.maxViewportDimensions[0] = 4096;
                    s->properties.limits.maxViewportDimensions[1] = 4096;
                    s->properties.limits.maxViewports = 1;
                    s->properties.limits.minInterpolationOffset = -0.5f;
                    s->properties.limits.minMemoryMapAlignment = 4096;
                    s->properties.limits.minStorageBufferOffsetAlignment = 256;
                    s->properties.limits.minTexelBufferOffsetAlignment = 256;
                    s->properties.limits.minTexelGatherOffset = -8;
                    s->properties.limits.minTexelOffset = -8;
                    s->properties.limits.minUniformBufferOffsetAlignment = 256;
                    s->properties.limits.mipmapPrecisionBits = 4;
                    s->properties.limits.pointSizeGranularity = 1;
                    s->properties.limits.sampledImageColorSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.sampledImageDepthSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.sampledImageIntegerSampleCounts = (VK_SAMPLE_COUNT_1_BIT);
                    s->properties.limits.sampledImageStencilSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.standardSampleLocations = VK_TRUE;
                    s->properties.limits.storageImageSampleCounts = (VK_SAMPLE_COUNT_1_BIT);
                    s->properties.limits.subPixelInterpolationOffsetBits = 4;
                    s->properties.limits.subPixelPrecisionBits = 4;
                    s->properties.limits.subTexelPrecisionBits = 4;
                    s->properties.limits.viewportBoundsRange[0] = -8192;
                    s->properties.limits.viewportBoundsRange[1] = 8191;
                } break;
                default: break;
            }
    },
    [](VkBaseOutStructure* p) -> bool {
        bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR: {
                    VkPhysicalDeviceProperties2KHR* s = static_cast<VkPhysicalDeviceProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (s->properties.limits.discreteQueuePriorities >= 2);
                    ret = ret && (vpCheckFlags(s->properties.limits.framebufferColorSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (vpCheckFlags(s->properties.limits.framebufferDepthSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (vpCheckFlags(s->properties.limits.framebufferNoAttachmentsSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (vpCheckFlags(s->properties.limits.framebufferStencilSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (s->properties.limits.maxBoundDescriptorSets >= 4);
                    ret = ret && (s->properties.limits.maxColorAttachments >= 4);
                    ret = ret && (s->properties.limits.maxComputeSharedMemorySize >= 16384);
                    ret = ret && (s->properties.limits.maxComputeWorkGroupCount[0] >= 65535);
                    ret = ret && (s->properties.limits.maxComputeWorkGroupCount[1] >= 65535);
                    ret = ret && (s->properties.limits.maxComputeWorkGroupCount[2] >= 65535);
                    ret = ret && (s->properties.limits.maxComputeWorkGroupInvocations >= 128);
                    ret = ret && (s->properties.limits.maxComputeWorkGroupSize[0] >= 128);
                    ret = ret && (s->properties.limits.maxComputeWorkGroupSize[1] >= 128);
                    ret = ret && (s->properties.limits.maxComputeWorkGroupSize[2] >= 64);
                    ret = ret && (s->properties.limits.maxDescriptorSetInputAttachments >= 4);
                    ret = ret && (s->properties.limits.maxDescriptorSetSampledImages >= 48);
                    ret = ret && (s->properties.limits.maxDescriptorSetSamplers >= 48);
                    ret = ret && (s->properties.limits.maxDescriptorSetStorageBuffers >= 24);
                    ret = ret && (s->properties.limits.maxDescriptorSetStorageBuffersDynamic >= 4);
                    ret = ret && (s->properties.limits.maxDescriptorSetStorageImages >= 12);
                    ret = ret && (s->properties.limits.maxDescriptorSetUniformBuffers >= 36);
                    ret = ret && (s->properties.limits.maxDescriptorSetUniformBuffersDynamic >= 8);
                    ret = ret && (s->properties.limits.maxDrawIndexedIndexValue >= 4294967295);
                    ret = ret && (s->properties.limits.maxDrawIndirectCount >= 1);
                    ret = ret && (s->properties.limits.maxFragmentCombinedOutputResources >= 8);
                    ret = ret && (s->properties.limits.maxFragmentInputComponents >= 64);
                    ret = ret && (s->properties.limits.maxFragmentOutputAttachments >= 4);
                    ret = ret && (s->properties.limits.maxFramebufferHeight >= 4096);
                    ret = ret && (s->properties.limits.maxFramebufferLayers >= 256);
                    ret = ret && (s->properties.limits.maxFramebufferWidth >= 4096);
                    ret = ret && (s->properties.limits.maxImageArrayLayers >= 256);
                    ret = ret && (s->properties.limits.maxImageDimension1D >= 4096);
                    ret = ret && (s->properties.limits.maxImageDimension2D >= 4096);
                    ret = ret && (s->properties.limits.maxImageDimension3D >= 512);
                    ret = ret && (s->properties.limits.maxImageDimensionCube >= 4096);
                    ret = ret && (s->properties.limits.maxInterpolationOffset >= 0.4375);
                    ret = ret && (s->properties.limits.maxMemoryAllocationCount >= 4096);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorInputAttachments >= 4);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorSampledImages >= 16);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorSamplers >= 16);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorStorageBuffers >= 4);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorStorageImages >= 4);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorUniformBuffers >= 12);
                    ret = ret && (s->properties.limits.maxPerStageResources >= 44);
                    ret = ret && (s->properties.limits.maxPushConstantsSize >= 128);
                    ret = ret && (s->properties.limits.maxSampleMaskWords >= 1);
                    ret = ret && (s->properties.limits.maxSamplerAllocationCount >= 4000);
                    ret = ret && (s->properties.limits.maxSamplerAnisotropy >= 1.0);
                    ret = ret && (s->properties.limits.maxSamplerLodBias >= 2.0);
                    ret = ret && (s->properties.limits.maxStorageBufferRange >= 134217728);
                    ret = ret && (s->properties.limits.maxTexelBufferElements >= 65536);
                    ret = ret && (s->properties.limits.maxTexelGatherOffset >= 7);
                    ret = ret && (s->properties.limits.maxTexelOffset >= 7);
                    ret = ret && (s->properties.limits.maxUniformBufferRange >= 16384);
                    ret = ret && (s->properties.limits.maxVertexInputAttributeOffset >= 2047);
                    ret = ret && (s->properties.limits.maxVertexInputAttributes >= 16);
                    ret = ret && (s->properties.limits.maxVertexInputBindingStride >= 2048);
                    ret = ret && (s->properties.limits.maxVertexInputBindings >= 16);
                    ret = ret && (s->properties.limits.maxVertexOutputComponents >= 64);
                    ret = ret && (s->properties.limits.maxViewportDimensions[0] >= 4096);
                    ret = ret && (s->properties.limits.maxViewportDimensions[1] >= 4096);
                    ret = ret && (s->properties.limits.maxViewports >= 1);
                    ret = ret && (s->properties.limits.minInterpolationOffset <= -0.5);
                    ret = ret && (s->properties.limits.minMemoryMapAlignment <= 4096);
                    ret = ret && ((s->properties.limits.minMemoryMapAlignment & (s->properties.limits.minMemoryMapAlignment - 1)) == 0);
                    ret = ret && (s->properties.limits.minStorageBufferOffsetAlignment <= 256);
                    ret = ret && ((s->properties.limits.minStorageBufferOffsetAlignment & (s->properties.limits.minStorageBufferOffsetAlignment - 1)) == 0);
                    ret = ret && (s->properties.limits.minTexelBufferOffsetAlignment <= 256);
                    ret = ret && ((s->properties.limits.minTexelBufferOffsetAlignment & (s->properties.limits.minTexelBufferOffsetAlignment - 1)) == 0);
                    ret = ret && (s->properties.limits.minTexelGatherOffset <= -8);
                    ret = ret && (s->properties.limits.minTexelOffset <= -8);
                    ret = ret && (s->properties.limits.minUniformBufferOffsetAlignment <= 256);
                    ret = ret && ((s->properties.limits.minUniformBufferOffsetAlignment & (s->properties.limits.minUniformBufferOffsetAlignment - 1)) == 0);
                    ret = ret && (s->properties.limits.mipmapPrecisionBits >= 4);
                    ret = ret && (s->properties.limits.pointSizeGranularity <= 1);
                    ret = ret && (isMultiple(1, s->properties.limits.pointSizeGranularity));
                    ret = ret && (vpCheckFlags(s->properties.limits.sampledImageColorSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (vpCheckFlags(s->properties.limits.sampledImageDepthSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (vpCheckFlags(s->properties.limits.sampledImageIntegerSampleCounts, (VK_SAMPLE_COUNT_1_BIT)));
                    ret = ret && (vpCheckFlags(s->properties.limits.sampledImageStencilSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (s->properties.limits.standardSampleLocations == VK_TRUE);
                    ret = ret && (vpCheckFlags(s->properties.limits.storageImageSampleCounts, (VK_SAMPLE_COUNT_1_BIT)));
                    ret = ret && (s->properties.limits.subPixelInterpolationOffsetBits >= 4);
                    ret = ret && (s->properties.limits.subPixelPrecisionBits >= 4);
                    ret = ret && (s->properties.limits.subTexelPrecisionBits >= 4);
                    ret = ret && (s->properties.limits.viewportBoundsRange[0] <= -8192);
                    ret = ret && (s->properties.limits.viewportBoundsRange[1] >= 8191);
                } break;
                default: break;
            }
        return ret;
    }
};

static const VpFormatDesc formatDesc[] = {
    {
        VK_FORMAT_A1R5G5B5_UNORM_PACK16,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A2B10G10R10_UINT_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A2B10G10R10_UNORM_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A8B8G8R8_SINT_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A8B8G8R8_SNORM_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A8B8G8R8_SRGB_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A8B8G8R8_UINT_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A8B8G8R8_UNORM_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_10x10_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_10x10_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_10x5_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_10x5_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_10x6_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_10x6_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_10x8_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_10x8_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_12x10_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_12x10_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_12x12_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_12x12_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_4x4_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_4x4_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_5x4_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_5x4_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_5x5_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_5x5_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_6x5_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_6x5_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_6x6_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_6x6_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_8x5_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_8x5_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_8x6_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_8x6_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_8x8_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ASTC_8x8_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_B10G11R11_UFLOAT_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_B4G4R4A4_UNORM_PACK16,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_B8G8R8A8_SRGB,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_B8G8R8A8_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_D16_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_D32_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_E5B9G9R9_UFLOAT_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_EAC_R11G11_SNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_EAC_R11G11_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_EAC_R11_SNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_EAC_R11_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16B16A16_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16B16A16_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16B16A16_SNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16B16A16_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16_SNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16_SNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32B32A32_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32B32A32_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32B32A32_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R5G6B5_UNORM_PACK16,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8B8A8_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8B8A8_SNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8B8A8_SRGB,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8B8A8_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8B8A8_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8_SNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8_SNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
};

static const VpStructChainerDesc chainerDesc = {
    [](VkBaseOutStructure* p, void* pUser, PFN_vpStructChainerCb pfnCb) {
        p->pNext = static_cast<VkBaseOutStructure*>(static_cast<void*>(nullptr));
        pfnCb(p, pUser);
    },
    [](VkBaseOutStructure* p, void* pUser, PFN_vpStructChainerCb pfnCb) {
        p->pNext = static_cast<VkBaseOutStructure*>(static_cast<void*>(nullptr));
        pfnCb(p, pUser);
    },
    [](VkBaseOutStructure* p, void* pUser, PFN_vpStructChainerCb pfnCb) {
        pfnCb(p, pUser);
    },
    [](VkBaseOutStructure* p, void* pUser, PFN_vpStructChainerCb pfnCb) {
        VkFormatProperties3KHR formatProperties3KHR{ VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_3_KHR, nullptr };
        p->pNext = static_cast<VkBaseOutStructure*>(static_cast<void*>(&formatProperties3KHR));
        pfnCb(p, pUser);
    },
};

} // namespace VP_ANDROID_BASELINE_2021
#endif

#ifdef VP_KHR_roadmap_2022
namespace VP_KHR_ROADMAP_2022 {

static const VkExtensionProperties deviceExtensions[] = {
    VkExtensionProperties{ VK_KHR_GLOBAL_PRIORITY_EXTENSION_NAME, 1 },
};

static const VkStructureType featureStructTypes[] = {
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
};

static const VkStructureType propertyStructTypes[] = {
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES,
};

static const VpFeatureDesc featureDesc = {
    [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR: {
                    VkPhysicalDeviceFeatures2KHR* s = static_cast<VkPhysicalDeviceFeatures2KHR*>(static_cast<void*>(p));
                    s->features.depthBiasClamp = VK_TRUE;
                    s->features.depthClamp = VK_TRUE;
                    s->features.drawIndirectFirstInstance = VK_TRUE;
                    s->features.fragmentStoresAndAtomics = VK_TRUE;
                    s->features.fullDrawIndexUint32 = VK_TRUE;
                    s->features.imageCubeArray = VK_TRUE;
                    s->features.independentBlend = VK_TRUE;
                    s->features.occlusionQueryPrecise = VK_TRUE;
                    s->features.robustBufferAccess = VK_TRUE;
                    s->features.sampleRateShading = VK_TRUE;
                    s->features.samplerAnisotropy = VK_TRUE;
                    s->features.shaderSampledImageArrayDynamicIndexing = VK_TRUE;
                    s->features.shaderStorageBufferArrayDynamicIndexing = VK_TRUE;
                    s->features.shaderStorageImageArrayDynamicIndexing = VK_TRUE;
                    s->features.shaderStorageImageExtendedFormats = VK_TRUE;
                    s->features.shaderUniformBufferArrayDynamicIndexing = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES: {
                    VkPhysicalDeviceVulkan11Features* s = static_cast<VkPhysicalDeviceVulkan11Features*>(static_cast<void*>(p));
                    s->multiview = VK_TRUE;
                    s->samplerYcbcrConversion = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES: {
                    VkPhysicalDeviceVulkan12Features* s = static_cast<VkPhysicalDeviceVulkan12Features*>(static_cast<void*>(p));
                    s->bufferDeviceAddress = VK_TRUE;
                    s->descriptorBindingPartiallyBound = VK_TRUE;
                    s->descriptorBindingSampledImageUpdateAfterBind = VK_TRUE;
                    s->descriptorBindingStorageBufferUpdateAfterBind = VK_TRUE;
                    s->descriptorBindingStorageImageUpdateAfterBind = VK_TRUE;
                    s->descriptorBindingStorageTexelBufferUpdateAfterBind = VK_TRUE;
                    s->descriptorBindingUniformTexelBufferUpdateAfterBind = VK_TRUE;
                    s->descriptorBindingUpdateUnusedWhilePending = VK_TRUE;
                    s->descriptorBindingVariableDescriptorCount = VK_TRUE;
                    s->descriptorIndexing = VK_TRUE;
                    s->hostQueryReset = VK_TRUE;
                    s->imagelessFramebuffer = VK_TRUE;
                    s->runtimeDescriptorArray = VK_TRUE;
                    s->samplerMirrorClampToEdge = VK_TRUE;
                    s->scalarBlockLayout = VK_TRUE;
                    s->separateDepthStencilLayouts = VK_TRUE;
                    s->shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
                    s->shaderStorageBufferArrayNonUniformIndexing = VK_TRUE;
                    s->shaderStorageImageArrayNonUniformIndexing = VK_TRUE;
                    s->shaderStorageTexelBufferArrayDynamicIndexing = VK_TRUE;
                    s->shaderStorageTexelBufferArrayNonUniformIndexing = VK_TRUE;
                    s->shaderSubgroupExtendedTypes = VK_TRUE;
                    s->shaderUniformBufferArrayNonUniformIndexing = VK_TRUE;
                    s->shaderUniformTexelBufferArrayDynamicIndexing = VK_TRUE;
                    s->shaderUniformTexelBufferArrayNonUniformIndexing = VK_TRUE;
                    s->subgroupBroadcastDynamicId = VK_TRUE;
                    s->timelineSemaphore = VK_TRUE;
                    s->uniformBufferStandardLayout = VK_TRUE;
                    s->vulkanMemoryModel = VK_TRUE;
                    s->vulkanMemoryModelDeviceScope = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES: {
                    VkPhysicalDeviceVulkan13Features* s = static_cast<VkPhysicalDeviceVulkan13Features*>(static_cast<void*>(p));
                    s->computeFullSubgroups = VK_TRUE;
                    s->descriptorBindingInlineUniformBlockUpdateAfterBind = VK_TRUE;
                    s->dynamicRendering = VK_TRUE;
                    s->inlineUniformBlock = VK_TRUE;
                    s->maintenance4 = VK_TRUE;
                    s->pipelineCreationCacheControl = VK_TRUE;
                    s->robustImageAccess = VK_TRUE;
                    s->shaderDemoteToHelperInvocation = VK_TRUE;
                    s->shaderIntegerDotProduct = VK_TRUE;
                    s->shaderTerminateInvocation = VK_TRUE;
                    s->shaderZeroInitializeWorkgroupMemory = VK_TRUE;
                    s->subgroupSizeControl = VK_TRUE;
                    s->synchronization2 = VK_TRUE;
                } break;
                default: break;
            }
    },
    [](VkBaseOutStructure* p) -> bool {
        bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR: {
                    VkPhysicalDeviceFeatures2KHR* s = static_cast<VkPhysicalDeviceFeatures2KHR*>(static_cast<void*>(p));
                    ret = ret && (s->features.depthBiasClamp == VK_TRUE);
                    ret = ret && (s->features.depthClamp == VK_TRUE);
                    ret = ret && (s->features.drawIndirectFirstInstance == VK_TRUE);
                    ret = ret && (s->features.fragmentStoresAndAtomics == VK_TRUE);
                    ret = ret && (s->features.fullDrawIndexUint32 == VK_TRUE);
                    ret = ret && (s->features.imageCubeArray == VK_TRUE);
                    ret = ret && (s->features.independentBlend == VK_TRUE);
                    ret = ret && (s->features.occlusionQueryPrecise == VK_TRUE);
                    ret = ret && (s->features.robustBufferAccess == VK_TRUE);
                    ret = ret && (s->features.sampleRateShading == VK_TRUE);
                    ret = ret && (s->features.samplerAnisotropy == VK_TRUE);
                    ret = ret && (s->features.shaderSampledImageArrayDynamicIndexing == VK_TRUE);
                    ret = ret && (s->features.shaderStorageBufferArrayDynamicIndexing == VK_TRUE);
                    ret = ret && (s->features.shaderStorageImageArrayDynamicIndexing == VK_TRUE);
                    ret = ret && (s->features.shaderStorageImageExtendedFormats == VK_TRUE);
                    ret = ret && (s->features.shaderUniformBufferArrayDynamicIndexing == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES: {
                    VkPhysicalDeviceVulkan11Features* s = static_cast<VkPhysicalDeviceVulkan11Features*>(static_cast<void*>(p));
                    ret = ret && (s->multiview == VK_TRUE);
                    ret = ret && (s->samplerYcbcrConversion == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES: {
                    VkPhysicalDeviceVulkan12Features* s = static_cast<VkPhysicalDeviceVulkan12Features*>(static_cast<void*>(p));
                    ret = ret && (s->bufferDeviceAddress == VK_TRUE);
                    ret = ret && (s->descriptorBindingPartiallyBound == VK_TRUE);
                    ret = ret && (s->descriptorBindingSampledImageUpdateAfterBind == VK_TRUE);
                    ret = ret && (s->descriptorBindingStorageBufferUpdateAfterBind == VK_TRUE);
                    ret = ret && (s->descriptorBindingStorageImageUpdateAfterBind == VK_TRUE);
                    ret = ret && (s->descriptorBindingStorageTexelBufferUpdateAfterBind == VK_TRUE);
                    ret = ret && (s->descriptorBindingUniformTexelBufferUpdateAfterBind == VK_TRUE);
                    ret = ret && (s->descriptorBindingUpdateUnusedWhilePending == VK_TRUE);
                    ret = ret && (s->descriptorBindingVariableDescriptorCount == VK_TRUE);
                    ret = ret && (s->descriptorIndexing == VK_TRUE);
                    ret = ret && (s->hostQueryReset == VK_TRUE);
                    ret = ret && (s->imagelessFramebuffer == VK_TRUE);
                    ret = ret && (s->runtimeDescriptorArray == VK_TRUE);
                    ret = ret && (s->samplerMirrorClampToEdge == VK_TRUE);
                    ret = ret && (s->scalarBlockLayout == VK_TRUE);
                    ret = ret && (s->separateDepthStencilLayouts == VK_TRUE);
                    ret = ret && (s->shaderSampledImageArrayNonUniformIndexing == VK_TRUE);
                    ret = ret && (s->shaderStorageBufferArrayNonUniformIndexing == VK_TRUE);
                    ret = ret && (s->shaderStorageImageArrayNonUniformIndexing == VK_TRUE);
                    ret = ret && (s->shaderStorageTexelBufferArrayDynamicIndexing == VK_TRUE);
                    ret = ret && (s->shaderStorageTexelBufferArrayNonUniformIndexing == VK_TRUE);
                    ret = ret && (s->shaderSubgroupExtendedTypes == VK_TRUE);
                    ret = ret && (s->shaderUniformBufferArrayNonUniformIndexing == VK_TRUE);
                    ret = ret && (s->shaderUniformTexelBufferArrayDynamicIndexing == VK_TRUE);
                    ret = ret && (s->shaderUniformTexelBufferArrayNonUniformIndexing == VK_TRUE);
                    ret = ret && (s->subgroupBroadcastDynamicId == VK_TRUE);
                    ret = ret && (s->timelineSemaphore == VK_TRUE);
                    ret = ret && (s->uniformBufferStandardLayout == VK_TRUE);
                    ret = ret && (s->vulkanMemoryModel == VK_TRUE);
                    ret = ret && (s->vulkanMemoryModelDeviceScope == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES: {
                    VkPhysicalDeviceVulkan13Features* s = static_cast<VkPhysicalDeviceVulkan13Features*>(static_cast<void*>(p));
                    ret = ret && (s->computeFullSubgroups == VK_TRUE);
                    ret = ret && (s->descriptorBindingInlineUniformBlockUpdateAfterBind == VK_TRUE);
                    ret = ret && (s->dynamicRendering == VK_TRUE);
                    ret = ret && (s->inlineUniformBlock == VK_TRUE);
                    ret = ret && (s->maintenance4 == VK_TRUE);
                    ret = ret && (s->pipelineCreationCacheControl == VK_TRUE);
                    ret = ret && (s->robustImageAccess == VK_TRUE);
                    ret = ret && (s->shaderDemoteToHelperInvocation == VK_TRUE);
                    ret = ret && (s->shaderIntegerDotProduct == VK_TRUE);
                    ret = ret && (s->shaderTerminateInvocation == VK_TRUE);
                    ret = ret && (s->shaderZeroInitializeWorkgroupMemory == VK_TRUE);
                    ret = ret && (s->subgroupSizeControl == VK_TRUE);
                    ret = ret && (s->synchronization2 == VK_TRUE);
                } break;
                default: break;
            }
        return ret;
    }
};

static const VpPropertyDesc propertyDesc = {
    [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR: {
                    VkPhysicalDeviceProperties2KHR* s = static_cast<VkPhysicalDeviceProperties2KHR*>(static_cast<void*>(p));
                    s->properties.limits.bufferImageGranularity = 4096;
                    s->properties.limits.maxColorAttachments = 7;
                    s->properties.limits.maxComputeWorkGroupInvocations = 256;
                    s->properties.limits.maxComputeWorkGroupSize[0] = 256;
                    s->properties.limits.maxComputeWorkGroupSize[1] = 256;
                    s->properties.limits.maxComputeWorkGroupSize[2] = 64;
                    s->properties.limits.maxDescriptorSetSampledImages = 1800;
                    s->properties.limits.maxDescriptorSetSamplers = 576;
                    s->properties.limits.maxDescriptorSetStorageBuffers = 96;
                    s->properties.limits.maxDescriptorSetStorageImages = 144;
                    s->properties.limits.maxDescriptorSetUniformBuffers = 90;
                    s->properties.limits.maxFragmentCombinedOutputResources = 16;
                    s->properties.limits.maxImageArrayLayers = 2048;
                    s->properties.limits.maxImageDimension1D = 8192;
                    s->properties.limits.maxImageDimension2D = 8192;
                    s->properties.limits.maxImageDimensionCube = 8192;
                    s->properties.limits.maxPerStageDescriptorSampledImages = 200;
                    s->properties.limits.maxPerStageDescriptorSamplers = 64;
                    s->properties.limits.maxPerStageDescriptorStorageBuffers = 30;
                    s->properties.limits.maxPerStageDescriptorStorageImages = 16;
                    s->properties.limits.maxPerStageDescriptorUniformBuffers = 15;
                    s->properties.limits.maxPerStageResources = 200;
                    s->properties.limits.maxSamplerLodBias = 14;
                    s->properties.limits.maxUniformBufferRange = 65536;
                    s->properties.limits.mipmapPrecisionBits = 6;
                    s->properties.limits.standardSampleLocations = VK_TRUE;
                    s->properties.limits.subTexelPrecisionBits = 8;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES: {
                    VkPhysicalDeviceVulkan11Properties* s = static_cast<VkPhysicalDeviceVulkan11Properties*>(static_cast<void*>(p));
                    s->maxMultiviewInstanceIndex = 134217727;
                    s->maxMultiviewViewCount = 6;
                    s->subgroupSize = 4;
                    s->subgroupSupportedOperations = (VK_SUBGROUP_FEATURE_BASIC_BIT | VK_SUBGROUP_FEATURE_VOTE_BIT | VK_SUBGROUP_FEATURE_ARITHMETIC_BIT | VK_SUBGROUP_FEATURE_BALLOT_BIT | VK_SUBGROUP_FEATURE_SHUFFLE_BIT | VK_SUBGROUP_FEATURE_SHUFFLE_RELATIVE_BIT | VK_SUBGROUP_FEATURE_QUAD_BIT);
                    s->subgroupSupportedStages = (VK_SHADER_STAGE_COMPUTE_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES: {
                    VkPhysicalDeviceVulkan12Properties* s = static_cast<VkPhysicalDeviceVulkan12Properties*>(static_cast<void*>(p));
                    s->maxDescriptorSetUpdateAfterBindInputAttachments = 7;
                    s->maxDescriptorSetUpdateAfterBindSampledImages = 500000;
                    s->maxDescriptorSetUpdateAfterBindSamplers = 500000;
                    s->maxDescriptorSetUpdateAfterBindStorageBuffers = 500000;
                    s->maxDescriptorSetUpdateAfterBindStorageBuffersDynamic = 4;
                    s->maxDescriptorSetUpdateAfterBindStorageImages = 500000;
                    s->maxDescriptorSetUpdateAfterBindUniformBuffers = 72;
                    s->maxDescriptorSetUpdateAfterBindUniformBuffersDynamic = 8;
                    s->maxPerStageDescriptorUpdateAfterBindInputAttachments = 7;
                    s->maxPerStageDescriptorUpdateAfterBindSampledImages = 500000;
                    s->maxPerStageDescriptorUpdateAfterBindSamplers = 500000;
                    s->maxPerStageDescriptorUpdateAfterBindStorageBuffers = 500000;
                    s->maxPerStageDescriptorUpdateAfterBindStorageImages = 500000;
                    s->maxPerStageDescriptorUpdateAfterBindUniformBuffers = 12;
                    s->maxPerStageUpdateAfterBindResources = 500000;
                    s->maxTimelineSemaphoreValueDifference = 2147483647;
                    s->shaderSignedZeroInfNanPreserveFloat16 = VK_TRUE;
                    s->shaderSignedZeroInfNanPreserveFloat32 = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES: {
                    VkPhysicalDeviceVulkan13Properties* s = static_cast<VkPhysicalDeviceVulkan13Properties*>(static_cast<void*>(p));
                    s->maxBufferSize = 1073741824;
                    s->maxDescriptorSetInlineUniformBlocks = 4;
                    s->maxDescriptorSetUpdateAfterBindInlineUniformBlocks = 4;
                    s->maxInlineUniformBlockSize = 256;
                    s->maxInlineUniformTotalSize = 256;
                    s->maxPerStageDescriptorInlineUniformBlocks = 4;
                    s->maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks = 4;
                } break;
                default: break;
            }
    },
    [](VkBaseOutStructure* p) -> bool {
        bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR: {
                    VkPhysicalDeviceProperties2KHR* s = static_cast<VkPhysicalDeviceProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (s->properties.limits.bufferImageGranularity <= 4096);
                    ret = ret && ((4096 % s->properties.limits.bufferImageGranularity) == 0);
                    ret = ret && (s->properties.limits.maxColorAttachments >= 7);
                    ret = ret && (s->properties.limits.maxComputeWorkGroupInvocations >= 256);
                    ret = ret && (s->properties.limits.maxComputeWorkGroupSize[0] >= 256);
                    ret = ret && (s->properties.limits.maxComputeWorkGroupSize[1] >= 256);
                    ret = ret && (s->properties.limits.maxComputeWorkGroupSize[2] >= 64);
                    ret = ret && (s->properties.limits.maxDescriptorSetSampledImages >= 1800);
                    ret = ret && (s->properties.limits.maxDescriptorSetSamplers >= 576);
                    ret = ret && (s->properties.limits.maxDescriptorSetStorageBuffers >= 96);
                    ret = ret && (s->properties.limits.maxDescriptorSetStorageImages >= 144);
                    ret = ret && (s->properties.limits.maxDescriptorSetUniformBuffers >= 90);
                    ret = ret && (s->properties.limits.maxFragmentCombinedOutputResources >= 16);
                    ret = ret && (s->properties.limits.maxImageArrayLayers >= 2048);
                    ret = ret && (s->properties.limits.maxImageDimension1D >= 8192);
                    ret = ret && (s->properties.limits.maxImageDimension2D >= 8192);
                    ret = ret && (s->properties.limits.maxImageDimensionCube >= 8192);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorSampledImages >= 200);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorSamplers >= 64);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorStorageBuffers >= 30);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorStorageImages >= 16);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorUniformBuffers >= 15);
                    ret = ret && (s->properties.limits.maxPerStageResources >= 200);
                    ret = ret && (s->properties.limits.maxSamplerLodBias >= 14);
                    ret = ret && (s->properties.limits.maxUniformBufferRange >= 65536);
                    ret = ret && (s->properties.limits.mipmapPrecisionBits >= 6);
                    ret = ret && (s->properties.limits.standardSampleLocations == VK_TRUE);
                    ret = ret && (s->properties.limits.subTexelPrecisionBits >= 8);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES: {
                    VkPhysicalDeviceVulkan11Properties* s = static_cast<VkPhysicalDeviceVulkan11Properties*>(static_cast<void*>(p));
                    ret = ret && (s->maxMultiviewInstanceIndex >= 134217727);
                    ret = ret && (s->maxMultiviewViewCount >= 6);
                    ret = ret && (s->subgroupSize >= 4);
                    ret = ret && ((s->subgroupSize & (s->subgroupSize - 1)) == 0);
                    ret = ret && (vpCheckFlags(s->subgroupSupportedOperations, (VK_SUBGROUP_FEATURE_BASIC_BIT | VK_SUBGROUP_FEATURE_VOTE_BIT | VK_SUBGROUP_FEATURE_ARITHMETIC_BIT | VK_SUBGROUP_FEATURE_BALLOT_BIT | VK_SUBGROUP_FEATURE_SHUFFLE_BIT | VK_SUBGROUP_FEATURE_SHUFFLE_RELATIVE_BIT | VK_SUBGROUP_FEATURE_QUAD_BIT)));
                    ret = ret && (vpCheckFlags(s->subgroupSupportedStages, (VK_SHADER_STAGE_COMPUTE_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)));
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES: {
                    VkPhysicalDeviceVulkan12Properties* s = static_cast<VkPhysicalDeviceVulkan12Properties*>(static_cast<void*>(p));
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindInputAttachments >= 7);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindSampledImages >= 500000);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindSamplers >= 500000);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindStorageBuffers >= 500000);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindStorageBuffersDynamic >= 4);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindStorageImages >= 500000);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindUniformBuffers >= 72);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindUniformBuffersDynamic >= 8);
                    ret = ret && (s->maxPerStageDescriptorUpdateAfterBindInputAttachments >= 7);
                    ret = ret && (s->maxPerStageDescriptorUpdateAfterBindSampledImages >= 500000);
                    ret = ret && (s->maxPerStageDescriptorUpdateAfterBindSamplers >= 500000);
                    ret = ret && (s->maxPerStageDescriptorUpdateAfterBindStorageBuffers >= 500000);
                    ret = ret && (s->maxPerStageDescriptorUpdateAfterBindStorageImages >= 500000);
                    ret = ret && (s->maxPerStageDescriptorUpdateAfterBindUniformBuffers >= 12);
                    ret = ret && (s->maxPerStageUpdateAfterBindResources >= 500000);
                    ret = ret && (s->maxTimelineSemaphoreValueDifference >= 2147483647);
                    ret = ret && (vpCheckFlags(s->shaderSignedZeroInfNanPreserveFloat16, VK_TRUE));
                    ret = ret && (vpCheckFlags(s->shaderSignedZeroInfNanPreserveFloat32, VK_TRUE));
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES: {
                    VkPhysicalDeviceVulkan13Properties* s = static_cast<VkPhysicalDeviceVulkan13Properties*>(static_cast<void*>(p));
                    ret = ret && (s->maxBufferSize >= 1073741824);
                    ret = ret && (s->maxDescriptorSetInlineUniformBlocks >= 4);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindInlineUniformBlocks >= 4);
                    ret = ret && (s->maxInlineUniformBlockSize >= 256);
                    ret = ret && (s->maxInlineUniformTotalSize >= 256);
                    ret = ret && (s->maxPerStageDescriptorInlineUniformBlocks >= 4);
                    ret = ret && (s->maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks >= 4);
                } break;
                default: break;
            }
        return ret;
    }
};

static const VpStructChainerDesc chainerDesc = {
    [](VkBaseOutStructure* p, void* pUser, PFN_vpStructChainerCb pfnCb) {
        VkPhysicalDeviceVulkan11Features physicalDeviceVulkan11Features{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES, nullptr };
        VkPhysicalDeviceVulkan12Features physicalDeviceVulkan12Features{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES, &physicalDeviceVulkan11Features };
        VkPhysicalDeviceVulkan13Features physicalDeviceVulkan13Features{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES, &physicalDeviceVulkan12Features };
        p->pNext = static_cast<VkBaseOutStructure*>(static_cast<void*>(&physicalDeviceVulkan13Features));
        pfnCb(p, pUser);
    },
    [](VkBaseOutStructure* p, void* pUser, PFN_vpStructChainerCb pfnCb) {
        VkPhysicalDeviceVulkan11Properties physicalDeviceVulkan11Properties{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES, nullptr };
        VkPhysicalDeviceVulkan12Properties physicalDeviceVulkan12Properties{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES, &physicalDeviceVulkan11Properties };
        VkPhysicalDeviceVulkan13Properties physicalDeviceVulkan13Properties{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES, &physicalDeviceVulkan12Properties };
        p->pNext = static_cast<VkBaseOutStructure*>(static_cast<void*>(&physicalDeviceVulkan13Properties));
        pfnCb(p, pUser);
    },
    [](VkBaseOutStructure* p, void* pUser, PFN_vpStructChainerCb pfnCb) {
        pfnCb(p, pUser);
    },
    [](VkBaseOutStructure* p, void* pUser, PFN_vpStructChainerCb pfnCb) {
        pfnCb(p, pUser);
    },
};

} // namespace VP_KHR_ROADMAP_2022
#endif

#ifdef VP_LUNARG_desktop_baseline_2022
namespace VP_LUNARG_DESKTOP_BASELINE_2022 {

static const VkExtensionProperties deviceExtensions[] = {
    VkExtensionProperties{ VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME, 2 },
    VkExtensionProperties{ VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_EXT_MEMORY_BUDGET_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_EXT_ROBUSTNESS_2_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_EXT_SCALAR_BLOCK_LAYOUT_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME, 2 },
    VkExtensionProperties{ VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME, 3 },
    VkExtensionProperties{ VK_KHR_16BIT_STORAGE_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_8BIT_STORAGE_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_BIND_MEMORY_2_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME, 3 },
    VkExtensionProperties{ VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_DESCRIPTOR_UPDATE_TEMPLATE_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_DEVICE_GROUP_EXTENSION_NAME, 4 },
    VkExtensionProperties{ VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_EXTERNAL_FENCE_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_MAINTENANCE_1_EXTENSION_NAME, 2 },
    VkExtensionProperties{ VK_KHR_MAINTENANCE_2_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_MAINTENANCE_3_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_MULTIVIEW_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_RELAXED_BLOCK_LAYOUT_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_SAMPLER_MIRROR_CLAMP_TO_EDGE_EXTENSION_NAME, 3 },
    VkExtensionProperties{ VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_STORAGE_BUFFER_STORAGE_CLASS_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_SWAPCHAIN_EXTENSION_NAME, 70 },
    VkExtensionProperties{ VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME, 2 },
    VkExtensionProperties{ VK_KHR_UNIFORM_BUFFER_STANDARD_LAYOUT_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_VARIABLE_POINTERS_EXTENSION_NAME, 1 },
};

static const VkStructureType featureStructTypes[] = {
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT16_INT8_FEATURES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_FEATURES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTERS_FEATURES,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_FEATURES_EXT,
};

static const VkStructureType propertyStructTypes[] = {
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_PROPERTIES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_PROPERTIES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_PROPERTIES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_PROPERTIES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_PROPERTIES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_PROPERTIES_EXT,
};

static const VkStructureType formatStructTypes[] = {
    VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR,
    VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_3_KHR,
};

static const VpFeatureDesc featureDesc = {
    [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES_KHR: {
                    VkPhysicalDevice16BitStorageFeaturesKHR* s = static_cast<VkPhysicalDevice16BitStorageFeaturesKHR*>(static_cast<void*>(p));
                    s->storageBuffer16BitAccess = VK_TRUE;
                    s->uniformAndStorageBuffer16BitAccess = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES_KHR: {
                    VkPhysicalDevice8BitStorageFeaturesKHR* s = static_cast<VkPhysicalDevice8BitStorageFeaturesKHR*>(static_cast<void*>(p));
                    s->storageBuffer8BitAccess = VK_TRUE;
                    s->uniformAndStorageBuffer8BitAccess = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT: {
                    VkPhysicalDeviceDescriptorIndexingFeaturesEXT* s = static_cast<VkPhysicalDeviceDescriptorIndexingFeaturesEXT*>(static_cast<void*>(p));
                    s->descriptorBindingPartiallyBound = VK_TRUE;
                    s->descriptorBindingSampledImageUpdateAfterBind = VK_TRUE;
                    s->descriptorBindingStorageBufferUpdateAfterBind = VK_TRUE;
                    s->descriptorBindingStorageImageUpdateAfterBind = VK_TRUE;
                    s->descriptorBindingStorageTexelBufferUpdateAfterBind = VK_TRUE;
                    s->descriptorBindingUniformTexelBufferUpdateAfterBind = VK_TRUE;
                    s->descriptorBindingUpdateUnusedWhilePending = VK_TRUE;
                    s->descriptorBindingVariableDescriptorCount = VK_TRUE;
                    s->runtimeDescriptorArray = VK_TRUE;
                    s->shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
                    s->shaderStorageTexelBufferArrayDynamicIndexing = VK_TRUE;
                    s->shaderUniformTexelBufferArrayDynamicIndexing = VK_TRUE;
                    s->shaderUniformTexelBufferArrayNonUniformIndexing = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR: {
                    VkPhysicalDeviceFeatures2KHR* s = static_cast<VkPhysicalDeviceFeatures2KHR*>(static_cast<void*>(p));
                    s->features.depthBiasClamp = VK_TRUE;
                    s->features.depthClamp = VK_TRUE;
                    s->features.drawIndirectFirstInstance = VK_TRUE;
                    s->features.dualSrcBlend = VK_TRUE;
                    s->features.fillModeNonSolid = VK_TRUE;
                    s->features.fragmentStoresAndAtomics = VK_TRUE;
                    s->features.fullDrawIndexUint32 = VK_TRUE;
                    s->features.imageCubeArray = VK_TRUE;
                    s->features.independentBlend = VK_TRUE;
                    s->features.inheritedQueries = VK_TRUE;
                    s->features.largePoints = VK_TRUE;
                    s->features.multiDrawIndirect = VK_TRUE;
                    s->features.multiViewport = VK_TRUE;
                    s->features.occlusionQueryPrecise = VK_TRUE;
                    s->features.robustBufferAccess = VK_TRUE;
                    s->features.sampleRateShading = VK_TRUE;
                    s->features.samplerAnisotropy = VK_TRUE;
                    s->features.shaderClipDistance = VK_TRUE;
                    s->features.shaderImageGatherExtended = VK_TRUE;
                    s->features.shaderSampledImageArrayDynamicIndexing = VK_TRUE;
                    s->features.shaderStorageBufferArrayDynamicIndexing = VK_TRUE;
                    s->features.shaderStorageImageArrayDynamicIndexing = VK_TRUE;
                    s->features.shaderStorageImageExtendedFormats = VK_TRUE;
                    s->features.shaderStorageImageWriteWithoutFormat = VK_TRUE;
                    s->features.shaderTessellationAndGeometryPointSize = VK_TRUE;
                    s->features.shaderUniformBufferArrayDynamicIndexing = VK_TRUE;
                    s->features.tessellationShader = VK_TRUE;
                    s->features.textureCompressionBC = VK_TRUE;
                    s->features.vertexPipelineStoresAndAtomics = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT16_INT8_FEATURES_KHR: {
                    VkPhysicalDeviceFloat16Int8FeaturesKHR* s = static_cast<VkPhysicalDeviceFloat16Int8FeaturesKHR*>(static_cast<void*>(p));
                    s->shaderInt8 = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES_EXT: {
                    VkPhysicalDeviceHostQueryResetFeaturesEXT* s = static_cast<VkPhysicalDeviceHostQueryResetFeaturesEXT*>(static_cast<void*>(p));
                    s->hostQueryReset = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES_EXT: {
                    VkPhysicalDeviceInlineUniformBlockFeaturesEXT* s = static_cast<VkPhysicalDeviceInlineUniformBlockFeaturesEXT*>(static_cast<void*>(p));
                    s->descriptorBindingInlineUniformBlockUpdateAfterBind = VK_TRUE;
                    s->inlineUniformBlock = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES_KHR: {
                    VkPhysicalDeviceMultiviewFeaturesKHR* s = static_cast<VkPhysicalDeviceMultiviewFeaturesKHR*>(static_cast<void*>(p));
                    s->multiview = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT: {
                    VkPhysicalDeviceRobustness2FeaturesEXT* s = static_cast<VkPhysicalDeviceRobustness2FeaturesEXT*>(static_cast<void*>(p));
                    s->robustImageAccess2 = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES_EXT: {
                    VkPhysicalDeviceScalarBlockLayoutFeaturesEXT* s = static_cast<VkPhysicalDeviceScalarBlockLayoutFeaturesEXT*>(static_cast<void*>(p));
                    s->scalarBlockLayout = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_FEATURES_EXT: {
                    VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT* s = static_cast<VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT*>(static_cast<void*>(p));
                    s->texelBufferAlignment = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES_KHR: {
                    VkPhysicalDeviceTimelineSemaphoreFeaturesKHR* s = static_cast<VkPhysicalDeviceTimelineSemaphoreFeaturesKHR*>(static_cast<void*>(p));
                    s->timelineSemaphore = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES_KHR: {
                    VkPhysicalDeviceUniformBufferStandardLayoutFeaturesKHR* s = static_cast<VkPhysicalDeviceUniformBufferStandardLayoutFeaturesKHR*>(static_cast<void*>(p));
                    s->uniformBufferStandardLayout = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTERS_FEATURES: {
                    VkPhysicalDeviceVariablePointersFeaturesKHR* s = static_cast<VkPhysicalDeviceVariablePointersFeaturesKHR*>(static_cast<void*>(p));
                    s->variablePointers = VK_TRUE;
                    s->variablePointersStorageBuffer = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_FEATURES_EXT: {
                    VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT* s = static_cast<VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT*>(static_cast<void*>(p));
                    s->vertexAttributeInstanceRateDivisor = VK_TRUE;
                    s->vertexAttributeInstanceRateZeroDivisor = VK_TRUE;
                } break;
                default: break;
            }
    },
    [](VkBaseOutStructure* p) -> bool {
        bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES_KHR: {
                    VkPhysicalDevice16BitStorageFeaturesKHR* s = static_cast<VkPhysicalDevice16BitStorageFeaturesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->storageBuffer16BitAccess == VK_TRUE);
                    ret = ret && (s->uniformAndStorageBuffer16BitAccess == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES_KHR: {
                    VkPhysicalDevice8BitStorageFeaturesKHR* s = static_cast<VkPhysicalDevice8BitStorageFeaturesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->storageBuffer8BitAccess == VK_TRUE);
                    ret = ret && (s->uniformAndStorageBuffer8BitAccess == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT: {
                    VkPhysicalDeviceDescriptorIndexingFeaturesEXT* s = static_cast<VkPhysicalDeviceDescriptorIndexingFeaturesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->descriptorBindingPartiallyBound == VK_TRUE);
                    ret = ret && (s->descriptorBindingSampledImageUpdateAfterBind == VK_TRUE);
                    ret = ret && (s->descriptorBindingStorageBufferUpdateAfterBind == VK_TRUE);
                    ret = ret && (s->descriptorBindingStorageImageUpdateAfterBind == VK_TRUE);
                    ret = ret && (s->descriptorBindingStorageTexelBufferUpdateAfterBind == VK_TRUE);
                    ret = ret && (s->descriptorBindingUniformTexelBufferUpdateAfterBind == VK_TRUE);
                    ret = ret && (s->descriptorBindingUpdateUnusedWhilePending == VK_TRUE);
                    ret = ret && (s->descriptorBindingVariableDescriptorCount == VK_TRUE);
                    ret = ret && (s->runtimeDescriptorArray == VK_TRUE);
                    ret = ret && (s->shaderSampledImageArrayNonUniformIndexing == VK_TRUE);
                    ret = ret && (s->shaderStorageTexelBufferArrayDynamicIndexing == VK_TRUE);
                    ret = ret && (s->shaderUniformTexelBufferArrayDynamicIndexing == VK_TRUE);
                    ret = ret && (s->shaderUniformTexelBufferArrayNonUniformIndexing == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR: {
                    VkPhysicalDeviceFeatures2KHR* s = static_cast<VkPhysicalDeviceFeatures2KHR*>(static_cast<void*>(p));
                    ret = ret && (s->features.depthBiasClamp == VK_TRUE);
                    ret = ret && (s->features.depthClamp == VK_TRUE);
                    ret = ret && (s->features.drawIndirectFirstInstance == VK_TRUE);
                    ret = ret && (s->features.dualSrcBlend == VK_TRUE);
                    ret = ret && (s->features.fillModeNonSolid == VK_TRUE);
                    ret = ret && (s->features.fragmentStoresAndAtomics == VK_TRUE);
                    ret = ret && (s->features.fullDrawIndexUint32 == VK_TRUE);
                    ret = ret && (s->features.imageCubeArray == VK_TRUE);
                    ret = ret && (s->features.independentBlend == VK_TRUE);
                    ret = ret && (s->features.inheritedQueries == VK_TRUE);
                    ret = ret && (s->features.largePoints == VK_TRUE);
                    ret = ret && (s->features.multiDrawIndirect == VK_TRUE);
                    ret = ret && (s->features.multiViewport == VK_TRUE);
                    ret = ret && (s->features.occlusionQueryPrecise == VK_TRUE);
                    ret = ret && (s->features.robustBufferAccess == VK_TRUE);
                    ret = ret && (s->features.sampleRateShading == VK_TRUE);
                    ret = ret && (s->features.samplerAnisotropy == VK_TRUE);
                    ret = ret && (s->features.shaderClipDistance == VK_TRUE);
                    ret = ret && (s->features.shaderImageGatherExtended == VK_TRUE);
                    ret = ret && (s->features.shaderSampledImageArrayDynamicIndexing == VK_TRUE);
                    ret = ret && (s->features.shaderStorageBufferArrayDynamicIndexing == VK_TRUE);
                    ret = ret && (s->features.shaderStorageImageArrayDynamicIndexing == VK_TRUE);
                    ret = ret && (s->features.shaderStorageImageExtendedFormats == VK_TRUE);
                    ret = ret && (s->features.shaderStorageImageWriteWithoutFormat == VK_TRUE);
                    ret = ret && (s->features.shaderTessellationAndGeometryPointSize == VK_TRUE);
                    ret = ret && (s->features.shaderUniformBufferArrayDynamicIndexing == VK_TRUE);
                    ret = ret && (s->features.tessellationShader == VK_TRUE);
                    ret = ret && (s->features.textureCompressionBC == VK_TRUE);
                    ret = ret && (s->features.vertexPipelineStoresAndAtomics == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT16_INT8_FEATURES_KHR: {
                    VkPhysicalDeviceFloat16Int8FeaturesKHR* s = static_cast<VkPhysicalDeviceFloat16Int8FeaturesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->shaderInt8 == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES_EXT: {
                    VkPhysicalDeviceHostQueryResetFeaturesEXT* s = static_cast<VkPhysicalDeviceHostQueryResetFeaturesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->hostQueryReset == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES_EXT: {
                    VkPhysicalDeviceInlineUniformBlockFeaturesEXT* s = static_cast<VkPhysicalDeviceInlineUniformBlockFeaturesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->descriptorBindingInlineUniformBlockUpdateAfterBind == VK_TRUE);
                    ret = ret && (s->inlineUniformBlock == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES_KHR: {
                    VkPhysicalDeviceMultiviewFeaturesKHR* s = static_cast<VkPhysicalDeviceMultiviewFeaturesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->multiview == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT: {
                    VkPhysicalDeviceRobustness2FeaturesEXT* s = static_cast<VkPhysicalDeviceRobustness2FeaturesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->robustImageAccess2 == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES_EXT: {
                    VkPhysicalDeviceScalarBlockLayoutFeaturesEXT* s = static_cast<VkPhysicalDeviceScalarBlockLayoutFeaturesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->scalarBlockLayout == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_FEATURES_EXT: {
                    VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT* s = static_cast<VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->texelBufferAlignment == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES_KHR: {
                    VkPhysicalDeviceTimelineSemaphoreFeaturesKHR* s = static_cast<VkPhysicalDeviceTimelineSemaphoreFeaturesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->timelineSemaphore == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES_KHR: {
                    VkPhysicalDeviceUniformBufferStandardLayoutFeaturesKHR* s = static_cast<VkPhysicalDeviceUniformBufferStandardLayoutFeaturesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->uniformBufferStandardLayout == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTERS_FEATURES: {
                    VkPhysicalDeviceVariablePointersFeaturesKHR* s = static_cast<VkPhysicalDeviceVariablePointersFeaturesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->variablePointers == VK_TRUE);
                    ret = ret && (s->variablePointersStorageBuffer == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_FEATURES_EXT: {
                    VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT* s = static_cast<VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->vertexAttributeInstanceRateDivisor == VK_TRUE);
                    ret = ret && (s->vertexAttributeInstanceRateZeroDivisor == VK_TRUE);
                } break;
                default: break;
            }
        return ret;
    }
};

static const VpPropertyDesc propertyDesc = {
    [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES_KHR: {
                    VkPhysicalDeviceDepthStencilResolvePropertiesKHR* s = static_cast<VkPhysicalDeviceDepthStencilResolvePropertiesKHR*>(static_cast<void*>(p));
                    s->independentResolve = VK_TRUE;
                    s->independentResolveNone = VK_TRUE;
                    s->supportedDepthResolveModes = (VK_RESOLVE_MODE_SAMPLE_ZERO_BIT);
                    s->supportedStencilResolveModes = (VK_RESOLVE_MODE_SAMPLE_ZERO_BIT);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES_EXT: {
                    VkPhysicalDeviceDescriptorIndexingPropertiesEXT* s = static_cast<VkPhysicalDeviceDescriptorIndexingPropertiesEXT*>(static_cast<void*>(p));
                    s->maxDescriptorSetUpdateAfterBindInputAttachments = 256;
                    s->maxDescriptorSetUpdateAfterBindSampledImages = 640;
                    s->maxDescriptorSetUpdateAfterBindSamplers = 80;
                    s->maxDescriptorSetUpdateAfterBindStorageBuffers = 155;
                    s->maxDescriptorSetUpdateAfterBindStorageBuffersDynamic = 8;
                    s->maxDescriptorSetUpdateAfterBindStorageImages = 40;
                    s->maxDescriptorSetUpdateAfterBindUniformBuffers = 90;
                    s->maxDescriptorSetUpdateAfterBindUniformBuffersDynamic = 8;
                    s->maxPerStageDescriptorUpdateAfterBindInputAttachments = 64;
                    s->maxPerStageDescriptorUpdateAfterBindSampledImages = 128;
                    s->maxPerStageDescriptorUpdateAfterBindSamplers = 16;
                    s->maxPerStageDescriptorUpdateAfterBindStorageBuffers = 31;
                    s->maxPerStageDescriptorUpdateAfterBindStorageImages = 8;
                    s->maxPerStageDescriptorUpdateAfterBindUniformBuffers = 15;
                    s->maxPerStageUpdateAfterBindResources = 159;
                    s->maxUpdateAfterBindDescriptorsInAllPools = 1048576;
                    s->quadDivergentImplicitLod = VK_TRUE;
                    s->robustBufferAccessUpdateAfterBind = VK_TRUE;
                    s->shaderInputAttachmentArrayNonUniformIndexingNative = VK_TRUE;
                    s->shaderSampledImageArrayNonUniformIndexingNative = VK_TRUE;
                    s->shaderStorageBufferArrayNonUniformIndexingNative = VK_TRUE;
                    s->shaderStorageImageArrayNonUniformIndexingNative = VK_TRUE;
                    s->shaderUniformBufferArrayNonUniformIndexingNative = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_PROPERTIES_EXT: {
                    VkPhysicalDeviceInlineUniformBlockPropertiesEXT* s = static_cast<VkPhysicalDeviceInlineUniformBlockPropertiesEXT*>(static_cast<void*>(p));
                    s->maxDescriptorSetInlineUniformBlocks = 4;
                    s->maxDescriptorSetUpdateAfterBindInlineUniformBlocks = 4;
                    s->maxInlineUniformBlockSize = 256;
                    s->maxPerStageDescriptorInlineUniformBlocks = 4;
                    s->maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks = 4;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES_KHR: {
                    VkPhysicalDeviceMaintenance3PropertiesKHR* s = static_cast<VkPhysicalDeviceMaintenance3PropertiesKHR*>(static_cast<void*>(p));
                    s->maxMemoryAllocationSize = 2147483648;
                    s->maxPerSetDescriptors = 700;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES_KHR: {
                    VkPhysicalDeviceMultiviewPropertiesKHR* s = static_cast<VkPhysicalDeviceMultiviewPropertiesKHR*>(static_cast<void*>(p));
                    s->maxMultiviewInstanceIndex = 134217727;
                    s->maxMultiviewViewCount = 6;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR: {
                    VkPhysicalDeviceProperties2KHR* s = static_cast<VkPhysicalDeviceProperties2KHR*>(static_cast<void*>(p));
                    s->properties.limits.bufferImageGranularity = 65536;
                    s->properties.limits.discreteQueuePriorities = 2;
                    s->properties.limits.framebufferColorSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.framebufferDepthSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.framebufferNoAttachmentsSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.framebufferStencilSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.lineWidthGranularity = 1;
                    s->properties.limits.lineWidthRange[0] = 1;
                    s->properties.limits.lineWidthRange[1] = 1;
                    s->properties.limits.maxBoundDescriptorSets = 8;
                    s->properties.limits.maxClipDistances = 8;
                    s->properties.limits.maxColorAttachments = 8;
                    s->properties.limits.maxCombinedClipAndCullDistances = 8;
                    s->properties.limits.maxComputeSharedMemorySize = 32768;
                    s->properties.limits.maxComputeWorkGroupCount[0] = 65535;
                    s->properties.limits.maxComputeWorkGroupCount[1] = 65535;
                    s->properties.limits.maxComputeWorkGroupCount[2] = 65535;
                    s->properties.limits.maxComputeWorkGroupInvocations = 1024;
                    s->properties.limits.maxCullDistances = 0;
                    s->properties.limits.maxDescriptorSetInputAttachments = 8;
                    s->properties.limits.maxDescriptorSetSampledImages = 640;
                    s->properties.limits.maxDescriptorSetSamplers = 80;
                    s->properties.limits.maxDescriptorSetStorageBuffers = 155;
                    s->properties.limits.maxDescriptorSetStorageBuffersDynamic = 8;
                    s->properties.limits.maxDescriptorSetStorageImages = 40;
                    s->properties.limits.maxDescriptorSetUniformBuffers = 90;
                    s->properties.limits.maxDescriptorSetUniformBuffersDynamic = 8;
                    s->properties.limits.maxDrawIndexedIndexValue = 4294967295;
                    s->properties.limits.maxDrawIndirectCount = 1073741824;
                    s->properties.limits.maxFragmentCombinedOutputResources = 8;
                    s->properties.limits.maxFragmentDualSrcAttachments = 1;
                    s->properties.limits.maxFragmentInputComponents = 116;
                    s->properties.limits.maxFragmentOutputAttachments = 8;
                    s->properties.limits.maxFramebufferHeight = 16384;
                    s->properties.limits.maxFramebufferLayers = 1024;
                    s->properties.limits.maxFramebufferWidth = 16384;
                    s->properties.limits.maxGeometryInputComponents = 0;
                    s->properties.limits.maxGeometryOutputComponents = 0;
                    s->properties.limits.maxGeometryOutputVertices = 0;
                    s->properties.limits.maxGeometryShaderInvocations = 0;
                    s->properties.limits.maxGeometryTotalOutputComponents = 0;
                    s->properties.limits.maxImageArrayLayers = 2048;
                    s->properties.limits.maxImageDimension1D = 16384;
                    s->properties.limits.maxImageDimension2D = 16384;
                    s->properties.limits.maxImageDimension3D = 2048;
                    s->properties.limits.maxImageDimensionCube = 16384;
                    s->properties.limits.maxInterpolationOffset = 0.4375f;
                    s->properties.limits.maxMemoryAllocationCount = 4096;
                    s->properties.limits.maxPerStageDescriptorInputAttachments = 8;
                    s->properties.limits.maxPerStageDescriptorSampledImages = 128;
                    s->properties.limits.maxPerStageDescriptorSamplers = 16;
                    s->properties.limits.maxPerStageDescriptorStorageBuffers = 31;
                    s->properties.limits.maxPerStageDescriptorStorageImages = 8;
                    s->properties.limits.maxPerStageDescriptorUniformBuffers = 15;
                    s->properties.limits.maxPerStageResources = 159;
                    s->properties.limits.maxPushConstantsSize = 128;
                    s->properties.limits.maxSampleMaskWords = 1;
                    s->properties.limits.maxSamplerAllocationCount = 1024;
                    s->properties.limits.maxSamplerAnisotropy = 16;
                    s->properties.limits.maxSamplerLodBias = 0;
                    s->properties.limits.maxStorageBufferRange = 1073741824;
                    s->properties.limits.maxTessellationControlPerPatchOutputComponents = 120;
                    s->properties.limits.maxTessellationControlPerVertexInputComponents = 124;
                    s->properties.limits.maxTessellationControlPerVertexOutputComponents = 124;
                    s->properties.limits.maxTessellationControlTotalOutputComponents = 2048;
                    s->properties.limits.maxTessellationEvaluationInputComponents = 124;
                    s->properties.limits.maxTessellationEvaluationOutputComponents = 124;
                    s->properties.limits.maxTessellationGenerationLevel = 64;
                    s->properties.limits.maxTessellationPatchSize = 32;
                    s->properties.limits.maxTexelBufferElements = 67108864;
                    s->properties.limits.maxTexelGatherOffset = 7;
                    s->properties.limits.maxTexelOffset = 7;
                    s->properties.limits.maxUniformBufferRange = 65536;
                    s->properties.limits.maxVertexInputAttributeOffset = 2047;
                    s->properties.limits.maxVertexInputAttributes = 28;
                    s->properties.limits.maxVertexInputBindingStride = 2048;
                    s->properties.limits.maxVertexInputBindings = 28;
                    s->properties.limits.maxVertexOutputComponents = 124;
                    s->properties.limits.maxViewportDimensions[0] = 16384;
                    s->properties.limits.maxViewportDimensions[1] = 16384;
                    s->properties.limits.maxViewports = 16;
                    s->properties.limits.minInterpolationOffset = -0.5f;
                    s->properties.limits.minMemoryMapAlignment = 4096;
                    s->properties.limits.minStorageBufferOffsetAlignment = 64;
                    s->properties.limits.minTexelBufferOffsetAlignment = 256;
                    s->properties.limits.minTexelGatherOffset = -8;
                    s->properties.limits.minTexelOffset = -8;
                    s->properties.limits.minUniformBufferOffsetAlignment = 256;
                    s->properties.limits.mipmapPrecisionBits = 4;
                    s->properties.limits.nonCoherentAtomSize = 256;
                    s->properties.limits.optimalBufferCopyOffsetAlignment = 256;
                    s->properties.limits.optimalBufferCopyRowPitchAlignment = 128;
                    s->properties.limits.pointSizeGranularity = 1;
                    s->properties.limits.pointSizeRange[0] = 1;
                    s->properties.limits.pointSizeRange[1] = 64;
                    s->properties.limits.sampledImageColorSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.sampledImageDepthSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.sampledImageIntegerSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.sampledImageStencilSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.sparseAddressSpaceSize = 0;
                    s->properties.limits.storageImageSampleCounts = (VK_SAMPLE_COUNT_1_BIT);
                    s->properties.limits.subPixelInterpolationOffsetBits = 4;
                    s->properties.limits.subPixelPrecisionBits = 4;
                    s->properties.limits.subTexelPrecisionBits = 4;
                    s->properties.limits.timestampComputeAndGraphics = VK_TRUE;
                    s->properties.limits.viewportBoundsRange[0] = -32768;
                    s->properties.limits.viewportBoundsRange[1] = 32767;
                    s->properties.limits.viewportSubPixelBits = 0;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_PROPERTIES_EXT: {
                    VkPhysicalDeviceRobustness2PropertiesEXT* s = static_cast<VkPhysicalDeviceRobustness2PropertiesEXT*>(static_cast<void*>(p));
                    s->robustStorageBufferAccessSizeAlignment = 4;
                    s->robustUniformBufferAccessSizeAlignment = 256;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_PROPERTIES_EXT: {
                    VkPhysicalDeviceSubgroupSizeControlPropertiesEXT* s = static_cast<VkPhysicalDeviceSubgroupSizeControlPropertiesEXT*>(static_cast<void*>(p));
                    s->maxComputeWorkgroupSubgroups = 16;
                    s->maxSubgroupSize = 1;
                    s->minSubgroupSize = 64;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_PROPERTIES_EXT: {
                    VkPhysicalDeviceTexelBufferAlignmentPropertiesEXT* s = static_cast<VkPhysicalDeviceTexelBufferAlignmentPropertiesEXT*>(static_cast<void*>(p));
                    s->storageTexelBufferOffsetAlignmentBytes = 256;
                    s->uniformTexelBufferOffsetAlignmentBytes = 256;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_PROPERTIES_KHR: {
                    VkPhysicalDeviceTimelineSemaphorePropertiesKHR* s = static_cast<VkPhysicalDeviceTimelineSemaphorePropertiesKHR*>(static_cast<void*>(p));
                    s->maxTimelineSemaphoreValueDifference = 2147483647;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_PROPERTIES_EXT: {
                    VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT* s = static_cast<VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT*>(static_cast<void*>(p));
                    s->maxVertexAttribDivisor = 268435455;
                } break;
                default: break;
            }
    },
    [](VkBaseOutStructure* p) -> bool {
        bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES_KHR: {
                    VkPhysicalDeviceDepthStencilResolvePropertiesKHR* s = static_cast<VkPhysicalDeviceDepthStencilResolvePropertiesKHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->independentResolve, VK_TRUE));
                    ret = ret && (vpCheckFlags(s->independentResolveNone, VK_TRUE));
                    ret = ret && (vpCheckFlags(s->supportedDepthResolveModes, (VK_RESOLVE_MODE_SAMPLE_ZERO_BIT)));
                    ret = ret && (vpCheckFlags(s->supportedStencilResolveModes, (VK_RESOLVE_MODE_SAMPLE_ZERO_BIT)));
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES_EXT: {
                    VkPhysicalDeviceDescriptorIndexingPropertiesEXT* s = static_cast<VkPhysicalDeviceDescriptorIndexingPropertiesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindInputAttachments >= 256);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindSampledImages >= 640);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindSamplers >= 80);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindStorageBuffers >= 155);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindStorageBuffersDynamic >= 8);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindStorageImages >= 40);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindUniformBuffers >= 90);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindUniformBuffersDynamic >= 8);
                    ret = ret && (s->maxPerStageDescriptorUpdateAfterBindInputAttachments >= 64);
                    ret = ret && (s->maxPerStageDescriptorUpdateAfterBindSampledImages >= 128);
                    ret = ret && (s->maxPerStageDescriptorUpdateAfterBindSamplers >= 16);
                    ret = ret && (s->maxPerStageDescriptorUpdateAfterBindStorageBuffers >= 31);
                    ret = ret && (s->maxPerStageDescriptorUpdateAfterBindStorageImages >= 8);
                    ret = ret && (s->maxPerStageDescriptorUpdateAfterBindUniformBuffers >= 15);
                    ret = ret && (s->maxPerStageUpdateAfterBindResources >= 159);
                    ret = ret && (s->maxUpdateAfterBindDescriptorsInAllPools >= 1048576);
                    ret = ret && (vpCheckFlags(s->quadDivergentImplicitLod, VK_TRUE));
                    ret = ret && (vpCheckFlags(s->robustBufferAccessUpdateAfterBind, VK_TRUE));
                    ret = ret && (vpCheckFlags(s->shaderInputAttachmentArrayNonUniformIndexingNative, VK_TRUE));
                    ret = ret && (vpCheckFlags(s->shaderSampledImageArrayNonUniformIndexingNative, VK_TRUE));
                    ret = ret && (vpCheckFlags(s->shaderStorageBufferArrayNonUniformIndexingNative, VK_TRUE));
                    ret = ret && (vpCheckFlags(s->shaderStorageImageArrayNonUniformIndexingNative, VK_TRUE));
                    ret = ret && (vpCheckFlags(s->shaderUniformBufferArrayNonUniformIndexingNative, VK_TRUE));
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_PROPERTIES_EXT: {
                    VkPhysicalDeviceInlineUniformBlockPropertiesEXT* s = static_cast<VkPhysicalDeviceInlineUniformBlockPropertiesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->maxDescriptorSetInlineUniformBlocks >= 4);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindInlineUniformBlocks >= 4);
                    ret = ret && (s->maxInlineUniformBlockSize >= 256);
                    ret = ret && (s->maxPerStageDescriptorInlineUniformBlocks >= 4);
                    ret = ret && (s->maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks >= 4);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES_KHR: {
                    VkPhysicalDeviceMaintenance3PropertiesKHR* s = static_cast<VkPhysicalDeviceMaintenance3PropertiesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->maxMemoryAllocationSize >= 2147483648);
                    ret = ret && (s->maxPerSetDescriptors >= 700);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES_KHR: {
                    VkPhysicalDeviceMultiviewPropertiesKHR* s = static_cast<VkPhysicalDeviceMultiviewPropertiesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->maxMultiviewInstanceIndex >= 134217727);
                    ret = ret && (s->maxMultiviewViewCount >= 6);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR: {
                    VkPhysicalDeviceProperties2KHR* s = static_cast<VkPhysicalDeviceProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (s->properties.limits.bufferImageGranularity <= 65536);
                    ret = ret && ((65536 % s->properties.limits.bufferImageGranularity) == 0);
                    ret = ret && (s->properties.limits.discreteQueuePriorities >= 2);
                    ret = ret && (vpCheckFlags(s->properties.limits.framebufferColorSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (vpCheckFlags(s->properties.limits.framebufferDepthSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (vpCheckFlags(s->properties.limits.framebufferNoAttachmentsSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (vpCheckFlags(s->properties.limits.framebufferStencilSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (s->properties.limits.lineWidthGranularity <= 1);
                    ret = ret && (isMultiple(1, s->properties.limits.lineWidthGranularity));
                    ret = ret && (s->properties.limits.lineWidthRange[0] <= 1);
                    ret = ret && (s->properties.limits.lineWidthRange[1] >= 1);
                    ret = ret && (s->properties.limits.maxBoundDescriptorSets >= 8);
                    ret = ret && (s->properties.limits.maxClipDistances >= 8);
                    ret = ret && (s->properties.limits.maxColorAttachments >= 8);
                    ret = ret && (s->properties.limits.maxCombinedClipAndCullDistances >= 8);
                    ret = ret && (s->properties.limits.maxComputeSharedMemorySize >= 32768);
                    ret = ret && (s->properties.limits.maxComputeWorkGroupCount[0] >= 65535);
                    ret = ret && (s->properties.limits.maxComputeWorkGroupCount[1] >= 65535);
                    ret = ret && (s->properties.limits.maxComputeWorkGroupCount[2] >= 65535);
                    ret = ret && (s->properties.limits.maxComputeWorkGroupInvocations >= 1024);
                    ret = ret && (s->properties.limits.maxCullDistances >= 0);
                    ret = ret && (s->properties.limits.maxDescriptorSetInputAttachments >= 8);
                    ret = ret && (s->properties.limits.maxDescriptorSetSampledImages >= 640);
                    ret = ret && (s->properties.limits.maxDescriptorSetSamplers >= 80);
                    ret = ret && (s->properties.limits.maxDescriptorSetStorageBuffers >= 155);
                    ret = ret && (s->properties.limits.maxDescriptorSetStorageBuffersDynamic >= 8);
                    ret = ret && (s->properties.limits.maxDescriptorSetStorageImages >= 40);
                    ret = ret && (s->properties.limits.maxDescriptorSetUniformBuffers >= 90);
                    ret = ret && (s->properties.limits.maxDescriptorSetUniformBuffersDynamic >= 8);
                    ret = ret && (s->properties.limits.maxDrawIndexedIndexValue >= 4294967295);
                    ret = ret && (s->properties.limits.maxDrawIndirectCount >= 1073741824);
                    ret = ret && (s->properties.limits.maxFragmentCombinedOutputResources >= 8);
                    ret = ret && (s->properties.limits.maxFragmentDualSrcAttachments >= 1);
                    ret = ret && (s->properties.limits.maxFragmentInputComponents >= 116);
                    ret = ret && (s->properties.limits.maxFragmentOutputAttachments >= 8);
                    ret = ret && (s->properties.limits.maxFramebufferHeight >= 16384);
                    ret = ret && (s->properties.limits.maxFramebufferLayers >= 1024);
                    ret = ret && (s->properties.limits.maxFramebufferWidth >= 16384);
                    ret = ret && (s->properties.limits.maxGeometryInputComponents >= 0);
                    ret = ret && (s->properties.limits.maxGeometryOutputComponents >= 0);
                    ret = ret && (s->properties.limits.maxGeometryOutputVertices >= 0);
                    ret = ret && (s->properties.limits.maxGeometryShaderInvocations >= 0);
                    ret = ret && (s->properties.limits.maxGeometryTotalOutputComponents >= 0);
                    ret = ret && (s->properties.limits.maxImageArrayLayers >= 2048);
                    ret = ret && (s->properties.limits.maxImageDimension1D >= 16384);
                    ret = ret && (s->properties.limits.maxImageDimension2D >= 16384);
                    ret = ret && (s->properties.limits.maxImageDimension3D >= 2048);
                    ret = ret && (s->properties.limits.maxImageDimensionCube >= 16384);
                    ret = ret && (s->properties.limits.maxInterpolationOffset >= 0.4375);
                    ret = ret && (s->properties.limits.maxMemoryAllocationCount >= 4096);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorInputAttachments >= 8);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorSampledImages >= 128);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorSamplers >= 16);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorStorageBuffers >= 31);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorStorageImages >= 8);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorUniformBuffers >= 15);
                    ret = ret && (s->properties.limits.maxPerStageResources >= 159);
                    ret = ret && (s->properties.limits.maxPushConstantsSize >= 128);
                    ret = ret && (s->properties.limits.maxSampleMaskWords >= 1);
                    ret = ret && (s->properties.limits.maxSamplerAllocationCount >= 1024);
                    ret = ret && (s->properties.limits.maxSamplerAnisotropy >= 16);
                    ret = ret && (s->properties.limits.maxSamplerLodBias >= 0);
                    ret = ret && (s->properties.limits.maxStorageBufferRange >= 1073741824);
                    ret = ret && (s->properties.limits.maxTessellationControlPerPatchOutputComponents >= 120);
                    ret = ret && (s->properties.limits.maxTessellationControlPerVertexInputComponents >= 124);
                    ret = ret && (s->properties.limits.maxTessellationControlPerVertexOutputComponents >= 124);
                    ret = ret && (s->properties.limits.maxTessellationControlTotalOutputComponents >= 2048);
                    ret = ret && (s->properties.limits.maxTessellationEvaluationInputComponents >= 124);
                    ret = ret && (s->properties.limits.maxTessellationEvaluationOutputComponents >= 124);
                    ret = ret && (s->properties.limits.maxTessellationGenerationLevel >= 64);
                    ret = ret && (s->properties.limits.maxTessellationPatchSize >= 32);
                    ret = ret && (s->properties.limits.maxTexelBufferElements >= 67108864);
                    ret = ret && (s->properties.limits.maxTexelGatherOffset >= 7);
                    ret = ret && (s->properties.limits.maxTexelOffset >= 7);
                    ret = ret && (s->properties.limits.maxUniformBufferRange >= 65536);
                    ret = ret && (s->properties.limits.maxVertexInputAttributeOffset >= 2047);
                    ret = ret && (s->properties.limits.maxVertexInputAttributes >= 28);
                    ret = ret && (s->properties.limits.maxVertexInputBindingStride >= 2048);
                    ret = ret && (s->properties.limits.maxVertexInputBindings >= 28);
                    ret = ret && (s->properties.limits.maxVertexOutputComponents >= 124);
                    ret = ret && (s->properties.limits.maxViewportDimensions[0] >= 16384);
                    ret = ret && (s->properties.limits.maxViewportDimensions[1] >= 16384);
                    ret = ret && (s->properties.limits.maxViewports >= 16);
                    ret = ret && (s->properties.limits.minInterpolationOffset <= -0.5);
                    ret = ret && (s->properties.limits.minMemoryMapAlignment <= 4096);
                    ret = ret && ((s->properties.limits.minMemoryMapAlignment & (s->properties.limits.minMemoryMapAlignment - 1)) == 0);
                    ret = ret && (s->properties.limits.minStorageBufferOffsetAlignment <= 64);
                    ret = ret && ((s->properties.limits.minStorageBufferOffsetAlignment & (s->properties.limits.minStorageBufferOffsetAlignment - 1)) == 0);
                    ret = ret && (s->properties.limits.minTexelBufferOffsetAlignment <= 256);
                    ret = ret && ((s->properties.limits.minTexelBufferOffsetAlignment & (s->properties.limits.minTexelBufferOffsetAlignment - 1)) == 0);
                    ret = ret && (s->properties.limits.minTexelGatherOffset <= -8);
                    ret = ret && (s->properties.limits.minTexelOffset <= -8);
                    ret = ret && (s->properties.limits.minUniformBufferOffsetAlignment <= 256);
                    ret = ret && ((s->properties.limits.minUniformBufferOffsetAlignment & (s->properties.limits.minUniformBufferOffsetAlignment - 1)) == 0);
                    ret = ret && (s->properties.limits.mipmapPrecisionBits >= 4);
                    ret = ret && (s->properties.limits.nonCoherentAtomSize <= 256);
                    ret = ret && ((s->properties.limits.nonCoherentAtomSize & (s->properties.limits.nonCoherentAtomSize - 1)) == 0);
                    ret = ret && (s->properties.limits.optimalBufferCopyOffsetAlignment <= 256);
                    ret = ret && ((s->properties.limits.optimalBufferCopyOffsetAlignment & (s->properties.limits.optimalBufferCopyOffsetAlignment - 1)) == 0);
                    ret = ret && (s->properties.limits.optimalBufferCopyRowPitchAlignment <= 128);
                    ret = ret && ((s->properties.limits.optimalBufferCopyRowPitchAlignment & (s->properties.limits.optimalBufferCopyRowPitchAlignment - 1)) == 0);
                    ret = ret && (s->properties.limits.pointSizeGranularity <= 1);
                    ret = ret && (isMultiple(1, s->properties.limits.pointSizeGranularity));
                    ret = ret && (s->properties.limits.pointSizeRange[0] <= 1);
                    ret = ret && (s->properties.limits.pointSizeRange[1] >= 64);
                    ret = ret && (vpCheckFlags(s->properties.limits.sampledImageColorSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (vpCheckFlags(s->properties.limits.sampledImageDepthSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (vpCheckFlags(s->properties.limits.sampledImageIntegerSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (vpCheckFlags(s->properties.limits.sampledImageStencilSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (s->properties.limits.sparseAddressSpaceSize >= 0);
                    ret = ret && (vpCheckFlags(s->properties.limits.storageImageSampleCounts, (VK_SAMPLE_COUNT_1_BIT)));
                    ret = ret && (s->properties.limits.subPixelInterpolationOffsetBits >= 4);
                    ret = ret && (s->properties.limits.subPixelPrecisionBits >= 4);
                    ret = ret && (s->properties.limits.subTexelPrecisionBits >= 4);
                    ret = ret && (vpCheckFlags(s->properties.limits.timestampComputeAndGraphics, VK_TRUE));
                    ret = ret && (s->properties.limits.viewportBoundsRange[0] <= -32768);
                    ret = ret && (s->properties.limits.viewportBoundsRange[1] >= 32767);
                    ret = ret && (s->properties.limits.viewportSubPixelBits >= 0);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_PROPERTIES_EXT: {
                    VkPhysicalDeviceRobustness2PropertiesEXT* s = static_cast<VkPhysicalDeviceRobustness2PropertiesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->robustStorageBufferAccessSizeAlignment <= 4);
                    ret = ret && ((s->robustStorageBufferAccessSizeAlignment & (s->robustStorageBufferAccessSizeAlignment - 1)) == 0);
                    ret = ret && (s->robustUniformBufferAccessSizeAlignment <= 256);
                    ret = ret && ((s->robustUniformBufferAccessSizeAlignment & (s->robustUniformBufferAccessSizeAlignment - 1)) == 0);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_PROPERTIES_EXT: {
                    VkPhysicalDeviceSubgroupSizeControlPropertiesEXT* s = static_cast<VkPhysicalDeviceSubgroupSizeControlPropertiesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->maxComputeWorkgroupSubgroups >= 16);
                    ret = ret && (s->maxSubgroupSize >= 1);
                    ret = ret && ((s->maxSubgroupSize & (s->maxSubgroupSize - 1)) == 0);
                    ret = ret && (s->minSubgroupSize <= 64);
                    ret = ret && ((s->minSubgroupSize & (s->minSubgroupSize - 1)) == 0);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_PROPERTIES_EXT: {
                    VkPhysicalDeviceTexelBufferAlignmentPropertiesEXT* s = static_cast<VkPhysicalDeviceTexelBufferAlignmentPropertiesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->storageTexelBufferOffsetAlignmentBytes <= 256);
                    ret = ret && ((s->storageTexelBufferOffsetAlignmentBytes & (s->storageTexelBufferOffsetAlignmentBytes - 1)) == 0);
                    ret = ret && (s->uniformTexelBufferOffsetAlignmentBytes <= 256);
                    ret = ret && ((s->uniformTexelBufferOffsetAlignmentBytes & (s->uniformTexelBufferOffsetAlignmentBytes - 1)) == 0);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_PROPERTIES_KHR: {
                    VkPhysicalDeviceTimelineSemaphorePropertiesKHR* s = static_cast<VkPhysicalDeviceTimelineSemaphorePropertiesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->maxTimelineSemaphoreValueDifference >= 2147483647);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_PROPERTIES_EXT: {
                    VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT* s = static_cast<VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->maxVertexAttribDivisor >= 268435455);
                } break;
                default: break;
            }
        return ret;
    }
};

static const VpFormatDesc formatDesc[] = {
    {
        VK_FORMAT_A2B10G10R10_SNORM_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A2B10G10R10_UINT_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A2B10G10R10_UNORM_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A2R10G10B10_UNORM_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A8B8G8R8_SINT_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A8B8G8R8_SNORM_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A8B8G8R8_SRGB_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A8B8G8R8_SSCALED_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A8B8G8R8_UINT_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A8B8G8R8_UNORM_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A8B8G8R8_USCALED_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_B10G11R11_UFLOAT_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_B8G8R8A8_SRGB,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_B8G8R8A8_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC1_RGBA_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC1_RGBA_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC1_RGB_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC1_RGB_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC2_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC2_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC3_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC3_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC4_SNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC4_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC5_SNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC5_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC6H_SFLOAT_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC6H_UFLOAT_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC7_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC7_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_D16_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_D32_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_E5B9G9R9_UFLOAT_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16B16A16_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16B16A16_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16B16A16_SNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16B16A16_SSCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16B16A16_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16B16A16_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16B16A16_USCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16_SNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16_SSCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16_USCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16_SNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16_SSCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16_USCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32B32A32_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32B32A32_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32B32A32_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32B32_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32B32_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32B32_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8B8A8_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8B8A8_SNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8B8A8_SRGB,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8B8A8_SSCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8B8A8_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8B8A8_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8B8A8_USCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8_SNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8_SSCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8_USCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8_SNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8_SSCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8_USCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
};

static const VpStructChainerDesc chainerDesc = {
    [](VkBaseOutStructure* p, void* pUser, PFN_vpStructChainerCb pfnCb) {
        VkPhysicalDevice16BitStorageFeaturesKHR physicalDevice16BitStorageFeaturesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES_KHR, nullptr };
        VkPhysicalDevice8BitStorageFeaturesKHR physicalDevice8BitStorageFeaturesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES_KHR, &physicalDevice16BitStorageFeaturesKHR };
        VkPhysicalDeviceDescriptorIndexingFeaturesEXT physicalDeviceDescriptorIndexingFeaturesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT, &physicalDevice8BitStorageFeaturesKHR };
        VkPhysicalDeviceFloat16Int8FeaturesKHR physicalDeviceFloat16Int8FeaturesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT16_INT8_FEATURES_KHR, &physicalDeviceDescriptorIndexingFeaturesEXT };
        VkPhysicalDeviceHostQueryResetFeaturesEXT physicalDeviceHostQueryResetFeaturesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES_EXT, &physicalDeviceFloat16Int8FeaturesKHR };
        VkPhysicalDeviceInlineUniformBlockFeaturesEXT physicalDeviceInlineUniformBlockFeaturesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES_EXT, &physicalDeviceHostQueryResetFeaturesEXT };
        VkPhysicalDeviceMultiviewFeaturesKHR physicalDeviceMultiviewFeaturesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES_KHR, &physicalDeviceInlineUniformBlockFeaturesEXT };
        VkPhysicalDeviceRobustness2FeaturesEXT physicalDeviceRobustness2FeaturesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT, &physicalDeviceMultiviewFeaturesKHR };
        VkPhysicalDeviceScalarBlockLayoutFeaturesEXT physicalDeviceScalarBlockLayoutFeaturesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES_EXT, &physicalDeviceRobustness2FeaturesEXT };
        VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT physicalDeviceTexelBufferAlignmentFeaturesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_FEATURES_EXT, &physicalDeviceScalarBlockLayoutFeaturesEXT };
        VkPhysicalDeviceTimelineSemaphoreFeaturesKHR physicalDeviceTimelineSemaphoreFeaturesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES_KHR, &physicalDeviceTexelBufferAlignmentFeaturesEXT };
        VkPhysicalDeviceUniformBufferStandardLayoutFeaturesKHR physicalDeviceUniformBufferStandardLayoutFeaturesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES_KHR, &physicalDeviceTimelineSemaphoreFeaturesKHR };
        VkPhysicalDeviceVariablePointersFeaturesKHR physicalDeviceVariablePointersFeaturesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTERS_FEATURES, &physicalDeviceUniformBufferStandardLayoutFeaturesKHR };
        VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT physicalDeviceVertexAttributeDivisorFeaturesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_FEATURES_EXT, &physicalDeviceVariablePointersFeaturesKHR };
        p->pNext = static_cast<VkBaseOutStructure*>(static_cast<void*>(&physicalDeviceVertexAttributeDivisorFeaturesEXT));
        pfnCb(p, pUser);
    },
    [](VkBaseOutStructure* p, void* pUser, PFN_vpStructChainerCb pfnCb) {
        VkPhysicalDeviceDepthStencilResolvePropertiesKHR physicalDeviceDepthStencilResolvePropertiesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES_KHR, nullptr };
        VkPhysicalDeviceDescriptorIndexingPropertiesEXT physicalDeviceDescriptorIndexingPropertiesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES_EXT, &physicalDeviceDepthStencilResolvePropertiesKHR };
        VkPhysicalDeviceInlineUniformBlockPropertiesEXT physicalDeviceInlineUniformBlockPropertiesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_PROPERTIES_EXT, &physicalDeviceDescriptorIndexingPropertiesEXT };
        VkPhysicalDeviceMaintenance3PropertiesKHR physicalDeviceMaintenance3PropertiesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES_KHR, &physicalDeviceInlineUniformBlockPropertiesEXT };
        VkPhysicalDeviceMultiviewPropertiesKHR physicalDeviceMultiviewPropertiesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES_KHR, &physicalDeviceMaintenance3PropertiesKHR };
        VkPhysicalDeviceRobustness2PropertiesEXT physicalDeviceRobustness2PropertiesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_PROPERTIES_EXT, &physicalDeviceMultiviewPropertiesKHR };
        VkPhysicalDeviceSubgroupSizeControlPropertiesEXT physicalDeviceSubgroupSizeControlPropertiesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_PROPERTIES_EXT, &physicalDeviceRobustness2PropertiesEXT };
        VkPhysicalDeviceTexelBufferAlignmentPropertiesEXT physicalDeviceTexelBufferAlignmentPropertiesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_PROPERTIES_EXT, &physicalDeviceSubgroupSizeControlPropertiesEXT };
        VkPhysicalDeviceTimelineSemaphorePropertiesKHR physicalDeviceTimelineSemaphorePropertiesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_PROPERTIES_KHR, &physicalDeviceTexelBufferAlignmentPropertiesEXT };
        VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT physicalDeviceVertexAttributeDivisorPropertiesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_PROPERTIES_EXT, &physicalDeviceTimelineSemaphorePropertiesKHR };
        p->pNext = static_cast<VkBaseOutStructure*>(static_cast<void*>(&physicalDeviceVertexAttributeDivisorPropertiesEXT));
        pfnCb(p, pUser);
    },
    [](VkBaseOutStructure* p, void* pUser, PFN_vpStructChainerCb pfnCb) {
        pfnCb(p, pUser);
    },
    [](VkBaseOutStructure* p, void* pUser, PFN_vpStructChainerCb pfnCb) {
        VkFormatProperties3KHR formatProperties3KHR{ VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_3_KHR, nullptr };
        p->pNext = static_cast<VkBaseOutStructure*>(static_cast<void*>(&formatProperties3KHR));
        pfnCb(p, pUser);
    },
};

} // namespace VP_LUNARG_DESKTOP_BASELINE_2022
#endif

#ifdef VP_LUNARG_desktop_portability_2022
namespace VP_LUNARG_DESKTOP_PORTABILITY_2022 {

static const VkExtensionProperties instanceExtensions[] = {
    VkExtensionProperties{ VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME, 1 },
};

static const VkExtensionProperties deviceExtensions[] = {
    VkExtensionProperties{ VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME, 2 },
    VkExtensionProperties{ VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_EXT_MEMORY_BUDGET_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_EXT_ROBUSTNESS_2_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_EXT_SCALAR_BLOCK_LAYOUT_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME, 2 },
    VkExtensionProperties{ VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME, 3 },
    VkExtensionProperties{ VK_KHR_16BIT_STORAGE_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_8BIT_STORAGE_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_BIND_MEMORY_2_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME, 3 },
    VkExtensionProperties{ VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_DESCRIPTOR_UPDATE_TEMPLATE_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_DEVICE_GROUP_EXTENSION_NAME, 4 },
    VkExtensionProperties{ VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_EXTERNAL_FENCE_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_MAINTENANCE_1_EXTENSION_NAME, 2 },
    VkExtensionProperties{ VK_KHR_MAINTENANCE_2_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_MAINTENANCE_3_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_MULTIVIEW_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_RELAXED_BLOCK_LAYOUT_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_SAMPLER_MIRROR_CLAMP_TO_EDGE_EXTENSION_NAME, 3 },
    VkExtensionProperties{ VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_STORAGE_BUFFER_STORAGE_CLASS_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_SWAPCHAIN_EXTENSION_NAME, 70 },
    VkExtensionProperties{ VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME, 2 },
    VkExtensionProperties{ VK_KHR_UNIFORM_BUFFER_STANDARD_LAYOUT_EXTENSION_NAME, 1 },
    VkExtensionProperties{ VK_KHR_VARIABLE_POINTERS_EXTENSION_NAME, 1 },
};

static const VkStructureType featureStructTypes[] = {
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT16_INT8_FEATURES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_FEATURES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_FEATURES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTERS_FEATURES,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_FEATURES_EXT,
};

static const VkStructureType propertyStructTypes[] = {
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_PROPERTIES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_PROPERTIES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_PROPERTIES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_PROPERTIES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_PROPERTIES_EXT,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_PROPERTIES_KHR,
    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_PROPERTIES_EXT,
};

static const VkStructureType formatStructTypes[] = {
    VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR,
    VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_3_KHR,
};

static const VpFeatureDesc featureDesc = {
    [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES_KHR: {
                    VkPhysicalDevice16BitStorageFeaturesKHR* s = static_cast<VkPhysicalDevice16BitStorageFeaturesKHR*>(static_cast<void*>(p));
                    s->storageBuffer16BitAccess = VK_TRUE;
                    s->uniformAndStorageBuffer16BitAccess = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES_KHR: {
                    VkPhysicalDevice8BitStorageFeaturesKHR* s = static_cast<VkPhysicalDevice8BitStorageFeaturesKHR*>(static_cast<void*>(p));
                    s->storageBuffer8BitAccess = VK_TRUE;
                    s->uniformAndStorageBuffer8BitAccess = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT: {
                    VkPhysicalDeviceDescriptorIndexingFeaturesEXT* s = static_cast<VkPhysicalDeviceDescriptorIndexingFeaturesEXT*>(static_cast<void*>(p));
                    s->descriptorBindingPartiallyBound = VK_TRUE;
                    s->descriptorBindingSampledImageUpdateAfterBind = VK_TRUE;
                    s->descriptorBindingStorageBufferUpdateAfterBind = VK_TRUE;
                    s->descriptorBindingStorageImageUpdateAfterBind = VK_TRUE;
                    s->descriptorBindingStorageTexelBufferUpdateAfterBind = VK_TRUE;
                    s->descriptorBindingUniformTexelBufferUpdateAfterBind = VK_TRUE;
                    s->descriptorBindingUpdateUnusedWhilePending = VK_TRUE;
                    s->descriptorBindingVariableDescriptorCount = VK_TRUE;
                    s->runtimeDescriptorArray = VK_TRUE;
                    s->shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
                    s->shaderStorageTexelBufferArrayDynamicIndexing = VK_TRUE;
                    s->shaderUniformTexelBufferArrayDynamicIndexing = VK_TRUE;
                    s->shaderUniformTexelBufferArrayNonUniformIndexing = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR: {
                    VkPhysicalDeviceFeatures2KHR* s = static_cast<VkPhysicalDeviceFeatures2KHR*>(static_cast<void*>(p));
                    s->features.depthBiasClamp = VK_TRUE;
                    s->features.depthClamp = VK_TRUE;
                    s->features.drawIndirectFirstInstance = VK_TRUE;
                    s->features.dualSrcBlend = VK_TRUE;
                    s->features.fillModeNonSolid = VK_TRUE;
                    s->features.fragmentStoresAndAtomics = VK_TRUE;
                    s->features.fullDrawIndexUint32 = VK_TRUE;
                    s->features.imageCubeArray = VK_TRUE;
                    s->features.independentBlend = VK_TRUE;
                    s->features.inheritedQueries = VK_TRUE;
                    s->features.largePoints = VK_TRUE;
                    s->features.multiDrawIndirect = VK_TRUE;
                    s->features.multiViewport = VK_TRUE;
                    s->features.occlusionQueryPrecise = VK_TRUE;
                    s->features.robustBufferAccess = VK_TRUE;
                    s->features.sampleRateShading = VK_TRUE;
                    s->features.samplerAnisotropy = VK_TRUE;
                    s->features.shaderClipDistance = VK_TRUE;
                    s->features.shaderImageGatherExtended = VK_TRUE;
                    s->features.shaderSampledImageArrayDynamicIndexing = VK_TRUE;
                    s->features.shaderStorageBufferArrayDynamicIndexing = VK_TRUE;
                    s->features.shaderStorageImageArrayDynamicIndexing = VK_TRUE;
                    s->features.shaderStorageImageExtendedFormats = VK_TRUE;
                    s->features.shaderStorageImageWriteWithoutFormat = VK_TRUE;
                    s->features.shaderTessellationAndGeometryPointSize = VK_TRUE;
                    s->features.shaderUniformBufferArrayDynamicIndexing = VK_TRUE;
                    s->features.tessellationShader = VK_TRUE;
                    s->features.textureCompressionBC = VK_TRUE;
                    s->features.vertexPipelineStoresAndAtomics = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT16_INT8_FEATURES_KHR: {
                    VkPhysicalDeviceFloat16Int8FeaturesKHR* s = static_cast<VkPhysicalDeviceFloat16Int8FeaturesKHR*>(static_cast<void*>(p));
                    s->shaderInt8 = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES_EXT: {
                    VkPhysicalDeviceHostQueryResetFeaturesEXT* s = static_cast<VkPhysicalDeviceHostQueryResetFeaturesEXT*>(static_cast<void*>(p));
                    s->hostQueryReset = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES_EXT: {
                    VkPhysicalDeviceInlineUniformBlockFeaturesEXT* s = static_cast<VkPhysicalDeviceInlineUniformBlockFeaturesEXT*>(static_cast<void*>(p));
                    s->descriptorBindingInlineUniformBlockUpdateAfterBind = VK_TRUE;
                    s->inlineUniformBlock = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES_KHR: {
                    VkPhysicalDeviceMultiviewFeaturesKHR* s = static_cast<VkPhysicalDeviceMultiviewFeaturesKHR*>(static_cast<void*>(p));
                    s->multiview = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_FEATURES_KHR: {
                    VkPhysicalDevicePortabilitySubsetFeaturesKHR* s = static_cast<VkPhysicalDevicePortabilitySubsetFeaturesKHR*>(static_cast<void*>(p));
                    s->constantAlphaColorBlendFactors = VK_TRUE;
                    s->events = VK_TRUE;
                    s->imageViewFormatReinterpretation = VK_TRUE;
                    s->imageViewFormatSwizzle = VK_TRUE;
                    s->multisampleArrayImage = VK_TRUE;
                    s->mutableComparisonSamplers = VK_TRUE;
                    s->separateStencilMaskRef = VK_TRUE;
                    s->vertexAttributeAccessBeyondStride = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT: {
                    VkPhysicalDeviceRobustness2FeaturesEXT* s = static_cast<VkPhysicalDeviceRobustness2FeaturesEXT*>(static_cast<void*>(p));
                    s->robustImageAccess2 = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES_EXT: {
                    VkPhysicalDeviceScalarBlockLayoutFeaturesEXT* s = static_cast<VkPhysicalDeviceScalarBlockLayoutFeaturesEXT*>(static_cast<void*>(p));
                    s->scalarBlockLayout = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_FEATURES_EXT: {
                    VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT* s = static_cast<VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT*>(static_cast<void*>(p));
                    s->texelBufferAlignment = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES_KHR: {
                    VkPhysicalDeviceTimelineSemaphoreFeaturesKHR* s = static_cast<VkPhysicalDeviceTimelineSemaphoreFeaturesKHR*>(static_cast<void*>(p));
                    s->timelineSemaphore = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES_KHR: {
                    VkPhysicalDeviceUniformBufferStandardLayoutFeaturesKHR* s = static_cast<VkPhysicalDeviceUniformBufferStandardLayoutFeaturesKHR*>(static_cast<void*>(p));
                    s->uniformBufferStandardLayout = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTERS_FEATURES: {
                    VkPhysicalDeviceVariablePointersFeaturesKHR* s = static_cast<VkPhysicalDeviceVariablePointersFeaturesKHR*>(static_cast<void*>(p));
                    s->variablePointers = VK_TRUE;
                    s->variablePointersStorageBuffer = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_FEATURES_EXT: {
                    VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT* s = static_cast<VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT*>(static_cast<void*>(p));
                    s->vertexAttributeInstanceRateDivisor = VK_TRUE;
                    s->vertexAttributeInstanceRateZeroDivisor = VK_TRUE;
                } break;
                default: break;
            }
    },
    [](VkBaseOutStructure* p) -> bool {
        bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES_KHR: {
                    VkPhysicalDevice16BitStorageFeaturesKHR* s = static_cast<VkPhysicalDevice16BitStorageFeaturesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->storageBuffer16BitAccess == VK_TRUE);
                    ret = ret && (s->uniformAndStorageBuffer16BitAccess == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES_KHR: {
                    VkPhysicalDevice8BitStorageFeaturesKHR* s = static_cast<VkPhysicalDevice8BitStorageFeaturesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->storageBuffer8BitAccess == VK_TRUE);
                    ret = ret && (s->uniformAndStorageBuffer8BitAccess == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT: {
                    VkPhysicalDeviceDescriptorIndexingFeaturesEXT* s = static_cast<VkPhysicalDeviceDescriptorIndexingFeaturesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->descriptorBindingPartiallyBound == VK_TRUE);
                    ret = ret && (s->descriptorBindingSampledImageUpdateAfterBind == VK_TRUE);
                    ret = ret && (s->descriptorBindingStorageBufferUpdateAfterBind == VK_TRUE);
                    ret = ret && (s->descriptorBindingStorageImageUpdateAfterBind == VK_TRUE);
                    ret = ret && (s->descriptorBindingStorageTexelBufferUpdateAfterBind == VK_TRUE);
                    ret = ret && (s->descriptorBindingUniformTexelBufferUpdateAfterBind == VK_TRUE);
                    ret = ret && (s->descriptorBindingUpdateUnusedWhilePending == VK_TRUE);
                    ret = ret && (s->descriptorBindingVariableDescriptorCount == VK_TRUE);
                    ret = ret && (s->runtimeDescriptorArray == VK_TRUE);
                    ret = ret && (s->shaderSampledImageArrayNonUniformIndexing == VK_TRUE);
                    ret = ret && (s->shaderStorageTexelBufferArrayDynamicIndexing == VK_TRUE);
                    ret = ret && (s->shaderUniformTexelBufferArrayDynamicIndexing == VK_TRUE);
                    ret = ret && (s->shaderUniformTexelBufferArrayNonUniformIndexing == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR: {
                    VkPhysicalDeviceFeatures2KHR* s = static_cast<VkPhysicalDeviceFeatures2KHR*>(static_cast<void*>(p));
                    ret = ret && (s->features.depthBiasClamp == VK_TRUE);
                    ret = ret && (s->features.depthClamp == VK_TRUE);
                    ret = ret && (s->features.drawIndirectFirstInstance == VK_TRUE);
                    ret = ret && (s->features.dualSrcBlend == VK_TRUE);
                    ret = ret && (s->features.fillModeNonSolid == VK_TRUE);
                    ret = ret && (s->features.fragmentStoresAndAtomics == VK_TRUE);
                    ret = ret && (s->features.fullDrawIndexUint32 == VK_TRUE);
                    ret = ret && (s->features.imageCubeArray == VK_TRUE);
                    ret = ret && (s->features.independentBlend == VK_TRUE);
                    ret = ret && (s->features.inheritedQueries == VK_TRUE);
                    ret = ret && (s->features.largePoints == VK_TRUE);
                    ret = ret && (s->features.multiDrawIndirect == VK_TRUE);
                    ret = ret && (s->features.multiViewport == VK_TRUE);
                    ret = ret && (s->features.occlusionQueryPrecise == VK_TRUE);
                    ret = ret && (s->features.robustBufferAccess == VK_TRUE);
                    ret = ret && (s->features.sampleRateShading == VK_TRUE);
                    ret = ret && (s->features.samplerAnisotropy == VK_TRUE);
                    ret = ret && (s->features.shaderClipDistance == VK_TRUE);
                    ret = ret && (s->features.shaderImageGatherExtended == VK_TRUE);
                    ret = ret && (s->features.shaderSampledImageArrayDynamicIndexing == VK_TRUE);
                    ret = ret && (s->features.shaderStorageBufferArrayDynamicIndexing == VK_TRUE);
                    ret = ret && (s->features.shaderStorageImageArrayDynamicIndexing == VK_TRUE);
                    ret = ret && (s->features.shaderStorageImageExtendedFormats == VK_TRUE);
                    ret = ret && (s->features.shaderStorageImageWriteWithoutFormat == VK_TRUE);
                    ret = ret && (s->features.shaderTessellationAndGeometryPointSize == VK_TRUE);
                    ret = ret && (s->features.shaderUniformBufferArrayDynamicIndexing == VK_TRUE);
                    ret = ret && (s->features.tessellationShader == VK_TRUE);
                    ret = ret && (s->features.textureCompressionBC == VK_TRUE);
                    ret = ret && (s->features.vertexPipelineStoresAndAtomics == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT16_INT8_FEATURES_KHR: {
                    VkPhysicalDeviceFloat16Int8FeaturesKHR* s = static_cast<VkPhysicalDeviceFloat16Int8FeaturesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->shaderInt8 == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES_EXT: {
                    VkPhysicalDeviceHostQueryResetFeaturesEXT* s = static_cast<VkPhysicalDeviceHostQueryResetFeaturesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->hostQueryReset == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES_EXT: {
                    VkPhysicalDeviceInlineUniformBlockFeaturesEXT* s = static_cast<VkPhysicalDeviceInlineUniformBlockFeaturesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->descriptorBindingInlineUniformBlockUpdateAfterBind == VK_TRUE);
                    ret = ret && (s->inlineUniformBlock == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES_KHR: {
                    VkPhysicalDeviceMultiviewFeaturesKHR* s = static_cast<VkPhysicalDeviceMultiviewFeaturesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->multiview == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_FEATURES_KHR: {
                    VkPhysicalDevicePortabilitySubsetFeaturesKHR* s = static_cast<VkPhysicalDevicePortabilitySubsetFeaturesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->constantAlphaColorBlendFactors == VK_TRUE);
                    ret = ret && (s->events == VK_TRUE);
                    ret = ret && (s->imageViewFormatReinterpretation == VK_TRUE);
                    ret = ret && (s->imageViewFormatSwizzle == VK_TRUE);
                    ret = ret && (s->multisampleArrayImage == VK_TRUE);
                    ret = ret && (s->mutableComparisonSamplers == VK_TRUE);
                    ret = ret && (s->separateStencilMaskRef == VK_TRUE);
                    ret = ret && (s->vertexAttributeAccessBeyondStride == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT: {
                    VkPhysicalDeviceRobustness2FeaturesEXT* s = static_cast<VkPhysicalDeviceRobustness2FeaturesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->robustImageAccess2 == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES_EXT: {
                    VkPhysicalDeviceScalarBlockLayoutFeaturesEXT* s = static_cast<VkPhysicalDeviceScalarBlockLayoutFeaturesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->scalarBlockLayout == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_FEATURES_EXT: {
                    VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT* s = static_cast<VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->texelBufferAlignment == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES_KHR: {
                    VkPhysicalDeviceTimelineSemaphoreFeaturesKHR* s = static_cast<VkPhysicalDeviceTimelineSemaphoreFeaturesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->timelineSemaphore == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES_KHR: {
                    VkPhysicalDeviceUniformBufferStandardLayoutFeaturesKHR* s = static_cast<VkPhysicalDeviceUniformBufferStandardLayoutFeaturesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->uniformBufferStandardLayout == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTERS_FEATURES: {
                    VkPhysicalDeviceVariablePointersFeaturesKHR* s = static_cast<VkPhysicalDeviceVariablePointersFeaturesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->variablePointers == VK_TRUE);
                    ret = ret && (s->variablePointersStorageBuffer == VK_TRUE);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_FEATURES_EXT: {
                    VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT* s = static_cast<VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->vertexAttributeInstanceRateDivisor == VK_TRUE);
                    ret = ret && (s->vertexAttributeInstanceRateZeroDivisor == VK_TRUE);
                } break;
                default: break;
            }
        return ret;
    }
};

static const VpPropertyDesc propertyDesc = {
    [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES_KHR: {
                    VkPhysicalDeviceDepthStencilResolvePropertiesKHR* s = static_cast<VkPhysicalDeviceDepthStencilResolvePropertiesKHR*>(static_cast<void*>(p));
                    s->independentResolve = VK_TRUE;
                    s->independentResolveNone = VK_TRUE;
                    s->supportedDepthResolveModes = (VK_RESOLVE_MODE_SAMPLE_ZERO_BIT);
                    s->supportedStencilResolveModes = (VK_RESOLVE_MODE_SAMPLE_ZERO_BIT);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES_EXT: {
                    VkPhysicalDeviceDescriptorIndexingPropertiesEXT* s = static_cast<VkPhysicalDeviceDescriptorIndexingPropertiesEXT*>(static_cast<void*>(p));
                    s->maxDescriptorSetUpdateAfterBindInputAttachments = 256;
                    s->maxDescriptorSetUpdateAfterBindSampledImages = 640;
                    s->maxDescriptorSetUpdateAfterBindSamplers = 80;
                    s->maxDescriptorSetUpdateAfterBindStorageBuffers = 155;
                    s->maxDescriptorSetUpdateAfterBindStorageBuffersDynamic = 8;
                    s->maxDescriptorSetUpdateAfterBindStorageImages = 40;
                    s->maxDescriptorSetUpdateAfterBindUniformBuffers = 90;
                    s->maxDescriptorSetUpdateAfterBindUniformBuffersDynamic = 8;
                    s->maxPerStageDescriptorUpdateAfterBindInputAttachments = 64;
                    s->maxPerStageDescriptorUpdateAfterBindSampledImages = 128;
                    s->maxPerStageDescriptorUpdateAfterBindSamplers = 16;
                    s->maxPerStageDescriptorUpdateAfterBindStorageBuffers = 31;
                    s->maxPerStageDescriptorUpdateAfterBindStorageImages = 8;
                    s->maxPerStageDescriptorUpdateAfterBindUniformBuffers = 15;
                    s->maxPerStageUpdateAfterBindResources = 159;
                    s->maxUpdateAfterBindDescriptorsInAllPools = 1048576;
                    s->quadDivergentImplicitLod = VK_TRUE;
                    s->robustBufferAccessUpdateAfterBind = VK_TRUE;
                    s->shaderInputAttachmentArrayNonUniformIndexingNative = VK_TRUE;
                    s->shaderSampledImageArrayNonUniformIndexingNative = VK_TRUE;
                    s->shaderStorageBufferArrayNonUniformIndexingNative = VK_TRUE;
                    s->shaderStorageImageArrayNonUniformIndexingNative = VK_TRUE;
                    s->shaderUniformBufferArrayNonUniformIndexingNative = VK_TRUE;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_PROPERTIES_EXT: {
                    VkPhysicalDeviceInlineUniformBlockPropertiesEXT* s = static_cast<VkPhysicalDeviceInlineUniformBlockPropertiesEXT*>(static_cast<void*>(p));
                    s->maxDescriptorSetInlineUniformBlocks = 4;
                    s->maxDescriptorSetUpdateAfterBindInlineUniformBlocks = 4;
                    s->maxInlineUniformBlockSize = 256;
                    s->maxPerStageDescriptorInlineUniformBlocks = 4;
                    s->maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks = 4;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES_KHR: {
                    VkPhysicalDeviceMaintenance3PropertiesKHR* s = static_cast<VkPhysicalDeviceMaintenance3PropertiesKHR*>(static_cast<void*>(p));
                    s->maxMemoryAllocationSize = 2147483648;
                    s->maxPerSetDescriptors = 700;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES_KHR: {
                    VkPhysicalDeviceMultiviewPropertiesKHR* s = static_cast<VkPhysicalDeviceMultiviewPropertiesKHR*>(static_cast<void*>(p));
                    s->maxMultiviewInstanceIndex = 134217727;
                    s->maxMultiviewViewCount = 6;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_PROPERTIES_KHR: {
                    VkPhysicalDevicePortabilitySubsetPropertiesKHR* s = static_cast<VkPhysicalDevicePortabilitySubsetPropertiesKHR*>(static_cast<void*>(p));
                    s->minVertexInputBindingStrideAlignment = 4;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR: {
                    VkPhysicalDeviceProperties2KHR* s = static_cast<VkPhysicalDeviceProperties2KHR*>(static_cast<void*>(p));
                    s->properties.limits.bufferImageGranularity = 65536;
                    s->properties.limits.discreteQueuePriorities = 2;
                    s->properties.limits.framebufferColorSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.framebufferDepthSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.framebufferNoAttachmentsSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.framebufferStencilSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.lineWidthGranularity = 1;
                    s->properties.limits.lineWidthRange[0] = 1;
                    s->properties.limits.lineWidthRange[1] = 1;
                    s->properties.limits.maxBoundDescriptorSets = 8;
                    s->properties.limits.maxClipDistances = 8;
                    s->properties.limits.maxColorAttachments = 8;
                    s->properties.limits.maxCombinedClipAndCullDistances = 8;
                    s->properties.limits.maxComputeSharedMemorySize = 32768;
                    s->properties.limits.maxComputeWorkGroupCount[0] = 65535;
                    s->properties.limits.maxComputeWorkGroupCount[1] = 65535;
                    s->properties.limits.maxComputeWorkGroupCount[2] = 65535;
                    s->properties.limits.maxComputeWorkGroupInvocations = 1024;
                    s->properties.limits.maxCullDistances = 0;
                    s->properties.limits.maxDescriptorSetInputAttachments = 8;
                    s->properties.limits.maxDescriptorSetSampledImages = 640;
                    s->properties.limits.maxDescriptorSetSamplers = 80;
                    s->properties.limits.maxDescriptorSetStorageBuffers = 155;
                    s->properties.limits.maxDescriptorSetStorageBuffersDynamic = 8;
                    s->properties.limits.maxDescriptorSetStorageImages = 40;
                    s->properties.limits.maxDescriptorSetUniformBuffers = 90;
                    s->properties.limits.maxDescriptorSetUniformBuffersDynamic = 8;
                    s->properties.limits.maxDrawIndexedIndexValue = 4294967295;
                    s->properties.limits.maxDrawIndirectCount = 1073741824;
                    s->properties.limits.maxFragmentCombinedOutputResources = 8;
                    s->properties.limits.maxFragmentDualSrcAttachments = 1;
                    s->properties.limits.maxFragmentInputComponents = 116;
                    s->properties.limits.maxFragmentOutputAttachments = 8;
                    s->properties.limits.maxFramebufferHeight = 16384;
                    s->properties.limits.maxFramebufferLayers = 1024;
                    s->properties.limits.maxFramebufferWidth = 16384;
                    s->properties.limits.maxGeometryInputComponents = 0;
                    s->properties.limits.maxGeometryOutputComponents = 0;
                    s->properties.limits.maxGeometryOutputVertices = 0;
                    s->properties.limits.maxGeometryShaderInvocations = 0;
                    s->properties.limits.maxGeometryTotalOutputComponents = 0;
                    s->properties.limits.maxImageArrayLayers = 2048;
                    s->properties.limits.maxImageDimension1D = 16384;
                    s->properties.limits.maxImageDimension2D = 16384;
                    s->properties.limits.maxImageDimension3D = 2048;
                    s->properties.limits.maxImageDimensionCube = 16384;
                    s->properties.limits.maxInterpolationOffset = 0.4375f;
                    s->properties.limits.maxMemoryAllocationCount = 4096;
                    s->properties.limits.maxPerStageDescriptorInputAttachments = 8;
                    s->properties.limits.maxPerStageDescriptorSampledImages = 128;
                    s->properties.limits.maxPerStageDescriptorSamplers = 16;
                    s->properties.limits.maxPerStageDescriptorStorageBuffers = 31;
                    s->properties.limits.maxPerStageDescriptorStorageImages = 8;
                    s->properties.limits.maxPerStageDescriptorUniformBuffers = 15;
                    s->properties.limits.maxPerStageResources = 159;
                    s->properties.limits.maxPushConstantsSize = 128;
                    s->properties.limits.maxSampleMaskWords = 1;
                    s->properties.limits.maxSamplerAllocationCount = 1024;
                    s->properties.limits.maxSamplerAnisotropy = 16;
                    s->properties.limits.maxSamplerLodBias = 0;
                    s->properties.limits.maxStorageBufferRange = 1073741824;
                    s->properties.limits.maxTessellationControlPerPatchOutputComponents = 120;
                    s->properties.limits.maxTessellationControlPerVertexInputComponents = 124;
                    s->properties.limits.maxTessellationControlPerVertexOutputComponents = 124;
                    s->properties.limits.maxTessellationControlTotalOutputComponents = 2048;
                    s->properties.limits.maxTessellationEvaluationInputComponents = 124;
                    s->properties.limits.maxTessellationEvaluationOutputComponents = 124;
                    s->properties.limits.maxTessellationGenerationLevel = 64;
                    s->properties.limits.maxTessellationPatchSize = 32;
                    s->properties.limits.maxTexelBufferElements = 67108864;
                    s->properties.limits.maxTexelGatherOffset = 7;
                    s->properties.limits.maxTexelOffset = 7;
                    s->properties.limits.maxUniformBufferRange = 65536;
                    s->properties.limits.maxVertexInputAttributeOffset = 2047;
                    s->properties.limits.maxVertexInputAttributes = 28;
                    s->properties.limits.maxVertexInputBindingStride = 2048;
                    s->properties.limits.maxVertexInputBindings = 28;
                    s->properties.limits.maxVertexOutputComponents = 124;
                    s->properties.limits.maxViewportDimensions[0] = 16384;
                    s->properties.limits.maxViewportDimensions[1] = 16384;
                    s->properties.limits.maxViewports = 16;
                    s->properties.limits.minInterpolationOffset = -0.5f;
                    s->properties.limits.minMemoryMapAlignment = 4096;
                    s->properties.limits.minStorageBufferOffsetAlignment = 64;
                    s->properties.limits.minTexelBufferOffsetAlignment = 256;
                    s->properties.limits.minTexelGatherOffset = -8;
                    s->properties.limits.minTexelOffset = -8;
                    s->properties.limits.minUniformBufferOffsetAlignment = 256;
                    s->properties.limits.mipmapPrecisionBits = 4;
                    s->properties.limits.nonCoherentAtomSize = 256;
                    s->properties.limits.optimalBufferCopyOffsetAlignment = 256;
                    s->properties.limits.optimalBufferCopyRowPitchAlignment = 128;
                    s->properties.limits.pointSizeGranularity = 1;
                    s->properties.limits.pointSizeRange[0] = 1;
                    s->properties.limits.pointSizeRange[1] = 64;
                    s->properties.limits.sampledImageColorSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.sampledImageDepthSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.sampledImageIntegerSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.sampledImageStencilSampleCounts = (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT);
                    s->properties.limits.sparseAddressSpaceSize = 0;
                    s->properties.limits.storageImageSampleCounts = (VK_SAMPLE_COUNT_1_BIT);
                    s->properties.limits.subPixelInterpolationOffsetBits = 4;
                    s->properties.limits.subPixelPrecisionBits = 4;
                    s->properties.limits.subTexelPrecisionBits = 4;
                    s->properties.limits.timestampComputeAndGraphics = VK_TRUE;
                    s->properties.limits.viewportBoundsRange[0] = -32768;
                    s->properties.limits.viewportBoundsRange[1] = 32767;
                    s->properties.limits.viewportSubPixelBits = 0;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_PROPERTIES_EXT: {
                    VkPhysicalDeviceRobustness2PropertiesEXT* s = static_cast<VkPhysicalDeviceRobustness2PropertiesEXT*>(static_cast<void*>(p));
                    s->robustStorageBufferAccessSizeAlignment = 4;
                    s->robustUniformBufferAccessSizeAlignment = 256;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_PROPERTIES_EXT: {
                    VkPhysicalDeviceSubgroupSizeControlPropertiesEXT* s = static_cast<VkPhysicalDeviceSubgroupSizeControlPropertiesEXT*>(static_cast<void*>(p));
                    s->maxComputeWorkgroupSubgroups = 16;
                    s->maxSubgroupSize = 1;
                    s->minSubgroupSize = 64;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_PROPERTIES_EXT: {
                    VkPhysicalDeviceTexelBufferAlignmentPropertiesEXT* s = static_cast<VkPhysicalDeviceTexelBufferAlignmentPropertiesEXT*>(static_cast<void*>(p));
                    s->storageTexelBufferOffsetAlignmentBytes = 256;
                    s->uniformTexelBufferOffsetAlignmentBytes = 256;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_PROPERTIES_KHR: {
                    VkPhysicalDeviceTimelineSemaphorePropertiesKHR* s = static_cast<VkPhysicalDeviceTimelineSemaphorePropertiesKHR*>(static_cast<void*>(p));
                    s->maxTimelineSemaphoreValueDifference = 2147483647;
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_PROPERTIES_EXT: {
                    VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT* s = static_cast<VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT*>(static_cast<void*>(p));
                    s->maxVertexAttribDivisor = 268435455;
                } break;
                default: break;
            }
    },
    [](VkBaseOutStructure* p) -> bool {
        bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES_KHR: {
                    VkPhysicalDeviceDepthStencilResolvePropertiesKHR* s = static_cast<VkPhysicalDeviceDepthStencilResolvePropertiesKHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->independentResolve, VK_TRUE));
                    ret = ret && (vpCheckFlags(s->independentResolveNone, VK_TRUE));
                    ret = ret && (vpCheckFlags(s->supportedDepthResolveModes, (VK_RESOLVE_MODE_SAMPLE_ZERO_BIT)));
                    ret = ret && (vpCheckFlags(s->supportedStencilResolveModes, (VK_RESOLVE_MODE_SAMPLE_ZERO_BIT)));
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES_EXT: {
                    VkPhysicalDeviceDescriptorIndexingPropertiesEXT* s = static_cast<VkPhysicalDeviceDescriptorIndexingPropertiesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindInputAttachments >= 256);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindSampledImages >= 640);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindSamplers >= 80);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindStorageBuffers >= 155);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindStorageBuffersDynamic >= 8);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindStorageImages >= 40);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindUniformBuffers >= 90);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindUniformBuffersDynamic >= 8);
                    ret = ret && (s->maxPerStageDescriptorUpdateAfterBindInputAttachments >= 64);
                    ret = ret && (s->maxPerStageDescriptorUpdateAfterBindSampledImages >= 128);
                    ret = ret && (s->maxPerStageDescriptorUpdateAfterBindSamplers >= 16);
                    ret = ret && (s->maxPerStageDescriptorUpdateAfterBindStorageBuffers >= 31);
                    ret = ret && (s->maxPerStageDescriptorUpdateAfterBindStorageImages >= 8);
                    ret = ret && (s->maxPerStageDescriptorUpdateAfterBindUniformBuffers >= 15);
                    ret = ret && (s->maxPerStageUpdateAfterBindResources >= 159);
                    ret = ret && (s->maxUpdateAfterBindDescriptorsInAllPools >= 1048576);
                    ret = ret && (vpCheckFlags(s->quadDivergentImplicitLod, VK_TRUE));
                    ret = ret && (vpCheckFlags(s->robustBufferAccessUpdateAfterBind, VK_TRUE));
                    ret = ret && (vpCheckFlags(s->shaderInputAttachmentArrayNonUniformIndexingNative, VK_TRUE));
                    ret = ret && (vpCheckFlags(s->shaderSampledImageArrayNonUniformIndexingNative, VK_TRUE));
                    ret = ret && (vpCheckFlags(s->shaderStorageBufferArrayNonUniformIndexingNative, VK_TRUE));
                    ret = ret && (vpCheckFlags(s->shaderStorageImageArrayNonUniformIndexingNative, VK_TRUE));
                    ret = ret && (vpCheckFlags(s->shaderUniformBufferArrayNonUniformIndexingNative, VK_TRUE));
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_PROPERTIES_EXT: {
                    VkPhysicalDeviceInlineUniformBlockPropertiesEXT* s = static_cast<VkPhysicalDeviceInlineUniformBlockPropertiesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->maxDescriptorSetInlineUniformBlocks >= 4);
                    ret = ret && (s->maxDescriptorSetUpdateAfterBindInlineUniformBlocks >= 4);
                    ret = ret && (s->maxInlineUniformBlockSize >= 256);
                    ret = ret && (s->maxPerStageDescriptorInlineUniformBlocks >= 4);
                    ret = ret && (s->maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks >= 4);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES_KHR: {
                    VkPhysicalDeviceMaintenance3PropertiesKHR* s = static_cast<VkPhysicalDeviceMaintenance3PropertiesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->maxMemoryAllocationSize >= 2147483648);
                    ret = ret && (s->maxPerSetDescriptors >= 700);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES_KHR: {
                    VkPhysicalDeviceMultiviewPropertiesKHR* s = static_cast<VkPhysicalDeviceMultiviewPropertiesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->maxMultiviewInstanceIndex >= 134217727);
                    ret = ret && (s->maxMultiviewViewCount >= 6);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_PROPERTIES_KHR: {
                    VkPhysicalDevicePortabilitySubsetPropertiesKHR* s = static_cast<VkPhysicalDevicePortabilitySubsetPropertiesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->minVertexInputBindingStrideAlignment <= 4);
                    ret = ret && ((s->minVertexInputBindingStrideAlignment & (s->minVertexInputBindingStrideAlignment - 1)) == 0);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR: {
                    VkPhysicalDeviceProperties2KHR* s = static_cast<VkPhysicalDeviceProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (s->properties.limits.bufferImageGranularity <= 65536);
                    ret = ret && ((65536 % s->properties.limits.bufferImageGranularity) == 0);
                    ret = ret && (s->properties.limits.discreteQueuePriorities >= 2);
                    ret = ret && (vpCheckFlags(s->properties.limits.framebufferColorSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (vpCheckFlags(s->properties.limits.framebufferDepthSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (vpCheckFlags(s->properties.limits.framebufferNoAttachmentsSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (vpCheckFlags(s->properties.limits.framebufferStencilSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (s->properties.limits.lineWidthGranularity <= 1);
                    ret = ret && (isMultiple(1, s->properties.limits.lineWidthGranularity));
                    ret = ret && (s->properties.limits.lineWidthRange[0] <= 1);
                    ret = ret && (s->properties.limits.lineWidthRange[1] >= 1);
                    ret = ret && (s->properties.limits.maxBoundDescriptorSets >= 8);
                    ret = ret && (s->properties.limits.maxClipDistances >= 8);
                    ret = ret && (s->properties.limits.maxColorAttachments >= 8);
                    ret = ret && (s->properties.limits.maxCombinedClipAndCullDistances >= 8);
                    ret = ret && (s->properties.limits.maxComputeSharedMemorySize >= 32768);
                    ret = ret && (s->properties.limits.maxComputeWorkGroupCount[0] >= 65535);
                    ret = ret && (s->properties.limits.maxComputeWorkGroupCount[1] >= 65535);
                    ret = ret && (s->properties.limits.maxComputeWorkGroupCount[2] >= 65535);
                    ret = ret && (s->properties.limits.maxComputeWorkGroupInvocations >= 1024);
                    ret = ret && (s->properties.limits.maxCullDistances >= 0);
                    ret = ret && (s->properties.limits.maxDescriptorSetInputAttachments >= 8);
                    ret = ret && (s->properties.limits.maxDescriptorSetSampledImages >= 640);
                    ret = ret && (s->properties.limits.maxDescriptorSetSamplers >= 80);
                    ret = ret && (s->properties.limits.maxDescriptorSetStorageBuffers >= 155);
                    ret = ret && (s->properties.limits.maxDescriptorSetStorageBuffersDynamic >= 8);
                    ret = ret && (s->properties.limits.maxDescriptorSetStorageImages >= 40);
                    ret = ret && (s->properties.limits.maxDescriptorSetUniformBuffers >= 90);
                    ret = ret && (s->properties.limits.maxDescriptorSetUniformBuffersDynamic >= 8);
                    ret = ret && (s->properties.limits.maxDrawIndexedIndexValue >= 4294967295);
                    ret = ret && (s->properties.limits.maxDrawIndirectCount >= 1073741824);
                    ret = ret && (s->properties.limits.maxFragmentCombinedOutputResources >= 8);
                    ret = ret && (s->properties.limits.maxFragmentDualSrcAttachments >= 1);
                    ret = ret && (s->properties.limits.maxFragmentInputComponents >= 116);
                    ret = ret && (s->properties.limits.maxFragmentOutputAttachments >= 8);
                    ret = ret && (s->properties.limits.maxFramebufferHeight >= 16384);
                    ret = ret && (s->properties.limits.maxFramebufferLayers >= 1024);
                    ret = ret && (s->properties.limits.maxFramebufferWidth >= 16384);
                    ret = ret && (s->properties.limits.maxGeometryInputComponents >= 0);
                    ret = ret && (s->properties.limits.maxGeometryOutputComponents >= 0);
                    ret = ret && (s->properties.limits.maxGeometryOutputVertices >= 0);
                    ret = ret && (s->properties.limits.maxGeometryShaderInvocations >= 0);
                    ret = ret && (s->properties.limits.maxGeometryTotalOutputComponents >= 0);
                    ret = ret && (s->properties.limits.maxImageArrayLayers >= 2048);
                    ret = ret && (s->properties.limits.maxImageDimension1D >= 16384);
                    ret = ret && (s->properties.limits.maxImageDimension2D >= 16384);
                    ret = ret && (s->properties.limits.maxImageDimension3D >= 2048);
                    ret = ret && (s->properties.limits.maxImageDimensionCube >= 16384);
                    ret = ret && (s->properties.limits.maxInterpolationOffset >= 0.4375);
                    ret = ret && (s->properties.limits.maxMemoryAllocationCount >= 4096);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorInputAttachments >= 8);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorSampledImages >= 128);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorSamplers >= 16);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorStorageBuffers >= 31);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorStorageImages >= 8);
                    ret = ret && (s->properties.limits.maxPerStageDescriptorUniformBuffers >= 15);
                    ret = ret && (s->properties.limits.maxPerStageResources >= 159);
                    ret = ret && (s->properties.limits.maxPushConstantsSize >= 128);
                    ret = ret && (s->properties.limits.maxSampleMaskWords >= 1);
                    ret = ret && (s->properties.limits.maxSamplerAllocationCount >= 1024);
                    ret = ret && (s->properties.limits.maxSamplerAnisotropy >= 16);
                    ret = ret && (s->properties.limits.maxSamplerLodBias >= 0);
                    ret = ret && (s->properties.limits.maxStorageBufferRange >= 1073741824);
                    ret = ret && (s->properties.limits.maxTessellationControlPerPatchOutputComponents >= 120);
                    ret = ret && (s->properties.limits.maxTessellationControlPerVertexInputComponents >= 124);
                    ret = ret && (s->properties.limits.maxTessellationControlPerVertexOutputComponents >= 124);
                    ret = ret && (s->properties.limits.maxTessellationControlTotalOutputComponents >= 2048);
                    ret = ret && (s->properties.limits.maxTessellationEvaluationInputComponents >= 124);
                    ret = ret && (s->properties.limits.maxTessellationEvaluationOutputComponents >= 124);
                    ret = ret && (s->properties.limits.maxTessellationGenerationLevel >= 64);
                    ret = ret && (s->properties.limits.maxTessellationPatchSize >= 32);
                    ret = ret && (s->properties.limits.maxTexelBufferElements >= 67108864);
                    ret = ret && (s->properties.limits.maxTexelGatherOffset >= 7);
                    ret = ret && (s->properties.limits.maxTexelOffset >= 7);
                    ret = ret && (s->properties.limits.maxUniformBufferRange >= 65536);
                    ret = ret && (s->properties.limits.maxVertexInputAttributeOffset >= 2047);
                    ret = ret && (s->properties.limits.maxVertexInputAttributes >= 28);
                    ret = ret && (s->properties.limits.maxVertexInputBindingStride >= 2048);
                    ret = ret && (s->properties.limits.maxVertexInputBindings >= 28);
                    ret = ret && (s->properties.limits.maxVertexOutputComponents >= 124);
                    ret = ret && (s->properties.limits.maxViewportDimensions[0] >= 16384);
                    ret = ret && (s->properties.limits.maxViewportDimensions[1] >= 16384);
                    ret = ret && (s->properties.limits.maxViewports >= 16);
                    ret = ret && (s->properties.limits.minInterpolationOffset <= -0.5);
                    ret = ret && (s->properties.limits.minMemoryMapAlignment <= 4096);
                    ret = ret && ((s->properties.limits.minMemoryMapAlignment & (s->properties.limits.minMemoryMapAlignment - 1)) == 0);
                    ret = ret && (s->properties.limits.minStorageBufferOffsetAlignment <= 64);
                    ret = ret && ((s->properties.limits.minStorageBufferOffsetAlignment & (s->properties.limits.minStorageBufferOffsetAlignment - 1)) == 0);
                    ret = ret && (s->properties.limits.minTexelBufferOffsetAlignment <= 256);
                    ret = ret && ((s->properties.limits.minTexelBufferOffsetAlignment & (s->properties.limits.minTexelBufferOffsetAlignment - 1)) == 0);
                    ret = ret && (s->properties.limits.minTexelGatherOffset <= -8);
                    ret = ret && (s->properties.limits.minTexelOffset <= -8);
                    ret = ret && (s->properties.limits.minUniformBufferOffsetAlignment <= 256);
                    ret = ret && ((s->properties.limits.minUniformBufferOffsetAlignment & (s->properties.limits.minUniformBufferOffsetAlignment - 1)) == 0);
                    ret = ret && (s->properties.limits.mipmapPrecisionBits >= 4);
                    ret = ret && (s->properties.limits.nonCoherentAtomSize <= 256);
                    ret = ret && ((s->properties.limits.nonCoherentAtomSize & (s->properties.limits.nonCoherentAtomSize - 1)) == 0);
                    ret = ret && (s->properties.limits.optimalBufferCopyOffsetAlignment <= 256);
                    ret = ret && ((s->properties.limits.optimalBufferCopyOffsetAlignment & (s->properties.limits.optimalBufferCopyOffsetAlignment - 1)) == 0);
                    ret = ret && (s->properties.limits.optimalBufferCopyRowPitchAlignment <= 128);
                    ret = ret && ((s->properties.limits.optimalBufferCopyRowPitchAlignment & (s->properties.limits.optimalBufferCopyRowPitchAlignment - 1)) == 0);
                    ret = ret && (s->properties.limits.pointSizeGranularity <= 1);
                    ret = ret && (isMultiple(1, s->properties.limits.pointSizeGranularity));
                    ret = ret && (s->properties.limits.pointSizeRange[0] <= 1);
                    ret = ret && (s->properties.limits.pointSizeRange[1] >= 64);
                    ret = ret && (vpCheckFlags(s->properties.limits.sampledImageColorSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (vpCheckFlags(s->properties.limits.sampledImageDepthSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (vpCheckFlags(s->properties.limits.sampledImageIntegerSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (vpCheckFlags(s->properties.limits.sampledImageStencilSampleCounts, (VK_SAMPLE_COUNT_1_BIT | VK_SAMPLE_COUNT_2_BIT | VK_SAMPLE_COUNT_4_BIT)));
                    ret = ret && (s->properties.limits.sparseAddressSpaceSize >= 0);
                    ret = ret && (vpCheckFlags(s->properties.limits.storageImageSampleCounts, (VK_SAMPLE_COUNT_1_BIT)));
                    ret = ret && (s->properties.limits.subPixelInterpolationOffsetBits >= 4);
                    ret = ret && (s->properties.limits.subPixelPrecisionBits >= 4);
                    ret = ret && (s->properties.limits.subTexelPrecisionBits >= 4);
                    ret = ret && (vpCheckFlags(s->properties.limits.timestampComputeAndGraphics, VK_TRUE));
                    ret = ret && (s->properties.limits.viewportBoundsRange[0] <= -32768);
                    ret = ret && (s->properties.limits.viewportBoundsRange[1] >= 32767);
                    ret = ret && (s->properties.limits.viewportSubPixelBits >= 0);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_PROPERTIES_EXT: {
                    VkPhysicalDeviceRobustness2PropertiesEXT* s = static_cast<VkPhysicalDeviceRobustness2PropertiesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->robustStorageBufferAccessSizeAlignment <= 4);
                    ret = ret && ((s->robustStorageBufferAccessSizeAlignment & (s->robustStorageBufferAccessSizeAlignment - 1)) == 0);
                    ret = ret && (s->robustUniformBufferAccessSizeAlignment <= 256);
                    ret = ret && ((s->robustUniformBufferAccessSizeAlignment & (s->robustUniformBufferAccessSizeAlignment - 1)) == 0);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_PROPERTIES_EXT: {
                    VkPhysicalDeviceSubgroupSizeControlPropertiesEXT* s = static_cast<VkPhysicalDeviceSubgroupSizeControlPropertiesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->maxComputeWorkgroupSubgroups >= 16);
                    ret = ret && (s->maxSubgroupSize >= 1);
                    ret = ret && ((s->maxSubgroupSize & (s->maxSubgroupSize - 1)) == 0);
                    ret = ret && (s->minSubgroupSize <= 64);
                    ret = ret && ((s->minSubgroupSize & (s->minSubgroupSize - 1)) == 0);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_PROPERTIES_EXT: {
                    VkPhysicalDeviceTexelBufferAlignmentPropertiesEXT* s = static_cast<VkPhysicalDeviceTexelBufferAlignmentPropertiesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->storageTexelBufferOffsetAlignmentBytes <= 256);
                    ret = ret && ((s->storageTexelBufferOffsetAlignmentBytes & (s->storageTexelBufferOffsetAlignmentBytes - 1)) == 0);
                    ret = ret && (s->uniformTexelBufferOffsetAlignmentBytes <= 256);
                    ret = ret && ((s->uniformTexelBufferOffsetAlignmentBytes & (s->uniformTexelBufferOffsetAlignmentBytes - 1)) == 0);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_PROPERTIES_KHR: {
                    VkPhysicalDeviceTimelineSemaphorePropertiesKHR* s = static_cast<VkPhysicalDeviceTimelineSemaphorePropertiesKHR*>(static_cast<void*>(p));
                    ret = ret && (s->maxTimelineSemaphoreValueDifference >= 2147483647);
                } break;
                case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_PROPERTIES_EXT: {
                    VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT* s = static_cast<VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT*>(static_cast<void*>(p));
                    ret = ret && (s->maxVertexAttribDivisor >= 268435455);
                } break;
                default: break;
            }
        return ret;
    }
};

static const VpFormatDesc formatDesc[] = {
    {
        VK_FORMAT_A2B10G10R10_SNORM_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A2B10G10R10_UINT_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A2B10G10R10_UNORM_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A2R10G10B10_UNORM_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A8B8G8R8_SINT_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A8B8G8R8_SNORM_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A8B8G8R8_SRGB_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A8B8G8R8_SSCALED_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A8B8G8R8_UINT_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A8B8G8R8_UNORM_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_A8B8G8R8_USCALED_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_B10G11R11_UFLOAT_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_B8G8R8A8_SRGB,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_B8G8R8A8_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC1_RGBA_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC1_RGBA_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC1_RGB_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC1_RGB_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC2_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC2_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC3_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC3_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC4_SNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC4_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC5_SNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC5_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC6H_SFLOAT_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC6H_UFLOAT_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC7_SRGB_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_BC7_UNORM_BLOCK,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_D16_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_D32_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_E5B9G9R9_UFLOAT_PACK32,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16B16A16_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16B16A16_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16B16A16_SNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16B16A16_SSCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16B16A16_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16B16A16_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16B16A16_USCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16_SNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16_SSCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16G16_USCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16_SNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16_SSCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R16_USCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32B32A32_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32B32A32_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32B32A32_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32B32_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32B32_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32B32_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32G32_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32_SFLOAT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R32_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8B8A8_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8B8A8_SNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8B8A8_SRGB,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8B8A8_SSCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8B8A8_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8B8A8_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8B8A8_USCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8_SNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8_SSCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8G8_USCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8_SINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8_SNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8_SSCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8_UINT,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8_UNORM,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                    s->formatProperties.linearTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                    s->formatProperties.optimalTilingFeatures = (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT | VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.linearTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                    ret = ret && (vpCheckFlags(s->formatProperties.optimalTilingFeatures, (VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT | VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT | VK_FORMAT_FEATURE_TRANSFER_SRC_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
    {
        VK_FORMAT_R8_USCALED,
        [](VkBaseOutStructure* p) {
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    s->formatProperties.bufferFeatures = (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT);
                } break;
                default: break;
            }
        },
        [](VkBaseOutStructure* p) -> bool {
            bool ret = true;
            switch (p->sType) {
                case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR: {
                    VkFormatProperties2KHR* s = static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p));
                    ret = ret && (vpCheckFlags(s->formatProperties.bufferFeatures, (VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)));
                } break;
                default: break;
            }
            return ret;
        }
    },
};

static const VpStructChainerDesc chainerDesc = {
    [](VkBaseOutStructure* p, void* pUser, PFN_vpStructChainerCb pfnCb) {
        VkPhysicalDevice16BitStorageFeaturesKHR physicalDevice16BitStorageFeaturesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES_KHR, nullptr };
        VkPhysicalDevice8BitStorageFeaturesKHR physicalDevice8BitStorageFeaturesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES_KHR, &physicalDevice16BitStorageFeaturesKHR };
        VkPhysicalDeviceDescriptorIndexingFeaturesEXT physicalDeviceDescriptorIndexingFeaturesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT, &physicalDevice8BitStorageFeaturesKHR };
        VkPhysicalDeviceFloat16Int8FeaturesKHR physicalDeviceFloat16Int8FeaturesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT16_INT8_FEATURES_KHR, &physicalDeviceDescriptorIndexingFeaturesEXT };
        VkPhysicalDeviceHostQueryResetFeaturesEXT physicalDeviceHostQueryResetFeaturesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES_EXT, &physicalDeviceFloat16Int8FeaturesKHR };
        VkPhysicalDeviceInlineUniformBlockFeaturesEXT physicalDeviceInlineUniformBlockFeaturesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES_EXT, &physicalDeviceHostQueryResetFeaturesEXT };
        VkPhysicalDeviceMultiviewFeaturesKHR physicalDeviceMultiviewFeaturesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES_KHR, &physicalDeviceInlineUniformBlockFeaturesEXT };
        VkPhysicalDevicePortabilitySubsetFeaturesKHR physicalDevicePortabilitySubsetFeaturesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_FEATURES_KHR, &physicalDeviceMultiviewFeaturesKHR };
        VkPhysicalDeviceRobustness2FeaturesEXT physicalDeviceRobustness2FeaturesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT, &physicalDevicePortabilitySubsetFeaturesKHR };
        VkPhysicalDeviceScalarBlockLayoutFeaturesEXT physicalDeviceScalarBlockLayoutFeaturesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES_EXT, &physicalDeviceRobustness2FeaturesEXT };
        VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT physicalDeviceTexelBufferAlignmentFeaturesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_FEATURES_EXT, &physicalDeviceScalarBlockLayoutFeaturesEXT };
        VkPhysicalDeviceTimelineSemaphoreFeaturesKHR physicalDeviceTimelineSemaphoreFeaturesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES_KHR, &physicalDeviceTexelBufferAlignmentFeaturesEXT };
        VkPhysicalDeviceUniformBufferStandardLayoutFeaturesKHR physicalDeviceUniformBufferStandardLayoutFeaturesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES_KHR, &physicalDeviceTimelineSemaphoreFeaturesKHR };
        VkPhysicalDeviceVariablePointersFeaturesKHR physicalDeviceVariablePointersFeaturesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTERS_FEATURES, &physicalDeviceUniformBufferStandardLayoutFeaturesKHR };
        VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT physicalDeviceVertexAttributeDivisorFeaturesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_FEATURES_EXT, &physicalDeviceVariablePointersFeaturesKHR };
        p->pNext = static_cast<VkBaseOutStructure*>(static_cast<void*>(&physicalDeviceVertexAttributeDivisorFeaturesEXT));
        pfnCb(p, pUser);
    },
    [](VkBaseOutStructure* p, void* pUser, PFN_vpStructChainerCb pfnCb) {
        VkPhysicalDeviceDepthStencilResolvePropertiesKHR physicalDeviceDepthStencilResolvePropertiesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES_KHR, nullptr };
        VkPhysicalDeviceDescriptorIndexingPropertiesEXT physicalDeviceDescriptorIndexingPropertiesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES_EXT, &physicalDeviceDepthStencilResolvePropertiesKHR };
        VkPhysicalDeviceInlineUniformBlockPropertiesEXT physicalDeviceInlineUniformBlockPropertiesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_PROPERTIES_EXT, &physicalDeviceDescriptorIndexingPropertiesEXT };
        VkPhysicalDeviceMaintenance3PropertiesKHR physicalDeviceMaintenance3PropertiesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES_KHR, &physicalDeviceInlineUniformBlockPropertiesEXT };
        VkPhysicalDeviceMultiviewPropertiesKHR physicalDeviceMultiviewPropertiesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES_KHR, &physicalDeviceMaintenance3PropertiesKHR };
        VkPhysicalDevicePortabilitySubsetPropertiesKHR physicalDevicePortabilitySubsetPropertiesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_PROPERTIES_KHR, &physicalDeviceMultiviewPropertiesKHR };
        VkPhysicalDeviceRobustness2PropertiesEXT physicalDeviceRobustness2PropertiesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_PROPERTIES_EXT, &physicalDevicePortabilitySubsetPropertiesKHR };
        VkPhysicalDeviceSubgroupSizeControlPropertiesEXT physicalDeviceSubgroupSizeControlPropertiesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_PROPERTIES_EXT, &physicalDeviceRobustness2PropertiesEXT };
        VkPhysicalDeviceTexelBufferAlignmentPropertiesEXT physicalDeviceTexelBufferAlignmentPropertiesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_PROPERTIES_EXT, &physicalDeviceSubgroupSizeControlPropertiesEXT };
        VkPhysicalDeviceTimelineSemaphorePropertiesKHR physicalDeviceTimelineSemaphorePropertiesKHR{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_PROPERTIES_KHR, &physicalDeviceTexelBufferAlignmentPropertiesEXT };
        VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT physicalDeviceVertexAttributeDivisorPropertiesEXT{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_PROPERTIES_EXT, &physicalDeviceTimelineSemaphorePropertiesKHR };
        p->pNext = static_cast<VkBaseOutStructure*>(static_cast<void*>(&physicalDeviceVertexAttributeDivisorPropertiesEXT));
        pfnCb(p, pUser);
    },
    [](VkBaseOutStructure* p, void* pUser, PFN_vpStructChainerCb pfnCb) {
        pfnCb(p, pUser);
    },
    [](VkBaseOutStructure* p, void* pUser, PFN_vpStructChainerCb pfnCb) {
        VkFormatProperties3KHR formatProperties3KHR{ VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_3_KHR, nullptr };
        p->pNext = static_cast<VkBaseOutStructure*>(static_cast<void*>(&formatProperties3KHR));
        pfnCb(p, pUser);
    },
};

} // namespace VP_LUNARG_DESKTOP_PORTABILITY_2022
#endif

static const VpProfileDesc vpProfiles[] = {
#ifdef VP_ANDROID_baseline_2021
    VpProfileDesc{
        VpProfileProperties{ VP_ANDROID_BASELINE_2021_NAME, VP_ANDROID_BASELINE_2021_SPEC_VERSION },
        VP_ANDROID_BASELINE_2021_MIN_API_VERSION,
        &VP_ANDROID_BASELINE_2021::instanceExtensions[0], static_cast<uint32_t>(sizeof(VP_ANDROID_BASELINE_2021::instanceExtensions) / sizeof(VP_ANDROID_BASELINE_2021::instanceExtensions[0])),
        &VP_ANDROID_BASELINE_2021::deviceExtensions[0], static_cast<uint32_t>(sizeof(VP_ANDROID_BASELINE_2021::deviceExtensions) / sizeof(VP_ANDROID_BASELINE_2021::deviceExtensions[0])),
        nullptr, 0,
        &VP_ANDROID_BASELINE_2021::featureStructTypes[0], static_cast<uint32_t>(sizeof(VP_ANDROID_BASELINE_2021::featureStructTypes) / sizeof(VP_ANDROID_BASELINE_2021::featureStructTypes[0])),
        VP_ANDROID_BASELINE_2021::featureDesc,
        &VP_ANDROID_BASELINE_2021::propertyStructTypes[0], static_cast<uint32_t>(sizeof(VP_ANDROID_BASELINE_2021::propertyStructTypes) / sizeof(VP_ANDROID_BASELINE_2021::propertyStructTypes[0])),
        VP_ANDROID_BASELINE_2021::propertyDesc,
        nullptr, 0,
        nullptr, 0,
        &VP_ANDROID_BASELINE_2021::formatStructTypes[0], static_cast<uint32_t>(sizeof(VP_ANDROID_BASELINE_2021::formatStructTypes) / sizeof(VP_ANDROID_BASELINE_2021::formatStructTypes[0])),
        &VP_ANDROID_BASELINE_2021::formatDesc[0], static_cast<uint32_t>(sizeof(VP_ANDROID_BASELINE_2021::formatDesc) / sizeof(VP_ANDROID_BASELINE_2021::formatDesc[0])),
        VP_ANDROID_BASELINE_2021::chainerDesc,
    },
#endif
#ifdef VP_KHR_roadmap_2022
    VpProfileDesc{
        VpProfileProperties{ VP_KHR_ROADMAP_2022_NAME, VP_KHR_ROADMAP_2022_SPEC_VERSION },
        VP_KHR_ROADMAP_2022_MIN_API_VERSION,
        nullptr, 0,
        &VP_KHR_ROADMAP_2022::deviceExtensions[0], static_cast<uint32_t>(sizeof(VP_KHR_ROADMAP_2022::deviceExtensions) / sizeof(VP_KHR_ROADMAP_2022::deviceExtensions[0])),
        nullptr, 0,
        &VP_KHR_ROADMAP_2022::featureStructTypes[0], static_cast<uint32_t>(sizeof(VP_KHR_ROADMAP_2022::featureStructTypes) / sizeof(VP_KHR_ROADMAP_2022::featureStructTypes[0])),
        VP_KHR_ROADMAP_2022::featureDesc,
        &VP_KHR_ROADMAP_2022::propertyStructTypes[0], static_cast<uint32_t>(sizeof(VP_KHR_ROADMAP_2022::propertyStructTypes) / sizeof(VP_KHR_ROADMAP_2022::propertyStructTypes[0])),
        VP_KHR_ROADMAP_2022::propertyDesc,
        nullptr, 0,
        nullptr, 0,
        nullptr, 0,
        nullptr, 0,
        VP_KHR_ROADMAP_2022::chainerDesc,
    },
#endif
#ifdef VP_LUNARG_desktop_baseline_2022
    VpProfileDesc{
        VpProfileProperties{ VP_LUNARG_DESKTOP_BASELINE_2022_NAME, VP_LUNARG_DESKTOP_BASELINE_2022_SPEC_VERSION },
        VP_LUNARG_DESKTOP_BASELINE_2022_MIN_API_VERSION,
        nullptr, 0,
        &VP_LUNARG_DESKTOP_BASELINE_2022::deviceExtensions[0], static_cast<uint32_t>(sizeof(VP_LUNARG_DESKTOP_BASELINE_2022::deviceExtensions) / sizeof(VP_LUNARG_DESKTOP_BASELINE_2022::deviceExtensions[0])),
        nullptr, 0,
        &VP_LUNARG_DESKTOP_BASELINE_2022::featureStructTypes[0], static_cast<uint32_t>(sizeof(VP_LUNARG_DESKTOP_BASELINE_2022::featureStructTypes) / sizeof(VP_LUNARG_DESKTOP_BASELINE_2022::featureStructTypes[0])),
        VP_LUNARG_DESKTOP_BASELINE_2022::featureDesc,
        &VP_LUNARG_DESKTOP_BASELINE_2022::propertyStructTypes[0], static_cast<uint32_t>(sizeof(VP_LUNARG_DESKTOP_BASELINE_2022::propertyStructTypes) / sizeof(VP_LUNARG_DESKTOP_BASELINE_2022::propertyStructTypes[0])),
        VP_LUNARG_DESKTOP_BASELINE_2022::propertyDesc,
        nullptr, 0,
        nullptr, 0,
        &VP_LUNARG_DESKTOP_BASELINE_2022::formatStructTypes[0], static_cast<uint32_t>(sizeof(VP_LUNARG_DESKTOP_BASELINE_2022::formatStructTypes) / sizeof(VP_LUNARG_DESKTOP_BASELINE_2022::formatStructTypes[0])),
        &VP_LUNARG_DESKTOP_BASELINE_2022::formatDesc[0], static_cast<uint32_t>(sizeof(VP_LUNARG_DESKTOP_BASELINE_2022::formatDesc) / sizeof(VP_LUNARG_DESKTOP_BASELINE_2022::formatDesc[0])),
        VP_LUNARG_DESKTOP_BASELINE_2022::chainerDesc,
    },
#endif
#ifdef VP_LUNARG_desktop_portability_2022
    VpProfileDesc{
        VpProfileProperties{ VP_LUNARG_DESKTOP_PORTABILITY_2022_NAME, VP_LUNARG_DESKTOP_PORTABILITY_2022_SPEC_VERSION },
        VP_LUNARG_DESKTOP_PORTABILITY_2022_MIN_API_VERSION,
        &VP_LUNARG_DESKTOP_PORTABILITY_2022::instanceExtensions[0], static_cast<uint32_t>(sizeof(VP_LUNARG_DESKTOP_PORTABILITY_2022::instanceExtensions) / sizeof(VP_LUNARG_DESKTOP_PORTABILITY_2022::instanceExtensions[0])),
        &VP_LUNARG_DESKTOP_PORTABILITY_2022::deviceExtensions[0], static_cast<uint32_t>(sizeof(VP_LUNARG_DESKTOP_PORTABILITY_2022::deviceExtensions) / sizeof(VP_LUNARG_DESKTOP_PORTABILITY_2022::deviceExtensions[0])),
        nullptr, 0,
        &VP_LUNARG_DESKTOP_PORTABILITY_2022::featureStructTypes[0], static_cast<uint32_t>(sizeof(VP_LUNARG_DESKTOP_PORTABILITY_2022::featureStructTypes) / sizeof(VP_LUNARG_DESKTOP_PORTABILITY_2022::featureStructTypes[0])),
        VP_LUNARG_DESKTOP_PORTABILITY_2022::featureDesc,
        &VP_LUNARG_DESKTOP_PORTABILITY_2022::propertyStructTypes[0], static_cast<uint32_t>(sizeof(VP_LUNARG_DESKTOP_PORTABILITY_2022::propertyStructTypes) / sizeof(VP_LUNARG_DESKTOP_PORTABILITY_2022::propertyStructTypes[0])),
        VP_LUNARG_DESKTOP_PORTABILITY_2022::propertyDesc,
        nullptr, 0,
        nullptr, 0,
        &VP_LUNARG_DESKTOP_PORTABILITY_2022::formatStructTypes[0], static_cast<uint32_t>(sizeof(VP_LUNARG_DESKTOP_PORTABILITY_2022::formatStructTypes) / sizeof(VP_LUNARG_DESKTOP_PORTABILITY_2022::formatStructTypes[0])),
        &VP_LUNARG_DESKTOP_PORTABILITY_2022::formatDesc[0], static_cast<uint32_t>(sizeof(VP_LUNARG_DESKTOP_PORTABILITY_2022::formatDesc) / sizeof(VP_LUNARG_DESKTOP_PORTABILITY_2022::formatDesc[0])),
        VP_LUNARG_DESKTOP_PORTABILITY_2022::chainerDesc,
    },
#endif
};
static const uint32_t vpProfileCount = static_cast<uint32_t>(sizeof(vpProfiles) / sizeof(vpProfiles[0]));

VPAPI_ATTR const VpProfileDesc* vpGetProfileDesc(const char profileName[VP_MAX_PROFILE_NAME_SIZE]) {
    for (uint32_t i = 0; i < vpProfileCount; ++i) {
        if (strncmp(vpProfiles[i].props.profileName, profileName, VP_MAX_PROFILE_NAME_SIZE) == 0) return &vpProfiles[i];
    }
    return nullptr;
}

VPAPI_ATTR bool vpCheckVersion(uint32_t actual, uint32_t expected) {
    uint32_t actualMajor = VK_API_VERSION_MAJOR(actual);
    uint32_t actualMinor = VK_API_VERSION_MINOR(actual);
    uint32_t expectedMajor = VK_API_VERSION_MAJOR(expected);
    uint32_t expectedMinor = VK_API_VERSION_MINOR(expected);
    return actualMajor > expectedMajor || (actualMajor == expectedMajor && actualMinor >= expectedMinor);
}

VPAPI_ATTR bool vpCheckExtension(const VkExtensionProperties *supportedProperties, size_t supportedSize,
                                 const char *requestedExtension) {
    bool found = false;
    for (size_t i = 0, n = supportedSize; i < n; ++i) {
        if (strcmp(supportedProperties[i].extensionName, requestedExtension) == 0) {
            found = true;
            // Drivers don't actually update their spec version, so we cannot rely on this
            // if (supportedProperties[i].specVersion >= expectedVersion) found = true;
        }
    }
    return found;
}

VPAPI_ATTR void vpGetExtensions(uint32_t requestedExtensionCount, const char *const *ppRequestedExtensionNames,
                                uint32_t profileExtensionCount, const VkExtensionProperties *pProfileExtensionProperties,
                                std::vector<const char *> &extensions, bool merge, bool override) {
    if (override) {
        for (uint32_t i = 0; i < requestedExtensionCount; ++i) {
            extensions.push_back(ppRequestedExtensionNames[i]);
        }
    } else {
        for (uint32_t i = 0; i < profileExtensionCount; ++i) {
            extensions.push_back(pProfileExtensionProperties[i].extensionName);
        }

        if (merge) {
            for (uint32_t i = 0; i < requestedExtensionCount; ++i) {
                if (vpCheckExtension(pProfileExtensionProperties, profileExtensionCount, ppRequestedExtensionNames[i])) {
                    continue;
                }
                extensions.push_back(ppRequestedExtensionNames[i]);
            }
        }
    }
}

VPAPI_ATTR const void* vpGetStructure(const void* pNext, VkStructureType type) {
    const VkBaseOutStructure *p = static_cast<const VkBaseOutStructure*>(pNext);
    while (p != nullptr) {
        if (p->sType == type) return p;
        p = p->pNext;
    }
    return nullptr;
}

VPAPI_ATTR void* vpGetStructure(void* pNext, VkStructureType type) {
    VkBaseOutStructure *p = static_cast<VkBaseOutStructure*>(pNext);
    while (p != nullptr) {
        if (p->sType == type) return p;
        p = p->pNext;
    }
    return nullptr;
}

} // namespace detail

VPAPI_ATTR VkResult vpGetProfiles(uint32_t *pPropertyCount, VpProfileProperties *pProperties) {
    VkResult result = VK_SUCCESS;

    if (pProperties == nullptr) {
        *pPropertyCount = detail::vpProfileCount;
    } else {
        if (*pPropertyCount < detail::vpProfileCount) {
            result = VK_INCOMPLETE;
        } else {
            *pPropertyCount = detail::vpProfileCount;
        }
        for (uint32_t i = 0; i < *pPropertyCount; ++i) {
            pProperties[i] = detail::vpProfiles[i].props;
        }
    }
    return result;
}

VPAPI_ATTR VkResult vpGetProfileFallbacks(const VpProfileProperties *pProfile, uint32_t *pPropertyCount, VpProfileProperties *pProperties) {
    VkResult result = VK_SUCCESS;

    const detail::VpProfileDesc* pDesc = detail::vpGetProfileDesc(pProfile->profileName);
    if (pDesc == nullptr) return VK_ERROR_UNKNOWN;

    if (pProperties == nullptr) {
        *pPropertyCount = pDesc->fallbackCount;
    } else {
        if (*pPropertyCount < pDesc->fallbackCount) {
            result = VK_INCOMPLETE;
        } else {
            *pPropertyCount = pDesc->fallbackCount;
        }
        for (uint32_t i = 0; i < *pPropertyCount; ++i) {
            pProperties[i] = pDesc->pFallbacks[i];
        }
    }
    return result;
}

VPAPI_ATTR VkResult vpGetInstanceProfileSupport(const char *pLayerName, const VpProfileProperties *pProfile, VkBool32 *pSupported) {
    VkResult result = VK_SUCCESS;

    uint32_t apiVersion = VK_MAKE_VERSION(1, 0, 0);
    static PFN_vkEnumerateInstanceVersion pfnEnumerateInstanceVersion =
        (PFN_vkEnumerateInstanceVersion)vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceVersion");
    if (pfnEnumerateInstanceVersion != nullptr) {
        result = pfnEnumerateInstanceVersion(&apiVersion);
        if (result != VK_SUCCESS) {
            return result;
        }
    }

    uint32_t extCount = 0;
    result = vkEnumerateInstanceExtensionProperties(pLayerName, &extCount, nullptr);
    if (result != VK_SUCCESS) {
        return result;
    }
    std::vector<VkExtensionProperties> ext(extCount);
    result = vkEnumerateInstanceExtensionProperties(pLayerName, &extCount, ext.data());
    if (result != VK_SUCCESS) {
        return result;
    }

    const detail::VpProfileDesc* pDesc = detail::vpGetProfileDesc(pProfile->profileName);
    if (pDesc == nullptr) return VK_ERROR_UNKNOWN;

    *pSupported = VK_TRUE;

    if (pDesc->props.specVersion < pProfile->specVersion) {
        *pSupported = VK_FALSE;
    }

    if (!detail::vpCheckVersion(apiVersion, pDesc->minApiVersion)) {
        *pSupported = VK_FALSE;
    }

    for (uint32_t i = 0; i < pDesc->instanceExtensionCount; ++i) {
        if (!detail::vpCheckExtension(ext.data(), ext.size(),
            pDesc->pInstanceExtensions[i].extensionName)) {
            *pSupported = VK_FALSE;
        }
    }

    // We require VK_KHR_get_physical_device_properties2 if we are on Vulkan 1.0
    if (apiVersion < VK_API_VERSION_1_1) {
        bool foundGPDP2 = false;
        for (size_t i = 0; i < ext.size(); ++i) {
            if (strcmp(ext[i].extensionName, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME) == 0) {
                foundGPDP2 = true;
                break;
            }
        }
        if (!foundGPDP2) {
            *pSupported = VK_FALSE;
        }
    }

    return result;
}

VPAPI_ATTR VkResult vpCreateInstance(const VpInstanceCreateInfo *pCreateInfo,
                                     const VkAllocationCallbacks *pAllocator, VkInstance *pInstance) {
    VkInstanceCreateInfo createInfo{ VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
    VkApplicationInfo appInfo{ VK_STRUCTURE_TYPE_APPLICATION_INFO };
    std::vector<const char*> extensions;
    VkInstanceCreateInfo* pInstanceCreateInfo = nullptr;

    if (pCreateInfo != nullptr && pCreateInfo->pCreateInfo != nullptr) {
        createInfo = *pCreateInfo->pCreateInfo;
        pInstanceCreateInfo = &createInfo;

        const detail::VpProfileDesc* pDesc = nullptr;
        if (pCreateInfo->pProfile != nullptr) {
            pDesc = detail::vpGetProfileDesc(pCreateInfo->pProfile->profileName);
            if (pDesc == nullptr) return VK_ERROR_UNKNOWN;
        }

        if (createInfo.pApplicationInfo == nullptr) {
            appInfo.apiVersion = pDesc->minApiVersion;
            createInfo.pApplicationInfo = &appInfo;
        }

        if (pDesc != nullptr && pDesc->pInstanceExtensions != nullptr) {
            bool merge = (pCreateInfo->flags & VP_INSTANCE_CREATE_MERGE_EXTENSIONS_BIT) != 0;
            bool override = (pCreateInfo->flags & VP_INSTANCE_CREATE_OVERRIDE_EXTENSIONS_BIT) != 0;

            if (!merge && !override && pCreateInfo->pCreateInfo->enabledExtensionCount > 0) {
                // If neither merge nor override is used then the application must not specify its
                // own extensions
                return VK_ERROR_UNKNOWN;
            }

            detail::vpGetExtensions(pCreateInfo->pCreateInfo->enabledExtensionCount,
                                    pCreateInfo->pCreateInfo->ppEnabledExtensionNames,
                                    pDesc->instanceExtensionCount,
                                    pDesc->pInstanceExtensions,
                                    extensions, merge, override);
            {
                bool foundPortEnum = false;
                for (size_t i = 0; i < extensions.size(); ++i) {
                    if (strcmp(extensions[i], VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME) == 0) {
                        foundPortEnum = true;
                        break;
                    }
                }
                if (foundPortEnum) {
                    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
                }
            }

            // Need to include VK_KHR_get_physical_device_properties2 if we are on Vulkan 1.0
            if (createInfo.pApplicationInfo->apiVersion < VK_API_VERSION_1_1) {
                bool foundGPDP2 = false;
                for (size_t i = 0; i < extensions.size(); ++i) {
                    if (strcmp(extensions[i], VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME) == 0) {
                        foundGPDP2 = true;
                        break;
                    }
                }
                if (!foundGPDP2) {
                    extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
                }
            }

            createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
            createInfo.ppEnabledExtensionNames = extensions.data();
        }
    }

    return vkCreateInstance(pInstanceCreateInfo, pAllocator, pInstance);
}

VPAPI_ATTR VkResult vpGetPhysicalDeviceProfileSupport(VkInstance instance, VkPhysicalDevice physicalDevice,
                                                      const VpProfileProperties *pProfile, VkBool32 *pSupported) {
    VkResult result = VK_SUCCESS;

    uint32_t extCount = 0;
    result = vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount, nullptr);
    if (result != VK_SUCCESS) {
        return result;
    }
    std::vector<VkExtensionProperties> ext;
    if (extCount > 0) {
        ext.resize(extCount);
    }
    result = vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount, ext.data());
    if (result != VK_SUCCESS) {
        return result;
    }

    // Workaround old loader bug where count could be smaller on the second call to vkEnumerateDeviceExtensionProperties
    if (extCount > 0) {
        ext.resize(extCount);
    }

    const detail::VpProfileDesc* pDesc = detail::vpGetProfileDesc(pProfile->profileName);
    if (pDesc == nullptr) return VK_ERROR_UNKNOWN;

    struct GPDP2EntryPoints {
        PFN_vkGetPhysicalDeviceFeatures2KHR                 pfnGetPhysicalDeviceFeatures2;
        PFN_vkGetPhysicalDeviceProperties2KHR               pfnGetPhysicalDeviceProperties2;
        PFN_vkGetPhysicalDeviceFormatProperties2KHR         pfnGetPhysicalDeviceFormatProperties2;
        PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR    pfnGetPhysicalDeviceQueueFamilyProperties2;
    };

    struct UserData {
        VkPhysicalDevice                    physicalDevice;
        const detail::VpProfileDesc*        pDesc;
        GPDP2EntryPoints                    gpdp2;
        uint32_t                            index;
        uint32_t                            count;
        detail::PFN_vpStructChainerCb       pfnCb;
        bool                                supported;
    } userData{ physicalDevice, pDesc };

    // Attempt to load core versions of the GPDP2 entry points
    userData.gpdp2.pfnGetPhysicalDeviceFeatures2 =
        (PFN_vkGetPhysicalDeviceFeatures2KHR)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFeatures2");
    userData.gpdp2.pfnGetPhysicalDeviceProperties2 =
        (PFN_vkGetPhysicalDeviceProperties2KHR)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceProperties2");
    userData.gpdp2.pfnGetPhysicalDeviceFormatProperties2 =
        (PFN_vkGetPhysicalDeviceFormatProperties2KHR)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFormatProperties2");
    userData.gpdp2.pfnGetPhysicalDeviceQueueFamilyProperties2 =
        (PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyProperties2");

    // If not successful, try to load KHR variant
    if (userData.gpdp2.pfnGetPhysicalDeviceFeatures2 == nullptr) {
        userData.gpdp2.pfnGetPhysicalDeviceFeatures2 =
            (PFN_vkGetPhysicalDeviceFeatures2KHR)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFeatures2KHR");
        userData.gpdp2.pfnGetPhysicalDeviceProperties2 =
            (PFN_vkGetPhysicalDeviceProperties2KHR)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceProperties2KHR");
        userData.gpdp2.pfnGetPhysicalDeviceFormatProperties2 =
            (PFN_vkGetPhysicalDeviceFormatProperties2KHR)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFormatProperties2KHR");
        userData.gpdp2.pfnGetPhysicalDeviceQueueFamilyProperties2 =
            (PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR)vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyProperties2KHR");
    }

    if (userData.gpdp2.pfnGetPhysicalDeviceFeatures2 == nullptr ||
        userData.gpdp2.pfnGetPhysicalDeviceProperties2 == nullptr ||
        userData.gpdp2.pfnGetPhysicalDeviceFormatProperties2 == nullptr ||
        userData.gpdp2.pfnGetPhysicalDeviceQueueFamilyProperties2 == nullptr) {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    *pSupported = VK_TRUE;

    if (pDesc->props.specVersion < pProfile->specVersion) {
        *pSupported = VK_FALSE;
    }

    {
        VkPhysicalDeviceProperties props{};
        vkGetPhysicalDeviceProperties(physicalDevice, &props);
        if (!detail::vpCheckVersion(props.apiVersion, pDesc->minApiVersion)) {
            *pSupported = VK_FALSE;
        }
    }

    for (uint32_t i = 0; i < pDesc->deviceExtensionCount; ++i) {
        if (!detail::vpCheckExtension(ext.data(), ext.size(),
            pDesc->pDeviceExtensions[i].extensionName)) {
            *pSupported = VK_FALSE;
        }
    }

    {
        VkPhysicalDeviceFeatures2KHR features{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR };
        pDesc->chainers.pfnFeature(static_cast<VkBaseOutStructure*>(static_cast<void*>(&features)), &userData,
            [](VkBaseOutStructure* p, void* pUser) {
                UserData* pUserData = static_cast<UserData*>(pUser);
                pUserData->gpdp2.pfnGetPhysicalDeviceFeatures2(pUserData->physicalDevice,
                                                               static_cast<VkPhysicalDeviceFeatures2KHR*>(static_cast<void*>(p)));
                pUserData->supported = true;
                while (p != nullptr) {
                    if (!pUserData->pDesc->feature.pfnComparator(p)) {
                        pUserData->supported = false;
                    }
                    p = p->pNext;
                }
            }
        );
        if (!userData.supported) {
            *pSupported = VK_FALSE;
        }
    }

    {
        VkPhysicalDeviceProperties2KHR props{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR };
        pDesc->chainers.pfnProperty(static_cast<VkBaseOutStructure*>(static_cast<void*>(&props)), &userData,
            [](VkBaseOutStructure* p, void* pUser) {
                UserData* pUserData = static_cast<UserData*>(pUser);
                pUserData->gpdp2.pfnGetPhysicalDeviceProperties2(pUserData->physicalDevice,
                                                                 static_cast<VkPhysicalDeviceProperties2KHR*>(static_cast<void*>(p)));
                pUserData->supported = true;
                while (p != nullptr) {
                    if (!pUserData->pDesc->property.pfnComparator(p)) {
                        pUserData->supported = false;
                    }
                    p = p->pNext;
                }
            }
        );
        if (!userData.supported) {
            *pSupported = VK_FALSE;
        }
    }

    for (uint32_t i = 0; i < pDesc->formatCount; ++i) {
        userData.index = i;
        VkFormatProperties2KHR props{ VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR };
        pDesc->chainers.pfnFormat(static_cast<VkBaseOutStructure*>(static_cast<void*>(&props)), &userData,
            [](VkBaseOutStructure* p, void* pUser) {
                UserData* pUserData = static_cast<UserData*>(pUser);
                pUserData->gpdp2.pfnGetPhysicalDeviceFormatProperties2(pUserData->physicalDevice,
                                                                       pUserData->pDesc->pFormats[pUserData->index].format,
                                                                       static_cast<VkFormatProperties2KHR*>(static_cast<void*>(p)));
                pUserData->supported = true;
                while (p != nullptr) {
                    if (!pUserData->pDesc->pFormats[pUserData->index].pfnComparator(p)) {
                        pUserData->supported = false;
                    }
                    p = p->pNext;
                }
            }
        );
        if (!userData.supported) {
            *pSupported = VK_FALSE;
        }
    }

    {
        userData.gpdp2.pfnGetPhysicalDeviceQueueFamilyProperties2(physicalDevice, &userData.count, nullptr);
        std::vector<VkQueueFamilyProperties2KHR> props(userData.count, { VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2_KHR });
        userData.index = 0;

        detail::PFN_vpStructChainerCb callback = [](VkBaseOutStructure* p, void* pUser) {
            UserData* pUserData = static_cast<UserData*>(pUser);
            VkQueueFamilyProperties2KHR* pProps = static_cast<VkQueueFamilyProperties2KHR*>(static_cast<void*>(p));
            if (++pUserData->index < pUserData->count) {
                pUserData->pDesc->chainers.pfnQueueFamily(static_cast<VkBaseOutStructure*>(static_cast<void*>(++pProps)),
                                                          pUser, pUserData->pfnCb);
            } else {
                pProps -= pUserData->count - 1;
                pUserData->gpdp2.pfnGetPhysicalDeviceQueueFamilyProperties2(pUserData->physicalDevice,
                                                                            &pUserData->count,
                                                                            pProps);
                pUserData->supported = true;

                // Check first that each queue family defined is supported by the device
                for (uint32_t i = 0; i < pUserData->pDesc->queueFamilyCount; ++i) {
                    bool found = false;
                    for (uint32_t j = 0; j < pUserData->count; ++j) {
                        bool propsMatch = true;
                        p = static_cast<VkBaseOutStructure*>(static_cast<void*>(&pProps[j]));
                        while (p != nullptr) {
                            if (!pUserData->pDesc->pQueueFamilies[i].pfnComparator(p)) {
                                propsMatch = false;
                                break;
                            }
                            p = p->pNext;
                        }
                        if (propsMatch) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        pUserData->supported = false;
                        return;
                    }
                }

                // Then check each permutation to ensure that while order of the queue families
                // doesn't matter, each queue family property criteria is matched with a separate
                // queue family of the actual device
                std::vector<uint32_t> permutation(pUserData->count);
                for (uint32_t i = 0; i < pUserData->count; ++i) {
                    permutation[i] = i;
                }
                bool found = false;
                do {
                    bool propsMatch = true;
                    for (uint32_t i = 0; i < pUserData->pDesc->queueFamilyCount && propsMatch; ++i) {
                        p = static_cast<VkBaseOutStructure*>(static_cast<void*>(&pProps[permutation[i]]));
                        while (p != nullptr) {
                            if (!pUserData->pDesc->pQueueFamilies[i].pfnComparator(p)) {
                                propsMatch = false;
                                break;
                            }
                            p = p->pNext;
                        }
                    }
                    if (propsMatch) {
                        found = true;
                        break;
                    }
                } while (std::next_permutation(permutation.begin(), permutation.end()));

                if (!found) {
                    pUserData->supported = false;
                }
            }
        };
        userData.pfnCb = callback;

        if (userData.count >= userData.pDesc->queueFamilyCount) {
            pDesc->chainers.pfnQueueFamily(static_cast<VkBaseOutStructure*>(static_cast<void*>(props.data())), &userData, callback);
            if (!userData.supported) {
                *pSupported = VK_FALSE;
            }
        } else {
            *pSupported = VK_FALSE;
        }
    }

    return result;
}

VPAPI_ATTR VkResult vpCreateDevice(VkPhysicalDevice physicalDevice, const VpDeviceCreateInfo *pCreateInfo,
                                   const VkAllocationCallbacks *pAllocator, VkDevice *pDevice) {
    if (physicalDevice == VK_NULL_HANDLE || pCreateInfo == nullptr || pDevice == nullptr) {
        return vkCreateDevice(physicalDevice, pCreateInfo == nullptr ? nullptr : pCreateInfo->pCreateInfo, pAllocator, pDevice);
    }

    const detail::VpProfileDesc* pDesc = detail::vpGetProfileDesc(pCreateInfo->pProfile->profileName);
    if (pDesc == nullptr) return VK_ERROR_UNKNOWN;

    struct UserData {
        VkPhysicalDevice                physicalDevice;
        const detail::VpProfileDesc*    pDesc;
        const VpDeviceCreateInfo*       pCreateInfo;
        const VkAllocationCallbacks*    pAllocator;
        VkDevice*                       pDevice;
        VkResult                        result;
    } userData{ physicalDevice, pDesc, pCreateInfo, pAllocator, pDevice };

    VkPhysicalDeviceFeatures2KHR features{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR };
    pDesc->chainers.pfnFeature(static_cast<VkBaseOutStructure*>(static_cast<void*>(&features)), &userData,
        [](VkBaseOutStructure* p, void* pUser) {
            UserData* pUserData = static_cast<UserData*>(pUser);
            const detail::VpProfileDesc* pDesc = pUserData->pDesc;
            const VpDeviceCreateInfo* pCreateInfo = pUserData->pCreateInfo;

            bool merge = (pCreateInfo->flags & VP_DEVICE_CREATE_MERGE_EXTENSIONS_BIT) != 0;
            bool override = (pCreateInfo->flags & VP_DEVICE_CREATE_OVERRIDE_EXTENSIONS_BIT) != 0;

            if (!merge && !override && pCreateInfo->pCreateInfo->enabledExtensionCount > 0) {
                // If neither merge nor override is used then the application must not specify its
                // own extensions
                pUserData->result = VK_ERROR_UNKNOWN;
                return;
            }

            std::vector<const char*> extensions;
            detail::vpGetExtensions(pCreateInfo->pCreateInfo->enabledExtensionCount,
                                    pCreateInfo->pCreateInfo->ppEnabledExtensionNames,
                                    pDesc->deviceExtensionCount,
                                    pDesc->pDeviceExtensions,
                                    extensions, merge, override);

            VkBaseOutStructure profileStructList;
            profileStructList.pNext = p;
            VkPhysicalDeviceFeatures2KHR* pFeatures = static_cast<VkPhysicalDeviceFeatures2KHR*>(static_cast<void*>(p));
            if (pDesc->feature.pfnFiller != nullptr) {
                while (p != nullptr) {
                    pDesc->feature.pfnFiller(p);
                    p = p->pNext;
                }
            }

            if (pCreateInfo->pCreateInfo->pEnabledFeatures != nullptr) {
                pFeatures->features = *pCreateInfo->pCreateInfo->pEnabledFeatures;
            }

            if (pCreateInfo->flags & VP_DEVICE_CREATE_DISABLE_ROBUST_BUFFER_ACCESS_BIT) {
                pFeatures->features.robustBufferAccess = VK_FALSE;
            }

#ifdef VK_EXT_robustness2
            VkPhysicalDeviceRobustness2FeaturesEXT* pRobustness2FeaturesEXT = static_cast<VkPhysicalDeviceRobustness2FeaturesEXT*>(
                detail::vpGetStructure(pFeatures, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT));
            if (pRobustness2FeaturesEXT != nullptr) {
                if (pCreateInfo->flags & VP_DEVICE_CREATE_DISABLE_ROBUST_BUFFER_ACCESS_BIT) {
                    pRobustness2FeaturesEXT->robustBufferAccess2 = VK_FALSE;
                }
                if (pCreateInfo->flags & VP_DEVICE_CREATE_DISABLE_ROBUST_IMAGE_ACCESS_BIT) {
                    pRobustness2FeaturesEXT->robustImageAccess2 = VK_FALSE;
                }
            }
#endif

#ifdef VK_EXT_image_robustness
            VkPhysicalDeviceImageRobustnessFeaturesEXT* pImageRobustnessFeaturesEXT = static_cast<VkPhysicalDeviceImageRobustnessFeaturesEXT*>(
                detail::vpGetStructure(pFeatures, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_ROBUSTNESS_FEATURES_EXT));
            if (pImageRobustnessFeaturesEXT != nullptr && (pCreateInfo->flags & VP_DEVICE_CREATE_DISABLE_ROBUST_IMAGE_ACCESS_BIT)) {
                pImageRobustnessFeaturesEXT->robustImageAccess = VK_FALSE;
            }
#endif

#ifdef VK_VERSION_1_3
            VkPhysicalDeviceVulkan13Features* pVulkan13Features = static_cast<VkPhysicalDeviceVulkan13Features*>(
                detail::vpGetStructure(pFeatures, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES));
            if (pVulkan13Features != nullptr && (pCreateInfo->flags & VP_DEVICE_CREATE_DISABLE_ROBUST_IMAGE_ACCESS_BIT)) {
                pVulkan13Features->robustImageAccess = VK_FALSE;
            }
#endif

            VkBaseOutStructure* pNext = static_cast<VkBaseOutStructure*>(const_cast<void*>(pCreateInfo->pCreateInfo->pNext));
            if ((pCreateInfo->flags & VP_DEVICE_CREATE_OVERRIDE_ALL_FEATURES_BIT) == 0) {
                for (uint32_t i = 0; i < pDesc->featureStructTypeCount; ++i) {
                    const void* pRequested = detail::vpGetStructure(pNext, pDesc->pFeatureStructTypes[i]);
                    if (pRequested == nullptr) {
                        VkBaseOutStructure* pPrevStruct = &profileStructList;
                        VkBaseOutStructure* pCurrStruct = pPrevStruct->pNext;
                        while (pCurrStruct->sType != pDesc->pFeatureStructTypes[i]) {
                            pPrevStruct = pCurrStruct;
                            pCurrStruct = pCurrStruct->pNext;
                        }
                        pPrevStruct->pNext = pCurrStruct->pNext;
                        pCurrStruct->pNext = pNext;
                        pNext = pCurrStruct;
                    } else
                    if ((pCreateInfo->flags & VP_DEVICE_CREATE_OVERRIDE_FEATURES_BIT) == 0) {
                        // If override is not used then the application must not specify its
                        // own feature structure for anything that the profile defines
                        pUserData->result = VK_ERROR_UNKNOWN;
                        return;
                    }
                }
            }

            VkDeviceCreateInfo createInfo{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
            createInfo.pNext = pNext;
            createInfo.queueCreateInfoCount = pCreateInfo->pCreateInfo->queueCreateInfoCount;
            createInfo.pQueueCreateInfos = pCreateInfo->pCreateInfo->pQueueCreateInfos;
            createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
            createInfo.ppEnabledExtensionNames = extensions.data();
            if (pCreateInfo->flags & VP_DEVICE_CREATE_OVERRIDE_ALL_FEATURES_BIT) {
                createInfo.pEnabledFeatures = pCreateInfo->pCreateInfo->pEnabledFeatures;
            }
            pUserData->result = vkCreateDevice(pUserData->physicalDevice, &createInfo, pUserData->pAllocator, pUserData->pDevice);
        }
    );

    return userData.result;
}

VPAPI_ATTR VkResult vpGetProfileInstanceExtensionProperties(const VpProfileProperties *pProfile, uint32_t *pPropertyCount,
                                                            VkExtensionProperties *pProperties) {
    VkResult result = VK_SUCCESS;

    const detail::VpProfileDesc* pDesc = detail::vpGetProfileDesc(pProfile->profileName);
    if (pDesc == nullptr) return VK_ERROR_UNKNOWN;

    if (pProperties == nullptr) {
        *pPropertyCount = pDesc->instanceExtensionCount;
    } else {
        if (*pPropertyCount < pDesc->instanceExtensionCount) {
            result = VK_INCOMPLETE;
        } else {
            *pPropertyCount = pDesc->instanceExtensionCount;
        }
        for (uint32_t i = 0; i < *pPropertyCount; ++i) {
            pProperties[i] = pDesc->pInstanceExtensions[i];
        }
    }
    return result;
}

VPAPI_ATTR VkResult vpGetProfileDeviceExtensionProperties(const VpProfileProperties *pProfile, uint32_t *pPropertyCount,
                                                          VkExtensionProperties *pProperties) {
    VkResult result = VK_SUCCESS;

    const detail::VpProfileDesc* pDesc = detail::vpGetProfileDesc(pProfile->profileName);
    if (pDesc == nullptr) return VK_ERROR_UNKNOWN;

    if (pProperties == nullptr) {
        *pPropertyCount = pDesc->deviceExtensionCount;
    } else {
        if (*pPropertyCount < pDesc->deviceExtensionCount) {
            result = VK_INCOMPLETE;
        } else {
            *pPropertyCount = pDesc->deviceExtensionCount;
        }
        for (uint32_t i = 0; i < *pPropertyCount; ++i) {
            pProperties[i] = pDesc->pDeviceExtensions[i];
        }
    }
    return result;
}

VPAPI_ATTR void vpGetProfileFeatures(const VpProfileProperties *pProfile, void *pNext) {
    const detail::VpProfileDesc* pDesc = detail::vpGetProfileDesc(pProfile->profileName);
    if (pDesc != nullptr && pDesc->feature.pfnFiller != nullptr) {
        VkBaseOutStructure* p = static_cast<VkBaseOutStructure*>(pNext);
        while (p != nullptr) {
            pDesc->feature.pfnFiller(p);
            p = p->pNext;
        }
    }
}

VPAPI_ATTR VkResult vpGetProfileFeatureStructureTypes(const VpProfileProperties *pProfile, uint32_t *pStructureTypeCount,
                                                      VkStructureType *pStructureTypes) {
    VkResult result = VK_SUCCESS;

    const detail::VpProfileDesc* pDesc = detail::vpGetProfileDesc(pProfile->profileName);
    if (pDesc == nullptr) return VK_ERROR_UNKNOWN;

    if (pStructureTypes == nullptr) {
        *pStructureTypeCount = pDesc->featureStructTypeCount;
    } else {
        if (*pStructureTypeCount < pDesc->featureStructTypeCount) {
            result = VK_INCOMPLETE;
        } else {
            *pStructureTypeCount = pDesc->featureStructTypeCount;
        }
        for (uint32_t i = 0; i < *pStructureTypeCount; ++i) {
            pStructureTypes[i] = pDesc->pFeatureStructTypes[i];
        }
    }
    return result;
}

VPAPI_ATTR void vpGetProfileProperties(const VpProfileProperties *pProfile, void *pNext) {
    const detail::VpProfileDesc* pDesc = detail::vpGetProfileDesc(pProfile->profileName);
    if (pDesc != nullptr && pDesc->property.pfnFiller != nullptr) {
        VkBaseOutStructure* p = static_cast<VkBaseOutStructure*>(pNext);
        while (p != nullptr) {
            pDesc->property.pfnFiller(p);
            p = p->pNext;
        }
    }
}

VPAPI_ATTR VkResult vpGetProfilePropertyStructureTypes(const VpProfileProperties *pProfile, uint32_t *pStructureTypeCount,
                                                       VkStructureType *pStructureTypes) {
    VkResult result = VK_SUCCESS;

    const detail::VpProfileDesc* pDesc = detail::vpGetProfileDesc(pProfile->profileName);
    if (pDesc == nullptr) return VK_ERROR_UNKNOWN;

    if (pStructureTypes == nullptr) {
        *pStructureTypeCount = pDesc->propertyStructTypeCount;
    } else {
        if (*pStructureTypeCount < pDesc->propertyStructTypeCount) {
            result = VK_INCOMPLETE;
        } else {
            *pStructureTypeCount = pDesc->propertyStructTypeCount;
        }
        for (uint32_t i = 0; i < *pStructureTypeCount; ++i) {
            pStructureTypes[i] = pDesc->pPropertyStructTypes[i];
        }
    }
    return result;
}

VPAPI_ATTR VkResult vpGetProfileQueueFamilyProperties(const VpProfileProperties *pProfile, uint32_t *pPropertyCount,
                                                      VkQueueFamilyProperties2KHR *pProperties) {
    VkResult result = VK_SUCCESS;

    const detail::VpProfileDesc* pDesc = detail::vpGetProfileDesc(pProfile->profileName);
    if (pDesc == nullptr) return VK_ERROR_UNKNOWN;

    if (pProperties == nullptr) {
        *pPropertyCount = pDesc->queueFamilyCount;
    } else {
        if (*pPropertyCount < pDesc->queueFamilyCount) {
            result = VK_INCOMPLETE;
        } else {
            *pPropertyCount = pDesc->queueFamilyCount;
        }
        for (uint32_t i = 0; i < *pPropertyCount; ++i) {
            VkBaseOutStructure* p = static_cast<VkBaseOutStructure*>(static_cast<void*>(&pProperties[i]));
            while (p != nullptr) {
                pDesc->pQueueFamilies[i].pfnFiller(p);
                p = p->pNext;
            }
        }
    }
    return result;
}

VPAPI_ATTR VkResult vpGetProfileQueueFamilyStructureTypes(const VpProfileProperties *pProfile, uint32_t *pStructureTypeCount,
                                                          VkStructureType *pStructureTypes) {
    VkResult result = VK_SUCCESS;

    const detail::VpProfileDesc* pDesc = detail::vpGetProfileDesc(pProfile->profileName);
    if (pDesc == nullptr) return VK_ERROR_UNKNOWN;

    if (pStructureTypes == nullptr) {
        *pStructureTypeCount = pDesc->queueFamilyStructTypeCount;
    } else {
        if (*pStructureTypeCount < pDesc->queueFamilyStructTypeCount) {
            result = VK_INCOMPLETE;
        } else {
            *pStructureTypeCount = pDesc->queueFamilyStructTypeCount;
        }
        for (uint32_t i = 0; i < *pStructureTypeCount; ++i) {
            pStructureTypes[i] = pDesc->pQueueFamilyStructTypes[i];
        }
    }
    return result;
}

VPAPI_ATTR VkResult vpGetProfileFormats(const VpProfileProperties *pProfile, uint32_t *pFormatCount, VkFormat *pFormats) {
    VkResult result = VK_SUCCESS;

    const detail::VpProfileDesc* pDesc = detail::vpGetProfileDesc(pProfile->profileName);
    if (pDesc == nullptr) return VK_ERROR_UNKNOWN;

    if (pFormats == nullptr) {
        *pFormatCount = pDesc->formatCount;
    } else {
        if (*pFormatCount < pDesc->formatCount) {
            result = VK_INCOMPLETE;
        } else {
            *pFormatCount = pDesc->formatCount;
        }
        for (uint32_t i = 0; i < *pFormatCount; ++i) {
            pFormats[i] = pDesc->pFormats[i].format;
        }
    }
    return result;
}

VPAPI_ATTR void vpGetProfileFormatProperties(const VpProfileProperties *pProfile, VkFormat format, void *pNext) {
    const detail::VpProfileDesc* pDesc = detail::vpGetProfileDesc(pProfile->profileName);
    if (pDesc == nullptr) return;

    for (uint32_t i = 0; i < pDesc->formatCount; ++i) {
        if (pDesc->pFormats[i].format == format) {
            VkBaseOutStructure* p = static_cast<VkBaseOutStructure*>(static_cast<void*>(pNext));
            while (p != nullptr) {
                pDesc->pFormats[i].pfnFiller(p);
                p = p->pNext;
            }
#if defined(VK_VERSION_1_3) || defined(VK_KHR_format_feature_flags2)
            VkFormatProperties2KHR* fp2 = static_cast<VkFormatProperties2KHR*>(
                detail::vpGetStructure(pNext, VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR));
            VkFormatProperties3KHR* fp3 = static_cast<VkFormatProperties3KHR*>(
                detail::vpGetStructure(pNext, VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_3_KHR));
            if (fp3 != nullptr) {
                VkFormatProperties2KHR fp{ VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2_KHR };
                pDesc->pFormats[i].pfnFiller(static_cast<VkBaseOutStructure*>(static_cast<void*>(&fp)));
                fp3->linearTilingFeatures = static_cast<VkFormatFeatureFlags2KHR>(fp3->linearTilingFeatures | fp.formatProperties.linearTilingFeatures);
                fp3->optimalTilingFeatures = static_cast<VkFormatFeatureFlags2KHR>(fp3->optimalTilingFeatures | fp.formatProperties.optimalTilingFeatures);
                fp3->bufferFeatures = static_cast<VkFormatFeatureFlags2KHR>(fp3->bufferFeatures | fp.formatProperties.bufferFeatures);
            }
            if (fp2 != nullptr) {
                VkFormatProperties3KHR fp{ VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_3_KHR };
                pDesc->pFormats[i].pfnFiller(static_cast<VkBaseOutStructure*>(static_cast<void*>(&fp)));
                fp2->formatProperties.linearTilingFeatures = static_cast<VkFormatFeatureFlags>(fp2->formatProperties.linearTilingFeatures | fp.linearTilingFeatures);
                fp2->formatProperties.optimalTilingFeatures = static_cast<VkFormatFeatureFlags>(fp2->formatProperties.optimalTilingFeatures | fp.optimalTilingFeatures);
                fp2->formatProperties.bufferFeatures = static_cast<VkFormatFeatureFlags>(fp2->formatProperties.bufferFeatures | fp.bufferFeatures);
            }
#endif
        }
    }
}

VPAPI_ATTR VkResult vpGetProfileFormatStructureTypes(const VpProfileProperties *pProfile, uint32_t *pStructureTypeCount,
                                                     VkStructureType *pStructureTypes) {
    VkResult result = VK_SUCCESS;

    const detail::VpProfileDesc* pDesc = detail::vpGetProfileDesc(pProfile->profileName);
    if (pDesc == nullptr) return VK_ERROR_UNKNOWN;

    if (pStructureTypes == nullptr) {
        *pStructureTypeCount = pDesc->formatStructTypeCount;
    } else {
        if (*pStructureTypeCount < pDesc->formatStructTypeCount) {
            result = VK_INCOMPLETE;
        } else {
            *pStructureTypeCount = pDesc->formatStructTypeCount;
        }
        for (uint32_t i = 0; i < *pStructureTypeCount; ++i) {
            pStructureTypes[i] = pDesc->pFormatStructTypes[i];
        }
    }
    return result;
}

#endif // VULKAN_PROFILES_HPP_
