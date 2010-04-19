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
View::View() :
myCenter          (),
mySize            (),
myRotation        (0),
myViewport        (0, 0, 1, 1),
myMatrixUpdated   (false),
myInvMatrixUpdated(false)
{
    Reset(FloatRect(0, 0, 1000, 1000));
}


////////////////////////////////////////////////////////////
View::View(const FloatRect& rectangle) :
myCenter          (),
mySize            (),
myRotation        (0),
myViewport        (0, 0, 1, 1),
myMatrixUpdated   (false),
myInvMatrixUpdated(false)
{
    Reset(rectangle);
}


////////////////////////////////////////////////////////////
View::View(const Vector2f& center, const Vector2f& size) :
myCenter          (center),
mySize            (size),
myRotation        (0),
myViewport        (0, 0, 1, 1),
myMatrixUpdated   (false),
myInvMatrixUpdated(false)
{

}

////////////////////////////////////////////////////////////
void View::SetCenter(float x, float y)
{
    myCenter.x = x;
    myCenter.y = y;

    myMatrixUpdated    = false;
    myInvMatrixUpdated = false;
}


////////////////////////////////////////////////////////////
void View::SetCenter(const Vector2f& center)
{
    SetCenter(center.x, center.y);
}


////////////////////////////////////////////////////////////
void View::SetSize(float width, float height)
{
    mySize.x = width;
    mySize.y = height;

    myMatrixUpdated    = false;
    myInvMatrixUpdated = false;
}


////////////////////////////////////////////////////////////
void View::SetSize(const Vector2f& size)
{
    SetSize(size.x, size.y);
}


////////////////////////////////////////////////////////////
void View::SetRotation(float angle)
{
    myRotation = static_cast<float>(fmod(angle, 360));
    if (myRotation < 0)
        myRotation += 360.f;

    myMatrixUpdated    = false;
    myInvMatrixUpdated = false;
}


////////////////////////////////////////////////////////////
void View::SetViewport(const FloatRect& viewport)
{
    myViewport = viewport;
}


////////////////////////////////////////////////////////////
void View::Reset(const FloatRect& rectangle)
{
    myCenter.x = rectangle.Left + rectangle.Width / 2.f;
    myCenter.y = rectangle.Top + rectangle.Height / 2.f;
    mySize.x   = rectangle.Width;
    mySize.y   = rectangle.Height;
    myRotation = 0;

    myMatrixUpdated    = false;
    myInvMatrixUpdated = false;
}


////////////////////////////////////////////////////////////
const Vector2f& View::GetCenter() const
{
    return myCenter;
}


////////////////////////////////////////////////////////////
const Vector2f& View::GetSize() const
{
    return mySize;
}


////////////////////////////////////////////////////////////
float View::GetRotation() const
{
    return myRotation;
}


////////////////////////////////////////////////////////////
const FloatRect& View::GetViewport() const
{
    return myViewport;
}


////////////////////////////////////////////////////////////
void View::Move(float offsetX, float offsetY)
{
    SetCenter(myCenter.x + offsetX, myCenter.y + offsetY);
}


////////////////////////////////////////////////////////////
void View::Move(const Vector2f& offset)
{
    SetCenter(myCenter + offset);
}


////////////////////////////////////////////////////////////
void View::Rotate(float angle)
{
    SetRotation(myRotation + angle);
}


////////////////////////////////////////////////////////////
void View::Zoom(float factor)
{
    SetSize(mySize.x * factor, mySize.y * factor);
}


////////////////////////////////////////////////////////////
const Matrix3& View::GetMatrix() const
{
    // Recompute the matrix if needed
    if (!myMatrixUpdated)
    {
        myMatrix = Matrix3::Projection(myCenter, mySize, myRotation);
        myMatrixUpdated = true;
    }

    return myMatrix;
}


////////////////////////////////////////////////////////////
const Matrix3& View::GetInverseMatrix() const
{
    // Recompute the matrix if needed
    if (!myInvMatrixUpdated)
    {
        myInverseMatrix = GetMatrix().GetInverse();
        myInvMatrixUpdated = true;
    }

    return myInverseMatrix;
}

} // namespace sf
