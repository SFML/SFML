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

#ifndef SFML_VIEW_H
#define SFML_VIEW_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Graphics/Rect.h>
#include <SFML/Graphics/Types.h>


////////////////////////////////////////////////////////////
/// Construct a default view (1000x1000)
///
////////////////////////////////////////////////////////////
CSFML_API sfView* sfView_Create(void);

////////////////////////////////////////////////////////////
/// Construct a view from a rectangle
///
/// \param rectangle : Rectangle defining the bounds of the view
///
////////////////////////////////////////////////////////////
CSFML_API sfView* sfView_CreateFromRect(sfFloatRect rectangle);

////////////////////////////////////////////////////////////
/// Copy an existing view
///
/// \param view : View to copy
///
/// \return Copied object
///
////////////////////////////////////////////////////////////
CSFML_API sfView* sfView_Copy(sfView* view);

////////////////////////////////////////////////////////////
/// Destroy an existing view
///
/// \param view : View to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfView_Destroy(sfView* view);

////////////////////////////////////////////////////////////
/// Change the center of a view
///
/// \param view : View to modify
/// \param x :    X coordinate of the new center
/// \param y :    Y coordinate of the new center
///
////////////////////////////////////////////////////////////
CSFML_API void sfView_SetCenter(sfView* view, float x, float y);

////////////////////////////////////////////////////////////
/// Change the size of a view
///
/// \param view :   View to modify
/// \param width :  New width
/// \param height : New height
///
////////////////////////////////////////////////////////////
CSFML_API void sfView_SetSize(sfView* view, float width, float height);

////////////////////////////////////////////////////////////
/// Set the angle of rotation of a view
///
/// \param view :  View to modify
/// \param angle : New angle, in degrees
///
////////////////////////////////////////////////////////////
CSFML_API void sfView_SetRotation(sfView* view, float angle);

////////////////////////////////////////////////////////////
/// Set the target viewport of a view
///
/// The viewport is the rectangle into which the contents of the
/// view are displayed, expressed as a factor (between 0 and 1)
/// of the size of the RenderTarget to which the view is applied.
///
/// \param view :     View to modify
/// \param viewport : New viewport
///
////////////////////////////////////////////////////////////
CSFML_API void sfView_SetViewport(sfView* view, sfFloatRect viewport);

////////////////////////////////////////////////////////////
/// Reset a view to the given rectangle.
/// Note: this function resets the rotation angle to 0.
///
/// \param view :      View to modify
/// \param rectangle : Rectangle defining the position and size of the view
///
////////////////////////////////////////////////////////////
CSFML_API void sfView_Reset(sfView* view, sfFloatRect rectangle);

////////////////////////////////////////////////////////////
/// Get the X coordinate of the center of a view
///
/// \param view : View to read
///
/// \return X coordinate of the center of the view
///
////////////////////////////////////////////////////////////
CSFML_API float sfView_GetCenterX(const sfView* view);

////////////////////////////////////////////////////////////
/// Get the Y coordinate of the center of a view
///
/// \param view : View to read
///
/// \return Y coordinate of the center of the view
///
////////////////////////////////////////////////////////////
CSFML_API float sfView_GetCenterY(const sfView* view);

////////////////////////////////////////////////////////////
/// Get the width of the view
///
/// \param view : View to read
///
/// \return Width of the view
///
////////////////////////////////////////////////////////////
CSFML_API float sfView_GetWidth(const sfView* view);

////////////////////////////////////////////////////////////
/// Get the height of the view
///
/// \param view : View to read
///
/// \return Height of the view
///
////////////////////////////////////////////////////////////
CSFML_API float sfView_GetHeight(const sfView* view);

////////////////////////////////////////////////////////////
/// Get the current rotation of a view
///
/// \param view : View to read
///
/// \return Rotation of the view, in degrees
///
////////////////////////////////////////////////////////////
CSFML_API float sfView_GetRotation(const sfView* view);

////////////////////////////////////////////////////////////
/// Get the target viewport of a view
///
/// \param view : View to read
///
/// \return Viewport rectangle, expressed as a factor of the target size
///
////////////////////////////////////////////////////////////
CSFML_API sfFloatRect sfView_GetViewport(const sfView* view);

////////////////////////////////////////////////////////////
/// Move a view
///
/// \param view :    View to move
/// \param offsetX : Offset to move the view, on X axis
/// \param offsetY : Offset to move the view, on Y axis
///
////////////////////////////////////////////////////////////
CSFML_API void sfView_Move(sfView* view, float offsetX, float offsetY);

////////////////////////////////////////////////////////////
/// Rotate a view
///
/// \param view :  View to rotate
/// \param angle : Angle in degrees
///
////////////////////////////////////////////////////////////
CSFML_API void sfView_Rotate(sfView* view, float angle);

////////////////////////////////////////////////////////////
/// Resize a view rectangle to simulate a zoom / unzoom effect
///
/// \param view :   View to zoom
/// \param factor : Zoom factor to apply, relative to the current zoom
///
////////////////////////////////////////////////////////////
CSFML_API void sfView_Zoom(sfView* view, float factor);


#endif // SFML_VIEW_H
