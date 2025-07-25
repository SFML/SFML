////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif
#include <hb-ft.h>
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include <SheenBidi/SheenBidi.h>
#include <algorithm>
#include <iterator>
#include <limits>
#include <mutex>
#include <utility>

#include <cassert>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <cstdint>


namespace
{
// Add a horizontal underline or strikethrough line to the vertex array
void addLineHorizontal(sf::VertexArray& vertices,
                       float            lineLeft,
                       float            lineRight,
                       float            lineTop,
                       sf::Color        color,
                       float            offset,
                       float            thickness,
                       float            outlineThickness = 0)
{
    const float top    = std::floor(lineTop + offset - (thickness / 2) + 0.5f);
    const float bottom = top + std::floor(thickness + 0.5f);

    vertices.append({{lineLeft - outlineThickness, top - outlineThickness}, color, {1.0f, 1.0f}});
    vertices.append({{lineRight + outlineThickness, top - outlineThickness}, color, {1.0f, 1.0f}});
    vertices.append({{lineLeft - outlineThickness, bottom + outlineThickness}, color, {1.0f, 1.0f}});
    vertices.append({{lineLeft - outlineThickness, bottom + outlineThickness}, color, {1.0f, 1.0f}});
    vertices.append({{lineRight + outlineThickness, top - outlineThickness}, color, {1.0f, 1.0f}});
    vertices.append({{lineRight + outlineThickness, bottom + outlineThickness}, color, {1.0f, 1.0f}});
}

// Add a vertical strikethrough line to the vertex array
void addLineVertical(sf::VertexArray& vertices,
                     float            lineTop,
                     float            lineBottom,
                     sf::Color        color,
                     float            offset,
                     float            thickness,
                     float            outlineThickness = 0)
{
    const float left  = std::floor(offset - (thickness / 2) + 0.5f);
    const float right = left + std::floor(thickness + 0.5f);

    vertices.append({{left - outlineThickness, lineTop - outlineThickness}, color, {1.0f, 1.0f}});
    vertices.append({{right + outlineThickness, lineTop - outlineThickness}, color, {1.0f, 1.0f}});
    vertices.append({{left - outlineThickness, lineBottom + outlineThickness}, color, {1.0f, 1.0f}});
    vertices.append({{left - outlineThickness, lineBottom + outlineThickness}, color, {1.0f, 1.0f}});
    vertices.append({{right + outlineThickness, lineTop - outlineThickness}, color, {1.0f, 1.0f}});
    vertices.append({{right + outlineThickness, lineBottom + outlineThickness}, color, {1.0f, 1.0f}});
}

// Add a glyph quad to the vertex array
void addGlyphQuad(sf::VertexArray& vertices, sf::Vector2f position, sf::Color color, const sf::Glyph& glyph, float italicShear)
{
    const sf::Vector2f padding(1.0f, 1.0f);

    const sf::Vector2f p1 = glyph.bounds.position - padding;
    const sf::Vector2f p2 = glyph.bounds.position + glyph.bounds.size + padding;

    const auto uv1 = sf::Vector2f(glyph.textureRect.position) - padding;
    const auto uv2 = sf::Vector2f(glyph.textureRect.position + glyph.textureRect.size) + padding;

    vertices.append({position + sf::Vector2f(p1.x - italicShear * p1.y, p1.y), color, {uv1.x, uv1.y}});
    vertices.append({position + sf::Vector2f(p2.x - italicShear * p1.y, p1.y), color, {uv2.x, uv1.y}});
    vertices.append({position + sf::Vector2f(p1.x - italicShear * p2.y, p2.y), color, {uv1.x, uv2.y}});
    vertices.append({position + sf::Vector2f(p1.x - italicShear * p2.y, p2.y), color, {uv1.x, uv2.y}});
    vertices.append({position + sf::Vector2f(p2.x - italicShear * p1.y, p1.y), color, {uv2.x, uv1.y}});
    vertices.append({position + sf::Vector2f(p2.x - italicShear * p2.y, p2.y), color, {uv2.x, uv2.y}});
}

struct TextSegment
{
    std::size_t    offset{};
    std::size_t    length{};
    hb_script_t    script{};
    hb_direction_t direction{};
};

// Split string into segments with uniform text properties
std::vector<TextSegment> segmentString(const sf::String& input)
{
    std::vector<TextSegment> segments;

    const SBCodepointSequence codepointSequence{SBStringEncodingUTF32,
                                                static_cast<const void*>(input.getData()),
                                                input.getSize()};
    auto* const               scriptLocator   = SBScriptLocatorCreate();
    auto* const               algorithm       = SBAlgorithmCreate(&codepointSequence);
    SBUInteger                paragraphOffset = 0;

    while (paragraphOffset < input.getSize())
    {
        SBUInteger paragraphLength{};
        SBUInteger separatorLength{};
        SBAlgorithmGetParagraphBoundary(algorithm,
                                        paragraphOffset,
                                        std::numeric_limits<SBUInteger>::max(),
                                        &paragraphLength,
                                        &separatorLength);

        // If the paragraph contains characters besides the separator,
        // split the separator off into its own paragraph in the next iteration
        // We do this to ensure line breaks are inserted into segments last
        // after all character runs on the same line have already been inserted
        // This allows us to draw our segments in left-to-right top-to-bottom order
        if (separatorLength < paragraphLength)
            paragraphLength -= separatorLength;

        auto* const paragraph = SBAlgorithmCreateParagraph(algorithm, paragraphOffset, paragraphLength, SBLevelDefaultLTR);
        auto* const line     = SBParagraphCreateLine(paragraph, paragraphOffset, paragraphLength);
        const auto  runCount = SBLineGetRunCount(line);
        const auto* runArray = SBLineGetRunsPtr(line);

        for (SBUInteger i = 0; i < runCount; i++)
        {
            // Odd levels are RTL, even levels are LTR
            const auto direction = (runArray[i].level % 2) ? HB_DIRECTION_RTL : HB_DIRECTION_LTR;

            const SBCodepointSequence codepointSubsequence{SBStringEncodingUTF32,
                                                           static_cast<const void*>(input.getData() + runArray[i].offset),
                                                           runArray[i].length};

            SBScriptLocatorLoadCodepoints(scriptLocator, &codepointSubsequence);

            while (SBScriptLocatorMoveNext(scriptLocator))
            {
                const auto* agent  = SBScriptLocatorGetAgent(scriptLocator);
                const auto  script = hb_script_from_iso15924_tag(SBScriptGetUnicodeTag(agent->script));

                segments.emplace_back(TextSegment{runArray[i].offset + agent->offset, agent->length, script, direction});
            }

            SBScriptLocatorReset(scriptLocator);
        }

        SBLineRelease(line);
        SBParagraphRelease(paragraph);

        paragraphOffset += paragraphLength;
    }


    SBAlgorithmRelease(algorithm);
    SBScriptLocatorRelease(scriptLocator);

    return segments;
}
} // namespace


namespace sf
{
////////////////////////////////////////////////////////////
struct Text::ShaperImpl
{
    // HarfBuzz font objects are linked to a specific
    // FreeType face and a specific character size
    // If we need to shape text using another font or another
    // character size we will have to create a new shaper
    ShaperImpl(void* fontHandle, std::uint64_t theFontId, unsigned int theCharacterSize) :
        fontId(theFontId),
        characterSize(theCharacterSize),
        shaper(hb_ft_font_create(static_cast<FT_Face>(fontHandle), nullptr)),
        shapingBuffer(hb_buffer_create())
    {
        // Make HarfBuzz use FreeType font functions
        hb_ft_font_set_funcs(shaper.get());
    }

    // To save on memory, instead of every sf::Text object having
    // its own shaper, we try to share shapers among multiple
    // sf::Text objects if they all reference the same font and
    // want to shape text at the same character size
    // Because we are allowing simultaneous access to shapers
    // from multiple sf::Text objects we have to guard both the
    // shaper cache and the shaper objects (which contain buffers)
    // themselves using mutexes
    static std::shared_ptr<ShaperImpl> getShaper(void* fontHandle, std::uint64_t fontId, unsigned int characterSize)
    {
        // Our shaper cache
        struct ShaperCache
        {
            std::mutex                             mutex;
            std::vector<std::weak_ptr<ShaperImpl>> cache;
        };
        static ShaperCache    shaperCache;
        const std::lock_guard lock(shaperCache.mutex);

        std::shared_ptr<ShaperImpl> result;

        // Iterate over the cache looking for a font ID and character size match
        // while at the same time cleaning out stale cache entries
        for (auto iter = shaperCache.cache.begin(); iter != shaperCache.cache.end();)
        {
            if (auto shaper = iter->lock(); !shaper)
            {
                // Remove stale entries
                iter = shaperCache.cache.erase(iter);
            }
            else
            {
                if (shaper->fontId == fontId && shaper->characterSize == characterSize)
                    result = shaper;

                // Don't break even if we find a match since we want to finish cleaning up
                ++iter;
            }
        }

        if (!result)
        {
            // We didn't find a cached shaper, create a new one
            result = std::make_shared<ShaperImpl>(fontHandle, fontId, characterSize);
            shaperCache.cache.emplace_back(result);
        }

        return result;
    }

    // The glyph data that is output after shaping
    struct GlyphData
    {
        std::uint32_t  id{};        //!< Glyph ID (not codepoint!)
        std::uint32_t  cluster{};   //!< Cluster the glyph belongs to
        sf::Vector2f   offset;      //!< The offset to apply to the glyph
        sf::Vector2f   advance;     //!< The advance to apply to the cursor when transitioning to the next glyph
        hb_direction_t direction{}; //!< Text direction
    };

    // Our shape function
    std::vector<GlyphData> shape(
        const sf::String&                 input,
        const std::vector<std::uint32_t>& indices,
        hb_script_t                       script,
        hb_direction_t                    direction,
        sf::Text::TextOrientation         orientation,
        sf::Text::ClusterGrouping         clusterGrouping,
        float                             outlineThickness,
        std::uint32_t                     style)
    {
        assert(input.getSize() == indices.size() && "Input string length does not match indices count");

        // Adjust the direction if a vertical orientation was set
        if (orientation == sf::Text::TextOrientation::TopToBottom)
        {
            direction = HB_DIRECTION_TTB;
        }
        else if (orientation == sf::Text::TextOrientation::BottomToTop)
        {
            direction = HB_DIRECTION_BTT;
        }

        const std::lock_guard lock(mutex);
        auto*                 buffer = shapingBuffer.get();

        // Clear out and add the input to the buffer
        hb_buffer_clear_contents(buffer);
        hb_buffer_pre_allocate(buffer, static_cast<unsigned int>(input.getSize()));

        // Instead of using hb_buffer_add_utf32, we have to use hb_buffer_add
        // to specify the initial cluster IDs for every character
        for (auto i = 0u; i < input.getSize(); ++i)
            hb_buffer_add(buffer, input[i], indices[i]);

        // hb_buffer_add doesn't automatically set the buffer content type so do it now
        hb_buffer_set_content_type(buffer, HB_BUFFER_CONTENT_TYPE_UNICODE);

        // Set the script and direction we detected during segmentation
        hb_buffer_set_script(buffer, script);
        hb_buffer_set_direction(buffer, direction);

        // Try to guess the language of the text the user provided
        hb_buffer_guess_segment_properties(buffer);

        // Set the cluster level
        switch (clusterGrouping)
        {
            case sf::Text::ClusterGrouping::Grapheme:
                hb_buffer_set_cluster_level(buffer, HB_BUFFER_CLUSTER_LEVEL_MONOTONE_GRAPHEMES);
                break;
            case sf::Text::ClusterGrouping::Character:
                hb_buffer_set_cluster_level(buffer, HB_BUFFER_CLUSTER_LEVEL_MONOTONE_CHARACTERS);
                break;
            case sf::Text::ClusterGrouping::None:
                hb_buffer_set_cluster_level(buffer, HB_BUFFER_CLUSTER_LEVEL_CHARACTERS);
                break;
        }

        // Set load flags analogous to the Font implementation
        FT_Int32 flags = FT_LOAD_TARGET_NORMAL;
        if (outlineThickness != 0)
            flags |= FT_LOAD_NO_BITMAP;
        hb_ft_font_set_load_flags(shaper.get(), flags);

        // Shape the text
        hb_shape(shaper.get(), buffer, nullptr, 0);

        // Retrieve position data glyph by glyph
        const auto  glyphCount     = hb_buffer_get_length(buffer);
        const auto* glyphInfo      = hb_buffer_get_glyph_infos(buffer, nullptr);
        const auto* glyphPositions = hb_buffer_get_glyph_positions(buffer, nullptr);

        std::vector<GlyphData> output;
        output.reserve(glyphCount);

        // HarfBuzz returns position data in scaled units
        // We need to convert them to pixels by dividing by the
        // current font scale factoring out the current character size
        // Also flip Y since HarfBuzz uses positive Y up coordinates
        Vector2i scale;
        hb_font_get_scale(shaper.get(), &scale.x, &scale.y);
        const Vector2f divisor{static_cast<float>(scale.x) / static_cast<float>(characterSize),
                               static_cast<float>(scale.y) / -static_cast<float>(characterSize)};

        for (auto i = 0u; i < glyphCount; ++i)
        {
            auto& glyphData = output.emplace_back(
                GlyphData{glyphInfo[i].codepoint,
                          glyphInfo[i].cluster,
                          {std::round(static_cast<float>(glyphPositions[i].x_offset) / divisor.x),
                           std::round(static_cast<float>(glyphPositions[i].y_offset) / divisor.y)},
                          {std::round(static_cast<float>(glyphPositions[i].x_advance) / divisor.x),
                           std::round(static_cast<float>(glyphPositions[i].y_advance) / divisor.y)},
                          hb_buffer_get_direction(buffer)});

            // Adjust advances if we are shaping bold text
            // There doesn't seem to be a standardized way to do this so we will use 0.8f
            // since it sits in the middle ground of common word processor applications
            if ((style & Text::Bold) != 0)
            {
                glyphData.advance.x += (glyphData.advance.x != 0.0f) ? ((glyphData.advance.x >= 0.0f) ? 0.8f : -0.8f) : 0.0f;
                glyphData.advance.y += (glyphData.advance.y != 0.0f) ? ((glyphData.advance.y >= 0.0f) ? 0.8f : -0.8f) : 0.0f;
            }
        }

        return output;
    }

    struct ShaperDeleter
    {
        void operator()(hb_font_t* pointer) const
        {
            hb_font_destroy(pointer);
        }
    };

    struct ShaperBufferDeleter
    {
        void operator()(hb_buffer_t* buffer) const
        {
            hb_buffer_destroy(buffer);
        }
    };

    std::mutex                                        mutex;           //!< Mutex guarding our shaper and shaping buffer
    const std::uint64_t                               fontId{};        //!< Font ID this shaper is linked to
    const unsigned int                                characterSize{}; //!< Character size this shaper was created with
    std::unique_ptr<hb_font_t, ShaperDeleter>         shaper;          //!< Our shaper
    std::unique_ptr<hb_buffer_t, ShaperBufferDeleter> shapingBuffer;   //!< The buffer to perform shaping with
};


////////////////////////////////////////////////////////////
Text::Text(const Font& font, String string, unsigned int characterSize) :
    m_string(std::move(string)),
    m_font(&font),
    m_characterSize(characterSize)
{
}


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
void Text::setFillColor(Color color)
{
    if (color != m_fillColor)
    {
        m_fillColor = color;

        // Change vertex colors directly, no need to update whole geometry
        // (if geometry is updated anyway, we can skip this step)
        if (!m_geometryNeedUpdate)
        {
            for (auto& vertex : m_vertices)
                vertex.color = m_fillColor;
        }
    }
}


////////////////////////////////////////////////////////////
void Text::setOutlineColor(Color color)
{
    if (color != m_outlineColor)
    {
        m_outlineColor = color;

        // Change vertex colors directly, no need to update whole geometry
        // (if geometry is updated anyway, we can skip this step)
        if (!m_geometryNeedUpdate)
        {
            for (auto& vertex : m_outlineVertices)
                vertex.color = m_outlineColor;
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
void Text::setLineAlignment(LineAlignment lineAlignment)
{
    if (m_lineAlignment != lineAlignment)
    {
        m_lineAlignment      = lineAlignment;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
void Text::setTextOrientation(TextOrientation textOrientation)
{
    if (m_textOrientation != textOrientation)
    {
        m_textOrientation    = textOrientation;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
const String& Text::getString() const
{
    return m_string;
}


////////////////////////////////////////////////////////////
const Font& Text::getFont() const
{
    return *m_font;
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
Color Text::getFillColor() const
{
    return m_fillColor;
}


////////////////////////////////////////////////////////////
Color Text::getOutlineColor() const
{
    return m_outlineColor;
}


////////////////////////////////////////////////////////////
float Text::getOutlineThickness() const
{
    return m_outlineThickness;
}


////////////////////////////////////////////////////////////
Text::LineAlignment Text::getLineAlignment() const
{
    return m_lineAlignment;
}


////////////////////////////////////////////////////////////
Text::TextOrientation Text::getTextOrientation() const
{
    return m_textOrientation;
}


////////////////////////////////////////////////////////////
Vector2f Text::findCharacterPos(std::size_t index) const
{
    ensureGeometryUpdate();

    // Adjust the index if it's out of range
    index = std::min(index, m_glyphs.size());

    // Precompute the variables needed by the algorithm
    const bool  isBold          = m_style & Bold;
    const float whitespaceWidth = m_font->getGlyph(U' ', m_characterSize, isBold).advance;
    const float letterSpacing   = (whitespaceWidth / 3.0f) * (m_letterSpacingFactor - 1.0f);
    const float lineSpacing     = m_font->getLineSpacing(m_characterSize) * m_lineSpacingFactor;

    // Special handling for newlines:
    // The old shaping implementation placed newline characters
    // at the start of the new line that they create
    // The new implementation places them at the end of the previous line
    // Placing them at the end of the previous line is normally what is expected
    // since the cursor is rendered before the glyph whose location marks the
    // location at which new characters will be inserted into the text
    // We provide a workaround for the old behavior until findCharacterPos can be removed

    if (m_glyphs.empty())
    {
        // Get the position of the Text itself in global coordinates
        return getTransform().transformPoint({});
    }

    if (index == m_glyphs.size())
    {
        // Return the position of the last glyph + its advance in global coordinates
        const auto& lastGlyph = m_glyphs.back();

        // Newline, past-the-end
        if (m_string[m_string.getSize() - 1] == '\n')
            return getTransform().transformPoint(
                Vector2f{0.0f, lastGlyph.position.y + lineSpacing - static_cast<float>(m_characterSize)});

        return getTransform().transformPoint(
            lastGlyph.position + Vector2f{lastGlyph.glyph.advance + letterSpacing, -static_cast<float>(m_characterSize)});
    }

    // Newline, not past-the-end
    if (m_string[index] == '\n')
        return getTransform().transformPoint(
            Vector2f{0.0f, m_glyphs[index].position.y + lineSpacing - static_cast<float>(m_characterSize)});

    return getTransform().transformPoint(m_glyphs[index].position - Vector2f{0.0f, static_cast<float>(m_characterSize)});
}


////////////////////////////////////////////////////////////
const std::vector<Text::ShapedGlyph>& Text::getShapedGlyphs() const
{
    ensureGeometryUpdate();

    return m_glyphs;
}


////////////////////////////////////////////////////////////
Text::ClusterGrouping Text::getClusterGrouping() const
{
    return m_clusterGrouping;
}


////////////////////////////////////////////////////////////
void Text::setClusterGrouping(ClusterGrouping clusterGrouping)
{
    if (m_clusterGrouping != clusterGrouping)
    {
        m_clusterGrouping    = clusterGrouping;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
void Text::setGlyphPreProcessor(GlyphPreProcessor glyphPreProcessor)
{
    m_glyphPreProcessor  = std::move(glyphPreProcessor);
    m_geometryNeedUpdate = true;
}


////////////////////////////////////////////////////////////
VertexArray& Text::getVertexData() const
{
    return m_vertices;
}


////////////////////////////////////////////////////////////
VertexArray& Text::getOutlineVertexData() const
{
    return m_outlineVertices;
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
    ensureGeometryUpdate();

    states.transform *= getTransform();
    states.texture        = &m_font->getTexture(m_characterSize);
    states.coordinateType = CoordinateType::Pixels;

    // Only draw the outline if there is something to draw
    if (m_outlineVertices.getVertexCount() > 0)
        target.draw(m_outlineVertices, states);

    target.draw(m_vertices, states);
}


////////////////////////////////////////////////////////////
void Text::ensureGeometryUpdate() const
{
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
    m_glyphs.clear();
    m_bounds = FloatRect();

    // No text: nothing to draw
    if (m_string.isEmpty())
        return;

    // Compute values related to the text style
    const bool  isBold             = m_style & Bold;
    const bool  isUnderlined       = m_style & Underlined;
    const bool  isStrikeThrough    = m_style & StrikeThrough;
    const float underlineOffset    = m_font->getUnderlinePosition(m_characterSize);
    const float underlineThickness = m_font->getUnderlineThickness(m_characterSize);

    // Compute the location of the strikethrough dynamically
    // We use the center point of the lowercase 'x' glyph as the reference
    // We reuse the underline thickness as the thickness of the strikethrough as well
    const float strikeThroughOffset = m_font->getGlyph(U'x', m_characterSize, isBold).bounds.getCenter().y;

    // Precompute the variables needed by the algorithm
    const float whitespaceWidth = m_font->getGlyph(U' ', m_characterSize, isBold).advance;
    const float letterSpacing   = (whitespaceWidth / 3.0f) * (m_letterSpacingFactor - 1.0f);
    const float lineSpacing     = m_font->getLineSpacing(m_characterSize) * m_lineSpacingFactor;
    float       x               = 0.0f;
    auto        y = (m_textOrientation == TextOrientation::Default) ? static_cast<float>(m_characterSize) : 0.0f;

    // Variables used to compute bounds
    auto minX = static_cast<float>(m_characterSize);
    auto minY = static_cast<float>(m_characterSize);
    auto maxX = 0.0f;
    auto maxY = 0.0f;

    // Check that we have a usable font
    const auto  fontId     = m_font->getInfo().id;
    auto* const fontHandle = m_font->getFontHandle();

    assert(fontId && fontHandle && "Font not usable for shaping text");

    // Ensure the font is set to the size expected by the shaper
    if (!m_font->setCurrentSize(m_characterSize))
    {
        assert(false && "Failed to set font size");
        return;
    }

    // Shaping only supports single lines, we will need to
    // break multi-line strings into individual lines ourselves
    sf::String                 currentLine;
    std::vector<std::uint32_t> currentLineIndices;
    std::vector<std::uint32_t> currentLineTabIndices;
    hb_script_t                currentScript{};
    hb_direction_t             currentDirection{};

    const auto outputLine = [&]
    {
        if (!m_shaper || m_shaper->fontId != fontId || m_shaper->characterSize != m_characterSize)
        {
            // We need to get a new shaper implementation
            m_shaper = ShaperImpl::getShaper(fontHandle, fontId, m_characterSize);
        }

        const auto shapeOutput = m_shaper->shape(currentLine,
                                                 currentLineIndices,
                                                 currentScript,
                                                 currentDirection,
                                                 m_textOrientation,
                                                 m_clusterGrouping,
                                                 m_outlineThickness,
                                                 m_style & Bold);

        // Variables used to compute bounds for the current line
        auto lineMinX = static_cast<float>(m_characterSize);
        auto lineMinY = static_cast<float>(m_characterSize);
        auto lineMaxX = 0.0f;
        auto lineMaxY = 0.0f;

        // Track widths and y positions of underline segments when applying underline to vertical text
        struct VerticalUnderlineEntry
        {
            sf::Vector2f position;
            float        width{};
        };

        std::vector<VerticalUnderlineEntry> verticalUnderlineData;

        if (m_textOrientation != TextOrientation::Default)
            verticalUnderlineData.reserve(shapeOutput.size());

        // We want to combine the multiple spaces we substituted a tab with back into a single glyph
        // To do that we just enlarge the first of its shaped spaces to 4 times its width
        // We then skip the following 3 spaces
        auto glyphsToSkip = 0;

        // Create one quad for each character
        for (const auto& shapeGlyph : shapeOutput)
        {
            // Skip trailing spaces of a tab
            if (glyphsToSkip > 0)
            {
                --glyphsToSkip;
                continue;
            }

            // Extract the current glyph's description
            const Glyph& glyph = m_font->getGlyphById(shapeGlyph.id, m_characterSize, isBold);

            // Add the glyph to the glyph list
            auto& glyphEntry    = m_glyphs.emplace_back(ShapedGlyph{glyph, {}, {}, {}});
            glyphEntry.cluster  = shapeGlyph.cluster;
            glyphEntry.position = sf::Vector2f{x, y} + shapeGlyph.offset;
            auto isHorizontal   = false;
            auto isVertical     = false;

            switch (shapeGlyph.direction)
            {
                case HB_DIRECTION_LTR:
                    glyphEntry.textDirection = TextDirection::LeftToRight;
                    glyphEntry.glyph.advance = shapeGlyph.advance.x;
                    glyphEntry.baseline      = y;
                    isHorizontal             = true;
                    break;
                case HB_DIRECTION_RTL:
                    glyphEntry.textDirection = TextDirection::RightToLeft;
                    glyphEntry.glyph.advance = shapeGlyph.advance.x;
                    glyphEntry.baseline      = y;
                    isHorizontal             = true;
                    break;
                case HB_DIRECTION_TTB:
                    glyphEntry.textDirection = TextDirection::TopToBottom;
                    glyphEntry.glyph.advance = shapeGlyph.advance.y;
                    glyphEntry.baseline      = x;
                    isVertical               = true;
                    break;
                case HB_DIRECTION_BTT:
                    glyphEntry.textDirection = TextDirection::BottomToTop;
                    glyphEntry.glyph.advance = shapeGlyph.advance.y;
                    glyphEntry.baseline      = x;
                    isVertical               = true;
                    break;
                default:
                    glyphEntry.textDirection = TextDirection::Unspecified;
                    break;
            }

            std::uint32_t style            = m_style;
            Color         fillColor        = m_fillColor;
            Color         outlineColor     = m_outlineColor;
            float         outlineThickness = m_outlineThickness;
            float         italicShear      = 0.0f;

            // Add the glyph to the vertices, if the texture rect has an area
            // (not the case with transparent glyphs e.g. space character)
            if (glyph.textureRect.size.x != 0 && glyph.textureRect.size.y != 0)
            {
                if (m_glyphPreProcessor)
                    m_glyphPreProcessor(glyphEntry, style, fillColor, outlineColor, outlineThickness);

                italicShear = (style & Italic) ? degrees(12).asRadians() : 0.0f;

                // Apply the outline
                if (outlineThickness != 0)
                {
                    const Glyph& outlineGlyph = m_font->getGlyphById(shapeGlyph.id, m_characterSize, style & Bold, outlineThickness);

                    // Add the outline glyph to the vertices
                    addGlyphQuad(m_outlineVertices, glyphEntry.position, outlineColor, outlineGlyph, italicShear);
                }

                glyphEntry.vertexOffset = m_vertices.getVertexCount();

                const Glyph& fillGlyph = m_font->getGlyphById(shapeGlyph.id, m_characterSize, style & Bold);
                addGlyphQuad(m_vertices, glyphEntry.position, fillColor, fillGlyph, italicShear);

                glyphEntry.vertexCount = m_vertices.getVertexCount() - glyphEntry.vertexOffset;
            }
            else
            {
                // Remove unwanted x-offset when dealing with vertical spaces
                if ((shapeGlyph.direction == HB_DIRECTION_TTB) || (shapeGlyph.direction == HB_DIRECTION_BTT))
                    glyphEntry.position.x -= shapeGlyph.offset.x;

                // Check if we are dealing with the start of a tab
                if (std::find(currentLineTabIndices.begin(), currentLineTabIndices.end(), shapeGlyph.cluster) !=
                    currentLineTabIndices.end())
                {
                    // Widen the size to 4 times the advance of the space glyph
                    glyphEntry.glyph.advance *= 4.0f;

                    if (isHorizontal)
                    {
                        glyphEntry.glyph.bounds.size.x = glyphEntry.glyph.advance;
                    }
                    else if (isVertical)
                    {
                        glyphEntry.glyph.bounds.size.y = glyphEntry.glyph.advance;
                    }

                    // Skip the next 3 space glyphs
                    glyphsToSkip = 3;
                }
                else
                {
                    // If the glyph doesn't have a texture it is probably a space
                    // Set the bounds width to the width of the space so the user can
                    // make use of it
                    if (isHorizontal)
                    {
                        glyphEntry.glyph.bounds.size = {shapeGlyph.advance.x + letterSpacing, 0.0f};
                    }
                    else if (isVertical)
                    {
                        glyphEntry.glyph.bounds.size = {0.0f, shapeGlyph.advance.y + letterSpacing};
                    }
                }

                if (m_glyphPreProcessor)
                    m_glyphPreProcessor(glyphEntry, style, fillColor, outlineColor, outlineThickness);

                italicShear = (style & Italic) ? degrees(12).asRadians() : 0.0f;
            }

            // Update the current bounds
            const Vector2f p1 = glyph.bounds.position + shapeGlyph.offset;
            const Vector2f p2 = p1 + glyphEntry.glyph.bounds.size;

            lineMinX = std::min(lineMinX, x + p1.x - italicShear * p2.y);
            lineMaxX = std::max(lineMaxX, x + p2.x - italicShear * p1.y);
            lineMinY = std::min(lineMinY, y + p1.y);
            lineMaxY = std::max(lineMaxY, y + p2.y);

            // Advance to the next character
            // Only apply additional letter spacing if the current glyph has an advance (base glyph)
            // Applying the letter spacing to glyphs without an advance would affect
            // mark glyphs as well which would lead to incorrect results
            const auto hasAdvance = glyphEntry.glyph.advance > 0.0f;

            if (isHorizontal)
            {
                x += glyphEntry.glyph.advance + (hasAdvance ? letterSpacing : 0.0f);
            }
            else if (isVertical)
            {
                // We only add data to the vertical underline list for base glyphs
                if (isUnderlined && hasAdvance)
                    verticalUnderlineData.emplace_back(
                        VerticalUnderlineEntry{glyphEntry.position + sf::Vector2f(glyphEntry.glyph.bounds.position.x, 0.0f),
                                               glyphEntry.glyph.bounds.size.x});

                y += glyphEntry.glyph.advance + (hasAdvance ? letterSpacing : 0.0f);
            }
        }

        // Update the multi-line bounds
        minX = std::min(minX, lineMinX);
        maxX = std::max(maxX, lineMaxX);
        minY = std::min(minY, lineMinY);
        maxY = std::max(maxY, lineMaxY);

        currentLine.clear();
        currentLineIndices.clear();
        currentLineTabIndices.clear();

        // If we're using the underlined style and there's a new line, draw a line
        if (isUnderlined)
        {
            if (m_textOrientation == TextOrientation::Default)
            {
                addLineHorizontal(m_vertices, 0.0f, x, y, m_fillColor, underlineOffset, underlineThickness);

                if (m_outlineThickness != 0)
                    addLineHorizontal(m_outlineVertices, 0.0f, x, y, m_outlineColor, underlineOffset, underlineThickness, m_outlineThickness);
            }
            else
            {
                // If we have to apply underline to vertical text we will have to do it per glyph
                for (const auto& entry : verticalUnderlineData)
                {
                    addLineHorizontal(m_vertices,
                                      entry.position.x,
                                      entry.position.x + entry.width,
                                      entry.position.y,
                                      m_fillColor,
                                      underlineOffset,
                                      underlineThickness);

                    if (m_outlineThickness != 0)
                        addLineHorizontal(m_outlineVertices,
                                          entry.position.x,
                                          entry.position.x + entry.width,
                                          entry.position.y,
                                          m_outlineColor,
                                          underlineOffset,
                                          underlineThickness,
                                          m_outlineThickness);
                }
            }
        }

        // If we're using the strikethrough style and there's a new line, draw a line across all characters
        if (isStrikeThrough)
        {
            if (m_textOrientation == TextOrientation::Default)
            {
                addLineHorizontal(m_vertices, 0.0f, x, y, m_fillColor, strikeThroughOffset, underlineThickness);

                if (m_outlineThickness != 0)
                    addLineHorizontal(m_outlineVertices, 0.0f, x, y, m_outlineColor, strikeThroughOffset, underlineThickness, m_outlineThickness);
            }
            else
            {
                // Slanting the text by 12 degrees means the strikethrough would
                // have to be offset by
                // width * sin(12 degrees) / 2
                // which is approximately
                // width * 0.1
                addLineVertical(m_vertices,
                                lineMinY,
                                lineMaxY,
                                m_fillColor,
                                (m_style & Italic) ? (lineMaxX - lineMinX) * 0.1f : 0.0f,
                                underlineThickness);

                if (m_outlineThickness != 0)
                    addLineVertical(m_outlineVertices,
                                    lineMinY,
                                    lineMaxY,
                                    m_outlineColor,
                                    (m_style & Italic) ? (lineMaxX - lineMinX) * 0.1f : 0.0f,
                                    underlineThickness,
                                    m_outlineThickness);
            }
        }
    };

    // Split the input string into multiple segments with uniform
    // script and direction using the unicode bidirectional algorithm
    const auto segments = segmentString(m_string);

    // In order to be able to align text we have to record all line data until we can compute the text metrics
    // We then use the record data to shift the necessary lines to the right/left as necessary
    struct LineRecord
    {
        std::size_t    glyphsStart{};
        std::size_t    glyphsCount{};
        std::size_t    verticesStart{};
        std::size_t    verticesCount{};
        std::size_t    outlineVerticesStart{};
        std::size_t    outlineVerticesCount{};
        std::size_t    firstCodepointOffset = std::numeric_limits<std::size_t>::max();
        hb_direction_t direction{};
        float          lineWidth{};
    };

    std::vector<LineRecord> lines;

    const auto beginLineRecord = [&]
    {
        // Start a new line record
        auto& lineRecord                = lines.emplace_back();
        lineRecord.glyphsStart          = m_glyphs.size();
        lineRecord.verticesStart        = m_vertices.getVertexCount();
        lineRecord.outlineVerticesStart = m_outlineVertices.getVertexCount();
    };

    const auto endLineRecord = [&]
    {
        // Complete the line record
        auto& lineRecord                = lines.back();
        lineRecord.glyphsCount          = m_glyphs.size() - lineRecord.glyphsStart;
        lineRecord.verticesCount        = m_vertices.getVertexCount() - lineRecord.verticesStart;
        lineRecord.outlineVerticesCount = m_outlineVertices.getVertexCount() - lineRecord.outlineVerticesStart;
        lineRecord.lineWidth            = x;
    };

    if (!segments.empty())
        beginLineRecord();

    // Iterate over all segments
    for (const auto& segment : segments)
    {
        currentScript    = segment.script;
        currentDirection = segment.direction;

        if (segment.offset < lines.back().firstCodepointOffset)
        {
            lines.back().firstCodepointOffset = segment.offset;
            lines.back().direction            = currentDirection;
        }

        // We use the index into the input string as the input cluster IDs as well
        // This way the user will be able to map the resulting cluster IDs back to
        // characters in the input text they provided for advanced functionality
        for (auto index = static_cast<std::uint32_t>(segment.offset);
             index < static_cast<std::uint32_t>(segment.offset + segment.length);
             ++index)
        {
            const auto& curChar = m_string[index];

            // Handle special characters
            if ((curChar == U'\n'))
            {
                if (!currentLine.isEmpty())
                    outputLine();

                // Add new entry to glyphs
                // For our purposes, we consider the newline
                // character to constitute its own cluster
                auto& glyph = m_glyphs.emplace_back(
                    ShapedGlyph{m_font->getGlyph('\n', m_characterSize, isBold), {}, {}, {}});
                glyph.glyph.bounds.size = {0.0f, 0.0f};
                glyph.baseline          = y;

                // We give some effort to position the newline glyph "after"
                // its preceeding glyph taking into account the text direction
                if (m_glyphs.size() == 1)
                {
                    // Nothing but a newline so far, nothing we can do
                    glyph.position = {x, y};
                }
                else
                {
                    // Search for the cluster with the highest cluster value, see comment below on why this works
                    // We are guaranteed to find an element, so the iterator returned is guaranteed to be valid
                    const auto& highestClusterGlyph = *std::max_element(m_glyphs.begin() +
                                                                            static_cast<int>(lines.back().glyphsStart),
                                                                        m_glyphs.end(),
                                                                        [](const ShapedGlyph& left, const ShapedGlyph& right)
                                                                        { return left.cluster < right.cluster; });

                    glyph.position = {highestClusterGlyph.position.x +
                                          (highestClusterGlyph.textDirection == TextDirection::RightToLeft
                                               ? 0.0f
                                               : highestClusterGlyph.glyph.advance),
                                      y};
                }

                // Only set the cluster here so we can search for the glyph
                // with the highest cluster before this one was inserted
                // ShapedGlyph clusters get initialized to 0 and if there
                // are multiple elements in m_glyphs this means they will
                // be returned instead of this glyph
                glyph.cluster = index;

                endLineRecord();
                beginLineRecord();

                // Update the current bounds (min coordinates)
                minX = std::min(minX, x);
                minY = std::min(minY, y);

                y += lineSpacing;
                x = 0;

                // Update the current bounds (max coordinates)
                maxX = std::max(maxX, x);
                maxY = std::max(maxY, y);

                // Next glyph, no need to create a quad for newline
                continue;
            }

            if (curChar == U'\t')
            {
                // Replace tab character with 4 spaces for shaping
                currentLine += "    ";
                currentLineIndices.resize(currentLineIndices.size() + 4u, index);
                currentLineTabIndices.emplace_back(index);
                continue;
            }

            if ((curChar < 0x80) && std::iscntrl(static_cast<int>(curChar)))
            {
                // Skip all other control characters to avoid weird graphical issues
                continue;
            }

            currentLine += curChar;
            currentLineIndices.emplace_back(index);
        }

        if (!currentLine.isEmpty())
            outputLine();
    }

    if (!segments.empty())
        endLineRecord();

    // Sort shaped glyphs so that clusters are in ascending order
    std::sort(m_glyphs.begin(),
              m_glyphs.end(),
              [](const ShapedGlyph& left, const ShapedGlyph& right) { return left.cluster < right.cluster; });

    // If we're using outline, update the current bounds
    if (m_outlineThickness != 0)
    {
        const float outline = std::abs(std::ceil(m_outlineThickness));
        minX -= outline;
        maxX += outline;
        minY -= outline;
        maxY += outline;
    }

    // Update the bounding rectangle
    m_bounds.position = Vector2f(minX, minY);
    m_bounds.size     = Vector2f(maxX, maxY) - Vector2f(minX, minY);

    // Use line record data to post-process lines e.g. re-alignment etc.
    if (!lines.empty())
    {
        // Get width of widest line
        const auto maxWidth = std::max_element(lines.begin(),
                                               lines.end(),
                                               [](const LineRecord& left, const LineRecord& right)
                                               { return left.lineWidth < right.lineWidth; })
                                  ->lineWidth;

        for (auto& line : lines)
        {
            auto shift = 0.0f;

            if (m_lineAlignment == LineAlignment::Center)
            {
                shift = line.lineWidth / -2.0f;
            }
            else if (m_lineAlignment == LineAlignment::Right)
            {
                shift = -line.lineWidth;
            }
            else if ((m_lineAlignment == LineAlignment::Default) && (line.direction == HB_DIRECTION_RTL))
            {
                shift = maxWidth - line.lineWidth;
            }
            else
            {
                // Skip modifying the data if there is nothing to shift
                continue;
            }

            // Shift glyphs
            for (auto i = line.glyphsStart; i < line.glyphsStart + line.glyphsCount; ++i)
                m_glyphs[i].position.x += shift;

            // Shift vertices
            for (auto i = line.verticesStart; i < line.verticesStart + line.verticesCount; ++i)
                m_vertices[i].position.x += shift;

            // Shift vertices
            for (auto i = line.outlineVerticesStart; i < line.outlineVerticesStart + line.outlineVerticesCount; ++i)
                m_outlineVertices[i].position.x += shift;
        }

        // Update bounds if necessary
        if (m_lineAlignment == LineAlignment::Center)
        {
            m_bounds.position.x -= m_bounds.size.x / 2.0f;
        }
        else if (m_lineAlignment == LineAlignment::Right)
        {
            m_bounds.position.x -= m_bounds.size.x;
        }
    }
}

} // namespace sf
