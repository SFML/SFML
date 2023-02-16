#include <SFML/Window/VideoMode.hpp>

#include <doctest/doctest.h>

#include <WindowUtil.hpp>
#include <type_traits>

static_assert(std::is_copy_constructible_v<sf::VideoMode>);
static_assert(std::is_copy_assignable_v<sf::VideoMode>);
static_assert(std::is_nothrow_move_constructible_v<sf::VideoMode>);
static_assert(std::is_nothrow_move_assignable_v<sf::VideoMode>);

TEST_CASE("[Window] sf::VideoMode")
{
    SUBCASE("Construction")
    {
        SUBCASE("Default constructor")
        {
            sf::VideoMode videoMode;
            CHECK(videoMode.size == sf::Vector2u(0, 0));
            CHECK(videoMode.bitsPerPixel == 0);
        }

        SUBCASE("Width, height constructor")
        {
            sf::VideoMode videoMode({800, 600});
            CHECK(videoMode.size == sf::Vector2u(800, 600));
            CHECK(videoMode.bitsPerPixel == 32);
        }

        SUBCASE("Width, height, bit depth constructor")
        {
            sf::VideoMode videoMode({800, 600}, 24);
            CHECK(videoMode.size == sf::Vector2u(800, 600));
            CHECK(videoMode.bitsPerPixel == 24);
        }

        SUBCASE("Aspect ratios")
        {
            SUBCASE("Edge cases")
            {
                CHECK(sf::VideoMode().getAspectRatio() == sf::Vector2u(0, 0));
                CHECK(sf::VideoMode({0, 1}).getAspectRatio() == sf::Vector2u(0, 1));
                CHECK(sf::VideoMode({1, 0}).getAspectRatio() == sf::Vector2u(1, 0));
            }

            SUBCASE("4:3")
            {
                sf::Vector2u aspectRatio(4, 3);
                CHECK(sf::VideoMode({640, 480}).getAspectRatio() == aspectRatio);
                CHECK(sf::VideoMode({768, 576}).getAspectRatio() == aspectRatio);
                CHECK(sf::VideoMode({800, 600}).getAspectRatio() == aspectRatio);
                CHECK(sf::VideoMode({1024, 768}).getAspectRatio() == aspectRatio);
                CHECK(sf::VideoMode({1152, 864}).getAspectRatio() == aspectRatio);
                CHECK(sf::VideoMode({1400, 1050}).getAspectRatio() == aspectRatio);
                CHECK(sf::VideoMode({1600, 1200}).getAspectRatio() == aspectRatio);
                CHECK(sf::VideoMode({2048, 1536}).getAspectRatio() == aspectRatio);
            }

            SUBCASE("16:9")
            {
                sf::Vector2u aspectRatio(16, 9);
                CHECK(sf::VideoMode({1024, 576}).getAspectRatio() == aspectRatio);
                CHECK(sf::VideoMode({1280, 720}).getAspectRatio() == aspectRatio);
                CHECK(sf::VideoMode({1366, 768}).getAspectRatio() == aspectRatio);
                CHECK(sf::VideoMode({1600, 900}).getAspectRatio() == aspectRatio);
                CHECK(sf::VideoMode({1920, 1080}).getAspectRatio() == aspectRatio);
                CHECK(sf::VideoMode({2560, 1440}).getAspectRatio() == aspectRatio);
            }
        }
    }

    SUBCASE("Operators")
    {
        SUBCASE("operator==")
        {
            CHECK(sf::VideoMode() == sf::VideoMode());
            CHECK(sf::VideoMode({0, 0}, 0) == sf::VideoMode({0, 0}, 0));
            CHECK(sf::VideoMode({1080, 1920}, 64) == sf::VideoMode({1080, 1920}, 64));
        }

        SUBCASE("operator!=")
        {
            CHECK(sf::VideoMode() != sf::VideoMode({1, 0}));
            CHECK(sf::VideoMode() != sf::VideoMode({0, 1}));
            CHECK(sf::VideoMode() != sf::VideoMode({0, 0}, 1));
            CHECK(sf::VideoMode({720, 720}) != sf::VideoMode({720, 720}, 24));
            CHECK(sf::VideoMode({1080, 1920}, 16) != sf::VideoMode({400, 600}));
        }

        SUBCASE("operator<")
        {
            CHECK(sf::VideoMode() < sf::VideoMode({0, 0}, 1));
            CHECK(sf::VideoMode({800, 800}, 24) < sf::VideoMode({1080, 1920}, 48));
            CHECK(sf::VideoMode({400, 600}, 48) < sf::VideoMode({600, 400}, 48));
            CHECK(sf::VideoMode({400, 400}, 48) < sf::VideoMode({400, 600}, 48));
        }

        SUBCASE("operator>")
        {
            CHECK(sf::VideoMode({1, 0}) > sf::VideoMode({0, 0}, 1));
            CHECK(sf::VideoMode({800, 800}, 48) > sf::VideoMode({1080, 1920}, 24));
            CHECK(sf::VideoMode({600, 400}, 48) > sf::VideoMode({400, 600}, 48));
            CHECK(sf::VideoMode({400, 600}, 48) > sf::VideoMode({400, 400}, 48));
        }

        SUBCASE("operator<=")
        {
            CHECK(sf::VideoMode() <= sf::VideoMode({0, 0}, 1));
            CHECK(sf::VideoMode({800, 800}, 24) <= sf::VideoMode({1080, 1920}, 48));
            CHECK(sf::VideoMode({400, 600}, 48) <= sf::VideoMode({600, 400}, 48));
            CHECK(sf::VideoMode({400, 400}, 48) <= sf::VideoMode({400, 600}, 48));
            CHECK(sf::VideoMode() <= sf::VideoMode());
            CHECK(sf::VideoMode({0, 0}, 0) <= sf::VideoMode({0, 0}, 0));
            CHECK(sf::VideoMode({1080, 1920}, 64) <= sf::VideoMode({1080, 1920}, 64));
        }

        SUBCASE("operator>=")
        {
            CHECK(sf::VideoMode({1, 0}) >= sf::VideoMode({0, 0}, 1));
            CHECK(sf::VideoMode({800, 800}, 48) >= sf::VideoMode({1080, 1920}, 24));
            CHECK(sf::VideoMode({600, 400}, 48) >= sf::VideoMode({400, 600}, 48));
            CHECK(sf::VideoMode({400, 600}, 48) >= sf::VideoMode({400, 400}, 48));
            CHECK(sf::VideoMode() >= sf::VideoMode());
            CHECK(sf::VideoMode({0, 0}, 0) >= sf::VideoMode({0, 0}, 0));
            CHECK(sf::VideoMode({1080, 1920}, 64) >= sf::VideoMode({1080, 1920}, 64));
        }
    }
}
