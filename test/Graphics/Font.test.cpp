#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

// Other 1st party headers
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
        STATIC_CHECK(!std::is_default_constructible_v<sf::Font>);
        STATIC_CHECK(std::is_copy_constructible_v<sf::Font>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Font>);
        STATIC_CHECK(std::is_move_constructible_v<sf::Font>);
        STATIC_CHECK(std::is_move_assignable_v<sf::Font>);
    }

    SECTION("Constructor")
    {
        SECTION("File")
        {
            SECTION("Invalid filename")
            {
                CHECK_THROWS_AS(sf::Font("does/not/exist.ttf"), std::runtime_error);
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
                CHECK_THROWS_AS(sf::Font(nullptr, 1), std::runtime_error);
                const std::byte testByte{0xCD};
                CHECK_THROWS_AS(sf::Font(&testByte, 0), std::runtime_error);
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
        SECTION("Invalid filename")
        {
            CHECK(!sf::Font::openFromFile("does/not/exist.ttf"));
        }

        SECTION("Valid file")
        {
            const auto font = sf::Font::openFromFile("Graphics/tuffy.ttf").value();
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
        SECTION("Invalid data and size")
        {
            CHECK(!sf::Font::openFromMemory(nullptr, 1));
            const std::byte testByte{0xCD};
            CHECK(!sf::Font::openFromMemory(&testByte, 0));
        }

        SECTION("Valid data")
        {
            const auto memory = loadIntoMemory("Graphics/tuffy.ttf");
            const auto font   = sf::Font::openFromMemory(memory.data(), memory.size()).value();
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
        auto       stream = sf::FileInputStream::open("Graphics/tuffy.ttf").value();
        const auto font   = sf::Font::openFromStream(stream).value();
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
        auto font = sf::Font::openFromFile("Graphics/tuffy.ttf").value();
        font.setSmooth(false);
        CHECK(!font.isSmooth());
    }
}
