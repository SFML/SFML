#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

#include <doctest/doctest.h>

#include <SystemUtil.hpp>
#include <type_traits>

static_assert(std::is_copy_constructible_v<sf::ConvexShape>);
static_assert(std::is_copy_assignable_v<sf::ConvexShape>);
static_assert(std::is_nothrow_move_constructible_v<sf::ConvexShape>);
static_assert(std::is_nothrow_move_assignable_v<sf::ConvexShape>);

TEST_CASE("[Graphics] sf::ConvexShape")
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

    SUBCASE("Reasonable geometric center")
    {
        sf::ConvexShape convex;
        for (unsigned i = 2; i < 10; ++i)
        {
            sf::CircleShape circle(4.f, i);
            convex.setPointCount(i);
            for (unsigned j = 0; j < i; ++j)
            {
                convex.setPoint(j, circle.getPoint(j));
            }
            CHECK(convex.getGeometricCenter() == Approx(circle.getGeometricCenter()));
        }
    }

    SUBCASE("Edge case geometric centers")
    {
        sf::ConvexShape convex2(1);
        convex2.setPoint(0, {1.f, 1.f});
        CHECK(convex2.getGeometricCenter() == sf::Vector2f(1.f, 1.f));

        sf::ConvexShape convex3(2);
        convex3.setPoint(0, {0.f, 0.f});
        convex3.setPoint(1, {4.f, 2.f});
        CHECK(convex3.getGeometricCenter() == sf::Vector2f(2.f, 1.f));
    }
}
