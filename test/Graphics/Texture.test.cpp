#include <SFML/Graphics/Texture.hpp>

// Other 1st party headers
#include <SFML/Graphics/Image.hpp>

#include <doctest/doctest.h>

#include <GraphicsUtil.hpp>
#include <type_traits>

static_assert(std::is_copy_constructible_v<sf::Texture>);
static_assert(std::is_copy_assignable_v<sf::Texture>);
static_assert(std::is_nothrow_move_constructible_v<sf::Texture>);
static_assert(std::is_nothrow_move_assignable_v<sf::Texture>);
static_assert(std::is_nothrow_swappable_v<sf::Texture>);

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

    SUBCASE("create()")
    {
        sf::Texture texture;

        SUBCASE("At least one zero dimension")
        {
            CHECK(!texture.create({}));
            CHECK(!texture.create({0, 1}));
            CHECK(!texture.create({1, 0}));
        }

        SUBCASE("Valid size")
        {
            CHECK(texture.create({100, 100}));
            CHECK(texture.getSize() == sf::Vector2u(100, 100));
            CHECK(texture.getNativeHandle() != 0);
        }

        SUBCASE("Too large")
        {
            CHECK(!texture.create({100'000, 100'000}));
            CHECK(!texture.create({1'000'000, 1'000'000}));
        }
    }

    SUBCASE("Copy semantics")
    {
        constexpr std::uint8_t red[] = {0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF};

        sf::Texture texture;
        REQUIRE(texture.create(sf::Vector2u(1, 2)));
        texture.update(red);

        SUBCASE("Construction")
        {
            const sf::Texture textureCopy(texture); // NOLINT(performance-unnecessary-copy-initialization)
            REQUIRE(textureCopy.getSize() == sf::Vector2u(1, 2));
            CHECK(textureCopy.copyToImage().getPixel(sf::Vector2u(0, 1)) == sf::Color::Red);
        }

        SUBCASE("Assignment")
        {
            sf::Texture textureCopy;
            textureCopy = texture;
            REQUIRE(textureCopy.getSize() == sf::Vector2u(1, 2));
            CHECK(textureCopy.copyToImage().getPixel(sf::Vector2u(0, 1)) == sf::Color::Red);
        }
    }

    SUBCASE("update()")
    {
        constexpr std::uint8_t yellow[] = {0xFF, 0xFF, 0x00, 0xFF};
        constexpr std::uint8_t cyan[]   = {0x00, 0xFF, 0xFF, 0xFF};

        sf::Texture texture;

        SUBCASE("Pixels")
        {
            REQUIRE(texture.create(sf::Vector2u(1, 1)));
            texture.update(yellow);
            CHECK(texture.copyToImage().getPixel(sf::Vector2u(0, 0)) == sf::Color::Yellow);
        }

        SUBCASE("Pixels, size and destination")
        {
            REQUIRE(texture.create(sf::Vector2u(2, 1)));
            texture.update(yellow, sf::Vector2u(1, 1), sf::Vector2u(0, 0));
            texture.update(cyan, sf::Vector2u(1, 1), sf::Vector2u(1, 0));
            CHECK(texture.copyToImage().getPixel(sf::Vector2u(0, 0)) == sf::Color::Yellow);
            CHECK(texture.copyToImage().getPixel(sf::Vector2u(1, 0)) == sf::Color::Cyan);
        }

        SUBCASE("Another texture")
        {
            sf::Texture otherTexture;
            REQUIRE(otherTexture.create(sf::Vector2u(1, 1)));
            otherTexture.update(cyan);
            REQUIRE(texture.create(sf::Vector2u(1, 1)));
            texture.update(otherTexture);
            CHECK(texture.copyToImage().getPixel(sf::Vector2u(0, 0)) == sf::Color::Cyan);
        }

        SUBCASE("Another texture and destination")
        {
            REQUIRE(texture.create(sf::Vector2u(2, 1)));
            sf::Texture otherTexture1;
            REQUIRE(otherTexture1.create(sf::Vector2u(1, 1)));
            otherTexture1.update(cyan);
            sf::Texture otherTexture2;
            REQUIRE(otherTexture2.create(sf::Vector2u(1, 1)));
            otherTexture2.update(yellow);
            texture.update(otherTexture1, sf::Vector2u(0, 0));
            texture.update(otherTexture2, sf::Vector2u(1, 0));
            CHECK(texture.copyToImage().getPixel(sf::Vector2u(0, 0)) == sf::Color::Cyan);
            CHECK(texture.copyToImage().getPixel(sf::Vector2u(1, 0)) == sf::Color::Yellow);
        }

        SUBCASE("Image")
        {
            REQUIRE(texture.create(sf::Vector2u(16, 32)));
            sf::Image image;
            image.create(sf::Vector2u(16, 32), sf::Color::Red);
            texture.update(image);
            CHECK(texture.copyToImage().getPixel(sf::Vector2u(7, 15)) == sf::Color::Red);
        }

        SUBCASE("Image and destination")
        {
            REQUIRE(texture.create(sf::Vector2u(16, 32)));
            sf::Image image1;
            image1.create(sf::Vector2u(16, 16), sf::Color::Red);
            texture.update(image1);
            sf::Image image2;
            image2.create(sf::Vector2u(16, 16), sf::Color::Green);
            texture.update(image1, sf::Vector2u(0, 0));
            texture.update(image2, sf::Vector2u(0, 16));
            CHECK(texture.copyToImage().getPixel(sf::Vector2u(7, 7)) == sf::Color::Red);
            CHECK(texture.copyToImage().getPixel(sf::Vector2u(7, 22)) == sf::Color::Green);
        }
    }

    SUBCASE("Set/get smooth")
    {
        sf::Texture texture;
        CHECK(!texture.isSmooth());
        texture.setSmooth(true);
        CHECK(texture.isSmooth());
        texture.setSmooth(false);
        CHECK(!texture.isSmooth());
    }

    SUBCASE("Set/get srgb")
    {
        sf::Texture texture;
        CHECK(!texture.isSrgb());
        texture.setSrgb(true);
        CHECK(texture.isSrgb());
        texture.setSrgb(false);
        CHECK(!texture.isSrgb());
    }

    SUBCASE("Set/get repeated")
    {
        sf::Texture texture;
        CHECK(!texture.isRepeated());
        texture.setRepeated(true);
        CHECK(texture.isRepeated());
        texture.setRepeated(false);
        CHECK(!texture.isRepeated());
    }

    SUBCASE("swap()")
    {
        constexpr std::uint8_t blue[]  = {0x00, 0x00, 0xFF, 0xFF};
        constexpr std::uint8_t green[] = {0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF};

        sf::Texture texture1;
        REQUIRE(texture1.create(sf::Vector2u(1, 1)));
        texture1.update(blue);
        texture1.setSrgb(true);
        texture1.setSmooth(false);
        texture1.setRepeated(true);

        sf::Texture texture2;
        REQUIRE(texture2.create(sf::Vector2u(2, 1)));
        texture2.update(green);
        texture2.setSrgb(false);
        texture2.setSmooth(true);
        texture2.setRepeated(false);

        std::swap(texture1, texture2);
        CHECK_FALSE(texture1.isSrgb());
        CHECK(texture1.isSmooth());
        CHECK_FALSE(texture1.isRepeated());
        CHECK(texture2.isSrgb());
        CHECK_FALSE(texture2.isSmooth());
        CHECK(texture2.isRepeated());

        sf::Image image1 = texture1.copyToImage();
        sf::Image image2 = texture2.copyToImage();
        REQUIRE(image1.getSize() == sf::Vector2u(2, 1));
        REQUIRE(image2.getSize() == sf::Vector2u(1, 1));
        CHECK(image1.getPixel(sf::Vector2u(1, 0)) == sf::Color::Green);
        CHECK(image2.getPixel(sf::Vector2u(0, 0)) == sf::Color::Blue);
    }

    SUBCASE("Get Maximum Size")
    {
        CHECK(sf::Texture::getMaximumSize() > 0);
    }
}
