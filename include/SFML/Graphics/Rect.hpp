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

#ifndef SFML_RECT_HPP
#define SFML_RECT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Vector2.hpp>
#include <algorithm>


namespace sf
{
////////////////////////////////////////////////////////////
/// Rect is an utility class for manipulating rectangles.
/// Template parameter defines the type of coordinates (integer, float, ...)
////////////////////////////////////////////////////////////
template <typename T>
class Rect
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Rect();

    ////////////////////////////////////////////////////////////
    /// Construct the rectangle from its coordinates
    ///
    /// \param left :   Left coordinate of the rectangle
    /// \param top :    Top coordinate of the rectangle
    /// \param right :  Right coordinate of the rectangle
    /// \param bottom : Bottom coordinate of the rectangle
    ///
    ////////////////////////////////////////////////////////////
    Rect(T left, T top, T right, T bottom);

    ////////////////////////////////////////////////////////////
    /// Get the size of the rectangle
    ///
    /// \return Size of rectangle
    ///
    ////////////////////////////////////////////////////////////
    Vector2<T> GetSize() const;

    ////////////////////////////////////////////////////////////
    /// Get the center of the rectangle
    ///
    /// \return Center of rectangle
    ///
    ////////////////////////////////////////////////////////////
    Vector2<T> GetCenter() const;

    ////////////////////////////////////////////////////////////
    /// Move the whole rectangle by the given offset
    ///
    /// \param offsetX : Horizontal offset
    /// \param offsetY : Vertical offset
    ///
    ////////////////////////////////////////////////////////////
    void Offset(T offsetX, T offsetY);

    ////////////////////////////////////////////////////////////
    /// Move the whole rectangle by the given offset
    ///
    /// \param offset : Offset to apply to the current position
    ///
    ////////////////////////////////////////////////////////////
    void Offset(const Vector2<T>& offset);

    ////////////////////////////////////////////////////////////
    /// Check if a point is inside the rectangle's area
    ///
    /// \param x : X coordinate of the point to test
    /// \param y : Y coordinate of the point to test
    ///
    /// \return True if the point is inside
    ///
    ////////////////////////////////////////////////////////////
    bool Contains(T x, T y) const;

    ////////////////////////////////////////////////////////////
    /// Check if a point is inside the rectangle's area
    ///
    /// \param point : Point to test
    ///
    /// \return True if the point is inside
    ///
    ////////////////////////////////////////////////////////////
    bool Contains(const Vector2<T>& point) const;

    ////////////////////////////////////////////////////////////
    /// Check intersection between two rectangles
    ///
    /// \param rectangle : Rectangle to test
    ///
    /// \return True if rectangles overlap
    ///
    ////////////////////////////////////////////////////////////
    bool Intersects(const Rect<T>& rectangle) const;

    ////////////////////////////////////////////////////////////
    /// Check intersection between two rectangles and return the
    /// resulting rectangle
    ///
    /// \param rectangle :    Rectangle to test
    /// \param intersection : Rectangle to be filled with the intersection of both rectangles
    ///
    /// \return True if rectangles overlap
    ///
    ////////////////////////////////////////////////////////////
    bool Intersects(const Rect<T>& rectangle, Rect<T>& intersection) const;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    T Left;   ///< Left coordinate of the rectangle
    T Top;    ///< Top coordinate of the rectangle
    T Right;  ///< Right coordinate of the rectangle
    T Bottom; ///< Bottom coordinate of the rectangle
};

#include <SFML/Graphics/Rect.inl>

// Define the most common types
typedef Rect<int>   IntRect;
typedef Rect<float> FloatRect;

} // namespace sf


#endif // SFML_RECT_HPP
