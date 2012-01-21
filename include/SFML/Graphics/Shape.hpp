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

#ifndef SFML_SHAPE_HPP
#define SFML_SHAPE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Base class for textured shapes with outline
///
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API Shape : public Drawable, public Transformable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Virtual destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~Shape();

    ////////////////////////////////////////////////////////////
    /// \brief Change the source texture of the shape
    ///
    /// The \a texture argument refers to a texture that must
    /// exist as long as the shape uses it. Indeed, the shape
    /// doesn't store its own copy of the texture, but rather keeps
    /// a pointer to the one that you passed to this function.
    /// If the source texture is destroyed and the shape tries to
    /// use it, the behaviour is undefined.
    /// \a texture can be NULL to disable texturing.
    /// If \a resetRect is true, the TextureRect property of
    /// the shape is automatically adjusted to the size of the new
    /// texture. If it is false, the texture rect is left unchanged.
    ///
    /// \param texture   New texture
    /// \param resetRect Should the texture rect be reset to the size of the new texture?
    ///
    /// \see GetTexture, SetTextureRect
    ///
    ////////////////////////////////////////////////////////////
    void SetTexture(const Texture* texture, bool resetRect = false);

    ////////////////////////////////////////////////////////////
    /// \brief Set the sub-rectangle of the texture that the shape will display
    ///
    /// The texture rect is useful when you don't want to display
    /// the whole texture, but rather a part of it.
    /// By default, the texture rect covers the entire texture.
    ///
    /// \param rect Rectangle defining the region of the texture to display
    ///
    /// \see GetTextureRect, SetTexture
    ///
    ////////////////////////////////////////////////////////////
    void SetTextureRect(const IntRect& rect);

    ////////////////////////////////////////////////////////////
    /// \brief Set the fill color of the shape
    ///
    /// This color is modulated (multiplied) with the shape's
    /// texture if any. It can be used to colorize the shape,
    /// or change its global opacity.
    /// You can use sf::Color::Transparent to make the inside of
    /// the shape transparent, and have the outline alone.
    /// By default, the shape's fill color is opaque white.
    ///
    /// \param color New color of the shape
    ///
    /// \see GetFillColor, SetOutlineColor
    ///
    ////////////////////////////////////////////////////////////
    void SetFillColor(const Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Set the outline color of the shape
    ///
    /// You can use sf::Color::Transparent to disable the outline.
    /// By default, the shape's outline color is opaque white.
    ///
    /// \param color New outline color of the shape
    ///
    /// \see GetOutlineColor, SetFillColor
    ///
    ////////////////////////////////////////////////////////////
    void SetOutlineColor(const Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Set the thickness of the shape's outline
    ///
    /// This number cannot be negative. Using zero disables
    /// the outline.
    /// By default, the outline thickness is 0.
    ///
    /// \param thickness New outline thickness
    ///
    /// \see GetOutlineThickness
    ///
    ////////////////////////////////////////////////////////////
    void SetOutlineThickness(float thickness);

    ////////////////////////////////////////////////////////////
    /// \brief Get the source texture of the shape
    ///
    /// If the shape has no source texture, a NULL pointer is returned.
    /// The returned pointer is const, which means that you can't
    /// modify the texture when you retrieve it with this function.
    ///
    /// \return Pointer to the shape's texture
    ///
    /// \see SetTexture
    ///
    ////////////////////////////////////////////////////////////
    const Texture* GetTexture() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the sub-rectangle of the texture displayed by the shape
    ///
    /// \return Texture rectangle of the shape
    ///
    /// \see SetTextureRect
    ///
    ////////////////////////////////////////////////////////////
    const IntRect& GetTextureRect() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the fill color of the shape
    ///
    /// \return Fill color of the shape
    ///
    /// \see SetFillColor
    ///
    ////////////////////////////////////////////////////////////
    const Color& GetFillColor() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the outline color of the shape
    ///
    /// \return Outline color of the shape
    ///
    /// \see SetOutlineColor
    ///
    ////////////////////////////////////////////////////////////
    const Color& GetOutlineColor() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the outline thickness of the shape
    ///
    /// \return Outline thickness of the shape
    ///
    /// \see SetOutlineThickness
    ///
    ////////////////////////////////////////////////////////////
    float GetOutlineThickness() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the total number of points of the shape
    ///
    /// \return Number of points of the shape
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int GetPointCount() const = 0;

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
    virtual Vector2f GetPoint(unsigned int index) const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Get the local bounding rectangle of the entity
    ///
    /// The returned rectangle is in local coordinates, which means
    /// that it ignores the transformations (translation, rotation,
    /// scale, ...) that are applied to the entity.
    /// In other words, this function returns the bounds of the
    /// entity in the entity's coordinate system.
    ///
    /// \return Local bounding rectangle of the entity
    ///
    ////////////////////////////////////////////////////////////
    FloatRect GetLocalBounds() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the global bounding rectangle of the entity
    ///
    /// The returned rectangle is in global coordinates, which means
    /// that it takes in account the transformations (translation,
    /// rotation, scale, ...) that are applied to the entity.
    /// In other words, this function returns the bounds of the
    /// sprite in the global 2D world's coordinate system.
    ///
    /// \return Global bounding rectangle of the entity
    ///
    ////////////////////////////////////////////////////////////
    FloatRect GetGlobalBounds() const;

protected :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Shape();

    ////////////////////////////////////////////////////////////
    /// \brief Recompute the internal geometry of the shape
    ///
    /// This function must be called by the derived class everytime
    /// the shape's points change (ie. the result of either
    /// GetPointCount or GetPoint is different).
    ///
    ////////////////////////////////////////////////////////////
    void Update();

private :

    ////////////////////////////////////////////////////////////
    /// \brief Draw the shape to a render target
    ///
    /// \param target Render target to draw to
    /// \param states Current render states
    ///
    ////////////////////////////////////////////////////////////
    virtual void Draw(RenderTarget& target, RenderStates states) const;

    ////////////////////////////////////////////////////////////
    /// \brief Update the fill vertices' color
    ///
    ////////////////////////////////////////////////////////////
    void UpdateFillColors();

    ////////////////////////////////////////////////////////////
    /// \brief Update the fill vertices' texture coordinates
    ///
    ////////////////////////////////////////////////////////////
    void UpdateTexCoords();

    ////////////////////////////////////////////////////////////
    /// \brief Update the outline vertices' position
    ///
    ////////////////////////////////////////////////////////////
    void UpdateOutline();

    ////////////////////////////////////////////////////////////
    /// \brief Update the outline vertices' color
    ///
    ////////////////////////////////////////////////////////////
    void UpdateOutlineColors();

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    const Texture* myTexture;          ///< Texture of the shape
    IntRect        myTextureRect;      ///< Rectangle defining the area of the source texture to display
    Color          myFillColor;        ///< Fill color
    Color          myOutlineColor;     ///< Outline color
    float          myOutlineThickness; ///< Thickness of the shape's outline
    VertexArray    myVertices;         ///< Vertex array containing the fill geometry
    VertexArray    myOutlineVertices;  ///< Vertex array containing the outline geometry
    FloatRect      myInsideBounds;     ///< Bounding rectangle of the inside (fill)
    FloatRect      myBounds;           ///< Bounding rectangle of the whole shape (outline + fill)
};

} // namespace sf


#endif // SFML_SHAPE_HPP


////////////////////////////////////////////////////////////
/// \class sf::Shape
/// \ingroup graphics
///
/// sf::Shape is a drawable class that allows to define and
/// display a custom convex shape on a render target.
/// It's only an abstract base, it needs to be specialized for
/// concrete types of shapes (circle, rectangle, convex polygon,
/// star, ...).
///
/// In addition to the attributes provided by the specialized
/// shape classes, a shape always has the following attributes:
/// \li a texture
/// \li a texture rectangle
/// \li a fill color
/// \li an outline color
/// \li an outline thickness
///
/// Each feature is optional, and can be disabled easily:
/// \li the texture can be null
/// \li the fill/outline colors can be sf::Color::Transparent
/// \li the outline thickness can be zero
///
/// You can write your own derived shape class, there are only
/// two virtual functions to override:
/// \li GetOutlinePointCount must return the number of points of the shape
/// \li GetOutlinePoint must return the points of the shape
///
/// \see sf::RectangleShape, sf::CircleShape, sf::ConvexShape, sf::Transformable
///
////////////////////////////////////////////////////////////
