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
Rect<T>::Rect(T LeftCoord, T TopCoord, T RightCoord, T BottomCoord) :
Left  (LeftCoord),
Top   (TopCoord),
Right (RightCoord),
Bottom(BottomCoord)
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
void Rect<T>::Offset(T OffsetX, T OffsetY)
{
    Left   += OffsetX;
    Right  += OffsetX;
    Top    += OffsetY;
    Bottom += OffsetY;
}


////////////////////////////////////////////////////////////
/// Move the whole rectangle by the given offset
////////////////////////////////////////////////////////////
template <typename T>
void Rect<T>::Offset(const Vector2<T>& Off)
{
    Offset(Off.x, Off.y);
}


////////////////////////////////////////////////////////////
/// Check if a point is inside the rectangle's area
////////////////////////////////////////////////////////////
template <typename T>
bool Rect<T>::Contains(T X, T Y) const
{
    return (X >= Left) && (X <= Right) && (Y >= Top) && (Y <= Bottom);
}


////////////////////////////////////////////////////////////
/// Check if a point is inside the rectangle's area
////////////////////////////////////////////////////////////
template <typename T>
bool Rect<T>::Contains(const Vector2<T>& Point) const
{
    return Contains(Point.x, Point.y);
}


////////////////////////////////////////////////////////////
/// Check intersection between two rectangles
////////////////////////////////////////////////////////////
template <typename T>
bool Rect<T>::Intersects(const Rect<T>& Rectangle) const
{
    // Compute overlapping rect
    Rect<T> Overlapping(std::max(Left,   Rectangle.Left),
                        std::max(Top,    Rectangle.Top),
                        std::min(Right,  Rectangle.Right),
                        std::min(Bottom, Rectangle.Bottom));

    // If overlapping rect is valid, then there is intersection
    return (Overlapping.Left < Overlapping.Right) && (Overlapping.Top < Overlapping.Bottom);
}


////////////////////////////////////////////////////////////
/// Check intersection between two rectangles and return the
/// resulting rectangle
////////////////////////////////////////////////////////////
template <typename T>
bool Rect<T>::Intersects(const Rect<T>& Rectangle, Rect<T>& OverlappingRect) const
{
    // Compute overlapping rect
    Rect<T> Overlapping(std::max(Left,   Rectangle.Left),
                        std::max(Top,    Rectangle.Top),
                        std::min(Right,  Rectangle.Right),
                        std::min(Bottom, Rectangle.Bottom));

    // If overlapping rect is valid, then there is intersection
    if ((Overlapping.Left < Overlapping.Right) && (Overlapping.Top < Overlapping.Bottom))
    {
        OverlappingRect = Overlapping;
        return true;
    }
    else
    {
        OverlappingRect = Rect<T>(0, 0, 0, 0);
        return false;
    }
}
