#include <SFML/Graphics/LineShape.hpp>

#include <doctest/doctest.h>

#include <SystemUtil.hpp>
#include <type_traits>

static_assert(std::is_copy_constructible_v<sf::LineShape>);
static_assert(std::is_copy_assignable_v<sf::LineShape>);
static_assert(std::is_move_constructible_v<sf::LineShape>);
static_assert(std::is_move_assignable_v<sf::LineShape>);

TEST_CASE("sf::LineShape class - [graphics]")
{
    SUBCASE("Construction")
    {
        const sf::LineShape line({10, 20}, {20, 20}, 2.f);
        CHECK(line.getThickness() == 2.f);
        CHECK(line.getEndPoint() == sf::Vector2f(20, 20));
        CHECK(line.getLength() == 10.f);
        CHECK(line.getPointCount() == 4);
        CHECK(line.getPoint(0) == sf::Vector2f(0, 1));
        CHECK(line.getPoint(1) == sf::Vector2f(10, 1));
        CHECK(line.getPoint(2) == sf::Vector2f(10, -1));
        CHECK(line.getPoint(3) == sf::Vector2f(0, -1));
    }

    SUBCASE("Set thickness")
    {
        sf::LineShape line({0, 0}, {1, 1}, 0);
        line.setThickness(3.14f);
        CHECK(line.getThickness() == 3.14f);
    }

    SUBCASE("Set end point")
    {
        sf::LineShape line({1, 2}, {3, 4}, 0);
        line.setEndPoint({5, 6});
        CHECK(line.getEndPoint() == sf::Vector2f(5, 6));
    }

    SUBCASE("Coincident start and end point")
    {
        const sf::LineShape line({}, {}, 0);
        CHECK(line.getPosition() == line.getEndPoint());
        CHECK(line.getLength() == 0.f);
    }
}
