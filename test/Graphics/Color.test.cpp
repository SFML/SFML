#include <SFML/Graphics/Color.hpp>

#include <catch2/catch_test_macros.hpp>

#include <GraphicsUtil.hpp>
#include <type_traits>
#include <vector>

TEST_CASE("[Graphics] sf::Color")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::Color>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Color>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Color>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Color>);
        STATIC_CHECK(std::is_trivially_copyable_v<sf::Color>);
    }

    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            constexpr sf::Color color;
            STATIC_CHECK(color.r == 0);
            STATIC_CHECK(color.g == 0);
            STATIC_CHECK(color.b == 0);
            STATIC_CHECK(color.a == 255);
        }

        SECTION("(r, g, b) constructor")
        {
            constexpr sf::Color color(1, 2, 3);
            STATIC_CHECK(color.r == 1);
            STATIC_CHECK(color.g == 2);
            STATIC_CHECK(color.b == 3);
            STATIC_CHECK(color.a == 255);
        }

        SECTION("(r, g, b, a) constructor")
        {
            constexpr sf::Color color(1, 2, 3, 4);
            STATIC_CHECK(color.r == 1);
            STATIC_CHECK(color.g == 2);
            STATIC_CHECK(color.b == 3);
            STATIC_CHECK(color.a == 4);
        }

        SECTION("std::int32_t constructor")
        {
            STATIC_CHECK(sf::Color(0x00000000) == sf::Color(0, 0, 0, 0));
            STATIC_CHECK(sf::Color(0x01020304) == sf::Color(1, 2, 3, 4));
            STATIC_CHECK(sf::Color(0xFFFFFFFF) == sf::Color(255, 255, 255, 255));
        }
    }

    SECTION("toInteger()")
    {
        STATIC_CHECK(sf::Color(0, 0, 0, 0).toInteger() == 0x00000000);
        STATIC_CHECK(sf::Color(1, 2, 3, 4).toInteger() == 0x01020304);
        STATIC_CHECK(sf::Color(255, 255, 255, 255).toInteger() == 0xFFFFFFFF);
    }

    SECTION("Operations")
    {
        SECTION("operator==")
        {
            STATIC_CHECK(sf::Color() == sf::Color());
            STATIC_CHECK(sf::Color(1, 2, 3, 4) == sf::Color(1, 2, 3, 4));

            STATIC_CHECK_FALSE(sf::Color(1, 0, 0, 0) == sf::Color(0, 0, 0, 0));
            STATIC_CHECK_FALSE(sf::Color(0, 1, 0, 0) == sf::Color(0, 0, 0, 0));
            STATIC_CHECK_FALSE(sf::Color(0, 0, 1, 0) == sf::Color(0, 0, 0, 0));
            STATIC_CHECK_FALSE(sf::Color(0, 0, 0, 1) == sf::Color(0, 0, 0, 0));
        }

        SECTION("operator!=")
        {
            STATIC_CHECK(sf::Color(1, 0, 0, 0) != sf::Color(0, 0, 0, 0));
            STATIC_CHECK(sf::Color(0, 1, 0, 0) != sf::Color(0, 0, 0, 0));
            STATIC_CHECK(sf::Color(0, 0, 1, 0) != sf::Color(0, 0, 0, 0));
            STATIC_CHECK(sf::Color(0, 0, 0, 1) != sf::Color(0, 0, 0, 0));

            STATIC_CHECK_FALSE(sf::Color() != sf::Color());
            STATIC_CHECK_FALSE(sf::Color(1, 2, 3, 4) != sf::Color(1, 2, 3, 4));
        }

        SECTION("operator+")
        {
            STATIC_CHECK(sf::Color(0, 0, 0, 0) + sf::Color(0, 0, 0, 0) == sf::Color(0, 0, 0, 0));
            STATIC_CHECK(sf::Color(50, 50, 50, 50) + sf::Color(50, 50, 50, 50) == sf::Color(100, 100, 100, 100));
            STATIC_CHECK(sf::Color(100, 100, 100, 100) + sf::Color(100, 100, 100, 100) == sf::Color(200, 200, 200, 200));
            STATIC_CHECK(sf::Color(150, 150, 150, 150) + sf::Color(150, 150, 150, 150) == sf::Color(255, 255, 255, 255));
            STATIC_CHECK(sf::Color(255, 255, 255, 255) + sf::Color(255, 255, 255, 255) == sf::Color(255, 255, 255, 255));
        }

        SECTION("operator-")
        {
            constexpr sf::Color c(50, 50, 50, 50);
            constexpr sf::Color c2(150, 150, 150, 150);
            STATIC_CHECK(c2 - c == sf::Color(100, 100, 100, 100));
            STATIC_CHECK(c - c2 == sf::Color(0, 0, 0, 0));
        }

        SECTION("operator*")
        {
            constexpr sf::Color c(255, 255, 255, 255);
            constexpr sf::Color c2(2, 2, 2, 2);
            STATIC_CHECK(c * c2 == sf::Color(2, 2, 2, 2));
            STATIC_CHECK(c2 * c == sf::Color(2, 2, 2, 2));
        }

        SECTION("operator+=")
        {
            sf::Color color(42, 42, 42, 42);
            color += sf::Color(1, 1, 1, 1);
            CHECK(color == sf::Color(43, 43, 43, 43));
            color += sf::Color(250, 250, 250, 250);
            CHECK(color == sf::Color(255, 255, 255, 255));
        }

        SECTION("operator-=")
        {
            sf::Color color(248, 248, 248, 248);
            color -= sf::Color(1, 1, 1, 1);
            CHECK(color == sf::Color(247, 247, 247, 247));
            color -= sf::Color(250, 250, 250, 250);
            CHECK(color == sf::Color(0, 0, 0, 0));
        }

        SECTION("operator*=")
        {
            sf::Color color(50, 50, 50, 50);
            color *= sf::Color(20, 20, 20, 20);
            CHECK(color == sf::Color(3, 3, 3, 3));
            color *= sf::Color(120, 120, 120, 120);
            CHECK(color == sf::Color(1, 1, 1, 1));
        }
    }

    SECTION("Constants")
    {
        STATIC_CHECK(sf::Color::Black == sf::Color(0, 0, 0));
        STATIC_CHECK(sf::Color::White == sf::Color(255, 255, 255));
        STATIC_CHECK(sf::Color::Red == sf::Color(255, 0, 0));
        STATIC_CHECK(sf::Color::Green == sf::Color(0, 255, 0));
        STATIC_CHECK(sf::Color::Blue == sf::Color(0, 0, 255));
        STATIC_CHECK(sf::Color::Yellow == sf::Color(255, 255, 0));
        STATIC_CHECK(sf::Color::Magenta == sf::Color(255, 0, 255));
        STATIC_CHECK(sf::Color::Cyan == sf::Color(0, 255, 255));
        STATIC_CHECK(sf::Color::Transparent == sf::Color(0, 0, 0, 0));
    }

    SECTION("Reinterpret as std::uint8_t*")
    {
        STATIC_CHECK(sizeof(sf::Color) == 4);
        STATIC_CHECK(alignof(sf::Color) == 1);

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
