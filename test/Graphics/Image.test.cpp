#include <SFML/Graphics/Image.hpp>

// Other 1st party headers
#include <SFML/System/FileInputStream.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <GraphicsUtil.hpp>
#include <array>
#include <type_traits>

TEST_CASE("[Graphics] sf::Image")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_default_constructible_v<sf::Image>);
        STATIC_CHECK(std::is_copy_constructible_v<sf::Image>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Image>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Image>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Image>);
    }

    SECTION("Construction")
    {
        SECTION("Vector2 constructor")
        {
            const sf::Image image(sf::Vector2u(10, 10));
            CHECK(image.getSize() == sf::Vector2u(10, 10));
            CHECK(image.getPixelsPtr() != nullptr);

            for (std::uint32_t i = 0; i < 10; ++i)
            {
                for (std::uint32_t j = 0; j < 10; ++j)
                {
                    CHECK(image.getPixel(sf::Vector2u(i, j)) == sf::Color::Black);
                }
            }
        }

        SECTION("Vector2 and color constructor")
        {
            const sf::Image image(sf::Vector2u(10, 10), sf::Color::Red);
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

        SECTION("Vector2 and std::uint8_t* constructor")
        {
            // 10 x 10, with 4 color channels array
            std::array<std::uint8_t, 400> pixels{};
            for (std::size_t i = 0; i < pixels.size(); i += 4)
            {
                pixels[i]     = 255; // r
                pixels[i + 1] = 0;   // g
                pixels[i + 2] = 0;   // b
                pixels[i + 3] = 255; // a
            }

            const sf::Image image(sf::Vector2u(10, 10), pixels.data());
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

    ErrReader errReader;

    SECTION("loadFromFile()")
    {
        SECTION("Invalid path")
        {
            CHECK(!sf::Image::loadFromFile("."));
            CHECK_THAT(errReader.get(), Catch::Matchers::StartsWith("Failed to load image"));
        }

        SECTION("Nonexistent file")
        {
            CHECK(!sf::Image::loadFromFile("this/does/not/exist.jpg"));
            CHECK_THAT(errReader.get(), Catch::Matchers::StartsWith("Failed to load image"));
            CHECK_THAT(errReader.get(), Catch::Matchers::ContainsSubstring("this/does/not/exist.jpg"));
        }

        SECTION("Successful load")
        {
            std::optional<sf::Image> image;

            SECTION("bmp")
            {
                image = sf::Image::loadFromFile("Graphics/sfml-logo-big.bmp").value();
                REQUIRE(image.has_value());
                CHECK(image->getPixel({0, 0}) == sf::Color::White);
                CHECK(image->getPixel({200, 150}) == sf::Color(144, 208, 62));
            }

            SECTION("png")
            {
                image = sf::Image::loadFromFile("Graphics/sfml-logo-big.png").value();
                REQUIRE(image.has_value());
                CHECK(image->getPixel({0, 0}) == sf::Color(255, 255, 255, 0));
                CHECK(image->getPixel({200, 150}) == sf::Color(144, 208, 62));
            }

            SECTION("jpg")
            {
                image = sf::Image::loadFromFile("Graphics/sfml-logo-big.jpg").value();
                REQUIRE(image.has_value());
                CHECK(image->getPixel({0, 0}) == sf::Color::White);
                CHECK(image->getPixel({200, 150}) == sf::Color(144, 208, 62));
            }

            SECTION("gif")
            {
                image = sf::Image::loadFromFile("Graphics/sfml-logo-big.gif").value();
                REQUIRE(image.has_value());
                CHECK(image->getPixel({0, 0}) == sf::Color::White);
                CHECK(image->getPixel({200, 150}) == sf::Color(146, 210, 62));
            }

            SECTION("psd")
            {
                image = sf::Image::loadFromFile("Graphics/sfml-logo-big.psd").value();
                REQUIRE(image.has_value());
                CHECK(image->getPixel({0, 0}) == sf::Color::White);
                CHECK(image->getPixel({200, 150}) == sf::Color(144, 208, 62));
            }

            CHECK(image->getSize() == sf::Vector2u(1001, 304));
            CHECK(image->getPixelsPtr() != nullptr);
            CHECK(errReader.get().empty());
        }
    }

    SECTION("loadFromMemory()")
    {
        SECTION("Invalid pointer")
        {
            CHECK(!sf::Image::loadFromMemory(nullptr, 1));
            CHECK_THAT(errReader.get(), Catch::Matchers::StartsWith("Failed to load image from memory, no data provided"));
        }

        SECTION("Invalid size")
        {
            const std::byte testByte{0xAB};
            CHECK(!sf::Image::loadFromMemory(&testByte, 0));
            CHECK_THAT(errReader.get(), Catch::Matchers::StartsWith("Failed to load image from memory, no data provided"));
        }

        SECTION("Failed load")
        {
            std::vector<std::uint8_t> memory;

            SECTION("Empty")
            {
                CHECK(!sf::Image::loadFromMemory(memory.data(), memory.size()));
                CHECK_THAT(errReader.get(),
                           Catch::Matchers::StartsWith("Failed to load image from memory, no data provided"));
            }

            SECTION("Junk data")
            {
                memory = {1, 2, 3, 4};
                CHECK(!sf::Image::loadFromMemory(memory.data(), memory.size()));
                CHECK_THAT(errReader.get(),
                           Catch::Matchers::StartsWith("Failed to load image from memory. Reason: Image not of any "
                                                       "known type, or corrupt"));
            }
        }

        SECTION("Successful load")
        {
            const auto memory = sf::Image({24, 24}, sf::Color::Green).saveToMemory("png").value();
            const auto image  = sf::Image::loadFromMemory(memory.data(), memory.size()).value();
            CHECK(image.getSize() == sf::Vector2u(24, 24));
            CHECK(image.getPixelsPtr() != nullptr);
            CHECK(image.getPixel({0, 0}) == sf::Color::Green);
            CHECK(image.getPixel({23, 23}) == sf::Color::Green);
            CHECK(errReader.get().empty());
        }
    }

    SECTION("loadFromStream()")
    {
        auto       stream = sf::FileInputStream::open("Graphics/sfml-logo-big.png").value();
        const auto image  = sf::Image::loadFromStream(stream).value();
        CHECK(image.getSize() == sf::Vector2u(1001, 304));
        CHECK(image.getPixelsPtr() != nullptr);
        CHECK(image.getPixel({0, 0}) == sf::Color(255, 255, 255, 0));
        CHECK(image.getPixel({200, 150}) == sf::Color(144, 208, 62));
        CHECK(errReader.get().empty());
    }

    SECTION("saveToFile()")
    {
        SECTION("Invalid size")
        {
            CHECK(!sf::Image({10, 0}, sf::Color::Magenta).saveToFile("test.jpg"));
            CHECK_THAT(errReader.get(), Catch::Matchers::StartsWith("Failed to save image"));
            CHECK_THAT(errReader.get(), Catch::Matchers::ContainsSubstring("test.jpg"));

            CHECK(!sf::Image({0, 10}, sf::Color::Magenta).saveToFile("test.jpg"));
        }

        const sf::Image image({256, 256}, sf::Color::Magenta);

        SECTION("No extension")
        {
            CHECK(!image.saveToFile("wheresmyextension"));
            CHECK_THAT(errReader.get(), Catch::Matchers::StartsWith("Image file extension \"\" not supported"));
            CHECK_THAT(errReader.get(), Catch::Matchers::ContainsSubstring("wheresmyextension"));

            CHECK(!image.saveToFile("pls/add/extension"));
            CHECK_THAT(errReader.get(), Catch::Matchers::ContainsSubstring("pls/add/extension"));
        }

        SECTION("Invalid extension")
        {
            CHECK(!image.saveToFile("test.ps"));
            CHECK_THAT(errReader.get(), Catch::Matchers::StartsWith("Image file extension \".ps\" not supported"));
            CHECK_THAT(errReader.get(), Catch::Matchers::ContainsSubstring("test.ps"));

            CHECK(!image.saveToFile("test.foo"));
        }

        SECTION("Successful save")
        {
            auto filename = std::filesystem::temp_directory_path();

            SECTION("To .bmp")
            {
                filename /= "test.bmp";
                CHECK(image.saveToFile(filename));
            }

            SECTION("To .tga")
            {
                filename /= "test.tga";
                CHECK(image.saveToFile(filename));
            }

            SECTION("To .png")
            {
                filename /= "test.png";
                CHECK(image.saveToFile(filename));
            }

            // Cannot test JPEG encoding due to it triggering UB in stbiw__jpg_writeBits

            const auto loadedImage = sf::Image::loadFromFile(filename).value();
            CHECK(loadedImage.getSize() == sf::Vector2u(256, 256));
            CHECK(loadedImage.getPixelsPtr() != nullptr);
            CHECK(errReader.get().empty());

            CHECK(std::filesystem::remove(filename));
        }
    }

    SECTION("saveToMemory()")
    {
        SECTION("Invalid size")
        {
            CHECK(!sf::Image({10, 0}, sf::Color::Magenta).saveToMemory("test.jpg"));
            CHECK_THAT(errReader.get(), Catch::Matchers::StartsWith("Failed to save image with format \"test.jpg\""));
            CHECK_THAT(errReader.get(), Catch::Matchers::ContainsSubstring("test.jpg"));

            CHECK(!sf::Image({0, 10}, sf::Color::Magenta).saveToMemory("test.jpg"));
        }

        const sf::Image image({16, 16}, sf::Color::Magenta);

        SECTION("No extension")
        {
            CHECK(!image.saveToMemory(""));
            CHECK_THAT(errReader.get(), Catch::Matchers::StartsWith("Failed to save image with format \"\""));
        }

        SECTION("Invalid extension")
        {
            CHECK(!image.saveToMemory("."));
            CHECK_THAT(errReader.get(), Catch::Matchers::StartsWith("Failed to save image with format \".\""));

            CHECK(!image.saveToMemory("gif"));
            CHECK(!image.saveToMemory(".jpg")); // Supposed to be "jpg"
        }

        SECTION("Successful save")
        {
            SECTION("To bmp")
            {
                const auto output = image.saveToMemory("bmp").value();
                REQUIRE(output.size() == 1146);
                CHECK(output[0] == 66);
                CHECK(output[1] == 77);
                CHECK(output[2] == 122);
                CHECK(output[3] == 4);
                CHECK(output[1000] == 255);
                CHECK(output[1001] == 255);
                CHECK(output[1002] == 255);
                CHECK(output[1003] == 0);
            }

            SECTION("To tga")
            {
                const auto output = image.saveToMemory("tga").value();
                REQUIRE(output.size() == 98);
                CHECK(output[0] == 0);
                CHECK(output[1] == 0);
                CHECK(output[2] == 10);
                CHECK(output[3] == 0);
            }

            SECTION("To png")
            {
                const auto output = image.saveToMemory("png").value();
                REQUIRE(output.size() == 92);
                CHECK(output[0] == 137);
                CHECK(output[1] == 80);
                CHECK(output[2] == 78);
                CHECK(output[3] == 71);
            }

            CHECK(errReader.get().empty());

            // Cannot test JPEG encoding due to it triggering UB in stbiw__jpg_writeBits
        }
    }

    SECTION("Set/get pixel")
    {
        sf::Image image(sf::Vector2u(10, 10), sf::Color::Green);
        CHECK(image.getPixel(sf::Vector2u(2, 2)) == sf::Color::Green);

        image.setPixel(sf::Vector2u(2, 2), sf::Color::Blue);
        CHECK(image.getPixel(sf::Vector2u(2, 2)) == sf::Color::Blue);
    }

    SECTION("Copy from Image")
    {
        SECTION("Copy (Image, Vector2u)")
        {
            const sf::Image image1(sf::Vector2u(10, 10), sf::Color::Blue);
            sf::Image       image2(sf::Vector2u(10, 10));
            CHECK(image2.copy(image1, sf::Vector2u(0, 0)));

            for (std::uint32_t i = 0; i < 10; ++i)
            {
                for (std::uint32_t j = 0; j < 10; ++j)
                {
                    CHECK(image1.getPixel(sf::Vector2u(i, j)) == image2.getPixel(sf::Vector2u(i, j)));
                }
            }
        }

        SECTION("Copy (Image, Vector2u, IntRect)")
        {
            const sf::Image image1(sf::Vector2u(5, 5), sf::Color::Blue);
            sf::Image       image2(sf::Vector2u(10, 10));
            CHECK(image2.copy(image1, sf::Vector2u(0, 0), sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(5, 5))));

            for (std::uint32_t i = 0; i < 10; ++i)
            {
                for (std::uint32_t j = 0; j < 10; ++j)
                {
                    if (i <= 4 && j <= 4)
                        CHECK(image2.getPixel(sf::Vector2u(i, j)) == sf::Color::Blue);
                    else
                        CHECK(image2.getPixel(sf::Vector2u(i, j)) == sf::Color::Black);
                }
            }
        }

        SECTION("Copy (Image, Vector2u, IntRect, bool)")
        {
            const sf::Color dest(255, 0, 0, 255);
            const sf::Color source(5, 255, 78, 232);

            // Create the composited color for via the alpha composite over operation
            const auto a = static_cast<std::uint8_t>(source.a + (dest.a * (255 - source.a)) / 255);
            const auto r = static_cast<std::uint8_t>(
                ((source.r * source.a) + ((dest.r * dest.a) * (255 - source.a)) / 255) / a);
            const auto g = static_cast<std::uint8_t>(
                ((source.g * source.a) + ((dest.g * dest.a) * (255 - source.a)) / 255) / a);
            const auto b = static_cast<std::uint8_t>(
                ((source.b * source.a) + ((dest.b * dest.a) * (255 - source.a)) / 255) / a);
            const sf::Color composite(r, g, b, a);

            sf::Image       image1(sf::Vector2u(10, 10), dest);
            const sf::Image image2(sf::Vector2u(10, 10), source);
            CHECK(image1.copy(image2, sf::Vector2u(0, 0), sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(10, 10)), true));

            for (std::uint32_t i = 0; i < 10; ++i)
            {
                for (std::uint32_t j = 0; j < 10; ++j)
                {
                    CHECK(image1.getPixel(sf::Vector2u(i, j)) == composite);
                }
            }
        }

        SECTION("Copy (Out of bounds sourceRect)")
        {
            const sf::Image image1(sf::Vector2u(5, 5), sf::Color::Blue);
            sf::Image       image2(sf::Vector2u(10, 10), sf::Color::Red);
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

    SECTION("Create mask from color")
    {
        SECTION("createMaskFromColor(Color)")
        {
            sf::Image image(sf::Vector2u(10, 10), sf::Color::Blue);
            image.createMaskFromColor(sf::Color::Blue);

            for (std::uint32_t i = 0; i < 10; ++i)
            {
                for (std::uint32_t j = 0; j < 10; ++j)
                {
                    CHECK(image.getPixel(sf::Vector2u(i, j)) == sf::Color(0, 0, 255, 0));
                }
            }
        }

        SECTION("createMaskFromColor(Color, std::uint8_t)")
        {
            sf::Image image(sf::Vector2u(10, 10), sf::Color::Blue);
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

    SECTION("Flip horizontally")
    {
        sf::Image image(sf::Vector2u(10, 10), sf::Color::Red);
        image.setPixel(sf::Vector2u(0, 0), sf::Color::Green);
        image.flipHorizontally();

        CHECK(image.getPixel(sf::Vector2u(9, 0)) == sf::Color::Green);
    }

    SECTION("Flip vertically")
    {
        sf::Image image(sf::Vector2u(10, 10), sf::Color::Red);
        image.setPixel(sf::Vector2u(0, 0), sf::Color::Green);
        image.flipVertically();

        CHECK(image.getPixel(sf::Vector2u(0, 9)) == sf::Color::Green);
    }
}
