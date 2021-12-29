#include <SFML/Graphics/Color.hpp>
#include "GraphicsUtil.hpp"

#include <doctest.h>

TEST_CASE("sf::Color class - [graphics]")
{
    SUBCASE("Construction")
    {
        SUBCASE("Default constructor")
        {
            sf::Color c;
            CHECK(c.r == 0);
            CHECK(c.g == 0);
            CHECK(c.b == 0);
            CHECK(c.a == 255);
        }

        SUBCASE("(r, g, b) constructor")
        {
            sf::Color c(1, 2, 3);
            CHECK(c.r == 1);
            CHECK(c.g == 2);
            CHECK(c.b == 3);
            CHECK(c.a == 255);
        }

        SUBCASE("(r, g, b, a) constructor")
        {
            sf::Color c(1, 2, 3, 4);
            CHECK(c.r == 1);
            CHECK(c.g == 2);
            CHECK(c.b == 3);
            CHECK(c.a == 4);
        }
    }

    SUBCASE("Integer conversions")
    {
        sf::Color c(1, 2, 3, 4);
        sf::Uint32 cInt = c.toInteger();

        sf::Color c2(cInt);
        CHECK(c2.r == 1);
        CHECK(c2.g == 2);
        CHECK(c2.b == 3);
        CHECK(c2.a == 4);
    }

    SUBCASE("Comparisons")
    {
        sf::Color c(1, 2, 3, 4);

        CHECK(c == c);
        CHECK(!(c != c));

        sf::Color c2(4, 3, 2, 1);

        CHECK(c2 != c);
        CHECK(!(c2 == c));
    }

    SUBCASE("Operations")
    {
        SUBCASE("Clamped addition")
        {
            sf::Color c(50, 50, 50, 50);
            CHECK(c + c == sf::Color(100, 100, 100, 100));

            sf::Color c2(150, 150, 150, 150);
            CHECK(c2 + c2 == sf::Color(255, 255, 255, 255));
        }

        SUBCASE("Clamped subtraction")
        {
            sf::Color c(50, 50, 50, 50);
            sf::Color c2(150, 150, 150, 150);

            CHECK(c2 - c == sf::Color(100, 100, 100, 100));
            CHECK(c - c2 == sf::Color(0, 0, 0, 0));
        }

        SUBCASE("Modulation")
        {
            sf::Color c(255, 255, 255, 255);
            sf::Color c2(2, 2, 2, 2);
            CHECK(c * c2 == sf::Color(2, 2, 2, 2));
            CHECK(c2 * c == sf::Color(2, 2, 2, 2));
        }
    }

    SUBCASE("Constexpr support")
    {
        constexpr sf::Color c(1, 2, 3, 4);
        static_assert(c.r == 1);
        static_assert(c.g == 2);
        static_assert(c.b == 3);
        static_assert(c.a == 4);

        static_assert(c + c == sf::Color(2, 4, 6, 8));
    }
}
