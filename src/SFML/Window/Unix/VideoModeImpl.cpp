////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Window/VideoModeImpl.hpp>
#include <SFML/Window/Unix/Display.hpp>
#include <SFML/System/Err.hpp>
#include <X11/Xlib-xcb.h>
#include <xcb/randr.h>
#include <algorithm>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
std::vector<VideoMode> VideoModeImpl::getFullscreenModes()
{
    std::vector<VideoMode> modes;

    // Open a connection with the X server
    Display* display = OpenDisplay();
    xcb_connection_t* connection = XGetXCBConnection(display);

    if (display)
    {
        // Check if the XRandR extension is present
        xcb_query_extension_cookie_t cookie = xcb_query_extension(connection, 5, "RANDR");

        // Retrieve the default screen
        xcb_screen_t* screen = XCBScreenOfDisplay(connection, DefaultScreen(display));

        // Check if the XRandR extension is present
        xcb_query_extension_reply_t* randr_ext = xcb_query_extension_reply(connection, cookie, NULL);
        if (randr_ext->present)
        {
            // Get the current configuration
            xcb_generic_error_t* errors;
            xcb_randr_get_screen_info_reply_t* config = xcb_randr_get_screen_info_reply(
                        connection, xcb_randr_get_screen_info(connection, screen->root), &errors);
            if (! errors)
            {
                // Get the available screen sizes
                xcb_randr_screen_size_t* sizes = xcb_randr_get_screen_info_sizes(config);
                if (sizes && (config->nSizes > 0))
                {
                    // Get the list of supported depths
                    xcb_depth_iterator_t iter = xcb_screen_allowed_depths_iterator(screen);
                    // Combine depths and sizes to fill the array of supported modes
                    for (; iter.rem; xcb_depth_next(&iter))
                    {
                        for (int j = 0; j < config->nSizes; ++j)
                        {
                            // Convert to VideoMode
                            VideoMode mode(sizes[j].width, sizes[j].height, iter.data->depth);

                            // Add it only if it is not already in the array
                            if (std::find(modes.begin(), modes.end(), mode) == modes.end())
                                modes.push_back(mode);
                        }
                    }
                }
            }
            else
            {
                // Failed to get the screen configuration
                err() << "Failed to retrieve the screen configuration while trying to get the supported video modes" << std::endl;
            }
            // Free the configuration instance
            free(errors);
            free(config);
        }
        else
        {
            // XRandr extension is not supported: we cannot get the video modes
            err() << "Failed to use the XRandR extension while trying to get the supported video modes" << std::endl;
        }
        free(randr_ext);
        // Close the connection with the X server
        CloseDisplay(display);
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
    Display* display = OpenDisplay();
    xcb_connection_t* connection = XGetXCBConnection(display);
    if (display)
    {
        xcb_query_extension_cookie_t cookie = xcb_query_extension(connection, 5, "RANDR");
        // Retrieve the default screen
        xcb_screen_t* screen = XCBScreenOfDisplay(connection, DefaultScreen(display));

        // Check if the XRandR extension is present
        xcb_query_extension_reply_t* randr_ext = xcb_query_extension_reply(connection, cookie, NULL);
        if (randr_ext->present)
        {
            // Get the current configuration
            xcb_generic_error_t* errors;
            xcb_randr_get_screen_info_reply_t* config =
                    xcb_randr_get_screen_info_reply(
                        connection, xcb_randr_get_screen_info(connection, screen->root), &errors);
            if (! errors)
            {
                // Get the current video mode
                xcb_randr_rotation_t Rotation = (xcb_randr_rotation_t)config->rotation;
                xcb_randr_mode_t currentMode = config->sizeID;

                // Get the available screen sizes
                int nbSizes = xcb_randr_get_screen_info_sizes_length(config);
                xcb_randr_screen_size_t* sizes= xcb_randr_get_screen_info_sizes(config);
                if (sizes && (nbSizes > 0))
                    desktopMode = VideoMode(sizes[currentMode].width, sizes[currentMode].height, screen->root_depth);
            }
            else
            {
                // Failed to get the screen configuration
                err() << "Failed to retrieve the screen configuration while trying to get the desktop video modes" << std::endl;
            }
            // Free the configuration instance
            free(errors);
            free(config);
        }
        else
        {
            // XRandr extension is not supported: we cannot get the video modes
            err() << "Failed to use the XRandR extension while trying to get the desktop video modes" << std::endl;
        }

        free(randr_ext);
        // Close the connection with the X server
        CloseDisplay(display);
    }
    else
    {
        // We couldn't connect to the X server
        err() << "Failed to connect to the X server while trying to get the desktop video modes" << std::endl;
    }

    return desktopMode;
}

} // namespace priv

} // namespace sf
