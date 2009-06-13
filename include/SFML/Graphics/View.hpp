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
////////////////////////////////////////////////////////////
/// This class defines a view (position, size, etc.);
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
    /// Construct the view from its center and size
    ///
    /// \param Center :   Center of the view
    /// \param HalfSize : Size of the view
    ///
    ////////////////////////////////////////////////////////////
    View(const Vector2f& Center, const Vector2f& Size);

    ////////////////////////////////////////////////////////////
    /// Set the rectangle defining the view by its center and size
    ///
    /// \param Center : Center of the view rectangle
    /// \param Size :   Size of the view rectangle
    ///
    ////////////////////////////////////////////////////////////
    void SetRect(const Vector2f& Center, const Vector2f& Size);

    ////////////////////////////////////////////////////////////
    /// Set the rectangle defining the view
    ///
    /// \param Rect : Rectangle defining the center and size of the view
    ///
    ////////////////////////////////////////////////////////////
    void SetRect(const FloatRect& Rect);

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
    void SetCenter(const Vector2f& Center);

    ////////////////////////////////////////////////////////////
    /// Change the size of the view (take 2 values)
    ///
    /// \param Width :  New width
    /// \param Height : New height
    ///
    ////////////////////////////////////////////////////////////
    void SetSize(float Width, float Height);

    ////////////////////////////////////////////////////////////
    /// Change the size of the view (take a vector)
    ///
    /// \param HalfSize : New half-size
    ///
    ////////////////////////////////////////////////////////////
    void SetSize(const Vector2f& Size);

    ////////////////////////////////////////////////////////////
    /// Get the rectangle defining the view,
    /// which is the combined center and size.
    ///
    /// \return View rectangle
    ///
    ////////////////////////////////////////////////////////////
    FloatRect GetRect() const;

    ////////////////////////////////////////////////////////////
    /// Get the center of the view
    ///
    /// \return Center of the view
    ///
    ////////////////////////////////////////////////////////////
    const Vector2f& GetCenter() const;

    ////////////////////////////////////////////////////////////
    /// Get the size of the view
    ///
    /// \return Size of the view
    ///
    ////////////////////////////////////////////////////////////
    const Vector2f& GetSize() const;

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
    void Move(const Vector2f& Offset);

    ////////////////////////////////////////////////////////////
    /// Resize the view rectangle to simulate a zoom / unzoom effect
    ///
    /// \param Factor : Zoom factor to apply, relative to the current zoom
    ///
    ////////////////////////////////////////////////////////////
    void Zoom(float Factor);

    ////////////////////////////////////////////////////////////
    /// Get the projection matrix of the view
    ///
    /// \return Projection matrix
    ///
    ////////////////////////////////////////////////////////////
    const Matrix3& GetMatrix() const;

    ////////////////////////////////////////////////////////////
    /// Get the inverse projection matrix of the view
    ///
    /// \return Inverse projection matrix
    ///
    ////////////////////////////////////////////////////////////
    const Matrix3& GetInverseMatrix() const;

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    sf::Vector2f    myCenter;        ///< Center of the view
    sf::Vector2f    mySize;          ///< Size of the view
    mutable Matrix3 myMatrix;        ///< Precomputed projection matrix corresponding to the view
    mutable Matrix3 myInverseMatrix; ///< Precomputed inverse projection matrix corresponding to the view
    mutable bool    myNeedUpdate;    ///< Internal state telling if the matrix needs to be updated
    mutable bool    myNeedInvUpdate; ///< Internal state telling if the matrix needs to be updated
};

} // namespace sf


#endif // SFML_VIEW_HPP
