#include <SFML/Graphics/Sprite.hpp>

// Other 1st party headers
#include <SFML/Graphics/Texture.hpp>

#include <catch2/catch_test_macros.hpp>

#include <WindowUtil.hpp>
#include <type_traits>

TEST_CASE("[Graphics] sf::Sprite", runDisplayTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::Sprite>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Sprite>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Sprite>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Sprite>);
    }

    const auto texture = sf::Texture::create({64, 64}).value();

    SECTION("Construction")
    {
        SECTION("Rectangle constructor")
        {
            const sf::Sprite sprite({{0, 0}, {40, 60}});
            CHECK(sprite.getTextureRect() == sf::IntRect({0, 0}, {40, 60}));
            CHECK(sprite.getColor() == sf::Color::White);
            CHECK(sprite.getLocalBounds() == sf::FloatRect({0, 0}, {40, 60}));
            CHECK(sprite.getGlobalBounds() == sf::FloatRect({0, 0}, {40, 60}));
        }
    }

    SECTION("Set/get texture rect")
    {
        sf::Sprite sprite({});
        sprite.setTextureRect({{1, 2}, {3, 4}});
        CHECK(sprite.getTextureRect() == sf::IntRect({1, 2}, {3, 4}));
    }

    SECTION("Set/get color")
    {
        sf::Sprite sprite({});
        sprite.setColor(sf::Color::Red);
        CHECK(sprite.getColor() == sf::Color::Red);
    }
}
