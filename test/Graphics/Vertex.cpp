#include <SFML/Graphics/Vertex.hpp>
#include "GraphicsUtil.hpp"

#include <doctest.h>

TEST_CASE("sf::Vertex class - [graphics]")
{
    SUBCASE("Construction")
    {
        SUBCASE("Default constructor")
        {
            const sf::Vertex vertex;
            CHECK(vertex.position == sf::Vector2f(0.f, 0.f));
            CHECK(vertex.color == sf::Color(255, 255, 255));
            CHECK(vertex.texCoords == sf::Vector2f(0.f, 0.f));
        }

        SUBCASE("Position constructor")
        {
            const sf::Vertex vertex({1.f, 2.f});
            CHECK(vertex.position == sf::Vector2f(1.f, 2.f));
            CHECK(vertex.color == sf::Color(255, 255, 255));
            CHECK(vertex.texCoords == sf::Vector2f(0.f, 0.f));
        }

        SUBCASE("Position and color constructor")
        {
            const sf::Vertex vertex({1.f, 2.f}, {3, 4, 5, 6});
            CHECK(vertex.position == sf::Vector2f(1.f, 2.f));
            CHECK(vertex.color == sf::Color(3, 4, 5, 6));
            CHECK(vertex.texCoords == sf::Vector2f(0.f, 0.f));
        }

        SUBCASE("Position and coords constructor")
        {
            const sf::Vertex vertex({1.f, 2.f}, {3.f, 4.f});
            CHECK(vertex.position == sf::Vector2f(1.f, 2.f));
            CHECK(vertex.color == sf::Color(255, 255, 255));
            CHECK(vertex.texCoords == sf::Vector2f(3.f, 4.f));
        }

        SUBCASE("Position, color, and coords constructor")
        {
            const sf::Vertex vertex({1.f, 2.f}, {3, 4, 5, 6}, {7.f, 8.f});
            CHECK(vertex.position == sf::Vector2f(1.f, 2.f));
            CHECK(vertex.color == sf::Color(3, 4, 5, 6));
            CHECK(vertex.texCoords == sf::Vector2f(7.f, 8.f));
        }
    }

    SUBCASE("Constexpr support")
    {
        constexpr sf::Vertex vertex({1.f, 2.f}, {3, 4, 5, 6}, {7.f, 8.f});
        static_assert(vertex.position == sf::Vector2f(1.f, 2.f));
        static_assert(vertex.color == sf::Color(3, 4, 5, 6));
        static_assert(vertex.texCoords == sf::Vector2f(7.f, 8.f));
    }
}
