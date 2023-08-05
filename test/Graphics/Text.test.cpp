#include <SFML/Graphics/Text.hpp>

// Other 1st party headers
#include <SFML/Graphics/Font.hpp>

#include <catch2/catch_test_macros.hpp>

#include <WindowUtil.hpp>
#include <type_traits>

TEST_CASE("[Graphics] sf::Text", runDisplayTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_constructible_v<sf::Text, sf::Font&&, sf::String, unsigned int>);
        STATIC_CHECK(std::is_copy_constructible_v<sf::Text>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Text>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Text>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Text>);
    }

    const sf::Font font;

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
            CHECK(text.findCharacterPos(0) == sf::Vector2f());
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
            CHECK(text.findCharacterPos(0) == sf::Vector2f());
            CHECK(text.getLocalBounds() == sf::FloatRect({0, 30}, {0, 0}));
            CHECK(text.getGlobalBounds() == sf::FloatRect({0, 30}, {0, 0}));
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
            CHECK(text.findCharacterPos(0) == sf::Vector2f());
            CHECK(text.getLocalBounds() == sf::FloatRect({0, 24}, {0, 0}));
            CHECK(text.getGlobalBounds() == sf::FloatRect({0, 24}, {0, 0}));
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
        const sf::Font otherFont;
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
        sf::Text text(font);
        text.setFillColor(sf::Color::Red);
        CHECK(text.getFillColor() == sf::Color::Red);
    }

    SECTION("Set/get outline color")
    {
        sf::Text text(font);
        text.setOutlineColor(sf::Color::Green);
        CHECK(text.getOutlineColor() == sf::Color::Green);
    }

    SECTION("Set/get outline thickness")
    {
        sf::Text text(font);
        text.setOutlineThickness(3.14f);
        CHECK(text.getOutlineThickness() == 3.14f);
    }
}
