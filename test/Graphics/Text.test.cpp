#include <SFML/Graphics/Text.hpp>

// Other 1st party headers
#include <SFML/Graphics/Font.hpp>

#include <catch2/catch_test_macros.hpp>

#include <GraphicsUtil.hpp>
#include <WindowUtil.hpp>
#include <algorithm>
#include <array>
#include <limits>
#include <type_traits>

#include <cmath>

// Allow testing deprecated functions
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#else
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

TEST_CASE("[Graphics] sf::Text", runDisplayTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_constructible_v<sf::Text, sf::Font&&, sf::String, unsigned int>);
        STATIC_CHECK(!std::is_constructible_v<sf::Text, const sf::Font&&, sf::String, unsigned int>);
        STATIC_CHECK(std::is_copy_constructible_v<sf::Text>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Text>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Text>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Text>);
    }

    const sf::Font font("tuffy.ttf");

    SECTION("Construction")
    {
        SECTION("Font constructor")
        {
            const sf::Text text(font);
            CHECK(text.getString() == "");
            CHECK(&text.getFont() == &font);
            CHECK(text.getCharacterSize() == 30);
            CHECK(text.getLetterSpacing() == 1.f);
            CHECK(text.getLineSpacing() == 1.f);
            CHECK(text.getStyle() == sf::Text::Regular);
            CHECK(text.getFillColor() == sf::Color::White);
            CHECK(text.getOutlineColor() == sf::Color::Black);
            CHECK(text.getOutlineThickness() == 0);
            CHECK(text.getLineAlignment() == sf::Text::LineAlignment::Default);
            CHECK(text.findCharacterPos(0) == sf::Vector2f());
            CHECK(text.getShapedGlyphs().empty());
            CHECK(text.getClusterGrouping() == sf::Text::ClusterGrouping::Character);
            CHECK(text.getLocalBounds() == sf::FloatRect());
            CHECK(text.getGlobalBounds() == sf::FloatRect());
        }

        SECTION("Font and string constructor")
        {
            const sf::Text text(font, "abcdefghijklmnopqrstuvwxyz");
            CHECK(text.getString() == "abcdefghijklmnopqrstuvwxyz");
            CHECK(&text.getFont() == &font);
            CHECK(text.getCharacterSize() == 30);
            CHECK(text.getLetterSpacing() == 1.f);
            CHECK(text.getLineSpacing() == 1.f);
            CHECK(text.getStyle() == sf::Text::Regular);
            CHECK(text.getFillColor() == sf::Color::White);
            CHECK(text.getOutlineColor() == sf::Color::Black);
            CHECK(text.getOutlineThickness() == 0);
            CHECK(text.getLineAlignment() == sf::Text::LineAlignment::Default);
            CHECK(text.findCharacterPos(0) == sf::Vector2f());
            CHECK_FALSE(text.getShapedGlyphs().empty());
            CHECK(text.getClusterGrouping() == sf::Text::ClusterGrouping::Character);
            CHECK_THAT(text.getLocalBounds(), equalsApprox(sf::FloatRect({1, 8}, {358, 28}), 1.f));
            CHECK_THAT(text.getGlobalBounds(), equalsApprox(sf::FloatRect({1, 8}, {358, 28}), 1.f));
        }

        SECTION("Font, string, and character size constructor")
        {
            const sf::Text text(font, "abcdefghijklmnopqrstuvwxyz", 24);
            CHECK(text.getString() == "abcdefghijklmnopqrstuvwxyz");
            CHECK(&text.getFont() == &font);
            CHECK(text.getCharacterSize() == 24);
            CHECK(text.getLetterSpacing() == 1.f);
            CHECK(text.getLineSpacing() == 1.f);
            CHECK(text.getStyle() == sf::Text::Regular);
            CHECK(text.getFillColor() == sf::Color::White);
            CHECK(text.getOutlineColor() == sf::Color::Black);
            CHECK(text.getOutlineThickness() == 0);
            CHECK(text.getLineAlignment() == sf::Text::LineAlignment::Default);
            CHECK(text.findCharacterPos(0) == sf::Vector2f());
            CHECK_FALSE(text.getShapedGlyphs().empty());
            CHECK(text.getClusterGrouping() == sf::Text::ClusterGrouping::Character);
            CHECK_THAT(text.getLocalBounds(), equalsApprox(sf::FloatRect({1, 7}, {292, 22}), 1.f));
            CHECK_THAT(text.getGlobalBounds(), equalsApprox(sf::FloatRect({1, 7}, {292, 22}), 1.f));
        }
    }

    SECTION("Set/get string")
    {
        sf::Text text(font);
        text.setString("abcdefghijklmnopqrstuvwxyz");
        CHECK(text.getString() == "abcdefghijklmnopqrstuvwxyz");
    }

    SECTION("Set/get font")
    {
        sf::Text       text(font);
        const sf::Font otherFont("tuffy.ttf");
        text.setFont(otherFont);
        CHECK(&text.getFont() == &otherFont);
    }

    SECTION("Set/get character size")
    {
        sf::Text text(font);
        text.setCharacterSize(48);
        CHECK(text.getCharacterSize() == 48);
    }

    SECTION("Set/get line spacing")
    {
        sf::Text text(font);
        text.setLineSpacing(42);
        CHECK(text.getLineSpacing() == 42);
    }

    SECTION("Set/get letter spacing")
    {
        sf::Text text(font);
        text.setLetterSpacing(15);
        CHECK(text.getLetterSpacing() == 15);
    }

    SECTION("Set/get style")
    {
        sf::Text text(font);
        text.setStyle(sf::Text::Bold | sf::Text::Italic);
        CHECK(text.getStyle() == (sf::Text::Bold | sf::Text::Italic));
    }

    SECTION("Set/get fill color")
    {
        sf::Text text(font, "Fill color");
        text.setFillColor(sf::Color::Red);
        CHECK(text.getFillColor() == sf::Color::Red);
    }

    SECTION("Set/get outline color")
    {
        sf::Text text(font, "Outline color");
        text.setOutlineColor(sf::Color::Green);
        CHECK(text.getOutlineColor() == sf::Color::Green);
    }

    SECTION("Set/get outline thickness")
    {
        sf::Text text(font);
        text.setOutlineThickness(3.14f);
        CHECK(text.getOutlineThickness() == 3.14f);
    }

    SECTION("Set get line alignment")
    {
        sf::Text text(font, "QWERTY");
        text.setPosition({50, 25});
        CHECK(text.findCharacterPos(0) == sf::Vector2f(50, 25));
        text.setLineAlignment(sf::Text::LineAlignment::Center);
        CHECK(text.getLineAlignment() == sf::Text::LineAlignment::Center);
        CHECK_THAT(text.findCharacterPos(0), equalsApprox(sf::Vector2f(-6, 25), 1.f));
        text.setLineAlignment(sf::Text::LineAlignment::Right);
        CHECK(text.getLineAlignment() == sf::Text::LineAlignment::Right);
        CHECK_THAT(text.findCharacterPos(0), equalsApprox(sf::Vector2f(-62, 25), 1.f));
    }

    SECTION("findCharacterPos()")
    {
        sf::Text text(font, "\tabcdefghijklmnopqrstuvwxyz \n");
        text.setPosition({120, 240});
        CHECK_THAT(text.findCharacterPos(0), equalsApprox(sf::Vector2f(120, 240), 1.f));
        CHECK_THAT(text.findCharacterPos(1), equalsApprox(sf::Vector2f(156, 240), 1.f));
        CHECK_THAT(text.findCharacterPos(2), equalsApprox(sf::Vector2f(170, 240), 1.f));
        CHECK_THAT(text.findCharacterPos(3), equalsApprox(sf::Vector2f(185, 240), 1.f));
        CHECK_THAT(text.findCharacterPos(4), equalsApprox(sf::Vector2f(199, 240), 1.f));

        // Indices that are too large are capped at maximum valid index
        CHECK_THAT(text.findCharacterPos(1'000), equalsApprox(sf::Vector2f(120, 277), 1.f));
    }

    SECTION("Set/get cluster grouping")
    {
        sf::Text text(font);
        text.setClusterGrouping(sf::Text::ClusterGrouping::Grapheme);
        CHECK(text.getClusterGrouping() == sf::Text::ClusterGrouping::Grapheme);
    }

    SECTION("Get bounds")
    {
        sf::Text text(font, "Test", 18);
        text.setPosition({100, 200});
        CHECK_THAT(text.getLocalBounds(), equalsApprox(sf::FloatRect({1, 5}, {32, 13}), 1.f));
        CHECK_THAT(text.getGlobalBounds(), equalsApprox(sf::FloatRect({101, 205}, {32, 13}), 1.f));

        SECTION("Add underline")
        {
            text.setStyle(sf::Text::Underlined);
            CHECK_THAT(text.getLocalBounds(), equalsApprox(sf::FloatRect({1, 5}, {32, 13}), 1.f));
            CHECK_THAT(text.getGlobalBounds(), equalsApprox(sf::FloatRect({101, 205}, {32, 13}), 1.f));
        }

        SECTION("Add strikethrough")
        {
            text.setStyle(sf::Text::StrikeThrough);
            CHECK_THAT(text.getLocalBounds(), equalsApprox(sf::FloatRect({1, 5}, {32, 13}), 1.f));
            CHECK_THAT(text.getGlobalBounds(), equalsApprox(sf::FloatRect({101, 205}, {32, 13}), 1.f));
        }

        SECTION("Change rotation")
        {
            text.setRotation(sf::degrees(180));
            CHECK_THAT(text.getLocalBounds(), equalsApprox(sf::FloatRect({1, 5}, {32, 13}), 1.f));
            CHECK_THAT(text.getGlobalBounds(), equalsApprox((sf::FloatRect({67, 182}, {32, 13})), 1.f));
        }
    }

    SECTION("Aligned bounds follow the rendered glyphs")
    {
        sf::Text text(font, "Test", 30);
        text.setLineAlignment(sf::Text::LineAlignment::Left);
        text.setPosition({100, 200});
        text.setScale({1.5f, 0.75f});
        text.setRotation(sf::degrees(25));

        SECTION("Regular text")
        {
        }

        SECTION("Letter spacing")
        {
            text.setLetterSpacing(2);
        }

        SECTION("Outline")
        {
            text.setOutlineThickness(2.5f);
        }

        SECTION("Italic text")
        {
            text.setStyle(sf::Text::Italic);
        }

        SECTION("Trailing whitespace")
        {
            text.setString("Test \t");
        }

        const auto leftBounds = text.getLocalBounds();
        const auto leftGlyphs = text.getShapedGlyphs();
        REQUIRE_FALSE(leftGlyphs.empty());

        for (const auto alignment : {sf::Text::LineAlignment::Center, sf::Text::LineAlignment::Right})
        {
            CAPTURE(static_cast<int>(alignment));
            text.setLineAlignment(alignment);

            const auto& glyphs = text.getShapedGlyphs();
            REQUIRE(glyphs.size() == leftGlyphs.size());
            const auto displacement = glyphs.front().position - leftGlyphs.front().position;

            // A single line moves rigidly, so its bounds must follow the same displacement.
            const sf::FloatRect expectedBounds(leftBounds.position + displacement, leftBounds.size);
            CHECK_THAT(text.getLocalBounds(), equalsApprox(expectedBounds, 0.0001f));
            CHECK_THAT(text.getGlobalBounds(), equalsApprox(text.getTransform().transformRect(expectedBounds), 0.0001f));

            for (std::size_t i = 0; i < glyphs.size(); ++i)
                CHECK_THAT(glyphs[i].position, equalsApprox(leftGlyphs[i].position + displacement, 0.0001f));
        }
    }

    SECTION("Aligned multiline bounds enclose each line")
    {
        std::array<sf::String, 2> strings{"j", "Test"};
        unsigned int              style = sf::Text::Regular;
        float                     outlineThickness{};
        float                     letterSpacing = 1;

        SECTION("Different line widths and bearings")
        {
        }

        SECTION("Reverse line order")
        {
            std::swap(strings[0], strings[1]);
        }

        SECTION("Italic text")
        {
            style = sf::Text::Italic;
        }

        SECTION("Outline and letter spacing")
        {
            outlineThickness = 2.5f;
            letterSpacing    = 2;
        }

        SECTION("Whitespace")
        {
            strings = {"\tj ", "Test "};
        }

        sf::Text text(font, strings[0] + "\n" + strings[1], 30);
        text.setStyle(style);
        text.setOutlineThickness(outlineThickness);
        text.setLetterSpacing(letterSpacing);
        text.setLineAlignment(sf::Text::LineAlignment::Left);
        const auto leftBounds = text.getLocalBounds();

        for (const auto alignment :
             {sf::Text::LineAlignment::Left, sf::Text::LineAlignment::Center, sf::Text::LineAlignment::Right})
        {
            CAPTURE(static_cast<int>(alignment));
            text.setLineAlignment(alignment);
            const auto& glyphs = text.getShapedGlyphs();
            auto        minX   = std::numeric_limits<float>::max();
            auto        maxX   = std::numeric_limits<float>::lowest();
            std::size_t firstCluster{};

            for (const auto& string : strings)
            {
                // Obtain each line's bounds independently, then locate that line in the shaped text.
                sf::Text line(font, string, 30);
                line.setStyle(style);
                line.setOutlineThickness(outlineThickness);
                line.setLetterSpacing(letterSpacing);
                line.setLineAlignment(sf::Text::LineAlignment::Left);
                const auto  lineBounds = line.getLocalBounds();
                const auto& lineGlyphs = line.getShapedGlyphs();
                REQUIRE_FALSE(lineGlyphs.empty());
                const auto firstGlyph = std::find_if(glyphs.begin(),
                                                     glyphs.end(),
                                                     [&](const sf::Text::ShapedGlyph& glyph)
                                                     { return glyph.cluster == firstCluster; });
                REQUIRE(firstGlyph != glyphs.end());
                const float displacement = firstGlyph->position.x - lineGlyphs.front().position.x;
                minX                     = std::min(minX, lineBounds.position.x + displacement);
                maxX                     = std::max(maxX, lineBounds.position.x + lineBounds.size.x + displacement);
                firstCluster += string.getSize() + 1;
            }

            const sf::FloatRect expectedBounds({minX, leftBounds.position.y}, {maxX - minX, leftBounds.size.y});
            CHECK_THAT(text.getLocalBounds(), equalsApprox(expectedBounds, 0.0001f));
        }
    }

    SECTION("Aligned empty strings and empty lines")
    {
        for (const auto* string : {"", "\n", "\n\n", "\nTest", "Test\n", "\nTest\n"})
        {
            CAPTURE(string);
            sf::Text text(font, string, 30);
            text.setLineAlignment(sf::Text::LineAlignment::Left);
            const auto leftBounds = text.getLocalBounds();

            for (const auto alignment : {sf::Text::LineAlignment::Center, sf::Text::LineAlignment::Right})
            {
                CAPTURE(static_cast<int>(alignment));
                text.setLineAlignment(alignment);
                const auto bounds = text.getLocalBounds();
                CHECK(std::isfinite(bounds.position.x));
                CHECK(std::isfinite(bounds.position.y));
                CHECK(std::isfinite(bounds.size.x));
                CHECK(std::isfinite(bounds.size.y));
                CHECK(bounds.size.x >= 0);
                CHECK(bounds.size.y >= 0);
                CHECK(bounds.position.y == leftBounds.position.y);
                CHECK(bounds.size.y == leftBounds.size.y);

                if (text.getString().isEmpty())
                    CHECK(bounds == sf::FloatRect());
            }
        }
    }
}
