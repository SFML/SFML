////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2021 Laurent Gomila (laurent@sfml-dev.org)
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
template <typename T>
constexpr Vector2<T>::Vector2() :
x(0),
y(0)
{

}


////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T>::Vector2(T X, T Y) :
x(X),
y(Y)
{

}


template<typename T>
Vector2<float> Vector2<T>::fromAngle(float angle) {
    float radians = (angle * 3.14159265) / 180.0;
    float x = cos(radians);
    float y = sin(radians);
    return Vector2<float>(x, y);

}

template <typename T>
float Vector2<T>::mag(const Vector2<T>& from) {
    return((float)sqrt(pow(from.x, 2) + pow(from.y, 2)));

}

template <typename T>
float Vector2<T>::dot(const Vector2<T>& one, const Vector2<T>& two) {
    return ((float)(one.x * two.x + one.y * two.y));

}

template <typename T>
float Vector2<T>::cross(const Vector2<T>& one, const Vector2<T>& two) {
    return ((float)(one.x * two.y - one.y * two.x));
}

template <typename T>
float Vector2<T>::heading(const Vector2<T>& vect) {
    return ((float)atan2f((float)vect.y, (float)vect.x)*180 / 3.14159265);
}

////////////////////////////////////////////////////////////
template <typename T>
template <typename U>
constexpr Vector2<T>::Vector2(const Vector2<U>& vector) :
x(static_cast<T>(vector.x)),
y(static_cast<T>(vector.y))
{
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T> operator -(const Vector2<T>& right)
{
    return Vector2<T>(-right.x, -right.y);
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T>& right)
{
    left.x += right.x;
    left.y += right.y;

    return left;
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T>& operator -=(Vector2<T>& left, const Vector2<T>& right)
{
    left.x -= right.x;
    left.y -= right.y;

    return left;
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T> operator +(const Vector2<T>& left, const Vector2<T>& right)
{
    return Vector2<T>(left.x + right.x, left.y + right.y);
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T> operator -(const Vector2<T>& left, const Vector2<T>& right)
{
    return Vector2<T>(left.x - right.x, left.y - right.y);
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T> operator *(const Vector2<T>& left, T right)
{
    return Vector2<T>(left.x * right, left.y * right);
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T> operator *(T left, const Vector2<T>& right)
{
    return Vector2<T>(right.x * left, right.y * left);
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T>& operator *=(Vector2<T>& left, T right)
{
    left.x *= right;
    left.y *= right;

    return left;
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T> operator /(const Vector2<T>& left, T right)
{
    return Vector2<T>(left.x / right, left.y / right);
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T>& operator /=(Vector2<T>& left, T right)
{
    left.x /= right;
    left.y /= right;

    return left;
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr bool operator ==(const Vector2<T>& left, const Vector2<T>& right)
{
    return (left.x == right.x) && (left.y == right.y);
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr bool operator !=(const Vector2<T>& left, const Vector2<T>& right)
{
    return (left.x != right.x) || (left.y != right.y);
}
