#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <doctest/doctest.h>

#include <GraphicsUtil.hpp>

TEST_CASE("sf::Rect class template - [graphics]")
{
    SUBCASE("Construction")
    {
        SUBCASE("Default constructor")
        {
            sf::IntRect rectangle;
            CHECK(rectangle.left == 0);
            CHECK(rectangle.top == 0);
            CHECK(rectangle.width == 0);
            CHECK(rectangle.height == 0);
        }

        SUBCASE("(left, top, width, height) constructor")
        {
            sf::IntRect rectangle({1, 2}, {3, 4});
            CHECK(rectangle.left == 1);
            CHECK(rectangle.top == 2);
            CHECK(rectangle.width == 3);
            CHECK(rectangle.height == 4);
        }

        SUBCASE("(Vector2, Vector2) constructor")
        {
            sf::Vector2i position(1, 2);
            sf::Vector2i dimension(3, 4);
            sf::IntRect  rectangle(position, dimension);

            CHECK(rectangle.left == 1);
            CHECK(rectangle.top == 2);
            CHECK(rectangle.width == 3);
            CHECK(rectangle.height == 4);
        }

        SUBCASE("Conversion constructor")
        {
            sf::FloatRect sourceRectangle({1.0f, 2.0f}, {3.0f, 4.0f});
            sf::IntRect   rectangle(sourceRectangle);

            CHECK(rectangle.left == static_cast<int>(sourceRectangle.left));
            CHECK(rectangle.top == static_cast<int>(sourceRectangle.top));
            CHECK(rectangle.width == static_cast<int>(sourceRectangle.width));
            CHECK(rectangle.height == static_cast<int>(sourceRectangle.height));
        }
    }

    SUBCASE("contains(Vector2)")
    {
        sf::IntRect rectangle({0, 0}, {10, 10});

        CHECK(rectangle.contains(sf::Vector2i(0, 0)) == true);
        CHECK(rectangle.contains(sf::Vector2i(9, 0)) == true);
        CHECK(rectangle.contains(sf::Vector2i(0, 9)) == true);
        CHECK(rectangle.contains(sf::Vector2i(9, 9)) == true);
        CHECK(rectangle.contains(sf::Vector2i(9, 10)) == false);
        CHECK(rectangle.contains(sf::Vector2i(10, 9)) == false);
        CHECK(rectangle.contains(sf::Vector2i(10, 10)) == false);
        CHECK(rectangle.contains(sf::Vector2i(15, 15)) == false);
    }

    SUBCASE("findIntersection()")
    {
        const sf::IntRect rectangle({0, 0}, {10, 10});
        const sf::IntRect intersectingRectangle({5, 5}, {10, 10});

        const auto intersectionResult = rectangle.findIntersection(intersectingRectangle);
        REQUIRE(intersectionResult.has_value());
        CHECK(intersectionResult->top == 5);
        CHECK(intersectionResult->left == 5);
        CHECK(intersectionResult->width == 5);
        CHECK(intersectionResult->height == 5);

        const sf::IntRect nonIntersectingRectangle({-5, -5}, {5, 5});
        CHECK_FALSE(rectangle.findIntersection(nonIntersectingRectangle).has_value());
    }

    SUBCASE("getPosition()")
    {
        CHECK(sf::IntRect({}, {}).getPosition() == sf::Vector2i());
        CHECK(sf::IntRect({1, 2}, {3, 4}).getPosition() == sf::Vector2i(1, 2));
    }

    SUBCASE("getSize()")
    {
        CHECK(sf::IntRect({}, {}).getSize() == sf::Vector2i());
        CHECK(sf::IntRect({1, 2}, {3, 4}).getSize() == sf::Vector2i(3, 4));
    }

    SUBCASE("Operators")
    {
        SUBCASE("operator==")
        {
            CHECK(sf::IntRect() == sf::IntRect());
            CHECK(sf::IntRect({1, 3}, {2, 5}) == sf::IntRect({1, 3}, {2, 5}));

            CHECK_FALSE(sf::IntRect({1, 0}, {0, 0}) == sf::IntRect({0, 0}, {0, 0}));
            CHECK_FALSE(sf::IntRect({0, 1}, {0, 0}) == sf::IntRect({0, 0}, {0, 0}));
            CHECK_FALSE(sf::IntRect({0, 0}, {1, 0}) == sf::IntRect({0, 0}, {0, 0}));
            CHECK_FALSE(sf::IntRect({0, 0}, {0, 1}) == sf::IntRect({0, 0}, {0, 0}));
        }

        SUBCASE("operator!=")
        {
            CHECK(sf::IntRect({1, 0}, {0, 0}) != sf::IntRect({0, 0}, {0, 0}));
            CHECK(sf::IntRect({0, 1}, {0, 0}) != sf::IntRect({0, 0}, {0, 0}));
            CHECK(sf::IntRect({0, 0}, {1, 0}) != sf::IntRect({0, 0}, {0, 0}));
            CHECK(sf::IntRect({0, 0}, {0, 1}) != sf::IntRect({0, 0}, {0, 0}));

            CHECK_FALSE(sf::IntRect() != sf::IntRect());
            CHECK_FALSE(sf::IntRect({1, 3}, {2, 5}) != sf::IntRect({1, 3}, {2, 5}));
        }
    }
}
