#include <SFML/Graphics/ConvexShape.hpp>

// Other 1st party headers
#include <SFML/Graphics/CircleShape.hpp>

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

    SECTION(
        "Construct clockwise ConvexShapes from CircleShapes to verify that they get approx. the same geometric center")
    {
        sf::ConvexShape convex;
        for (unsigned int i = 2; i < 10; ++i)
        {
            const sf::CircleShape circle(4.f, i);
            convex.setPointCount(i);
            for (unsigned int j = 0; j < i; ++j)
            {
                convex.setPoint(j, circle.getPoint(j));
            }
            CHECK(convex.getGeometricCenter() == Approx(circle.getGeometricCenter()));
        }
    }

    SECTION(
        "Construct counterclockwise ConvexShapes from CircleShapes to verify that they get approx. the same geometric "
        "center")
    {
        sf::ConvexShape convex;
        for (unsigned int i = 2; i < 10; ++i)
        {
            const sf::CircleShape circle(4.f, i);
            convex.setPointCount(i);
            for (unsigned int j = 0; j < i; ++j)
            {
                convex.setPoint(i - 1 - j, circle.getPoint(j));
            }
            CHECK(convex.getGeometricCenter() == Approx(circle.getGeometricCenter()));
        }
    }

    SECTION("Geometric center for one point")
    {
        sf::ConvexShape convex(1);
        convex.setPoint(0, {1.f, 1.f});
        CHECK(convex.getGeometricCenter() == sf::Vector2f(1.f, 1.f));
    }

    SECTION("Geometric center for two points")
    {
        sf::ConvexShape convex(2);
        convex.setPoint(0, {0.f, 0.f});
        convex.setPoint(1, {4.f, 2.f});
        CHECK(convex.getGeometricCenter() == sf::Vector2f(2.f, 1.f));
    }

    SECTION("Geometric center for three points with a small area")
    {
        sf::ConvexShape convex(3);
        convex.setPoint(0, {-100000.f, 0.f});
        convex.setPoint(1, {100000.f, 0.f});
        convex.setPoint(2, {100000.f, 0.000001f});
        CHECK(convex.getGeometricCenter() == Approx(sf::Vector2f(100000.f / 3.f, 0.f)));
    }

    SECTION("Geometric center for aligned points")
    {
        SECTION("Geometric center for partly aligned points")
        {
            sf::ConvexShape convex(3);
            convex.setPoint(0, {-100.f, 0.f});
            convex.setPoint(1, {0.f, 0.f});
            convex.setPoint(2, {100.f, 1.f});
            CHECK(convex.getGeometricCenter() == Approx(sf::Vector2f(0.f, 1.f / 3.f)));
        }

        SECTION("Geometric center for aligned points with the two furthest apart not first and last")
        {
            sf::ConvexShape convex(4);
            convex.setPoint(0, {-50.f, -50.f});
            convex.setPoint(1, {-150.f, -150.f});
            convex.setPoint(2, {150.f, 150.f});
            convex.setPoint(3, {50.f, 50.f});
            CHECK(convex.getGeometricCenter() == sf::Vector2f(0.f, 0.f));
        }

        SECTION("Geometric center for aligned points increasing x and y")
        {
            sf::ConvexShape convex(3);
            convex.setPoint(0, {1.f, 1.f});
            convex.setPoint(1, {5.f, 3.f});
            convex.setPoint(2, {9.f, 5.f});
            CHECK(convex.getGeometricCenter() == sf::Vector2f(5.f, 3.f));
        }

        SECTION("Geometric center for aligned points increasing x, decreasing y")
        {
            sf::ConvexShape convex(3);
            convex.setPoint(0, {1.f, 5.f});
            convex.setPoint(1, {5.f, 3.f});
            convex.setPoint(2, {9.f, 1.f});
            CHECK(convex.getGeometricCenter() == sf::Vector2f(5.f, 3.f));
        }

        SECTION("Geometric center for aligned points decreasing x and y")
        {
            sf::ConvexShape convex(3);
            convex.setPoint(0, {9.f, 5.f});
            convex.setPoint(1, {5.f, 3.f});
            convex.setPoint(2, {1.f, 1.f});
            CHECK(convex.getGeometricCenter() == sf::Vector2f(5.f, 3.f));
        }

        SECTION("Geometric center for aligned points decreasing x, increasing y")
        {
            sf::ConvexShape convex(3);
            convex.setPoint(0, {9.f, 1.f});
            convex.setPoint(1, {5.f, 3.f});
            convex.setPoint(2, {1.f, 5.f});
            CHECK(convex.getGeometricCenter() == sf::Vector2f(5.f, 3.f));
        }

        SECTION("Geometric center for aligned points with the same x value")
        {
            sf::ConvexShape convex(3);
            convex.setPoint(0, {1.f, 2.f});
            convex.setPoint(1, {1.f, 3.f});
            convex.setPoint(2, {1.f, 1.f});
            CHECK(convex.getGeometricCenter() == sf::Vector2f(1.f, 2.f));
        }

        SECTION("Geometric center for aligned points with the same y value")
        {
            sf::ConvexShape convex(3);
            convex.setPoint(0, {2.f, 5.f});
            convex.setPoint(1, {3.f, 5.f});
            convex.setPoint(2, {1.f, 5.f});
            CHECK(convex.getGeometricCenter() == sf::Vector2f(2.f, 5.f));
        }

        SECTION("Geometric center for aligned points out of order")
        {
            sf::ConvexShape convex(3);
            convex.setPoint(0, {5.f, 3.f});
            convex.setPoint(1, {1.f, 5.f});
            convex.setPoint(2, {9.f, 1.f});
            CHECK(convex.getGeometricCenter() == sf::Vector2f(5.f, 3.f));
        }
    }
}
