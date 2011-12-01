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
    /// Creates a circle with a radius of 10.
    ///
    ////////////////////////////////////////////////////////////
    CircleShape();

    ////////////////////////////////////////////////////////////
    /// \brief Set the radius of the circle
    ///
    /// The default radius of a circle is 10.
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

private :

    ////////////////////////////////////////////////////////////
    /// \brief Get the number of points defining the shape
    ///
    /// \return Number of points of the shape
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int GetOutlinePointsCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get a point of the shape
    ///
    /// \param index Index of the point to get
    ///
    /// \return Index-th point of the shape
    ///
    ////////////////////////////////////////////////////////////
    virtual Vector2f GetOutlinePoint(unsigned int index) const;

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    float myRadius; ///< Radius of the circle
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
/// \see sf::Shape, sf::StarShape, sf::RectangleShape, sf::ConvexShape
///
////////////////////////////////////////////////////////////
