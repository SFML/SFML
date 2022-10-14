////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <cstdlib>


namespace sf
{
////////////////////////////////////////////////////////////
Sprite::Sprite() : m_texture(nullptr), m_textureRect()
{
}


////////////////////////////////////////////////////////////
Sprite::Sprite(const Texture& texture) : m_texture(nullptr), m_textureRect()
{
    setTexture(texture, true);
}


////////////////////////////////////////////////////////////
Sprite::Sprite(const Texture& texture, const IntRect& rectangle) : m_texture(nullptr), m_textureRect()
{
    // Compute the texture area
    setTextureRect(rectangle);
    // Assign texture
    setTexture(texture, false);
}


////////////////////////////////////////////////////////////
void Sprite::setTexture(const Texture& texture, bool resetRect)
{
    // Recompute the texture area if requested, or if there was no valid texture & rect before
    if (resetRect || (!m_texture && (m_textureRect == sf::IntRect())))
    {
        setTextureRect(IntRect({0, 0}, Vector2i(texture.getSize())));
    }

    // Assign the new texture
    m_texture = &texture;
}


////////////////////////////////////////////////////////////
void Sprite::setTextureRect(const IntRect& rectangle)
{
    if (rectangle != m_textureRect)
    {
        m_textureRect = rectangle;
        updatePositions();
        updateTexCoords();
    }
}


////////////////////////////////////////////////////////////
void Sprite::setColor(const Color& color)
{
    // Update the vertices' color
    m_vertices[0].color = color;
    m_vertices[1].color = color;
    m_vertices[2].color = color;
    m_vertices[3].color = color;
}


////////////////////////////////////////////////////////////
const Texture* Sprite::getTexture() const
{
    return m_texture;
}


////////////////////////////////////////////////////////////
const IntRect& Sprite::getTextureRect() const
{
    return m_textureRect;
}


////////////////////////////////////////////////////////////
const Color& Sprite::getColor() const
{
    return m_vertices[0].color;
}


////////////////////////////////////////////////////////////
FloatRect Sprite::getLocalBounds() const
{
    return FloatRect({0, 0}, Vector2f(m_textureRect.getAbsoluteSize()));
}


////////////////////////////////////////////////////////////
FloatRect Sprite::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}


////////////////////////////////////////////////////////////
void Sprite::draw(RenderTarget& target, const RenderStates& states) const
{
    if (m_texture)
    {
        RenderStates statesCopy(states);

        statesCopy.transform *= getTransform();
        statesCopy.texture = m_texture;
        target.draw(m_vertices, 4, TriangleStrip, statesCopy);
    }
}


////////////////////////////////////////////////////////////
void Sprite::updatePositions()
{
    Vector2f size(m_textureRect.getAbsoluteSize());

    m_vertices[0].position = Vector2f();
    m_vertices[1].position = Vector2f(0, size.y);
    m_vertices[2].position = Vector2f(size.x, 0);
    m_vertices[3].position = size;
}


////////////////////////////////////////////////////////////
void Sprite::updateTexCoords()
{
    FloatRect textureRect(m_textureRect);

    m_vertices[0].texCoords = textureRect.position;
    m_vertices[1].texCoords = Vector2f(textureRect.position.x, textureRect.position.y + textureRect.size.y);
    m_vertices[2].texCoords = Vector2f(textureRect.position.x + textureRect.size.x, textureRect.position.y);
    m_vertices[3].texCoords = textureRect.position + textureRect.size;
}

} // namespace sf
