#include <SFML/System/Vector2.hpp>
#include "SystemUtil.hpp"
#include <type_traits>

#include <doctest.h>

using namespace sf::Literals;
using doctest::Approx;

// Use sf::Vector2i for tests (except for float vector algebra).
// Test coverage is given, as there are no template specializations.


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
            const sf::Vector2f sourceVector(1.0f, 2.0f);
            const sf::Vector2i vector(sourceVector);

            CHECK(vector.x == static_cast<int>(sourceVector.x));
            CHECK(vector.y == static_cast<int>(sourceVector.y));
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
        sf::Vector2i firstVector(2, 5);
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
        int scalar = 2;

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

        CHECK(v.length() == Approx(3.84187));
        CHECK(v.lengthSq() == Approx(14.7599650969));
        CHECK(v.normalized() == ApproxVec(0.624695, 0.780869));

        const sf::Vector2f w(-0.7f, -2.2f);
        
        CHECK(w.length() == Approx(2.30868));
        CHECK(w.lengthSq() == Approx(5.3300033));
        CHECK(w.normalized() == ApproxVec(-0.303204, -0.952926));
    }

    SUBCASE("Rotations and angles")
    {
        const sf::Vector2f v(2.4f, 3.0f);
        
        CHECK(v.angle() == ApproxDeg(51.3402));
        CHECK(sf::Vector2f::UnitX.angleTo(v) == ApproxDeg(51.3402));
        CHECK(sf::Vector2f::UnitY.angleTo(v) == ApproxDeg(-38.6598));

        const sf::Vector2f w(-0.7f, -2.2f);

        CHECK(w.angle() == ApproxDeg(-107.65));
        CHECK(sf::Vector2f::UnitX.angleTo(w) == ApproxDeg(-107.65));
        CHECK(sf::Vector2f::UnitY.angleTo(w) == ApproxDeg(162.35));

        CHECK(v.angleTo(w) == ApproxDeg(-158.9902));
        CHECK(w.angleTo(v) == ApproxDeg(158.9902));

        const float ratio = w.length() / v.length();
        CHECK(v.rotatedBy(-158.9902_deg) * ratio  == ApproxVec(w));
        CHECK(w.rotatedBy(158.9902_deg) / ratio == ApproxVec(v));

        CHECK(v.perpendicular() == sf::Vector2f(-3.0f, 2.4f));
        CHECK(v.perpendicular().perpendicular().perpendicular().perpendicular() == v);

        CHECK(v.rotatedBy(90_deg) == ApproxVec(-3.0, 2.4));
        CHECK(v.rotatedBy(27.14_deg) == ApproxVec(0.767248, 3.76448));
        CHECK(v.rotatedBy(-36.11_deg) == ApproxVec(3.70694, 1.00925));
    }
    
    SUBCASE("Products and quotients")
    {
        const sf::Vector2f v(2.4f, 3.0f);
        const sf::Vector2f w(-0.7f, -2.2f);

        CHECK(v.dot(w) == Approx(-8.28));
        CHECK(w.dot(v) == Approx(-8.28));

        CHECK(v.cross(w) == Approx(-3.18));
        CHECK(w.cross(v) == Approx(+3.18));
        
        CHECK(v.cwiseMul(w) == ApproxVec(-1.68, -6.6));
        CHECK(w.cwiseMul(v) == ApproxVec(-1.68, -6.6));
        CHECK(v.cwiseDiv(w) == ApproxVec(-3.428571, -1.363636));
        CHECK(w.cwiseDiv(v) == ApproxVec(-0.291666, -0.733333));
    }
    
    SUBCASE("Projection")
    {
        const sf::Vector2f v(2.4f, 3.0f);
        const sf::Vector2f w(-0.7f, -2.2f);
        
        CHECK(v.projectedOnto(w) == ApproxVec(1.087430, 3.417636));
        CHECK(v.projectedOnto(w) == ApproxVec(-1.55347f * w));

        CHECK(w.projectedOnto(v) == ApproxVec(-1.346342, -1.682927));
        CHECK(w.projectedOnto(v) == ApproxVec(-0.560976f * v));

        CHECK(v.projectedOnto(sf::Vector2f::UnitX) == ApproxVec(2.4, 0.0));
        CHECK(v.projectedOnto(sf::Vector2f::UnitY) == ApproxVec(0.0, 3.0));
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
