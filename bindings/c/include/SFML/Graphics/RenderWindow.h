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
/// \param mode :     Video mode to use
/// \param title :    Title of the window
/// \param style :    Window style
/// \param settings : Creation settings (pass NULL to use default values)
///
////////////////////////////////////////////////////////////
CSFML_API sfRenderWindow* sfRenderWindow_Create(sfVideoMode mode, const char* title, unsigned long style, const sfContextSettings* settings);

////////////////////////////////////////////////////////////
/// Construct a renderwindow from an existing control
///
/// \param handle :   Platform-specific handle of the control
/// \param settings : Creation settings (pass NULL to use default values)
///
////////////////////////////////////////////////////////////
CSFML_API sfRenderWindow* sfRenderWindow_CreateFromHandle(sfWindowHandle handle, const sfContextSettings* settings);

////////////////////////////////////////////////////////////
/// Destroy an existing renderwindow
///
/// \param renderWindow : Renderwindow to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_Destroy(sfRenderWindow* renderWindow);

////////////////////////////////////////////////////////////
/// Close a renderwindow (but doesn't destroy the internal data)
///
/// \param renderWindow : Renderwindow to close
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_Close(sfRenderWindow* renderWindow);

////////////////////////////////////////////////////////////
/// Tell whether or not a renderwindow is opened
///
/// \param renderWindow : Renderwindow object
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfRenderWindow_IsOpened(const sfRenderWindow* renderWindow);

////////////////////////////////////////////////////////////
/// Get the width of the rendering region of a window
///
/// \param renderWindow : Renderwindow object
///
/// \return Width in pixels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfRenderWindow_GetWidth(const sfRenderWindow* renderWindow);

////////////////////////////////////////////////////////////
/// Get the height of the rendering region of a window
///
/// \param renderWindow : Renderwindow object
///
/// \return Height in pixels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfRenderWindow_GetHeight(const sfRenderWindow* renderWindow);

////////////////////////////////////////////////////////////
/// Get the creation settings of a window
///
/// \param renderWindow : Renderwindow object
///
/// \return Settings used to create the window
///
////////////////////////////////////////////////////////////
CSFML_API sfContextSettings sfRenderWindow_GetSettings(const sfRenderWindow* renderWindow);

////////////////////////////////////////////////////////////
/// Get the event on top of events stack of a window, if any, and pop it
///
/// \param renderWindow : Renderwindow object
/// \param event :        Event to fill, if any
///
/// \return sfTrue if an event was returned, sfFalse if events stack was empty
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfRenderWindow_GetEvent(sfRenderWindow* renderWindow, sfEvent* event);

////////////////////////////////////////////////////////////
/// Wait for an event and return it
///
/// \param renderWindow : Renderwindow object
/// \param event :        Event to fill
///
/// \return sfFalse if an error occured
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfRenderWindow_WaitEvent(sfRenderWindow* renderWindow, sfEvent* event);

////////////////////////////////////////////////////////////
/// Enable / disable vertical synchronization on a window
///
/// \param renderWindow : Renderwindow object
/// \param enabled :      sfTrue to enable v-sync, sfFalse to deactivate
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_EnableVerticalSync(sfRenderWindow* renderWindow, sfBool enabled);

////////////////////////////////////////////////////////////
/// Show or hide the mouse cursor on a window
///
/// \param renderWindow : RenderWindow object
/// \param show :         sfTrue to show, sfFalse to hide
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_ShowMouseCursor(sfRenderWindow* renderWindow, sfBool show);

////////////////////////////////////////////////////////////
/// Change the position of the mouse cursor on a window
///
/// \param renderWindow : Renderwindow object
/// \param left :         Left coordinate of the cursor, relative to the window
/// \param top :          Top coordinate of the cursor, relative to the window
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_SetCursorPosition(sfRenderWindow* renderWindow, unsigned int left, unsigned int top);

////////////////////////////////////////////////////////////
/// Change the position of a window on screen.
/// Only works for top-level windows
///
/// \param renderWindow : Renderwindow object
/// \param left :         Left position
/// \param top :          Top position
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_SetPosition(sfRenderWindow* renderWindow, int left, int top);

////////////////////////////////////////////////////////////
/// Change the size of the rendering region of a window
///
/// \param renderWindow : Renderwindow object
/// \param width :        New Width
/// \param height :       New Height
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_SetSize(sfRenderWindow* renderWindow, unsigned int width, unsigned int height);

////////////////////////////////////////////////////////////
/// Show or hide a window
///
/// \param renderWindow : Renderwindow object
/// \param show :         sfTrue to show, sfFalse to hide
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_Show(sfRenderWindow* renderWindow, sfBool show);

////////////////////////////////////////////////////////////
/// Enable or disable automatic key-repeat for keydown events.
/// Automatic key-repeat is enabled by default
///
/// \param renderWindow : Renderwindow object
/// \param enabled :      sfTrue to enable, sfFalse to disable
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_EnableKeyRepeat(sfRenderWindow* renderWindow, sfBool enabled);

////////////////////////////////////////////////////////////
/// Change the window's icon
///
/// \param renderWindow : Renderwindow object
/// \param width :        Icon's width, in pixels
/// \param height :       Icon's height, in pixels
/// \param pixels :       Pointer to the pixels in memory, format must be RGBA 32 bits
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_SetIcon(sfRenderWindow* renderWindow, unsigned int width, unsigned int height, const sfUint8* pixels);

////////////////////////////////////////////////////////////
/// Activate or deactivate a window as the current target for rendering
///
/// \param renderWindow : Renderwindow object
/// \param active :       sfTrue to activate, sfFalse to deactivate
///
/// \return True if operation was successful, false otherwise
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfRenderWindow_SetActive(sfRenderWindow* renderWindow, sfBool active);

////////////////////////////////////////////////////////////
/// Save the current OpenGL render states and matrices
///
/// \param renderWindow : Renderwindow object
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_SaveGLStates(sfRenderWindow* renderWindow);

////////////////////////////////////////////////////////////
/// Restore the previously saved OpenGL render states and matrices
///
/// \param renderWindow : Renderwindow object
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_RestoreGLStates(sfRenderWindow* renderWindow);

////////////////////////////////////////////////////////////
/// Display a window on screen
///
/// \param renderWindow : Renderwindow object
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_Display(sfRenderWindow* renderWindow);

////////////////////////////////////////////////////////////
/// Get the input manager of a window
///
/// \param renderWindow : Renderwindow object
///
/// \return Reference to the input
///
////////////////////////////////////////////////////////////
CSFML_API const sfInput* sfRenderWindow_GetInput(const sfRenderWindow* renderWindow);

////////////////////////////////////////////////////////////
/// Limit the framerate to a maximum fixed frequency for a window
///
/// \param renderWindow : Renderwindow object
/// \param limit :        Framerate limit, in frames per seconds (use 0 to disable limit)
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_SetFramerateLimit(sfRenderWindow* renderWindow, unsigned int limit);

////////////////////////////////////////////////////////////
/// Get time elapsed since last frame of a window
///
/// \param renderWindow : Renderwindow object
///
/// \return Time elapsed, in seconds
///
////////////////////////////////////////////////////////////
CSFML_API float sfRenderWindow_GetFrameTime(const sfRenderWindow* renderWindow);

////////////////////////////////////////////////////////////
/// Change the joystick threshold, ie. the value below which
/// no move event will be generated
///
/// \param renderWindow : Renderwindow object
/// \param threshold :    New threshold, in range [0, 100]
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_SetJoystickThreshold(sfRenderWindow* renderWindow, float threshold);

////////////////////////////////////////////////////////////
/// Retrieve the Os-specific handle of a window
///
/// \param renderWindow : Renderwindow object
///
/// \return Window handle
///
////////////////////////////////////////////////////////////
CSFML_API sfWindowHandle sfRenderWindow_GetSystemHandle(const sfRenderWindow* renderWindow);

////////////////////////////////////////////////////////////
/// Draw something on a renderwindow
///
/// \param renderWindow :          Renderwindow to draw in
/// \param sprite / text / shape : Object to draw
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_DrawSprite(sfRenderWindow* renderWindow, const sfSprite* sprite);
CSFML_API void sfRenderWindow_DrawShape (sfRenderWindow* renderWindow, const sfShape*  shape);
CSFML_API void sfRenderWindow_DrawText  (sfRenderWindow* renderWindow, const sfText*   text);

////////////////////////////////////////////////////////////
/// Draw something on a renderwindow with a shader
///
/// \param renderWindow :          Renderwindow to draw in
/// \param sprite / text / shape : Object to draw
/// \param shader :                Shader to use
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_DrawSpriteWithShader(sfRenderWindow* renderWindow, const sfSprite* sprite, const sfShader* shader);
CSFML_API void sfRenderWindow_DrawShapeWithShader (sfRenderWindow* renderWindow, const sfShape*  shape,  const sfShader* shader);
CSFML_API void sfRenderWindow_DrawTextWithShader  (sfRenderWindow* renderWindow, const sfText*   text,   const sfShader* shader);

////////////////////////////////////////////////////////////
/// Clear the screen with the given color
///
/// \param renderWindow : Renderwindow to modify
/// \param color :        Fill color
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_Clear(sfRenderWindow* renderWindow, sfColor color);

////////////////////////////////////////////////////////////
/// Change the current active view of a renderwindow
///
/// \param renderWindow : Renderwindow to modify
/// \param view :         Pointer to the new view
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_SetView(sfRenderWindow* renderWindow, const sfView* view);

////////////////////////////////////////////////////////////
/// Get the current active view of a renderwindow
///
/// \param renderWindow : Renderwindow
///
/// \return Current active view
///
////////////////////////////////////////////////////////////
CSFML_API const sfView* sfRenderWindow_GetView(const sfRenderWindow* renderWindow);

////////////////////////////////////////////////////////////
/// Get the default view of a renderwindow
///
/// \param renderWindow : Renderwindow
///
/// \return Default view of the render window
///
////////////////////////////////////////////////////////////
CSFML_API const sfView* sfRenderWindow_GetDefaultView(const sfRenderWindow* renderWindow);

////////////////////////////////////////////////////////////
/// Get the viewport of a view applied to this target
///
/// \param renderWindow : Renderwindow
/// \param view :         Target view
///
/// \return Viewport rectangle, expressed in pixels in the current target
///
////////////////////////////////////////////////////////////
CSFML_API sfIntRect sfRenderWindow_GetViewport(const sfRenderWindow* renderWindow, const sfView* view);

////////////////////////////////////////////////////////////
/// Convert a point in window coordinates into view coordinates
///
/// \param renderWindow : Target Renderwindow
/// \param windowX :      X coordinate of the point to convert, relative to the window
/// \param windowY :      Y coordinate of the point to convert, relative to the window
/// \param viewX :        Pointer to fill with the X coordinate of the converted point
/// \param viewY :        Pointer to fill with the Y coordinate of the converted point
/// \param targetView :   Target view to convert the point to (pass NULL to use the current view)
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderWindow_ConvertCoords(const sfRenderWindow* renderWindow, unsigned int windowX, unsigned int windowY, float* viewX, float* viewY, const sfView* targetView);


#endif // SFML_RENDERWINDOW_H
