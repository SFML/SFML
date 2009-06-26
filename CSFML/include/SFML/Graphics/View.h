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
CSFML_API sfView* sfView_Create();

////////////////////////////////////////////////////////////
/// Construct a view from a rectangle
///
/// \param Rect : Rectangle defining the bounds of the view
///
////////////////////////////////////////////////////////////
CSFML_API sfView* sfView_CreateFromRect(sfFloatRect Rect);

////////////////////////////////////////////////////////////
/// Destroy an existing view
///
/// \param View : View to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfView_Destroy(sfView* View);

////////////////////////////////////////////////////////////
/// Change the center of a view
///
/// \param View : View to modify
/// \param X :    X coordinate of the new center
/// \param Y :    Y coordinate of the new center
///
////////////////////////////////////////////////////////////
CSFML_API void sfView_SetCenter(sfView* View, float X, float Y);

////////////////////////////////////////////////////////////
/// Change the size of a view
///
/// \param View :   View to modify
/// \param Width :  New width
/// \param Height : New height
///
////////////////////////////////////////////////////////////
CSFML_API void sfView_SetSize(sfView* View, float Width, float Height);

////////////////////////////////////////////////////////////
/// Set the angle of rotation of a view
///
/// \param View :  View to modify
/// \param Angle : New angle, in degrees
///
////////////////////////////////////////////////////////////
CSFML_API void sfView_SetRotation(sfView* View, float Angle);

////////////////////////////////////////////////////////////
/// Set the target viewport of a view
///
/// The viewport is the rectangle into which the contents of the
/// view are displayed, expressed as a factor (between 0 and 1)
/// of the size of the RenderTarget to which the view is applied.
///
/// \param View :     View to modify
/// \param Viewport : New viewport
///
////////////////////////////////////////////////////////////
CSFML_API void sfView_SetViewport(sfView* View, sfFloatRect Viewport);

////////////////////////////////////////////////////////////
/// Reset a view to the given rectangle.
/// Note: this function resets the rotation angle to 0.
///
/// \param View :      View to modify
/// \param Rectangle : Rectangle defining the position and size of the view
///
////////////////////////////////////////////////////////////
CSFML_API void sfView_Reset(sfView* View, sfFloatRect Rectangle);

////////////////////////////////////////////////////////////
/// Get the X coordinate of the center of a view
///
/// \param View : View to read
///
/// \return X coordinate of the center of the view
///
////////////////////////////////////////////////////////////
CSFML_API float sfView_GetCenterX(sfView* View);

////////////////////////////////////////////////////////////
/// Get the Y coordinate of the center of a view
///
/// \param View : View to read
///
/// \return Y coordinate of the center of the view
///
////////////////////////////////////////////////////////////
CSFML_API float sfView_GetCenterY(sfView* View);

////////////////////////////////////////////////////////////
/// Get the width of the view
///
/// \param View : View to read
///
/// \return Width of the view
///
////////////////////////////////////////////////////////////
CSFML_API float sfView_GetWidth(sfView* View);

////////////////////////////////////////////////////////////
/// Get the height of the view
///
/// \param View : View to read
///
/// \return Height of the view
///
////////////////////////////////////////////////////////////
CSFML_API float sfView_GetHeight(sfView* View);

////////////////////////////////////////////////////////////
/// Get the current rotation of a view
///
/// \param View : View to read
///
/// \return Rotation of the view, in degrees
///
////////////////////////////////////////////////////////////
CSFML_API float sfView_GetRotation(sfView* View);

////////////////////////////////////////////////////////////
/// Get the target viewport of a view
///
/// \param View : View to read
///
/// \return Viewport rectangle, expressed as a factor of the target size
///
////////////////////////////////////////////////////////////
CSFML_API sfFloatRect sfView_GetViewport(sfView* View);

////////////////////////////////////////////////////////////
/// Move a view
///
/// \param View :    View to move
/// \param OffsetX : Offset to move the view, on X axis
/// \param OffsetY : Offset to move the view, on Y axis
///
////////////////////////////////////////////////////////////
CSFML_API void sfView_Move(sfView* View, float OffsetX, float OffsetY);

////////////////////////////////////////////////////////////
/// Rotate a view
///
/// \param View :  View to rotate
/// \param Angle : Angle in degrees
///
////////////////////////////////////////////////////////////
CSFML_API void sfView_Rotate(sfView* View, float Angle);

////////////////////////////////////////////////////////////
/// Resize a view rectangle to simulate a zoom / unzoom effect
///
/// \param View :   View to zoom
/// \param Factor : Zoom factor to apply, relative to the current zoom
///
////////////////////////////////////////////////////////////
CSFML_API void sfView_Zoom(sfView* View, float Factor);


#endif // SFML_VIEW_H
