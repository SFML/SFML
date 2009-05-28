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
/// Change the half-size of a view
///
/// \param View :       View to modify
/// \param HalfWidth :  New half-width
/// \param HalfHeight : New half-height
///
////////////////////////////////////////////////////////////
CSFML_API void sfView_SetHalfSize(sfView* View, float HalfWidth, float HalfHeight);

////////////////////////////////////////////////////////////
/// Rebuild a view from a rectangle
///
/// \param View :     View to modify
/// \param ViewRect : Rectangle defining the position and size of the view
///
////////////////////////////////////////////////////////////
CSFML_API void sfView_SetFromRect(sfView* View, sfFloatRect ViewRect);

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
/// Get the half-width of the view
///
/// \param View : View to read
///
/// \return Half-width of the view
///
////////////////////////////////////////////////////////////
CSFML_API float sfView_GetHalfSizeX(sfView* View);

////////////////////////////////////////////////////////////
/// Get the half-height of the view
///
/// \param View : View to read
///
/// \return Half-height of the view
///
////////////////////////////////////////////////////////////
CSFML_API float sfView_GetHalfSizeY(sfView* View);

////////////////////////////////////////////////////////////
/// Get the bounding rectangle of a view
///
/// \param View : View to read
///
/// \return Bounding rectangle of the view
///
////////////////////////////////////////////////////////////
CSFML_API sfFloatRect sfView_GetRect(sfView* View);

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
/// Resize a view rectangle to simulate a zoom / unzoom effect
///
/// \param View :   View to zoom
/// \param Factor : Zoom factor to apply, relative to the current zoom
///
////////////////////////////////////////////////////////////
CSFML_API void sfView_Zoom(sfView* View, float Factor);


#endif // SFML_VIEW_H
