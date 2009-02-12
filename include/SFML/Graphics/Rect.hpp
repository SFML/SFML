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
    /// \param LeftCoord :   Left coordinate of the rectangle
    /// \param TopCoord :    Top coordinate of the rectangle
    /// \param RightCoord :  Right coordinate of the rectangle
    /// \param BottomCoord : Bottom coordinate of the rectangle
    ///
    ////////////////////////////////////////////////////////////
    Rect(T LeftCoord, T TopCoord, T RightCoord, T BottomCoord);

    ////////////////////////////////////////////////////////////
    /// Get the width of the rectangle
    ///
    /// \return Width of rectangle
    ///
    ////////////////////////////////////////////////////////////
    T GetWidth() const;

    ////////////////////////////////////////////////////////////
    /// Get the height of the rectangle
    ///
    /// \return Height of rectangle
    ///
    ////////////////////////////////////////////////////////////
    T GetHeight() const;

    ////////////////////////////////////////////////////////////
    /// Move the whole rectangle by the given offset
    ///
    /// \param OffsetX : Horizontal offset
    /// \param OffsetY : Vertical offset
    ///
    ////////////////////////////////////////////////////////////
    void Offset(T OffsetX, T OffsetY);

    ////////////////////////////////////////////////////////////
    /// Check if a point is inside the rectangle's area
    ///
    /// \param X : X coordinate of the point to test
    /// \param Y : Y coordinate of the point to test
    ///
    /// \return True if the point is inside
    ///
    ////////////////////////////////////////////////////////////
    bool Contains(T X, T Y) const;

    ////////////////////////////////////////////////////////////
    /// Check intersection between two rectangles
    ///
    /// \param Rectangle :       Rectangle to test
    /// \param OverlappingRect : Rectangle to be filled with overlapping rect (NULL by default)
    ///
    /// \return True if rectangles overlap
    ///
    ////////////////////////////////////////////////////////////
    bool Intersects(const Rect<T>& Rectangle, Rect<T>* OverlappingRect = NULL) const;

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
