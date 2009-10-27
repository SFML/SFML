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
#include <SFML/Window/Window.h>
#include <SFML/Window/WindowStruct.h>
#include <SFML/Internal.h>
#include <SFML/ConvertEvent.h>


////////////////////////////////////////////////////////////
/// Construct a new window
////////////////////////////////////////////////////////////
sfWindow* sfWindow_Create(sfVideoMode mode, const char* title, unsigned long style, sfContextSettings settings)
{
    // Convert video mode
    sf::VideoMode videoMode(mode.Width, mode.Height, mode.BitsPerPixel);

    // Create the window
    sfWindow* window = new sfWindow;
    sf::ContextSettings params(settings.DepthBits, settings.StencilBits, settings.AntialiasingLevel);
    window->This.Create(videoMode, title, style, params);
    window->Input.This = &window->This.GetInput();

    return window;
}


////////////////////////////////////////////////////////////
/// Construct a window from an existing control
////////////////////////////////////////////////////////////
sfWindow* sfWindow_CreateFromHandle(sfWindowHandle handle, sfContextSettings settings)
{
    sfWindow* window = new sfWindow;
    sf::ContextSettings params(settings.DepthBits, settings.StencilBits, settings.AntialiasingLevel);
    window->This.Create(handle, params);
    window->Input.This = &window->This.GetInput();

    return window;
}


////////////////////////////////////////////////////////////
/// Destroy an existing window
////////////////////////////////////////////////////////////
void sfWindow_Destroy(sfWindow* window)
{
    delete window;
}

////////////////////////////////////////////////////////////
/// Close a window (but doesn't destroy the internal data)
////////////////////////////////////////////////////////////
void sfWindow_Close(sfWindow* window)
{
    CSFML_CALL(window, Close());
}


////////////////////////////////////////////////////////////
/// Tell whether or not a window is opened
////////////////////////////////////////////////////////////
sfBool sfWindow_IsOpened(sfWindow* window)
{
    CSFML_CALL_RETURN(window, IsOpened(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the width of the rendering region of a window
////////////////////////////////////////////////////////////
unsigned int sfWindow_GetWidth(sfWindow* window)
{
    CSFML_CALL_RETURN(window, GetWidth(), 0)
}


////////////////////////////////////////////////////////////
/// Get the height of the rendering region of a window
////////////////////////////////////////////////////////////
unsigned int sfWindow_GetHeight(sfWindow* window)
{
    CSFML_CALL_RETURN(window, GetHeight(), 0)
}


////////////////////////////////////////////////////////////
/// Get the creation settings of a window
////////////////////////////////////////////////////////////
sfContextSettings sfWindow_GetSettings(sfWindow* window)
{
    sfContextSettings settings = {0, 0, 0};
    CSFML_CHECK_RETURN(window, settings);

    const sf::ContextSettings& params = window->This.GetSettings();
    settings.DepthBits         = params.DepthBits;
    settings.StencilBits       = params.StencilBits;
    settings.AntialiasingLevel = params.AntialiasingLevel;

    return settings;
}


////////////////////////////////////////////////////////////
/// Get the event on top of events stack of a window, if any, and pop it
////////////////////////////////////////////////////////////
sfBool sfWindow_GetEvent(sfWindow* window, sfEvent* event)
{
    CSFML_CHECK_RETURN(window, sfFalse);
    CSFML_CHECK_RETURN(event, sfFalse);

    // Get the event
    sf::Event SFMLEvent;
    sfBool ret = window->This.GetEvent(SFMLEvent);

    // No event, return
    if (!ret)
        return sfFalse;

    // Convert the sf::Event event to a sfEvent
    ConvertEvent(SFMLEvent, event);

    return sfTrue;
}


////////////////////////////////////////////////////////////
/// Wait for an event and return it
////////////////////////////////////////////////////////////
sfBool sfWindow_WaitEvent(sfWindow* window, sfEvent* event)
{
    CSFML_CHECK_RETURN(window, sfFalse);
    CSFML_CHECK_RETURN(event, sfFalse);

    // Get the event
    sf::Event SFMLEvent;
    sfBool ret = window->This.WaitEvent(SFMLEvent);

    // Error, return
    if (!ret)
        return sfFalse;

    // Convert the sf::Event event to a sfEvent
    ConvertEvent(SFMLEvent, event);

    return sfTrue;
}


////////////////////////////////////////////////////////////
/// Enable / disable vertical synchronization on a window
////////////////////////////////////////////////////////////
void sfWindow_UseVerticalSync(sfWindow* window, sfBool enabled)
{
    CSFML_CALL(window, UseVerticalSync(enabled == sfTrue))
}


////////////////////////////////////////////////////////////
/// Show or hide the mouse cursor on a window
////////////////////////////////////////////////////////////
void sfWindow_ShowMouseCursor(sfWindow* window, sfBool show)
{
    CSFML_CALL(window, ShowMouseCursor(show == sfTrue))
}


////////////////////////////////////////////////////////////
/// Change the position of the mouse cursor on a window
////////////////////////////////////////////////////////////
void sfWindow_SetCursorPosition(sfWindow* window, unsigned int left, unsigned int top)
{
    CSFML_CALL(window, SetCursorPosition(left, top))
}


////////////////////////////////////////////////////////////
/// Change the position of a window on screen.
/// Only works for top-level windows
////////////////////////////////////////////////////////////
void sfWindow_SetPosition(sfWindow* window, int left, int top)
{
    CSFML_CALL(window, SetPosition(left, top))
}


////////////////////////////////////////////////////////////
/// Change the size of the rendering region of a window
////////////////////////////////////////////////////////////
void sfWindow_SetSize(sfWindow* window, unsigned int width, unsigned int height)
{
    CSFML_CALL(window, SetSize(width, height))
}


////////////////////////////////////////////////////////////
/// Show or hide a window
////////////////////////////////////////////////////////////
void sfWindow_Show(sfWindow* window, sfBool show)
{
    CSFML_CALL(window, Show(show == sfTrue))
}


////////////////////////////////////////////////////////////
/// Enable or disable automatic key-repeat for keydown events.
/// Automatic key-repeat is enabled by default
////////////////////////////////////////////////////////////
void sfWindow_EnableKeyRepeat(sfWindow* window, sfBool enabled)
{
    CSFML_CALL(window, EnableKeyRepeat(enabled == sfTrue))
}


////////////////////////////////////////////////////////////
/// Change the window's icon
////////////////////////////////////////////////////////////
void sfWindow_SetIcon(sfWindow* window, unsigned int width, unsigned int height, sfUint8* pixels)
{
    CSFML_CALL(window, SetIcon(width, height, pixels))
}


////////////////////////////////////////////////////////////
/// Activate or deactivate a window as the current target for rendering
////////////////////////////////////////////////////////////
sfBool sfWindow_SetActive(sfWindow* window, sfBool active)
{
    CSFML_CALL_RETURN(window, SetActive(active == sfTrue), sfFalse)
}


////////////////////////////////////////////////////////////
/// Display a window on screen
////////////////////////////////////////////////////////////
void sfWindow_Display(sfWindow* window)
{
    CSFML_CALL(window, Display())
}


////////////////////////////////////////////////////////////
/// Get the input manager of a window
////////////////////////////////////////////////////////////
sfInput* sfWindow_GetInput(sfWindow* window)
{
    CSFML_CHECK_RETURN(window, NULL);

    return &window->Input;
}


////////////////////////////////////////////////////////////
/// Limit the framerate to a maximum fixed frequency for a window
////////////////////////////////////////////////////////////
void sfWindow_SetFramerateLimit(sfWindow* window, unsigned int limit)
{
    CSFML_CALL(window, SetFramerateLimit(limit))
}


////////////////////////////////////////////////////////////
/// Get time elapsed since last frame of a window
////////////////////////////////////////////////////////////
float sfWindow_GetFrameTime(sfWindow* window)
{
    CSFML_CALL_RETURN(window, GetFrameTime(), 0.f)
}

////////////////////////////////////////////////////////////
/// Change the joystick threshold, ie. the value below which
/// no move event will be generated
////////////////////////////////////////////////////////////
void sfWindow_SetJoystickThreshold(sfWindow* window, float threshold)
{
    CSFML_CALL(window, SetJoystickThreshold(threshold))
}
