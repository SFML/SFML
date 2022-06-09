#include <SFML/Graphics/RectangleShape.hpp>
#include "SystemUtil.hpp"

#include <doctest.h>

TEST_CASE("sf::RectangleShape class - [graphics]")
{
    SUBCASE("Default constructor")
    {
        const sf::RectangleShape rectangle;
        CHECK(rectangle.getSize() == sf::Vector2f(0.f, 0.f));
        CHECK(rectangle.getPointCount() == 4);
        CHECK(rectangle.getPoint(0) == sf::Vector2f(0.f, 0.f));
        CHECK(rectangle.getPoint(1) == sf::Vector2f(0.f, 0.f));
        CHECK(rectangle.getPoint(2) == sf::Vector2f(0.f, 0.f));
        CHECK(rectangle.getPoint(3) == sf::Vector2f(0.f, 0.f));
    }

    SUBCASE("Size constructor")
    {
        const sf::RectangleShape rectangle({9.f, 8.f});
        CHECK(rectangle.getSize() == sf::Vector2f(9.f, 8.f));
        CHECK(rectangle.getPointCount() == 4);
        CHECK(rectangle.getPoint(0) == sf::Vector2f(0.f, 0.f));
        CHECK(rectangle.getPoint(1) == sf::Vector2f(9.f, 0.f));
        CHECK(rectangle.getPoint(2) == sf::Vector2f(9.f, 8.f));
        CHECK(rectangle.getPoint(3) == sf::Vector2f(0.f, 8.f));
    }

    SUBCASE("Set size")
    {
        sf::RectangleShape rectangle({7.f, 6.f});
        rectangle.setSize({5.f, 4.f});
        CHECK(rectangle.getSize() == sf::Vector2f(5.f, 4.f));
    }
}
