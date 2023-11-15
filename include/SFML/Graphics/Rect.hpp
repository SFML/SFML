////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Vector2.hpp>

#include <optional>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Utility class for manipulating 2D axis aligned rectangles
///
////////////////////////////////////////////////////////////
template <typename T>
class Rect
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty rectangle (it is equivalent to calling
    /// Rect({0, 0}, {0, 0})).
    ///
    ////////////////////////////////////////////////////////////
    constexpr Rect();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the rectangle from position and size
    ///
    /// Be careful, the last parameter is the size,
    /// not the bottom-right corner!
    ///
    /// \param position Position of the top-left corner of the rectangle
    /// \param size     Size of the rectangle
    ///
    ////////////////////////////////////////////////////////////
    constexpr Rect(const Vector2<T>& position, const Vector2<T>& size);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the rectangle from another type of rectangle
    ///
    /// This constructor doesn't replace the copy constructor,
    /// it's called only when U != T.
    /// A call to this constructor will fail to compile if U
    /// is not convertible to T.
    ///
    /// \param rectangle Rectangle to convert
    ///
    ////////////////////////////////////////////////////////////
    template <typename U>
    constexpr explicit Rect(const Rect<U>& rectangle);

    ////////////////////////////////////////////////////////////
    /// \brief Check if a point is inside the rectangle's area
    ///
    /// This check is non-inclusive. If the point lies on the
    /// edge of the rectangle, this function will return false.
    ///
    /// \param point Point to test
    ///
    /// \return True if the point is inside, false otherwise
    ///
    /// \see findIntersection
    ///
    ////////////////////////////////////////////////////////////
    constexpr bool contains(const Vector2<T>& point) const;

    ////////////////////////////////////////////////////////////
    /// \brief Check the intersection between two rectangles
    ///
    /// \param rectangle Rectangle to test
    ///
    /// \return Intersection rectangle if intersecting, std::nullopt otherwise
    ///
    /// \see contains
    ///
    ////////////////////////////////////////////////////////////
    constexpr std::optional<Rect<T>> findIntersection(const Rect<T>& rectangle) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the position of the rectangle's top-left corner
    ///
    /// \return Position of rectangle
    ///
    /// \see getSize, getCenter
    ///
    ////////////////////////////////////////////////////////////
    constexpr Vector2<T> getPosition() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the size of the rectangle
    ///
    /// \return Size of rectangle
    ///
    /// \see getPosition, getCenter
    ///
    ////////////////////////////////////////////////////////////
    constexpr Vector2<T> getSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the position of the center of the rectangle
    ///
    /// \return Center of rectangle
    ///
    /// \see getSize, getPosition
    ///
    ////////////////////////////////////////////////////////////
    constexpr Vector2<T> getCenter() const;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    T left{};   //!< Left coordinate of the rectangle
    T top{};    //!< Top coordinate of the rectangle
    T width{};  //!< Width of the rectangle
    T height{}; //!< Height of the rectangle
};

////////////////////////////////////////////////////////////
/// \relates Rect
/// \brief Overload of binary operator ==
///
/// This operator compares strict equality between two rectangles.
///
/// \param left  Left operand (a rectangle)
/// \param right Right operand (a rectangle)
///
/// \return True if \a left is equal to \a right
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr bool operator==(const Rect<T>& left, const Rect<T>& right);

////////////////////////////////////////////////////////////
/// \relates Rect
/// \brief Overload of binary operator !=
///
/// This operator compares strict difference between two rectangles.
///
/// \param left  Left operand (a rectangle)
/// \param right Right operand (a rectangle)
///
/// \return True if \a left is not equal to \a right
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr bool operator!=(const Rect<T>& left, const Rect<T>& right);

#include <SFML/Graphics/Rect.inl>

// Create type aliases for the most common types
using IntRect   = Rect<int>;
using FloatRect = Rect<float>;

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::Rect
/// \ingroup graphics
///
/// A rectangle is defined by its top-left corner and its size.
/// It is a very simple class defined for convenience, so
/// its member variables (left, top, width and height) are public
/// and can be accessed directly, just like the vector classes
/// (Vector2 and Vector3).
///
/// To keep things simple, sf::Rect doesn't define
/// functions to emulate the properties that are not directly
/// members (such as right, bottom, center, etc.), it rather
/// only provides intersection functions.
///
/// sf::Rect uses the usual rules for its boundaries:
/// \li The left and top edges are included in the rectangle's area
/// \li The right (left + width) and bottom (top + height) edges are excluded from the rectangle's area
///
/// This means that sf::IntRect({0, 0}, {1, 1}) and sf::IntRect({1, 1}, {1, 1})
/// don't intersect.
///
/// sf::Rect is a template and may be used with any numeric type, but
/// for simplicity type aliases for the instantiations used by SFML are given:
/// \li sf::Rect<int> is sf::IntRect
/// \li sf::Rect<float> is sf::FloatRect
///
/// So that you don't have to care about the template syntax.
///
/// Usage example:
/// \code
/// // Define a rectangle, located at (0, 0) with a size of 20x5
/// sf::IntRect r1({0, 0}, {20, 5});
///
/// // Define another rectangle, located at (4, 2) with a size of 18x10
/// sf::Vector2i position(4, 2);
/// sf::Vector2i size(18, 10);
/// sf::IntRect r2(position, size);
///
/// // Test intersections with the point (3, 1)
/// bool b1 = r1.contains({3, 1}); // true
/// bool b2 = r2.contains({3, 1}); // false
///
/// // Test the intersection between r1 and r2
/// std::optional<sf::IntRect> result = r1.findIntersection(r2);
/// // result.has_value() == true
/// // result.value() == (4, 2, 16, 3)
/// \endcode
///
////////////////////////////////////////////////////////////
