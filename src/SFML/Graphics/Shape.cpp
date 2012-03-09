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
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Err.hpp>
#include <cmath>


namespace
{
    // Compute the normal of a segment
    sf::Vector2f ComputeNormal(const sf::Vector2f& p1, const sf::Vector2f& p2)
    {
        sf::Vector2f normal(p1.y - p2.y, p2.x - p1.x);
        float length = std::sqrt(normal.x * normal.x + normal.y * normal.y);
        if (length != 0.f)
            normal /= length;
        return normal;
    }
}


namespace sf
{
////////////////////////////////////////////////////////////
Shape::~Shape()
{
}


////////////////////////////////////////////////////////////
void Shape::SetTexture(const Texture* texture, bool resetRect)
{
    // Recompute the texture area if requested, or if there was no texture before
    if (texture && (resetRect || !m_texture))
        SetTextureRect(IntRect(0, 0, texture->GetWidth(), texture->GetHeight()));

    // Assign the new texture
    m_texture = texture;
}


////////////////////////////////////////////////////////////
const Texture* Shape::GetTexture() const
{
    return m_texture;
}


////////////////////////////////////////////////////////////
void Shape::SetTextureRect(const IntRect& rect)
{
    m_textureRect = rect;
    UpdateTexCoords();
}


////////////////////////////////////////////////////////////
const IntRect& Shape::GetTextureRect() const
{
    return m_textureRect;
}


////////////////////////////////////////////////////////////
void Shape::SetFillColor(const Color& color)
{
    m_fillColor = color;
    UpdateFillColors();
}


////////////////////////////////////////////////////////////
const Color& Shape::GetFillColor() const
{
    return m_fillColor;
}


////////////////////////////////////////////////////////////
void Shape::SetOutlineColor(const Color& color)
{
    m_outlineColor = color;
    UpdateOutlineColors();
}


////////////////////////////////////////////////////////////
const Color& Shape::GetOutlineColor() const
{
    return m_outlineColor;
}


////////////////////////////////////////////////////////////
void Shape::SetOutlineThickness(float thickness)
{
    m_outlineThickness = thickness;
    Update(); // recompute everything because the whole shape must be offset
}


////////////////////////////////////////////////////////////
float Shape::GetOutlineThickness() const
{
    return m_outlineThickness;
}


////////////////////////////////////////////////////////////
FloatRect Shape::GetLocalBounds() const
{
    return m_bounds;
}


////////////////////////////////////////////////////////////
FloatRect Shape::GetGlobalBounds() const
{
    return GetTransform().TransformRect(GetLocalBounds());
}


////////////////////////////////////////////////////////////
Shape::Shape() :
m_texture         (NULL),
m_textureRect     (),
m_fillColor       (255, 255, 255),
m_outlineColor    (255, 255, 255),
m_outlineThickness(0),
m_vertices        (TrianglesFan),
m_outlineVertices (TrianglesStrip),
m_insideBounds    (),
m_bounds          ()
{
}


////////////////////////////////////////////////////////////
void Shape::Update()
{
    // Get the total number of points of the shape
    unsigned int count = GetPointCount();
    if (count < 3)
    {
        m_vertices.Resize(0);
        m_outlineVertices.Resize(0);
        return;
    }

    m_vertices.Resize(count + 2); // + 2 for center and repeated first point

    // Position
    for (unsigned int i = 0; i < count; ++i)
        m_vertices[i + 1].Position = GetPoint(i);
    m_vertices[count + 1].Position = m_vertices[1].Position;

    // Update the bounding rectangle
    m_vertices[0] = m_vertices[1]; // so that the result of GetBounds() is correct
    m_insideBounds = m_vertices.GetBounds();

    // Compute the center and make it the first vertex
    m_vertices[0].Position.x = m_insideBounds.Left + m_insideBounds.Width / 2;
    m_vertices[0].Position.y = m_insideBounds.Top + m_insideBounds.Height / 2;

    // Color
    UpdateFillColors();

    // Texture coordinates
    UpdateTexCoords();

    // Outline
    UpdateOutline();
}


////////////////////////////////////////////////////////////
void Shape::Draw(RenderTarget& target, RenderStates states) const
{
    states.Transform *= GetTransform();

    // Render the inside
    if (m_fillColor.a > 0)
    {
        states.Texture = m_texture;
        target.Draw(m_vertices, states);
    }

    // Render the outline
    if ((m_outlineColor.a > 0) && (m_outlineThickness > 0))
    {
        states.Texture = NULL;
        target.Draw(m_outlineVertices, states);
    }
}


////////////////////////////////////////////////////////////
void Shape::UpdateFillColors()
{
    for (unsigned int i = 0; i < m_vertices.GetVertexCount(); ++i)
        m_vertices[i].Color = m_fillColor;
}


////////////////////////////////////////////////////////////
void Shape::UpdateTexCoords()
{
    for (unsigned int i = 0; i < m_vertices.GetVertexCount(); ++i)
    {
        float xratio = (m_vertices[i].Position.x - m_insideBounds.Left) / m_insideBounds.Width;
        float yratio = (m_vertices[i].Position.y - m_insideBounds.Top) / m_insideBounds.Height;
        m_vertices[i].TexCoords.x = m_textureRect.Left + m_textureRect.Width * xratio;
        m_vertices[i].TexCoords.y = m_textureRect.Top + m_textureRect.Height * yratio;
    }
}


////////////////////////////////////////////////////////////
void Shape::UpdateOutline()
{
    unsigned int count = m_vertices.GetVertexCount() - 2;
    m_outlineVertices.Resize((count + 1) * 2);

    for (unsigned int i = 0; i < count; ++i)
    {
        unsigned int index = i + 1;

        // Get the two segments shared by the current point
        Vector2f p0 = (i == 0) ? m_vertices[count].Position : m_vertices[index - 1].Position;
        Vector2f p1 = m_vertices[index].Position;
        Vector2f p2 = m_vertices[index + 1].Position;

        // Compute their normal
        Vector2f n1 = ComputeNormal(p0, p1);
        Vector2f n2 = ComputeNormal(p1, p2);

        // Combine them to get the extrusion direction
        float factor = 1.f + (n1.x * n2.x + n1.y * n2.y);
        Vector2f normal = -(n1 + n2) / factor;

        // Update the outline points
        m_outlineVertices[i * 2 + 0].Position = p1;
        m_outlineVertices[i * 2 + 1].Position = p1 + normal * m_outlineThickness;
    }

    // Duplicate the first point at the end, to close the outline
    m_outlineVertices[count * 2 + 0].Position = m_outlineVertices[0].Position;
    m_outlineVertices[count * 2 + 1].Position = m_outlineVertices[1].Position;

    // Update outline colors
    UpdateOutlineColors();

    // Update the shape's bounds
    m_bounds = m_outlineVertices.GetBounds();
}


////////////////////////////////////////////////////////////
void Shape::UpdateOutlineColors()
{
    for (unsigned int i = 0; i < m_outlineVertices.GetVertexCount(); ++i)
        m_outlineVertices[i].Color = m_outlineColor;
}

} // namespace sf
