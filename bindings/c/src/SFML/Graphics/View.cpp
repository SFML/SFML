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
sfView* sfView_Create(void)
{
    return new sfView;
}


////////////////////////////////////////////////////////////
/// Construct a view from a rectangle
////////////////////////////////////////////////////////////
sfView* sfView_CreateFromRect(sfFloatRect rectangle)
{
    sfView* view = new sfView;
    sfView_Reset(view, rectangle);

    return view;
}


////////////////////////////////////////////////////////////
/// Copy an existing view
////////////////////////////////////////////////////////////
sfView* sfView_Copy(sfView* view)
{
    CSFML_CHECK_RETURN(view, NULL);

    return new sfView(*view);
}


////////////////////////////////////////////////////////////
/// Destroy an existing view
////////////////////////////////////////////////////////////
void sfView_Destroy(sfView* view)
{
    delete view;
}


////////////////////////////////////////////////////////////
/// Change the center of a view
////////////////////////////////////////////////////////////
void sfView_SetCenter(sfView* view, float x, float y)
{
    CSFML_CALL(view, SetCenter(x, y));
}


////////////////////////////////////////////////////////////
/// Change the size of a view
////////////////////////////////////////////////////////////
void sfView_SetSize(sfView* view, float width, float height)
{
    CSFML_CALL(view, SetSize(width, height));
}


////////////////////////////////////////////////////////////
/// Set the angle of rotation of a view
////////////////////////////////////////////////////////////
void sfView_SetRotation(sfView* view, float angle)
{
    CSFML_CALL(view, SetRotation(angle));
}


////////////////////////////////////////////////////////////
/// Set the target viewport of a view
////////////////////////////////////////////////////////////
void sfView_SetViewport(sfView* view, sfFloatRect viewport)
{
    CSFML_CALL(view, SetViewport(sf::FloatRect(viewport.Left, viewport.Top, viewport.Width, viewport.Height)));
}


////////////////////////////////////////////////////////////
/// Reset a view to the given rectangle.
/// Note: this function resets the rotation angle to 0.
////////////////////////////////////////////////////////////
void sfView_Reset(sfView* view, sfFloatRect rectangle)
{
    CSFML_CALL(view, Reset(sf::FloatRect(rectangle.Left, rectangle.Top, rectangle.Width, rectangle.Height)));
}


////////////////////////////////////////////////////////////
/// Get the X coordinate of the center of a view
////////////////////////////////////////////////////////////
float sfView_GetCenterX(const sfView* view)
{
    CSFML_CHECK_RETURN(view, 0.f);

    return view->This.GetCenter().x;
}


////////////////////////////////////////////////////////////
/// Get the Y coordinate of the center of a view
////////////////////////////////////////////////////////////
float sfView_GetCenterY(const sfView* view)
{
    CSFML_CHECK_RETURN(view, 0.f);

    return view->This.GetCenter().y;
}


////////////////////////////////////////////////////////////
/// Get the width of the view
////////////////////////////////////////////////////////////
float sfView_GetWidth(const sfView* view)
{
    CSFML_CHECK_RETURN(view, 0.f);

    return view->This.GetSize().x;
}


////////////////////////////////////////////////////////////
/// Get the height of the view
////////////////////////////////////////////////////////////
float sfView_GetHeight(const sfView* view)
{
    CSFML_CHECK_RETURN(view, 0.f);

    return view->This.GetSize().y;
}


////////////////////////////////////////////////////////////
/// Get the current rotation of a view
////////////////////////////////////////////////////////////
float sfView_GetRotation(const sfView* view)
{
    CSFML_CALL_RETURN(view, GetRotation(), 0.f);
}


////////////////////////////////////////////////////////////
/// Get the target viewport of a view
////////////////////////////////////////////////////////////
sfFloatRect sfView_GetViewport(const sfView* view)
{
    sfFloatRect rect = {0, 0, 0, 0};
    CSFML_CHECK_RETURN(view, rect);

    sf::FloatRect SFMLRect = view->This.GetViewport();
    rect.Left   = SFMLRect.Left;
    rect.Top    = SFMLRect.Top;
    rect.Width  = SFMLRect.Width;
    rect.Height = SFMLRect.Height;

    return rect;
}


////////////////////////////////////////////////////////////
/// Move a view
////////////////////////////////////////////////////////////
void sfView_Move(sfView* view, float offsetX, float offsetY)
{
    CSFML_CALL(view, Move(offsetX, offsetY));
}


////////////////////////////////////////////////////////////
/// Rotate a view
////////////////////////////////////////////////////////////
void sfView_Rotate(sfView* view, float angle)
{
    CSFML_CALL(view, Rotate(angle));
}


////////////////////////////////////////////////////////////
/// Resize a view rectangle to simulate a zoom / unzoom effect
////////////////////////////////////////////////////////////
void sfView_Zoom(sfView* view, float factor)
{
    CSFML_CALL(view, Zoom(factor));
}
