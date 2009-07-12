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
#include <SFML/Window/Linux/DisplayRef.hpp>
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
void VideoModeSupport::GetSupportedVideoModes(std::vector<VideoMode>& modes)
{
    // First, clear array to fill
    modes.clear();

    // Get an access to the display
    DisplayRef disp;
    int screen = DefaultScreen(disp.GetDisplay());

    // Check if the XRandR extension is present
    int version;
    if (XQueryExtension(disp.GetDisplay(), "RANDR", &version, &version, &version))
    {
        // Get the current configuration
        XRRScreenConfiguration* config = XRRGetScreenInfo(disp.GetDisplay(), RootWindow(disp.GetDisplay(), screen));
        if (config)
        {
            // Get the available screen sizes
            int nbSizes;
            XRRScreenSize* sizes = XRRConfigSizes(config, &nbSizes);
            if (sizes && (nbSizes > 0))
            {
                // Get the list of supported depths
                int nbDepths = 0;
                int* depths = XListDepths(disp.GetDisplay(), screen, &nbDepths);
                if (depths && (nbDepths > 0))
                {
                    // Combine depths and sizes to fill the array of supported modes
                    for (int i = 0; i < nbDepths; ++i)
                    {
                        for (int j = 0; j < nbSizes; ++j)
                        {
                            // Convert to VideoMode
                            VideoMode mode(sizes[j].width, sizes[j].height, depths[i]);
        
                            // Add it only if it is not already in the array
                            if (std::find(modes.begin(), modes.end(), mode) == modes.end())
                                modes.push_back(mode);
                        }
                    }
                }
            }

            // Free the configuration instance
            XRRFreeScreenConfigInfo(config);
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
    VideoMode desktopMode;

    // Get an access to the display
    DisplayRef disp;
    int screen = DefaultScreen(disp.GetDisplay());

    // Check if the XRandR extension is present
    int version;
    if (XQueryExtension(disp.GetDisplay(), "RANDR", &version, &version, &version))
    {
        // Get the current configuration
        XRRScreenConfiguration* config = XRRGetScreenInfo(disp.GetDisplay(), RootWindow(disp.GetDisplay(), screen));
        if (config)
        {
            // Get the current video mode
            Rotation currentRotation;
            int currentMode = XRRConfigCurrentConfiguration(config, &currentRotation);

            // Get the available screen sizes
            int nbSizes;
            XRRScreenSize* sizes = XRRConfigSizes(config, &nbSizes);
            if (sizes && (nbSizes > 0))
                desktopMode = VideoMode(sizes[currentMode].width, sizes[currentMode].height, DefaultDepth(disp.GetDisplay(), screen));

            // Free the configuration instance
            XRRFreeScreenConfigInfo(config);
        }
    }

    return desktopMode;
}

} // namespace priv

} // namespace sf
