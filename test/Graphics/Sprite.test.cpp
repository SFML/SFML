#include <SFML/Graphics/Sprite.hpp>

// Other 1st party headers
#include <SFML/Graphics/SpriteGeometry.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <catch2/catch_test_macros.hpp>

#include <WindowUtil.hpp>
#include <type_traits>

TEST_CASE("[Graphics] sf::Sprite", runDisplayTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_constructible_v<sf::Sprite, const sf::SpriteGeometry&, const sf::Texture&>);
        STATIC_CHECK(!std::is_copy_constructible_v<sf::Sprite>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::Sprite>);
        STATIC_CHECK(!std::is_nothrow_move_constructible_v<sf::Sprite>);
        STATIC_CHECK(!std::is_nothrow_move_assignable_v<sf::Sprite>);
    }

    const auto texture = sf::Texture::create({64, 64}).value();

    SECTION("Construction")
    {
        SECTION("Geometry and texture constructor")
        {
            const sf::Sprite sprite(sf::SpriteGeometry({{0, 0}, {40, 60}}), texture);
        }
    }
}
