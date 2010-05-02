////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Window/VideoMode.h>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Get the current desktop video mode
////////////////////////////////////////////////////////////
sfVideoMode sfVideoMode_GetDesktopMode(void)
{
    sf::VideoMode desktop = sf::VideoMode::GetDesktopMode();
    sfVideoMode ret;
    ret.Width        = desktop.Width;
    ret.Height       = desktop.Height;
    ret.BitsPerPixel = desktop.BitsPerPixel;

    return ret;
}


////////////////////////////////////////////////////////////
/// Get all the supported video modes for fullscreen mode.
/// Modes are sorted from best to worst.
////////////////////////////////////////////////////////////
const sfVideoMode* sfVideoMode_GetFullscreenModes(size_t* Count)
{
    static std::vector<sfVideoMode> modes;

    // Populate the array on first call
    if (modes.empty())
    {
        const std::vector<sf::VideoMode>& SFMLModes = sf::VideoMode::GetFullscreenModes();
        for (std::vector<sf::VideoMode>::const_iterator it = SFMLModes.begin(); it != SFMLModes.end(); ++it)
        {
            sfVideoMode mode;
            mode.Width        = it->Width;
            mode.Height       = it->Height;
            mode.BitsPerPixel = it->BitsPerPixel;
            modes.push_back(mode);
        }
    }

    if (Count)
        *Count = modes.size();

    return !modes.empty() ? &modes[0] : NULL;
}


////////////////////////////////////////////////////////////
/// Tell whether or not a video mode is supported
////////////////////////////////////////////////////////////
sfBool sfVideoMode_IsValid(sfVideoMode mode)
{
    sf::VideoMode videoMode(mode.Width, mode.Height, mode.BitsPerPixel);
    return videoMode.IsValid() ? sfTrue : sfFalse;
}
