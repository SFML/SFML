////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/MonitorImpl.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
VideoMode::VideoMode(const Vector2u& modeSize, unsigned int modeBitsPerPixel) :
size(modeSize),
bitsPerPixel(modeBitsPerPixel)
{
}


////////////////////////////////////////////////////////////
bool operator==(const VideoMode& left, const VideoMode& right)
{
    return (left.size == right.size) && (left.bitsPerPixel == right.bitsPerPixel);
}


////////////////////////////////////////////////////////////
bool operator!=(const VideoMode& left, const VideoMode& right)
{
    return !(left == right);
}


////////////////////////////////////////////////////////////
bool operator<(const VideoMode& left, const VideoMode& right)
{
    if (left.bitsPerPixel == right.bitsPerPixel)
    {
        if (left.size.x == right.size.x)
        {
            return left.size.y < right.size.y;
        }
        else
        {
            return left.size.x < right.size.x;
        }
    }
    else
    {
        return left.bitsPerPixel < right.bitsPerPixel;
    }
}


////////////////////////////////////////////////////////////
bool operator>(const VideoMode& left, const VideoMode& right)
{
    return right < left;
}


////////////////////////////////////////////////////////////
bool operator<=(const VideoMode& left, const VideoMode& right)
{
    return !(right < left);
}


////////////////////////////////////////////////////////////
bool operator>=(const VideoMode& left, const VideoMode& right)
{
    return !(left < right);
}

} // namespace sf
