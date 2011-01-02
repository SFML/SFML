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

#ifndef SFML_WINDOW_H
#define SFML_WINDOW_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Window/Event.h>
#include <SFML/Window/VideoMode.h>
#include <SFML/Window/WindowHandle.h>
#include <SFML/Window/Types.h>


////////////////////////////////////////////////////////////
/// Enumeration of window creation styles
///
////////////////////////////////////////////////////////////
enum
{
    sfNone         = 0,      ///< No border / title bar (this flag and all others are mutually exclusive)
    sfTitlebar     = 1 << 0, ///< Title bar + fixed border
    sfResize       = 1 << 1, ///< Titlebar + resizable border + maximize button
    sfClose        = 1 << 2, ///< Titlebar + close button
    sfFullscreen   = 1 << 3, ///< Fullscreen mode (this flag and all others are mutually exclusive)
    sfDefaultStyle = sfTitlebar | sfResize | sfClose ///< Default window style
};


////////////////////////////////////////////////////////////
/// Structure defining the window's creation settings
////////////////////////////////////////////////////////////
typedef struct
{
    unsigned int DepthBits;         ///< Bits of the depth buffer
    unsigned int StencilBits;       ///< Bits of the stencil buffer
    unsigned int AntialiasingLevel; ///< Level of antialiasing
    unsigned int MajorVersion;      ///< Major number of the context version to create
    unsigned int MinorVersion;      ///< Minor number of the context version to create
} sfContextSettings;


////////////////////////////////////////////////////////////
/// Construct a new window
///
/// \param mode :     Video mode to use
/// \param title :    Title of the window
/// \param style :    Window style
/// \param settings : Creation settings (pass NULL to use default values)
///
////////////////////////////////////////////////////////////
CSFML_API sfWindow* sfWindow_Create(sfVideoMode mode, const char* title, unsigned long style, const sfContextSettings* settings);

////////////////////////////////////////////////////////////
/// Construct a window from an existing control
///
/// \param handle :   Platform-specific handle of the control
/// \param settings : Creation settings (pass NULL to use default values)
///
////////////////////////////////////////////////////////////
CSFML_API sfWindow* sfWindow_CreateFromHandle(sfWindowHandle handle, const sfContextSettings* settings);

////////////////////////////////////////////////////////////
/// Destroy an existing window
///
/// \param window : Window to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_Destroy(sfWindow* window);

////////////////////////////////////////////////////////////
/// Close a window (but doesn't destroy the internal data)
///
/// \param window : Window to close
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_Close(sfWindow* window);

////////////////////////////////////////////////////////////
/// Tell whether or not a window is opened
///
/// \param window : Window object
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfWindow_IsOpened(const sfWindow* window);

////////////////////////////////////////////////////////////
/// Get the width of the rendering region of a window
///
/// \param window : Window object
///
/// \return Width in pixels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfWindow_GetWidth(const sfWindow* window);

////////////////////////////////////////////////////////////
/// Get the height of the rendering region of a window
///
/// \param window : Window object
///
/// \return Height in pixels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfWindow_GetHeight(const sfWindow* window);

////////////////////////////////////////////////////////////
/// Get the creation settings of a window
///
/// \param window : Window object
///
/// \return Settings used to create the window
///
////////////////////////////////////////////////////////////
CSFML_API sfContextSettings sfWindow_GetSettings(const sfWindow* window);

////////////////////////////////////////////////////////////
/// Get the event on top of events stack of a window, if any, and pop it
///
/// \param window : Window object
/// \param event :  Event to fill, if any
///
/// \return sfTrue if an event was returned, sfFalse if events stack was empty
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfWindow_GetEvent(sfWindow* window, sfEvent* event);

////////////////////////////////////////////////////////////
/// Wait for an event and return it
///
/// \param window : Window object
/// \param event :  Event to fill
///
/// \return sfFalse if an error occured
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfWindow_WaitEvent(sfWindow* window, sfEvent* event);

////////////////////////////////////////////////////////////
/// Enable / disable vertical synchronization on a window
///
/// \param window :  Window object
/// \param enabled : sfTrue to enable v-sync, sfFalse to deactivate
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_EnableVerticalSync(sfWindow* window, sfBool enabled);

////////////////////////////////////////////////////////////
/// Show or hide the mouse cursor on a window
///
/// \param window : Window object
/// \param show :   sfTrue to show, sfFalse to hide
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_ShowMouseCursor(sfWindow* window, sfBool show);

////////////////////////////////////////////////////////////
/// Change the position of the mouse cursor on a window
///
/// \param window : Window object
/// \param left :   Left coordinate of the cursor, relative to the window
/// \param top :    Top coordinate of the cursor, relative to the window
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_SetCursorPosition(sfWindow* window, unsigned int left, unsigned int Top);

////////////////////////////////////////////////////////////
/// Change the position of a window on screen.
/// Only works for top-level windows
///
/// \param window : Window object
/// \param left :   Left position
/// \param top :    Top position
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_SetPosition(sfWindow* window, int left, int top);

////////////////////////////////////////////////////////////
/// Change the size of the rendering region of a window
///
/// \param window : Window object
/// \param width :  New Width
/// \param height : New Height
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_SetSize(sfWindow* window, unsigned int width, unsigned int height);

////////////////////////////////////////////////////////////
/// Show or hide a window
///
/// \param window : Window object
/// \param show :   sfTrue to show, sfFalse to hide
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_Show(sfWindow* window, sfBool show);

////////////////////////////////////////////////////////////
/// Enable or disable automatic key-repeat for keydown events.
/// Automatic key-repeat is enabled by default
///
/// \param window :  Window object
/// \param enabled : sfTrue to enable, sfFalse to disable
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_EnableKeyRepeat(sfWindow* window, sfBool enabled);

////////////////////////////////////////////////////////////
/// Change the window's icon
///
/// \param window : Window object
/// \param width :  Icon's width, in pixels
/// \param height : Icon's height, in pixels
/// \param pixels : Pointer to the pixels in memory, format must be RGBA 32 bits
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_SetIcon(sfWindow* window, unsigned int width, unsigned int height, const sfUint8* pixels);

////////////////////////////////////////////////////////////
/// Activate or deactivate a window as the current target for rendering
///
/// \param window : Window object
/// \param active : sfTrue to activate, sfFalse to deactivate
///
/// \return True if operation was successful, false otherwise
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfWindow_SetActive(sfWindow* window, sfBool active);

////////////////////////////////////////////////////////////
/// Display a window on screen
///
/// \param window : Window object
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_Display(sfWindow* window);

////////////////////////////////////////////////////////////
/// Get the input manager of a window
///
/// \param window : Window object
///
/// \return Reference to the input
///
////////////////////////////////////////////////////////////
CSFML_API const sfInput* sfWindow_GetInput(sfWindow* window);

////////////////////////////////////////////////////////////
/// Limit the framerate to a maximum fixed frequency for a window
///
/// \param window : Window object
/// \param limit :  Framerate limit, in frames per seconds (use 0 to disable limit)
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_SetFramerateLimit(sfWindow* window, unsigned int limit);

////////////////////////////////////////////////////////////
/// Get time elapsed since last frame of a window
///
/// \param window : Window object
///
/// \return Time elapsed, in seconds
///
////////////////////////////////////////////////////////////
CSFML_API float sfWindow_GetFrameTime(const sfWindow* window);

////////////////////////////////////////////////////////////
/// Change the joystick threshold, ie. the value below which
/// no move event will be generated
///
/// \param window :    Window object
/// \param threshold : New threshold, in range [0, 100]
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_SetJoystickThreshold(sfWindow* window, float threshold);

////////////////////////////////////////////////////////////
/// Retrieve the Os-specific handle of a window
///
/// \param window : Window object
///
/// \return Window handle
///
////////////////////////////////////////////////////////////
CSFML_API sfWindowHandle sfWindow_GetSystemHandle(const sfWindow* window);


#endif // SFML_WINDOW_H
