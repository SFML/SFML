#include <SFML/Graphics/ConvexShape.hpp>

#include <doctest/doctest.h>

#include <SystemUtil.hpp>

TEST_CASE("sf::ConvexShape class - [graphics]")
{
    SUBCASE("Default constructor")
    {
        const sf::ConvexShape convex;
        CHECK(convex.getPointCount() == 0);
    }

    SUBCASE("Point count constructor")
    {
        const sf::ConvexShape convex(15);
        CHECK(convex.getPointCount() == 15);
        for (std::size_t i = 0; i < convex.getPointCount(); ++i)
            CHECK(convex.getPoint(i) == sf::Vector2f(0, 0));
    }

    SUBCASE("Set point count")
    {
        sf::ConvexShape convex;
        convex.setPointCount(42);
        CHECK(convex.getPointCount() == 42);
        for (std::size_t i = 0; i < convex.getPointCount(); ++i)
            CHECK(convex.getPoint(i) == sf::Vector2f(0, 0));
    }

    SUBCASE("Set point")
    {
        sf::ConvexShape convex;
        convex.setPointCount(1);
        convex.setPoint(0, {3, 4});
        CHECK(convex.getPoint(0) == sf::Vector2f(3, 4));
    }
}
