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
#include <SFML/Window/Vulkan.hpp>

#include <cassert>

#if defined(SFML_SYSTEM_WINDOWS)

#include <SFML/Window/VulkanImpl.hpp>

#elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD) || defined(SFML_SYSTEM_OPENBSD) || \
    defined(SFML_SYSTEM_NETBSD)

#if defined(SFML_USE_DRM)

#define SFML_VULKAN_IMPLEMENTATION_NOT_AVAILABLE

#else

#include <SFML/Window/VulkanImpl.hpp>

#endif

#else

#define SFML_VULKAN_IMPLEMENTATION_NOT_AVAILABLE

#endif


namespace sf::Vulkan
{
////////////////////////////////////////////////////////////
bool isAvailable([[maybe_unused]] bool requireGraphics)
{
#if defined(SFML_VULKAN_IMPLEMENTATION_NOT_AVAILABLE)

    return false;

#else

    return priv::VulkanImpl::isAvailable(requireGraphics);

#endif
}


////////////////////////////////////////////////////////////
VulkanFunctionPointer getFunction([[maybe_unused]] const char* name)
{
    assert(name && "Name cannot be a null pointer");

#if defined(SFML_VULKAN_IMPLEMENTATION_NOT_AVAILABLE)

    return nullptr;

#else

    return priv::VulkanImpl::getFunction(name);

#endif
}


////////////////////////////////////////////////////////////
const std::vector<const char*>& getGraphicsRequiredInstanceExtensions()
{
#if defined(SFML_VULKAN_IMPLEMENTATION_NOT_AVAILABLE)

    static const std::vector<const char*> empty;

    return empty;

#else

    return priv::VulkanImpl::getGraphicsRequiredInstanceExtensions();

#endif
}

} // namespace sf::Vulkan
