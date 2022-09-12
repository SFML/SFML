////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/Unix/Display.hpp>
#include <SFML/Window/Unix/VulkanImplX11.hpp>

#include <cstring>
#include <dlfcn.h>
#include <map>
#include <string>
#define VK_USE_PLATFORM_XLIB_KHR
#define VK_NO_PROTOTYPES
#include <vulkan.h>


namespace
{
struct VulkanLibraryWrapper
{
    VulkanLibraryWrapper() :
    library(nullptr),
    vkGetInstanceProcAddr(nullptr),
    vkEnumerateInstanceLayerProperties(nullptr),
    vkEnumerateInstanceExtensionProperties(nullptr)
    {
    }

    ~VulkanLibraryWrapper()
    {
        if (library)
            dlclose(library);
    }

    // Try to load the library and all the required entry points
    bool loadLibrary()
    {
        if (library)
            return true;

        library = dlopen("libvulkan.so.1", RTLD_LAZY);

        if (!library)
            return false;

        if (!loadEntryPoint(vkGetInstanceProcAddr, "vkGetInstanceProcAddr"))
        {
            dlclose(library);
            library = nullptr;
            return false;
        }

        if (!loadEntryPoint(vkEnumerateInstanceLayerProperties, "vkEnumerateInstanceLayerProperties"))
        {
            dlclose(library);
            library = nullptr;
            return false;
        }

        if (!loadEntryPoint(vkEnumerateInstanceExtensionProperties, "vkEnumerateInstanceExtensionProperties"))
        {
            dlclose(library);
            library = nullptr;
            return false;
        }

        return true;
    }

    template <typename T>
    bool loadEntryPoint(T& entryPoint, const char* name)
    {
        entryPoint = reinterpret_cast<T>(dlsym(library, name));

        return (entryPoint != nullptr);
    }

    void* library;

    PFN_vkGetInstanceProcAddr                  vkGetInstanceProcAddr;
    PFN_vkEnumerateInstanceLayerProperties     vkEnumerateInstanceLayerProperties;
    PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
};

VulkanLibraryWrapper wrapper;
} // namespace


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
bool VulkanImplX11::isAvailable(bool requireGraphics)
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

            wrapper.vkEnumerateInstanceExtensionProperties(0, &extensionCount, nullptr);

            extensionProperties.resize(extensionCount);

            wrapper.vkEnumerateInstanceExtensionProperties(0, &extensionCount, extensionProperties.data());

            // Check if the necessary extensions are available
            bool has_VK_KHR_surface          = false;
            bool has_VK_KHR_platform_surface = false;

            for (const VkExtensionProperties& properties : extensionProperties)
            {
                if (!std::strcmp(properties.extensionName, VK_KHR_SURFACE_EXTENSION_NAME))
                {
                    has_VK_KHR_surface = true;
                }
                else if (!std::strcmp(properties.extensionName, VK_KHR_XLIB_SURFACE_EXTENSION_NAME))
                {
                    has_VK_KHR_platform_surface = true;
                }
            }

            if (!has_VK_KHR_surface || !has_VK_KHR_platform_surface)
                graphicsAvailable = false;
        }
    }

    if (requireGraphics)
        return graphicsAvailable;

    return computeAvailable;
}


////////////////////////////////////////////////////////////
VulkanFunctionPointer VulkanImplX11::getFunction(const char* name)
{
    if (!isAvailable(false))
        return 0;

    return reinterpret_cast<VulkanFunctionPointer>(dlsym(wrapper.library, name));
}


////////////////////////////////////////////////////////////
const std::vector<const char*>& VulkanImplX11::getGraphicsRequiredInstanceExtensions()
{
    static std::vector<const char*> extensions;

    if (extensions.empty())
    {
        extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
        extensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
    }

    return extensions;
}


////////////////////////////////////////////////////////////
bool VulkanImplX11::createVulkanSurface(const VkInstance&            instance,
                                        WindowHandle                 windowHandle,
                                        VkSurfaceKHR&                surface,
                                        const VkAllocationCallbacks* allocator)
{
    if (!isAvailable())
        return false;

    // Make a copy of the instance handle since we get it passed as a reference
    VkInstance inst = instance;

    auto vkCreateXlibSurfaceKHR = reinterpret_cast<PFN_vkCreateXlibSurfaceKHR>(
        wrapper.vkGetInstanceProcAddr(inst, "vkCreateXlibSurfaceKHR"));

    if (!vkCreateXlibSurfaceKHR)
        return false;

    // Since the surface is basically attached to the window, the connection
    // to the X display will stay open even after we open and close it here
    VkXlibSurfaceCreateInfoKHR surfaceCreateInfo = VkXlibSurfaceCreateInfoKHR();
    surfaceCreateInfo.sType                      = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.dpy                        = OpenDisplay();
    surfaceCreateInfo.window                     = windowHandle;

    bool result = (vkCreateXlibSurfaceKHR(instance, &surfaceCreateInfo, allocator, &surface) == VK_SUCCESS);

    CloseDisplay(surfaceCreateInfo.dpy);

    return result;
}

} // namespace priv

} // namespace sf
