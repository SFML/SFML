#include <SFML/Graphics/RectangleShape.hpp>

#include <catch2/catch_test_macros.hpp>

#include <SystemUtil.hpp>
#include <type_traits>

TEST_CASE("[Graphics] sf::RectangleShape")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::RectangleShape>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::RectangleShape>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::RectangleShape>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::RectangleShape>);
    }

    SECTION("Default constructor")
    {
        const sf::RectangleShape rectangle;
        CHECK(rectangle.getSize() == sf::Vector2f(0, 0));
        CHECK(rectangle.getPointCount() == 4);
        CHECK(rectangle.getPoint(0) == sf::Vector2f(0, 0));
        CHECK(rectangle.getPoint(1) == sf::Vector2f(0, 0));
        CHECK(rectangle.getPoint(2) == sf::Vector2f(0, 0));
        CHECK(rectangle.getPoint(3) == sf::Vector2f(0, 0));
        CHECK(rectangle.getGeometricCenter() == sf::Vector2f(0, 0));
    }

    SECTION("Size constructor")
    {
        const sf::RectangleShape rectangle({9, 8});
        CHECK(rectangle.getSize() == sf::Vector2f(9, 8));
        CHECK(rectangle.getPointCount() == 4);
        CHECK(rectangle.getPoint(0) == sf::Vector2f(0, 0));
        CHECK(rectangle.getPoint(1) == sf::Vector2f(9, 0));
        CHECK(rectangle.getPoint(2) == sf::Vector2f(9, 8));
        CHECK(rectangle.getPoint(3) == sf::Vector2f(0, 8));
        CHECK(rectangle.getGeometricCenter() == sf::Vector2f(9.f, 8.f) / 2.f);
    }

    SECTION("Set size")
    {
        sf::RectangleShape rectangle({7, 6});
        rectangle.setSize({5, 4});
        CHECK(rectangle.getSize() == sf::Vector2f(5, 4));
        CHECK(rectangle.getGeometricCenter() == sf::Vector2f(5.f, 4.f) / 2.f);
    }
}
