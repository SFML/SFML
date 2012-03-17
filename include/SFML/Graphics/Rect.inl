////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2012 Laurent Gomila (laurent.gom@gmail.com)
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
Rect<T>::Rect() :
left  (0),
top   (0),
width (0),
height(0)
{

}


////////////////////////////////////////////////////////////
template <typename T>
Rect<T>::Rect(T rectLeft, T rectTop, T rectWidth, T rectHeight) :
left  (rectLeft),
top   (rectTop),
width (rectWidth),
height(rectHeight)
{

}


////////////////////////////////////////////////////////////
template <typename T>
Rect<T>::Rect(const Vector2<T>& position, const Vector2<T>& size) :
left  (position.x),
top   (position.y),
width (size.x),
height(size.y)
{

}


////////////////////////////////////////////////////////////
template <typename T>
template <typename U>
Rect<T>::Rect(const Rect<U>& rectangle) :
left  (static_cast<T>(rectangle.left)),
top   (static_cast<T>(rectangle.top)),
width (static_cast<T>(rectangle.width)),
height(static_cast<T>(rectangle.height))
{
}


////////////////////////////////////////////////////////////
template <typename T>
bool Rect<T>::contains(T x, T y) const
{
    return (x >= left) && (x < left + width) && (y >= top) && (y < top + height);
}


////////////////////////////////////////////////////////////
template <typename T>
bool Rect<T>::contains(const Vector2<T>& point) const
{
    return contains(point.x, point.y);
}


////////////////////////////////////////////////////////////
template <typename T>
bool Rect<T>::intersects(const Rect<T>& rectangle) const
{
    Rect<T> intersection;
    return intersects(rectangle, intersection);
}


////////////////////////////////////////////////////////////
template <typename T>
bool Rect<T>::intersects(const Rect<T>& rectangle, Rect<T>& intersection) const
{
    // Compute the intersection boundaries
    T interLeft   = std::max(left,         rectangle.left);
    T interTop    = std::max(top,          rectangle.top);
    T interRight  = std::min(left + width, rectangle.left + rectangle.width);
    T interBottom = std::min(top + height, rectangle.top + rectangle.height);

    // If the intersection is valid (positive non zero area), then there is an intersection
    if ((interLeft < interRight) && (interTop < interBottom))
    {
        intersection = Rect<T>(interLeft, interTop, interRight - interLeft, interBottom - interTop);
        return true;
    }
    else
    {
        intersection = Rect<T>(0, 0, 0, 0);
        return false;
    }
}


////////////////////////////////////////////////////////////
template <typename T>
inline bool operator ==(const Rect<T>& left, const Rect<T>& right)
{
    return (left.left == right.left) && (left.width == right.width) &&
           (left.top == right.top) && (left.height == right.height);
}


////////////////////////////////////////////////////////////
template <typename T>
inline bool operator !=(const Rect<T>& left, const Rect<T>& right)
{
    return !(left == right);
}
