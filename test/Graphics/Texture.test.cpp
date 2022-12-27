#include <SFML/Graphics/Texture.hpp>

#include <doctest/doctest.h>

#include <GraphicsUtil.hpp>
#include <type_traits>

static_assert(std::is_copy_constructible_v<sf::Texture>);
static_assert(std::is_copy_assignable_v<sf::Texture>);
static_assert(std::is_move_constructible_v<sf::Texture>);
static_assert(!std::is_nothrow_move_constructible_v<sf::Texture>);
static_assert(std::is_move_assignable_v<sf::Texture>);
static_assert(!std::is_nothrow_move_assignable_v<sf::Texture>);

TEST_CASE("[Graphics] sf::Texture" * doctest::skip(skipDisplayTests))
{
    SUBCASE("Construction")
    {
        sf::Texture texture;
        CHECK(texture.getSize() == sf::Vector2u());
        CHECK(!texture.isSmooth());
        CHECK(!texture.isSrgb());
        CHECK(!texture.isRepeated());
        CHECK(texture.getNativeHandle() == 0);
    }
}
