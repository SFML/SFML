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

#ifndef SFML_CIRCLESHAPE_HPP
#define SFML_CIRCLESHAPE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Shape.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Specialized shape representing a circle
///
////////////////////////////////////////////////////////////
class SFML_API CircleShape : public Shape
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// \param radius     Radius of the circle
    /// \param pointCount Number of points composing the circle
    ///
    ////////////////////////////////////////////////////////////
    explicit CircleShape(float radius = 0, unsigned int pointCount = 30);

    ////////////////////////////////////////////////////////////
    /// \brief Set the radius of the circle
    ///
    /// \param radius New radius of the circle
    ///
    /// \see GetRadius
    ///
    ////////////////////////////////////////////////////////////
    void SetRadius(float radius);

    ////////////////////////////////////////////////////////////
    /// \brief Get the radius of the circle
    ///
    /// \return Radius of the circle
    ///
    /// \see SetRadius
    ///
    ////////////////////////////////////////////////////////////
    float GetRadius() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the number of points of the circle
    ///
    /// \param count New number of points of the circle
    ///
    /// \see GetPointCount
    ///
    ////////////////////////////////////////////////////////////
    void SetPointCount(unsigned int count);

    ////////////////////////////////////////////////////////////
    /// \brief Get the number of points of the shape
    ///
    /// \return Number of points of the shape
    ///
    /// \see SetPointCount
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int GetPointCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get a point of the shape
    ///
    /// The result is undefined if \a index is out of the valid range.
    ///
    /// \param index Index of the point to get, in range [0 .. GetPointCount() - 1]
    ///
    /// \return Index-th point of the shape
    ///
    ////////////////////////////////////////////////////////////
    virtual Vector2f GetPoint(unsigned int index) const;

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    float        myRadius;     ///< Radius of the circle
    unsigned int myPointCount; ///< Number of points composing the circle
};

} // namespace sf


#endif // SFML_CIRCLESHAPE_HPP


////////////////////////////////////////////////////////////
/// \class sf::CircleShape
/// \ingroup graphics
///
/// This class inherits all the functions of sf::Transformable
/// (position, rotation, scale, bounds, ...) as well as the
/// functions of sf::Shape (outline, color, texture, ...).
///
/// Usage example:
/// \code
/// sf::CircleShape circle;
/// circle.SetRadius(150);
/// circle.SetOutlineColor(sf::Color::Red);
/// circle.SetOutlineThickness(5);
/// circle.SetPosition(10, 20);
/// ...
/// window.Draw(circle);
/// \endcode
///
/// Since the graphics card can't draw perfect circles, we have to
/// fake them with multiple triangles connected to each other. The
/// "points count" property of sf::CircleShape defines how many of these
/// triangles to use, and therefore defines the quality of the circle.
///
/// The number of points can also be used for another purpose; with
/// small numbers you can create any regular polygon shape:
/// equilateral triangle, square, pentagon, hexagon, ...
///
/// \see sf::Shape, sf::RectangleShape, sf::ConvexShape
///
////////////////////////////////////////////////////////////
