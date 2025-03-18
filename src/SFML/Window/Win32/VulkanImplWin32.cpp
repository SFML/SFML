////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/VulkanImpl.hpp>

#include <SFML/System/Win32/WindowsHeader.hpp>

#include <string_view>

#include <cstdint>

#define VK_USE_PLATFORM_WIN32_KHR
#define VK_NO_PROTOTYPES
#include <vulkan.h>


namespace
{
struct VulkanLibraryWrapper
{
    ~VulkanLibraryWrapper()
    {
        if (library)
            FreeLibrary(library);
    }

    // Try to load the library and all the required entry points
    bool loadLibrary()
    {
        if (library)
            return true;

        library = LoadLibraryA("vulkan-1.dll");

        if (!library)
            return false;

        if (!loadEntryPoint(vkGetInstanceProcAddr, "vkGetInstanceProcAddr"))
        {
            FreeLibrary(library);
            library = nullptr;
            return false;
        }

        if (!loadEntryPoint(vkEnumerateInstanceLayerProperties, "vkEnumerateInstanceLayerProperties"))
        {
            FreeLibrary(library);
            library = nullptr;
            return false;
        }

        if (!loadEntryPoint(vkEnumerateInstanceExtensionProperties, "vkEnumerateInstanceExtensionProperties"))
        {
            FreeLibrary(library);
            library = nullptr;
            return false;
        }

        return true;
    }

    template <typename T>
    bool loadEntryPoint(T& entryPoint, const char* name)
    {
        entryPoint = reinterpret_cast<T>(reinterpret_cast<void*>(GetProcAddress(library, name)));

        return entryPoint != nullptr;
    }

    HMODULE library{};

    PFN_vkGetInstanceProcAddr                  vkGetInstanceProcAddr{};
    PFN_vkEnumerateInstanceLayerProperties     vkEnumerateInstanceLayerProperties{};
    PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties{};
};

VulkanLibraryWrapper wrapper;
} // namespace


namespace sf::priv
{
////////////////////////////////////////////////////////////
bool VulkanImpl::isAvailable(bool requireGraphics)
{
    static bool checked           = false;
    static bool computeAvailable  = false;
    static bool graphicsAvailable = false;

    if (!checked)
    {
        checked = true;

        // Check if the library is available
        computeAvailable = wrapper.loadLibrary();

        // To check for instance extensions we don't need to differentiate between graphics and compute
        graphicsAvailable = computeAvailable;

        if (graphicsAvailable)
        {
            // Retrieve the available instance extensions
            std::vector<VkExtensionProperties> extensionProperties;

            std::uint32_t extensionCount = 0;

            wrapper.vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

            extensionProperties.resize(extensionCount);

            wrapper.vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProperties.data());

            // Check if the necessary extensions are available
            bool hasVkKhrSurface         = false;
            bool hasVkKhrPlatformSurface = false;

            for (const VkExtensionProperties& properties : extensionProperties)
            {
                if (std::string_view(properties.extensionName) == VK_KHR_SURFACE_EXTENSION_NAME)
                {
                    hasVkKhrSurface = true;
                }
                else if (std::string_view(properties.extensionName) == VK_KHR_WIN32_SURFACE_EXTENSION_NAME)
                {
                    hasVkKhrPlatformSurface = true;
                }
            }

            if (!hasVkKhrSurface || !hasVkKhrPlatformSurface)
                graphicsAvailable = false;
        }
    }

    if (requireGraphics)
        return graphicsAvailable;

    return computeAvailable;
}


////////////////////////////////////////////////////////////
VulkanFunctionPointer VulkanImpl::getFunction(const char* name)
{
    if (!isAvailable(false))
        return nullptr;

    return reinterpret_cast<VulkanFunctionPointer>(GetProcAddress(wrapper.library, name));
}


////////////////////////////////////////////////////////////
const std::vector<const char*>& VulkanImpl::getGraphicsRequiredInstanceExtensions()
{
    static const std::vector<const char*> extensions{VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME};
    return extensions;
}


////////////////////////////////////////////////////////////
bool VulkanImpl::createVulkanSurface(const VkInstance&            instance,
                                     WindowHandle                 windowHandle,
                                     VkSurfaceKHR&                surface,
                                     const VkAllocationCallbacks* allocator)
{
    if (!isAvailable())
        return false;

    // Make a copy of the instance handle since we get it passed as a reference
    VkInstance inst = instance;

    auto vkCreateWin32SurfaceKHR = reinterpret_cast<PFN_vkCreateWin32SurfaceKHR>(
        wrapper.vkGetInstanceProcAddr(inst, "vkCreateWin32SurfaceKHR"));

    if (!vkCreateWin32SurfaceKHR)
        return false;

    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = VkWin32SurfaceCreateInfoKHR();
    surfaceCreateInfo.sType                       = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.hinstance                   = GetModuleHandleA(nullptr);
    surfaceCreateInfo.hwnd                        = windowHandle;

    return vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, allocator, &surface) == VK_SUCCESS;
}

} // namespace sf::priv
