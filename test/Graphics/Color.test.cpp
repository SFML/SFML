#include <SFML/Graphics/Color.hpp>

#include <doctest/doctest.h>

#include <GraphicsUtil.hpp>
#include <type_traits>
#include <vector>

static_assert(std::is_copy_constructible_v<sf::Color>);
static_assert(std::is_copy_assignable_v<sf::Color>);
static_assert(std::is_nothrow_move_constructible_v<sf::Color>);
static_assert(std::is_nothrow_move_assignable_v<sf::Color>);

TEST_CASE("[Graphics] sf::Color")
{
    SUBCASE("Construction")
    {
        SUBCASE("Default constructor")
        {
            const sf::Color color;
            CHECK(color.r == 0);
            CHECK(color.g == 0);
            CHECK(color.b == 0);
            CHECK(color.a == 255);
        }

        SUBCASE("(r, g, b) constructor")
        {
            const sf::Color color(1, 2, 3);
            CHECK(color.r == 1);
            CHECK(color.g == 2);
            CHECK(color.b == 3);
            CHECK(color.a == 255);
        }

        SUBCASE("(r, g, b, a) constructor")
        {
            const sf::Color color(1, 2, 3, 4);
            CHECK(color.r == 1);
            CHECK(color.g == 2);
            CHECK(color.b == 3);
            CHECK(color.a == 4);
        }

        SUBCASE("std::int32_t constructor")
        {
            CHECK(sf::Color(0x00000000) == sf::Color(0, 0, 0, 0));
            CHECK(sf::Color(0x01020304) == sf::Color(1, 2, 3, 4));
            CHECK(sf::Color(0xFFFFFFFF) == sf::Color(255, 255, 255, 255));
        }
    }

    SUBCASE("toInteger()")
    {
        CHECK(sf::Color(0, 0, 0, 0).toInteger() == 0x00000000);
        CHECK(sf::Color(1, 2, 3, 4).toInteger() == 0x01020304);
        CHECK(sf::Color(255, 255, 255, 255).toInteger() == 0xFFFFFFFF);
    }

    SUBCASE("Operations")
    {
        SUBCASE("operator==")
        {
            CHECK(sf::Color() == sf::Color());
            CHECK(sf::Color(1, 2, 3, 4) == sf::Color(1, 2, 3, 4));

            CHECK_FALSE(sf::Color(1, 0, 0, 0) == sf::Color(0, 0, 0, 0));
            CHECK_FALSE(sf::Color(0, 1, 0, 0) == sf::Color(0, 0, 0, 0));
            CHECK_FALSE(sf::Color(0, 0, 1, 0) == sf::Color(0, 0, 0, 0));
            CHECK_FALSE(sf::Color(0, 0, 0, 1) == sf::Color(0, 0, 0, 0));
        }

        SUBCASE("operator!=")
        {
            CHECK(sf::Color(1, 0, 0, 0) != sf::Color(0, 0, 0, 0));
            CHECK(sf::Color(0, 1, 0, 0) != sf::Color(0, 0, 0, 0));
            CHECK(sf::Color(0, 0, 1, 0) != sf::Color(0, 0, 0, 0));
            CHECK(sf::Color(0, 0, 0, 1) != sf::Color(0, 0, 0, 0));

            CHECK_FALSE(sf::Color() != sf::Color());
            CHECK_FALSE(sf::Color(1, 2, 3, 4) != sf::Color(1, 2, 3, 4));
        }

        SUBCASE("operator+")
        {
            CHECK(sf::Color(0, 0, 0, 0) + sf::Color(0, 0, 0, 0) == sf::Color(0, 0, 0, 0));
            CHECK(sf::Color(50, 50, 50, 50) + sf::Color(50, 50, 50, 50) == sf::Color(100, 100, 100, 100));
            CHECK(sf::Color(100, 100, 100, 100) + sf::Color(100, 100, 100, 100) == sf::Color(200, 200, 200, 200));
            CHECK(sf::Color(150, 150, 150, 150) + sf::Color(150, 150, 150, 150) == sf::Color(255, 255, 255, 255));
            CHECK(sf::Color(255, 255, 255, 255) + sf::Color(255, 255, 255, 255) == sf::Color(255, 255, 255, 255));
        }

        SUBCASE("operator-")
        {
            const sf::Color c(50, 50, 50, 50);
            const sf::Color c2(150, 150, 150, 150);
            CHECK(c2 - c == sf::Color(100, 100, 100, 100));
            CHECK(c - c2 == sf::Color(0, 0, 0, 0));
        }

        SUBCASE("operator*")
        {
            const sf::Color c(255, 255, 255, 255);
            const sf::Color c2(2, 2, 2, 2);
            CHECK(c * c2 == sf::Color(2, 2, 2, 2));
            CHECK(c2 * c == sf::Color(2, 2, 2, 2));
        }

        SUBCASE("operator+=")
        {
            sf::Color color(42, 42, 42, 42);
            color += sf::Color(1, 1, 1, 1);
            CHECK(color == sf::Color(43, 43, 43, 43));
            color += sf::Color(250, 250, 250, 250);
            CHECK(color == sf::Color(255, 255, 255, 255));
        }

        SUBCASE("operator-=")
        {
            sf::Color color(248, 248, 248, 248);
            color -= sf::Color(1, 1, 1, 1);
            CHECK(color == sf::Color(247, 247, 247, 247));
            color -= sf::Color(250, 250, 250, 250);
            CHECK(color == sf::Color(0, 0, 0, 0));
        }

        SUBCASE("operator*=")
        {
            sf::Color color(50, 50, 50, 50);
            color *= sf::Color(20, 20, 20, 20);
            CHECK(color == sf::Color(3, 3, 3, 3));
            color *= sf::Color(120, 120, 120, 120);
            CHECK(color == sf::Color(1, 1, 1, 1));
        }
    }

    SUBCASE("Constants")
    {
        CHECK(sf::Color::Black == sf::Color(0, 0, 0));
        CHECK(sf::Color::White == sf::Color(255, 255, 255));
        CHECK(sf::Color::Red == sf::Color(255, 0, 0));
        CHECK(sf::Color::Green == sf::Color(0, 255, 0));
        CHECK(sf::Color::Blue == sf::Color(0, 0, 255));
        CHECK(sf::Color::Yellow == sf::Color(255, 255, 0));
        CHECK(sf::Color::Magenta == sf::Color(255, 0, 255));
        CHECK(sf::Color::Cyan == sf::Color(0, 255, 255));
        CHECK(sf::Color::Transparent == sf::Color(0, 0, 0, 0));
    }

    SUBCASE("Constexpr support")
    {
        constexpr sf::Color color(1, 2, 3, 4);
        static_assert(color.r == 1);
        static_assert(color.g == 2);
        static_assert(color.b == 3);
        static_assert(color.a == 4);

        static_assert(color + color == sf::Color(2, 4, 6, 8));

        static_assert(sf::Color::Black == sf::Color(0, 0, 0, 255));
    }

    SUBCASE("Reinterpret as std::uint8_t*")
    {
        static_assert(sizeof(sf::Color) == 4);
        static_assert(alignof(sf::Color) == 1);

        const std::vector<sf::Color> pixels = {{10, 11, 12, 13}, {14, 15, 16, 17}, {18, 19, 20, 21}};
        const auto*                  begin  = reinterpret_cast<const std::uint8_t*>(pixels.data());
        CHECK(begin[0] == pixels[0].r);
        CHECK(begin[1] == pixels[0].g);
        CHECK(begin[2] == pixels[0].b);
        CHECK(begin[3] == pixels[0].a);

        CHECK(begin[4] == pixels[1].r);
        CHECK(begin[5] == pixels[1].g);
        CHECK(begin[6] == pixels[1].b);
        CHECK(begin[7] == pixels[1].a);

        CHECK(begin[8] == pixels[2].r);
        CHECK(begin[9] == pixels[2].g);
        CHECK(begin[10] == pixels[2].b);
        CHECK(begin[11] == pixels[2].a);
    }
}
