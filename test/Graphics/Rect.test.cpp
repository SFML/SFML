#include <SFML/Graphics/Rect.hpp>

#include <SFML/System/Vector2.hpp>

#include <catch2/catch_test_macros.hpp>

#include <GraphicsUtil.hpp>
#include <type_traits>

TEST_CASE("[Graphics] sf::Rect")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::IntRect>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::IntRect>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::IntRect>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::IntRect>);
    }

    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            constexpr sf::IntRect rectangle;
            STATIC_CHECK(rectangle.left == 0);
            STATIC_CHECK(rectangle.top == 0);
            STATIC_CHECK(rectangle.width == 0);
            STATIC_CHECK(rectangle.height == 0);
        }

        SECTION("(left, top, width, height) constructor")
        {
            constexpr sf::IntRect rectangle({1, 2}, {3, 4});
            STATIC_CHECK(rectangle.left == 1);
            STATIC_CHECK(rectangle.top == 2);
            STATIC_CHECK(rectangle.width == 3);
            STATIC_CHECK(rectangle.height == 4);
        }

        SECTION("(Vector2, Vector2) constructor")
        {
            constexpr sf::Vector2i position(1, 2);
            constexpr sf::Vector2i dimension(3, 4);
            constexpr sf::IntRect  rectangle(position, dimension);

            STATIC_CHECK(rectangle.left == 1);
            STATIC_CHECK(rectangle.top == 2);
            STATIC_CHECK(rectangle.width == 3);
            STATIC_CHECK(rectangle.height == 4);
        }

        SECTION("Conversion constructor")
        {
            constexpr sf::FloatRect sourceRectangle({1.0f, 2.0f}, {3.0f, 4.0f});
            constexpr sf::IntRect   rectangle(sourceRectangle);

            STATIC_CHECK(rectangle.left == static_cast<int>(sourceRectangle.left));
            STATIC_CHECK(rectangle.top == static_cast<int>(sourceRectangle.top));
            STATIC_CHECK(rectangle.width == static_cast<int>(sourceRectangle.width));
            STATIC_CHECK(rectangle.height == static_cast<int>(sourceRectangle.height));
        }
    }

    SECTION("contains(Vector2)")
    {
        constexpr sf::IntRect rectangle({0, 0}, {10, 10});

        STATIC_CHECK(rectangle.contains(sf::Vector2i(0, 0)) == true);
        STATIC_CHECK(rectangle.contains(sf::Vector2i(9, 0)) == true);
        STATIC_CHECK(rectangle.contains(sf::Vector2i(0, 9)) == true);
        STATIC_CHECK(rectangle.contains(sf::Vector2i(9, 9)) == true);
        STATIC_CHECK(rectangle.contains(sf::Vector2i(9, 10)) == false);
        STATIC_CHECK(rectangle.contains(sf::Vector2i(10, 9)) == false);
        STATIC_CHECK(rectangle.contains(sf::Vector2i(10, 10)) == false);
        STATIC_CHECK(rectangle.contains(sf::Vector2i(15, 15)) == false);
    }

    SECTION("findIntersection()")
    {
        constexpr sf::IntRect rectangle({0, 0}, {10, 10});
        constexpr sf::IntRect intersectingRectangle({5, 5}, {10, 10});

        constexpr auto intersectionResult = rectangle.findIntersection(intersectingRectangle);
        STATIC_REQUIRE(intersectionResult.has_value());
        STATIC_CHECK(intersectionResult->top == 5);
        STATIC_CHECK(intersectionResult->left == 5);
        STATIC_CHECK(intersectionResult->width == 5);
        STATIC_CHECK(intersectionResult->height == 5);

        constexpr sf::IntRect nonIntersectingRectangle({-5, -5}, {5, 5});
        STATIC_CHECK_FALSE(rectangle.findIntersection(nonIntersectingRectangle).has_value());
    }

    SECTION("getPosition()")
    {
        STATIC_CHECK(sf::IntRect({}, {}).getPosition() == sf::Vector2i());
        STATIC_CHECK(sf::IntRect({1, 2}, {3, 4}).getPosition() == sf::Vector2i(1, 2));
    }

    SECTION("getSize()")
    {
        STATIC_CHECK(sf::IntRect({}, {}).getSize() == sf::Vector2i());
        STATIC_CHECK(sf::IntRect({1, 2}, {3, 4}).getSize() == sf::Vector2i(3, 4));
    }

    SECTION("Operators")
    {
        SECTION("operator==")
        {
            STATIC_CHECK(sf::IntRect() == sf::IntRect());
            STATIC_CHECK(sf::IntRect({1, 3}, {2, 5}) == sf::IntRect({1, 3}, {2, 5}));

            STATIC_CHECK_FALSE(sf::IntRect({1, 0}, {0, 0}) == sf::IntRect({0, 0}, {0, 0}));
            STATIC_CHECK_FALSE(sf::IntRect({0, 1}, {0, 0}) == sf::IntRect({0, 0}, {0, 0}));
            STATIC_CHECK_FALSE(sf::IntRect({0, 0}, {1, 0}) == sf::IntRect({0, 0}, {0, 0}));
            STATIC_CHECK_FALSE(sf::IntRect({0, 0}, {0, 1}) == sf::IntRect({0, 0}, {0, 0}));
        }

        SECTION("operator!=")
        {
            STATIC_CHECK(sf::IntRect({1, 0}, {0, 0}) != sf::IntRect({0, 0}, {0, 0}));
            STATIC_CHECK(sf::IntRect({0, 1}, {0, 0}) != sf::IntRect({0, 0}, {0, 0}));
            STATIC_CHECK(sf::IntRect({0, 0}, {1, 0}) != sf::IntRect({0, 0}, {0, 0}));
            STATIC_CHECK(sf::IntRect({0, 0}, {0, 1}) != sf::IntRect({0, 0}, {0, 0}));

            STATIC_CHECK_FALSE(sf::IntRect() != sf::IntRect());
            STATIC_CHECK_FALSE(sf::IntRect({1, 3}, {2, 5}) != sf::IntRect({1, 3}, {2, 5}));
        }
    }
}
