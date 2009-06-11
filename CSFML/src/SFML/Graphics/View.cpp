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
#include <SFML/Graphics/View.h>
#include <SFML/Graphics/ViewStruct.h>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Construct a default view (1000x1000)
////////////////////////////////////////////////////////////
sfView* sfView_Create()
{
    return new sfView;
}


////////////////////////////////////////////////////////////
/// Construct a view from a rectangle
////////////////////////////////////////////////////////////
sfView* sfView_CreateFromRect(sfFloatRect Rect)
{
    return new sfView(Rect);
}


////////////////////////////////////////////////////////////
/// Destroy an existing view
////////////////////////////////////////////////////////////
void sfView_Destroy(sfView* View)
{
    delete View;
}


////////////////////////////////////////////////////////////
/// Change the center of a view
////////////////////////////////////////////////////////////
void sfView_SetCenter(sfView* View, float X, float Y)
{
    CSFML_CALL_PTR(View, SetCenter(X, Y));
}


////////////////////////////////////////////////////////////
/// Change the half-size of a view
////////////////////////////////////////////////////////////
void sfView_SetHalfSize(sfView* View, float HalfWidth, float HalfHeight)
{
    CSFML_CALL_PTR(View, SetHalfSize(HalfWidth, HalfHeight));
}


////////////////////////////////////////////////////////////
/// Rebuild a view from a rectangle
////////////////////////////////////////////////////////////
void sfView_SetFromRect(sfView* View, sfFloatRect ViewRect)
{
    CSFML_CALL_PTR(View, SetFromRect(sf::FloatRect(ViewRect.Left, ViewRect.Top, ViewRect.Right, ViewRect.Bottom)));
}


////////////////////////////////////////////////////////////
/// Get the X coordinate of the center of a view
////////////////////////////////////////////////////////////
float sfView_GetCenterX(sfView* View)
{
    CSFML_CHECK_RETURN(View, 0.f);

    return View->This->GetCenter().x;
}


////////////////////////////////////////////////////////////
/// Get the Y coordinate of the center of a view
////////////////////////////////////////////////////////////
float sfView_GetCenterY(sfView* View)
{
    CSFML_CHECK_RETURN(View, 0.f);

    return View->This->GetCenter().y;
}


////////////////////////////////////////////////////////////
/// Get the half-width of the view
////////////////////////////////////////////////////////////
float sfView_GetHalfSizeY(sfView* View)
{
    CSFML_CHECK_RETURN(View, 0.f);

    return View->This->GetHalfSize().x;
}


////////////////////////////////////////////////////////////
/// Get the half-height of the view
////////////////////////////////////////////////////////////
float sfView_GetHalfSizeX(sfView* View)
{
    CSFML_CHECK_RETURN(View, 0.f);

    return View->This->GetHalfSize().y;
}


////////////////////////////////////////////////////////////
/// Get the bounding rectangle of a view
////////////////////////////////////////////////////////////
sfFloatRect sfView_GetRect(sfView* View)
{
    sfFloatRect Rect = {0, 0, 0, 0};
    CSFML_CHECK_RETURN(View, Rect);

    sf::FloatRect SFMLRect = View->This->GetRect();
    Rect.Left   = SFMLRect.Left;
    Rect.Top    = SFMLRect.Top;
    Rect.Right  = SFMLRect.Right;
    Rect.Bottom = SFMLRect.Bottom;

    return Rect;
}


////////////////////////////////////////////////////////////
/// Move a view
////////////////////////////////////////////////////////////
void sfView_Move(sfView* View, float OffsetX, float OffsetY)
{
    CSFML_CALL_PTR(View, Move(OffsetX, OffsetY));
}


////////////////////////////////////////////////////////////
/// Resize a view rectangle to simulate a zoom / unzoom effect
////////////////////////////////////////////////////////////
void sfView_Zoom(sfView* View, float Factor)
{
    CSFML_CALL_PTR(View, Zoom(Factor));
}
