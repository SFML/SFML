////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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

#include <cassert>
#include <cstdlib>


namespace sf
{
////////////////////////////////////////////////////////////
Sprite::Sprite(const Texture& texture)
{
    setTexture(texture, true);
}


////////////////////////////////////////////////////////////
Sprite::Sprite(const Texture& texture, const IntRect& rectangle)
{
    // Compute the texture area
    setTextureRect(rectangle);
    // Assign texture
    setTexture(texture, false);
}


////////////////////////////////////////////////////////////
void Sprite::setTexture(const Texture& texture, bool resetRect)
{
    // Recompute the texture area if requested
    if (resetRect)
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
    for (auto& vertex : m_vertices)
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
    const auto width  = static_cast<float>(std::abs(m_textureRect.width));
    const auto height = static_cast<float>(std::abs(m_textureRect.height));

    return {{0.f, 0.f}, {width, height}};
}


////////////////////////////////////////////////////////////
FloatRect Sprite::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}


////////////////////////////////////////////////////////////
void Sprite::draw(RenderTarget& target, const RenderStates& states) const
{
    RenderStates statesCopy(states);

    statesCopy.transform *= getTransform();
    statesCopy.texture = m_texture;
    target.draw(m_vertices.data(), m_vertices.size(), PrimitiveType::TriangleStrip, statesCopy);
}


////////////////////////////////////////////////////////////
void Sprite::updatePositions()
{
    const FloatRect bounds = getLocalBounds();

    m_vertices[0].position = Vector2f(0, 0);
    m_vertices[1].position = Vector2f(0, bounds.height);
    m_vertices[2].position = Vector2f(bounds.width, 0);
    m_vertices[3].position = Vector2f(bounds.width, bounds.height);
}


////////////////////////////////////////////////////////////
void Sprite::updateTexCoords()
{
    const FloatRect convertedTextureRect(m_textureRect);

    const float left   = convertedTextureRect.left;
    const float right  = left + convertedTextureRect.width;
    const float top    = convertedTextureRect.top;
    const float bottom = top + convertedTextureRect.height;

    m_vertices[0].texCoords = Vector2f(left, top);
    m_vertices[1].texCoords = Vector2f(left, bottom);
    m_vertices[2].texCoords = Vector2f(right, top);
    m_vertices[3].texCoords = Vector2f(right, bottom);
}

} // namespace sf
