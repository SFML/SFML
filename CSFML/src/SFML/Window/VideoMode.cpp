////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2008-2008 Laurent Gomila (laurent.gom@gmail.com)
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
sfVideoMode sfVideoMode_GetDesktopMode()
{
    sf::VideoMode Desktop = sf::VideoMode::GetDesktopMode();
    sfVideoMode Ret;
    Ret.Width        = Desktop.Width;
    Ret.Height       = Desktop.Height;
    Ret.BitsPerPixel = Desktop.BitsPerPixel;

    return Ret;
}


////////////////////////////////////////////////////////////
/// Get a valid video mode
/// Index must be in range [0, GetModesCount()[
/// Modes are sorted from best to worst
////////////////////////////////////////////////////////////
sfVideoMode sfVideoMode_GetMode(size_t Index)
{
    sf::VideoMode Mode = sf::VideoMode::GetMode(Index);
    sfVideoMode Ret;
    Ret.Width        = Mode.Width;
    Ret.Height       = Mode.Height;
    Ret.BitsPerPixel = Mode.BitsPerPixel;

    return Ret;
}


////////////////////////////////////////////////////////////
/// Get valid video modes count
////////////////////////////////////////////////////////////
size_t sfVideoMode_GetModesCount()
{
    return sf::VideoMode::GetModesCount();
}


////////////////////////////////////////////////////////////
/// Tell whether or not a video mode is supported
////////////////////////////////////////////////////////////
sfBool sfVideoMode_IsValid(sfVideoMode Mode)
{
    sf::VideoMode VideoMode(Mode.Width, Mode.Height, Mode.BitsPerPixel);
    return VideoMode.IsValid() ? sfTrue : sfFalse;
}
