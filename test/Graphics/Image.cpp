#include <SFML/Graphics/Image.hpp>

#include <doctest/doctest.h>

#include <GraphicsUtil.hpp>
#include <array>

TEST_CASE("sf::Image - [graphics]")
{
    SUBCASE("Default constructor")
    {
        const sf::Image image;
        CHECK(image.getSize() == sf::Vector2u());
        CHECK(image.getPixelsPtr() == nullptr);
    }

    SUBCASE("Create")
    {
        SUBCASE("create(Vector2)")
        {
            sf::Image image;
            image.create(sf::Vector2u(10, 10));
            CHECK(image.getSize() == sf::Vector2u(10, 10));
            CHECK(image.getPixelsPtr() != nullptr);

            for (std::uint32_t i = 0; i < 10; ++i)
            {
                for (std::uint32_t j = 0; j < 10; ++j)
                {
                    CHECK(image.getPixel(sf::Vector2u(i, j)) == sf::Color(0, 0, 0));
                }
            }
        }

        SUBCASE("create(Vector2, Color)")
        {
            sf::Image image;
            image.create(sf::Vector2u(10, 10), sf::Color::Red);

            CHECK(image.getSize() == sf::Vector2u(10, 10));
            CHECK(image.getPixelsPtr() != nullptr);

            for (std::uint32_t i = 0; i < 10; ++i)
            {
                for (std::uint32_t j = 0; j < 10; ++j)
                {
                    CHECK(image.getPixel(sf::Vector2u(i, j)) == sf::Color::Red);
                }
            }
        }

        SUBCASE("create(Vector2, std::uint8_t*)")
        {
            // 10 x 10, with 4 colour channels array
            std::array<std::uint8_t, 400> pixels;
            for (std::size_t i = 0; i < pixels.size(); i += 4)
            {
                pixels[i]     = 255; // r
                pixels[i + 1] = 0;   // g
                pixels[i + 2] = 0;   // b
                pixels[i + 3] = 255; // a
            }

            sf::Image image;
            image.create(sf::Vector2u(10, 10), pixels.data());

            CHECK(image.getSize() == sf::Vector2u(10, 10));
            CHECK(image.getPixelsPtr() != nullptr);

            for (std::uint32_t i = 0; i < 10; ++i)
            {
                for (std::uint32_t j = 0; j < 10; ++j)
                {
                    CHECK(image.getPixel(sf::Vector2u(i, j)) == sf::Color::Red);
                }
            }
        }
    }

    SUBCASE("Set/get pixel")
    {
        sf::Image image;

        image.create(sf::Vector2u(10, 10), sf::Color::Green);
        CHECK(image.getPixel(sf::Vector2u(2, 2)) == sf::Color::Green);

        image.setPixel(sf::Vector2u(2, 2), sf::Color::Blue);
        CHECK(image.getPixel(sf::Vector2u(2, 2)) == sf::Color::Blue);
    }

    SUBCASE("Copy from Image")
    {
        SUBCASE("Copy (Image, Vector2u)")
        {
            sf::Image image1;
            image1.create(sf::Vector2u(10, 10), sf::Color::Blue);

            sf::Image image2;
            image2.create(sf::Vector2u(10, 10));
            CHECK(image2.copy(image1, sf::Vector2u(0, 0)));

            for (std::uint32_t i = 0; i < 10; ++i)
            {
                for (std::uint32_t j = 0; j < 10; ++j)
                {
                    CHECK(image1.getPixel(sf::Vector2u(i, j)) == image2.getPixel(sf::Vector2u(i, j)));
                }
            }
        }

        SUBCASE("Copy (Image, Vector2u, IntRect)")
        {
            sf::Image image1;
            image1.create(sf::Vector2u(5, 5), sf::Color::Blue);

            sf::Image image2;
            image2.create(sf::Vector2u(10, 10));
            CHECK(image2.copy(image1, sf::Vector2u(0, 0), sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(5, 5))));

            for (std::uint32_t i = 0; i < 10; ++i)
            {
                for (std::uint32_t j = 0; j < 10; ++j)
                {
                    if (i <= 4 && j <= 4)
                        CHECK(image2.getPixel(sf::Vector2u(i, j)) == sf::Color::Blue);
                    else
                        CHECK(image2.getPixel(sf::Vector2u(i, j)) == sf::Color(0, 0, 0));
                }
            }
        }

        SUBCASE("Copy (Image, Vector2u, IntRect, bool)")
        {
            const sf::Color dest(255, 0, 0, 255);
            const sf::Color source(5, 255, 78, 232);

            // Create the composited colour for via the alpha composite over operation
            const auto a = static_cast<std::uint8_t>(source.a + (dest.a * (255 - source.a)) / 255);
            const auto r = static_cast<std::uint8_t>(
                ((source.r * source.a) + (((dest.r * dest.a) * (255 - source.a))) / 255) / a);
            const auto g = static_cast<std::uint8_t>(
                ((source.g * source.a) + (((dest.g * dest.a) * (255 - source.a))) / 255) / a);
            const auto b = static_cast<std::uint8_t>(
                ((source.b * source.a) + (((dest.b * dest.a) * (255 - source.a))) / 255) / a);
            const sf::Color composite(r, g, b, a);

            sf::Image image1;
            image1.create(sf::Vector2u(10, 10), dest);

            sf::Image image2;
            image2.create(sf::Vector2u(10, 10), source);
            CHECK(image1.copy(image2, sf::Vector2u(0, 0), sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(10, 10)), true));

            for (std::uint32_t i = 0; i < 10; ++i)
            {
                for (std::uint32_t j = 0; j < 10; ++j)
                {
                    CHECK(image1.getPixel(sf::Vector2u(i, j)) == composite);
                }
            }
        }

        SUBCASE("Copy (Empty image)")
        {
            sf::Image image1;
            sf::Image image2;

            image2.create(sf::Vector2u(10, 10), sf::Color::Red);
            CHECK(!image2.copy(image1, sf::Vector2u(0, 0), sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(9, 9))));

            for (std::uint32_t i = 0; i < 10; ++i)
            {
                for (std::uint32_t j = 0; j < 10; ++j)
                {
                    CHECK(image2.getPixel(sf::Vector2u(i, j)) == sf::Color::Red);
                }
            }
        }

        SUBCASE("Copy (Out of bounds sourceRect)")
        {
            sf::Image image1;
            image1.create(sf::Vector2u(5, 5), sf::Color::Blue);

            sf::Image image2;

            image2.create(sf::Vector2u(10, 10), sf::Color::Red);
            CHECK(!image2.copy(image1, sf::Vector2u(0, 0), sf::IntRect(sf::Vector2i(5, 5), sf::Vector2i(9, 9))));

            for (std::uint32_t i = 0; i < 10; ++i)
            {
                for (std::uint32_t j = 0; j < 10; ++j)
                {
                    CHECK(image2.getPixel(sf::Vector2u(i, j)) == sf::Color::Red);
                }
            }
        }
    }

    SUBCASE("Create mask from color")
    {
        SUBCASE("createMaskFromColor(Color)")
        {
            sf::Image image;
            image.create(sf::Vector2u(10, 10), sf::Color::Blue);
            image.createMaskFromColor(sf::Color::Blue);

            for (std::uint32_t i = 0; i < 10; ++i)
            {
                for (std::uint32_t j = 0; j < 10; ++j)
                {
                    CHECK(image.getPixel(sf::Vector2u(i, j)) == sf::Color(0, 0, 255, 0));
                }
            }
        }

        SUBCASE("createMaskFromColor(Color, std::uint8_t)")
        {
            sf::Image image;
            image.create(sf::Vector2u(10, 10), sf::Color::Blue);
            image.createMaskFromColor(sf::Color::Blue, 100);

            for (std::uint32_t i = 0; i < 10; ++i)
            {
                for (std::uint32_t j = 0; j < 10; ++j)
                {
                    CHECK(image.getPixel(sf::Vector2u(i, j)) == sf::Color(0, 0, 255, 100));
                }
            }
        }
    }

    SUBCASE("Flip horizontally")
    {
        sf::Image image;
        image.create(sf::Vector2u(10, 10), sf::Color::Red);
        image.setPixel(sf::Vector2u(0, 0), sf::Color::Green);
        image.flipHorizontally();

        CHECK(image.getPixel(sf::Vector2u(9, 0)) == sf::Color::Green);
    }

    SUBCASE("Flip vertically")
    {
        sf::Image image;
        image.create(sf::Vector2u(10, 10), sf::Color::Red);
        image.setPixel(sf::Vector2u(0, 0), sf::Color::Green);
        image.flipVertically();

        CHECK(image.getPixel(sf::Vector2u(0, 9)) == sf::Color::Green);
    }
}
