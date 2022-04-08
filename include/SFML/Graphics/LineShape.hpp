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

#ifndef SFML_LINESHAPE_HPP
#define SFML_LINESHAPE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>

#include <SFML/Graphics/Shape.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Specialized shape representing a line
///
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API LineShape : public Shape
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// \param beginning Beginning of line
    /// \param end       End of line
    /// \param thickness Thickness of line
    ///
    ////////////////////////////////////////////////////////////
    LineShape(const Vector2f& beginning, const Vector2f& end, float thickness);

    ////////////////////////////////////////////////////////////
    /// \brief Set the thickness of the line
    ///
    /// \param size New thickness of the line
    ///
    /// \see getThickness
    ///
    ////////////////////////////////////////////////////////////
    void setThickness(float thickness);

    ////////////////////////////////////////////////////////////
    /// \brief Set the end point of the line
    ///
    /// \param size New end point of the line
    ///
    /// \see setEndPoint
    ///
    ////////////////////////////////////////////////////////////
    void setEndPoint(const Vector2f& point);

    ////////////////////////////////////////////////////////////
    /// \brief Get the thickness of the line
    ///
    /// \return Thickness of the line
    ///
    /// \see setThickness
    ///
    ////////////////////////////////////////////////////////////
    float getThickness() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the end point of the line
    ///
    /// \return End point of the line
    ///
    /// \see setEndPoint
    ///
    ////////////////////////////////////////////////////////////
    Vector2f getEndPoint() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the length of the line
    ///
    /// \return Length of the line
    ///
    ////////////////////////////////////////////////////////////
    float getLength() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the number of points defining the shape
    ///
    /// \return Number of points of the shape. For line
    ///         shapes, this number is always 4.
    ///
    ////////////////////////////////////////////////////////////
    std::size_t getPointCount() const override;

    ////////////////////////////////////////////////////////////
    /// \brief Get a point of the line
    ///
    /// The returned point is in local coordinates, that is,
    /// the shape's transforms (position, rotation, scale) are
    /// not taken into account.
    /// The result is undefined if \a index is out of the valid range.
    ///
    /// \param index Index of the point to get, in range [0 .. 3]
    ///
    /// \return index-th point of the shape
    ///
    ////////////////////////////////////////////////////////////
    Vector2f getPoint(std::size_t index) const override;

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Vector2f m_direction; //!< Direction of the line
    float    m_thickness; //!< Thickness of the line
};

} // namespace sf


#endif // SFML_LINESHAPE_HPP


////////////////////////////////////////////////////////////
/// \class sf::LineShape
/// \ingroup graphics
///
/// This class inherits all the functions of sf::Transformable
/// (position, rotation, scale, bounds, ...) as well as the
/// functions of sf::Shape (outline, color, texture, ...).
///
/// Usage example:
/// \code
/// sf::LineShape line({0, 0}, {10, 10}, 10.f);
/// line.setOutlineColor(sf::Color::Red);
/// line.setOutlineThickness(5);
/// ...
/// window.draw(line);
/// \endcode
///
/// \see sf::Shape, sf::CircleShape, sf::ConvexShape, sf::RectangleShape
///
////////////////////////////////////////////////////////////
