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
sfRenderWindow* sfRenderWindow_Create(sfVideoMode mode, const char* title, unsigned long style, sfContextSettings params)
{
    // Convert video mode
    sf::VideoMode videoMode(mode.Width, mode.Height, mode.BitsPerPixel);

    // Create the window
    sfRenderWindow* renderWindow = new sfRenderWindow;
    sf::ContextSettings settings(params.DepthBits, params.StencilBits, params.AntialiasingLevel);
    renderWindow->This.Create(videoMode, title, style, settings);
    renderWindow->Input.This  = &renderWindow->This.GetInput();
    renderWindow->DefaultView = new sfView(const_cast<sf::View*>(&renderWindow->This.GetDefaultView()));
    renderWindow->CurrentView = renderWindow->DefaultView;

    return renderWindow;
}


////////////////////////////////////////////////////////////
/// Construct a renderwindow from an existing control
////////////////////////////////////////////////////////////
sfRenderWindow* sfRenderWindow_CreateFromHandle(sfWindowHandle handle, sfContextSettings params)
{
    sfRenderWindow* renderWindow = new sfRenderWindow;
    sf::ContextSettings settings(params.DepthBits, params.StencilBits, params.AntialiasingLevel);
    renderWindow->This.Create(handle, settings);
    renderWindow->Input.This  = &renderWindow->This.GetInput();
    renderWindow->DefaultView = new sfView(const_cast<sf::View*>(&renderWindow->This.GetDefaultView()));
    renderWindow->CurrentView = renderWindow->DefaultView;

    return renderWindow;
}


////////////////////////////////////////////////////////////
/// Destroy an existing renderwindow
////////////////////////////////////////////////////////////
void sfRenderWindow_Destroy(sfRenderWindow* renderWindow)
{
    delete renderWindow->DefaultView;
    delete renderWindow;
}

////////////////////////////////////////////////////////////
/// Close a renderwindow (but doesn't destroy the internal data)
////////////////////////////////////////////////////////////
void sfRenderWindow_Close(sfRenderWindow* renderWindow)
{
    CSFML_CALL(renderWindow, Close());
}


////////////////////////////////////////////////////////////
/// Tell whether or not a renderwindow is opened
////////////////////////////////////////////////////////////
sfBool sfRenderWindow_IsOpened(sfRenderWindow* renderWindow)
{
    CSFML_CALL_RETURN(renderWindow, IsOpened(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the width of the rendering region of a window
////////////////////////////////////////////////////////////
unsigned int sfRenderWindow_GetWidth(sfRenderWindow* renderWindow)
{
    CSFML_CALL_RETURN(renderWindow, GetWidth(), 0);
}


////////////////////////////////////////////////////////////
/// Get the height of the rendering region of a window
////////////////////////////////////////////////////////////
unsigned int sfRenderWindow_GetHeight(sfRenderWindow* renderWindow)
{
    CSFML_CALL_RETURN(renderWindow, GetHeight(), 0);
}


////////////////////////////////////////////////////////////
/// Get the creation settings of a window
////////////////////////////////////////////////////////////
sfContextSettings sfRenderWindow_GetSettings(sfRenderWindow* renderWindow)
{
    sfContextSettings settings = {0, 0, 0};
    CSFML_CHECK_RETURN(renderWindow, settings);

    const sf::ContextSettings& params = renderWindow->This.GetSettings();
    settings.DepthBits         = params.DepthBits;
    settings.StencilBits       = params.StencilBits;
    settings.AntialiasingLevel = params.AntialiasingLevel;

    return settings;
}



////////////////////////////////////////////////////////////
/// Get the event on top of events stack of a window, if any, and pop it
////////////////////////////////////////////////////////////
sfBool sfRenderWindow_GetEvent(sfRenderWindow* renderWindow, sfEvent* event)
{
    CSFML_CHECK_RETURN(renderWindow, sfFalse);
    CSFML_CHECK_RETURN(event,        sfFalse);

    // Get the event
    sf::Event SFMLEvent;
    sfBool ret = renderWindow->This.GetEvent(SFMLEvent);

    // No event, return
    if (!ret)
        return sfFalse;

    // Convert its type
    event->Type = static_cast<sfEventType>(SFMLEvent.Type);

    // Fill its fields
    switch (event->Type)
    {
        case sfEvtResized :
            event->Size.Width  = SFMLEvent.Size.Width;
            event->Size.Height = SFMLEvent.Size.Height;
            break;

        case sfEvtTextEntered :
            event->Text.Unicode = SFMLEvent.Text.Unicode;
            break;

        case sfEvtKeyReleased :
        case sfEvtKeyPressed :
            event->Key.Code    = static_cast<sfKeyCode>(SFMLEvent.Key.Code);
            event->Key.Alt     = SFMLEvent.Key.Alt     ? sfTrue : sfFalse;
            event->Key.Control = SFMLEvent.Key.Control ? sfTrue : sfFalse;
            event->Key.Shift   = SFMLEvent.Key.Shift   ? sfTrue : sfFalse;
            break;

        case sfEvtMouseWheelMoved :
            event->MouseWheel.Delta = SFMLEvent.MouseWheel.Delta;
            break;

        case sfEvtMouseButtonPressed :
        case sfEvtMouseButtonReleased :
            event->MouseButton.Button = static_cast<sfMouseButton>(SFMLEvent.MouseButton.Button);
            event->MouseButton.X      = SFMLEvent.MouseButton.X;
            event->MouseButton.Y      = SFMLEvent.MouseButton.Y;
            break;

        case sfEvtMouseMoved :
            event->MouseMove.X = SFMLEvent.MouseMove.X;
            event->MouseMove.Y = SFMLEvent.MouseMove.Y;
            break;

        case sfEvtJoyButtonPressed :
        case sfEvtJoyButtonReleased :
            event->JoyButton.JoystickId = SFMLEvent.JoyButton.JoystickId;
            event->JoyButton.Button     = SFMLEvent.JoyButton.Button;
            break;

        case sfEvtJoyMoved :
            event->JoyMove.JoystickId = SFMLEvent.JoyMove.JoystickId;
            event->JoyMove.Axis       = static_cast<sfJoyAxis>(SFMLEvent.JoyMove.Axis);
            event->JoyMove.Position   = SFMLEvent.JoyMove.Position;
            break;

        default :
            break;
    }

    return sfTrue;
}


////////////////////////////////////////////////////////////
/// Enable / disable vertical synchronization on a window
////////////////////////////////////////////////////////////
void sfRenderWindow_UseVerticalSync(sfRenderWindow* renderWindow, sfBool enabled)
{
    CSFML_CALL(renderWindow, UseVerticalSync(enabled == sfTrue));
}


////////////////////////////////////////////////////////////
/// Show or hide the mouse cursor on a window
////////////////////////////////////////////////////////////
void sfRenderWindow_ShowMouseCursor(sfRenderWindow* renderWindow, sfBool show)
{
    CSFML_CALL(renderWindow, ShowMouseCursor(show == sfTrue));
}


////////////////////////////////////////////////////////////
/// Change the position of the mouse cursor on a window
////////////////////////////////////////////////////////////
void sfRenderWindow_SetCursorPosition(sfRenderWindow* renderWindow, unsigned int left, unsigned int top)
{
    CSFML_CALL(renderWindow, SetCursorPosition(left, top));
}


////////////////////////////////////////////////////////////
/// Change the position of a window on screen.
/// Only works for top-level windows
////////////////////////////////////////////////////////////
void sfRenderWindow_SetPosition(sfRenderWindow* renderWindow, int left, int top)
{
    CSFML_CALL(renderWindow, SetPosition(left, top));
}


////////////////////////////////////////////////////////////
/// Change the size of the rendering region of a window
////////////////////////////////////////////////////////////
void sfRenderWindow_SetSize(sfRenderWindow* renderWindow, unsigned int width, unsigned int height)
{
    CSFML_CALL(renderWindow, SetSize(width, height))
}


////////////////////////////////////////////////////////////
/// Show or hide a window
////////////////////////////////////////////////////////////
void sfRenderWindow_Show(sfRenderWindow* renderWindow, sfBool show)
{
    CSFML_CALL(renderWindow, Show(show == sfTrue));
}


////////////////////////////////////////////////////////////
/// Enable or disable automatic key-repeat for keydown events.
/// Automatic key-repeat is enabled by default
////////////////////////////////////////////////////////////
void sfRenderWindow_EnableKeyRepeat(sfRenderWindow* renderWindow, sfBool enabled)
{
    CSFML_CALL(renderWindow, EnableKeyRepeat(enabled == sfTrue));
}


////////////////////////////////////////////////////////////
/// Change the window's icon
////////////////////////////////////////////////////////////
void sfRenderWindow_SetIcon(sfRenderWindow* renderWindow, unsigned int width, unsigned int height, sfUint8* pixels)
{
    CSFML_CALL(renderWindow, SetIcon(width, height, pixels))
}


////////////////////////////////////////////////////////////
/// Set a window as the current target for rendering
////////////////////////////////////////////////////////////
sfBool sfRenderWindow_SetActive(sfRenderWindow* renderWindow, sfBool active)
{
    CSFML_CALL_RETURN(renderWindow, SetActive(active == sfTrue), sfFalse)
}


////////////////////////////////////////////////////////////
/// Display a window on screen
////////////////////////////////////////////////////////////
void sfRenderWindow_Display(sfRenderWindow* renderWindow)
{
    CSFML_CALL(renderWindow, Display());
}


////////////////////////////////////////////////////////////
/// Get the input manager of a window
////////////////////////////////////////////////////////////
sfInput* sfRenderWindow_GetInput(sfRenderWindow* renderWindow)
{
    CSFML_CHECK_RETURN(renderWindow, NULL);

    return &renderWindow->Input;
}


////////////////////////////////////////////////////////////
/// Limit the framerate to a maximum fixed frequency for a window
////////////////////////////////////////////////////////////
void sfRenderWindow_SetFramerateLimit(sfRenderWindow* renderWindow, unsigned int limit)
{
    CSFML_CALL(renderWindow, SetFramerateLimit(limit));
}


////////////////////////////////////////////////////////////
/// Get time elapsed since last frame of a window
////////////////////////////////////////////////////////////
float sfRenderWindow_GetFrameTime(sfRenderWindow* renderWindow)
{
    CSFML_CALL_RETURN(renderWindow, GetFrameTime(), 0.f);
}


////////////////////////////////////////////////////////////
/// Change the joystick threshold, ie. the value below which
/// no move event will be generated
////////////////////////////////////////////////////////////
void sfRenderWindow_SetJoystickThreshold(sfRenderWindow* renderWindow, float threshold)
{
    CSFML_CALL(renderWindow, SetJoystickThreshold(threshold));
}


////////////////////////////////////////////////////////////
/// Draw something on a renderwindow
////////////////////////////////////////////////////////////
void sfRenderWindow_DrawPostFX(sfRenderWindow* renderWindow, sfPostFX* postFX)
{
    CSFML_CHECK(postFX);
    CSFML_CALL(renderWindow, Draw(postFX->This));
}
void sfRenderWindow_DrawShape(sfRenderWindow* renderWindow, sfShape* shape)
{
    CSFML_CHECK(shape);
    CSFML_CALL(renderWindow, Draw(shape->This));
}
void sfRenderWindow_DrawSprite(sfRenderWindow* renderWindow, sfSprite* sprite)
{
    CSFML_CHECK(sprite);
    CSFML_CALL(renderWindow, Draw(sprite->This));
}
void sfRenderWindow_DrawString(sfRenderWindow* renderWindow, sfString* string)
{
    CSFML_CHECK(string);
    CSFML_CALL(renderWindow, Draw(string->This));
}


////////////////////////////////////////////////////////////
/// Save the content of a renderwindow to an image
////////////////////////////////////////////////////////////
sfImage* sfRenderWindow_Capture(sfRenderWindow* renderWindow)
{
    CSFML_CHECK_RETURN(renderWindow, NULL);

    sfImage* image = new sfImage;
    *image->This = renderWindow->This.Capture();

    return image;
}

////////////////////////////////////////////////////////////
/// Clear the screen with the given color
////////////////////////////////////////////////////////////
void sfRenderWindow_Clear(sfRenderWindow* renderWindow, sfColor color)
{
    sf::Color SFMLColor(color.r, color.g, color.b, color.a);

    CSFML_CALL(renderWindow, Clear(SFMLColor));
}


////////////////////////////////////////////////////////////
/// Change the current active view of a renderwindow
////////////////////////////////////////////////////////////
void sfRenderWindow_SetView(sfRenderWindow* renderWindow, sfView* view)
{
    CSFML_CHECK(view);
    CSFML_CALL(renderWindow, SetView(*view->This));
    renderWindow->CurrentView = view;
}


////////////////////////////////////////////////////////////
/// Get the current active view of a renderwindow
////////////////////////////////////////////////////////////
const sfView* sfRenderWindow_GetView(sfRenderWindow* renderWindow)
{
    CSFML_CHECK_RETURN(renderWindow, NULL);

    return renderWindow->CurrentView;
}


////////////////////////////////////////////////////////////
/// Get the default view of a renderwindow
////////////////////////////////////////////////////////////
sfView* sfRenderWindow_GetDefaultView(sfRenderWindow* renderWindow)
{
    CSFML_CHECK_RETURN(renderWindow, NULL);

    return renderWindow->DefaultView;
}


////////////////////////////////////////////////////////////
/// Get the viewport of a view applied to this target
////////////////////////////////////////////////////////////
sfIntRect sfRenderWindow_GetViewport(sfRenderWindow* renderWindow, sfView* view)
{
    sfIntRect rect = {0, 0, 0, 0};
    CSFML_CHECK_RETURN(view, rect);
    CSFML_CHECK_RETURN(renderWindow, rect);

    sf::IntRect SFMLrect = renderWindow->This.GetViewport(*view->This);
    rect.Left   = SFMLrect.Left;
    rect.Top    = SFMLrect.Top;
    rect.Right  = SFMLrect.Right;
    rect.Bottom = SFMLrect.Bottom;

    return rect;
}


////////////////////////////////////////////////////////////
/// Convert a point in window coordinates into view coordinates
////////////////////////////////////////////////////////////
void sfRenderWindow_ConvertCoords(sfRenderWindow* renderWindow, unsigned int windowX, unsigned int windowY, float* viewX, float* viewY, sfView* targetView)
{
    CSFML_CHECK(renderWindow);

    sf::Vector2f point;
    if (targetView)
        point = renderWindow->This.ConvertCoords(windowX, windowY, *targetView->This);
    else
        point = renderWindow->This.ConvertCoords(windowX, windowY);

    if (viewX) *viewX = point.x;
    if (viewY) *viewY = point.y;
}
