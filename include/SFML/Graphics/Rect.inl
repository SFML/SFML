////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
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
constexpr Rect<T>::Rect() = default;


////////////////////////////////////////////////////////////
template <typename T>
constexpr Rect<T>::Rect(const Vector2<T>& thePosition, const Vector2<T>& theSize) : position(thePosition), size(theSize)
{
}


////////////////////////////////////////////////////////////
template <typename T>
template <typename U>
constexpr Rect<T>::Rect(const Rect<U>& rectangle) : position(rectangle.position), size(rectangle.size)
{
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr T Rect<T>::getLeft() const
{
    return size.x >= 0 ? position.x : position.x + size.x;
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr T Rect<T>::getTop() const
{
    return size.y >= 0 ? position.y : position.y + size.y;
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr T Rect<T>::getRight() const
{
    return size.x >= 0 ? position.x + size.x : position.x;
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr T Rect<T>::getBottom() const
{
    return size.y >= 0 ? position.y + size.y : position.y;
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T> Rect<T>::getStart() const
{
    return Vector2<T>(getLeft(), getTop());
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T> Rect<T>::getEnd() const
{
    return Vector2<T>(getRight(), getBottom());
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T> Rect<T>::getCenter() const
{
    return position + size / static_cast<T>(2);
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T> Rect<T>::getAbsoluteSize() const
{
    return Vector2<T>(size.x >= 0 ? size.x : -size.x, size.y >= 0 ? size.y : -size.y);
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr Rect<T> Rect<T>::getAbsoluteRect() const
{
    return Rect<T>(getStart(), getAbsoluteSize());
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr bool Rect<T>::contains(const Vector2<T>& point) const
{
    return getLeft() <= point.x && point.x < getRight() && getTop() <= point.y && point.y < getBottom();
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr std::optional<Rect<T>> Rect<T>::findIntersection(const Rect<T>& rectangle) const
{
    T left   = getLeft();
    T top    = getTop();
    T right  = getRight();
    T bottom = getBottom();

    T rectLeft   = rectangle.getLeft();
    T rectTop    = rectangle.getTop();
    T rectRight  = rectangle.getRight();
    T rectBottom = rectangle.getBottom();

    if (rectRight <= left || rectBottom <= top || rectLeft >= right || rectTop >= bottom)
    {
        return std::nullopt;
    }

    T x      = rectLeft > left ? rectLeft : left;
    T y      = rectTop > top ? rectTop : top;
    T width  = rectRight < right ? rectRight - x : right - x;
    T height = rectBottom < bottom ? rectBottom - y : bottom - y;

    return Rect({x, y}, {width, height});
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr bool operator==(const Rect<T>& left, const Rect<T>& right)
{
    return left.position == right.position && left.size == right.size;
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr bool operator!=(const Rect<T>& left, const Rect<T>& right)
{
    return !(left == right);
}
