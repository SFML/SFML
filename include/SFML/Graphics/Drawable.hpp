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
class Renderer;
class RenderTarget;

////////////////////////////////////////////////////////////
/// \brief Available blending modes for drawable objects
///
////////////////////////////////////////////////////////////
namespace Blend
{
    enum Mode
    {
        Alpha,    ///< Pixel = Src * Src.a + Dest * (1 - Src.a)
        Add,      ///< Pixel = Src + Dest
        Multiply, ///< Pixel = Src * Dest
        None      ///< Pixel = Src
    };
}

////////////////////////////////////////////////////////////
/// \brief Abstract base class for objects that can be drawn
///        to a render target
///
////////////////////////////////////////////////////////////
class SFML_API Drawable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// \param position Position of the object
    /// \param scale    Scale factor
    /// \param rotation Orientation, in degrees
    /// \param color    Global color of the object
    ///
    ////////////////////////////////////////////////////////////
    Drawable(const Vector2f& position = Vector2f(0, 0), const Vector2f& scale = Vector2f(1, 1), float rotation = 0.f, const Color& color = Color(255, 255, 255));

    ////////////////////////////////////////////////////////////
    /// \brief Virtual destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~Drawable();

    ////////////////////////////////////////////////////////////
    /// \brief Set the position of the object
    ///
    /// This function completely overwrites the previous position.
    /// See Move to apply an offset based on the previous position instead.
    /// The default position of a drawable object is (0, 0).
    ///
    /// \param x X coordinate of the new position
    /// \param y Y coordinate of the new position
    ///
    /// \see Move, SetX, SetY, GetPosition
    ///
    ////////////////////////////////////////////////////////////
    void SetPosition(float x, float y);

    ////////////////////////////////////////////////////////////
    /// \brief Set the position of the object
    ///
    /// This function completely overwrites the previous position.
    /// See Move to apply an offset based on the previous position instead.
    /// The default position of a drawable object is (0, 0).
    ///
    /// \param position New position
    ///
    /// \see Move, SetX, SetY, GetPosition
    ///
    ////////////////////////////////////////////////////////////
    void SetPosition(const Vector2f& position);

    ////////////////////////////////////////////////////////////
    /// \brief Set the X position of the object
    ///
    /// \param x New X coordinate
    ///
    /// \see SetY, SetPosition, GetPosition
    ///
    ////////////////////////////////////////////////////////////
    void SetX(float x);

    ////////////////////////////////////////////////////////////
    /// \brief Set the Y position of the object
    ///
    /// \param y New Y coordinate
    ///
    /// \see SetX, SetPosition, GetPosition
    ///
    ////////////////////////////////////////////////////////////
    void SetY(float y);

    ////////////////////////////////////////////////////////////
    /// \brief Set the scale factors of the object
    ///
    /// \a factorX and \a factorY must be strictly positive,
    /// otherwise they are ignored.
    /// This function completely overwrites the previous scale.
    /// See Scale to add a factor based on the previous scale instead.
    /// The default scale of a drawable object is (1, 1).
    ///
    /// \param factorX New horizontal scale factor
    /// \param factorY New vertical scale factor
    ///
    /// \see Scale, SetScaleX, SetScaleY, GetScale
    ///
    ////////////////////////////////////////////////////////////
    void SetScale(float factorX, float factorY);

    ////////////////////////////////////////////////////////////
    /// \brief Set the scale factors of the object
    ///
    /// \a scale.x and \a scale.y must be strictly positive,
    /// otherwise they are ignored.
    /// This function completely overwrites the previous scale.
    /// See Scale to add a factor based on the previous scale instead.
    /// The default scale of a drawable object is (1, 1).
    ///
    /// \param scale New scale factors
    ///
    /// \see Scale, SetScaleX, SetScaleY, GetScale
    ///
    ////////////////////////////////////////////////////////////
    void SetScale(const Vector2f& scale);

    ////////////////////////////////////////////////////////////
    /// \brief Set the X scale factor of the object
    ///
    /// \a factor must be strictly positive, otherwise it is ignored.
    ///
    /// \param factor New horizontal scale factor
    ///
    /// \see SetScaleY, SetScale, GetScale
    ///
    ////////////////////////////////////////////////////////////
    void SetScaleX(float factor);

    ////////////////////////////////////////////////////////////
    /// \brief Set the Y scale factor of the object
    ///
    /// \a factor must be strictly positive, otherwise it is ignored.
    ///
    /// \param factor New vertical scale factor
    ///
    /// \see SetScaleX, SetScale, GetScale
    ///
    ////////////////////////////////////////////////////////////
    void SetScaleY(float factor);

    ////////////////////////////////////////////////////////////
    /// \brief Set the local origin of the object
    ///
    /// The origin of an object defines the center point for
    /// all transformations (position, scale, rotation).
    /// The coordinates of this point must be relative to the
    /// top-left corner of the object, and ignore all
    /// transformations (position, scale, rotation).
    /// The default origin of a drawable object is (0, 0).
    ///
    /// \param x X coordinate of the new origin
    /// \param y Y coordinate of the new origin
    ///
    /// \see GetOrigin
    ///
    ////////////////////////////////////////////////////////////
    void SetOrigin(float x, float y);

    ////////////////////////////////////////////////////////////
    /// \brief Set the local origin of the object
    ///
    /// The origin of an object defines the center point for
    /// all transformations (position, scale, rotation).
    /// The coordinates of this point must be relative to the
    /// top-left corner of the object, and ignore all
    /// transformations (position, scale, rotation).
    /// The default origin of a drawable object is (0, 0).
    ///
    /// \param origin New origin
    ///
    /// \see GetOrigin
    ///
    ////////////////////////////////////////////////////////////
    void SetOrigin(const Vector2f& origin);

    ////////////////////////////////////////////////////////////
    /// \brief Set the orientation of the object
    ///
    /// This function completely overwrites the previous rotation.
    /// See Rotate to add an angle based on the previous rotation instead.
    /// The default rotation of a drawable object is 0.
    ///
    /// \param angle New rotation, in degrees
    ///
    /// \see Rotate, GetRotation
    ///
    ////////////////////////////////////////////////////////////
    void SetRotation(float angle);

    ////////////////////////////////////////////////////////////
    /// \brief Set the global color of the object
    ///
    /// This global color affects the entire object, and modulates
    /// (multiplies) its original pixels.
    ///
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
    Vector2f TransformToLocal(const Vector2f& point) const;

    ////////////////////////////////////////////////////////////
    /// Transform a point from local coordinates into global coordinates
    /// (ie it applies the object's origin, translation, rotation and scale to the point)
    ///
    /// \param point : Point to transform
    ///
    /// \return Transformed point
    ///
    ////////////////////////////////////////////////////////////
    Vector2f TransformToGlobal(const Vector2f& point) const;

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
    /// \param target :   Target into which render the object
    /// \param renderer : Renderer that processes the rendering commands
    ///
    ////////////////////////////////////////////////////////////
    void Draw(RenderTarget& target, Renderer& renderer) const;

    ////////////////////////////////////////////////////////////
    /// Render the specific geometry of the object
    ///
    /// \param target :   Target into which render the object
    /// \param renderer : Renderer that processes the rendering commands
    ///
    ////////////////////////////////////////////////////////////
    virtual void Render(RenderTarget& target, Renderer& renderer) const = 0;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Vector2f        myPosition;         ///< Position of the object on screen
    Vector2f        myScale;            ///< Scale of the object
    Vector2f        myOrigin;           ///< Origin of translation / rotation / scaling of the object
    float           myRotation;         ///< Orientation of the object, in degrees
    Color           myColor;            ///< Overlay color of the object
    Blend::Mode     myBlendMode;        ///< Blending mode
    mutable Matrix3 myMatrix;           ///< Precomputed transform matrix gathering the translation / rotation / scale / center
    mutable Matrix3 myInvMatrix;        ///< Precomputed inverse transform matrix gathering the translation / rotation / scale / center
    mutable bool    myMatrixUpdated;    ///< Do we need to recompute the transform matrix ?
    mutable bool    myInvMatrixUpdated; ///< Do we need to recompute the inverse transform matrix ?
};

} // namespace sf


#endif // SFML_DRAWABLE_HPP
