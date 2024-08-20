#include <SFML/Graphics/Font.hpp>

// Other 1st party headers
#include <SFML/Graphics/Texture.hpp>

#include <SFML/System/Exception.hpp>
#include <SFML/System/FileInputStream.hpp>

#include <catch2/catch_test_macros.hpp>

#include <GraphicsUtil.hpp>
#include <WindowUtil.hpp>
#include <fstream>
#include <type_traits>

TEST_CASE("[Graphics] sf::Font", runDisplayTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::Font>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Font>);
        STATIC_CHECK(std::is_move_constructible_v<sf::Font>);
        STATIC_CHECK(std::is_move_assignable_v<sf::Font>);
    }

    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            const sf::Font font;
            CHECK(font.getInfo().family.empty());
            CHECK(!font.hasGlyph(0));
            CHECK(font.getLineSpacing(0) == 0);
            CHECK(font.getUnderlinePosition(0) == 0);
            CHECK(font.getUnderlineThickness(0) == 0);
            CHECK(font.isSmooth());
        }

        SECTION("File")
        {
            SECTION("Invalid filename")
            {
                CHECK_THROWS_AS(sf::Font("does/not/exist.ttf"), sf::Exception);
            }

            SECTION("Successful load")
            {
                const sf::Font font("Graphics/tuffy.ttf");
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
        }

        SECTION("Memory")
        {
            SECTION("Invalid data and size")
            {
                CHECK_THROWS_AS(sf::Font(nullptr, 1), sf::Exception);
                const std::byte testByte{0xCD};
                CHECK_THROWS_AS(sf::Font(&testByte, 0), sf::Exception);
            }

            SECTION("Successful load")
            {
                const auto     memory = loadIntoMemory("Graphics/tuffy.ttf");
                const sf::Font font(memory.data(), memory.size());
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
        }

        SECTION("Stream")
        {
            sf::FileInputStream stream("Graphics/tuffy.ttf");
            const sf::Font      font(stream);
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
    }

    SECTION("openFromFile()")
    {
        sf::Font font;

        SECTION("Invalid filename")
        {
            CHECK(!font.openFromFile("does/not/exist.ttf"));
        }

        SECTION("Successful load")
        {
            REQUIRE(font.openFromFile("Graphics/tuffy.ttf"));
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
    }

    SECTION("openFromMemory()")
    {
        sf::Font font;

        SECTION("Invalid data and size")
        {
            CHECK(!font.openFromMemory(nullptr, 1));
            const std::byte testByte{0xCD};
            CHECK(!font.openFromMemory(&testByte, 0));
        }

        SECTION("Successful load")
        {
            const auto memory = loadIntoMemory("Graphics/tuffy.ttf");
            REQUIRE(font.openFromMemory(memory.data(), memory.size()));
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
    }

    SECTION("openFromStream()")
    {
        sf::Font            font;
        sf::FileInputStream stream;

        SECTION("Invalid stream")
        {
            CHECK(!font.openFromStream(stream));
        }

        SECTION("Successful load")
        {
            REQUIRE(stream.open("Graphics/tuffy.ttf"));
            REQUIRE(font.openFromStream(stream));
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
    }

    SECTION("Set/get smooth")
    {
        sf::Font font("Graphics/tuffy.ttf");
        font.setSmooth(false);
        CHECK(!font.isSmooth());
    }
}
