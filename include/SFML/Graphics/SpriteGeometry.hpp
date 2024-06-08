////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <array>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Transformable geometry for a textured quad
///
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API SpriteGeometry : public Transformable
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Construct the sprite geometry from a sub-rectangle of an eventual source texture
    ///
    /// \param rectangle Sub-rectangle of the eventual texture (specified during sprite drawing)
    ///
    /// \see setTextureRect
    ///
    ////////////////////////////////////////////////////////////
    explicit SpriteGeometry(const IntRect& rectangle);

    ////////////////////////////////////////////////////////////
    /// \brief Set the sub-rectangle of the texture that the sprite geometry will display
    ///
    /// The texture rect is useful when you don't want to display
    /// the whole texture, but rather a part of it.
    ///
    /// \param rectangle Rectangle defining the region of the texture to display
    ///
    /// \see getTextureRect, setTexture
    ///
    ////////////////////////////////////////////////////////////
    void setTextureRect(const IntRect& rectangle);

    ////////////////////////////////////////////////////////////
    /// \brief Set the global color of the sprite geometry
    ///
    /// This color is modulated (multiplied) with the sprite's
    /// texture. It can be used to colorize the sprite, or change
    /// its global opacity.
    /// By default, the sprite's color is opaque white.
    ///
    /// \param color New color of the sprite geometry
    ///
    /// \see getColor
    ///
    ////////////////////////////////////////////////////////////
    void setColor(const Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Get the sub-rectangle of the texture displayed by the sprite
    ///
    /// \return Texture rectangle of the sprite geometry
    ///
    /// \see setTextureRect
    ///
    ////////////////////////////////////////////////////////////
    IntRect getTextureRect() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the global color of the sprite geometry
    ///
    /// \return Global color of the sprite geometry
    ///
    /// \see setColor
    ///
    ////////////////////////////////////////////////////////////
    const Color& getColor() const;

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
    FloatRect getLocalBounds() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the global bounding rectangle of the entity
    ///
    /// The returned rectangle is in global coordinates, which means
    /// that it takes into account the transformations (translation,
    /// rotation, scale, ...) that are applied to the entity.
    /// In other words, this function returns the bounds of the
    /// sprite in the global 2D world's coordinate system.
    ///
    /// \return Global bounding rectangle of the entity
    ///
    ////////////////////////////////////////////////////////////
    FloatRect getGlobalBounds() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the vertices of the sprite geometry
    ///
    ////////////////////////////////////////////////////////////
    const std::array<Vertex, 4>& getVertices() const;

private:
    ////////////////////////////////////////////////////////////
    /// \brief Update the vertices' positions and texture coordinates
    ///
    ////////////////////////////////////////////////////////////
    void updateVertices(const IntRect& rectangle);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::array<Vertex, 4> m_vertices; //!< Vertices defining the sprite's geometry
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::Sprite
/// \ingroup graphics
///
/// sf::Sprite is a drawable class that allows to easily display
/// a texture (or a part of it) on a render target.
///
/// It inherits all the functions from sf::Transformable:
/// position, rotation, scale, origin. It also adds sprite-specific
/// properties such as the texture to use, the part of it to display,
/// and some convenience functions to change the overall color of the
/// sprite, or to get its bounding rectangle.
///
/// sf::Sprite works in combination with the sf::Texture class, which
/// loads and provides the pixel data of a given texture.
///
/// The separation of sf::Sprite and sf::Texture allows more flexibility
/// and better performances: indeed a sf::Texture is a heavy resource,
/// and any operation on it is slow (often too slow for real-time
/// applications). On the other side, a sf::Sprite is a lightweight
/// object which can use the pixel data of a sf::Texture and draw
/// it with its own transformation/color/blending attributes.
///
/// It is important to note that the sf::Sprite instance doesn't
/// copy the texture that it uses, it only keeps a reference to it.
/// Thus, a sf::Texture must not be destroyed while it is
/// used by a sf::Sprite (i.e. never write a function that
/// uses a local sf::Texture instance for creating a sprite).
///
/// See also the note on coordinates and undistorted rendering in sf::Transformable.
///
/// Usage example:
/// \code
/// // Load a texture
/// const auto texture = sf::Texture::loadFromFile("texture.png").value();
///
/// // Create the sprite geometry
/// sf::SpriteGeometry geometry(texture.getRect);
/// geometry.setTextureRect({{10, 10}, {50, 30}});
/// geometry.setColor({255, 255, 255, 200});
/// geometry.setPosition(100, 25);
///
/// // Create and draw the sprite
/// window.draw(sf::Sprite(geometry, texture));
/// \endcode
///
/// \see sf::Sprite, sf::Texture, sf::Transformable
///
////////////////////////////////////////////////////////////
