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
            STATIC_CHECK(rectangle.position.x == 0);
            STATIC_CHECK(rectangle.position.y == 0);
            STATIC_CHECK(rectangle.size.x == 0);
            STATIC_CHECK(rectangle.size.y == 0);
        }

        SECTION("(left, top, width, height) constructor")
        {
            constexpr sf::Rect<TestType> rectangle({1, 2}, {3, 4});
            STATIC_CHECK(rectangle.position.x == 1);
            STATIC_CHECK(rectangle.position.y == 2);
            STATIC_CHECK(rectangle.size.x == 3);
            STATIC_CHECK(rectangle.size.y == 4);
        }

        SECTION("(Vector2, Vector2) constructor")
        {
            constexpr sf::Vector2<TestType> position(1, 2);
            constexpr sf::Vector2<TestType> dimension(3, 4);
            constexpr sf::Rect<TestType>    rectangle(position, dimension);

            STATIC_CHECK(rectangle.position.x == 1);
            STATIC_CHECK(rectangle.position.y == 2);
            STATIC_CHECK(rectangle.size.x == 3);
            STATIC_CHECK(rectangle.size.y == 4);
        }

        SECTION("Conversion constructor")
        {
            constexpr sf::FloatRect sourceRectangle({1.0f, 2.0f}, {3.0f, 4.0f});
            constexpr sf::IntRect   rectangle(sourceRectangle);

            STATIC_CHECK(rectangle.position.x == static_cast<int>(sourceRectangle.position.x));
            STATIC_CHECK(rectangle.position.y == static_cast<int>(sourceRectangle.position.y));
            STATIC_CHECK(rectangle.size.x == static_cast<int>(sourceRectangle.size.x));
            STATIC_CHECK(rectangle.size.y == static_cast<int>(sourceRectangle.size.y));
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
        STATIC_CHECK(intersectionResult->position.x == 5);
        STATIC_CHECK(intersectionResult->position.y == 5);
        STATIC_CHECK(intersectionResult->size.x == 5);
        STATIC_CHECK(intersectionResult->size.y == 5);

        constexpr sf::Rect<TestType> nonIntersectingRectangle({-5, -5}, {5, 5});
        STATIC_CHECK_FALSE(rectangle.findIntersection(nonIntersectingRectangle).has_value());
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
