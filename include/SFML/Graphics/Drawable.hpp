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

#ifndef SFML_DRAWABLE_HPP
#define SFML_DRAWABLE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Matrix3.hpp>


namespace sf
{
class RenderQueue;
class RenderTarget;

////////////////////////////////////////////////////////////
/// Enumerate the blending modes for drawable objects
////////////////////////////////////////////////////////////
namespace Blend
{
    enum Mode
    {
        Alpha,    ///< Pixel = Src * a + Dest * (1 - a)
        Add,      ///< Pixel = Src + Dest
        Multiply, ///< Pixel = Src * Dest
        None      ///< No blending
    };
}

////////////////////////////////////////////////////////////
/// Abstract base class for every object that can be drawn
/// into a render window
////////////////////////////////////////////////////////////
class SFML_API Drawable
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    /// \param position : Position of the object ((0, 0) by default)
    /// \param scale :    Scale factor ((1, 1) by default)
    /// \param rotation : Orientation, in degrees (0 by default)
    /// \param color :    Color of the object (white by default)
    ///
    ////////////////////////////////////////////////////////////
    Drawable(const Vector2f& position = Vector2f(0, 0), const Vector2f& scale = Vector2f(1, 1), float rotation = 0.f, const Color& color = Color(255, 255, 255));

    ////////////////////////////////////////////////////////////
    /// Virtual destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~Drawable();

    ////////////////////////////////////////////////////////////
    /// Set the position of the object (take 2 values)
    ///
    /// \param x : New X coordinate
    /// \param y : New Y coordinate
    ///
    ////////////////////////////////////////////////////////////
    void SetPosition(float x, float y);

    ////////////////////////////////////////////////////////////
    /// Set the position of the object (take a 2D vector)
    ///
    /// \param position : New position
    ///
    ////////////////////////////////////////////////////////////
    void SetPosition(const Vector2f& position);

    ////////////////////////////////////////////////////////////
    /// Set the X position of the object
    ///
    /// \param x : New X coordinate
    ///
    ////////////////////////////////////////////////////////////
    void SetX(float x);

    ////////////////////////////////////////////////////////////
    /// Set the Y position of the object
    ///
    /// \param y : New Y coordinate
    ///
    ////////////////////////////////////////////////////////////
    void SetY(float y);

    ////////////////////////////////////////////////////////////
    /// Set the scale of the object (take 2 values)
    ///
    /// \param factorX : New horizontal scale (must be strictly positive)
    /// \param factorY : New vertical scale (must be strictly positive)
    ///
    ////////////////////////////////////////////////////////////
    void SetScale(float factorX, float factorY);

    ////////////////////////////////////////////////////////////
    /// Set the scale of the object (take a 2D vector)
    ///
    /// \param Scale : New scale (both values must be strictly positive)
    ///
    ////////////////////////////////////////////////////////////
    void SetScale(const Vector2f& scale);

    ////////////////////////////////////////////////////////////
    /// Set the X scale factor of the object
    ///
    /// \param factor : New X scale factor
    ///
    ////////////////////////////////////////////////////////////
    void SetScaleX(float factor);

    ////////////////////////////////////////////////////////////
    /// Set the Y scale factor of the object
    ///
    /// \param factor : New Y scale factor
    ///
    ////////////////////////////////////////////////////////////
    void SetScaleY(float factor);

    ////////////////////////////////////////////////////////////
    /// Set the local origin of the object, in coordinates relative to the
    /// top-left of the object (take 2 values).
    /// The default origin is (0, 0)
    ///
    /// \param x : X coordinate of the origin
    /// \param y : Y coordinate of the origin
    ///
    ////////////////////////////////////////////////////////////
    void SetOrigin(float x, float y);

    ////////////////////////////////////////////////////////////
    /// Set the local origin of the object, in coordinates relative to the
    /// top-left of the object (take a 2D vector).
    /// The default origin is (0, 0)
    ///
    /// \param origin : New origin
    ///
    ////////////////////////////////////////////////////////////
    void SetOrigin(const Vector2f& origin);

    ////////////////////////////////////////////////////////////
    /// Set the orientation of the object
    ///
    /// \param angle : Angle of rotation, in degrees
    ///
    ////////////////////////////////////////////////////////////
    void SetRotation(float angle);

    ////////////////////////////////////////////////////////////
    /// Set the color of the object.
    /// The default color is white
    ///
    /// \param color : New color
    ///
    ////////////////////////////////////////////////////////////
    void SetColor(const Color& color);

    ////////////////////////////////////////////////////////////
    /// Set the blending mode for the object.
    /// The default blend mode is Blend::Alpha
    ///
    /// \param mode : New blending mode
    ///
    ////////////////////////////////////////////////////////////
    void SetBlendMode(Blend::Mode mode);

    ////////////////////////////////////////////////////////////
    /// Get the position of the object
    ///
    /// \return Current position
    ///
    ////////////////////////////////////////////////////////////
    const Vector2f& GetPosition() const;

    ////////////////////////////////////////////////////////////
    /// Get the current scale of the object
    ///
    /// \return Current scale factor (always positive)
    ///
    ////////////////////////////////////////////////////////////
    const Vector2f& GetScale() const;

    ////////////////////////////////////////////////////////////
    /// Get the local origin of the object
    ///
    /// \return Current position of the origin
    ///
    ////////////////////////////////////////////////////////////
    const Vector2f& GetOrigin() const;

    ////////////////////////////////////////////////////////////
    /// Get the orientation of the object.
    /// Rotation is always in the range [0, 360]
    ///
    /// \return Current rotation, in degrees
    ///
    ////////////////////////////////////////////////////////////
    float GetRotation() const;

    ////////////////////////////////////////////////////////////
    /// Get the color of the object
    ///
    /// \return Current color
    ///
    ////////////////////////////////////////////////////////////
    const Color& GetColor() const;

    ////////////////////////////////////////////////////////////
    /// Get the current blending mode
    ///
    /// \return Current blending mode
    ///
    ////////////////////////////////////////////////////////////
    Blend::Mode GetBlendMode() const;

    ////////////////////////////////////////////////////////////
    /// Move the object of a given offset (take 2 values)
    ///
    /// \param offsetX : X offset
    /// \param offsetY : Y offset
    ///
    ////////////////////////////////////////////////////////////
    void Move(float offsetX, float offsetY);

    ////////////////////////////////////////////////////////////
    /// Move the object of a given offset (take a 2D vector)
    ///
    /// \param offset : Amount of units to move the object of
    ///
    ////////////////////////////////////////////////////////////
    void Move(const Vector2f& offset);

    ////////////////////////////////////////////////////////////
    /// Scale the object (take 2 values)
    ///
    /// \param factorX : Scaling factor on X (must be strictly positive)
    /// \param factorY : Scaling factor on Y (must be strictly positive)
    ///
    ////////////////////////////////////////////////////////////
    void Scale(float factorX, float factorY);

    ////////////////////////////////////////////////////////////
    /// Scale the object (take a 2D vector)
    ///
    /// \param factor : Scaling factors (both values must be strictly positive)
    ///
    ////////////////////////////////////////////////////////////
    void Scale(const Vector2f& factor);

    ////////////////////////////////////////////////////////////
    /// Rotate the object
    ///
    /// \param Angle : Angle of rotation, in degrees
    ///
    ////////////////////////////////////////////////////////////
    void Rotate(float Angle);

    ////////////////////////////////////////////////////////////
    /// Transform a point from global coordinates into local coordinates
    /// (ie it applies the inverse of object's origin, translation, rotation and scale to the point)
    ///
    /// \param point : Point to transform
    ///
    /// \return Transformed point
    ///
    ////////////////////////////////////////////////////////////
    sf::Vector2f TransformToLocal(const sf::Vector2f& point) const;

    ////////////////////////////////////////////////////////////
    /// Transform a point from local coordinates into global coordinates
    /// (ie it applies the object's origin, translation, rotation and scale to the point)
    ///
    /// \param point : Point to transform
    ///
    /// \return Transformed point
    ///
    ////////////////////////////////////////////////////////////
    sf::Vector2f TransformToGlobal(const sf::Vector2f& point) const;

protected :

    ////////////////////////////////////////////////////////////
    /// Get the transform matrix of the drawable
    ///
    /// \return Transform matrix
    ///
    ////////////////////////////////////////////////////////////
    const Matrix3& GetMatrix() const;

    ////////////////////////////////////////////////////////////
    /// Get the inverse transform matrix of the drawable
    ///
    /// \return Inverse transform matrix
    ///
    ////////////////////////////////////////////////////////////
    const Matrix3& GetInverseMatrix() const;

private :

    friend class RenderTarget;

    ////////////////////////////////////////////////////////////
    /// Draw the object into the specified render target
    ///
    /// \param target : Target into which render the object
    /// \param queue :  Render queue to add the rendering commands to
    ///
    ////////////////////////////////////////////////////////////
    void Draw(RenderTarget& target, RenderQueue& queue) const;

    ////////////////////////////////////////////////////////////
    /// Render the specific geometry of the object
    ///
    /// \param target : Target into which render the object
    /// \param queue :  Render queue to add the rendering commands to
    ///
    ////////////////////////////////////////////////////////////
    virtual void Render(RenderTarget& target, RenderQueue& queue) const = 0;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Vector2f        myPosition;      ///< Position of the object on screen
    Vector2f        myScale;         ///< Scale of the object
    Vector2f        myOrigin;        ///< Origin of translation / rotation / scaling of the object
    float           myRotation;      ///< Orientation of the object, in degrees
    Color           myColor;         ///< Overlay color of the object
    Blend::Mode     myBlendMode;     ///< Blending mode
    mutable bool    myNeedUpdate;    ///< Do we need to recompute the transform matrix ?
    mutable bool    myInvNeedUpdate; ///< Do we need to recompute the inverse transform matrix ?
    mutable Matrix3 myMatrix;        ///< Precomputed transform matrix gathering the translation / rotation / scale / center
    mutable Matrix3 myInvMatrix;     ///< Precomputed inverse transform matrix gathering the translation / rotation / scale / center
};

} // namespace sf


#endif // SFML_DRAWABLE_HPP
