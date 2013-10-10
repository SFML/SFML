////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Graphics/ComplexText.hpp>
#include <cassert>

#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>

namespace sf
{
////////////////////////////////////////////////////////////
ComplexText::ComplexText() : 
m_dir     (HB_DIRECTION_INVALID), 
m_script  (HB_SCRIPT_INVALID)
{

}


////////////////////////////////////////////////////////////
void ComplexText::SetDirection(hb_direction_t dir)
{
    m_dir = dir;
}


////////////////////////////////////////////////////////////
void ComplexText::SetScript(hb_script_t script)
{
    m_script = script;
}

////////////////////////////////////////////////////////////
void ComplexText::updateGeometry()
{
    // Clear the previous geometry
    m_vertices.clear();
    m_bounds = FloatRect();

    // No font: nothing to draw
    if (!m_font)
        return;

    // No text: nothing to draw
    if (m_string.isEmpty())
        return;

    // Compute values related to the text style
    bool  bold               = (m_style & Bold) != 0;
    bool  underlined         = (m_style & Underlined) != 0;
    float italic             = (m_style & Italic) ? 0.208f : 0.f; // 12 degrees
    float underlineOffset    = m_characterSize * 0.1f;
    float underlineThickness = m_characterSize * (bold ? 0.1f : 0.07f);

    // Precompute the variables needed by the algorithm
    float hspace = static_cast<float>(m_font->getGlyph(L' ', m_characterSize, bold).advance);
    float vspace = static_cast<float>(m_font->getLineSpacing(m_characterSize));
    float x      = 0.f;
    float y      = static_cast<float>(m_characterSize);

    // Create one quad for each character
    float minX = static_cast<float>(m_characterSize);
    float minY = static_cast<float>(m_characterSize);
    float maxX = 0.f;
    float maxY = 0.f;
    Uint32 prevChar = 0;
    
    // prepare harfbuzz buffer and font face to shape the complex text
    hb_font_t *hb_ft_font = hb_ft_font_create(static_cast<FT_Face>(m_font->m_face), NULL);
    hb_buffer_t *buf = hb_buffer_create();
    if (m_dir != HB_DIRECTION_INVALID)
        hb_buffer_set_direction(buf, m_dir);
    if (m_script != HB_SCRIPT_INVALID)
        hb_buffer_set_script(buf, m_script);    
    hb_buffer_add_utf32(buf, m_string.getData(), m_string.getSize(), 0, m_string.getSize());
    
    // if the script or direction aren't set, try to guess them before shaping
    hb_buffer_guess_segment_properties(buf);
    hb_shape(hb_ft_font, buf, NULL, 0);

    // from the shaped text we get the glyphs and positions
    unsigned int         glyph_count;
    hb_glyph_info_t     *glyph_info = hb_buffer_get_glyph_infos(buf, &glyph_count);
    hb_glyph_position_t *glyph_pos  = hb_buffer_get_glyph_positions(buf, &glyph_count);

    // convert this data into geometry for us to render it
    for (std::size_t i = 0; i < glyph_count; ++i)
    {
        hb_glyph_info_t curGlyph = glyph_info[i];
        hb_glyph_position_t curGlyphPos = glyph_pos[i];
        Uint32 curChar = m_string[i];

        prevChar = curChar;

        // If we're using the underlined style and there's a new line, draw a line
        if (underlined && (curChar == L'\n'))
        {
            float top = y + underlineOffset;
            float bottom = top + underlineThickness;

            m_vertices.append(Vertex(Vector2f(0, top),    m_color, Vector2f(1, 1)));
            m_vertices.append(Vertex(Vector2f(x, top),    m_color, Vector2f(1, 1)));
            m_vertices.append(Vertex(Vector2f(x, bottom), m_color, Vector2f(1, 1)));
            m_vertices.append(Vertex(Vector2f(0, bottom), m_color, Vector2f(1, 1)));
        }

        // Extract the current glyph's description
        const Glyph& glyph = m_font->getGlyphByIndex(curGlyph.codepoint, m_characterSize, bold);

        int left   = glyph.bounds.left;
        int top    = glyph.bounds.top;
        int right  = glyph.bounds.left + glyph.bounds.width;
        int bottom = glyph.bounds.top  + glyph.bounds.height;

        float u1 = static_cast<float>(glyph.textureRect.left);
        float v1 = static_cast<float>(glyph.textureRect.top);
        float u2 = static_cast<float>(glyph.textureRect.left + glyph.textureRect.width);
        float v2 = static_cast<float>(glyph.textureRect.top  + glyph.textureRect.height);

        
        int currentX = x + (curGlyphPos.x_offset >> 6);
        int currentY = y + (curGlyphPos.y_offset >> 6);
        // Add a quad for the current character
        m_vertices.append(Vertex(Vector2f(currentX + left  - italic * top,    currentY + top),    m_color, Vector2f(u1, v1)));
        m_vertices.append(Vertex(Vector2f(currentX + right - italic * top,    currentY + top),    m_color, Vector2f(u2, v1)));
        m_vertices.append(Vertex(Vector2f(currentX + right - italic * bottom, currentY + bottom), m_color, Vector2f(u2, v2)));
        m_vertices.append(Vertex(Vector2f(currentX + left  - italic * bottom, currentY + bottom), m_color, Vector2f(u1, v2)));

        // Update the current bounds
        minX = std::min(minX, x + left - italic * bottom);
        maxX = std::max(maxX, x + right - italic * top);
        minY = std::min(minY, y + top);
        maxY = std::max(maxY, y + bottom);

        // Advance to the next character
        x += curGlyphPos.x_advance >> 6;
        y += curGlyphPos.y_advance >> 6;
    }

    // If we're using the underlined style, add the last line
    if (underlined)
    {
        float top = y + underlineOffset;
        float bottom = top + underlineThickness;

        m_vertices.append(Vertex(Vector2f(0, top),    m_color, Vector2f(1, 1)));
        m_vertices.append(Vertex(Vector2f(x, top),    m_color, Vector2f(1, 1)));
        m_vertices.append(Vertex(Vector2f(x, bottom), m_color, Vector2f(1, 1)));
        m_vertices.append(Vertex(Vector2f(0, bottom), m_color, Vector2f(1, 1)));
    }

    // Update the bounding rectangle
    m_bounds.left = minX;
    m_bounds.top = minY;
    m_bounds.width = maxX - minX;
    m_bounds.height = maxY - minY;
    
    hb_buffer_destroy(buf);
    hb_font_destroy(hb_ft_font);
}

}