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


////////////////////////////////////////////////////////////
/// Construct a new window
////////////////////////////////////////////////////////////
sfWindow* sfWindow_Create(sfVideoMode Mode, const char* Title, unsigned long Style, sfContextSettings Params)
{
    // Convert video mode
    sf::VideoMode VideoMode(Mode.Width, Mode.Height, Mode.BitsPerPixel);

    // Create the window
    sfWindow* Window = new sfWindow;
    sf::ContextSettings Settings(Params.DepthBits, Params.StencilBits, Params.AntialiasingLevel);
    Window->This.Create(VideoMode, Title, Style, Settings);
    Window->Input.This = &Window->This.GetInput();

    return Window;
}


////////////////////////////////////////////////////////////
/// Construct a window from an existing control
////////////////////////////////////////////////////////////
sfWindow* sfWindow_CreateFromHandle(sfWindowHandle Handle, sfContextSettings Params)
{
    sfWindow* Window = new sfWindow;
    sf::ContextSettings Settings(Params.DepthBits, Params.StencilBits, Params.AntialiasingLevel);
    Window->This.Create(Handle, Settings);
    Window->Input.This = &Window->This.GetInput();

    return Window;
}


////////////////////////////////////////////////////////////
/// Destroy an existing window
////////////////////////////////////////////////////////////
void sfWindow_Destroy(sfWindow* Window)
{
    delete Window;
}

////////////////////////////////////////////////////////////
/// Close a window (but doesn't destroy the internal data)
////////////////////////////////////////////////////////////
void sfWindow_Close(sfWindow* Window)
{
    CSFML_CALL(Window, Close());
}


////////////////////////////////////////////////////////////
/// Tell whether or not a window is opened
////////////////////////////////////////////////////////////
sfBool sfWindow_IsOpened(sfWindow* Window)
{
    CSFML_CALL_RETURN(Window, IsOpened(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the width of the rendering region of a window
////////////////////////////////////////////////////////////
unsigned int sfWindow_GetWidth(sfWindow* Window)
{
    CSFML_CALL_RETURN(Window, GetWidth(), 0)
}


////////////////////////////////////////////////////////////
/// Get the height of the rendering region of a window
////////////////////////////////////////////////////////////
unsigned int sfWindow_GetHeight(sfWindow* Window)
{
    CSFML_CALL_RETURN(Window, GetHeight(), 0)
}


////////////////////////////////////////////////////////////
/// Get the creation settings of a window
////////////////////////////////////////////////////////////
sfContextSettings sfWindow_GetSettings(sfWindow* Window)
{
    sfContextSettings Settings = {0, 0, 0};
    CSFML_CHECK_RETURN(Window, Settings);

    const sf::ContextSettings& Params = Window->This.GetSettings();
    Settings.DepthBits         = Params.DepthBits;
    Settings.StencilBits       = Params.StencilBits;
    Settings.AntialiasingLevel = Params.AntialiasingLevel;

    return Settings;
}


////////////////////////////////////////////////////////////
/// Get the event on top of events stack of a window, if any, and pop it
////////////////////////////////////////////////////////////
sfBool sfWindow_GetEvent(sfWindow* Window, sfEvent* Event)
{
    CSFML_CHECK_RETURN(Window, sfFalse);
    CSFML_CHECK_RETURN(Event, sfFalse);

    // Get the event
    sf::Event SFMLEvent;
    sfBool Ret = Window->This.GetEvent(SFMLEvent);

    // No event, return
    if (!Ret)
        return sfFalse;

    // Convert its type
    Event->Type = static_cast<sfEventType>(SFMLEvent.Type);

    // Fill its fields
    switch (Event->Type)
    {
        case sfEvtResized :
            Event->Size.Width  = SFMLEvent.Size.Width;
            Event->Size.Height = SFMLEvent.Size.Height;
            break;

        case sfEvtTextEntered :
            Event->Text.Unicode = SFMLEvent.Text.Unicode;
            break;

        case sfEvtKeyReleased :
        case sfEvtKeyPressed :
            Event->Key.Code    = static_cast<sfKeyCode>(SFMLEvent.Key.Code);
            Event->Key.Alt     = SFMLEvent.Key.Alt     ? sfTrue : sfFalse;
            Event->Key.Control = SFMLEvent.Key.Control ? sfTrue : sfFalse;
            Event->Key.Shift   = SFMLEvent.Key.Shift   ? sfTrue : sfFalse;
            break;

        case sfEvtMouseWheelMoved :
            Event->MouseWheel.Delta = SFMLEvent.MouseWheel.Delta;
            break;

        case sfEvtMouseButtonPressed :
        case sfEvtMouseButtonReleased :
            Event->MouseButton.Button = static_cast<sfMouseButton>(SFMLEvent.MouseButton.Button);
            Event->MouseButton.X      = SFMLEvent.MouseButton.X;
            Event->MouseButton.Y      = SFMLEvent.MouseButton.Y;
            break;

        case sfEvtMouseMoved :
            Event->MouseMove.X = SFMLEvent.MouseMove.X;
            Event->MouseMove.Y = SFMLEvent.MouseMove.Y;
            break;

        case sfEvtJoyButtonPressed :
        case sfEvtJoyButtonReleased :
            Event->JoyButton.JoystickId = SFMLEvent.JoyButton.JoystickId;
            Event->JoyButton.Button     = SFMLEvent.JoyButton.Button;
            break;

        case sfEvtJoyMoved :
            Event->JoyMove.JoystickId = SFMLEvent.JoyMove.JoystickId;
            Event->JoyMove.Axis       = static_cast<sfJoyAxis>(SFMLEvent.JoyMove.Axis);
            Event->JoyMove.Position   = SFMLEvent.JoyMove.Position;
            break;

        default :
            break;
    }

    return sfTrue;
}


////////////////////////////////////////////////////////////
/// Enable / disable vertical synchronization on a window
////////////////////////////////////////////////////////////
void sfWindow_UseVerticalSync(sfWindow* Window, sfBool Enabled)
{
    CSFML_CALL(Window, UseVerticalSync(Enabled == sfTrue))
}


////////////////////////////////////////////////////////////
/// Show or hide the mouse cursor on a window
////////////////////////////////////////////////////////////
void sfWindow_ShowMouseCursor(sfWindow* Window, sfBool Show)
{
    CSFML_CALL(Window, ShowMouseCursor(Show == sfTrue))
}


////////////////////////////////////////////////////////////
/// Change the position of the mouse cursor on a window
////////////////////////////////////////////////////////////
void sfWindow_SetCursorPosition(sfWindow* Window, unsigned int Left, unsigned int Top)
{
    CSFML_CALL(Window, SetCursorPosition(Left, Top))
}


////////////////////////////////////////////////////////////
/// Change the position of a window on screen.
/// Only works for top-level windows
////////////////////////////////////////////////////////////
void sfWindow_SetPosition(sfWindow* Window, int Left, int Top)
{
    CSFML_CALL(Window, SetPosition(Left, Top))
}


////////////////////////////////////////////////////////////
/// Change the size of the rendering region of a window
////////////////////////////////////////////////////////////
void sfWindow_SetSize(sfWindow* Window, unsigned int Width, unsigned int Height)
{
    CSFML_CALL(Window, SetSize(Width, Height))
}


////////////////////////////////////////////////////////////
/// Show or hide a window
////////////////////////////////////////////////////////////
void sfWindow_Show(sfWindow* Window, sfBool State)
{
    CSFML_CALL(Window, Show(State == sfTrue))
}


////////////////////////////////////////////////////////////
/// Enable or disable automatic key-repeat for keydown events.
/// Automatic key-repeat is enabled by default
////////////////////////////////////////////////////////////
void sfWindow_EnableKeyRepeat(sfWindow* Window, sfBool Enabled)
{
    CSFML_CALL(Window, EnableKeyRepeat(Enabled == sfTrue))
}


////////////////////////////////////////////////////////////
/// Change the window's icon
////////////////////////////////////////////////////////////
void sfWindow_SetIcon(sfWindow* Window, unsigned int Width, unsigned int Height, sfUint8* Pixels)
{
    CSFML_CALL(Window, SetIcon(Width, Height, Pixels))
}


////////////////////////////////////////////////////////////
/// Activate or deactivate a window as the current target for rendering
////////////////////////////////////////////////////////////
sfBool sfWindow_SetActive(sfWindow* Window, sfBool Active)
{
    CSFML_CALL_RETURN(Window, SetActive(Active == sfTrue), sfFalse)
}


////////////////////////////////////////////////////////////
/// Display a window on screen
////////////////////////////////////////////////////////////
void sfWindow_Display(sfWindow* Window)
{
    CSFML_CALL(Window, Display())
}


////////////////////////////////////////////////////////////
/// Get the input manager of a window
////////////////////////////////////////////////////////////
sfInput* sfWindow_GetInput(sfWindow* Window)
{
    CSFML_CHECK_RETURN(Window, NULL);

    return &Window->Input;
}


////////////////////////////////////////////////////////////
/// Limit the framerate to a maximum fixed frequency for a window
////////////////////////////////////////////////////////////
void sfWindow_SetFramerateLimit(sfWindow* Window, unsigned int Limit)
{
    CSFML_CALL(Window, SetFramerateLimit(Limit))
}


////////////////////////////////////////////////////////////
/// Get time elapsed since last frame of a window
////////////////////////////////////////////////////////////
float sfWindow_GetFrameTime(sfWindow* Window)
{
    CSFML_CALL_RETURN(Window, GetFrameTime(), 0.f)
}

////////////////////////////////////////////////////////////
/// Change the joystick threshold, ie. the value below which
/// no move event will be generated
///
/// \param Threshold : New threshold, in range [0, 100]
///
////////////////////////////////////////////////////////////
void sfWindow_SetJoystickThreshold(sfWindow* Window, float Threshold)
{
    CSFML_CALL(Window, SetJoystickThreshold(Threshold))
}
