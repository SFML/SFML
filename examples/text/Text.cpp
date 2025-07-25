//////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>

#include <algorithm>
#include <array>
#include <chrono>
#include <filesystem>
#include <memory>
#include <string_view>
#include <vector>

#include <cmath>


#define STR_HELPER(x) #x
#define STR(x)        STR_HELPER(x)

// SFML version string wrapped in unicode directional formatting characters
#define SFML_VERSION_STRING \
    "\u202A" STR(SFML_VERSION_MAJOR) "." STR(SFML_VERSION_MINOR) "." STR(SFML_VERSION_PATCH) "\u202C"


namespace
{
constexpr auto windowWidth           = 1200u;
constexpr auto textSize              = 23u;
constexpr auto textSpacing           = 5.0f;
constexpr auto rainbowDeltaPerSecond = 60.f;
} // namespace


////////////////////////////////////////////////////////////
// Demonstration Text class
////////////////////////////////////////////////////////////
struct DemoText
{
    DemoText(const std::filesystem::path& fontFilename, std::string_view message) :
        font(std::make_unique<sf::Font>("resources" / fontFilename)),
        text(*font, sf::String::fromUtf8(message.begin(), message.end()), textSize)
    {
        // Generate per-character effect data
        // Since this is peformed on the input string data it only has to be done once
        generateEffectData();

        const auto localBounds = text.getLocalBounds();

        // Fix cases where the text is so tall it starts at a negative offset
        currentYPosition -= localBounds.position.y;
        text.setPosition({(static_cast<float>(windowWidth) - localBounds.size.x) / 2.0f, currentYPosition});
        currentYPosition = text.getGlobalBounds().position.y + text.getGlobalBounds().size.y + textSpacing;

        // Set up text bounding box
        boundingBox.setFillColor(sf::Color::Transparent);
        boundingBox.setOutlineColor(sf::Color::Red);
        boundingBox.setOutlineThickness(1.0f);

        // Set up glyph bounding box
        glyphBox.setFillColor(sf::Color::Transparent);
        glyphBox.setOutlineColor(sf::Color::Green);
        glyphBox.setOutlineThickness(1.0f);

        // Set up cursor
        cursor.setFillColor(sf::Color::White);

        // Set up dynamic data
        update();
    }

    void setPosition(const sf::Vector2f& position)
    {
        text.setPosition(position);
        update();
    }

    void setLineAlignment(sf::Text::LineAlignment lineAlignment)
    {
        text.setLineAlignment(lineAlignment);
        update();
    }

    void setStyle(std::uint32_t style)
    {
        text.setStyle(style);
        update();
    }

    void setTextOrientation(sf::Text::TextOrientation orientation)
    {
        text.setTextOrientation(orientation);
        update();
    }

    void colorTextDirection(bool enable)
    {
        colorDirection = enable;
    }

    void drawBoundingBox(sf::RenderTarget& target) const
    {
        target.draw(boundingBox);
    }

    void drawGlyphBox(sf::RenderTarget& target) const
    {
        target.draw(glyphBox, text.getTransform());
    }

    void drawCursor(sf::RenderTarget& target) const
    {
        target.draw(cursor, text.getTransform());
    }

    void drawText(sf::RenderTarget& target) const
    {
        target.draw(text);
    }

    void update()
    {
        // Update text bounding box
        const auto localBounds = text.getLocalBounds();
        boundingBox.setPosition(text.getPosition() + localBounds.position);
        boundingBox.setSize(localBounds.size);

        // Position the glyph bounding box and cursor
        updateGlyphBox(0);
        computeCursorPositions();
        updateCursorPosition(0);

        // Update rainbow text default y positions
        const auto& vertexData = text.getVertexData();
        rainbowYPositions.resize(vertexData.getVertexCount());
        std::transform(vertexData.begin(),
                       vertexData.end(),
                       rainbowYPositions.begin(),
                       [](const sf::Vertex& vertex) { return vertex.position.y; });
    }

    void updateGlyphBox(int offset)
    {
        const auto& shapedGlyphs = text.getShapedGlyphs();

        glyphIndex = static_cast<std::size_t>(
            std::clamp(static_cast<int>(glyphIndex) + offset, 0, static_cast<int>(shapedGlyphs.size()) - 1));

        const auto& shapedGlyph = shapedGlyphs[glyphIndex];
        const auto& localBounds = text.getLocalBounds();
        const auto  boxHeight   = static_cast<float>(text.getCharacterSize());

        // Check if we are dealing with whitespace or a glyph with a texture
        // The bounds of a whitespace glyph will either have no width or no height depending on orientation
        if (shapedGlyph.glyph.bounds.size.y == 0.0f)
        {
            // Horizontal space
            glyphBox.setPosition({(shapedGlyph.position + shapedGlyph.glyph.bounds.position).x,
                                  localBounds.position.y + shapedGlyph.position.y - boxHeight});
            glyphBox.setSize({shapedGlyph.glyph.bounds.size.x, boxHeight});
            glyphBox.setOutlineColor(sf::Color(0, 127, 255));
        }
        else if (shapedGlyph.glyph.bounds.size.x == 0.0f)
        {
            // Vertical space
            glyphBox.setPosition({-localBounds.size.x / 2.0f,
                                  localBounds.position.y + shapedGlyph.position.y - shapedGlyph.glyph.bounds.size.y});
            glyphBox.setSize({localBounds.size.x, shapedGlyph.glyph.bounds.size.y});
            glyphBox.setOutlineColor(sf::Color(0, 127, 255));
        }
        else
        {
            glyphBox.setPosition({shapedGlyph.position + shapedGlyph.glyph.bounds.position});
            glyphBox.setSize(shapedGlyph.glyph.bounds.size);
            glyphBox.setOutlineColor(sf::Color::Green);
        }
    }

    void computeCursorPositions()
    {
        cursorPositions.clear();

        const auto              globalBounds    = text.getGlobalBounds();
        const auto&             shapedGlyphs    = text.getShapedGlyphs();
        const auto              ascent          = font->getAscent(text.getCharacterSize());
        const auto              descent         = font->getDescent(text.getCharacterSize());
        auto                    previousCluster = 0u;
        sf::Vector2f            previousClusterPosition;
        sf::Vector2f            previousClusterSize;
        float                   previousClusterAdvance{};
        sf::Text::TextDirection previousClusterDirection{};

        // Set size of cursor depending on text orientation
        if (text.getTextOrientation() == sf::Text::TextOrientation::Default)
        {
            // Horizontal (Vertical cursor)
            cursor.setSize({1.0f, ascent - descent});
        }
        else
        {
            // Vertical (Horizontal cursor)
            // In this example we don't do anything fancy and just set the cursor width to the width of the bounds
            cursor.setSize({globalBounds.size.x, 1.0f});
        }

        for (const auto& shapedGlyph : shapedGlyphs)
        {
            // Combining marks can't be valid cursor positions so
            // we just skip over trailing glyphs that share the same
            // cluster value as the base glyph that preceeds them
            if (previousCluster != 0u && shapedGlyph.cluster == previousCluster)
                continue;

            sf::Vector2f clusterPosition{shapedGlyph.position.x, shapedGlyph.baseline - ascent};

            // Remember that in RTL text, the "before grapheme position" is to the right of the glyph
            if (shapedGlyph.textDirection == sf::Text::TextDirection::RightToLeft)
                clusterPosition.x += shapedGlyph.glyph.bounds.size.x;

            const auto isVerticalWhitespace = shapedGlyph.glyph.bounds.size.x == 0.0f;

            // For vertical orientations, the "before grapheme position" is to the top/bottom of the glyph
            if (shapedGlyph.textDirection == sf::Text::TextDirection::TopToBottom)
            {
                clusterPosition = sf::Vector2f{shapedGlyph.position.x +
                                                   (shapedGlyph.glyph.bounds.size.x - globalBounds.size.x) / 2.0f,
                                               shapedGlyph.position.y -
                                                   (isVerticalWhitespace ? shapedGlyph.glyph.bounds.size.y / 2.0f : ascent)};
            }
            else if (shapedGlyph.textDirection == sf::Text::TextDirection::BottomToTop)
            {
                clusterPosition = sf::Vector2f{shapedGlyph.position.x +
                                                   (shapedGlyph.glyph.bounds.size.x - globalBounds.size.x) / 2.0f,
                                               shapedGlyph.position.y -
                                                   (isVerticalWhitespace ? -shapedGlyph.glyph.bounds.size.y / 2.0f : descent)};
            }

            // When using high quality fonts, adjacent graphemes can be combined
            // into single glyph ligatures if the font designer finds that it
            // would result in a better graphical representation.
            // Common examples in latin script would be the character combinations:
            // ff, fi, fl, ffi and ffl
            // While being the exception in latin script, other scripts might
            // be represented primarily using ligatures.
            // In most software, positioning the cursor within a single glyph
            // ligature is done by splitting the total width of the ligature up
            // into equal parts for each of its constituent graphemes
            const auto clusterSubWidth = (clusterPosition - previousClusterPosition).x /
                                         static_cast<float>(shapedGlyph.cluster - previousCluster);

            // Fill in the "gaps" left by shaping i.e. the character index values
            // that got merged into another cluster and are no longer present
            // in the shaped glyph data
            while (shapedGlyph.cluster - previousCluster > 1)
            {
                // We need to interpolate grapheme's position based on the position
                // of the preceeding base glyph and the following base glyph
                previousClusterPosition.x += clusterSubWidth;
                ++previousCluster;
                cursorPositions.emplace_back(previousClusterPosition);
            }

            // Add the entry for this base glyph
            previousCluster          = shapedGlyph.cluster;
            previousClusterPosition  = clusterPosition;
            previousClusterSize      = shapedGlyph.glyph.bounds.size;
            previousClusterAdvance   = shapedGlyph.glyph.advance;
            previousClusterDirection = shapedGlyph.textDirection;
            cursorPositions.emplace_back(previousClusterPosition);
        }

        // Handle special case in which the last shaped glyph is itself a ligature
        // This is the case if we get to this point and there are less cursor
        // positions than codepoints in the input string
        const auto codepointCount = text.getString().getSize();

        if (cursorPositions.size() < codepointCount)
        {
            // In this case, we don't have a following glyph to use for interpolation
            // We will just have to fall back to using the glyph advance
            const auto followingCluster = codepointCount;
            const auto clusterSubWidth  = (previousClusterDirection == sf::Text::TextDirection::RightToLeft
                                               ? -previousClusterAdvance
                                               : previousClusterAdvance) /
                                         static_cast<float>(followingCluster - previousCluster);

            // Keep adding positions until we have an equal amount as codepoints
            auto currentPosition = previousClusterPosition;
            while (cursorPositions.size() < codepointCount)
            {
                currentPosition.x += clusterSubWidth;
                cursorPositions.emplace_back(currentPosition);
            }
        }

        // Add past-the-end position
        switch (previousClusterDirection)
        {
            case sf::Text::TextDirection::LeftToRight:
                cursorPositions.emplace_back(
                    previousClusterPosition + sf::Vector2f{std::max(previousClusterSize.x, previousClusterAdvance), 0.0f});
                break;
            case sf::Text::TextDirection::RightToLeft:
                cursorPositions.emplace_back(previousClusterPosition +
                                             sf::Vector2f{-std::max(previousClusterSize.x, previousClusterAdvance), 0.0f});
                break;
            case sf::Text::TextDirection::TopToBottom:
                cursorPositions.emplace_back(
                    previousClusterPosition + sf::Vector2f{0.0f, std::max(previousClusterSize.y, previousClusterAdvance)});
                break;
            case sf::Text::TextDirection::BottomToTop:
                cursorPositions.emplace_back(previousClusterPosition +
                                             sf::Vector2f{0.0f, -std::max(previousClusterSize.y, previousClusterAdvance)});
                break;
            default:
                break;
        }
    }

    void updateCursorPosition(int offset)
    {
        const auto& string = text.getString();

        // The Unicode standard recommends that grapheme boundaries
        // be used as text insertion/deletion points since this is
        // what most users would expect from text editing software

        // We first increment/decrement the cursor based on user input
        cursorIndex = static_cast<std::size_t>(
            std::clamp(static_cast<int>(cursorIndex) + offset, 0, static_cast<int>(string.getSize())));

        // As long as we are not at a grapheme boundary yet, keep
        // moving the cursor position until we arrive at one
        while (cursorIndex < string.getSize() && !string.isGraphemeBoundary(cursorIndex))
            cursorIndex += static_cast<std::size_t>(offset ? (offset / std::abs(offset)) : 0);

        // If we are beyond the end of the string just return the correct
        // end-of-text position based on the direction of the text
        cursor.setPosition(
            cursorPositions[(cursorIndex >= cursorPositions.size()) ? cursorPositions.size() - 1 : cursorIndex]);
    }

    void handleMousePress(const sf::Vector2f& position)
    {
        // Filter out the event if the press was not on this text
        if (!boundingBox.getGlobalBounds().contains(position))
            return;

        const auto& transform = text.getTransform();

        // Since performance isn't really important here we will just use brute-force

        // Glyph box
        {
            // Compute the distance between the center of each glyph box
            // and the mouse position and pick the index with the lowest distance
            auto closestIndex    = std::size_t{0};
            auto closestDistance = text.getLocalBounds().size.x;

            for (auto i = 0u; i < text.getShapedGlyphs().size(); ++i)
            {
                glyphIndex = i;
                updateGlyphBox(0);

                const auto distance = (transform.transformPoint(glyphBox.getGlobalBounds().getCenter()) - position).length();

                if (distance < closestDistance)
                {
                    closestIndex    = i;
                    closestDistance = distance;
                }
            }

            glyphIndex = closestIndex;
            updateGlyphBox(0);
        }

        // Cursor
        {
            // Compute the distance between the center of each valid cursor position
            // and the mouse position and pick the index with the lowest distance
            auto closestIndex    = std::size_t{0};
            auto closestDistance = text.getLocalBounds().size.x;

            cursorIndex = 0u;
            updateCursorPosition(0);

            // Keep moving the cursor until it no longer wants to move
            while (true)
            {
                const auto distance = (transform.transformPoint(cursor.getGlobalBounds().getCenter()) - position).length();

                if (distance < closestDistance)
                {
                    closestIndex    = cursorIndex;
                    closestDistance = distance;
                }

                const auto previousCursorIndex = cursorIndex;
                updateCursorPosition(1);

                if (cursorIndex == previousCursorIndex)
                    break;
            }

            cursorIndex = closestIndex;
            updateCursorPosition(0);
        }
    }

    void generateEffectData()
    {
        static constexpr std::array<sf::Color, 6> colors =
            {sf::Color(255, 127, 127),
             sf::Color(127, 255, 127),
             sf::Color(127, 127, 255),
             sf::Color(127, 255, 255),
             sf::Color(255, 127, 255),
             sf::Color(255, 255, 127)};

        effectData.resize(text.getString().getSize());

        std::size_t nextFillColorIndex{};
        bool        isCurrentlyOutlined{};
        bool        isCurrentlyItalic{};
        bool        isCurrentlyBold{};

        // Generate effect data per unicode codepoint
        for (auto i = 0u; i < text.getString().getSize(); ++i)
        {
            // Skip whitespace
            if ((text.getString()[i] == ' ') || (text.getString()[i] == '\t') || (text.getString()[i] == '\n'))
                continue;

            if (text.getString().isWordBoundary(i))
            {
                // Color each word a different color
                ++nextFillColorIndex;
                if (nextFillColorIndex >= colors.size())
                    nextFillColorIndex = 0;

                // Outline alternating words
                isCurrentlyOutlined = !isCurrentlyOutlined;
            }

            if (text.getString().isSentenceBoundary(i))
            {
                // Italicize alternating sentences
                isCurrentlyItalic = !isCurrentlyItalic;

                // Embolden alternating sentences
                isCurrentlyBold = !isCurrentlyBold;
            }

            effectData[i].color     = colors[nextFillColorIndex];
            effectData[i].outline   = isCurrentlyOutlined;
            effectData[i].italicize = isCurrentlyItalic;
            effectData[i].embolden  = isCurrentlyBold;
        }
    }

    void preProcessGlyph(const sf::Text::ShapedGlyph& shapedGlyph,
                         std::uint32_t&               style,
                         sf::Color&                   fillColor,
                         sf::Color&                   outlineColor,
                         float&                       outlineThickness)
    {
        switch (preProcessingMode)
        {
            case PreProcessingMode::None:
                break;
            case PreProcessingMode::Color:
                if (colorDirection)
                {
                    // Color based on shaped glyph direction
                    fillColor = (shapedGlyph.textDirection == sf::Text::TextDirection::RightToLeft)
                                    ? sf::Color(127, 255, 127)
                                    : sf::Color(255, 255, 127);
                }
                else
                {
                    // Color each word a different color
                    fillColor = effectData[shapedGlyph.cluster].color;
                }
                break;
            case PreProcessingMode::Outline:
                // Outline alternating words
                if (effectData[shapedGlyph.cluster].outline)
                {
                    outlineColor     = sf::Color::Magenta;
                    outlineThickness = 1.0f;
                }
                break;
            case PreProcessingMode::Italicize:
                // Italicize alternating sentences
                if (effectData[shapedGlyph.cluster].italicize)
                    style |= sf::Text::Italic;
                break;
            case PreProcessingMode::Embolden:
                // Embolden alternating sentences
                if (effectData[shapedGlyph.cluster].embolden)
                    style |= sf::Text::Bold;
                break;
        }
    }

    void cyclePreProcessing()
    {
        switch (preProcessingMode)
        {
            case PreProcessingMode::None:
                preProcessingMode = PreProcessingMode::Color;
                break;
            case PreProcessingMode::Color:
                preProcessingMode = PreProcessingMode::Outline;
                break;
            case PreProcessingMode::Outline:
                preProcessingMode = PreProcessingMode::Italicize;
                break;
            case PreProcessingMode::Italicize:
                preProcessingMode = PreProcessingMode::Embolden;
                break;
            case PreProcessingMode::Embolden:
                preProcessingMode = PreProcessingMode::None;
                break;
        }

        // Set up glyph pre-processor
        // Even if one is already set up, calling this rebuilds the text using the new effect
        text.setGlyphPreProcessor(
            [this](const sf::Text::ShapedGlyph& shapedGlyph,
                   std::uint32_t&               style,
                   sf::Color&                   fillColor,
                   sf::Color&                   outlineColor,
                   float&                       outlineThickness)
            { preProcessGlyph(shapedGlyph, style, fillColor, outlineColor, outlineThickness); });
    }

    void updateRainbowText(float delta)
    {
        rainbowColorHue += delta;
        while (rainbowColorHue >= 360.0f)
            rainbowColorHue -= 360.0f;

        const auto&   shapedGlyphs = text.getShapedGlyphs();
        auto&         vertices     = text.getVertexData();
        const auto    hueStep      = 360.0f / static_cast<float>(text.getString().getSize());
        float         currentHue   = rainbowColorHue;
        std::uint32_t previousCluster{};

        auto getColor = [&currentHue]()
        {
            // Convert hue to RGB using the textbook method
            const auto h = currentHue / 60.f;
            const auto x = static_cast<std::uint8_t>(std::round((1.0f - std::abs(std::fmod(h, 2.0f) - 1.0f)) * 255.0f));

            if (h < 1.0f)
                return sf::Color(255, x, 0);
            if (h < 2.0f)
                return sf::Color(x, 255, 0);
            if (h < 3.0f)
                return sf::Color(0, 255, x);
            if (h < 4.0f)
                return sf::Color(0, x, 255);
            if (h < 5.0f)
                return sf::Color(x, 0, 255);
            return sf::Color(255, 0, x);
        };

        for (const auto& shapedGlyph : shapedGlyphs)
        {
            // Increment hue per cluster
            if (shapedGlyph.cluster != previousCluster)
            {
                currentHue += hueStep;
                previousCluster = shapedGlyph.cluster;
            }

            while (currentHue >= 360.0f)
                currentHue -= 360.0f;

            // Update vertex colors and y position
            for (auto index = shapedGlyph.vertexOffset; index < shapedGlyph.vertexOffset + shapedGlyph.vertexCount; ++index)
            {
                vertices[index].color      = getColor();
                vertices[index].position.y = rainbowYPositions[index] +
                                             std::sin(sf::degrees(currentHue * 2.0f).asRadians()) * 2.0f;
            }
        }
    }

    static inline auto        currentYPosition = 140.0f;
    std::unique_ptr<sf::Font> font;
    sf::Text                  text;
    sf::RectangleShape        boundingBox;
    std::size_t               glyphIndex{};
    sf::RectangleShape        glyphBox;
    std::size_t               cursorIndex{};
    std::vector<sf::Vector2f> cursorPositions;
    sf::RectangleShape        cursor;
    bool                      colorDirection{};
    float                     rainbowColorHue{};
    std::vector<float>        rainbowYPositions;

    struct EffectData
    {
        sf::Color color;
        bool      outline{};
        bool      italicize{};
        bool      embolden{};
    };

    std::vector<EffectData> effectData;

    enum class PreProcessingMode
    {
        None,
        Color,
        Outline,
        Italicize,
        Embolden
    };

    PreProcessingMode preProcessingMode = PreProcessingMode::None;
};


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    sf::RenderWindow window(sf::VideoMode({windowWidth, 800u}), "SFML Text", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    const sf::Font font("resources/tuffy.ttf");
    sf::Text       instructions(font,
                          "F1: Toggle text bounding box\nF2: Toggle glyph bounding box\nF3: Toggle cursor\nF4: Cycle "
                                "pre-processing (None, Color, Outline, Italicize, Embolden)\nLeft/Right "
                                "Arrow: Move cursor and glyph bounding box\nMouse Press: Set cursor and glyph bounding box "
                                "position",
                          14u);
    instructions.setPosition({10.0f, 10.0f});

    std::vector<DemoText> demoTexts;

    // Reserve space to make sure the objects don't get moved
    demoTexts.reserve(64);

    // clang-format off
    demoTexts.emplace_back("NotoSerif-Regular.ttf",           u8"Hello. How are you? I hope you're having a great day!");
    demoTexts.emplace_back("NotoSerifArmenian-Regular.ttf",   u8"Բարև։ Ինչպե՞ս ես։ Հուսով եմ՝ լավ օր ես անցկացնում։");
    demoTexts.emplace_back("NotoSerifGeorgian-Regular.ttf",   u8"გამარჯობა. როგორ ხარ? იმედია, კარგად გაატარე დღე!");
    demoTexts.emplace_back("NotoNaskhArabic-Regular.ttf",     u8"مرحباً، كيف حالك؟ أتمنى أن يكون يومك سعيداً!");
    demoTexts.emplace_back("NotoSerif-Regular.ttf",           u8"Hallo. Wie geht es dir? Ich hoffe, du hast einen schönen Tag!");
    demoTexts.emplace_back("NotoSerifDevanagari-Regular.ttf", u8"नमस्ते। आप कैसे हैं? आशा है आपका दिन अच्छा गुजर रहा होगा!");
    demoTexts.emplace_back("NotoSerifHebrew-Regular.ttf",     u8"שלום. מה שלומך? מקווה שיש לך יום נהדר!");
    demoTexts.emplace_back("NotoSerifJP-Regular.subset.ttf",  u8"こんにちは。お元気ですか？素敵な一日をお過ごしください！");
    demoTexts.emplace_back("NotoSerif-Regular.ttf",           u8"Γεια σας. Τι κάνετε; Ελπίζω να έχετε μια υπέροχη μέρα!");
    demoTexts.emplace_back("NotoSerifKhmer-Regular.ttf",      u8"ជំរាបសួរ។ សុខសប្បាយជាទេ? សង្ឃឹមថាអ្នកមានថ្ងៃដ៏អស្ចារ្យ!");
    demoTexts.emplace_back("NotoSerif-Regular.ttf",           u8"Hola. ¿Cómo estás? ¡Que tengas un buen día!");
    demoTexts.emplace_back("NotoSerifKR-Regular.subset.ttf",  u8"안녕하세요. 잘 지내시죠? 좋은 하루 보내시길 바랍니다!");
    demoTexts.emplace_back("NotoSerifMyanmar-Regular.ttf",    u8"မင်္ဂလာပါ။ နေကောင်းလား? ကောင်းသောနေ့ကို ပိုင်ဆိုင်နိုင်မည်ဟု မျှော်လင့်ပါသည်။");
    demoTexts.emplace_back("NotoSerifTamil-Regular.ttf",      u8"வணக்கம். எப்படி இருக்கீங்க? இந்த நாள் உங்களுக்கு ரொம்ப நல்லா அமைய வாழ்த்துக்கள்!");
    demoTexts.emplace_back("NotoSerif-Regular.ttf",           u8"Здравствуйте. Как поживаете? Надеюсь, у вас отличный день!");
    demoTexts.emplace_back("NotoSerifTC-Regular.subset.ttf",  u8"你好。你好嗎？祝你今天過得愉快！");
    demoTexts.emplace_back("NotoSerifThai-Regular.ttf",       u8"สวัสดี สบายดีไหม หวังว่าคุณคงมีวันดีๆ นะ!");
    demoTexts.emplace_back("NotoSerifTibetan-Regular.ttf",    u8"བདེ་མོ། ཁྱོད་བདེ་མོ? ཁྱེད་རང་ལ་ཉིན་མོ་ཡག་པོ་ཞིག་ཡོང་བའི་རེ་བ་ཡོད།");
    demoTexts.emplace_back("NotoSerif-Regular.ttf",           u8"Bonjour. Comment allez-vous ? J'espère que vous passez une bonne journée !");
    demoTexts.emplace_back("NotoSerifEthiopic-Regular.ttf",   u8"ሀሎ። ስላም፧ ጥሩ ቀን እንዳለህ ተስፋ አድርግ!");

    auto& ligatureText            = demoTexts.emplace_back("NotoSerif-Regular.ttf",          u8"ligatures:\nflowers\nmuffins\nwaffles\nstuff");
    auto& normalText              = demoTexts.emplace_back("NotoSerif-Regular.ttf",          u8"Normal/Bold/Italic");
    auto& boldText                = demoTexts.emplace_back("NotoSerif-Regular.ttf",          u8"Normal/Bold/Italic");
    auto& italicText              = demoTexts.emplace_back("NotoSerif-Regular.ttf",          u8"Normal/Bold/Italic");
    auto& zalgoText               = demoTexts.emplace_back("NotoSerif-Regular.ttf",          u8"Z̴̢̧͕̱͍͖̱̙̲̳͉̘̐̒͜a̴̲̖̖̖͈͉̪͎̜̽̓̀̀̒͌l̸̰͙̳̗͎͙̼͓̭̥͖̾̀̅̏͐̾͊̃͂̾ͅg̷̡̢͚̗͎̰̦͔̰̬͍̖̩̺̞͕̓̃̇ö̴̜̳̼̘̗͖͍̻͎͓̣̮̝̯͉̞́̄̉̀̄͆̈́͛̐̂̾͐̓̄");
    auto& tabText                 = demoTexts.emplace_back("NotoSerif-Regular.ttf",          u8"Tab: >\t<");
    auto& rainboxText             = demoTexts.emplace_back("NotoSerif-Regular.ttf",          u8"Rainbow Text!");
    auto& bidiText                = demoTexts.emplace_back("NotoNaskhArabic-Regular.ttf",    u8"Aligned multi-line\nنص ثنائي الاتجاه باستخدام مكتبة SFML الإصدار " SFML_VERSION_STRING ".\nالأرقام: 0123456789");
    auto& leftAligned             = demoTexts.emplace_back("NotoNaskhArabic-Regular.ttf",    u8"Left-Aligned\nText\nنص محاذي");
    auto& centered                = demoTexts.emplace_back("NotoNaskhArabic-Regular.ttf",    u8"Center-Aligned\nText\nنص محاذي");
    auto& rightAligned            = demoTexts.emplace_back("NotoNaskhArabic-Regular.ttf",    u8"Right-Aligned\nText\nنص محاذي");
    auto& horizontalStrikethrough = demoTexts.emplace_back("NotoSerif-Regular.ttf",          u8"Strikethrough");
    auto& horizontalUnderlined    = demoTexts.emplace_back("NotoSerif-Regular.ttf",          u8"Underlined");
    auto& verticalKorean          = demoTexts.emplace_back("NotoSerifKR-Regular.subset.ttf", u8"누워서 떡 먹기");
    auto& verticalJapanese        = demoTexts.emplace_back("NotoSerifJP-Regular.subset.ttf", u8"七転び八起き");
    auto& verticalChinese         = demoTexts.emplace_back("NotoSerifTC-Regular.subset.ttf", u8"一字值千金");
    auto& topToBottomText         = demoTexts.emplace_back("NotoSerif-Regular.ttf",          u8"Down Text");
    auto& bottomToTopText         = demoTexts.emplace_back("NotoSerif-Regular.ttf",          u8"Up Text");
    auto& verticalStrikethrough   = demoTexts.emplace_back("NotoSerif-Regular.ttf",          u8"Strikethrough");
    auto& verticalUnderlined      = demoTexts.emplace_back("NotoSerif-Regular.ttf",          u8"Underlined");
    // clang-format on

    ligatureText.setPosition({20.0f, 350.0f});
    normalText.setPosition({20.0f, 620.0f});
    boldText.setPosition({20.0f, 650.0f});
    boldText.setStyle(sf::Text::Bold);
    italicText.setPosition({20.0f, 680.0f});
    italicText.setStyle(sf::Text::Italic);
    zalgoText.setPosition({160.0f, 370.0f});
    tabText.setPosition({20.0f, 300.0f});

    // Rainbow text
    rainboxText.setPosition({300.0f, 10.0f});
    rainboxText.setStyle(sf::Text::Style::Bold);

    // Set coloring to indicate text direction
    bidiText.setPosition({windowWidth - bidiText.text.getGlobalBounds().size.x - 20.0f, 10.0f});
    bidiText.colorTextDirection(true);

    // Set alignment and position for aligned text
    leftAligned.setPosition({20.0f, 150.0f});
    leftAligned.setLineAlignment(sf::Text::LineAlignment::Left);

    centered.setPosition({windowWidth / 2.0f, 10.0f});
    centered.setLineAlignment(sf::Text::LineAlignment::Center);

    rightAligned.setPosition({windowWidth - 20.0f, 600.0f});
    rightAligned.setLineAlignment(sf::Text::LineAlignment::Right);

    horizontalStrikethrough.setPosition({windowWidth - 250.0f, 130.0f});
    horizontalStrikethrough.setStyle(sf::Text::StrikeThrough);

    horizontalUnderlined.setPosition({windowWidth - 250.0f, 170.0f});
    horizontalUnderlined.setStyle(sf::Text::Underlined);

    // Set orientation and position for vertical text
    // Set orientation first since we need the vertical bounds to position
    verticalKorean.setTextOrientation(sf::Text::TextOrientation::TopToBottom);
    verticalKorean.setPosition({windowWidth - verticalKorean.text.getGlobalBounds().size.x - 200.0f, 240.0f});

    verticalJapanese.setTextOrientation(sf::Text::TextOrientation::TopToBottom);
    verticalJapanese.setPosition({windowWidth - verticalJapanese.text.getGlobalBounds().size.x - 170.0f, 260.0f});

    verticalChinese.setTextOrientation(sf::Text::TextOrientation::TopToBottom);
    verticalChinese.setPosition({windowWidth - verticalChinese.text.getGlobalBounds().size.x - 140.0f, 270.0f});

    topToBottomText.setTextOrientation(sf::Text::TextOrientation::TopToBottom);
    topToBottomText.setPosition({windowWidth - topToBottomText.text.getGlobalBounds().size.x - 110.0f, 200.0f});

    bottomToTopText.setTextOrientation(sf::Text::TextOrientation::BottomToTop);
    bottomToTopText.setPosition({windowWidth - bottomToTopText.text.getGlobalBounds().size.x - 80.0f, 225.0f});

    verticalStrikethrough.setTextOrientation(sf::Text::TextOrientation::TopToBottom);
    verticalStrikethrough.setPosition({windowWidth - verticalStrikethrough.text.getGlobalBounds().size.x - 50.0f, 140.0f});
    verticalStrikethrough.setStyle(sf::Text::StrikeThrough);

    verticalUnderlined.setTextOrientation(sf::Text::TextOrientation::TopToBottom);
    verticalUnderlined.setPosition({windowWidth - verticalUnderlined.text.getGlobalBounds().size.x - 20.0f, 180.0f});
    verticalUnderlined.setStyle(sf::Text::Underlined);

    auto drawBoundingBox = false;
    auto drawGlyphBox    = false;
    auto drawCursor      = false;

    auto lastUpdate = std::chrono::steady_clock::now();

    // Main loop
    while (window.isOpen())
    {
        // Handle events
        window.handleEvents([&](const sf::Event::Closed&) { window.close(); },
                            [&](const sf::Event::KeyPressed& event)
                            {
                                switch (event.scancode)
                                {
                                    case sf::Keyboard::Scancode::Left:
                                        for (auto& demoText : demoTexts)
                                        {
                                            demoText.updateGlyphBox(-1);
                                            demoText.updateCursorPosition(-1);
                                        }
                                        break;
                                    case sf::Keyboard::Scancode::Right:
                                        for (auto& demoText : demoTexts)
                                        {
                                            demoText.updateGlyphBox(1);
                                            demoText.updateCursorPosition(1);
                                        }
                                        break;
                                    case sf::Keyboard::Scancode::Escape:
                                        window.close();
                                        break;
                                    case sf::Keyboard::Scancode::F1:
                                        drawBoundingBox = !drawBoundingBox;
                                        break;
                                    case sf::Keyboard::Scancode::F2:
                                        drawGlyphBox = !drawGlyphBox;
                                        break;
                                    case sf::Keyboard::Scancode::F3:
                                        drawCursor = !drawCursor;
                                        break;
                                    case sf::Keyboard::Scancode::F4:
                                        for (auto& demoText : demoTexts)
                                            demoText.cyclePreProcessing();
                                        break;
                                    default:
                                        break;
                                }
                            },
                            [&](const sf::Event::MouseButtonPressed& event)
                            {
                                if (event.button != sf::Mouse::Button::Left)
                                    return;

                                for (auto& demoText : demoTexts)
                                    demoText.handleMousePress(window.mapPixelToCoords(event.position));
                            });

        window.clear();

        const auto now   = std::chrono::steady_clock::now();
        const auto delta = now - lastUpdate;
        lastUpdate       = now;

        rainboxText.updateRainbowText(
            static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(delta).count()) *
            rainbowDeltaPerSecond / 1'000'000.0f);

        // Draw demo text elements based on active flags
        for (const auto& demoText : demoTexts)
        {
            if (drawBoundingBox)
                demoText.drawBoundingBox(window);

            if (drawGlyphBox)
                demoText.drawGlyphBox(window);

            demoText.drawText(window);

            if (drawCursor)
                demoText.drawCursor(window);
        }

        window.draw(instructions);
        window.display();
    }
}
