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

#ifndef SFML_RENDERWINDOW_H
#define SFML_RENDERWINDOW_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Graphics/Color.h>
#include <SFML/Graphics/Rect.h>
#include <SFML/Graphics/Types.h>
#include <SFML/Window/Event.h>
#include <SFML/Window/VideoMode.h>
#include <SFML/Window/WindowHandle.h>
#include <SFML/Window/Window.h>


////////////////////////////////////////////////////////////
/// Construct a new renderwindow
///
/// \param Mode :   Video mode to use
/// \param Title :  Title of the window
/// \param Style :  Window style
/// \param Params : Creation settings
///
////////////////////////////////////////////////////////////
CSFML_API sfRenderWindow* sfRenderWindow_Create(sfVideoMode Mode, const char* Title, unsigned long Style, sfWindowSettings Params);

////////////////////////////////////////////////////////////
/// Construct a renderwindow from an existing control
///
/// \param Handle : Platform-specific handle of the control
/// \param Params : Creation settings
///
////////////////////////////////////////////////////////////
CSFML_API sfRenderWindow* sfRenderWindow_CreateFromHandle(sfWindowHandle Handle, sfWindowSettings Params);

////////////////////////////////////////////////////////////
/// Destroy an existing renderwindow
///
/// \param RenderWindow : Renderwindow to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_Destroy(sfRenderWindow* RenderWindow);

////////////////////////////////////////////////////////////
/// Close a renderwindow (but doesn't destroy the internal data)
///
/// \param RenderWindow : Renderwindow to close
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_Close(sfRenderWindow* RenderWindow);

////////////////////////////////////////////////////////////
/// Tell whether or not a renderwindow is opened
///
/// \param RenderWindow : Renderwindow object
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfRenderWindow_IsOpened(sfRenderWindow* RenderWindow);

////////////////////////////////////////////////////////////
/// Get the width of the rendering region of a window
///
/// \param RenderWindow : Renderwindow object
///
/// \return Width in pixels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfRenderWindow_GetWidth(sfRenderWindow* RenderWindow);

////////////////////////////////////////////////////////////
/// Get the height of the rendering region of a window
///
/// \param RenderWindow : Renderwindow object
///
/// \return Height in pixels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfRenderWindow_GetHeight(sfRenderWindow* RenderWindow);

////////////////////////////////////////////////////////////
/// Get the creation settings of a window
///
/// \param RenderWindow : Renderwindow object
///
/// \return Settings used to create the window
///
////////////////////////////////////////////////////////////
CSFML_API sfWindowSettings sfRenderWindow_GetSettings(sfRenderWindow* RenderWindow);

////////////////////////////////////////////////////////////
/// Get the event on top of events stack of a window, if any, and pop it
///
/// \param RenderWindow : Renderwindow object
/// \param Event :        Event to fill, if any
///
/// \return sfTrue if an event was returned, sfFalse if events stack was empty
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfRenderWindow_GetEvent(sfRenderWindow* RenderWindow, sfEvent* Event);

////////////////////////////////////////////////////////////
/// Enable / disable vertical synchronization on a window
///
/// \param RenderWindow : Renderwindow object
/// \param Enabled :      sfTrue to enable v-sync, sfFalse to deactivate
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_UseVerticalSync(sfRenderWindow* RenderWindow, sfBool Enabled);

////////////////////////////////////////////////////////////
/// Show or hide the mouse cursor on a window
///
/// \param RenderWindow : RenderWindow object
/// \param Show :   sfTrue to show, sfFalse to hide
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_ShowMouseCursor(sfRenderWindow* RenderWindow, sfBool Show);

////////////////////////////////////////////////////////////
/// Change the position of the mouse cursor on a window
///
/// \param RenderWindow : Renderwindow object
/// \param Left :         Left coordinate of the cursor, relative to the window
/// \param Top :          Top coordinate of the cursor, relative to the window
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_SetCursorPosition(sfRenderWindow* RenderWindow, unsigned int Left, unsigned int Top);

////////////////////////////////////////////////////////////
/// Change the position of a window on screen.
/// Only works for top-level windows
///
/// \param RenderWindow : Renderwindow object
/// \param Left :         Left position
/// \param Top :          Top position
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_SetPosition(sfRenderWindow* RenderWindow, int Left, int Top);

////////////////////////////////////////////////////////////
/// Change the size of the rendering region of a window
///
/// \param RenderWindow : Renderwindow object
/// \param Width :        New Width
/// \param Height :       New Height
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_SetSize(sfRenderWindow* RenderWindow, unsigned int Width, unsigned int Height);

////////////////////////////////////////////////////////////
/// Show or hide a window
///
/// \param RenderWindow : Renderwindow object
/// \param State :        sfTrue to show, sfFalse to hide
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_Show(sfRenderWindow* RenderWindow, sfBool State);

////////////////////////////////////////////////////////////
/// Enable or disable automatic key-repeat for keydown events.
/// Automatic key-repeat is enabled by default
///
/// \param RenderWindow : Renderwindow object
/// \param Enabled :      sfTrue to enable, sfFalse to disable
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_EnableKeyRepeat(sfRenderWindow* RenderWindow, sfBool Enabled);

////////////////////////////////////////////////////////////
/// Change the window's icon
///
/// \param RenderWindow : Renderwindow object
/// \param Width :        Icon's width, in pixels
/// \param Height :       Icon's height, in pixels
/// \param Pixels :       Pointer to the pixels in memory, format must be RGBA 32 bits
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_SetIcon(sfRenderWindow* RenderWindow, unsigned int Width, unsigned int Height, sfUint8* Pixels);

////////////////////////////////////////////////////////////
/// Activate or deactivate a window as the current target for rendering
///
/// \param RenderWindow : Renderwindow object
/// \param Active :       sfTrue to activate, sfFalse to deactivate
///
/// \return True if operation was successful, false otherwise
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfRenderWindow_SetActive(sfRenderWindow* RenderWindow, sfBool Active);

////////////////////////////////////////////////////////////
/// Display a window on screen
///
/// \param RenderWindow : Renderwindow object
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_Display(sfRenderWindow* RenderWindow);

////////////////////////////////////////////////////////////
/// Get the input manager of a window
///
/// \param RenderWindow : Renderwindow object
///
/// \return Reference to the input
///
////////////////////////////////////////////////////////////
CSFML_API sfInput* sfRenderWindow_GetInput(sfRenderWindow* RenderWindow);

////////////////////////////////////////////////////////////
/// Limit the framerate to a maximum fixed frequency for a window
///
/// \param RenderWindow : Renderwindow object
///
/// \param Limit : Framerate limit, in frames per seconds (use 0 to disable limit)
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_SetFramerateLimit(sfRenderWindow* RenderWindow, unsigned int Limit);

////////////////////////////////////////////////////////////
/// Get time elapsed since last frame of a window
///
/// \param RenderWindow : Renderwindow object
///
/// \return Time elapsed, in seconds
///
////////////////////////////////////////////////////////////
CSFML_API float sfRenderWindow_GetFrameTime(sfRenderWindow* RenderWindow);

////////////////////////////////////////////////////////////
/// Change the joystick threshold, ie. the value below which
/// no move event will be generated
///
/// \param RenderWindow : Renderwindow object
/// \param Threshold :    New threshold, in range [0, 100]
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_SetJoystickThreshold(sfRenderWindow* RenderWindow, float Threshold);

////////////////////////////////////////////////////////////
/// Draw something on a renderwindow
///
/// \param RenderWindow :                     Renderwindow to draw in
/// \param PostFX / Sprite / String / shape : Object to draw
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_DrawPostFX(sfRenderWindow* RenderWindow, sfPostFX* PostFX);
CSFML_API void sfRenderWindow_DrawSprite(sfRenderWindow* RenderWindow, sfSprite* Sprite);
CSFML_API void sfRenderWindow_DrawShape (sfRenderWindow* RenderWindow, sfShape*  Shape);
CSFML_API void sfRenderWindow_DrawString(sfRenderWindow* RenderWindow, sfString* String);

////////////////////////////////////////////////////////////
/// Save the content of a renderwindow to an image
///
/// \param RenderWindow : Renderwindow to capture
///
/// \return Image instance containing the contents of the screen
///
////////////////////////////////////////////////////////////
CSFML_API sfImage* sfRenderWindow_Capture(sfRenderWindow* RenderWindow);

////////////////////////////////////////////////////////////
/// Clear the screen with the given color
///
/// \param RenderWindow : Renderwindow to modify
/// \param Color :        Fill color
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_Clear(sfRenderWindow* RenderWindow, sfColor Color);

////////////////////////////////////////////////////////////
/// Change the current active view of a renderwindow
///
/// \param RenderWindow : Renderwindow to modify
/// \param NewView :      Pointer to the new view
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_SetView(sfRenderWindow* RenderWindow, sfView* View);

////////////////////////////////////////////////////////////
/// Get the current active view of a renderwindow
///
/// \param RenderWindow : Renderwindow
///
/// \return Current active view
///
////////////////////////////////////////////////////////////
CSFML_API const sfView* sfRenderWindow_GetView(sfRenderWindow* RenderWindow);

////////////////////////////////////////////////////////////
/// Get the default view of a renderwindow
///
/// \param RenderWindow : Renderwindow
///
/// \return Default view of the render window
///
////////////////////////////////////////////////////////////
CSFML_API sfView* sfRenderWindow_GetDefaultView(sfRenderWindow* RenderWindow);

////////////////////////////////////////////////////////////
/// Convert a point in window coordinates into view coordinates
///
/// \param RenderWindow : Target Renderwindow
/// \param WindowX :      X coordinate of the point to convert, relative to the window
/// \param WindowY :      Y coordinate of the point to convert, relative to the window
/// \param ViewX :        Pointer to fill with the X coordinate of the converted point
/// \param ViewY :        Pointer to fill with the Y coordinate of the converted point
/// \param TargetView :   Target view to convert the point to (pass NULL to use the current view)
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_ConvertCoords(sfRenderWindow* RenderWindow, unsigned int WindowX, unsigned int WindowY, float* ViewX, float* ViewY, sfView* TargetView);

////////////////////////////////////////////////////////////
/// Tell SFML to preserve external OpenGL states, at the expense of
/// more CPU charge. Use this function if you don't want SFML
/// to mess up your own OpenGL states (if any).
/// Don't enable state preservation if not needed, as it will allow
/// SFML to do internal optimizations and improve performances.
/// This parameter is false by default
///
/// \param RenderWindow : Target Renderwindow
/// \param Preserve :     True to preserve OpenGL states, false to let SFML optimize
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_PreserveOpenGLStates(sfRenderWindow* RenderWindow, sfBool Preserve);


#endif // SFML_RENDERWINDOW_H
