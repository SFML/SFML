#include <SFML/Graphics/Glyph.hpp>

#include <catch2/catch_test_macros.hpp>

#include <GraphicsUtil.hpp>
#include <type_traits>

TEST_CASE("[Graphics] sf::Glyph")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::Glyph>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Glyph>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Glyph>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Glyph>);
    }

    SECTION("Construction")
    {
        constexpr sf::Glyph glyph;
        STATIC_CHECK(glyph.advance == 0.f);
        STATIC_CHECK(glyph.lsbDelta == 0);
        STATIC_CHECK(glyph.rsbDelta == 0);
        STATIC_CHECK(glyph.bounds == sf::FloatRect());
        STATIC_CHECK(glyph.textureRect == sf::IntRect());
    }
}
