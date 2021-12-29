#include <SFML/System/Vector2.hpp>
#include "SystemUtil.hpp"
#include <type_traits>

#include <doctest.h>

// Use sf::Vector2i for tests. Test coverage is given, as there are no template specializations.

TEST_CASE("sf::Vector2 class template - [system]")
{
    SUBCASE("Construction")
    {
        SUBCASE("Default constructor")
        {
            sf::Vector2i vector;
            CHECK(vector.x == 0);
            CHECK(vector.y == 0);
        }

        SUBCASE("(x, y) coordinate constructor")
        {
            sf::Vector2i vector(1, 2);
            CHECK(vector.x == 1);
            CHECK(vector.y == 2);
        }

        SUBCASE("Conversion constructor")
        {
            sf::Vector2f sourceVector(1.0f, 2.0f);
            sf::Vector2i vector(sourceVector);

            CHECK(vector.x == static_cast<int>(sourceVector.x));
            CHECK(vector.y == static_cast<int>(sourceVector.y));
        }
    }

    SUBCASE("Unary operations")
    {
        SUBCASE("-vector")
        {
            sf::Vector2i vector(1, 2);
            sf::Vector2i negatedVector = -vector;

            CHECK(negatedVector.x == -1);
            CHECK(negatedVector.y == -2);
        }
    }

    SUBCASE("Arithmetic operations between two vectors")
    {
        sf::Vector2i firstVector(2, 5);
        sf::Vector2i secondVector(8, 3);

        SUBCASE("vector += vector")
        {
            firstVector += secondVector;

            CHECK(firstVector.x == 10);
            CHECK(firstVector.y == 8);
        }

        SUBCASE("vector -= vector")
        {
            firstVector -= secondVector;

            CHECK(firstVector.x == -6);
            CHECK(firstVector.y == 2);
        }

        SUBCASE("vector + vector")
        {
            sf::Vector2i result = firstVector + secondVector;

            CHECK(result.x == 10);
            CHECK(result.y == 8);
        }

        SUBCASE("vector - vector")
        {
            sf::Vector2i result = firstVector - secondVector;

            CHECK(result.x == -6);
            CHECK(result.y == 2);
        }
    }

    SUBCASE("Arithmetic operations between vector and scalar value")
    {
        sf::Vector2i vector(26, 12);
        int scalar = 2;

        SUBCASE("vector * scalar")
        {
            sf::Vector2i result = vector * scalar;

            CHECK(result.x == 52);
            CHECK(result.y == 24);
        }

        SUBCASE("scalar * vector")
        {
            sf::Vector2i result = scalar * vector;

            CHECK(result.x == 52);
            CHECK(result.y == 24);
        }

        SUBCASE("vector *= scalar")
        {
            vector *= scalar;

            CHECK(vector.x == 52);
            CHECK(vector.y == 24);
        }

        SUBCASE("vector / scalar")
        {
            sf::Vector2i result = vector / scalar;

            CHECK(result.x == 13);
            CHECK(result.y == 6);
        }

        SUBCASE("vector /= scalar")
        {
            vector /= scalar;

            CHECK(vector.x == 13);
            CHECK(vector.y == 6);
        }
    }

    SUBCASE("Comparison operations (two equal and one different vector)")
    {
        sf::Vector2i firstEqualVector(1, 5);
        sf::Vector2i secondEqualVector(1, 5);
        sf::Vector2i differentVector(6, 9);

        SUBCASE("vector == vector")
        {
            CHECK(firstEqualVector == secondEqualVector);
            CHECK_FALSE(firstEqualVector == differentVector);
        }

        SUBCASE("vector != vector")
        {
            CHECK(firstEqualVector != differentVector);
            CHECK_FALSE(firstEqualVector != secondEqualVector);
        }
    }

    SUBCASE("Structured bindings")
    {
        sf::Vector2i vector(1, 2);

        SUBCASE("destructure by value")
        {
            auto [x, y] = vector;

            CHECK(x == 1);
            CHECK(y == 2);

            static_assert(std::is_same_v<decltype(x), decltype(vector.x)>);

            x = 3;

            CHECK(x == 3);
            CHECK(vector.x == 1);
        }

        SUBCASE("destructure by ref")
        {
            auto& [x, y] = vector;

            CHECK(x == 1);
            CHECK(y == 2);

            static_assert(std::is_same_v<decltype(x), decltype(vector.x)>);

            x = 3;

            CHECK(x == 3);
            CHECK(vector.x == 3);
        }
    }

    SUBCASE("Constexpr support")
    {
        constexpr sf::Vector2i vector(1, 2);
        static_assert(vector.x == 1);
        static_assert(vector.y == 2);
        static_assert(vector + sf::Vector2i(2, 1) == sf::Vector2i(3, 3));
    }
}
