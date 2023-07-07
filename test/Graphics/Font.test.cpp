#include <SFML/Graphics/Font.hpp>

#include <catch2/catch_test_macros.hpp>

#include <GraphicsUtil.hpp>
#include <type_traits>

TEST_CASE("[Graphics] sf::Font", runDisplayTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::Font>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Font>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Font>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Font>);
    }

    SECTION("Construction")
    {
        const sf::Font font;
        CHECK(font.getInfo().family.empty());
        CHECK(!font.hasGlyph(0));
        CHECK(font.getLineSpacing(0) == 0);
        CHECK(font.getUnderlinePosition(0) == 0);
        CHECK(font.getUnderlineThickness(0) == 0);
        CHECK(font.isSmooth());
    }

    SECTION("loadFromFile()")
    {
        sf::Font font;
        REQUIRE(font.loadFromFile("Graphics/tuffy.ttf"));
        CHECK(font.getInfo().family == "Tuffy");
        const auto& glyph = font.getGlyph(0x45, 16, false);
        CHECK(glyph.advance == 9);
        CHECK(glyph.lsbDelta == 9);
        CHECK(glyph.rsbDelta == 16);
        CHECK(glyph.bounds == sf::FloatRect({0, -12}, {8, 12}));
        CHECK(glyph.textureRect == sf::IntRect({2, 5}, {8, 12}));
        CHECK(font.hasGlyph(0x41));
        CHECK(font.hasGlyph(0xC0));
        CHECK(font.getKerning(0x41, 0x42, 12) == -1);
        CHECK(font.getKerning(0x43, 0x44, 24, true) == 0);
        CHECK(font.getLineSpacing(24) == 30);
        CHECK(font.getUnderlinePosition(36) == Approx(2.20312f));
        CHECK(font.getUnderlineThickness(48) == Approx(1.17188f));
        const auto& texture = font.getTexture(10);
        CHECK(texture.getSize() == sf::Vector2u(128, 128));
        CHECK(texture.isSmooth());
        CHECK(!texture.isSrgb());
        CHECK(!texture.isRepeated());
        CHECK(texture.getNativeHandle() != 0);
        CHECK(font.isSmooth());
    }

    SECTION("Set/get smooth")
    {
        sf::Font font;
        font.setSmooth(false);
        CHECK(!font.isSmooth());
    }
}
