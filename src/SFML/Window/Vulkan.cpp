////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2021 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/Vulkan.hpp>

#if defined(SFML_SYSTEM_WINDOWS)

#include <SFML/Window/Win32/VulkanImplWin32.hpp>
using VulkanImplType = sf::priv::VulkanImplWin32;

#elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD) || defined(SFML_SYSTEM_OPENBSD) || defined(SFML_SYSTEM_NETBSD)

#include <SFML/Window/Unix/VulkanImplX11.hpp>
using VulkanImplType = sf::priv::VulkanImplX11;

#else

#define SFML_VULKAN_IMPLEMENTATION_NOT_AVAILABLE

#endif


namespace sf
{
////////////////////////////////////////////////////////////
bool Vulkan::isAvailable(bool requireGraphics)
{
#if defined(SFML_VULKAN_IMPLEMENTATION_NOT_AVAILABLE)

    (void) requireGraphics;
    return false;

#else

    return VulkanImplType::isAvailable(requireGraphics);

#endif
}


////////////////////////////////////////////////////////////
VulkanFunctionPointer Vulkan::getFunction(const char* name)
{
#if defined(SFML_VULKAN_IMPLEMENTATION_NOT_AVAILABLE)

    (void) name;
    return nullptr;

#else

    return VulkanImplType::getFunction(name);

#endif
}


////////////////////////////////////////////////////////////
const std::vector<const char*>& Vulkan::getGraphicsRequiredInstanceExtensions()
{
#if defined(SFML_VULKAN_IMPLEMENTATION_NOT_AVAILABLE)

    static const std::vector<const char*> empty;

    return empty;

#else

    return VulkanImplType::getGraphicsRequiredInstanceExtensions();

#endif
}

} // namespace sf
