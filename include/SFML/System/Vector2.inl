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
/// Default constructor
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T>::Vector2() :
x(0),
y(0)
{

}


////////////////////////////////////////////////////////////
/// Construct the color from its coordinates
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T>::Vector2(T X, T Y) :
x(X),
y(Y)
{

}


////////////////////////////////////////////////////////////
/// Operator - overload ; returns the opposite of a vector
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T> operator -(const Vector2<T>& V)
{
    return Vector2<T>(-V.x, -V.y);
}


////////////////////////////////////////////////////////////
/// Operator += overload ; add two vectors and assign to the first op
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T>& operator +=(Vector2<T>& V1, const Vector2<T>& V2)
{
    V1.x += V2.x;
    V1.y += V2.y;

    return V1;
}


////////////////////////////////////////////////////////////
/// Operator -= overload ; subtract two vectors and assign to the first op
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T>& operator -=(Vector2<T>& V1, const Vector2<T>& V2)
{
    V1.x -= V2.x;
    V1.y -= V2.y;

    return V1;
}


////////////////////////////////////////////////////////////
/// Operator + overload ; adds two vectors
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T> operator +(const Vector2<T>& V1, const Vector2<T>& V2)
{
    return Vector2<T>(V1.x + V2.x, V1.y + V2.y);
}


////////////////////////////////////////////////////////////
/// Operator - overload ; subtracts two vectors
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T> operator -(const Vector2<T>& V1, const Vector2<T>& V2)
{
    return Vector2<T>(V1.x - V2.x, V1.y - V2.y);
}


////////////////////////////////////////////////////////////
/// Operator * overload ; multiply a vector by a scalar value
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T> operator *(const Vector2<T>& V, T X)
{
    return Vector2<T>(V.x * X, V.y * X);
}


////////////////////////////////////////////////////////////
/// Operator * overload ; multiply a scalar value by a vector
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T> operator *(T X, const Vector2<T>& V)
{
    return Vector2<T>(V.x * X, V.y * X);
}


////////////////////////////////////////////////////////////
/// Operator *= overload ; multiply-assign a vector by a scalar value
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T>& operator *=(Vector2<T>& V, T X)
{
    V.x *= X;
    V.y *= X;

    return V;
}


////////////////////////////////////////////////////////////
/// Operator / overload ; divide a vector by a scalar value
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T> operator /(const Vector2<T>& V, T X)
{
    return Vector2<T>(V.x / X, V.y / X);
}


////////////////////////////////////////////////////////////
/// Operator /= overload ; divide-assign a vector by a scalar value
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T>& operator /=(Vector2<T>& V, T X)
{
    V.x /= X;
    V.y /= X;

    return V;
}


////////////////////////////////////////////////////////////
/// Operator == overload ; compares the equality of two vectors
////////////////////////////////////////////////////////////
template <typename T>
bool operator ==(const Vector2<T>& V1, const Vector2<T>& V2)
{
    return (V1.x == V2.x) && (V1.y == V2.y);
}


////////////////////////////////////////////////////////////
/// Operator != overload ; compares the difference of two vectors
////////////////////////////////////////////////////////////
template <typename T>
bool operator !=(const Vector2<T>& V1, const Vector2<T>& V2)
{
    return (V1.x != V2.x) || (V1.y != V2.y);
}
