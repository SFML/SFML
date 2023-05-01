#include <SFML/System/Vector2.hpp>

#include <doctest/doctest.h>

#include <SystemUtil.hpp>
#include <type_traits>

#include <cmath>

using namespace sf::Literals;

// Use sf::Vector2i for tests (except for float vector algebra).
// Test coverage is given, as there are no template specializations.

static_assert(std::is_copy_constructible_v<sf::Vector2i>);
static_assert(std::is_copy_assignable_v<sf::Vector2i>);
static_assert(std::is_nothrow_move_constructible_v<sf::Vector2i>);
static_assert(std::is_nothrow_move_assignable_v<sf::Vector2i>);

TEST_CASE("[System] sf::Vector2")
{
    SUBCASE("Construction")
    {
        SUBCASE("Default constructor")
        {
            const sf::Vector2i vector;
            CHECK(vector.x == 0);
            CHECK(vector.y == 0);
        }

        SUBCASE("(x, y) coordinate constructor")
        {
            const sf::Vector2i vector(1, 2);
            CHECK(vector.x == 1);
            CHECK(vector.y == 2);
        }

        SUBCASE("Conversion constructor")
        {
            const sf::Vector2f sourceVector(1.0f, 2.0f);
            const sf::Vector2i vector(sourceVector);

            CHECK(vector.x == static_cast<int>(sourceVector.x));
            CHECK(vector.y == static_cast<int>(sourceVector.y));
        }

        SUBCASE("Length and angle constructor")
        {
            CHECK(sf::Vector2f(0, 0_deg) == sf::Vector2f(0, 0));
            CHECK(sf::Vector2f(0, 45_deg) == sf::Vector2f(0, 0));
            CHECK(sf::Vector2f(0, 90_deg) == sf::Vector2f(0, 0));
            CHECK(sf::Vector2f(0, 135_deg) == sf::Vector2f(0, 0));
            CHECK(sf::Vector2f(0, 180_deg) == sf::Vector2f(0, 0));
            CHECK(sf::Vector2f(0, 270_deg) == sf::Vector2f(0, 0));
            CHECK(sf::Vector2f(0, 360_deg) == sf::Vector2f(0, 0));
            CHECK(sf::Vector2f(0, -90_deg) == sf::Vector2f(0, 0));
            CHECK(sf::Vector2f(0, -180_deg) == sf::Vector2f(0, 0));
            CHECK(sf::Vector2f(0, -270_deg) == sf::Vector2f(0, 0));
            CHECK(sf::Vector2f(0, -360_deg) == sf::Vector2f(0, 0));

            CHECK(sf::Vector2f(1, 0_deg) == sf::Vector2f(1, 0));
            CHECK(sf::Vector2f(1, 45_deg) == Approx(sf::Vector2f(std::sqrt(2.f) / 2.f, std::sqrt(2.f) / 2.f)));
            CHECK(sf::Vector2f(1, 90_deg) == Approx(sf::Vector2f(0, 1)));
            CHECK(sf::Vector2f(1, 135_deg) == Approx(sf::Vector2f(-std::sqrt(2.f) / 2.f, std::sqrt(2.f) / 2.f)));
            CHECK(sf::Vector2f(1, 180_deg) == Approx(sf::Vector2f(-1, 0)));
            CHECK(sf::Vector2f(1, 270_deg) == Approx(sf::Vector2f(0, -1)));
            CHECK(sf::Vector2f(1, 360_deg) == Approx(sf::Vector2f(1, 0)));
            CHECK(sf::Vector2f(1, -90_deg) == Approx(sf::Vector2f(0, -1)));
            CHECK(sf::Vector2f(1, -180_deg) == Approx(sf::Vector2f(-1, 0)));
            CHECK(sf::Vector2f(1, -270_deg) == Approx(sf::Vector2f(0, 1)));
            CHECK(sf::Vector2f(1, -360_deg) == Approx(sf::Vector2f(1, 0)));

            CHECK(sf::Vector2f(-1, 0_deg) == sf::Vector2f(-1, 0));
            CHECK(sf::Vector2f(-1, 45_deg) == Approx(sf::Vector2f(-std::sqrt(2.f) / 2.f, -std::sqrt(2.f) / 2.f)));
            CHECK(sf::Vector2f(-1, 90_deg) == Approx(sf::Vector2f(0, -1)));
            CHECK(sf::Vector2f(-1, 135_deg) == Approx(sf::Vector2f(std::sqrt(2.f) / 2.f, -std::sqrt(2.f) / 2.f)));
            CHECK(sf::Vector2f(-1, 180_deg) == Approx(sf::Vector2f(1, 0)));
            CHECK(sf::Vector2f(-1, 270_deg) == Approx(sf::Vector2f(0, 1)));
            CHECK(sf::Vector2f(-1, 360_deg) == Approx(sf::Vector2f(-1, 0)));
            CHECK(sf::Vector2f(-1, -90_deg) == Approx(sf::Vector2f(0, 1)));
            CHECK(sf::Vector2f(-1, -180_deg) == Approx(sf::Vector2f(1, 0)));
            CHECK(sf::Vector2f(-1, -270_deg) == Approx(sf::Vector2f(0, -1)));
            CHECK(sf::Vector2f(-1, -360_deg) == Approx(sf::Vector2f(-1, 0)));

            CHECK(sf::Vector2f(4.2f, 0_deg) == sf::Vector2f(4.2f, 0));
            CHECK(sf::Vector2f(4.2f, 45_deg) ==
                  Approx(sf::Vector2f(4.2f * std::sqrt(2.f) / 2.f, 4.2f * std::sqrt(2.f) / 2.f)));
            CHECK(sf::Vector2f(4.2f, 90_deg) == Approx(sf::Vector2f(0, 4.2f)));
            CHECK(sf::Vector2f(4.2f, 135_deg) ==
                  Approx(sf::Vector2f(-4.2f * std::sqrt(2.f) / 2.f, 4.2f * std::sqrt(2.f) / 2.f)));
            CHECK(sf::Vector2f(4.2f, 180_deg) == Approx(sf::Vector2f(-4.2f, 0)));
            CHECK(sf::Vector2f(4.2f, 270_deg) == Approx(sf::Vector2f(0, -4.2f)));
            CHECK(sf::Vector2f(4.2f, 360_deg) == Approx(sf::Vector2f(4.2f, 0)));
            CHECK(sf::Vector2f(4.2f, -90_deg) == Approx(sf::Vector2f(0, -4.2f)));
            CHECK(sf::Vector2f(4.2f, -180_deg) == Approx(sf::Vector2f(-4.2f, 0)));
            CHECK(sf::Vector2f(4.2f, -270_deg) == Approx(sf::Vector2f(0, 4.2f)));
            CHECK(sf::Vector2f(4.2f, -360_deg) == Approx(sf::Vector2f(4.2f, 0)));
        }
    }

    SUBCASE("Unary operations")
    {
        SUBCASE("-vector")
        {
            const sf::Vector2i vector(1, 2);
            const sf::Vector2i negatedVector = -vector;

            CHECK(negatedVector.x == -1);
            CHECK(negatedVector.y == -2);
        }
    }

    SUBCASE("Arithmetic operations between two vectors")
    {
        sf::Vector2i       firstVector(2, 5);
        const sf::Vector2i secondVector(8, 3);

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
            const sf::Vector2i result = firstVector + secondVector;

            CHECK(result.x == 10);
            CHECK(result.y == 8);
        }

        SUBCASE("vector - vector")
        {
            const sf::Vector2i result = firstVector - secondVector;

            CHECK(result.x == -6);
            CHECK(result.y == 2);
        }
    }

    SUBCASE("Arithmetic operations between vector and scalar value")
    {
        sf::Vector2i vector(26, 12);
        const int    scalar = 2;

        SUBCASE("vector * scalar")
        {
            const sf::Vector2i result = vector * scalar;

            CHECK(result.x == 52);
            CHECK(result.y == 24);
        }

        SUBCASE("scalar * vector")
        {
            const sf::Vector2i result = scalar * vector;

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
            const sf::Vector2i result = vector / scalar;

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
        const sf::Vector2i firstEqualVector(1, 5);
        const sf::Vector2i secondEqualVector(1, 5);
        const sf::Vector2i differentVector(6, 9);

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

    SUBCASE("Length and normalization")
    {
        const sf::Vector2f v(2.4f, 3.0f);

        CHECK(v.length() == Approx(3.84187f));
        CHECK(v.lengthSq() == Approx(14.7599650969f));
        CHECK(v.normalized() == Approx(sf::Vector2f(0.624695f, 0.780869f)));

        const sf::Vector2f w(-0.7f, -2.2f);

        CHECK(w.length() == Approx(2.30868f));
        CHECK(w.lengthSq() == Approx(5.3300033f));
        CHECK(w.normalized() == Approx(sf::Vector2f(-0.303204f, -0.952926f)));
    }

    SUBCASE("Rotations and angles")
    {
        const sf::Vector2f v(2.4f, 3.0f);

        CHECK(v.angle() == Approx(51.3402_deg));
        CHECK(sf::Vector2f::UnitX.angleTo(v) == Approx(51.3402_deg));
        CHECK(sf::Vector2f::UnitY.angleTo(v) == Approx(-38.6598_deg));

        const sf::Vector2f w(-0.7f, -2.2f);

        CHECK(w.angle() == Approx(-107.65_deg));
        CHECK(sf::Vector2f::UnitX.angleTo(w) == Approx(-107.65_deg));
        CHECK(sf::Vector2f::UnitY.angleTo(w) == Approx(162.35_deg));

        CHECK(v.angleTo(w) == Approx(-158.9902_deg));
        CHECK(w.angleTo(v) == Approx(158.9902_deg));

        const float ratio = w.length() / v.length();
        CHECK(v.rotatedBy(-158.9902_deg) * ratio == Approx(w));
        CHECK(w.rotatedBy(158.9902_deg) / ratio == Approx(v));

        CHECK(v.perpendicular() == sf::Vector2f(-3.0f, 2.4f));
        CHECK(v.perpendicular().perpendicular().perpendicular().perpendicular() == v);

        CHECK(v.rotatedBy(90_deg) == Approx(sf::Vector2f(-3.0f, 2.4f)));
        CHECK(v.rotatedBy(27.14_deg) == Approx(sf::Vector2f(0.767248f, 3.76448f)));
        CHECK(v.rotatedBy(-36.11_deg) == Approx(sf::Vector2f(3.70694f, 1.00925f)));
    }

    SUBCASE("Products and quotients")
    {
        const sf::Vector2f v(2.4f, 3.0f);
        const sf::Vector2f w(-0.7f, -2.2f);

        CHECK(v.dot(w) == Approx(-8.28f));
        CHECK(w.dot(v) == Approx(-8.28f));

        CHECK(v.cross(w) == Approx(-3.18f));
        CHECK(w.cross(v) == Approx(+3.18f));

        CHECK(v.cwiseMul(w) == Approx(sf::Vector2f(-1.68f, -6.6f)));
        CHECK(w.cwiseMul(v) == Approx(sf::Vector2f(-1.68f, -6.6f)));
        CHECK(v.cwiseDiv(w) == Approx(sf::Vector2f(-3.428571f, -1.363636f)));
        CHECK(w.cwiseDiv(v) == Approx(sf::Vector2f(-0.291666f, -0.733333f)));
    }

    SUBCASE("Projection")
    {
        const sf::Vector2f v(2.4f, 3.0f);
        const sf::Vector2f w(-0.7f, -2.2f);

        CHECK(v.projectedOnto(w) == Approx(sf::Vector2f(1.087430f, 3.417636f)));
        CHECK(v.projectedOnto(w) == Approx(sf::Vector2f(-1.55347f * w)));

        CHECK(w.projectedOnto(v) == Approx(sf::Vector2f(-1.346342f, -1.682927f)));
        CHECK(w.projectedOnto(v) == Approx(sf::Vector2f(-0.560976f * v)));

        CHECK(v.projectedOnto(sf::Vector2f::UnitX) == Approx(sf::Vector2f(2.4f, 0.0f)));
        CHECK(v.projectedOnto(sf::Vector2f::UnitY) == Approx(sf::Vector2f(0.0f, 3.0f)));
    }

    SUBCASE("Constexpr support")
    {
        constexpr sf::Vector2i v(1, 2);
        constexpr sf::Vector2i w(2, -3);

        static_assert(v.x == 1);
        static_assert(v.y == 2);
        static_assert(v + w == sf::Vector2i(3, -1));

        static_assert(v.lengthSq() == 5);
        static_assert(v.perpendicular() == sf::Vector2i(-2, 1));

        static_assert(v.dot(w) == -4);
        static_assert(v.cross(w) == -7);
        static_assert(v.cwiseMul(w) == sf::Vector2i(2, -6));
        static_assert(w.cwiseDiv(v) == sf::Vector2i(2, -1));
    }
}
