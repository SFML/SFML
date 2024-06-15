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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <cmath>


namespace sf
{
////////////////////////////////////////////////////////////
Sprite::Sprite(const Texture& texture) : Sprite(texture, IntRect({0, 0}, Vector2i(texture.getSize())))
{
}


////////////////////////////////////////////////////////////
Sprite::Sprite(const Texture& texture, const IntRect& rectangle) : m_texture(&texture), m_textureRect(rectangle)
{
    updateVertices();

    SFML_UPDATE_LIFETIME_DEPENDANT(Texture, Sprite, m_texture);
}


////////////////////////////////////////////////////////////
void Sprite::setTexture(const Texture& texture, bool resetRect)
{
    // Recompute the texture area if requested
    if (resetRect)
        setTextureRect(IntRect({0, 0}, Vector2i(texture.getSize())));

    // Assign the new texture
    m_texture = &texture;

    SFML_UPDATE_LIFETIME_DEPENDANT(Texture, Sprite, m_texture);
}


////////////////////////////////////////////////////////////
void Sprite::setTextureRect(const IntRect& rectangle)
{
    if (rectangle != m_textureRect)
    {
        m_textureRect = rectangle;
        updateVertices();
    }
}


////////////////////////////////////////////////////////////
void Sprite::setColor(const Color& color)
{
    for (Vertex& vertex : m_vertices)
        vertex.color = color;
}


////////////////////////////////////////////////////////////
const Texture& Sprite::getTexture() const
{
    return *m_texture;
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
    // Last vertex posiion is equal to texture rect size absolute value
    return {{0.f, 0.f}, m_vertices[3].position};
}


////////////////////////////////////////////////////////////
FloatRect Sprite::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}


////////////////////////////////////////////////////////////
void Sprite::draw(RenderTarget& target, RenderStates states) const
{
    states.transform *= getTransform();
    states.texture        = m_texture;
    states.coordinateType = CoordinateType::Pixels;

    target.draw(m_vertices.data(), m_vertices.size(), PrimitiveType::TriangleStrip, states);
}


////////////////////////////////////////////////////////////
void Sprite::updateVertices()
{
    const auto left   = static_cast<float>(m_textureRect.left);
    const auto top    = static_cast<float>(m_textureRect.top);
    const auto width  = static_cast<float>(m_textureRect.width);
    const auto height = static_cast<float>(m_textureRect.height);
    const auto right  = float{left + width};
    const auto bottom = float{top + height};

    // Absolute value is used to support negative texture rect sizes
    const auto absWidth  = float{std::abs(width)};
    const auto absHeight = float{std::abs(height)};

    // Update positions
    m_vertices[0].position = {0.f, 0.f};
    m_vertices[1].position = {0.f, absHeight};
    m_vertices[2].position = {absWidth, 0.f};
    m_vertices[3].position = {absWidth, absHeight};

    // Update texture coordinates
    m_vertices[0].texCoords = {left, top};
    m_vertices[1].texCoords = {left, bottom};
    m_vertices[2].texCoords = {right, top};
    m_vertices[3].texCoords = {right, bottom};
}

} // namespace sf
