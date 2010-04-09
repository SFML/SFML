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
template <typename T>
Rect<T>::Rect() :
Left  (0),
Top   (0),
Width (0),
Height(0)
{

}


////////////////////////////////////////////////////////////
template <typename T>
Rect<T>::Rect(T left, T top, T width, T height) :
Left  (left),
Top   (top),
Width (width),
Height(height)
{

}


////////////////////////////////////////////////////////////
template <typename T>
Rect<T>::Rect(const Vector2<T>& position, const Vector2<T>& size) :
Left  (position.x),
Top   (position.y),
Width (size.x),
Height(size.y)
{

}


////////////////////////////////////////////////////////////
template <typename T>
bool Rect<T>::Contains(T x, T y) const
{
    return (x >= Left) && (x < Left + Width) && (y >= Top) && (y < Top + Height);
}


////////////////////////////////////////////////////////////
template <typename T>
bool Rect<T>::Contains(const Vector2<T>& point) const
{
    return Contains(point.x, point.y);
}


////////////////////////////////////////////////////////////
template <typename T>
bool Rect<T>::Intersects(const Rect<T>& rectangle) const
{
    Rect<T> intersection;
    return Intersects(rectangle, intersection);
}


////////////////////////////////////////////////////////////
template <typename T>
bool Rect<T>::Intersects(const Rect<T>& rectangle, Rect<T>& intersection) const
{
    // Compute the intersection boundaries
    T left   = std::max(Left,         rectangle.Left);
    T top    = std::max(Top,          rectangle.Top);
    T right  = std::min(Left + Width, rectangle.Left + rectangle.Width);
    T bottom = std::min(Top + Height, rectangle.Top + rectangle.Height);

    // If the intersection is valid (positive non zero area), then there is an intersection
    if ((left < right) && (top < bottom))
    {
        intersection = Rect<T>(left, top, right - left, bottom - top);
        return true;
    }
    else
    {
        intersection = Rect<T>(0, 0, 0, 0);
        return false;
    }
}
