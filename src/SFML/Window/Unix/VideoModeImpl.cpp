////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/Unix/Display.hpp>
#include <SFML/Window/Unix/Utils.hpp>
#include <SFML/Window/VideoModeImpl.hpp>

#include <SFML/System/Err.hpp>

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

#include <algorithm>
#include <ostream>


namespace sf::priv
{
////////////////////////////////////////////////////////////
template <>
struct XDeleter<XRRScreenConfiguration>
{
    void operator()(XRRScreenConfiguration* config) const
    {
        XRRFreeScreenConfigInfo(config);
    }
};


////////////////////////////////////////////////////////////
std::vector<VideoMode> VideoModeImpl::getFullscreenModes()
{
    std::vector<VideoMode> modes;

    // Open a connection with the X server
    Display* display = openDisplay();
    if (display)
    {
        // Retrieve the default screen number
        const int screen = DefaultScreen(display);

        // Check if the XRandR extension is present
        int version;
        if (XQueryExtension(display, "RANDR", &version, &version, &version))
        {
            // Get the current configuration
            auto config = X11Ptr<XRRScreenConfiguration>(XRRGetScreenInfo(display, RootWindow(display, screen)));
            if (config)
            {
                // Get the available screen sizes
                int            nbSizes;
                XRRScreenSize* sizes = XRRConfigSizes(config.get(), &nbSizes);
                if (sizes && (nbSizes > 0))
                {
                    // Get the list of supported depths
                    int  nbDepths = 0;
                    auto depths   = X11Ptr<int[]>(XListDepths(display, screen, &nbDepths));
                    if (depths && (nbDepths > 0))
                    {
                        // Combine depths and sizes to fill the array of supported modes
                        for (std::size_t i = 0; i < static_cast<std::size_t>(nbDepths); ++i)
                        {
                            for (int j = 0; j < nbSizes; ++j)
                            {
                                // Convert to VideoMode
                                VideoMode mode({static_cast<unsigned int>(sizes[j].width),
                                                static_cast<unsigned int>(sizes[j].height)},
                                               static_cast<unsigned int>(depths[i]));

                                Rotation currentRotation;
                                XRRConfigRotations(config.get(), &currentRotation);

                                if (currentRotation == RR_Rotate_90 || currentRotation == RR_Rotate_270)
                                    std::swap(mode.size.x, mode.size.y);

                                // Add it only if it is not already in the array
                                if (std::find(modes.begin(), modes.end(), mode) == modes.end())
                                    modes.push_back(mode);
                            }
                        }
                    }
                }
            }
            else
            {
                // Failed to get the screen configuration
                err() << "Failed to retrieve the screen configuration while trying to get the supported video modes"
                      << std::endl;
            }
        }
        else
        {
            // XRandr extension is not supported: we cannot get the video modes
            err() << "Failed to use the XRandR extension while trying to get the supported video modes" << std::endl;
        }

        // Close the connection with the X server
        closeDisplay(display);
    }
    else
    {
        // We couldn't connect to the X server
        err() << "Failed to connect to the X server while trying to get the supported video modes" << std::endl;
    }

    return modes;
}


////////////////////////////////////////////////////////////
VideoMode VideoModeImpl::getDesktopMode()
{
    VideoMode desktopMode;

    // Open a connection with the X server
    Display* display = openDisplay();
    if (display)
    {
        // Retrieve the default screen number
        const int screen = DefaultScreen(display);

        // Check if the XRandR extension is present
        int version;
        if (XQueryExtension(display, "RANDR", &version, &version, &version))
        {
            // Get the current configuration
            auto config = X11Ptr<XRRScreenConfiguration>(XRRGetScreenInfo(display, RootWindow(display, screen)));
            if (config)
            {
                // Get the current video mode
                Rotation  currentRotation;
                const int currentMode = XRRConfigCurrentConfiguration(config.get(), &currentRotation);

                // Get the available screen sizes
                int            nbSizes;
                XRRScreenSize* sizes = XRRConfigSizes(config.get(), &nbSizes);
                if (sizes && (nbSizes > 0))
                {
                    desktopMode = VideoMode({static_cast<unsigned int>(sizes[currentMode].width),
                                             static_cast<unsigned int>(sizes[currentMode].height)},
                                            static_cast<unsigned int>(DefaultDepth(display, screen)));

                    Rotation modeRotation;
                    XRRConfigRotations(config.get(), &modeRotation);

                    if (modeRotation == RR_Rotate_90 || modeRotation == RR_Rotate_270)
                        std::swap(desktopMode.size.x, desktopMode.size.y);
                }
            }
            else
            {
                // Failed to get the screen configuration
                err() << "Failed to retrieve the screen configuration while trying to get the desktop video modes"
                      << std::endl;
            }
        }
        else
        {
            // XRandr extension is not supported: we cannot get the video modes
            err() << "Failed to use the XRandR extension while trying to get the desktop video modes" << std::endl;
        }

        // Close the connection with the X server
        closeDisplay(display);
    }
    else
    {
        // We couldn't connect to the X server
        err() << "Failed to connect to the X server while trying to get the desktop video modes" << std::endl;
    }

    return desktopMode;
}

} // namespace sf::priv
