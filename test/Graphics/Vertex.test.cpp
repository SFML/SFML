#include <SFML/Graphics/Vertex.hpp>

#include <catch2/catch_test_macros.hpp>

#include <GraphicsUtil.hpp>
#include <type_traits>

TEST_CASE("[Graphics] sf::Vertex")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::Vertex>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Vertex>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Vertex>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Vertex>);
    }

    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            constexpr sf::Vertex vertex;
            STATIC_CHECK(vertex.position == sf::Vector2f(0.0f, 0.0f));
            STATIC_CHECK(vertex.color == sf::Color(255, 255, 255));
            STATIC_CHECK(vertex.texCoords == sf::Vector2f(0.0f, 0.0f));
        }

        SECTION("Position constructor")
        {
            constexpr sf::Vertex vertex({1, 2});
            STATIC_CHECK(vertex.position == sf::Vector2f(1.0f, 2.0f));
            STATIC_CHECK(vertex.color == sf::Color(255, 255, 255));
            STATIC_CHECK(vertex.texCoords == sf::Vector2f(0.0f, 0.0f));
        }

        SECTION("Position and color constructor")
        {
            constexpr sf::Vertex vertex({1, 2}, {3, 4, 5, 6});
            STATIC_CHECK(vertex.position == sf::Vector2f(1.0f, 2.0f));
            STATIC_CHECK(vertex.color == sf::Color(3, 4, 5, 6));
            STATIC_CHECK(vertex.texCoords == sf::Vector2f(0.0f, 0.0f));
        }

        SECTION("Position and coords constructor")
        {
            constexpr sf::Vertex vertex({1, 2}, {3, 4});
            STATIC_CHECK(vertex.position == sf::Vector2f(1.0f, 2.0f));
            STATIC_CHECK(vertex.color == sf::Color(255, 255, 255));
            STATIC_CHECK(vertex.texCoords == sf::Vector2f(3.0f, 4.0f));
        }

        SECTION("Position, color, and coords constructor")
        {
            constexpr sf::Vertex vertex({1, 2}, {3, 4, 5, 6}, {7, 8});
            STATIC_CHECK(vertex.position == sf::Vector2f(1.0f, 2.0f));
            STATIC_CHECK(vertex.color == sf::Color(3, 4, 5, 6));
            STATIC_CHECK(vertex.texCoords == sf::Vector2f(7.0f, 8.0f));
        }
    }
}
