////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/Export.hpp>

#include <SFML/Graphics/Shape.hpp>

#include <vector>

#include <cstddef>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Specialized shape representing a convex polygon
///
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API ConvexShape : public Shape
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// \param pointCount Number of points of the polygon
    ///
    ////////////////////////////////////////////////////////////
    explicit ConvexShape(std::size_t pointCount = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Set the number of points of the polygon
    ///
    /// For the shape to be rendered as expected, `count` must
    /// be greater or equal to 3.
    ///
    /// \param count New number of points of the polygon
    ///
    /// \see `getPointCount`
    ///
    ////////////////////////////////////////////////////////////
    void setPointCount(std::size_t count);

    ////////////////////////////////////////////////////////////
    /// \brief Get the number of points of the polygon
    ///
    /// \return Number of points of the polygon
    ///
    /// \see `setPointCount`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::size_t getPointCount() const override;

    ////////////////////////////////////////////////////////////
    /// \brief Set the position of a point
    ///
    /// Don't forget that the shape must be convex and the
    /// order of points matters. Points should not overlap.
    /// This applies to rendering; it is explicitly allowed
    /// to temporarily have non-convex or degenerate shapes
    /// when not drawn (e.g. during shape initialization).
    ///
    /// Point count must be specified beforehand. The behavior is
    /// undefined if `index` is greater than or equal to getPointCount.
    ///
    /// \param index Index of the point to change, in range [0 .. getPointCount() - 1]
    /// \param point New position of the point
    ///
    /// \see `getPoint`
    ///
    ////////////////////////////////////////////////////////////
    void setPoint(std::size_t index, Vector2f point);

    ////////////////////////////////////////////////////////////
    /// \brief Get the position of a point
    ///
    /// The returned point is in local coordinates, that is,
    /// the shape's transforms (position, rotation, scale) are
    /// not taken into account.
    /// The result is undefined if `index` is out of the valid range.
    ///
    /// \param index Index of the point to get, in range [0 .. getPointCount() - 1]
    ///
    /// \return Position of the `index`-th point of the polygon
    ///
    /// \see `setPoint`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Vector2f getPoint(std::size_t index) const override;

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::vector<Vector2f> m_points; //!< Points composing the convex polygon
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::ConvexShape
/// \ingroup graphics
///
/// This class inherits all the functions of `sf::Transformable`
/// (position, rotation, scale, bounds, ...) as well as the
/// functions of `sf::Shape` (outline, color, texture, ...).
///
/// It is important to keep in mind that a convex shape must
/// always be... convex, otherwise it may not be drawn correctly.
/// Moreover, the points must be defined in order; using a random
/// order would result in an incorrect shape.
///
/// Usage example:
/// \code
/// sf::ConvexShape polygon;
/// polygon.setPointCount(3);
/// polygon.setPoint(0, sf::Vector2f(0, 0));
/// polygon.setPoint(1, sf::Vector2f(0, 10));
/// polygon.setPoint(2, sf::Vector2f(25, 5));
/// polygon.setOutlineColor(sf::Color::Red);
/// polygon.setOutlineThickness(5);
/// polygon.setPosition({10, 20});
/// ...
/// window.draw(polygon);
/// \endcode
///
/// \see `sf::Shape`, `sf::RectangleShape`, `sf::CircleShape`
///
////////////////////////////////////////////////////////////
