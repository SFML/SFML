#include <SFML/Graphics/Text.hpp>

// Other 1st party headers
#include <SFML/Graphics/Font.hpp>

#include <catch2/catch_test_macros.hpp>

#include <GraphicsUtil.hpp>
#include <WindowUtil.hpp>
#include <type_traits>

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

        SECTION("Change alignment")
        {
            text.setLineAlignment(sf::Text::LineAlignment::Center);
            CHECK_THAT(text.getLocalBounds(), equalsApprox(sf::FloatRect({-15, 5}, {32, 13}), 1.f));
            CHECK_THAT(text.getGlobalBounds(), equalsApprox(sf::FloatRect({85, 205}, {32, 13}), 1.f));
            text.setLineAlignment(sf::Text::LineAlignment::Right);
            CHECK_THAT(text.getLocalBounds(), equalsApprox(sf::FloatRect({-31, 5}, {32, 13}), 1.f));
            CHECK_THAT(text.getGlobalBounds(), equalsApprox(sf::FloatRect({69, 205}, {32, 13}), 1.f));
        }
    }
}
