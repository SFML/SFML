#include <SFML/System/Vector2.hpp>

#include <catch2/catch_template_test_macros.hpp>

#include <SystemUtil.hpp>
#include <type_traits>

#include <cmath>

using namespace sf::Literals;

TEMPLATE_TEST_CASE("[System] sf::Vector2", "", int, float)
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_trivially_copy_constructible_v<sf::Vector2<TestType>>);
        STATIC_CHECK(std::is_trivially_copy_assignable_v<sf::Vector2<TestType>>);
        STATIC_CHECK(std::is_trivially_move_constructible_v<sf::Vector2<TestType>>);
        STATIC_CHECK(std::is_trivially_move_assignable_v<sf::Vector2<TestType>>);
        STATIC_CHECK(std::is_trivially_copyable_v<sf::Vector2<TestType>>);
    }

    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            constexpr sf::Vector2<TestType> vector;
            STATIC_CHECK(vector.x == 0);
            STATIC_CHECK(vector.y == 0);
        }

        SECTION("(x, y) coordinate constructor")
        {
            constexpr sf::Vector2<TestType> vector(1, 2);
            STATIC_CHECK(vector.x == 1);
            STATIC_CHECK(vector.y == 2);
        }

        SECTION("Conversion operator")
        {
            STATIC_CHECK(!std::is_convertible_v<sf::Vector2f, sf::Vector2i>);

            constexpr sf::Vector2f sourceVector(1.0f, 2.0f);
            constexpr sf::Vector2i vector(sourceVector);

            STATIC_CHECK(vector.x == static_cast<int>(sourceVector.x));
            STATIC_CHECK(vector.y == static_cast<int>(sourceVector.y));
        }

        SECTION("Length and angle constructor")
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

    SECTION("Unary operations")
    {
        SECTION("-vector")
        {
            constexpr sf::Vector2<TestType> vector(1, 2);
            constexpr sf::Vector2<TestType> negatedVector = -vector;

            STATIC_CHECK(negatedVector.x == -1);
            STATIC_CHECK(negatedVector.y == -2);
        }
    }

    SECTION("Arithmetic operations between two vectors")
    {
        sf::Vector2<TestType>           firstVector(2, 5);
        constexpr sf::Vector2<TestType> secondVector(8, 3);

        SECTION("vector += vector")
        {
            firstVector += secondVector;

            CHECK(firstVector.x == 10);
            CHECK(firstVector.y == 8);
        }

        SECTION("vector -= vector")
        {
            firstVector -= secondVector;

            CHECK(firstVector.x == -6);
            CHECK(firstVector.y == 2);
        }

        SECTION("vector + vector")
        {
            const sf::Vector2<TestType> result = firstVector + secondVector;

            CHECK(result.x == 10);
            CHECK(result.y == 8);
        }

        SECTION("vector - vector")
        {
            const sf::Vector2<TestType> result = firstVector - secondVector;

            CHECK(result.x == -6);
            CHECK(result.y == 2);
        }
    }

    SECTION("Arithmetic operations between vector and scalar value")
    {
        sf::Vector2<TestType> vector(26, 12);
        const TestType        scalar = 2;

        SECTION("vector * scalar")
        {
            const sf::Vector2<TestType> result = vector * scalar;

            CHECK(result.x == 52);
            CHECK(result.y == 24);
        }

        SECTION("scalar * vector")
        {
            const sf::Vector2<TestType> result = scalar * vector;

            CHECK(result.x == 52);
            CHECK(result.y == 24);
        }

        SECTION("vector *= scalar")
        {
            vector *= scalar;

            CHECK(vector.x == 52);
            CHECK(vector.y == 24);
        }

        SECTION("vector / scalar")
        {
            const sf::Vector2<TestType> result = vector / scalar;

            CHECK(result.x == 13);
            CHECK(result.y == 6);
        }

        SECTION("vector /= scalar")
        {
            vector /= scalar;

            CHECK(vector.x == 13);
            CHECK(vector.y == 6);
        }
    }

    SECTION("Comparison operations (two equal and one different vector)")
    {
        constexpr sf::Vector2<TestType> firstEqualVector(1, 5);
        constexpr sf::Vector2<TestType> secondEqualVector(1, 5);
        constexpr sf::Vector2<TestType> differentVector(6, 9);

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
        sf::Vector2<TestType> vector(1, 2); // NOLINT(misc-const-correctness)

        SECTION("destructure by value")
        {
            auto [x, y] = vector;

            CHECK(x == 1);
            CHECK(y == 2);

            STATIC_CHECK(std::is_same_v<decltype(x), decltype(vector.x)>);

            x = 3;

            CHECK(x == 3);
            CHECK(vector.x == 1);
        }

        SECTION("destructure by ref")
        {
            auto& [x, y] = vector;

            CHECK(x == 1);
            CHECK(y == 2);

            STATIC_CHECK(std::is_same_v<decltype(x), decltype(vector.x)>);

            x = 3;

            CHECK(x == 3);
            CHECK(vector.x == 3);
        }
    }

    SECTION("Length and normalization")
    {
        constexpr sf::Vector2f v(2.4f, 3.0f);

        CHECK(v.length() == Approx(3.84187f));
        CHECK(v.lengthSquared() == Approx(14.7599650969f));
        CHECK(v.normalized() == Approx(sf::Vector2f(0.624695f, 0.780869f)));

        constexpr sf::Vector2f w(-0.7f, -2.2f);

        CHECK(w.length() == Approx(2.30868f));
        CHECK(w.lengthSquared() == Approx(5.3300033f));
        CHECK(w.normalized() == Approx(sf::Vector2f(-0.303204f, -0.952926f)));
    }

    SECTION("Rotations and angles")
    {
        constexpr sf::Vector2f v(2.4f, 3.0f);

        CHECK(v.angle() == Approx(51.3402_deg));
        CHECK(sf::Vector2f(1.f, 0.f).angleTo(v) == Approx(51.3402_deg));
        CHECK(sf::Vector2f(0.f, 1.f).angleTo(v) == Approx(-38.6598_deg));

        constexpr sf::Vector2f w(-0.7f, -2.2f);

        CHECK(w.angle() == Approx(-107.65_deg));
        CHECK(sf::Vector2f(1.f, 0.f).angleTo(w) == Approx(-107.65_deg));
        CHECK(sf::Vector2f(0.f, 1.f).angleTo(w) == Approx(162.35_deg));

        CHECK(v.angleTo(w) == Approx(-158.9902_deg));
        CHECK(w.angleTo(v) == Approx(158.9902_deg));

        const float ratio = w.length() / v.length();
        CHECK(v.rotatedBy(-158.9902_deg) * ratio == Approx(w));
        CHECK(w.rotatedBy(158.9902_deg) / ratio == Approx(v));

        STATIC_CHECK(v.perpendicular() == sf::Vector2f(-3.0f, 2.4f));
        STATIC_CHECK(v.perpendicular().perpendicular().perpendicular().perpendicular() == v);

        CHECK(v.rotatedBy(90_deg) == Approx(sf::Vector2f(-3.0f, 2.4f)));
        CHECK(v.rotatedBy(27.14_deg) == Approx(sf::Vector2f(0.767248f, 3.76448f)));
        CHECK(v.rotatedBy(-36.11_deg) == Approx(sf::Vector2f(3.70694f, 1.00925f)));
    }

    SECTION("Products and quotients")
    {
        constexpr sf::Vector2f v(2.4f, 3.0f);
        constexpr sf::Vector2f w(-0.7f, -2.2f);

        CHECK(v.dot(w) == Approx(-8.28f));
        CHECK(w.dot(v) == Approx(-8.28f));

        CHECK(v.cross(w) == Approx(-3.18f));
        CHECK(w.cross(v) == Approx(+3.18f));

        CHECK(v.componentWiseMul(w) == Approx(sf::Vector2f(-1.68f, -6.6f)));
        CHECK(w.componentWiseMul(v) == Approx(sf::Vector2f(-1.68f, -6.6f)));
        CHECK(v.componentWiseDiv(w) == Approx(sf::Vector2f(-3.428571f, -1.363636f)));
        CHECK(w.componentWiseDiv(v) == Approx(sf::Vector2f(-0.291666f, -0.733333f)));
    }

    SECTION("Projection")
    {
        constexpr sf::Vector2f v(2.4f, 3.0f);
        constexpr sf::Vector2f w(-0.7f, -2.2f);

        CHECK(v.projectedOnto(w) == Approx(sf::Vector2f(1.087430f, 3.417636f)));
        CHECK(v.projectedOnto(w) == Approx(-1.55347f * w));

        CHECK(w.projectedOnto(v) == Approx(sf::Vector2f(-1.346342f, -1.682927f)));
        CHECK(w.projectedOnto(v) == Approx(-0.560976f * v));

        CHECK(v.projectedOnto(sf::Vector2f(1.f, 0.f)) == Approx(sf::Vector2f(2.4f, 0.0f)));
        CHECK(v.projectedOnto(sf::Vector2f(0.f, 1.f)) == Approx(sf::Vector2f(0.0f, 3.0f)));
    }

    SECTION("Constexpr support")
    {
        constexpr sf::Vector2<TestType> v(1, 2);
        constexpr sf::Vector2<TestType> w(2, -6);

        STATIC_CHECK(v.x == 1);
        STATIC_CHECK(v.y == 2);
        STATIC_CHECK(v + w == sf::Vector2<TestType>(3, -4));

        STATIC_CHECK(v.lengthSquared() == 5);
        STATIC_CHECK(v.perpendicular() == sf::Vector2<TestType>(-2, 1));

        STATIC_CHECK(v.dot(w) == -10);
        STATIC_CHECK(v.cross(w) == -10);
        STATIC_CHECK(v.componentWiseMul(w) == sf::Vector2<TestType>(2, -12));
        STATIC_CHECK(w.componentWiseDiv(v) == sf::Vector2<TestType>(2, -3));
    }
}
