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
myViewport     (0, 0, 1, 1),
myNeedUpdate   (true),
myNeedInvUpdate(true)
{
    Reset(FloatRect(0, 0, 1000, 1000));
}


////////////////////////////////////////////////////////////
/// Construct the view from a rectangle
////////////////////////////////////////////////////////////
View::View(const FloatRect& rectangle) :
myCenter       (),
mySize         (),
myRotation     (0),
myViewport     (0, 0, 1, 1),
myNeedUpdate   (true),
myNeedInvUpdate(true)
{
    Reset(rectangle);
}


////////////////////////////////////////////////////////////
/// Construct the view from its center and size
////////////////////////////////////////////////////////////
View::View(const Vector2f& center, const Vector2f& size) :
myCenter       (center),
mySize         (size),
myRotation     (0),
myViewport     (0, 0, 1, 1),
myNeedUpdate   (true),
myNeedInvUpdate(true)
{

}

////////////////////////////////////////////////////////////
/// Change the center of the view
////////////////////////////////////////////////////////////
void View::SetCenter(float x, float y)
{
    myCenter.x      = x;
    myCenter.y      = y;
    myNeedUpdate    = true;
    myNeedInvUpdate = true;
}


////////////////////////////////////////////////////////////
/// Change the center of the view
////////////////////////////////////////////////////////////
void View::SetCenter(const Vector2f& center)
{
    SetCenter(center.x, center.y);
}


////////////////////////////////////////////////////////////
/// Change the size of the view
////////////////////////////////////////////////////////////
void View::SetSize(float width, float height)
{
    mySize.x        = width;
    mySize.y        = height;
    myNeedUpdate    = true;
    myNeedInvUpdate = true;
}


////////////////////////////////////////////////////////////
/// Change the size of the view
////////////////////////////////////////////////////////////
void View::SetSize(const Vector2f& size)
{
    SetSize(size.x, size.y);
}


////////////////////////////////////////////////////////////
/// Set the angle of rotation of the view
////////////////////////////////////////////////////////////
void View::SetRotation(float angle)
{
    myRotation = static_cast<float>(fmod(angle, 360));
    if (myRotation < 0)
        myRotation += 360.f;
    myNeedUpdate    = true;
    myNeedInvUpdate = true;
}


////////////////////////////////////////////////////////////
/// Set the target viewport
////////////////////////////////////////////////////////////
void View::SetViewport(const FloatRect& viewport)
{
    myViewport = viewport;
}


////////////////////////////////////////////////////////////
/// Reset the view to the given rectangle
////////////////////////////////////////////////////////////
void View::Reset(const FloatRect& rectangle)
{
    myCenter        = rectangle.GetCenter();
    mySize          = rectangle.GetSize();
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
/// Get the current rotation
////////////////////////////////////////////////////////////
float View::GetRotation() const
{
    return myRotation;
}


////////////////////////////////////////////////////////////
/// Get the target viewport
////////////////////////////////////////////////////////////
const FloatRect& View::GetViewport() const
{
    return myViewport;
}


////////////////////////////////////////////////////////////
/// Move the view
////////////////////////////////////////////////////////////
void View::Move(float offsetX, float offsetY)
{
    SetCenter(myCenter.x + offsetX, myCenter.y + offsetY);
}


////////////////////////////////////////////////////////////
/// Move the view
////////////////////////////////////////////////////////////
void View::Move(const Vector2f& offset)
{
    SetCenter(myCenter + offset);
}


////////////////////////////////////////////////////////////
/// Rotate the view
////////////////////////////////////////////////////////////
void View::Rotate(float angle)
{
    SetRotation(myRotation + angle);
}


////////////////////////////////////////////////////////////
/// Resize the view rectangle to simulate a zoom / unzoom effect
////////////////////////////////////////////////////////////
void View::Zoom(float factor)
{
    SetSize(mySize.x * factor, mySize.y * factor);
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
