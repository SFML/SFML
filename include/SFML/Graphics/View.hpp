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

#ifndef SFML_VIEW_HPP
#define SFML_VIEW_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Matrix3.hpp>
#include <SFML/System/Vector2.hpp>


namespace sf
{
class RenderTarget;

////////////////////////////////////////////////////////////
/// This class defines a view (position, size, etc.) ;
/// you can consider it as a 2D camera
////////////////////////////////////////////////////////////
class SFML_API View
{
public :

    ////////////////////////////////////////////////////////////
    /// Construct the view from a rectangle
    ///
    /// \param ViewRect : Rectangle defining the position and size of the view (1000x1000 by default)
    ///
    ////////////////////////////////////////////////////////////
    explicit View(const FloatRect& ViewRect = FloatRect(0, 0, 1000, 1000));

    ////////////////////////////////////////////////////////////
    /// Construct the view from its center and half-size
    ///
    /// \param Center :   Center of the view
    /// \param HalfSize : Half-size of the view (from center to corner)
    ///
    ////////////////////////////////////////////////////////////
    View(const sf::Vector2f& Center, const sf::Vector2f& HalfSize);

    ////////////////////////////////////////////////////////////
    /// Change the center of the view (take 2 values)
    ///
    /// \param X : X coordinate of the new center
    /// \param Y : Y coordinate of the new center
    ///
    ////////////////////////////////////////////////////////////
    void SetCenter(float X, float Y);

    ////////////////////////////////////////////////////////////
    /// Change the center of the view (take a vector)
    ///
    /// \param Center : New center
    ///
    ////////////////////////////////////////////////////////////
    void SetCenter(const sf::Vector2f& Center);

    ////////////////////////////////////////////////////////////
    /// Change the half-size of the view (take 2 values)
    ///
    /// \param HalfWidth :  New half-width
    /// \param HalfHeight : New half-height
    ///
    ////////////////////////////////////////////////////////////
    void SetHalfSize(float HalfWidth, float HalfHeight);

    ////////////////////////////////////////////////////////////
    /// Change the half-size of the view (take a vector)
    ///
    /// \param HalfSize : New half-size
    ///
    ////////////////////////////////////////////////////////////
    void SetHalfSize(const sf::Vector2f& HalfSize);

    ////////////////////////////////////////////////////////////
    /// Rebuild the view from a rectangle
    ///
    /// \param ViewRect : Rectangle defining the position and size of the view
    ///
    ////////////////////////////////////////////////////////////
    void SetFromRect(const FloatRect& ViewRect);

    ////////////////////////////////////////////////////////////
    /// Get the center of the view
    ///
    /// \return Center of the view
    ///
    ////////////////////////////////////////////////////////////
    const sf::Vector2f& GetCenter() const;

    ////////////////////////////////////////////////////////////
    /// Get the half-size of the view
    ///
    /// \return Half-size of the view
    ///
    ////////////////////////////////////////////////////////////
    const sf::Vector2f& GetHalfSize() const;

    ////////////////////////////////////////////////////////////
    /// Get the bounding rectangle of the view
    ///
    /// \return Bounding rectangle of the view
    ///
    ////////////////////////////////////////////////////////////
    const sf::FloatRect& GetRect() const;

    ////////////////////////////////////////////////////////////
    /// Move the view (take 2 values)
    ///
    /// \param OffsetX : Offset to move the view, on X axis
    /// \param OffsetY : Offset to move the view, on Y axis
    ///
    ////////////////////////////////////////////////////////////
    void Move(float OffsetX, float OffsetY);

    ////////////////////////////////////////////////////////////
    /// Move the view (take a vector)
    ///
    /// \param Offset : Offset to move the view
    ///
    ////////////////////////////////////////////////////////////
    void Move(const sf::Vector2f& Offset);

    ////////////////////////////////////////////////////////////
    /// Resize the view rectangle to simulate a zoom / unzoom effect
    ///
    /// \param Factor : Zoom factor to apply, relative to the current zoom
    ///
    ////////////////////////////////////////////////////////////
    void Zoom(float Factor);

private :

    friend class RenderTarget;

    ////////////////////////////////////////////////////////////
    /// Get the projection matrix of the view
    ///
    /// \return Projection matrix containing the view settings
    ///
    ////////////////////////////////////////////////////////////
    const Matrix3& GetMatrix() const;

    ////////////////////////////////////////////////////////////
    /// Recompute the view rectangle and the projection matrix
    ///
    ////////////////////////////////////////////////////////////
    void RecomputeMatrix();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    sf::Vector2f myCenter;     ///< Center of the view
    sf::Vector2f myHalfSize;   ///< Half-size of the view
    FloatRect    myRect;       ///< Rectangle defining the bounds of the view
    Matrix3      myMatrix;     ///< Precomputed projection matrix corresponding to the view
    bool         myNeedUpdate; ///< Internal state telling if the matrix needs to be updated
};

} // namespace sf


#endif // SFML_VIEW_HPP
