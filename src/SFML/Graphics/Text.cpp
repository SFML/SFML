////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
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

#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>

namespace
{
    // helper class that divides the string into lines.
    class LineView {
    public:
        LineView(const sf::Uint32* start, std::size_t length) : m_lineBegin(start), m_end(start+length) {
            findNextEnd();
        }

        const sf::Uint32 * getLineStart() const { return m_lineBegin; }
        std::size_t        getLineLen()   const { return m_lineLen;   }
        bool hasNext() const { return m_lineBegin < m_end; }
        void next() { m_lineBegin += m_lineLen+1; findNextEnd(); }
        
    private:
        void findNextEnd() {
            std::size_t lineLen = 0;
            const sf::Uint32 delimiter = static_cast<sf::Uint32>('\n');
            for(const sf::Uint32* it = m_lineBegin; *it != delimiter && it < m_end; ++it, ++lineLen);
            m_lineLen = lineLen;
        }

        const sf::Uint32 * m_lineBegin;
        const sf::Uint32 * m_end;
        std::size_t        m_lineLen;
    };
}

namespace sf
{
////////////////////////////////////////////////////////////
Text::Text() :
m_string            (),
m_font              (NULL),
m_characterSize     (30),
m_style             (Regular),
m_color             (255, 255, 255),
m_vertices          (Triangles),
m_bounds            (),
m_geometryNeedUpdate(false)
{

}


////////////////////////////////////////////////////////////
Text::Text(const String& string, const Font& font, unsigned int characterSize) :
m_string            (string),
m_font              (&font),
m_characterSize     (characterSize),
m_style             (Regular),
m_color             (255, 255, 255),
m_vertices          (Triangles),
m_bounds            (),
m_geometryNeedUpdate(true)
{

}


////////////////////////////////////////////////////////////
void Text::setString(const String& string)
{
    if (m_string != string)
    {
        m_string = string;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
void Text::setFont(const Font& font)
{
    if (m_font != &font)
    {
        m_font = &font;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
void Text::setCharacterSize(unsigned int size)
{
    if (m_characterSize != size)
    {
        m_characterSize = size;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
void Text::setStyle(Uint32 style)
{
    if (m_style != style)
    {
        m_style = style;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
void Text::setColor(const Color& color)
{
    if (color != m_color)
    {
        m_color = color;

        // Change vertex colors directly, no need to update whole geometry
        // (if geometry is updated anyway, we can skip this step)
        if (!m_geometryNeedUpdate)
        {
            for (unsigned int i = 0; i < m_vertices.getVertexCount(); ++i)
                m_vertices[i].color = m_color;
        }
    }
}


////////////////////////////////////////////////////////////
const String& Text::getString() const
{
    return m_string;
}


////////////////////////////////////////////////////////////
const Font* Text::getFont() const
{
    return m_font;
}


////////////////////////////////////////////////////////////
unsigned int Text::getCharacterSize() const
{
    return m_characterSize;
}


////////////////////////////////////////////////////////////
Uint32 Text::getStyle() const
{
    return m_style;
}


////////////////////////////////////////////////////////////
const Color& Text::getColor() const
{
    return m_color;
}


////////////////////////////////////////////////////////////
Vector2f Text::findCharacterPos(std::size_t index) const
{
    // Make sure that we have a valid font
    if (!m_font)
        return Vector2f();

    // Adjust the index if it's out of range
    if (index > m_string.getSize())
        index = m_string.getSize();

    //Algorithm:
    // 1. Find out in which line the character is
    // 2. Shape that line
    // 3. travel over the shaped glyphs until you find the one with the correct cluster
    // Note: Cluster always contains the *lowest* glyph index that went into the glyph
    //       So for example "differ" with "ff" replaced by a ligature would return cluster 
    //       values 0,1,2,4,5, but for RTL languages for example الله with the لله replaced 
    //       by a ligature would return 1,0 (monotonic descending whereas LTR is monotonic ascending)
    
    // Precompute the variables needed by the algorithm
    bool  bold   = (m_style & Bold) != 0;
    float hspace = static_cast<float>(m_font->getGlyph(L' ', m_characterSize, bold).advance);
    float vspace = static_cast<float>(m_font->getLineSpacing(m_characterSize));

    std::size_t lineNumber = 0;
    std::size_t lineStart = 0;
    LineView lineView(m_string.getData(), m_string.getSize());
    for(std::size_t i = 0; i < index; ++i) {
        if (m_string[i] == '\n') {
            lineNumber++;
            lineStart = i + 1;
            lineView.next();
        }
    }
    
    float y;
    if (m_string[index] == '\n') //user requested the position of a newline character, return the beginning of the new line
        return getTransform().transformPoint(Vector2f(0.0f, vspace * (lineNumber + 1)));
    else
        y = vspace * lineNumber;
    
    
    // prepare harfbuzz buffer and font face to shape the complex text
    hb_font_t *hbFtFont = hb_ft_font_create(static_cast<FT_Face>(m_font->m_face), NULL);
    hb_buffer_t *hbBuffer = hb_buffer_create();
    
    // setup the Harfbuzz structures to get the position
    hb_buffer_add_utf32(hbBuffer, lineView.getLineStart(), lineView.getLineLen(), 0, lineView.getLineLen());
        
    // if the script or direction aren't set, try to guess them before shaping
    hb_buffer_guess_segment_properties(hbBuffer);
    hb_shape(hbFtFont, hbBuffer, NULL, 0);
    
    // check if the text is rendered right to left        
    hb_segment_properties_t props;
    hb_buffer_get_segment_properties (hbBuffer, &props);
    const bool isRTL = (props.direction == HB_DIRECTION_RTL);
    
    // from the shaped text we get the glyphs and positions
    unsigned int         glyphCount;
    hb_glyph_info_t     *glyphInfo = hb_buffer_get_glyph_infos(hbBuffer, &glyphCount);
    hb_glyph_position_t *glyphPos  = hb_buffer_get_glyph_positions(hbBuffer, &glyphCount);
    
    // index of the character in the line
    const std::size_t clusterIndex = index - lineStart; 
    
    // We have all the data, now compute the position
    Vector2f position(0.0f, y);
    Vector2f prevPosition = position;
    for ( std::size_t i = 0; i < glyphCount; ++i) {
        hb_glyph_info_t     curGlyph    = glyphInfo[i];
        hb_glyph_position_t curGlyphPos = glyphPos[i];
        
        const int xOffset = (curGlyphPos.x_offset >> 6);
        const int yOffset = (curGlyphPos.y_offset >> 6);
        // Three cases:
        //   1. A glyph matches the letter
        //   2. In RTL mode we found a glyph that corresponds to a letter the preceeds our index
        //   3. In LTR mode we found a glyph that corresponds to a letter the proceeds our index
        //   4. In LTR we got to the last glyph in and still didn't find the index (in RTL the last glyph should always have cluster==0)
        // In cases 2 and 3 we're dealing with a ligature that swallowed up our index.
        if ((curGlyph.cluster == clusterIndex) || (isRTL && curGlyph.cluster < clusterIndex)) {
            // Cases 1 and 2
            position.x += xOffset;
            position.y += yOffset;
            break;
        } else if (!isRTL && curGlyph.cluster > clusterIndex) {
            // Case 3 current position is *after* our index, we need the previous one
            position = prevPosition;
            position.x += xOffset;
            position.y += yOffset;
            break;
        } else if (!isRTL && i == glyphCount - 1) {
            position.x += xOffset;
            position.y += yOffset;
            break;
        }
        
        prevPosition = position;
        prevPosition.x += xOffset;
        prevPosition.y += yOffset;
        
        position.x += curGlyphPos.x_advance >> 6;
        position.y += curGlyphPos.y_advance >> 6;
    }
    
    // Transform the position to global coordinates
    return getTransform().transformPoint(position);
}


////////////////////////////////////////////////////////////
FloatRect Text::getLocalBounds() const
{
    ensureGeometryUpdate();

    return m_bounds;
}


////////////////////////////////////////////////////////////
FloatRect Text::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}


////////////////////////////////////////////////////////////
void Text::draw(RenderTarget& target, RenderStates states) const
{
    if (m_font)
    {
        ensureGeometryUpdate();

        states.transform *= getTransform();
        states.texture = &m_font->getTexture(m_characterSize);
        target.draw(m_vertices, states);
    }
}


////////////////////////////////////////////////////////////
void Text::ensureGeometryUpdate() const
{
    // Do nothing, if geometry has not changed
    if (!m_geometryNeedUpdate)
        return;

    // Mark geometry as updated
    m_geometryNeedUpdate = false;

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
    
    hb_font_t *hbFtFont = hb_ft_font_create(static_cast<FT_Face>(m_font->m_face), NULL);
    hb_buffer_t *hbBuffer = hb_buffer_create();
    
    LineView lineView(m_string.getData(), m_string.getSize());
    while(lineView.hasNext())
    {
        hb_buffer_add_utf32(hbBuffer, lineView.getLineStart(), lineView.getLineLen(), 0, lineView.getLineLen());
        
        // if the script or direction aren't set, try to guess them before shaping
        hb_buffer_guess_segment_properties(hbBuffer);
        hb_shape(hbFtFont, hbBuffer, NULL, 0);
        
        // from the shaped text we get the glyphs and positions
        unsigned int         glyphCount;
        hb_glyph_info_t     *glyphInfo = hb_buffer_get_glyph_infos(hbBuffer, &glyphCount);
        hb_glyph_position_t *glyphPos  = hb_buffer_get_glyph_positions(hbBuffer, &glyphCount);
        
        for (std::size_t i = 0; i < glyphCount; ++i)
        {
            hb_glyph_info_t     curGlyph    = glyphInfo[i];
            hb_glyph_position_t curGlyphPos = glyphPos[i];

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
            m_vertices.append(Vertex(Vector2f(currentX + left  - italic * bottom, currentY + bottom), m_color, Vector2f(u1, v2)));
            m_vertices.append(Vertex(Vector2f(currentX + left  - italic * bottom, currentY + bottom), m_color, Vector2f(u1, v2)));
            m_vertices.append(Vertex(Vector2f(currentX + right - italic * top,    currentY + top),    m_color, Vector2f(u2, v1)));
            m_vertices.append(Vertex(Vector2f(currentX + right - italic * bottom, currentY + bottom), m_color, Vector2f(u2, v2)));

            // Update the current bounds
            minX = std::min(minX, (float)currentX + left - italic * bottom);
            maxX = std::max(maxX, (float)currentX + right - italic * top);
            minY = std::min(minY, (float)currentY + top);
            maxY = std::max(maxY, (float)currentY + bottom);

            // Advance to the next character
            x += curGlyphPos.x_advance >> 6;
            y += curGlyphPos.y_advance >> 6;
        }
        // If we're using the underlined style and there's a new line, draw a line
        if (underlined)
        {
            float top = y + underlineOffset;
            float bottom = top + underlineThickness;

            m_vertices.append(Vertex(Vector2f(0, top),    m_color, Vector2f(1, 1)));
            m_vertices.append(Vertex(Vector2f(x, top),    m_color, Vector2f(1, 1)));
            m_vertices.append(Vertex(Vector2f(0, bottom), m_color, Vector2f(1, 1)));
            m_vertices.append(Vertex(Vector2f(0, bottom), m_color, Vector2f(1, 1)));
            m_vertices.append(Vertex(Vector2f(x, top),    m_color, Vector2f(1, 1)));
            m_vertices.append(Vertex(Vector2f(x, bottom), m_color, Vector2f(1, 1)));
        }

        //next line
        y += vspace; x = 0;
        lineView.next();
    }

    // Update the bounding rectangle
    m_bounds.left = minX;
    m_bounds.top = minY;
    m_bounds.width = maxX - minX;
    m_bounds.height = maxY - minY;
    
    hb_buffer_destroy(hbBuffer);
    hb_font_destroy(hbFtFont);
}

} // namespace sf
