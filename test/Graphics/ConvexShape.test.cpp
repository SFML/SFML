#include <SFML/Graphics/ConvexShape.hpp>

#include <catch2/catch_test_macros.hpp>

#include <SystemUtil.hpp>
#include <type_traits>

TEST_CASE("[Graphics] sf::ConvexShape")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::ConvexShape>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::ConvexShape>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::ConvexShape>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::ConvexShape>);
    }

    SECTION("Default constructor")
    {
        const sf::ConvexShape convex;
        CHECK(convex.getPointCount() == 0);
    }

    SECTION("Point count constructor")
    {
        const sf::ConvexShape convex(15);
        CHECK(convex.getPointCount() == 15);
        for (std::size_t i = 0; i < convex.getPointCount(); ++i)
            CHECK(convex.getPoint(i) == sf::Vector2f(0, 0));
    }

    SECTION("Set point count")
    {
        sf::ConvexShape convex;
        convex.setPointCount(42);
        CHECK(convex.getPointCount() == 42);
        for (std::size_t i = 0; i < convex.getPointCount(); ++i)
            CHECK(convex.getPoint(i) == sf::Vector2f(0, 0));
    }

    SECTION("Set point")
    {
        sf::ConvexShape convex;
        convex.setPointCount(1);
        convex.setPoint(0, {3, 4});
        CHECK(convex.getPoint(0) == sf::Vector2f(3, 4));
    }
}
