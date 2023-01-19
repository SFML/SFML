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
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <algorithm>
#include <cmath>


namespace
{
// Add an underline or strikethrough line to the vertex array
void addLine(sf::VertexArray& vertices,
             float            lineLength,
             float            lineTop,
             const sf::Color& color,
             float            offset,
             float            thickness,
             float            outlineThickness = 0)
{
    float top    = std::floor(lineTop + offset - (thickness / 2) + 0.5f);
    float bottom = top + std::floor(thickness + 0.5f);

    vertices.append(sf::Vertex(sf::Vector2f(-outlineThickness, top - outlineThickness), color, sf::Vector2f(1, 1)));
    vertices.append(
        sf::Vertex(sf::Vector2f(lineLength + outlineThickness, top - outlineThickness), color, sf::Vector2f(1, 1)));
    vertices.append(sf::Vertex(sf::Vector2f(-outlineThickness, bottom + outlineThickness), color, sf::Vector2f(1, 1)));
    vertices.append(sf::Vertex(sf::Vector2f(-outlineThickness, bottom + outlineThickness), color, sf::Vector2f(1, 1)));
    vertices.append(
        sf::Vertex(sf::Vector2f(lineLength + outlineThickness, top - outlineThickness), color, sf::Vector2f(1, 1)));
    vertices.append(
        sf::Vertex(sf::Vector2f(lineLength + outlineThickness, bottom + outlineThickness), color, sf::Vector2f(1, 1)));
}

// Add a glyph quad to the vertex array
void addGlyphQuad(sf::VertexArray& vertices, sf::Vector2f position, const sf::Color& color, const sf::Glyph& glyph, float italicShear)
{
    float padding = 1.0;

    float left   = glyph.bounds.left - padding;
    float top    = glyph.bounds.top - padding;
    float right  = glyph.bounds.left + glyph.bounds.width + padding;
    float bottom = glyph.bounds.top + glyph.bounds.height + padding;

    float u1 = static_cast<float>(glyph.textureRect.left) - padding;
    float v1 = static_cast<float>(glyph.textureRect.top) - padding;
    float u2 = static_cast<float>(glyph.textureRect.left + glyph.textureRect.width) + padding;
    float v2 = static_cast<float>(glyph.textureRect.top + glyph.textureRect.height) + padding;

    vertices.append(
        sf::Vertex(sf::Vector2f(position.x + left - italicShear * top, position.y + top), color, sf::Vector2f(u1, v1)));
    vertices.append(
        sf::Vertex(sf::Vector2f(position.x + right - italicShear * top, position.y + top), color, sf::Vector2f(u2, v1)));
    vertices.append(
        sf::Vertex(sf::Vector2f(position.x + left - italicShear * bottom, position.y + bottom), color, sf::Vector2f(u1, v2)));
    vertices.append(
        sf::Vertex(sf::Vector2f(position.x + left - italicShear * bottom, position.y + bottom), color, sf::Vector2f(u1, v2)));
    vertices.append(
        sf::Vertex(sf::Vector2f(position.x + right - italicShear * top, position.y + top), color, sf::Vector2f(u2, v1)));
    vertices.append(sf::Vertex(sf::Vector2f(position.x + right - italicShear * bottom, position.y + bottom),
                               color,
                               sf::Vector2f(u2, v2)));
}
} // namespace


namespace sf
{
////////////////////////////////////////////////////////////
Text::Text() = default;


////////////////////////////////////////////////////////////
Text::Text(const String& string, const Font& font, unsigned int characterSize) :
m_string(string),
m_font(&font),
m_characterSize(characterSize)
{
}


////////////////////////////////////////////////////////////
Text::Text(const Text&) = default;


////////////////////////////////////////////////////////////
Text& Text::operator=(const Text&) = default;


////////////////////////////////////////////////////////////
Text::Text(Text&&) noexcept = default;


////////////////////////////////////////////////////////////
Text& Text::operator=(Text&&) noexcept = default;


////////////////////////////////////////////////////////////
void Text::setString(const String& string)
{
    if (m_string != string)
    {
        m_string             = string;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
void Text::setFont(const Font& font)
{
    if (m_font != &font)
    {
        m_font               = &font;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
void Text::setCharacterSize(unsigned int size)
{
    if (m_characterSize != size)
    {
        m_characterSize      = size;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
void Text::setLetterSpacing(float spacingFactor)
{
    if (m_letterSpacingFactor != spacingFactor)
    {
        m_letterSpacingFactor = spacingFactor;
        m_geometryNeedUpdate  = true;
    }
}


////////////////////////////////////////////////////////////
void Text::setLineSpacing(float spacingFactor)
{
    if (m_lineSpacingFactor != spacingFactor)
    {
        m_lineSpacingFactor  = spacingFactor;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
void Text::setStyle(std::uint32_t style)
{
    if (m_style != style)
    {
        m_style              = style;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
void Text::setFillColor(const Color& color)
{
    //In case we use the same color on all characters, no need to store
    //the same color multiple times
    if (m_multiFillColor)
        m_fillColor.resize(1, Color::White);
    m_multiFillColor = 0;
    m_fillColor[0]   = color;

    // Change vertex colors directly, no need to update whole geometry
    // (if geometry is updated anyway, we can skip this step)
    if (!m_geometryNeedUpdate)
    {
        for (std::size_t i = 0; i < m_vertices.getVertexCount(); ++i)
            m_vertices[i].color = color;
    }
}


////////////////////////////////////////////////////////////
void Text::setCharacterFillColor(unsigned int index, const Color& color)
{
    //Set the new per character colors to the color set by setFillColor()
    if (!m_multiFillColor)
    {
        m_fillColor.resize(m_string.getSize());
        for (std::size_t i = 0; i < m_fillColor.size(); i++)
            m_fillColor[i] = m_fillColor[0];
    }
    //Resize the vector so that each character can have a unique color
    //If index is out of bounds the vector will resize to be able to store it
    if (m_fillColor.size() < index + 1)
    {
        m_fillColor.resize(index + 1, Color::White);
    }
    m_multiFillColor = 1;
    if (color != m_fillColor[index])
    {
        m_fillColor[index] = color;

        // Change vertex colors directly, no need to update whole geometry
        // (if geometry is updated anyway, we can skip this step)
        if (!m_geometryNeedUpdate)
        {
            for (std::size_t i = index * 6; i < (index + 1) * 6; ++i)
                m_vertices[i].color = color;
        }
    }
}


////////////////////////////////////////////////////////////
void Text::setOutlineColor(const Color& color)
{
    //In case we use the same color on all characters, no need to store
    //the same color multiple times
    if (m_multiOutlineColor)
        m_outlineColor.resize(1, Color::Black);
    m_multiOutlineColor = 0;
    m_outlineColor[0]   = color;

    // Change vertex colors directly, no need to update whole geometry
    // (if geometry is updated anyway, we can skip this step)
    if (!m_geometryNeedUpdate)
    {
        for (std::size_t i = 0; i < m_outlineVertices.getVertexCount(); ++i)
            m_outlineVertices[i].color = color;
    }
}


////////////////////////////////////////////////////////////
void Text::setCharacterOutlineColor(unsigned int index, const Color& color)
{
    //Set the new per character colors to the color set by setFillColor()
    if (!m_multiOutlineColor)
    {
        m_outlineColor.resize(m_string.getSize());
        for (std::size_t i = 0; i < m_outlineColor.size(); i++)
            m_outlineColor[i] = m_outlineColor[0];
    }
    //Resize the vector so that each character can have a unique color
    //If index is out of bounds the vector will resize to be able to store it
    if (m_outlineColor.size() < index + 1)
    {
        m_outlineColor.resize(index + 1, Color::Black);
    }
    m_multiOutlineColor = 1;
    if (color != m_outlineColor[index])
    {
        m_outlineColor[index] = color;

        // Change vertex colors directly, no need to update whole geometry
        // (if geometry is updated anyway, we can skip this step)
        if (!m_geometryNeedUpdate)
        {
            for (std::size_t i = index * 6; i < (index + 1) * 6; ++i)
                m_outlineVertices[i].color = color;
        }
    }
}


////////////////////////////////////////////////////////////
void Text::setOutlineThickness(float thickness)
{
    if (thickness != m_outlineThickness)
    {
        m_outlineThickness   = thickness;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
void Text::setUnderlineFillColor(const Color& color)
{
    m_underlineFillColor = color;

    // Change vertex colors directly, no need to update whole geometry
    // (if geometry is updated anyway, we can skip this step)
    if (!m_geometryNeedUpdate)
    {
        updateUnderline(m_vertices, color);
    }
}


////////////////////////////////////////////////////////////
void Text::setUnderlineOutlineColor(const Color& color)
{
    m_underlineOutlineColor = color;

    // Change vertex colors directly, no need to update whole geometry
    // (if geometry is updated anyway, we can skip this step)
    if (!m_geometryNeedUpdate)
    {
        updateUnderline(m_outlineVertices, color);
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
float Text::getLetterSpacing() const
{
    return m_letterSpacingFactor;
}


////////////////////////////////////////////////////////////
float Text::getLineSpacing() const
{
    return m_lineSpacingFactor;
}


////////////////////////////////////////////////////////////
std::uint32_t Text::getStyle() const
{
    return m_style;
}


////////////////////////////////////////////////////////////
const Color& Text::getFillColor() const
{
    return getCharacterFillColor(0);
}


////////////////////////////////////////////////////////////
const Color& Text::getCharacterFillColor(unsigned int index) const
{
    return m_fillColor[index];
}


////////////////////////////////////////////////////////////
const Color& Text::getOutlineColor() const
{
    return getCharacterOutlineColor(0);
}


////////////////////////////////////////////////////////////
const Color& Text::getCharacterOutlineColor(unsigned int index) const
{
    return m_outlineColor[index];
}


////////////////////////////////////////////////////////////
float Text::getOutlineThickness() const
{
    return m_outlineThickness;
}


////////////////////////////////////////////////////////////
const Color& Text::getUnderlineFillColor() const
{
    return m_underlineFillColor;
}


////////////////////////////////////////////////////////////
const Color& Text::getUnderlineOutlineColor() const
{
    return m_underlineOutlineColor;
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

    // Precompute the variables needed by the algorithm
    bool  isBold          = m_style & Bold;
    float whitespaceWidth = m_font->getGlyph(U' ', m_characterSize, isBold).advance;
    float letterSpacing   = (whitespaceWidth / 3.f) * (m_letterSpacingFactor - 1.f);
    whitespaceWidth += letterSpacing;
    float lineSpacing = m_font->getLineSpacing(m_characterSize) * m_lineSpacingFactor;

    // Compute the position
    Vector2f      position;
    std::uint32_t prevChar = 0;
    for (std::size_t i = 0; i < index; ++i)
    {
        std::uint32_t curChar = m_string[i];

        // Apply the kerning offset
        position.x += m_font->getKerning(prevChar, curChar, m_characterSize, isBold);
        prevChar = curChar;

        // Handle special characters
        switch (curChar)
        {
            case U' ':
                position.x += whitespaceWidth;
                continue;
            case U'\t':
                position.x += whitespaceWidth * 4;
                continue;
            case U'\n':
                position.y += lineSpacing;
                position.x = 0;
                continue;
        }

        // For regular characters, add the advance offset of the glyph
        position.x += m_font->getGlyph(curChar, m_characterSize, isBold).advance + letterSpacing;
    }

    // Transform the position to global coordinates
    position = getTransform().transformPoint(position);

    return position;
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
void Text::draw(RenderTarget& target, const RenderStates& states) const
{
    if (m_font)
    {
        ensureGeometryUpdate();

        RenderStates statesCopy(states);

        statesCopy.transform *= getTransform();
        statesCopy.texture = &m_font->getTexture(m_characterSize);

        // Only draw the outline if there is something to draw
        if (m_outlineThickness != 0)
            target.draw(m_outlineVertices, statesCopy);

        target.draw(m_vertices, statesCopy);
    }
}


////////////////////////////////////////////////////////////
void Text::ensureGeometryUpdate() const
{
    if (!m_font)
        return;

    // Do nothing, if geometry has not changed and the font texture has not changed
    if (!m_geometryNeedUpdate && m_font->getTexture(m_characterSize).m_cacheId == m_fontTextureId)
        return;

    // Save the current fonts texture id
    m_fontTextureId = m_font->getTexture(m_characterSize).m_cacheId;

    // Mark geometry as updated
    m_geometryNeedUpdate = false;

    // Clear the previous geometry
    m_vertices.clear();
    m_outlineVertices.clear();
    m_bounds = FloatRect();

    // No text: nothing to draw
    if (m_string.isEmpty())
        return;

    //In case that string size changed in between this function and setCharacterColor()
    if (m_fillColor.size() < m_string.getSize())
        m_fillColor.resize(m_string.getSize(), Color::White);
    if (m_outlineColor.size() < m_string.getSize())
        m_outlineColor.resize(m_string.getSize(), Color::Black);

    // Compute values related to the text style
    bool  isBold             = m_style & Bold;
    bool  isUnderlined       = m_style & Underlined;
    bool  isStrikeThrough    = m_style & StrikeThrough;
    float italicShear        = (m_style & Italic) ? sf::degrees(12).asRadians() : 0.f;
    float underlineOffset    = m_font->getUnderlinePosition(m_characterSize);
    float underlineThickness = m_font->getUnderlineThickness(m_characterSize);

    // Compute the location of the strike through dynamically
    // We use the center point of the lowercase 'x' glyph as the reference
    // We reuse the underline thickness as the thickness of the strike through as well
    FloatRect xBounds             = m_font->getGlyph(U'x', m_characterSize, isBold).bounds;
    float     strikeThroughOffset = xBounds.top + xBounds.height / 2.f;

    // Precompute the variables needed by the algorithm
    float whitespaceWidth = m_font->getGlyph(U' ', m_characterSize, isBold).advance;
    float letterSpacing   = (whitespaceWidth / 3.f) * (m_letterSpacingFactor - 1.f);
    whitespaceWidth += letterSpacing;
    float lineSpacing = m_font->getLineSpacing(m_characterSize) * m_lineSpacingFactor;
    float x           = 0.f;
    auto  y           = static_cast<float>(m_characterSize);

    // Create one quad for each character
    auto          minX     = static_cast<float>(m_characterSize);
    auto          minY     = static_cast<float>(m_characterSize);
    float         maxX     = 0.f;
    float         maxY     = 0.f;
    std::uint32_t prevChar = 0;
    for (std::size_t i = 0; i < m_string.getSize(); ++i)
    {
        std::uint32_t curChar = m_string[i];

        // Skip the \r char to avoid weird graphical issues
        if (curChar == U'\r')
            continue;

        // Apply the kerning offset
        x += m_font->getKerning(prevChar, curChar, m_characterSize, isBold);

        // If we're using the underlined style and there's a new line, draw a line
        if (isUnderlined && (curChar == U'\n' && prevChar != U'\n'))
        {
            addLine(m_vertices, x, y, m_underlineFillColor, underlineOffset, underlineThickness);

            if (m_outlineThickness != 0)
                addLine(m_outlineVertices, x, y, m_underlineOutlineColor, underlineOffset, underlineThickness, m_outlineThickness);
        }

        // If we're using the strike through style and there's a new line, draw a line across all characters
        if (isStrikeThrough && (curChar == U'\n' && prevChar != U'\n'))
        {
            addLine(m_vertices, x, y, m_underlineFillColor, strikeThroughOffset, underlineThickness);

            if (m_outlineThickness != 0)
                addLine(m_outlineVertices, x, y, m_underlineOutlineColor, strikeThroughOffset, underlineThickness, m_outlineThickness);
        }

        prevChar = curChar;

        // Handle special characters
        if ((curChar == U' ') || (curChar == U'\n') || (curChar == U'\t'))
        {
            // Update the current bounds (min coordinates)
            minX = std::min(minX, x);
            minY = std::min(minY, y);

            switch (curChar)
            {
                case U' ':
                    x += whitespaceWidth;
                    break;
                case U'\t':
                    x += whitespaceWidth * 4;
                    break;
                case U'\n':
                    y += lineSpacing;
                    x = 0;
                    break;
            }

            // Update the current bounds (max coordinates)
            maxX = std::max(maxX, x);
            maxY = std::max(maxY, y);

            // Next glyph, no need to create a quad for whitespace
            continue;
        }

        // Apply the outline
        if (m_outlineThickness != 0)
        {
            const Glyph& glyph = m_font->getGlyph(curChar, m_characterSize, isBold, m_outlineThickness);

            // Add the outline glyph to the vertices
            addGlyphQuad(m_outlineVertices,
                         Vector2f(x, y),
                         m_multiOutlineColor ? m_outlineColor[i] : m_outlineColor[0],
                         glyph,
                         italicShear);
        }

        // Extract the current glyph's description
        const Glyph& glyph = m_font->getGlyph(curChar, m_characterSize, isBold);

        // Add the glyph to the vertices
        addGlyphQuad(m_vertices, Vector2f(x, y), m_multiFillColor ? m_fillColor[i] : m_fillColor[0], glyph, italicShear);

        // Update the current bounds
        float left   = glyph.bounds.left;
        float top    = glyph.bounds.top;
        float right  = glyph.bounds.left + glyph.bounds.width;
        float bottom = glyph.bounds.top + glyph.bounds.height;

        minX = std::min(minX, x + left - italicShear * bottom);
        maxX = std::max(maxX, x + right - italicShear * top);
        minY = std::min(minY, y + top);
        maxY = std::max(maxY, y + bottom);

        // Advance to the next character
        x += glyph.advance + letterSpacing;
    }

    // If we're using outline, update the current bounds
    if (m_outlineThickness != 0)
    {
        float outline = std::abs(std::ceil(m_outlineThickness));
        minX -= outline;
        maxX += outline;
        minY -= outline;
        maxY += outline;
    }

    // If we're using the underlined style, add the last line
    if (isUnderlined && (x > 0))
    {
        addLine(m_vertices, x, y, m_underlineFillColor, underlineOffset, underlineThickness);

        if (m_outlineThickness != 0)
            addLine(m_outlineVertices, x, y, m_underlineOutlineColor, underlineOffset, underlineThickness, m_outlineThickness);
    }

    // If we're using the strike through style, add the last line across all characters
    if (isStrikeThrough && (x > 0))
    {
        addLine(m_vertices, x, y, m_underlineFillColor, strikeThroughOffset, underlineThickness);

        if (m_outlineThickness != 0)
            addLine(m_outlineVertices, x, y, m_underlineOutlineColor, strikeThroughOffset, underlineThickness, m_outlineThickness);
    }

    // Update the bounding rectangle
    m_bounds.left   = minX;
    m_bounds.top    = minY;
    m_bounds.width  = maxX - minX;
    m_bounds.height = maxY - minY;
}


////////////////////////////////////////////////////////////
void Text::updateUnderline(VertexArray& arr, const Color& color) const
{
    const bool isUnderlined    = m_style & Underlined;
    const bool isStrikeThrough = m_style & StrikeThrough;

    //In case of newlines, find them and change lines
    unsigned int  lineCount = 0;
    std::uint32_t prevChar  = 0;
    for (unsigned int i = 0; i < m_string.getSize(); ++i)
    {
        std::uint32_t curChar = m_string[i];

        if (isUnderlined && (curChar == U'\n' && prevChar != U'\n'))
        {
            for (unsigned int j = 0; j < 6; j++)
                arr[(i + lineCount) * 6U + j].color = color;
            lineCount++;
        }
        if (isStrikeThrough && (curChar == U'\n' && prevChar != U'\n'))
        {
            for (unsigned int j = 0; j < 6; j++)
                arr[(i + lineCount) * 6U + j].color = color;
            lineCount++;
        }
        if (curChar == U'\n' && prevChar != U'\n')
            lineCount--;
    }
    //At the end, there is an extra line
    for (std::size_t i = arr.getVertexCount() - 6 * isUnderlined - 6 * isStrikeThrough; i < arr.getVertexCount(); ++i)
        arr[i].color = color;
}

} // namespace sf
