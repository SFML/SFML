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

namespace Blend
{
    ////////////////////////////////////////////////////////////
    /// \ingroup graphics
    /// \brief Available blending modes for drawable objects
    ///
    ////////////////////////////////////////////////////////////
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
    /// \param factors New scale factors
    ///
    /// \see Scale, SetScaleX, SetScaleY, GetScale
    ///
    ////////////////////////////////////////////////////////////
    void SetScale(const Vector2f& factors);

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
    /// The default color is white.
    ///
    /// \param color New color
    ///
    /// \see GetColor
    ///
    ////////////////////////////////////////////////////////////
    void SetColor(const Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Set the blending mode of the object
    ///
    /// This property defines how the pixels of an object are
    /// blended with the pixels of the render target to which
    /// it is drawn. To know more about the blending modes
    /// available, see the sf::Blend::Mode enum.
    /// The default blend mode is Blend::Alpha.
    ///
    /// \param mode New blending mode
    ///
    /// \see GetBlendMode
    ///
    ////////////////////////////////////////////////////////////
    void SetBlendMode(Blend::Mode mode);

    ////////////////////////////////////////////////////////////
    /// \brief Get the position of the object
    ///
    /// \return Current position
    ///
    /// \see SetPosition
    ///
    ////////////////////////////////////////////////////////////
    const Vector2f& GetPosition() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the current scale of the object
    ///
    /// \return Current scale factors
    ///
    /// \see SetScale
    ///
    ////////////////////////////////////////////////////////////
    const Vector2f& GetScale() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the local origin of the object
    ///
    /// \return Current origin
    ///
    /// \see SetOrigin
    ///
    ////////////////////////////////////////////////////////////
    const Vector2f& GetOrigin() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the orientation of the object
    ///
    /// The rotation is always in the range [0, 360].
    ///
    /// \return Current rotation, in degrees
    ///
    /// \see SetRotation
    ///
    ////////////////////////////////////////////////////////////
    float GetRotation() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the color of the object
    ///
    /// \return Current color
    ///
    /// \see SetColor
    ///
    ////////////////////////////////////////////////////////////
    const Color& GetColor() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the blend mode of the object
    ///
    /// \return Current blend mode
    ///
    /// \see SetBlendMode
    ///
    ////////////////////////////////////////////////////////////
    Blend::Mode GetBlendMode() const;

    ////////////////////////////////////////////////////////////
    /// \brief Move the object by a given offset
    ///
    /// This function adds to the current position of the object,
    /// unlike SetPosition which overwrites it.
    /// Thus, it is equivalent to the following code:
    /// \code
    /// sf::Vector2f pos = object.GetPosition();
    /// object.SetPosition(pos.x + offsetX, pos.y + offsetY);
    /// \endcode
    ///
    /// \param offsetX X offset
    /// \param offsetY Y offset
    ///
    /// \see SetPosition
    ///
    ////////////////////////////////////////////////////////////
    void Move(float offsetX, float offsetY);

    ////////////////////////////////////////////////////////////
    /// \brief Move the object by a given offset
    ///
    /// This function adds to the current position of the object,
    /// unlike SetPosition which overwrites it.
    /// Thus, it is equivalent to the following code:
    /// \code
    /// object.SetPosition(object.GetPosition() + offset);
    /// \endcode
    ///
    /// \param offset Offset
    ///
    /// \see SetPosition
    ///
    ////////////////////////////////////////////////////////////
    void Move(const Vector2f& offset);

    ////////////////////////////////////////////////////////////
    /// \brief Scale the object
    ///
    /// This function multiplies the current scale of the object,
    /// unlike SetScale which overwrites it.
    /// Thus, it is equivalent to the following code:
    /// \code
    /// sf::Vector2f scale = object.GetScale();
    /// object.SetScale(scale.x * factorX, scale.y * factorY);
    /// \endcode
    ///
    /// \param factorX Horizontal scale factor
    /// \param factorY Vertical scale factor
    ///
    /// \see SetScale
    ///
    ////////////////////////////////////////////////////////////
    void Scale(float factorX, float factorY);

    ////////////////////////////////////////////////////////////
    /// \brief Scale the object
    ///
    /// This function multiplies the current scale of the object,
    /// unlike SetScale which overwrites it.
    /// Thus, it is equivalent to the following code:
    /// \code
    /// sf::Vector2f scale = object.GetScale();
    /// object.SetScale(scale.x * factor.x, scale.y * factor.y);
    /// \endcode
    ///
    /// \param factor Scale factors
    ///
    /// \see SetScale
    ///
    ////////////////////////////////////////////////////////////
    void Scale(const Vector2f& factor);

    ////////////////////////////////////////////////////////////
    /// \brief Rotate the object
    ///
    /// This function ads to the current rotation of the object,
    /// unlike SetRotation which overwrites it.
    /// Thus, it is equivalent to the following code:
    /// \code
    /// object.SetRotation(object.GetRotation() + angle);
    /// \endcode
    ///
    /// \param angle Angle of rotation, in degrees
    ///
    ////////////////////////////////////////////////////////////
    void Rotate(float angle);

    ////////////////////////////////////////////////////////////
    /// \brief Transform a point in object local coordinates
    ///
    /// This function takes a point in global coordinates, and
    /// transforms it in coordinates local to the object.
    /// In other words, it applies the inverse of all the
    /// transformations applied to the object (origin,
    /// translation, rotation and scale).
    ///
    /// \param point Point to transform
    ///
    /// \return The transformed point
    ///
    /// \see TransformToGlobal
    ///
    ////////////////////////////////////////////////////////////
    Vector2f TransformToLocal(const Vector2f& point) const;

    ////////////////////////////////////////////////////////////
    /// \brief Transform a local point in global coordinates
    ///
    /// This function takes a point in local coordinates, and
    /// transforms it in global coordinates. In other words,
    /// it applies the same transformations that are applied
    /// to the object (origin, translation, rotation and scale).
    ///
    /// \param point Point to transform
    ///
    /// \return The transformed point
    ///
    /// \see TransformToLocal
    ///
    ////////////////////////////////////////////////////////////
    Vector2f TransformToGlobal(const Vector2f& point) const;

protected :

    ////////////////////////////////////////////////////////////
    /// \brief Get the transform matrix of the object
    ///
    /// \return Transform matrix
    ///
    /// \see GetInverseMatrix
    ///
    ////////////////////////////////////////////////////////////
    const Matrix3& GetMatrix() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the inverse transform matrix of the object
    ///
    /// \return Inverse transform matrix
    ///
    /// \see GetMatrix
    ///
    ////////////////////////////////////////////////////////////
    const Matrix3& GetInverseMatrix() const;

private :

    friend class RenderTarget;

    ////////////////////////////////////////////////////////////
    /// \brief Draw the object to a render target
    ///
    /// This function applies the common states of the object,
    /// then calls the virtual Render functions to let the derived
    /// class draw the geometry of the object.
    ///
    /// \param target   Render target
    /// \param renderer Renderer providing low-level rendering commands
    ///
    ////////////////////////////////////////////////////////////
    void Draw(RenderTarget& target, Renderer& renderer) const;

    ////////////////////////////////////////////////////////////
    /// \brief Draw the object to a render target
    ///
    /// This is a pure virtual function that has to be implemented
    /// by the derived class to define how the drawable should be
    /// rendered.
    ///
    /// \param target   Render target
    /// \param renderer Renderer providing low-level rendering commands
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


////////////////////////////////////////////////////////////
/// \class sf::Drawable
/// \ingroup graphics
///
/// sf::Drawable defines the attributes and operations that
/// are common to all the drawable classes:
/// \li transformations (position, rotation, scale, local origin)
/// \li global overlay color
/// \li blending mode with background pixels
/// \li the ability to be drawn on a sf::RenderTarget (either RenderWindow or RenderImage)
///
/// Please note that all these attributes are hardware accelerated,
/// therefore they are extremely cheap to use (unlike older
/// libraries that perform slow transformations on the CPU, such as
/// rotation or scale).
///
/// Usage example:
/// \code
/// // Here we'll use a sf::Sprite to demonstrate the features of sf::Drawable
/// sf::Sprite drawable = /* ...whatever... */;
///
/// drawable.SetOrigin(10, 20);               // set its origin to the local point (10, 20)
/// drawable.SetPosition(100, 100);           // set its position to (100, 100)
/// drawable.SetRotation(45);                 // set its orientation to 45 degrees
/// drawable.SetColor(sf::Color::Red);        // set its global color to red
/// drawable.SetBlendingMode(sf::Blend::Add); // set an additive blend mode
///
/// window.Draw(drawable); // finally draw it (window is a sf::RenderWindow)
/// \endcode
///
/// Deriving your own class from sf::Drawable is possible, however
/// you have to use the sf::Renderer class instead of direct OpenGL
/// calls, which is more limited. To create a derived drawable class,
/// all you have to do is to override the virtual Render function.
///
/// One of the main benefits of creating your own drawable class is
/// that you can build hierarchies of drawable objects. Indeed,
/// when you draw a drawable inside the Render function of another
/// drawable, the former inherits the transformations and color of
/// the latter and combines them with its own attributes.
/// This way, you can apply global transformations/color to a set
/// of drawables as if it was a single entity.
///
/// Example:
/// \code
/// class MyDrawable : public sf::Drawable
/// {
/// public :
///
///    ...
///
/// private :
///
///     virtual void Render(sf::RenderTarget& target, sf::Renderer& renderer) const
///     {
///         // Low-level geometry rendering
///         renderer.SetTexture(&myTexture);
///         renderer.Begin(sf::Renderer::QuadList);
///             renderer.AddVertex(...);
///             renderer.AddVertex(...);
///             renderer.AddVertex(...);
///             renderer.AddVertex(...);
///         renderer.End();
///
///         // High-level drawable rendering
///         target.Draw(mySubSprite);
///     }
///
///     sf::Image myTexture;
///     sf::Sprite mySubSprite;
/// };
/// \endcode
///
/// \see sf::Shape, sf::Sprite, sf::Text
///
////////////////////////////////////////////////////////////
