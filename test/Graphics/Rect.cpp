#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <doctest/doctest.h>

#include <GraphicsUtil.hpp>
#include <type_traits>

static_assert(std::is_copy_constructible_v<sf::IntRect>);
static_assert(std::is_copy_assignable_v<sf::IntRect>);
static_assert(std::is_nothrow_move_constructible_v<sf::IntRect>);
static_assert(std::is_nothrow_move_assignable_v<sf::IntRect>);

TEST_CASE("[Graphics] sf::Rect")
{
    SUBCASE("Construction")
    {
        SUBCASE("Default constructor")
        {
            const sf::IntRect rectangle;

            CHECK(rectangle.position == sf::Vector2i());
            CHECK(rectangle.size == sf::Vector2i());
        }

        SUBCASE("(Vector2, Vector2) constructor")
        {
            const sf::IntRect rectangle({1, 2}, {3, 4});

            CHECK(rectangle.position == sf::Vector2i(1, 2));
            CHECK(rectangle.size == sf::Vector2i(3, 4));
        }

        SUBCASE("Conversion constructor")
        {
            const sf::FloatRect sourceRectangle({1, 2}, {3, 4});
            const sf::IntRect   rectangle(sourceRectangle);

            CHECK(rectangle.position == sf::Vector2i(sourceRectangle.position));
            CHECK(rectangle.size == sf::Vector2i(sourceRectangle.size));
        }
    }

    SUBCASE("Acquiring edges")
    {
        SUBCASE("Positive size")
        {
            const sf::IntRect rectangle({1, 2}, {3, 4});

            CHECK(rectangle.getLeft() == 1);
            CHECK(rectangle.getTop() == 2);
            CHECK(rectangle.getRight() == 4);
            CHECK(rectangle.getBottom() == 6);
        }

        SUBCASE("Negative size")
        {
            const sf::IntRect rectangle({1, 2}, {-3, -4});

            CHECK(rectangle.getLeft() == -2);
            CHECK(rectangle.getTop() == -2);
            CHECK(rectangle.getRight() == 1);
            CHECK(rectangle.getBottom() == 2);
        }
    }

    SUBCASE("Acquiring absolute measurements")
    {
        SUBCASE("Positive size")
        {
            const sf::IntRect rectangle({2, 4}, {6, 8});

            CHECK(rectangle.getStart() == sf::Vector2i(2, 4));
            CHECK(rectangle.getEnd() == sf::Vector2i(8, 12));
            CHECK(rectangle.getCenter() == sf::Vector2i(5, 8));
            CHECK(rectangle.getAbsoluteSize() == sf::Vector2i(6, 8));
            CHECK(rectangle.getAbsoluteRect() == sf::IntRect({2, 4}, {6, 8}));
        }

        SUBCASE("Negative size")
        {
            const sf::IntRect rectangle({2, 4}, {-6, -8});

            CHECK(rectangle.getStart() == sf::Vector2i(-4, -4));
            CHECK(rectangle.getEnd() == sf::Vector2i(2, 4));
            CHECK(rectangle.getCenter() == sf::Vector2i(-1, 0));
            CHECK(rectangle.getAbsoluteSize() == sf::Vector2i(6, 8));
            CHECK(rectangle.getAbsoluteRect() == sf::IntRect({-4, -4}, {6, 8}));
        }
    }

    SUBCASE("contains(Vector2)")
    {
        SUBCASE("Positive size")
        {
            const sf::IntRect rectangle({1, 2}, {3, 4});

            CHECK(rectangle.contains(sf::Vector2i(1, 2)));
            CHECK(rectangle.contains(sf::Vector2i(3, 5)));
            CHECK_FALSE(rectangle.contains(sf::Vector2i(0, 1)));
            CHECK_FALSE(rectangle.contains(sf::Vector2i(4, 6)));
        }

        SUBCASE("Negative size")
        {
            const sf::IntRect rectangle({1, 2}, {-3, -4});

            CHECK(rectangle.contains(sf::Vector2i(-2, -2)));
            CHECK(rectangle.contains(sf::Vector2i(0, 1)));
            CHECK_FALSE(rectangle.contains(sf::Vector2i(-3, -3)));
            CHECK_FALSE(rectangle.contains(sf::Vector2i(1, 2)));
        }
    }

    SUBCASE("findIntersection(Rect)")
    {
        const sf::IntRect rectangle1({1, 2}, {3, 4});

        SUBCASE("Positive size")
        {
            // Interescts
            const sf::IntRect rectangle2({2, 3}, {4, 5});

            const std::optional<sf::IntRect> intersection1(rectangle1.findIntersection(rectangle2));
            REQUIRE(intersection1.has_value());
            CHECK(*intersection1 == sf::IntRect({2, 3}, {2, 3}));

            // Doesn't intersect
            const sf::IntRect rectangle4({4, 6}, {4, 6});
            CHECK_FALSE(rectangle1.findIntersection(rectangle4).has_value());
        }

        SUBCASE("Negative size")
        {
            // Interescts
            const sf::IntRect rectangle2({2, 3}, {-4, -5});

            const std::optional<sf::IntRect> intersection1(rectangle1.findIntersection(rectangle2));
            REQUIRE(intersection1.has_value());
            CHECK(*intersection1 == sf::IntRect({1, 2}, {1, 1}));

            // Doesn't intersect
            const sf::IntRect rectangle4({1, 2}, {-3, -4});
            CHECK_FALSE(rectangle1.findIntersection(rectangle4).has_value());
        }
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
