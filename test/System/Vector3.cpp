#include <SFML/System/Vector3.hpp>
#include "SystemUtil.hpp"
#include <type_traits>

#include <doctest.h>

// Use sf::Vector3i for tests. Test coverage is given, as there are no template specializations.

TEST_CASE("sf::Vector3 class template - [system]")
{
    SUBCASE("Construction")
    {
        SUBCASE("Default constructor")
        {
            sf::Vector3i vector;
            CHECK(vector.x == 0);
            CHECK(vector.y == 0);
            CHECK(vector.z == 0);
        }

        SUBCASE("(x, y, z) coordinate constructor")
        {
            sf::Vector3i vector(1, 2, 3);
            CHECK(vector.x == 1);
            CHECK(vector.y == 2);
            CHECK(vector.z == 3);
        }

        SUBCASE("Conversion constructor")
        {
            sf::Vector3f sourceVector(1.0f, 2.0f, 3.0f);
            sf::Vector3i vector(sourceVector);

            CHECK(vector.x == static_cast<int>(sourceVector.x));
            CHECK(vector.y == static_cast<int>(sourceVector.y));
            CHECK(vector.z == static_cast<int>(sourceVector.z));
        }
    }

    SUBCASE("Unary operations")
    {
        SUBCASE("-vector")
        {
            sf::Vector3i vector(1, 2, 3);
            sf::Vector3i negatedVector = -vector;

            CHECK(negatedVector.x == -1);
            CHECK(negatedVector.y == -2);
            CHECK(negatedVector.z == -3);
        }
    }

    SUBCASE("Arithmetic operations between two vectors")
    {
        sf::Vector3i firstVector(2, 5, 6);
        sf::Vector3i secondVector(8, 3, 7);

        SUBCASE("vector += vector")
        {
            firstVector += secondVector;

            CHECK(firstVector.x == 10);
            CHECK(firstVector.y == 8);
            CHECK(firstVector.z == 13);
        }

        SUBCASE("vector -= vector")
        {
            firstVector -= secondVector;

            CHECK(firstVector.x == -6);
            CHECK(firstVector.y == 2);
            CHECK(firstVector.z == -1);
        }

        SUBCASE("vector + vector")
        {
            sf::Vector3i result = firstVector + secondVector;

            CHECK(result.x == 10);
            CHECK(result.y == 8);
            CHECK(result.z == 13);
        }

        SUBCASE("vector - vector")
        {
            sf::Vector3i result = firstVector - secondVector;

            CHECK(result.x == -6);
            CHECK(result.y == 2);
            CHECK(result.z == -1);
        }
    }

    SUBCASE("Arithmetic operations between vector and scalar value")
    {
        sf::Vector3i vector(26, 12, 6);
        int scalar = 2;

        SUBCASE("vector * scalar")
        {
            sf::Vector3i result = vector * scalar;

            CHECK(result.x == 52);
            CHECK(result.y == 24);
            CHECK(result.z == 12);
        }

        SUBCASE("scalar * vector")
        {
            sf::Vector3i result = scalar * vector;

            CHECK(result.x == 52);
            CHECK(result.y == 24);
            CHECK(result.z == 12);
        }

        SUBCASE("vector *= scalar")
        {
            vector *= scalar;

            CHECK(vector.x == 52);
            CHECK(vector.y == 24);
            CHECK(vector.z == 12);
        }

        SUBCASE("vector / scalar")
        {
            sf::Vector3i result = vector / scalar;

            CHECK(result.x == 13);
            CHECK(result.y == 6);
            CHECK(result.z == 3);
        }

        SUBCASE("vector /= scalar")
        {
            vector /= scalar;

            CHECK(vector.x == 13);
            CHECK(vector.y == 6);
            CHECK(vector.z == 3);
        }
    }

    SUBCASE("Comparison operations (two equal and one different vector)")
    {
        sf::Vector3i firstEqualVector(1, 5, 6);
        sf::Vector3i secondEqualVector(1, 5, 6);
        sf::Vector3i differentVector(6, 9, 7);

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
        sf::Vector3i vector(1, 2, 3);

        SUBCASE("destructure by value")
        {
            auto [x, y, z] = vector;

            CHECK(x == 1);
            CHECK(y == 2);
            CHECK(z == 3);

            static_assert(std::is_same_v<decltype(x), decltype(vector.x)>);

            x = 3;

            CHECK(x == 3);
            CHECK(vector.x == 1);
        }

        SUBCASE("destructure by ref")
        {
            auto& [x, y, z] = vector;

            CHECK(x == 1);
            CHECK(y == 2);
            CHECK(z == 3);

            static_assert(std::is_same_v<decltype(x), decltype(vector.x)>);

            x = 3;

            CHECK(x == 3);
            CHECK(vector.x == 3);
        }
    }

    SUBCASE("Constexpr support")
    {
        constexpr sf::Vector3i vector(1, 2, 3);
        static_assert(vector.x == 1);
        static_assert(vector.y == 2);
        static_assert(vector.z == 3);
        static_assert(vector + sf::Vector3i(3, 2, 1) == sf::Vector3i(4, 4, 4));
    }
}
