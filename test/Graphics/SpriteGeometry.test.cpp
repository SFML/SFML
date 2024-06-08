#include <SFML/Graphics/SpriteGeometry.hpp>

#include <catch2/catch_test_macros.hpp>

#include <WindowUtil.hpp>
#include <type_traits>

TEST_CASE("[Graphics] sf::SpriteGeometry", runDisplayTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_constructible_v<sf::SpriteGeometry, const sf::IntRect&>);
        STATIC_CHECK(std::is_copy_constructible_v<sf::SpriteGeometry>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::SpriteGeometry>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::SpriteGeometry>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::SpriteGeometry>);
    }

    SECTION("Construction")
    {
        SECTION("Rectangle constructor")
        {
            const sf::SpriteGeometry geometry({{0, 0}, {40, 60}});
            CHECK(geometry.getTextureRect() == sf::IntRect({0, 0}, {40, 60}));
            CHECK(geometry.getColor() == sf::Color::White);
            CHECK(geometry.getLocalBounds() == sf::FloatRect({0, 0}, {40, 60}));
            CHECK(geometry.getGlobalBounds() == sf::FloatRect({0, 0}, {40, 60}));
        }
    }

    SECTION("Set/get texture rect")
    {
        sf::SpriteGeometry geometry({});
        geometry.setTextureRect({{1, 2}, {3, 4}});
        CHECK(geometry.getTextureRect() == sf::IntRect({1, 2}, {3, 4}));
    }

    SECTION("Set/get color")
    {
        sf::SpriteGeometry geometry({});
        geometry.setColor(sf::Color::Red);
        CHECK(geometry.getColor() == sf::Color::Red);
    }
}
