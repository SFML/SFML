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
#include <SFML/Graphics/View.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// Construct the view from a rectangle
////////////////////////////////////////////////////////////
View::View(const FloatRect& Rect)
{
    SetRect(Rect);
}


////////////////////////////////////////////////////////////
/// Construct the view from its center and size
////////////////////////////////////////////////////////////
View::View(const Vector2f& Center, const Vector2f& Size) :
myCenter       (Center),
mySize         (Size),
myNeedUpdate   (true),
myNeedInvUpdate(true)
{

}


////////////////////////////////////////////////////////////
/// Set the rectangle defining the view by its center and size
////////////////////////////////////////////////////////////
void View::SetRect(const Vector2f& Center, const Vector2f& Size)
{
    myCenter        = Center;
    mySize          = Size;
    myNeedUpdate    = true;
    myNeedInvUpdate = true;
}


////////////////////////////////////////////////////////////
/// Set the rectangle defining the view
////////////////////////////////////////////////////////////
void View::SetRect(const FloatRect& Rect)
{
    SetRect(Rect.GetCenter(), Rect.GetSize());
}


////////////////////////////////////////////////////////////
/// Change the center of the view (take 2 values)
////////////////////////////////////////////////////////////
void View::SetCenter(float X, float Y)
{
    myCenter.x      = X;
    myCenter.y      = Y;
    myNeedUpdate    = true;
    myNeedInvUpdate = true;
}


////////////////////////////////////////////////////////////
/// Change the center of the view (take a vector)
////////////////////////////////////////////////////////////
void View::SetCenter(const Vector2f& Center)
{
    SetCenter(Center.x, Center.y);
}


////////////////////////////////////////////////////////////
/// Change the size of the view (take 2 values)
////////////////////////////////////////////////////////////
void View::SetSize(float Width, float Height)
{
    mySize.x        = Width;
    mySize.y        = Height;
    myNeedUpdate    = true;
    myNeedInvUpdate = true;
}


////////////////////////////////////////////////////////////
/// Change the size of the view (take a vector)
////////////////////////////////////////////////////////////
void View::SetSize(const Vector2f& Size)
{
    SetSize(Size.x, Size.y);
}


////////////////////////////////////////////////////////////
/// Get the rectangle defining the view,
/// which is the combined center and size.
////////////////////////////////////////////////////////////
FloatRect View::GetRect() const
{
    return FloatRect(myCenter.x - mySize.x / 2,
                     myCenter.y - mySize.y / 2,
                     myCenter.x + mySize.x / 2,
                     myCenter.y + mySize.y / 2);
}


////////////////////////////////////////////////////////////
/// Get the center of the view
////////////////////////////////////////////////////////////
const Vector2f& View::GetCenter() const
{
    return myCenter;
}


////////////////////////////////////////////////////////////
/// Get the size of the view
////////////////////////////////////////////////////////////
const Vector2f& View::GetSize() const
{
    return mySize;
}


////////////////////////////////////////////////////////////
/// Move the view (take 2 values)
////////////////////////////////////////////////////////////
void View::Move(float OffsetX, float OffsetY)
{
    myCenter.x     += OffsetX;
    myCenter.y     += OffsetY;
    myNeedUpdate    = true;
    myNeedInvUpdate = true;
}


////////////////////////////////////////////////////////////
/// Move the view (take a vector)
////////////////////////////////////////////////////////////
void View::Move(const Vector2f& Offset)
{
    Move(Offset.x, Offset.y);
}


////////////////////////////////////////////////////////////
/// Resize the view rectangle to simulate a zoom / unzoom effect
////////////////////////////////////////////////////////////
void View::Zoom(float Factor)
{
    if (Factor != 0)
    {
        mySize /= Factor;
        myNeedUpdate    = true;
        myNeedInvUpdate = true;
    }
}


////////////////////////////////////////////////////////////
/// Get the projection matrix of the view
////////////////////////////////////////////////////////////
const Matrix3& View::GetMatrix() const
{
    // Recompute the matrix if needed
    if (myNeedUpdate)
    {
        myMatrix.SetFromProjection(GetRect());
        myNeedUpdate = false;
    }

    return myMatrix;
}


////////////////////////////////////////////////////////////
/// Get the inverse projection matrix of the view
////////////////////////////////////////////////////////////
const Matrix3& View::GetInverseMatrix() const
{
    // Recompute the matrix if needed
    if (myNeedInvUpdate)
    {
        myInverseMatrix = GetMatrix().GetInverse();
        myNeedInvUpdate = false;
    }

    return myInverseMatrix;
}

} // namespace sf
