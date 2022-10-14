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

#ifndef SFML_RECT_HPP
#define SFML_RECT_HPP

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
    /// \brief Default constructor. Creates an empty rectangle
    ///
    ////////////////////////////////////////////////////////////
    constexpr Rect();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the rectangle from position and size
    ///
    /// \param position Position of the rectangle
    /// \param size     Size of the rectangle
    ///
    ////////////////////////////////////////////////////////////
    constexpr Rect(const Vector2<T>& position, const Vector2<T>& size);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the rectangle from another type of rectangle
    ///
    /// This constructor doesn't replace the copy constructor.
    ///
    /// \param rectangle Rectangle to convert
    ///
    ////////////////////////////////////////////////////////////
    template <typename U>
    constexpr explicit Rect(const Rect<U>& rectangle);

    ////////////////////////////////////////////////////////////
    /// \brief Get the left edge of the rectangle
    ///
    /// \return Coordinate of the left edge of the rectangle
    ///
    ////////////////////////////////////////////////////////////
    constexpr T getLeft() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the top edge of the rectangle
    ///
    /// \return Coordinate of the top edge of the rectangle
    ///
    ////////////////////////////////////////////////////////////
    constexpr T getTop() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the right edge of the rectangle
    ///
    /// \return Coordinate of the right edge of the rectangle
    ///
    ////////////////////////////////////////////////////////////
    constexpr T getRight() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the bottom edge of the rectangle
    ///
    /// \return Coordinate of the bottom edge of the rectangle
    ///
    ////////////////////////////////////////////////////////////
    constexpr T getBottom() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the top-left point of the rectangle
    ///
    /// \return Top-left point of the rectangle
    ///
    ////////////////////////////////////////////////////////////
    constexpr Vector2<T> getStart() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the bottom-right point of the rectangle
    ///
    /// \return Bottom-right point of the rectangle
    ///
    ////////////////////////////////////////////////////////////
    constexpr Vector2<T> getEnd() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the center point of the rectangle
    ///
    /// \return Center point of the rectangle
    ///
    ////////////////////////////////////////////////////////////
    constexpr Vector2<T> getCenter() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the absolute size of the rectangle
    ///
    /// \return Absolute size of the rectangle
    ///
    ////////////////////////////////////////////////////////////
    constexpr Vector2<T> getAbsoluteSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get positive sized representaion of the rectangle
    ///
    /// \return Same rectangle, represented with positive size
    ///
    ////////////////////////////////////////////////////////////
    constexpr Rect<T> getAbsoluteRect() const;

    ////////////////////////////////////////////////////////////
    /// \brief Check if a point is inside the rectangle's area
    ///
    /// This check includes the left and top edges of the rectangle
    /// as it's area, but excludes right and bottom edges from it.
    ///
    /// \param point Point to check
    ///
    /// \return True if the point is inside the rectangle, false otherwise
    ///
    /// \see findIntersection
    ///
    ////////////////////////////////////////////////////////////
    constexpr bool contains(const Vector2<T>& point) const;

    ////////////////////////////////////////////////////////////
    /// \brief Find the intersection between two rectangles
    ///
    /// \param rectangle Rectangle to find the intersection with
    ///
    /// \return Intersection rectangle if intersects, std::nullopt otherwise
    ///
    /// \see contains
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] constexpr std::optional<Rect<T>> findIntersection(const Rect<T>& rectangle) const;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Vector2<T> position; //!< Position of the rectangle
    Vector2<T> size;     //!< Size of the rectangle
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


#endif // SFML_RECT_HPP


////////////////////////////////////////////////////////////
/// \class sf::Rect
/// \ingroup graphics
///
/// A rectangle is defined by its sf::Vector2 position and sf::Vector2 size
///
/// sf::Rect uses the following rules for its boundaries:
/// \li The left and top edges are included in the rectangle's area
/// \li The right and bottom edges are excluded from the rectangle's area
///
/// sf::Rect is a template and may be used with any numeric type, but
/// for simplicity the following type aliases are given:
/// \li sf::Rect<int> is sf::IntRect
/// \li sf::Rect<float> is sf::FloatRect
///
/// Usage example:
/// \code
/// // Define a rectangle, located at {0, 0} with a size of {10, 20}
/// sf::IntRect rect1({0, 0}, {10, 20});
///
/// // Be careful, negative size is allowed!
/// sf::IntRect rect2({5, 5}, {-10, -20});
///
/// // Test intersections with a point
/// bool test1 = rect1.contains({9, 9}); // true
/// bool test2 = rect1.contains({10, 10}); // false
///
/// // Test intersection with another rectangle
/// std::optional<sf::IntRect> result = rect1.findIntersection(rect2);
/// // result.has_value() == true
/// // result.value() == sf::IntRect({0, 0}, {5, 5})
/// \endcode
///
////////////////////////////////////////////////////////////
