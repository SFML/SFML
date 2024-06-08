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

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>


namespace sf
{
class SpriteGeometry;
class Texture;

////////////////////////////////////////////////////////////
/// \brief Drawable representation of a texture, with its
///        own transformations, color, etc.
///
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API Sprite : public Drawable
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Construct the sprite from a source texture
    ///
    /// \param texture Source texture
    ///
    /// \see setTexture
    ///
    ////////////////////////////////////////////////////////////
    explicit Sprite(const SpriteGeometry& geometry, const Texture& texture);

    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy constructor
    ///
    ////////////////////////////////////////////////////////////
    Sprite(const Sprite&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy assignment
    ///
    ////////////////////////////////////////////////////////////
    Sprite& operator=(const Sprite&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Deleted move constructor
    ///
    ////////////////////////////////////////////////////////////
    Sprite(Sprite&&) noexcept = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Deleted move assignment
    ///
    ////////////////////////////////////////////////////////////
    Sprite& operator=(Sprite&&) noexcept = delete;

private:
    ////////////////////////////////////////////////////////////
    /// \brief Draw the sprite to a render target
    ///
    /// \param target Render target to draw to
    /// \param states Current render states
    ///
    ////////////////////////////////////////////////////////////
    void draw(RenderTarget& target, RenderStates states) const override;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    const SpriteGeometry& m_geometry; //!< Geometry of the sprite
    const Texture&        m_texture;  //!< Texture of the sprite
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
/// sf::SpriteGeometry geometry(texture.getRect());
/// geometry.setTextureRect(sf::IntRect({10, 10}, {50, 30}));
/// geometry.setColor(sf::Color(255, 255, 255, 200));
/// geometry.setPosition(100, 25);
///
/// // Create and draw the sprite
/// window.draw(sf::Sprite(geometry, texture));
/// \endcode
///
/// \see sf::SpriteGeometry, sf::Texture, sf::Transformable, sf::Drawable
///
////////////////////////////////////////////////////////////
