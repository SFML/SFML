#include <SFML/System/Vector2.hpp>
#include <catch.hpp>

// Use sf::Vector2i for tests. Test coverage is given, as there are no template specializations.

SCENARIO("Constructing sf::Vector2")
{
    WHEN("default constructor is used")
    {
        sf::Vector2i vector;

        THEN("x and y are 0")
        {
            CHECK(vector.x == 0);
            CHECK(vector.y == 0);
        }
    }

    WHEN("x, y constructor is used")
    {
        sf::Vector2i vector(1, 2);

        THEN("x and y are set accordingly")
        {
            CHECK(vector.x == 1);
            CHECK(vector.y == 2);
        }
    }

    GIVEN("a vector of a different type")
    {
        sf::Vector2f sourceVector(1.0f, 2.0f);

        WHEN("a vector is constructed from the another vector")
        {
            sf::Vector2i vector(sourceVector);

            THEN("x and y are equal to the source vector's elements")
            {
                CHECK(vector.x == static_cast<int>(sourceVector.x));
                CHECK(vector.y == static_cast<int>(sourceVector.y));
            }
        }
    }
}

SCENARIO("sf::Vector2 algebra")
{
    GIVEN("a vector with x and y != 0")
    {
        sf::Vector2i vector(1, 2);

        WHEN("the vector is negated")
        {
            sf::Vector2i negatedVector = -vector;

            THEN("x and y are negated")
            {
                CHECK(negatedVector.x == -1);
                CHECK(negatedVector.y == -2);
            }
        }
    }

    GIVEN("two different vectors with x and y != 0")
    {
        sf::Vector2i firstVector(2, 5);
        sf::Vector2i secondVector(8, 3);

        WHEN("one vector is increased by another one")
        {
            firstVector += secondVector;

            THEN("the first operand's x and y are increased by the second one's")
            {
                CHECK(firstVector.x == 10);
                CHECK(firstVector.y == 8);
            }
        }

        WHEN("one vector is decreased by another one")
        {
            firstVector -= secondVector;

            THEN("the first operand's x and y are decreased by the second one's")
            {
                CHECK(firstVector.x == -6);
                CHECK(firstVector.y == 2);
            }
        }

        WHEN("two vectors are summed")
        {
            sf::Vector2i result = firstVector + secondVector;

            THEN("the result vector's x and y are the sum of the vectors' x and y")
            {
                CHECK(result.x == 10);
                CHECK(result.y == 8);
            }
        }

        WHEN("one vector is subtracted from another one")
        {
            sf::Vector2i result = firstVector - secondVector;

            THEN("the result vector's x and y are the difference of the vectors' x and y")
            {
                CHECK(result.x == -6);
                CHECK(result.y == 2);
            }
        }
    }

    GIVEN("a vector with x and y != 0 and a scalar value")
    {
        sf::Vector2i vector(26, 12);
        int scalar(2);

        WHEN("a vector is multiplied by a scalar")
        {
            sf::Vector2i result = vector * scalar;

            THEN("the result vector's x and y are the product with the scalar")
            {
                CHECK(result.x == 52);
                CHECK(result.y == 24);
            }
        }

        WHEN("a scalar is multiplied by a vector")
        {
            sf::Vector2i result = scalar * vector ;

            THEN("the result vector's x and y are the product with the scalar")
            {
                CHECK(result.x == 52);
                CHECK(result.y == 24);
            }
        }

        WHEN("a vector is multiplied by a scalar in itself")
        {
            vector *= scalar;

            THEN("the vector's x and y are the product with the scalar")
            {
                CHECK(vector.x == 52);
                CHECK(vector.y == 24);
            }
        }

        WHEN("a vector is divided by a scalar")
        {
            sf::Vector2i result = vector / scalar;

            THEN("the result vector's x and y are the division with the scalar")
            {
                CHECK(result.x == 13);
                CHECK(result.y == 6);
            }
        }

        WHEN("a vector is divided by a scalar in itself")
        {
            vector /= scalar;

            THEN("the vector's x and y are the division with the scalar")
            {
                CHECK(vector.x == 13);
                CHECK(vector.y == 6);
            }
        }
    }

    GIVEN("3 vectors, where two are equal")
    {
        sf::Vector2i firstEqualVector(1, 5);
        sf::Vector2i secondEqualVector(1, 5);
        sf::Vector2i differentVector(6, 9);

        WHEN("equal vectors are tested for equality")
        {
            bool equal = (firstEqualVector == secondEqualVector);

            THEN("the result is true")
            {
                CHECK(equal == true);
            }
        }

        WHEN("different vectors are tested for equality")
        {
            bool equal = (firstEqualVector == differentVector);

            THEN("the result is false")
            {
                CHECK(equal == false);
            }
        }

        WHEN("equal vectors are tested for inequality")
        {
            bool equal = (firstEqualVector != secondEqualVector);

            THEN("the result is false")
            {
                CHECK(equal == false);
            }
        }

        WHEN("different vectors are tested for inequality")
        {
            bool equal = (firstEqualVector != differentVector);

            THEN("the result is true")
            {
                CHECK(equal == true);
            }
        }
    }
}
