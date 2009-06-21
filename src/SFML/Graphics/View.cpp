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
/// Default constructor
/// Constructs a default view of (0, 0, 1000, 1000)
////////////////////////////////////////////////////////////
View::View() :
myCenter       (),
mySize         (),
myRotation     (0),
myNeedUpdate   (true),
myNeedInvUpdate(true)
{
    Reset(FloatRect(0, 0, 1000, 1000));
}


////////////////////////////////////////////////////////////
/// Construct the view from a rectangle
////////////////////////////////////////////////////////////
View::View(const FloatRect& Rectangle) :
myCenter       (),
mySize         (),
myRotation     (0),
myNeedUpdate   (true),
myNeedInvUpdate(true)
{
    Reset(Rectangle);
}


////////////////////////////////////////////////////////////
/// Construct the view from its center and size
////////////////////////////////////////////////////////////
View::View(const Vector2f& Center, const Vector2f& Size) :
myCenter       (Center),
mySize         (Size),
myRotation     (0),
myNeedUpdate   (true),
myNeedInvUpdate(true)
{

}

////////////////////////////////////////////////////////////
/// Change the center of the view
////////////////////////////////////////////////////////////
void View::SetCenter(float X, float Y)
{
    myCenter.x      = X;
    myCenter.y      = Y;
    myNeedUpdate    = true;
    myNeedInvUpdate = true;
}


////////////////////////////////////////////////////////////
/// Change the center of the view
////////////////////////////////////////////////////////////
void View::SetCenter(const Vector2f& Center)
{
    SetCenter(Center.x, Center.y);
}


////////////////////////////////////////////////////////////
/// Change the size of the view
////////////////////////////////////////////////////////////
void View::SetSize(float Width, float Height)
{
    mySize.x        = Width;
    mySize.y        = Height;
    myNeedUpdate    = true;
    myNeedInvUpdate = true;
}


////////////////////////////////////////////////////////////
/// Change the size of the view
////////////////////////////////////////////////////////////
void View::SetSize(const Vector2f& Size)
{
    SetSize(Size.x, Size.y);
}


////////////////////////////////////////////////////////////
/// Set the angle of rotation of the view
////////////////////////////////////////////////////////////
void View::SetRotation(float Angle)
{
    myRotation      = Angle;
    myNeedUpdate    = true;
    myNeedInvUpdate = true;
}


////////////////////////////////////////////////////////////
/// Reset the view to the given rectangle
////////////////////////////////////////////////////////////
void View::Reset(const FloatRect& Rectangle)
{
    myCenter        = Rectangle.GetCenter();
    mySize          = Rectangle.GetSize();
    myNeedUpdate    = true;
    myNeedInvUpdate = true;
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
/// Move the view
////////////////////////////////////////////////////////////
void View::Move(float OffsetX, float OffsetY)
{
    myCenter.x     += OffsetX;
    myCenter.y     += OffsetY;
    myNeedUpdate    = true;
    myNeedInvUpdate = true;
}


////////////////////////////////////////////////////////////
/// Move the view
////////////////////////////////////////////////////////////
void View::Move(const Vector2f& Offset)
{
    Move(Offset.x, Offset.y);
}


////////////////////////////////////////////////////////////
/// Rotate the view
////////////////////////////////////////////////////////////
void View::Rotate(float Angle)
{
    myRotation     += Angle;
    myNeedUpdate    = true;
    myNeedInvUpdate = true;
}


////////////////////////////////////////////////////////////
/// Resize the view rectangle to simulate a zoom / unzoom effect
////////////////////////////////////////////////////////////
void View::Zoom(float Factor)
{
    mySize.x       *= Factor;
    mySize.y       *= Factor;
    myNeedUpdate    = true;
    myNeedInvUpdate = true;
}


////////////////////////////////////////////////////////////
/// Get the projection matrix of the view
////////////////////////////////////////////////////////////
const Matrix3& View::GetMatrix() const
{
    // Recompute the matrix if needed
    if (myNeedUpdate)
    {
        myMatrix.SetFromProjection(myCenter, mySize, myRotation);
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
