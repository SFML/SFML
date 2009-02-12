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
    /// \param Position : Position of the object (0, 0 by default)
    /// \param Scale :    Scale factor (1, 1 by default)
    /// \param Rotation : Orientation, in degrees (0 by default)
    /// \param Col :      Color of the object (white by default)
    ///
    ////////////////////////////////////////////////////////////
    Drawable(const Vector2f& Position = Vector2f(0, 0), const Vector2f& Scale = Vector2f(1, 1), float Rotation = 0.f, const Color& Col = Color(255, 255, 255, 255));

    ////////////////////////////////////////////////////////////
    /// Virtual destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~Drawable();

    ////////////////////////////////////////////////////////////
    /// Set the position of the object (take 2 values)
    ///
    /// \param X : New X coordinate
    /// \param Y : New Y coordinate
    ///
    ////////////////////////////////////////////////////////////
    void SetPosition(float X, float Y);

    ////////////////////////////////////////////////////////////
    /// Set the position of the object (take a 2D vector)
    ///
    /// \param Position : New position
    ///
    ////////////////////////////////////////////////////////////
    void SetPosition(const Vector2f& Position);

    ////////////////////////////////////////////////////////////
    /// Set the X position of the object
    ///
    /// \param X : New X coordinate
    ///
    ////////////////////////////////////////////////////////////
    void SetX(float X);

    ////////////////////////////////////////////////////////////
    /// Set the Y position of the object
    ///
    /// \param Y : New Y coordinate
    ///
    ////////////////////////////////////////////////////////////
    void SetY(float Y);

    ////////////////////////////////////////////////////////////
    /// Set the scale of the object (take 2 values)
    ///
    /// \param ScaleX : New horizontal scale (must be strictly positive)
    /// \param ScaleY : New vertical scale (must be strictly positive)
    ///
    ////////////////////////////////////////////////////////////
    void SetScale(float ScaleX, float ScaleY);

    ////////////////////////////////////////////////////////////
    /// Set the scale of the object (take a 2D vector)
    ///
    /// \param Scale : New scale (both values must be strictly positive)
    ///
    ////////////////////////////////////////////////////////////
    void SetScale(const Vector2f& Scale);

    ////////////////////////////////////////////////////////////
    /// Set the X scale factor of the object
    ///
    /// \param X : New X scale factor
    ///
    ////////////////////////////////////////////////////////////
    void SetScaleX(float FactorX);

    ////////////////////////////////////////////////////////////
    /// Set the Y scale factor of the object
    ///
    /// \param Y : New Y scale factor
    ///
    ////////////////////////////////////////////////////////////
    void SetScaleY(float FactorY);

    ////////////////////////////////////////////////////////////
    /// Set the center of the object, in coordinates relative to the
    /// top-left of the object (take 2 values).
    /// The default center is (0, 0)
    ///
    /// \param CenterX : X coordinate of the center
    /// \param CenterY : Y coordinate of the center
    ///
    ////////////////////////////////////////////////////////////
    void SetCenter(float CenterX, float CenterY);

    ////////////////////////////////////////////////////////////
    /// Set the center of the object, in coordinates relative to the
    /// top-left of the object (take a 2D vector).
    /// The default center is (0, 0)
    ///
    /// \param Center : New center
    ///
    ////////////////////////////////////////////////////////////
    void SetCenter(const Vector2f& Center);

    ////////////////////////////////////////////////////////////
    /// Set the orientation of the object
    ///
    /// \param Rotation : Angle of rotation, in degrees
    ///
    ////////////////////////////////////////////////////////////
    void SetRotation(float Rotation);

    ////////////////////////////////////////////////////////////
    /// Set the color of the object.
    /// The default color is white
    ///
    /// \param Col : New color
    ///
    ////////////////////////////////////////////////////////////
    void SetColor(const Color& Col);

    ////////////////////////////////////////////////////////////
    /// Set the blending mode for the object.
    /// The default blend mode is Blend::Alpha
    ///
    /// \param Mode : New blending mode
    ///
    ////////////////////////////////////////////////////////////
    void SetBlendMode(Blend::Mode Mode);

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
    /// Get the center of the object
    ///
    /// \return Current position of the center
    ///
    ////////////////////////////////////////////////////////////
    const Vector2f& GetCenter() const;

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
    /// \param OffsetX : X offset
    /// \param OffsetY : Y offset
    ///
    ////////////////////////////////////////////////////////////
    void Move(float OffsetX, float OffsetY);

    ////////////////////////////////////////////////////////////
    /// Move the object of a given offset (take a 2D vector)
    ///
    /// \param Offset : Amount of units to move the object of
    ///
    ////////////////////////////////////////////////////////////
    void Move(const Vector2f& Offset);

    ////////////////////////////////////////////////////////////
    /// Scale the object (take 2 values)
    ///
    /// \param FactorX : Scaling factor on X (must be strictly positive)
    /// \param FactorY : Scaling factor on Y (must be strictly positive)
    ///
    ////////////////////////////////////////////////////////////
    void Scale(float FactorX, float FactorY);

    ////////////////////////////////////////////////////////////
    /// Scale the object (take a 2D vector)
    ///
    /// \param Factor : Scaling factors (both values must be strictly positive)
    ///
    ////////////////////////////////////////////////////////////
    void Scale(const Vector2f& Factor);

    ////////////////////////////////////////////////////////////
    /// Rotate the object
    ///
    /// \param Angle : Angle of rotation, in degrees
    ///
    ////////////////////////////////////////////////////////////
    void Rotate(float Angle);

    ////////////////////////////////////////////////////////////
    /// Transform a point from global coordinates into local coordinates
    /// (ie it applies the inverse of object's center, translation, rotation and scale to the point)
    ///
    /// \param Point : Point to transform
    ///
    /// \return Transformed point
    ///
    ////////////////////////////////////////////////////////////
    sf::Vector2f TransformToLocal(const sf::Vector2f& Point) const;

    ////////////////////////////////////////////////////////////
    /// Transform a point from local coordinates into global coordinates
    /// (ie it applies the object's center, translation, rotation and scale to the point)
    ///
    /// \param Point : Point to transform
    ///
    /// \return Transformed point
    ///
    ////////////////////////////////////////////////////////////
    sf::Vector2f TransformToGlobal(const sf::Vector2f& Point) const;

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
    /// Draw the object into the specified window
    ///
    /// \param Target : Target into which render the object
    ///
    ////////////////////////////////////////////////////////////
    void Draw(RenderTarget& Target) const;

    ////////////////////////////////////////////////////////////
    /// Render the specific geometry of the object
    ///
    /// \param Target : Target into which render the object
    ///
    ////////////////////////////////////////////////////////////
    virtual void Render(RenderTarget& Target) const = 0;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Vector2f        myPosition;      ///< Position of the object on screen
    Vector2f        myScale;         ///< Scale of the object
    Vector2f        myCenter;        ///< Origin of translation / rotation / scaling of the object
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
