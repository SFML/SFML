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
#include <SFML/Graphics/RenderWindow.h>
#include <SFML/Graphics/RenderWindowStruct.h>
#include <SFML/Graphics/ShapeStruct.h>
#include <SFML/Graphics/PostFXStruct.h>
#include <SFML/Graphics/ImageStruct.h>
#include <SFML/Graphics/SpriteStruct.h>
#include <SFML/Graphics/StringStruct.h>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Construct a new renderwindow
////////////////////////////////////////////////////////////
sfRenderWindow* sfRenderWindow_Create(sfVideoMode Mode, const char* Title, unsigned long Style, sfContextSettings Params)
{
    // Convert video mode
    sf::VideoMode VideoMode(Mode.Width, Mode.Height, Mode.BitsPerPixel);

    // Create the window
    sfRenderWindow* RenderWindow = new sfRenderWindow;
    sf::ContextSettings Settings(Params.DepthBits, Params.StencilBits, Params.AntialiasingLevel);
    RenderWindow->This.Create(VideoMode, Title, Style, Settings);
    RenderWindow->Input.This  = &RenderWindow->This.GetInput();
    RenderWindow->DefaultView = new sfView(const_cast<sf::View*>(&RenderWindow->This.GetDefaultView()));
    RenderWindow->CurrentView = RenderWindow->DefaultView;

    return RenderWindow;
}


////////////////////////////////////////////////////////////
/// Construct a renderwindow from an existing control
////////////////////////////////////////////////////////////
sfRenderWindow* sfRenderWindow_CreateFromHandle(sfWindowHandle Handle, sfContextSettings Params)
{
    sfRenderWindow* RenderWindow = new sfRenderWindow;
    sf::ContextSettings Settings(Params.DepthBits, Params.StencilBits, Params.AntialiasingLevel);
    RenderWindow->This.Create(Handle, Settings);
    RenderWindow->Input.This  = &RenderWindow->This.GetInput();
    RenderWindow->DefaultView = new sfView(const_cast<sf::View*>(&RenderWindow->This.GetDefaultView()));
    RenderWindow->CurrentView = RenderWindow->DefaultView;

    return RenderWindow;
}


////////////////////////////////////////////////////////////
/// Destroy an existing renderwindow
////////////////////////////////////////////////////////////
void sfRenderWindow_Destroy(sfRenderWindow* RenderWindow)
{
    delete RenderWindow->DefaultView;
    delete RenderWindow;
}

////////////////////////////////////////////////////////////
/// Close a renderwindow (but doesn't destroy the internal data)
////////////////////////////////////////////////////////////
void sfRenderWindow_Close(sfRenderWindow* RenderWindow)
{
    CSFML_CALL(RenderWindow, Close());
}


////////////////////////////////////////////////////////////
/// Tell whether or not a renderwindow is opened
////////////////////////////////////////////////////////////
sfBool sfRenderWindow_IsOpened(sfRenderWindow* RenderWindow)
{
    CSFML_CALL_RETURN(RenderWindow, IsOpened(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the width of the rendering region of a window
////////////////////////////////////////////////////////////
unsigned int sfRenderWindow_GetWidth(sfRenderWindow* RenderWindow)
{
    CSFML_CALL_RETURN(RenderWindow, GetWidth(), 0);
}


////////////////////////////////////////////////////////////
/// Get the height of the rendering region of a window
////////////////////////////////////////////////////////////
unsigned int sfRenderWindow_GetHeight(sfRenderWindow* RenderWindow)
{
    CSFML_CALL_RETURN(RenderWindow, GetHeight(), 0);
}


////////////////////////////////////////////////////////////
/// Get the creation settings of a window
////////////////////////////////////////////////////////////
sfContextSettings sfRenderWindow_GetSettings(sfRenderWindow* RenderWindow)
{
    sfContextSettings Settings = {0, 0, 0};
    CSFML_CHECK_RETURN(RenderWindow, Settings);

    const sf::ContextSettings& Params = RenderWindow->This.GetSettings();
    Settings.DepthBits         = Params.DepthBits;
    Settings.StencilBits       = Params.StencilBits;
    Settings.AntialiasingLevel = Params.AntialiasingLevel;

    return Settings;
}



////////////////////////////////////////////////////////////
/// Get the event on top of events stack of a window, if any, and pop it
////////////////////////////////////////////////////////////
sfBool sfRenderWindow_GetEvent(sfRenderWindow* RenderWindow, sfEvent* Event)
{
    CSFML_CHECK_RETURN(RenderWindow, sfFalse);
    CSFML_CHECK_RETURN(Event,        sfFalse);

    // Get the event
    sf::Event SFMLEvent;
    sfBool Ret = RenderWindow->This.GetEvent(SFMLEvent);

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
void sfRenderWindow_UseVerticalSync(sfRenderWindow* RenderWindow, sfBool Enabled)
{
    CSFML_CALL(RenderWindow, UseVerticalSync(Enabled == sfTrue));
}


////////////////////////////////////////////////////////////
/// Show or hide the mouse cursor on a window
////////////////////////////////////////////////////////////
void sfRenderWindow_ShowMouseCursor(sfRenderWindow* RenderWindow, sfBool Show)
{
    CSFML_CALL(RenderWindow, ShowMouseCursor(Show == sfTrue));
}


////////////////////////////////////////////////////////////
/// Change the position of the mouse cursor on a window
////////////////////////////////////////////////////////////
void sfRenderWindow_SetCursorPosition(sfRenderWindow* RenderWindow, unsigned int Left, unsigned int Top)
{
    CSFML_CALL(RenderWindow, SetCursorPosition(Left, Top));
}


////////////////////////////////////////////////////////////
/// Change the position of a window on screen.
/// Only works for top-level windows
////////////////////////////////////////////////////////////
void sfRenderWindow_SetPosition(sfRenderWindow* RenderWindow, int Left, int Top)
{
    CSFML_CALL(RenderWindow, SetPosition(Left, Top));
}


////////////////////////////////////////////////////////////
/// Change the size of the rendering region of a window
////////////////////////////////////////////////////////////
void sfRenderWindow_SetSize(sfRenderWindow* RenderWindow, unsigned int Width, unsigned int Height)
{
    CSFML_CALL(RenderWindow, SetSize(Width, Height))
}


////////////////////////////////////////////////////////////
/// Show or hide a window
////////////////////////////////////////////////////////////
void sfRenderWindow_Show(sfRenderWindow* RenderWindow, sfBool State)
{
    CSFML_CALL(RenderWindow, Show(State == sfTrue));
}


////////////////////////////////////////////////////////////
/// Enable or disable automatic key-repeat for keydown events.
/// Automatic key-repeat is enabled by default
////////////////////////////////////////////////////////////
void sfRenderWindow_EnableKeyRepeat(sfRenderWindow* RenderWindow, sfBool Enabled)
{
    CSFML_CALL(RenderWindow, EnableKeyRepeat(Enabled == sfTrue));
}


////////////////////////////////////////////////////////////
/// Change the window's icon
////////////////////////////////////////////////////////////
void sfRenderWindow_SetIcon(sfRenderWindow* RenderWindow, unsigned int Width, unsigned int Height, sfUint8* Pixels)
{
    CSFML_CALL(RenderWindow, SetIcon(Width, Height, Pixels))
}


////////////////////////////////////////////////////////////
/// Set a window as the current target for rendering
////////////////////////////////////////////////////////////
sfBool sfRenderWindow_SetActive(sfRenderWindow* RenderWindow, sfBool Active)
{
    CSFML_CALL_RETURN(RenderWindow, SetActive(Active == sfTrue), sfFalse)
}


////////////////////////////////////////////////////////////
/// Display a window on screen
////////////////////////////////////////////////////////////
void sfRenderWindow_Display(sfRenderWindow* RenderWindow)
{
    CSFML_CALL(RenderWindow, Display());
}


////////////////////////////////////////////////////////////
/// Get the input manager of a window
////////////////////////////////////////////////////////////
sfInput* sfRenderWindow_GetInput(sfRenderWindow* RenderWindow)
{
    CSFML_CHECK_RETURN(RenderWindow, NULL);

    return &RenderWindow->Input;
}


////////////////////////////////////////////////////////////
/// Limit the framerate to a maximum fixed frequency for a window
////////////////////////////////////////////////////////////
void sfRenderWindow_SetFramerateLimit(sfRenderWindow* RenderWindow, unsigned int Limit)
{
    CSFML_CALL(RenderWindow, SetFramerateLimit(Limit));
}


////////////////////////////////////////////////////////////
/// Get time elapsed since last frame of a window
////////////////////////////////////////////////////////////
float sfRenderWindow_GetFrameTime(sfRenderWindow* RenderWindow)
{
    CSFML_CALL_RETURN(RenderWindow, GetFrameTime(), 0.f);
}


////////////////////////////////////////////////////////////
/// Change the joystick threshold, ie. the value below which
/// no move event will be generated
////////////////////////////////////////////////////////////
void sfRenderWindow_SetJoystickThreshold(sfRenderWindow* RenderWindow, float Threshold)
{
    CSFML_CALL(RenderWindow, SetJoystickThreshold(Threshold));
}


////////////////////////////////////////////////////////////
/// Draw something on a renderwindow
////////////////////////////////////////////////////////////
void sfRenderWindow_DrawPostFX(sfRenderWindow* RenderWindow, sfPostFX* PostFX)
{
    CSFML_CHECK(PostFX);
    CSFML_CALL(RenderWindow, Draw(PostFX->This));
}
void sfRenderWindow_DrawShape(sfRenderWindow* RenderWindow, sfShape* Shape)
{
    CSFML_CHECK(Shape);
    CSFML_CALL(RenderWindow, Draw(Shape->This));
}
void sfRenderWindow_DrawSprite(sfRenderWindow* RenderWindow, sfSprite* Sprite)
{
    CSFML_CHECK(Sprite);
    CSFML_CALL(RenderWindow, Draw(Sprite->This));
}
void sfRenderWindow_DrawString(sfRenderWindow* RenderWindow, sfString* String)
{
    CSFML_CHECK(String);
    CSFML_CALL(RenderWindow, Draw(String->This));
}


////////////////////////////////////////////////////////////
/// Save the content of a renderwindow to an image
////////////////////////////////////////////////////////////
sfImage* sfRenderWindow_Capture(sfRenderWindow* RenderWindow)
{
    CSFML_CHECK_RETURN(RenderWindow, NULL);

    sfImage* Image = new sfImage;
    *Image->This = RenderWindow->This.Capture();

    return Image;
}

////////////////////////////////////////////////////////////
/// Clear the screen with the given color
////////////////////////////////////////////////////////////
void sfRenderWindow_Clear(sfRenderWindow* RenderWindow, sfColor Color)
{
    sf::Color SFMLColor(Color.r, Color.g, Color.b, Color.a);

    CSFML_CALL(RenderWindow, Clear(SFMLColor));
}


////////////////////////////////////////////////////////////
/// Change the current active view of a renderwindow
////////////////////////////////////////////////////////////
void sfRenderWindow_SetView(sfRenderWindow* RenderWindow, sfView* View)
{
    CSFML_CHECK(View);
    CSFML_CALL(RenderWindow, SetView(*View->This));
    RenderWindow->CurrentView = View;
}


////////////////////////////////////////////////////////////
/// Get the current active view of a renderwindow
////////////////////////////////////////////////////////////
const sfView* sfRenderWindow_GetView(sfRenderWindow* RenderWindow)
{
    CSFML_CHECK_RETURN(RenderWindow, NULL);

    return RenderWindow->CurrentView;
}


////////////////////////////////////////////////////////////
/// Get the default view of a renderwindow
////////////////////////////////////////////////////////////
sfView* sfRenderWindow_GetDefaultView(sfRenderWindow* RenderWindow)
{
    CSFML_CHECK_RETURN(RenderWindow, NULL);

    return RenderWindow->DefaultView;
}


////////////////////////////////////////////////////////////
/// Convert a point in window coordinates into view coordinates
////////////////////////////////////////////////////////////
void sfRenderWindow_ConvertCoords(sfRenderWindow* RenderWindow, unsigned int WindowX, unsigned int WindowY, float* ViewX, float* ViewY, sfView* TargetView)
{
    CSFML_CHECK(RenderWindow);

    sf::Vector2f Point;
    if (TargetView)
        Point = RenderWindow->This.ConvertCoords(WindowX, WindowY, *TargetView->This);
    else
        Point = RenderWindow->This.ConvertCoords(WindowX, WindowY);

    if (ViewX) *ViewX = Point.x;
    if (ViewY) *ViewY = Point.y;
}


////////////////////////////////////////////////////////////
/// Tell SFML to preserve external OpenGL states, at the expense of
/// more CPU charge. Use this function if you don't want SFML
/// to mess up your own OpenGL states (if any).
/// Don't enable state preservation if not needed, as it will allow
/// SFML to do internal optimizations and improve performances.
/// This parameter is false by default
////////////////////////////////////////////////////////////
void sfRenderWindow_PreserveOpenGLStates(sfRenderWindow* RenderWindow, sfBool Preserve)
{
    CSFML_CALL(RenderWindow, PreserveOpenGLStates(Preserve == sfTrue));
}
