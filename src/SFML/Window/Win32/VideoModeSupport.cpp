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
#include <SFML/Window/Win32/VideoModeSupport.hpp>
#include <windows.h>
#include <algorithm>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Get supported video modes
////////////////////////////////////////////////////////////
void VideoModeSupport::GetSupportedVideoModes(std::vector<VideoMode>& Modes)
{
    // First, clear array to fill
    Modes.clear();

    // Enumerate all available video modes for primary display adapter
    DEVMODE Win32Mode;
    Win32Mode.dmSize = sizeof(DEVMODE);
    for (int Count = 0; EnumDisplaySettings(NULL, Count, &Win32Mode); ++Count)
    {
        // Convert to sfVideoMode
        VideoMode Mode(Win32Mode.dmPelsWidth, Win32Mode.dmPelsHeight, Win32Mode.dmBitsPerPel);

        // Add it only if it is not already in the array
        if (std::find(Modes.begin(), Modes.end(), Mode) == Modes.end())
            Modes.push_back(Mode);
    }
}


////////////////////////////////////////////////////////////
/// Get current desktop video mode
////////////////////////////////////////////////////////////
VideoMode VideoModeSupport::GetDesktopVideoMode()
{
    DEVMODE Win32Mode;
    Win32Mode.dmSize = sizeof(DEVMODE);
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &Win32Mode);

    return VideoMode(Win32Mode.dmPelsWidth, Win32Mode.dmPelsHeight, Win32Mode.dmBitsPerPel);
}

} // namespace priv

} // namespace sf
