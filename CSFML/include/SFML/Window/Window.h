////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2008-2008 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Window/Input.h>
#include <SFML/Window/VideoMode.h>
#include <SFML/Window/WindowHandle.h>


////////////////////////////////////////////////////////////
/// sfWindow is a rendering window ; it can create a new window
/// or connect to an existing one
////////////////////////////////////////////////////////////
typedef struct sfWindow sfWindow;


////////////////////////////////////////////////////////////
/// Enumeration of window creation styles
///
////////////////////////////////////////////////////////////
enum
{
    sfNone       = 0,      ///< No border / title bar (this flag and all others are mutually exclusive)
    sfTitlebar   = 1 << 0, ///< Title bar + fixed border
    sfResize     = 1 << 1, ///< Titlebar + resizable border + maximize button
    sfClose      = 1 << 2, ///< Titlebar + close button
    sfFullscreen = 1 << 3  ///< Fullscreen mode (this flag and all others are mutually exclusive)
};


////////////////////////////////////////////////////////////
/// Structure defining the window's creation settings
////////////////////////////////////////////////////////////
typedef struct
{
    unsigned int DepthBits;         ///< Bits of the depth buffer
    unsigned int StencilBits;       ///< Bits of the stencil buffer
    unsigned int AntialiasingLevel; ///< Level of antialiasing
} sfWindowSettings;


////////////////////////////////////////////////////////////
/// Construct a new window
///
/// \param Mode :   Video mode to use
/// \param Title :  Title of the window
/// \param Style :  Window style
/// \param Params : Creation settings
///
////////////////////////////////////////////////////////////
CSFML_API sfWindow* sfWindow_Create(sfVideoMode Mode, const char* Title, unsigned long Style, sfWindowSettings Params);

////////////////////////////////////////////////////////////
/// Construct a window from an existing control
///
/// \param Handle : Platform-specific handle of the control
/// \param Params : Creation settings
///
////////////////////////////////////////////////////////////
CSFML_API sfWindow* sfWindow_CreateFromHandle(sfWindowHandle Handle, sfWindowSettings Params);

////////////////////////////////////////////////////////////
/// Destroy an existing window
///
/// \param Window : Window to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_Destroy(sfWindow* Window);

////////////////////////////////////////////////////////////
/// Close a window (but doesn't destroy the internal data)
///
/// \param Window : Window to close
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_Close(sfWindow* Window);

////////////////////////////////////////////////////////////
/// Tell whether or not a window is opened
///
/// \param Window : Window object
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfWindow_IsOpened(sfWindow* Window);

////////////////////////////////////////////////////////////
/// Get the width of the rendering region of a window
///
/// \param Window : Window object
///
/// \return Width in pixels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfWindow_GetWidth(sfWindow* Window);

////////////////////////////////////////////////////////////
/// Get the height of the rendering region of a window
///
/// \param Window : Window object
///
/// \return Height in pixels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfWindow_GetHeight(sfWindow* Window);

////////////////////////////////////////////////////////////
/// Get the creation settings of a window
///
/// \param Window : Window object
///
/// \return Settings used to create the window
///
////////////////////////////////////////////////////////////
CSFML_API sfWindowSettings sfWindow_GetSettings(sfWindow* Window);

////////////////////////////////////////////////////////////
/// Get the event on top of events stack of a window, if any, and pop it
///
/// \param Window : Window object
/// \param Event :  Event to fill, if any
///
/// \return sfTrue if an event was returned, sfFalse if events stack was empty
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfWindow_GetEvent(sfWindow* Window, sfEvent* Event);

////////////////////////////////////////////////////////////
/// Enable / disable vertical synchronization on a window
///
/// \param Window :  Window object
/// \param Enabled : sfTrue to enable v-sync, sfFalse to deactivate
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_UseVerticalSync(sfWindow* Window, sfBool Enabled);

////////////////////////////////////////////////////////////
/// Show or hide the mouse cursor on a window
///
/// \param Window : Window object
/// \param Show :   sfTrue to show, sfFalse to hide
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_ShowMouseCursor(sfWindow* Window, sfBool Show);

////////////////////////////////////////////////////////////
/// Change the position of the mouse cursor on a window
///
/// \param Window : Window object
/// \param Left :   Left coordinate of the cursor, relative to the window
/// \param Top :    Top coordinate of the cursor, relative to the window
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_SetCursorPosition(sfWindow* Window, unsigned int Left, unsigned int Top);

////////////////////////////////////////////////////////////
/// Change the position of a window on screen.
/// Only works for top-level windows
///
/// \param Window : Window object
/// \param Left :   Left position
/// \param Top :    Top position
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_SetPosition(sfWindow* Window, int Left, int Top);

////////////////////////////////////////////////////////////
/// Change the size of the rendering region of a window
///
/// \param Window : Window object
/// \param Width :  New Width
/// \param Height : New Height
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_SetSize(sfWindow* Window, unsigned int Width, unsigned int Height);

////////////////////////////////////////////////////////////
/// Show or hide a window
///
/// \param Window : Window object
/// \param State :  sfTrue to show, sfFalse to hide
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_Show(sfWindow* Window, sfBool State);

////////////////////////////////////////////////////////////
/// Enable or disable automatic key-repeat for keydown events.
/// Automatic key-repeat is enabled by default
///
/// \param Window :  Window object
/// \param Enabled : sfTrue to enable, sfFalse to disable
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_EnableKeyRepeat(sfWindow* Window, sfBool Enabled);

////////////////////////////////////////////////////////////
/// Change the window's icon
///
/// \param Window : Window object
/// \param Width :  Icon's width, in pixels
/// \param Height : Icon's height, in pixels
/// \param Pixels : Pointer to the pixels in memory, format must be RGBA 32 bits
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_SetIcon(sfWindow* Window, unsigned int Width, unsigned int Height, sfUint8* Pixels);

////////////////////////////////////////////////////////////
/// Activate or deactivate a window as the current target for rendering
///
/// \param Window : Window object
/// \param Active : sfTrue to activate, sfFalse to deactivate
///
/// \return True if operation was successful, false otherwise
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfWindow_SetActive(sfWindow* Window, sfBool Active);

////////////////////////////////////////////////////////////
/// Display a window on screen
///
/// \param Window : Window object
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_Display(sfWindow* Window);

////////////////////////////////////////////////////////////
/// Get the input manager of a window
///
/// \param Window : Window object
///
/// \return Reference to the input
///
////////////////////////////////////////////////////////////
CSFML_API sfInput* sfWindow_GetInput(sfWindow* Window);

////////////////////////////////////////////////////////////
/// Limit the framerate to a maximum fixed frequency for a window
///
/// \param Window : Window object
///
/// \param Limit : Framerate limit, in frames per seconds (use 0 to disable limit)
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_SetFramerateLimit(sfWindow* Window, unsigned int Limit);

////////////////////////////////////////////////////////////
/// Get time elapsed since last frame of a window
///
/// \param Window : Window object
///
/// \return Time elapsed, in seconds
///
////////////////////////////////////////////////////////////
CSFML_API float sfWindow_GetFrameTime(sfWindow* Window);

////////////////////////////////////////////////////////////
/// Change the joystick threshold, ie. the value below which
/// no move event will be generated
///
/// \param Window :    Window object
/// \param Threshold : New threshold, in range [0, 100]
///
////////////////////////////////////////////////////////////
CSFML_API void sfWindow_SetJoystickThreshold(sfWindow* Window, float Threshold);


#endif // SFML_WINDOW_H
