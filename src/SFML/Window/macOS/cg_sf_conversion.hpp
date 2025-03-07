////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Marco Antognini (antognini.marco@gmail.com),
//                         Laurent Gomila (laurent@sfml-dev.org)
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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/VideoMode.hpp>

#include <ApplicationServices/ApplicationServices.h>

namespace sf::priv
{
////////////////////////////////////////////////////////////
/// \brief Get bpp of a video mode for OS 10.6 or later
///
/// With OS 10.6 and later, Quartz doesn't use dictionaries any more
/// to represent video mode. Instead it uses a CGDisplayMode opaque type.
///
////////////////////////////////////////////////////////////
unsigned int modeBitsPerPixel(CGDisplayModeRef mode);

////////////////////////////////////////////////////////////
/// \brief Convert a Quartz video mode into a sf::VideoMode object
///
////////////////////////////////////////////////////////////
VideoMode convertCGModeToSFMode(CGDisplayModeRef cgmode);

} // namespace sf::priv
