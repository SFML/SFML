#include <SFML/Graphics/Rect.hpp>

#include <SFML/System/Vector2.hpp>

#include <catch2/catch_template_test_macros.hpp>

#include <GraphicsUtil.hpp>
#include <type_traits>

TEMPLATE_TEST_CASE("[Graphics] sf::Rect", "", int, float)
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::Rect<TestType>>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Rect<TestType>>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Rect<TestType>>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Rect<TestType>>);
    }

    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            constexpr sf::Rect<TestType> rectangle;
            STATIC_CHECK(rectangle.left == 0);
            STATIC_CHECK(rectangle.top == 0);
            STATIC_CHECK(rectangle.width == 0);
            STATIC_CHECK(rectangle.height == 0);
        }

        SECTION("(left, top, width, height) constructor")
        {
            constexpr sf::Rect<TestType> rectangle({1, 2}, {3, 4});
            STATIC_CHECK(rectangle.left == 1);
            STATIC_CHECK(rectangle.top == 2);
            STATIC_CHECK(rectangle.width == 3);
            STATIC_CHECK(rectangle.height == 4);
        }

        SECTION("(Vector2, Vector2) constructor")
        {
            constexpr sf::Vector2<TestType> position(1, 2);
            constexpr sf::Vector2<TestType> dimension(3, 4);
            constexpr sf::Rect<TestType>    rectangle(position, dimension);

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
        constexpr sf::Rect<TestType> rectangle({0, 0}, {10, 10});

        STATIC_CHECK(rectangle.contains(sf::Vector2<TestType>(0, 0)) == true);
        STATIC_CHECK(rectangle.contains(sf::Vector2<TestType>(9, 0)) == true);
        STATIC_CHECK(rectangle.contains(sf::Vector2<TestType>(0, 9)) == true);
        STATIC_CHECK(rectangle.contains(sf::Vector2<TestType>(9, 9)) == true);
        STATIC_CHECK(rectangle.contains(sf::Vector2<TestType>(9, 10)) == false);
        STATIC_CHECK(rectangle.contains(sf::Vector2<TestType>(10, 9)) == false);
        STATIC_CHECK(rectangle.contains(sf::Vector2<TestType>(10, 10)) == false);
        STATIC_CHECK(rectangle.contains(sf::Vector2<TestType>(15, 15)) == false);
    }

    SECTION("findIntersection()")
    {
        constexpr sf::Rect<TestType> rectangle({0, 0}, {10, 10});
        constexpr sf::Rect<TestType> intersectingRectangle({5, 5}, {10, 10});

        constexpr auto intersectionResult = rectangle.findIntersection(intersectingRectangle);
        STATIC_REQUIRE(intersectionResult.has_value());
        STATIC_CHECK(intersectionResult->top == 5);
        STATIC_CHECK(intersectionResult->left == 5);
        STATIC_CHECK(intersectionResult->width == 5);
        STATIC_CHECK(intersectionResult->height == 5);

        constexpr sf::Rect<TestType> nonIntersectingRectangle({-5, -5}, {5, 5});
        STATIC_CHECK_FALSE(rectangle.findIntersection(nonIntersectingRectangle).has_value());
    }

    SECTION("getPosition()")
    {
        STATIC_CHECK(sf::Rect<TestType>({}, {}).getPosition() == sf::Vector2<TestType>());
        STATIC_CHECK(sf::Rect<TestType>({1, 2}, {3, 4}).getPosition() == sf::Vector2<TestType>(1, 2));
    }

    SECTION("getSize()")
    {
        STATIC_CHECK(sf::Rect<TestType>({}, {}).getSize() == sf::Vector2<TestType>());
        STATIC_CHECK(sf::Rect<TestType>({1, 2}, {3, 4}).getSize() == sf::Vector2<TestType>(3, 4));
    }

    SECTION("getCenter()")
    {
        STATIC_CHECK(sf::Rect<TestType>({}, {}).getCenter() == sf::Vector2<TestType>());
        STATIC_CHECK(sf::Rect<TestType>({1, 2}, {4, 6}).getCenter() == sf::Vector2<TestType>(3, 5));
    }

    SECTION("Operators")
    {
        SECTION("operator==")
        {
            STATIC_CHECK(sf::Rect<TestType>() == sf::Rect<TestType>());
            STATIC_CHECK(sf::Rect<TestType>({1, 3}, {2, 5}) == sf::Rect<TestType>({1, 3}, {2, 5}));

            STATIC_CHECK_FALSE(sf::Rect<TestType>({1, 0}, {0, 0}) == sf::Rect<TestType>({0, 0}, {0, 0}));
            STATIC_CHECK_FALSE(sf::Rect<TestType>({0, 1}, {0, 0}) == sf::Rect<TestType>({0, 0}, {0, 0}));
            STATIC_CHECK_FALSE(sf::Rect<TestType>({0, 0}, {1, 0}) == sf::Rect<TestType>({0, 0}, {0, 0}));
            STATIC_CHECK_FALSE(sf::Rect<TestType>({0, 0}, {0, 1}) == sf::Rect<TestType>({0, 0}, {0, 0}));
        }

        SECTION("operator!=")
        {
            STATIC_CHECK(sf::Rect<TestType>({1, 0}, {0, 0}) != sf::Rect<TestType>({0, 0}, {0, 0}));
            STATIC_CHECK(sf::Rect<TestType>({0, 1}, {0, 0}) != sf::Rect<TestType>({0, 0}, {0, 0}));
            STATIC_CHECK(sf::Rect<TestType>({0, 0}, {1, 0}) != sf::Rect<TestType>({0, 0}, {0, 0}));
            STATIC_CHECK(sf::Rect<TestType>({0, 0}, {0, 1}) != sf::Rect<TestType>({0, 0}, {0, 0}));

            STATIC_CHECK_FALSE(sf::Rect<TestType>() != sf::Rect<TestType>());
            STATIC_CHECK_FALSE(sf::Rect<TestType>({1, 3}, {2, 5}) != sf::Rect<TestType>({1, 3}, {2, 5}));
        }
    }
}
