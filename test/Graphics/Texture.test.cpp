#include <SFML/Graphics/Texture.hpp>

// Other 1st party headers
#include <SFML/Graphics/Image.hpp>

#include <SFML/System/FileInputStream.hpp>

#include <catch2/catch_test_macros.hpp>

#include <GraphicsUtil.hpp>
#include <type_traits>

TEST_CASE("[Graphics] sf::Texture", runDisplayTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::Texture>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Texture>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Texture>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Texture>);
        STATIC_CHECK(std::is_nothrow_swappable_v<sf::Texture>);
    }

    SECTION("Construction")
    {
        const sf::Texture texture;
        CHECK(texture.getSize() == sf::Vector2u());
        CHECK(!texture.isSmooth());
        CHECK(!texture.isSrgb());
        CHECK(!texture.isRepeated());
        CHECK(texture.getNativeHandle() == 0);
    }

    SECTION("create()")
    {
        sf::Texture texture;

        SECTION("At least one zero dimension")
        {
            CHECK(!texture.create({}));
            CHECK(!texture.create({0, 1}));
            CHECK(!texture.create({1, 0}));
        }

        SECTION("Valid size")
        {
            CHECK(texture.create({100, 100}));
            CHECK(texture.getSize() == sf::Vector2u(100, 100));
            CHECK(texture.getNativeHandle() != 0);
        }

        SECTION("Too large")
        {
            CHECK(!texture.create({100'000, 100'000}));
            CHECK(!texture.create({1'000'000, 1'000'000}));
        }
    }

    SECTION("loadFromFile()")
    {
        sf::Texture texture;
        REQUIRE(texture.loadFromFile("Graphics/sfml-logo-big.png"));
        CHECK(texture.getSize() == sf::Vector2u(1001, 304));
        CHECK(!texture.isSmooth());
        CHECK(!texture.isSrgb());
        CHECK(!texture.isRepeated());
        CHECK(texture.getNativeHandle() != 0);
    }

    SECTION("loadFromStream()")
    {
        sf::Texture         texture;
        sf::FileInputStream stream;
        REQUIRE(stream.open("Graphics/sfml-logo-big.png"));
        REQUIRE(texture.loadFromStream(stream));
        CHECK(texture.getSize() == sf::Vector2u(1001, 304));
        CHECK(!texture.isSmooth());
        CHECK(!texture.isSrgb());
        CHECK(!texture.isRepeated());
        CHECK(texture.getNativeHandle() != 0);
    }

    SECTION("Copy semantics")
    {
        constexpr std::uint8_t red[] = {0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF};

        sf::Texture texture;
        REQUIRE(texture.create(sf::Vector2u(1, 2)));
        texture.update(red);

        SECTION("Construction")
        {
            const sf::Texture textureCopy(texture); // NOLINT(performance-unnecessary-copy-initialization)
            REQUIRE(textureCopy.getSize() == sf::Vector2u(1, 2));
            CHECK(textureCopy.copyToImage().getPixel(sf::Vector2u(0, 1)) == sf::Color::Red);
        }

        SECTION("Assignment")
        {
            sf::Texture textureCopy;
            textureCopy = texture;
            REQUIRE(textureCopy.getSize() == sf::Vector2u(1, 2));
            CHECK(textureCopy.copyToImage().getPixel(sf::Vector2u(0, 1)) == sf::Color::Red);
        }
    }

    SECTION("update()")
    {
        constexpr std::uint8_t yellow[] = {0xFF, 0xFF, 0x00, 0xFF};
        constexpr std::uint8_t cyan[]   = {0x00, 0xFF, 0xFF, 0xFF};

        sf::Texture texture;

        SECTION("Pixels")
        {
            REQUIRE(texture.create(sf::Vector2u(1, 1)));
            texture.update(yellow);
            CHECK(texture.copyToImage().getPixel(sf::Vector2u(0, 0)) == sf::Color::Yellow);
        }

        SECTION("Pixels, size and destination")
        {
            REQUIRE(texture.create(sf::Vector2u(2, 1)));
            texture.update(yellow, sf::Vector2u(1, 1), sf::Vector2u(0, 0));
            texture.update(cyan, sf::Vector2u(1, 1), sf::Vector2u(1, 0));
            CHECK(texture.copyToImage().getPixel(sf::Vector2u(0, 0)) == sf::Color::Yellow);
            CHECK(texture.copyToImage().getPixel(sf::Vector2u(1, 0)) == sf::Color::Cyan);
        }

        SECTION("Another texture")
        {
            sf::Texture otherTexture;
            REQUIRE(otherTexture.create(sf::Vector2u(1, 1)));
            otherTexture.update(cyan);
            REQUIRE(texture.create(sf::Vector2u(1, 1)));
            texture.update(otherTexture);
            CHECK(texture.copyToImage().getPixel(sf::Vector2u(0, 0)) == sf::Color::Cyan);
        }

        SECTION("Another texture and destination")
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

        SECTION("Image")
        {
            REQUIRE(texture.create(sf::Vector2u(16, 32)));
            sf::Image image;
            image.create(sf::Vector2u(16, 32), sf::Color::Red);
            texture.update(image);
            CHECK(texture.copyToImage().getPixel(sf::Vector2u(7, 15)) == sf::Color::Red);
        }

        SECTION("Image and destination")
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

    SECTION("Set/get smooth")
    {
        sf::Texture texture;
        CHECK(!texture.isSmooth());
        texture.setSmooth(true);
        CHECK(texture.isSmooth());
        texture.setSmooth(false);
        CHECK(!texture.isSmooth());
    }

    SECTION("Set/get srgb")
    {
        sf::Texture texture;
        CHECK(!texture.isSrgb());
        texture.setSrgb(true);
        CHECK(texture.isSrgb());
        texture.setSrgb(false);
        CHECK(!texture.isSrgb());
    }

    SECTION("Set/get repeated")
    {
        sf::Texture texture;
        CHECK(!texture.isRepeated());
        texture.setRepeated(true);
        CHECK(texture.isRepeated());
        texture.setRepeated(false);
        CHECK(!texture.isRepeated());
    }

    SECTION("generateMipmap()")
    {
        sf::Texture texture;
        CHECK(!texture.generateMipmap());
        CHECK(texture.create({100, 100}));
        CHECK(texture.generateMipmap());
    }

    SECTION("swap()")
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

        sf::swap(texture1, texture2);
        CHECK_FALSE(texture1.isSrgb());
        CHECK(texture1.isSmooth());
        CHECK_FALSE(texture1.isRepeated());
        CHECK(texture2.isSrgb());
        CHECK_FALSE(texture2.isSmooth());
        CHECK(texture2.isRepeated());

        const sf::Image image1 = texture1.copyToImage();
        const sf::Image image2 = texture2.copyToImage();
        REQUIRE(image1.getSize() == sf::Vector2u(2, 1));
        REQUIRE(image2.getSize() == sf::Vector2u(1, 1));
        CHECK(image1.getPixel(sf::Vector2u(1, 0)) == sf::Color::Green);
        CHECK(image2.getPixel(sf::Vector2u(0, 0)) == sf::Color::Blue);
    }

    SECTION("Get Maximum Size")
    {
        CHECK(sf::Texture::getMaximumSize() > 0);
    }
}
