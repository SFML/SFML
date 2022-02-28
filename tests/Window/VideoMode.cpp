#include <SFML/Window/VideoMode.hpp>
#include "WindowUtil.hpp"

#include <doctest.h>

TEST_CASE("sf::VideoMode class - [window]")
{
    SUBCASE("Construction")
    {
        SUBCASE("Default constructor")
        {
            sf::VideoMode videoMode;
            CHECK(videoMode.width == 0);
            CHECK(videoMode.height == 0);
            CHECK(videoMode.bitsPerPixel == 0);
        }

        SUBCASE("Width, height constructor")
        {
            sf::VideoMode videoMode(800, 600);
            CHECK(videoMode.width == 800);
            CHECK(videoMode.height == 600);
            CHECK(videoMode.bitsPerPixel == 32);
        }

        SUBCASE("Width, height, bit depth constructor")
        {
            sf::VideoMode videoMode(800, 600, 24);
            CHECK(videoMode.width == 800);
            CHECK(videoMode.height == 600);
            CHECK(videoMode.bitsPerPixel == 24);
        }
    }

    SUBCASE("Operators")
    {
        SUBCASE("operator==")
        {
            CHECK(sf::VideoMode() == sf::VideoMode());
            CHECK(sf::VideoMode(0, 0, 0) == sf::VideoMode(0, 0, 0));
            CHECK(sf::VideoMode(1080, 1920, 64) == sf::VideoMode(1080, 1920, 64));
        }

        SUBCASE("operator!=")
        {
            CHECK(sf::VideoMode() != sf::VideoMode(1, 0));
            CHECK(sf::VideoMode() != sf::VideoMode(0, 1));
            CHECK(sf::VideoMode() != sf::VideoMode(0, 0, 1));
            CHECK(sf::VideoMode(720, 720) != sf::VideoMode(720, 720, 24));
            CHECK(sf::VideoMode(1080, 1920, 16) != sf::VideoMode(400, 600));
        }

        SUBCASE("operator<")
        {
            CHECK(sf::VideoMode() < sf::VideoMode(0, 0, 1));
            CHECK(sf::VideoMode(800, 800, 24) < sf::VideoMode(1080, 1920, 48));
            CHECK(sf::VideoMode(400, 600, 48) < sf::VideoMode(600, 400, 48));
            CHECK(sf::VideoMode(400, 400, 48) < sf::VideoMode(400, 600, 48));
        }

        SUBCASE("operator>")
        {
            CHECK(sf::VideoMode(1, 0) > sf::VideoMode(0, 0, 1));
            CHECK(sf::VideoMode(800, 800, 48) > sf::VideoMode(1080, 1920, 24));
            CHECK(sf::VideoMode(600, 400, 48) > sf::VideoMode(400, 600, 48));
            CHECK(sf::VideoMode(400, 600, 48) > sf::VideoMode(400, 400, 48));
        }

        SUBCASE("operator<=")
        {
            CHECK(sf::VideoMode() <= sf::VideoMode(0, 0, 1));
            CHECK(sf::VideoMode(800, 800, 24) <= sf::VideoMode(1080, 1920, 48));
            CHECK(sf::VideoMode(400, 600, 48) <= sf::VideoMode(600, 400, 48));
            CHECK(sf::VideoMode(400, 400, 48) <= sf::VideoMode(400, 600, 48));
            CHECK(sf::VideoMode() <= sf::VideoMode());
            CHECK(sf::VideoMode(0, 0, 0) <= sf::VideoMode(0, 0, 0));
            CHECK(sf::VideoMode(1080, 1920, 64) <= sf::VideoMode(1080, 1920, 64));
        }

        SUBCASE("operator>=")
        {
            CHECK(sf::VideoMode(1, 0) >= sf::VideoMode(0, 0, 1));
            CHECK(sf::VideoMode(800, 800, 48) >= sf::VideoMode(1080, 1920, 24));
            CHECK(sf::VideoMode(600, 400, 48) >= sf::VideoMode(400, 600, 48));
            CHECK(sf::VideoMode(400, 600, 48) >= sf::VideoMode(400, 400, 48));
            CHECK(sf::VideoMode() >= sf::VideoMode());
            CHECK(sf::VideoMode(0, 0, 0) >= sf::VideoMode(0, 0, 0));
            CHECK(sf::VideoMode(1080, 1920, 64) >= sf::VideoMode(1080, 1920, 64));
        }
    }
}
