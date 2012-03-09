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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
Sprite::Sprite() :
m_texture    (NULL),
m_textureRect(0, 0, 0, 0)
{
}


////////////////////////////////////////////////////////////
Sprite::Sprite(const Texture& texture) :
m_texture    (NULL),
m_textureRect(0, 0, 0, 0)
{
    SetTexture(texture);
}


////////////////////////////////////////////////////////////
Sprite::Sprite(const Texture& texture, const IntRect& rectangle) :
m_texture    (NULL),
m_textureRect(0, 0, 0, 0)
{
    SetTexture(texture);
    SetTextureRect(rectangle);
}


////////////////////////////////////////////////////////////
void Sprite::SetTexture(const Texture& texture, bool resetRect)
{
    // Recompute the texture area if requested, or if there was no valid texture before
    if (resetRect || !m_texture)
        SetTextureRect(IntRect(0, 0, texture.GetWidth(), texture.GetHeight()));

    // Assign the new texture
    m_texture = &texture;
}


////////////////////////////////////////////////////////////
void Sprite::SetTextureRect(const IntRect& rectangle)
{
    if (rectangle != m_textureRect)
    {
        m_textureRect = rectangle;
        UpdatePositions();
        UpdateTexCoords();
    }
}


////////////////////////////////////////////////////////////
void Sprite::SetColor(const Color& color)
{
    // Update the vertices' color
    m_vertices[0].Color = color;
    m_vertices[1].Color = color;
    m_vertices[2].Color = color;
    m_vertices[3].Color = color;
}


////////////////////////////////////////////////////////////
const Texture* Sprite::GetTexture() const
{
    return m_texture;
}


////////////////////////////////////////////////////////////
const IntRect& Sprite::GetTextureRect() const
{
    return m_textureRect;
}


////////////////////////////////////////////////////////////
const Color& Sprite::GetColor() const
{
    return m_vertices[0].Color;
}


////////////////////////////////////////////////////////////
FloatRect Sprite::GetLocalBounds() const
{
    float width = static_cast<float>(m_textureRect.Width);
    float height = static_cast<float>(m_textureRect.Height);

    return FloatRect(0.f, 0.f, width, height);
}


////////////////////////////////////////////////////////////
FloatRect Sprite::GetGlobalBounds() const
{
    return GetTransform().TransformRect(GetLocalBounds());
}


////////////////////////////////////////////////////////////
void Sprite::Draw(RenderTarget& target, RenderStates states) const
{
    if (m_texture)
    {
        states.Transform *= GetTransform();
        states.Texture = m_texture;
        target.Draw(m_vertices, 4, Quads, states);
    }
}


////////////////////////////////////////////////////////////
void Sprite::UpdatePositions()
{
    float width  = static_cast<float>(m_textureRect.Width);
    float height = static_cast<float>(m_textureRect.Height);

    m_vertices[0].Position = Vector2f(0, 0);
    m_vertices[1].Position = Vector2f(0, height);
    m_vertices[2].Position = Vector2f(width, height);
    m_vertices[3].Position = Vector2f(width, 0);
}


////////////////////////////////////////////////////////////
void Sprite::UpdateTexCoords()
{
    float left   = static_cast<float>(m_textureRect.Left);
    float right  = left + m_textureRect.Width;
    float top    = static_cast<float>(m_textureRect.Top);
    float bottom = top + m_textureRect.Height;

    m_vertices[0].TexCoords = Vector2f(left, top);
    m_vertices[1].TexCoords = Vector2f(left, bottom);
    m_vertices[2].TexCoords = Vector2f(right, bottom);
    m_vertices[3].TexCoords = Vector2f(right, top);
}

} // namespace sf
