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
#include <SFML/Window/Linux/VideoModeSupport.hpp>
#include <SFML/Window/Linux/WindowImplX11.hpp>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <algorithm>
#include <iostream>


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

    // Get the display and screen from WindowImplUnix
    WindowImplX11::OpenDisplay(false);
    Display* Disp   = WindowImplX11::ourDisplay;
    int      Screen = WindowImplX11::ourScreen;

    // Check if the XRandR extension is present
    int Version;
    if (XQueryExtension(Disp, "RANDR", &Version, &Version, &Version))
    {
        // Get the current configuration
        XRRScreenConfiguration* Config = XRRGetScreenInfo(Disp, RootWindow(Disp, Screen));
        if (Config)
        {
            // Get the available screen sizes
            int NbSizes;
            XRRScreenSize* Sizes = XRRConfigSizes(Config, &NbSizes);
            if (Sizes && (NbSizes > 0))
            {
                // Get the list of supported depths
                int NbDepths = 0;
                int* Depths = XListDepths(Disp, Screen, &NbDepths);
                if (Depths && (NbDepths > 0))
                {
                    // Combine depths and sizes to fill the array of supported modes
                    for (int i = 0; i < NbDepths; ++i)
                    {
                        for (int j = 0; j < NbSizes; ++j)
                        {
                            // Convert to VideoMode
                            VideoMode Mode(Sizes[j].width, Sizes[j].height, Depths[i]);
        
                            // Add it only if it is not already in the array
                            if (std::find(Modes.begin(), Modes.end(), Mode) == Modes.end())
                                Modes.push_back(Mode);
                        }
                    }
                }
            }

            // Free the configuration instance
            XRRFreeScreenConfigInfo(Config);
        }
        else
        {
            // Failed to get the screen configuration
            std::cerr << "Failed to get the list of available video modes" << std::endl;
        }
    }
    else
    {
        // XRandr extension is not supported : we cannot get the video modes
        std::cerr << "Failed to get the list of available video modes" << std::endl;
    }
}


////////////////////////////////////////////////////////////
/// Get current desktop video mode
////////////////////////////////////////////////////////////
VideoMode VideoModeSupport::GetDesktopVideoMode()
{
    VideoMode DesktopMode;

    // Get the display and screen from WindowImplUnix
    WindowImplX11::OpenDisplay(false);
    Display* Disp   = WindowImplX11::ourDisplay;
    int      Screen = WindowImplX11::ourScreen;

    // Check if the XRandR extension is present
    int Version;
    if (XQueryExtension(Disp, "RANDR", &Version, &Version, &Version))
    {
        // Get the current configuration
        XRRScreenConfiguration* Config = XRRGetScreenInfo(Disp, RootWindow(Disp, Screen));
        if (Config)
        {
            // Get the current video mode
            Rotation CurrentRotation;
            int CurrentMode = XRRConfigCurrentConfiguration(Config, &CurrentRotation);

            // Get the available screen sizes
            int NbSizes;
            XRRScreenSize* Sizes = XRRConfigSizes(Config, &NbSizes);
            if (Sizes && (NbSizes > 0))
                DesktopMode = VideoMode(Sizes[CurrentMode].width, Sizes[CurrentMode].height, DefaultDepth(Disp, Screen));

            // Free the configuration instance
            XRRFreeScreenConfigInfo(Config);
        }
    }

    return DesktopMode;
}

} // namespace priv

} // namespace sf
