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
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <cassert>


namespace sf
{
////////////////////////////////////////////////////////////
Text::Text() :
m_string       (),
m_font         (&Font::GetDefaultFont()),
m_characterSize(30),
m_style        (Regular),
m_color        (255, 255, 255),
m_vertices     (Quads),
m_bounds       ()
{

}


////////////////////////////////////////////////////////////
Text::Text(const String& string, const Font& font, unsigned int characterSize) :
m_string       (string),
m_font         (&font),
m_characterSize(characterSize),
m_style        (Regular),
m_color        (255, 255, 255),
m_vertices     (Quads),
m_bounds       ()
{
    UpdateGeometry();
}


////////////////////////////////////////////////////////////
void Text::SetString(const String& string)
{
    m_string = string;
    UpdateGeometry();
}


////////////////////////////////////////////////////////////
void Text::SetFont(const Font& font)
{
    if (m_font != &font)
    {
        m_font = &font;
        UpdateGeometry();
    }
}


////////////////////////////////////////////////////////////
void Text::SetCharacterSize(unsigned int size)
{
    if (m_characterSize != size)
    {
        m_characterSize = size;
        UpdateGeometry();
    }
}


////////////////////////////////////////////////////////////
void Text::SetStyle(Uint32 style)
{
    if (m_style != style)
    {
        m_style = style;
        UpdateGeometry();
    }
}


////////////////////////////////////////////////////////////
void Text::SetColor(const Color& color)
{
    if (color != m_color)
    {
        m_color = color;
        for (unsigned int i = 0; i < m_vertices.GetVertexCount(); ++i)
            m_vertices[i].Color = m_color;
    }
}


////////////////////////////////////////////////////////////
const String& Text::GetString() const
{
    return m_string;
}


////////////////////////////////////////////////////////////
const Font& Text::GetFont() const
{
    assert(m_font != NULL); // can never be NULL, always &Font::GetDefaultFont() by default
    return *m_font;
}


////////////////////////////////////////////////////////////
unsigned int Text::GetCharacterSize() const
{
    return m_characterSize;
}


////////////////////////////////////////////////////////////
Uint32 Text::GetStyle() const
{
    return m_style;
}


////////////////////////////////////////////////////////////
const Color& Text::GetColor() const
{
    return m_color;
}


////////////////////////////////////////////////////////////
Vector2f Text::FindCharacterPos(std::size_t index) const
{
    assert(m_font != NULL);

    // Adjust the index if it's out of range
    if (index > m_string.GetSize())
        index = m_string.GetSize();

    // Precompute the variables needed by the algorithm
    bool  bold   = (m_style & Bold) != 0;
    float hspace = static_cast<float>(m_font->GetGlyph(L' ', m_characterSize, bold).Advance);
    float vspace = static_cast<float>(m_font->GetLineSpacing(m_characterSize));

    // Compute the position
    Vector2f position;
    Uint32 prevChar = 0;
    for (std::size_t i = 0; i < index; ++i)
    {
        Uint32 curChar = m_string[i];

        // Apply the kerning offset
        position.x += static_cast<float>(m_font->GetKerning(prevChar, curChar, m_characterSize));
        prevChar = curChar;

        // Handle special characters
        switch (curChar)
        {
            case L' ' :  position.x += hspace;                 continue;
            case L'\t' : position.x += hspace * 4;             continue;
            case L'\n' : position.y += vspace; position.x = 0; continue;
            case L'\v' : position.y += vspace * 4;             continue;
        }

        // For regular characters, add the advance offset of the glyph
        position.x += static_cast<float>(m_font->GetGlyph(curChar, m_characterSize, bold).Advance);
    }

    // Transform the position to global coordinates
    position = GetTransform().TransformPoint(position);

    return position;
}


////////////////////////////////////////////////////////////
FloatRect Text::GetLocalBounds() const
{
    return m_bounds;
}


////////////////////////////////////////////////////////////
FloatRect Text::GetGlobalBounds() const
{
    return GetTransform().TransformRect(GetLocalBounds());
}


////////////////////////////////////////////////////////////
void Text::Draw(RenderTarget& target, RenderStates states) const
{
    assert(m_font != NULL);

    states.Transform *= GetTransform();
    states.BlendMode = BlendAlpha; // alpha blending is mandatory for proper text rendering
    states.Texture = &m_font->GetTexture(m_characterSize);
    target.Draw(m_vertices, states);
}


////////////////////////////////////////////////////////////
void Text::UpdateGeometry()
{
    assert(m_font != NULL);

    // Clear the previous geometry
    m_vertices.Clear();

    // No text: nothing to draw
    if (m_string.IsEmpty())
        return;

    // Compute values related to the text style
    bool  bold               = (m_style & Bold) != 0;
    bool  underlined         = (m_style & Underlined) != 0;
    float italic             = (m_style & Italic) ? 0.208f : 0.f; // 12 degrees
    float underlineOffset    = m_characterSize * 0.1f;
    float underlineThickness = m_characterSize * (bold ? 0.1f : 0.07f);

    // Precompute the variables needed by the algorithm
    float hspace = static_cast<float>(m_font->GetGlyph(L' ', m_characterSize, bold).Advance);
    float vspace = static_cast<float>(m_font->GetLineSpacing(m_characterSize));
    float x      = 0.f;
    float y      = static_cast<float>(m_characterSize);

    // Create one quad for each character
    Uint32 prevChar = 0;
    for (std::size_t i = 0; i < m_string.GetSize(); ++i)
    {
        Uint32 curChar = m_string[i];

        // Apply the kerning offset
        x += static_cast<float>(m_font->GetKerning(prevChar, curChar, m_characterSize));
        prevChar = curChar;

        // If we're using the underlined style and there's a new line, draw a line
        if (underlined && (curChar == L'\n'))
        {
            float top = y + underlineOffset;
            float bottom = top + underlineThickness;

            m_vertices.Append(Vertex(Vector2f(0, top),    m_color, Vector2f(1, 1)));
            m_vertices.Append(Vertex(Vector2f(x, top),    m_color, Vector2f(1, 1)));
            m_vertices.Append(Vertex(Vector2f(x, bottom), m_color, Vector2f(1, 1)));
            m_vertices.Append(Vertex(Vector2f(0, bottom), m_color, Vector2f(1, 1)));
        }

        // Handle special characters
        switch (curChar)
        {
            case L' ' :  x += hspace;        continue;
            case L'\t' : x += hspace * 4;    continue;
            case L'\n' : y += vspace; x = 0; continue;
            case L'\v' : y += vspace * 4;    continue;
        }

        // Extract the current glyph's description
        const Glyph& glyph = m_font->GetGlyph(curChar, m_characterSize, bold);

        int left   = glyph.Bounds.Left;
        int top    = glyph.Bounds.Top;
        int right  = glyph.Bounds.Left + glyph.Bounds.Width;
        int bottom = glyph.Bounds.Top  + glyph.Bounds.Height;

        float u1 = static_cast<float>(glyph.TextureRect.Left);
        float v1 = static_cast<float>(glyph.TextureRect.Top);
        float u2 = static_cast<float>(glyph.TextureRect.Left + glyph.TextureRect.Width);
        float v2 = static_cast<float>(glyph.TextureRect.Top  + glyph.TextureRect.Height);

        // Add a quad for the current character
        m_vertices.Append(Vertex(Vector2f(x + left  - italic * top,    y + top),    m_color, Vector2f(u1, v1)));
        m_vertices.Append(Vertex(Vector2f(x + right - italic * top,    y + top),    m_color, Vector2f(u2, v1)));
        m_vertices.Append(Vertex(Vector2f(x + right - italic * bottom, y + bottom), m_color, Vector2f(u2, v2)));
        m_vertices.Append(Vertex(Vector2f(x + left  - italic * bottom, y + bottom), m_color, Vector2f(u1, v2)));

        // Advance to the next character
        x += glyph.Advance;
    }

    // If we're using the underlined style, add the last line
    if (underlined)
    {
        float top = y + underlineOffset;
        float bottom = top + underlineThickness;

        m_vertices.Append(Vertex(Vector2f(0, top),    m_color, Vector2f(1, 1)));
        m_vertices.Append(Vertex(Vector2f(x, top),    m_color, Vector2f(1, 1)));
        m_vertices.Append(Vertex(Vector2f(x, bottom), m_color, Vector2f(1, 1)));
        m_vertices.Append(Vertex(Vector2f(0, bottom), m_color, Vector2f(1, 1)));
    }

    // Recompute the bounding rectangle
    m_bounds = m_vertices.GetBounds();
}

} // namespace sf
