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
#include <SFML/Graphics/Transform.hpp>
#include <cmath>


namespace sf
{
////////////////////////////////////////////////////////////
const Transform Transform::Identity;


////////////////////////////////////////////////////////////
Transform::Transform()
{
    // Identity matrix
    myMatrix[0] = 1.f; myMatrix[4] = 0.f; myMatrix[8]  = 0.f; myMatrix[12] = 0.f;
    myMatrix[1] = 0.f; myMatrix[5] = 1.f; myMatrix[9]  = 0.f; myMatrix[13] = 0.f;
    myMatrix[2] = 0.f; myMatrix[6] = 0.f; myMatrix[10] = 1.f; myMatrix[14] = 0.f;
    myMatrix[3] = 0.f; myMatrix[7] = 0.f; myMatrix[11] = 0.f; myMatrix[15] = 1.f;
}


////////////////////////////////////////////////////////////
Transform::Transform(float a00, float a01, float a02,
                     float a10, float a11, float a12,
                     float a20, float a21, float a22)
{
    myMatrix[0] = a00; myMatrix[4] = a01; myMatrix[8]  = 0.f; myMatrix[12] = a02;
    myMatrix[1] = a10; myMatrix[5] = a11; myMatrix[9]  = 0.f; myMatrix[13] = a12;
    myMatrix[2] = 0.f; myMatrix[6] = 0.f; myMatrix[10] = 1.f; myMatrix[14] = 0.f;
    myMatrix[3] = a20; myMatrix[7] = a21; myMatrix[11] = 0.f; myMatrix[15] = a22;
}


////////////////////////////////////////////////////////////
const float* Transform::GetMatrix() const
{
    return myMatrix;
}


////////////////////////////////////////////////////////////
Transform Transform::GetInverse() const
{
    // Compute the determinant
    float det = myMatrix[0] * (myMatrix[15] * myMatrix[5] - myMatrix[7] * myMatrix[13]) -
                myMatrix[1] * (myMatrix[15] * myMatrix[4] - myMatrix[7] * myMatrix[12]) +
                myMatrix[3] * (myMatrix[13] * myMatrix[4] - myMatrix[5] * myMatrix[12]);

    // Compute the inverse if the determinant is not zero
    // (don't use an epsilon because the determinant may *really* be tiny)
    if (det != 0.f)
    {
        return Transform( (myMatrix[15] * myMatrix[5] - myMatrix[7] * myMatrix[13]) / det,
                         -(myMatrix[15] * myMatrix[4] - myMatrix[7] * myMatrix[12]) / det,
                          (myMatrix[13] * myMatrix[4] - myMatrix[5] * myMatrix[12]) / det,
                         -(myMatrix[15] * myMatrix[1] - myMatrix[3] * myMatrix[13]) / det,
                          (myMatrix[15] * myMatrix[0] - myMatrix[3] * myMatrix[12]) / det,
                         -(myMatrix[13] * myMatrix[0] - myMatrix[1] * myMatrix[12]) / det,
                          (myMatrix[7]  * myMatrix[1] - myMatrix[3] * myMatrix[5])  / det,
                         -(myMatrix[7]  * myMatrix[0] - myMatrix[3] * myMatrix[4])  / det,
                          (myMatrix[5]  * myMatrix[0] - myMatrix[1] * myMatrix[4])  / det);
    }
    else
    {
        return Identity;
    }
}


////////////////////////////////////////////////////////////
Vector2f Transform::TransformPoint(float x, float y) const
{
    return Vector2f(myMatrix[0] * x + myMatrix[4] * y + myMatrix[12],
                    myMatrix[1] * x + myMatrix[5] * y + myMatrix[13]);
}


////////////////////////////////////////////////////////////
Vector2f Transform::TransformPoint(const Vector2f& point) const
{
    return TransformPoint(point.x, point.y);
}


////////////////////////////////////////////////////////////
FloatRect Transform::TransformRect(const FloatRect& rectangle) const
{
    // Transform the 4 corners of the rectangle
    const Vector2f points[] =
    {
        TransformPoint(rectangle.Left, rectangle.Top),
        TransformPoint(rectangle.Left, rectangle.Top + rectangle.Height),
        TransformPoint(rectangle.Left + rectangle.Width, rectangle.Top),
        TransformPoint(rectangle.Left + rectangle.Width, rectangle.Top + rectangle.Height)
    };

    // Compute the bounding rectangle of the transformed points
    float left = points[0].x;
    float top = points[0].y;
    float right = points[0].x;
    float bottom = points[0].y;
    for (int i = 1; i < 4; ++i)
    {
        if      (points[i].x < left)   left = points[i].x;
        else if (points[i].x > right)  right = points[i].x;
        if      (points[i].y < top)    top = points[i].y;
        else if (points[i].y > bottom) bottom = points[i].y;
    }

    return FloatRect(left, top, right - left, bottom - top);
}


////////////////////////////////////////////////////////////
Transform Transform::Combine(const Transform& transform) const
{
    const float* a = myMatrix;
    const float* b = transform.myMatrix;

    return Transform(a[0] * b[0]  + a[4] * b[1]  + a[12] * b[3],
                     a[0] * b[4]  + a[4] * b[5]  + a[12] * b[7],
                     a[0] * b[12] + a[4] * b[13] + a[12] * b[15],
                     a[1] * b[0]  + a[5] * b[1]  + a[13] * b[3],
                     a[1] * b[4]  + a[5] * b[5]  + a[13] * b[7],
                     a[1] * b[12] + a[5] * b[13] + a[13] * b[15],
                     a[3] * b[0]  + a[7] * b[1]  + a[15] * b[3],
                     a[3] * b[4]  + a[7] * b[5]  + a[15] * b[7],
                     a[3] * b[12] + a[7] * b[13] + a[15] * b[15]);
}


////////////////////////////////////////////////////////////
Transform& Transform::Translate(float x, float y)
{
    Transform translation(1, 0, x,
                          0, 1, y,
                          0, 0, 1);

    return *this = Combine(translation);
}


////////////////////////////////////////////////////////////
Transform& Transform::Translate(const Vector2f& offset)
{
    return Translate(offset.x, offset.y);
}


////////////////////////////////////////////////////////////
Transform& Transform::Rotate(float angle)
{
    float rad = angle * 3.141592654f / 180.f;
    float cos = std::cos(rad);
    float sin = std::sin(rad);

    Transform rotation(cos, -sin, 0,
                       sin,  cos, 0,
                       0,    0,   1);

    return *this = Combine(rotation);
}


////////////////////////////////////////////////////////////
Transform& Transform::Rotate(float angle, float centerX, float centerY)
{
    float rad = angle * 3.141592654f / 180.f;
    float cos = std::cos(rad);
    float sin = std::sin(rad);

    Transform rotation(cos, -sin, centerX * (1 - cos) + centerY * sin,
                       sin,  cos, centerY * (1 - cos) - centerX * sin,
                       0,    0,   1);

    return *this = Combine(rotation);
}


////////////////////////////////////////////////////////////
Transform& Transform::Rotate(float angle, const Vector2f& center)
{
    return Rotate(angle, center.x, center.y);
}


////////////////////////////////////////////////////////////
Transform& Transform::Scale(float scaleX, float scaleY)
{
    Transform scaling(scaleX, 0,      0,
                      0,      scaleY, 0,
                      0,      0,      1);

    return *this = Combine(scaling);
}


////////////////////////////////////////////////////////////
Transform& Transform::Scale(float scaleX, float scaleY, float centerX, float centerY)
{
    Transform scaling(scaleX, 0,      centerX * (1 - scaleX),
                      0,      scaleY, centerY * (1 - scaleY),
                      0,      0,      1);

    return *this = Combine(scaling);
}


////////////////////////////////////////////////////////////
Transform& Transform::Scale(const Vector2f& factors)
{
    return Scale(factors.x, factors.y);
}


////////////////////////////////////////////////////////////
Transform& Transform::Scale(const Vector2f& factors, const Vector2f& center)
{
    return Scale(factors.x, factors.y, center.x, center.y);
}


////////////////////////////////////////////////////////////
Transform operator *(const Transform& left, const Transform& right)
{
    return left.Combine(right);
}


////////////////////////////////////////////////////////////
Transform& operator *=(Transform& left, const Transform& right)
{
    return left = left.Combine(right);
}


////////////////////////////////////////////////////////////
Vector2f operator *(const Transform& left, const Vector2f& right)
{
    return left.TransformPoint(right);
}

} // namespace sf
