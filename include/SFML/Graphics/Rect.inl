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
Rect<T>::Rect() :
Left  (0),
Top   (0),
Right (0),
Bottom(0)
{

}


////////////////////////////////////////////////////////////
/// Construct the color from its coordinates
////////////////////////////////////////////////////////////
template <typename T>
Rect<T>::Rect(T left, T top, T right, T bottom) :
Left  (left),
Top   (top),
Right (right),
Bottom(bottom)
{

}


////////////////////////////////////////////////////////////
/// Get the size of the rectangle
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T> Rect<T>::GetSize() const
{
    return Vector2<T>(Right - Left, Bottom - Top);
}


////////////////////////////////////////////////////////////
/// Get the center of the rectangle
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T> Rect<T>::GetCenter() const
{
    return Vector2<T>((Left + Right) / 2, (Top + Bottom) / 2);
}


////////////////////////////////////////////////////////////
/// Move the whole rectangle by the given offset
////////////////////////////////////////////////////////////
template <typename T>
void Rect<T>::Offset(T offsetX, T offsetY)
{
    Left   += offsetX;
    Right  += offsetX;
    Top    += offsetY;
    Bottom += offsetY;
}


////////////////////////////////////////////////////////////
/// Move the whole rectangle by the given offset
////////////////////////////////////////////////////////////
template <typename T>
void Rect<T>::Offset(const Vector2<T>& offset)
{
    Offset(offset.x, offset.y);
}


////////////////////////////////////////////////////////////
/// Check if a point is inside the rectangle's area
////////////////////////////////////////////////////////////
template <typename T>
bool Rect<T>::Contains(T x, T y) const
{
    return (x >= Left) && (x <= Right) && (y >= Top) && (y <= Bottom);
}


////////////////////////////////////////////////////////////
/// Check if a point is inside the rectangle's area
////////////////////////////////////////////////////////////
template <typename T>
bool Rect<T>::Contains(const Vector2<T>& point) const
{
    return Contains(point.x, point.y);
}


////////////////////////////////////////////////////////////
/// Check intersection between two rectangles
////////////////////////////////////////////////////////////
template <typename T>
bool Rect<T>::Intersects(const Rect<T>& rectangle) const
{
    // Compute overlapping rect
    Rect<T> overlapping(std::max(Left,   rectangle.Left),
                        std::max(Top,    rectangle.Top),
                        std::min(Right,  rectangle.Right),
                        std::min(Bottom, rectangle.Bottom));

    // If overlapping rect is valid, then there is intersection
    return (overlapping.Left < overlapping.Right) && (overlapping.Top < overlapping.Bottom);
}


////////////////////////////////////////////////////////////
/// Check intersection between two rectangles and return the
/// resulting rectangle
////////////////////////////////////////////////////////////
template <typename T>
bool Rect<T>::Intersects(const Rect<T>& rectangle, Rect<T>& intersection) const
{
    // Compute overlapping rect
    Rect<T> overlapping(std::max(Left,   rectangle.Left),
                        std::max(Top,    rectangle.Top),
                        std::min(Right,  rectangle.Right),
                        std::min(Bottom, rectangle.Bottom));

    // If overlapping rect is valid, then there is intersection
    if ((overlapping.Left < overlapping.Right) && (overlapping.Top < overlapping.Bottom))
    {
        intersection = overlapping;
        return true;
    }
    else
    {
        intersection = Rect<T>(0, 0, 0, 0);
        return false;
    }
}
