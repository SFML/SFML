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
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/VideoModeImpl.hpp>
#include <algorithm>
#include <functional>


namespace sf
{
////////////////////////////////////////////////////////////
VideoMode::VideoMode() :
Width       (0),
Height      (0),
BitsPerPixel(0)
{

}


////////////////////////////////////////////////////////////
VideoMode::VideoMode(unsigned int width, unsigned int height, unsigned int bitsPerPixel) :
Width       (width),
Height      (height),
BitsPerPixel(bitsPerPixel)
{

}


////////////////////////////////////////////////////////////
VideoMode VideoMode::GetDesktopMode()
{
    // Directly forward to the OS-specific implementation
    return priv::VideoModeImpl::GetDesktopMode();
}


////////////////////////////////////////////////////////////
const std::vector<VideoMode>& VideoMode::GetFullscreenModes()
{
    static std::vector<VideoMode> modes;

    // Populate the array on first call
    if (modes.empty())
    {
        modes = priv::VideoModeImpl::GetFullscreenModes();
        std::sort(modes.begin(), modes.end(), std::greater<VideoMode>());
    }

    return modes;
}


////////////////////////////////////////////////////////////
bool VideoMode::IsValid() const
{
    const std::vector<VideoMode>& modes = GetFullscreenModes();

    return std::find(modes.begin(), modes.end(), *this) != modes.end();
}


////////////////////////////////////////////////////////////
bool operator ==(const VideoMode& left, const VideoMode& right)
{
    return (left.Width        == right.Width)        &&
           (left.Height       == right.Height)       &&
           (left.BitsPerPixel == right.BitsPerPixel);
}


////////////////////////////////////////////////////////////
bool operator !=(const VideoMode& left, const VideoMode& right)
{
    return !(left == right);
}


////////////////////////////////////////////////////////////
bool operator <(const VideoMode& left, const VideoMode& right)
{
    if (left.BitsPerPixel == right.BitsPerPixel)
    {
        if (left.Width == right.Width)
        {
            return left.Height < right.Height;
        }
        else
        {
            return left.Width < right.Width;
        }
    }
    else
    {
        return left.BitsPerPixel < right.BitsPerPixel;
    }
}


////////////////////////////////////////////////////////////
bool operator >(const VideoMode& left, const VideoMode& right)
{
    return right < left;
}


////////////////////////////////////////////////////////////
bool operator <=(const VideoMode& left, const VideoMode& right)
{
    return !(right < left);
}


////////////////////////////////////////////////////////////
bool operator >=(const VideoMode& left, const VideoMode& right)
{
    return !(left < right);
}

} // namespace sf
