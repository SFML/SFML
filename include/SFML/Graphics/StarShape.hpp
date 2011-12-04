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

#ifndef SFML_STARSHAPE_HPP
#define SFML_STARSHAPE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Shape.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Specialized shape representing a star
///
////////////////////////////////////////////////////////////
class SFML_API StarShape : public Shape
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    StarShape();

    ////////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    /// \param innerRadius Inner radius of the star
    /// \param outerRadius Outer radius of the star
    /// \param pointsCount Number of points of the star
    ///
    ////////////////////////////////////////////////////////////
    StarShape(float innerRadius, float outerRadius, unsigned int pointsCount);

    ////////////////////////////////////////////////////////////
    /// \brief Set the inner radius of the star
    ///
    /// \param radius New inner radius of the star
    ///
    /// \see GetInnerRadius
    ///
    ////////////////////////////////////////////////////////////
    void SetInnerRadius(float radius);

    ////////////////////////////////////////////////////////////
    /// \brief Get the inner radius of the star
    ///
    /// \return Inner radius of the star
    ///
    /// \see SetInnerRadius
    ///
    ////////////////////////////////////////////////////////////
    float GetInnerRadius() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the outer radius of the star
    ///
    /// \param radius New outer radius of the star
    ///
    /// \see GetOuterRadius
    ///
    ////////////////////////////////////////////////////////////
    void SetOuterRadius(float radius);

    ////////////////////////////////////////////////////////////
    /// \brief Get the outer radius of the star
    ///
    /// \return Outer radius of the star
    ///
    /// \see SetOuterRadius
    ///
    ////////////////////////////////////////////////////////////
    float GetOuterRadius() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the number of points of the star
    ///
    /// \param count New number of points of the star
    ///
    /// \see GetPointsCount
    ///
    ////////////////////////////////////////////////////////////
    void SetPointsCount(unsigned int count);

    ////////////////////////////////////////////////////////////
    /// \brief Get the number of points of the star
    ///
    /// \return Number of points of the star
    ///
    /// \see SetPointsCount
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetPointsCount() const;

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
    float        myInnerRadius; ///< Inner radius of the star
    float        myOuterRadius; ///< Outer radius of the star
    unsigned int myPointsCount; ///< Number of points of the star
};

} // namespace sf


#endif // SFML_STARSHAPE_HPP


////////////////////////////////////////////////////////////
/// \class sf::StarShape
/// \ingroup graphics
///
/// This class inherits all the functions of sf::Transformable
/// (position, rotation, scale, bounds, ...) as well as the
/// functions of sf::Shape (outline, color, texture, ...).
///
/// Usage example:
/// \code
/// sf::StarShape star;
/// star.SetInnerRadius(25);
/// star.SetInnerRadius(40);
/// star.SetPointsCount(6);
/// star.SetOutlineColor(sf::Color::Red);
/// star.SetOutlineThickness(5);
/// star.SetPosition(10, 20);
/// ...
/// window.Draw(star);
/// \endcode
///
/// \see sf::Shape, sf::CircleShape, sf::RectangleShape, sf::ConvexShape
///
////////////////////////////////////////////////////////////
