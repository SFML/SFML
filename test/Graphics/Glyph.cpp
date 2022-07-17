#include <SFML/Graphics/Glyph.hpp>

#include <doctest/doctest.h>

#include <GraphicsUtil.hpp>

TEST_CASE("sf::Glyph class - [graphics]")
{
    SUBCASE("Construction")
    {
        const sf::Glyph glyph;
        CHECK(glyph.advance == 0.f);
        CHECK(glyph.lsbDelta == 0);
        CHECK(glyph.rsbDelta == 0);
        CHECK(glyph.bounds == sf::FloatRect());
        CHECK(glyph.textureRect == sf::IntRect());
    }
}
