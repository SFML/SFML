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
#include <SFML/Window/VideoModeSupport.hpp>
#include <algorithm>
#include <vector>


////////////////////////////////////////////////////////////
/// Private data
////////////////////////////////////////////////////////////
namespace
{
    // Global array of supported video modes
    std::vector<sf::VideoMode> supportedModes;

    // Functor for sorting modes from highest to lowest
    struct CompareModes
    {
        bool operator ()(const sf::VideoMode& left, const sf::VideoMode& right) const
        {
            if (left.BitsPerPixel > right.BitsPerPixel)
                return true;
            else if (left.BitsPerPixel < right.BitsPerPixel)
                return false;
            else if (left.Width > right.Width)
                return true;
            else if (left.Width < right.Width)
                return false;
            else
                return (left.Height > right.Height);
        }
    };

    // Get and sort valid video modes
    static void InitializeModes()
    {
        // We request the array of valid modes
        sf::priv::VideoModeSupport::GetSupportedVideoModes(supportedModes);

        // And we sort them from highest to lowest (so that number 0 is the best)
        std::sort(supportedModes.begin(), supportedModes.end(), CompareModes());
    }
}


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
    // Directly forward to the video mode support
    return priv::VideoModeSupport::GetDesktopVideoMode();
}


////////////////////////////////////////////////////////////
VideoMode VideoMode::GetMode(std::size_t index)
{
    // Build and cache the list of valid modes on first call
    if (supportedModes.empty())
        InitializeModes();

    if (index < GetModesCount())
        return supportedModes[index];
    else
        return VideoMode();
}


////////////////////////////////////////////////////////////
std::size_t VideoMode::GetModesCount()
{
    // Build and cache the list of valid modes on first call
    if (supportedModes.empty())
        InitializeModes();

    return supportedModes.size();
}


////////////////////////////////////////////////////////////
bool VideoMode::IsValid() const
{
    // Build and cache the list of valid modes on first call
    if (supportedModes.empty())
        InitializeModes();

    return std::find(supportedModes.begin(), supportedModes.end(), *this) != supportedModes.end();
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

} // namespace sf
