////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2019 Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef SFML_VULKANIMPLX11_HPP
#define SFML_VULKANIMPLX11_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Vulkan.hpp>
#include <SFML/Window/WindowHandle.hpp>
#include <vector>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Linux (X11) implementation of Vulkan
///
////////////////////////////////////////////////////////////
class VulkanImplX11
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether or not the system supports Vulkan
    ///
    /// This function should always be called before using
    /// the Vulkan features. If it returns false, then
    /// any attempt to use Vulkan will fail.
    ///
    /// If only compute is required, set \a requireGraphics
    /// to false to skip checking for the extensions necessary
    /// for graphics rendering.
    ///
    /// \param requireGraphics
    ///
    /// \return True if Vulkan is supported, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool isAvailable(bool requireGraphics = true);

    ////////////////////////////////////////////////////////////
    /// \brief Get the address of a Vulkan function
    ///
    /// \param name Name of the function to get the address of
    ///
    /// \return Address of the Vulkan function, 0 on failure
    ///
    ////////////////////////////////////////////////////////////
    static VulkanFunctionPointer getFunction(const char* name);

    ////////////////////////////////////////////////////////////
    /// \brief Get Vulkan instance extensions required for graphics
    ///
    /// \return Vulkan instance extensions required for graphics
    ///
    ////////////////////////////////////////////////////////////
    static const std::vector<const char*>& getGraphicsRequiredInstanceExtensions();

    ////////////////////////////////////////////////////////////
    /// \brief Create a Vulkan rendering surface
    ///
    /// \param instance     Vulkan instance
    /// \param windowHandle Handle to the window to create the surface for
    /// \param surface      Created surface
    /// \param allocator    Allocator to use
    ///
    /// \return True if surface creation was successful, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool createVulkanSurface(const VkInstance& instance, WindowHandle windowHandle, VkSurfaceKHR& surface, const VkAllocationCallbacks* allocator);
};

} // namespace priv

} // namespace sf


#endif // SFML_VULKANIMPLX11_HPP
