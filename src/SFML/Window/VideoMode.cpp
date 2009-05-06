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
/// Internal data
////////////////////////////////////////////////////////////
namespace
{
    // Global array of supported video modes
    std::vector<sf::VideoMode> SupportedModes;

    // Functor for sorting modes from highest to lowest
    struct CompareModes
    {
        bool operator ()(const sf::VideoMode& v1, const sf::VideoMode& v2) const
        {
            if (v1.BitsPerPixel > v2.BitsPerPixel)
                return true;
            else if (v1.BitsPerPixel < v2.BitsPerPixel)
                return false;
            else if (v1.Width > v2.Width)
                return true;
            else if (v1.Width < v2.Width)
                return false;
            else
                return (v1.Height > v2.Height);
        }
    };
}


namespace sf
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
VideoMode::VideoMode() :
Width       (0),
Height      (0),
BitsPerPixel(0)
{

}


////////////////////////////////////////////////////////////
/// Construct the video mode with its attributes
////////////////////////////////////////////////////////////
VideoMode::VideoMode(unsigned int ModeWidth, unsigned int ModeHeight, unsigned int ModeBpp) :
Width       (ModeWidth),
Height      (ModeHeight),
BitsPerPixel(ModeBpp)
{

}


////////////////////////////////////////////////////////////
/// Get the current desktop video mode
////////////////////////////////////////////////////////////
VideoMode VideoMode::GetDesktopMode()
{
    // Directly forward to the video mode support
    return priv::VideoModeSupport::GetDesktopVideoMode();
}


////////////////////////////////////////////////////////////
/// Get a valid video mode
/// Index must be in range [0, GetModesCount()[
////////////////////////////////////////////////////////////
VideoMode VideoMode::GetMode(std::size_t Index)
{
    if (SupportedModes.empty())
        InitializeModes();

    if (Index < GetModesCount())
        return SupportedModes[Index];
    else
        return VideoMode();
}


////////////////////////////////////////////////////////////
/// Get valid video modes count
////////////////////////////////////////////////////////////
std::size_t VideoMode::GetModesCount()
{
    if (SupportedModes.empty())
        InitializeModes();

    return SupportedModes.size();
}


////////////////////////////////////////////////////////////
/// Tell whether or not the video mode is supported
////////////////////////////////////////////////////////////
bool VideoMode::IsValid() const
{
    if (SupportedModes.empty())
        InitializeModes();

    return std::find(SupportedModes.begin(), SupportedModes.end(), *this) != SupportedModes.end();
}


////////////////////////////////////////////////////////////
/// Comparison operator overload -- tell if two video modes are equal
////////////////////////////////////////////////////////////
bool VideoMode::operator ==(const VideoMode& Other) const
{
    return (Width        == Other.Width)        &&
           (Height       == Other.Height)       &&
           (BitsPerPixel == Other.BitsPerPixel);
}


////////////////////////////////////////////////////////////
/// Comparison operator overload -- tell if two video modes are different
////////////////////////////////////////////////////////////
bool VideoMode::operator !=(const VideoMode& Other) const
{
    return !(*this == Other);
}


////////////////////////////////////////////////////////////
/// Get and sort valid video modes
////////////////////////////////////////////////////////////
void VideoMode::InitializeModes()
{
    // We request the array of valid modes
    priv::VideoModeSupport::GetSupportedVideoModes(SupportedModes);

    // And we sort them from highest to lowest (so that number 0 is the best)
    std::sort(SupportedModes.begin(), SupportedModes.end(), CompareModes());
}

} // namespace sf
