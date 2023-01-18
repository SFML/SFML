#include <SFML/System/Vector3.hpp>

#include <catch2/catch_test_macros.hpp>

#include <SystemUtil.hpp>
#include <type_traits>

// Use sf::Vector3i for tests (except for float vector algebra).
// Test coverage is given, as there are no template specializations.

TEST_CASE("[System] sf::Vector3")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::Vector3i>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Vector3i>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Vector3i>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Vector3i>);
    }

    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            constexpr sf::Vector3i vector;
            STATIC_CHECK(vector.x == 0);
            STATIC_CHECK(vector.y == 0);
            STATIC_CHECK(vector.z == 0);
        }

        SECTION("(x, y, z) coordinate constructor")
        {
            constexpr sf::Vector3i vector(1, 2, 3);
            STATIC_CHECK(vector.x == 1);
            STATIC_CHECK(vector.y == 2);
            STATIC_CHECK(vector.z == 3);
        }

        SECTION("Conversion constructor")
        {
            constexpr sf::Vector3f sourceVector(1.0f, 2.0f, 3.0f);
            constexpr sf::Vector3i vector(sourceVector);

            STATIC_CHECK(vector.x == static_cast<int>(sourceVector.x));
            STATIC_CHECK(vector.y == static_cast<int>(sourceVector.y));
            STATIC_CHECK(vector.z == static_cast<int>(sourceVector.z));
        }
    }

    SECTION("Unary operations")
    {
        SECTION("-vector")
        {
            constexpr sf::Vector3i vector(1, 2, 3);
            constexpr sf::Vector3i negatedVector = -vector;

            STATIC_CHECK(negatedVector.x == -1);
            STATIC_CHECK(negatedVector.y == -2);
            STATIC_CHECK(negatedVector.z == -3);
        }
    }

    SECTION("Arithmetic operations between two vectors")
    {
        sf::Vector3i           firstVector(2, 5, 6);
        constexpr sf::Vector3i secondVector(8, 3, 7);

        SECTION("vector += vector")
        {
            firstVector += secondVector;

            CHECK(firstVector.x == 10);
            CHECK(firstVector.y == 8);
            CHECK(firstVector.z == 13);
        }

        SECTION("vector -= vector")
        {
            firstVector -= secondVector;

            CHECK(firstVector.x == -6);
            CHECK(firstVector.y == 2);
            CHECK(firstVector.z == -1);
        }

        SECTION("vector + vector")
        {
            const sf::Vector3i result = firstVector + secondVector;

            CHECK(result.x == 10);
            CHECK(result.y == 8);
            CHECK(result.z == 13);
        }

        SECTION("vector - vector")
        {
            const sf::Vector3i result = firstVector - secondVector;

            CHECK(result.x == -6);
            CHECK(result.y == 2);
            CHECK(result.z == -1);
        }
    }

    SECTION("Arithmetic operations between vector and scalar value")
    {
        sf::Vector3i  vector(26, 12, 6);
        constexpr int scalar = 2;

        SECTION("vector * scalar")
        {
            const sf::Vector3i result = vector * scalar;

            CHECK(result.x == 52);
            CHECK(result.y == 24);
            CHECK(result.z == 12);
        }

        SECTION("scalar * vector")
        {
            const sf::Vector3i result = scalar * vector;

            CHECK(result.x == 52);
            CHECK(result.y == 24);
            CHECK(result.z == 12);
        }

        SECTION("vector *= scalar")
        {
            vector *= scalar;

            CHECK(vector.x == 52);
            CHECK(vector.y == 24);
            CHECK(vector.z == 12);
        }

        SECTION("vector / scalar")
        {
            const sf::Vector3i result = vector / scalar;

            CHECK(result.x == 13);
            CHECK(result.y == 6);
            CHECK(result.z == 3);
        }

        SECTION("vector /= scalar")
        {
            vector /= scalar;

            CHECK(vector.x == 13);
            CHECK(vector.y == 6);
            CHECK(vector.z == 3);
        }
    }

    SECTION("Comparison operations (two equal and one different vector)")
    {
        constexpr sf::Vector3i firstEqualVector(1, 5, 6);
        constexpr sf::Vector3i secondEqualVector(1, 5, 6);
        constexpr sf::Vector3i differentVector(6, 9, 7);

        SECTION("vector == vector")
        {
            STATIC_CHECK(firstEqualVector == secondEqualVector);
            STATIC_CHECK_FALSE(firstEqualVector == differentVector);
        }

        SECTION("vector != vector")
        {
            STATIC_CHECK(firstEqualVector != differentVector);
            STATIC_CHECK_FALSE(firstEqualVector != secondEqualVector);
        }
    }

    SECTION("Structured bindings")
    {
        sf::Vector3i vector(1, 2, 3); // NOLINT(misc-const-correctness)

        SECTION("destructure by value")
        {
            auto [x, y, z] = vector;

            CHECK(x == 1);
            CHECK(y == 2);
            CHECK(z == 3);

            STATIC_CHECK(std::is_same_v<decltype(x), decltype(vector.x)>);

            x = 3;

            CHECK(x == 3);
            CHECK(vector.x == 1);
        }

        SECTION("destructure by ref")
        {
            auto& [x, y, z] = vector;

            CHECK(x == 1);
            CHECK(y == 2);
            CHECK(z == 3);

            STATIC_CHECK(std::is_same_v<decltype(x), decltype(vector.x)>);

            x = 3;

            CHECK(x == 3);
            CHECK(vector.x == 3);
        }
    }

    SECTION("Length and normalization")
    {
        constexpr sf::Vector3f v(2.4f, 3.0f, 5.2f);

        CHECK(v.length() == Approx(6.46529f));
        CHECK(v.lengthSq() == Approx(41.79997f));
        CHECK(v.normalized() == Approx(sf::Vector3f(0.37121f, 0.46401f, 0.80429f)));
    }

    SECTION("Products and quotients")
    {
        constexpr sf::Vector3f v(2.4f, 3.0f, 5.2f);
        constexpr sf::Vector3f w(-0.7f, -2.2f, -4.8f);

        CHECK(v.dot(w) == Approx(-33.24f));
        CHECK(w.dot(v) == Approx(-33.24f));

        CHECK(v.cross(w) == Approx(sf::Vector3f(-2.96f, 7.88f, -3.18f)));
        CHECK(w.cross(v) == Approx(sf::Vector3f(2.96f, -7.88f, 3.18f)));

        CHECK(v.cwiseMul(w) == Approx(sf::Vector3f(-1.68f, -6.6f, -24.96f)));
        CHECK(w.cwiseMul(v) == Approx(sf::Vector3f(-1.68f, -6.6f, -24.96f)));
        CHECK(v.cwiseDiv(w) == Approx(sf::Vector3f(-3.428571f, -1.363636f, -1.0833333f)));
        CHECK(w.cwiseDiv(v) == Approx(sf::Vector3f(-0.291666f, -0.733333f, -0.9230769f)));
    }
}
