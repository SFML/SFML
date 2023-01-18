#include <SFML/Graphics/VertexArray.hpp>

#include <catch2/catch_test_macros.hpp>

#include <GraphicsUtil.hpp>
#include <type_traits>

TEST_CASE("[Graphics] sf::VertexArray")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::VertexArray>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::VertexArray>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::VertexArray>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::VertexArray>);
    }

    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            const sf::VertexArray vertexArray;
            CHECK(vertexArray.getVertexCount() == 0);
            CHECK(vertexArray.getPrimitiveType() == sf::PrimitiveType::Points);
            CHECK(vertexArray.getBounds() == sf::FloatRect({0, 0}, {0, 0}));
        }

        SECTION("Explicit constructor with default argument")
        {
            const sf::VertexArray vertexArray(sf::PrimitiveType::Lines);
            CHECK(vertexArray.getVertexCount() == 0);
            CHECK(vertexArray.getPrimitiveType() == sf::PrimitiveType::Lines);
            CHECK(vertexArray.getBounds() == sf::FloatRect({0, 0}, {0, 0}));
        }

        SECTION("Explicit constructor")
        {
            const sf::VertexArray vertexArray(sf::PrimitiveType::Lines, 10);
            CHECK(vertexArray.getVertexCount() == 10);
            CHECK(vertexArray.getPrimitiveType() == sf::PrimitiveType::Lines);
            CHECK(vertexArray.getBounds() == sf::FloatRect({0, 0}, {0, 0}));
            for (std::size_t i = 0; i < vertexArray.getVertexCount(); ++i)
            {
                CHECK(vertexArray[i].position == sf::Vertex().position);
                CHECK(vertexArray[i].color == sf::Vertex().color);
                CHECK(vertexArray[i].texCoords == sf::Vertex().texCoords);
            }
        }
    }

    SECTION("Resize array")
    {
        sf::VertexArray vertexArray;
        vertexArray.resize(42);
        CHECK(vertexArray.getVertexCount() == 42);
        for (std::size_t i = 0; i < vertexArray.getVertexCount(); ++i)
        {
            CHECK(vertexArray[i].position == sf::Vertex().position);
            CHECK(vertexArray[i].color == sf::Vertex().color);
            CHECK(vertexArray[i].texCoords == sf::Vertex().texCoords);
        }
    }

    SECTION("Clear array")
    {
        sf::VertexArray vertexArray;
        vertexArray.append({});
        vertexArray.clear();
        CHECK(vertexArray.getVertexCount() == 0);
    }

    SECTION("Append to array")
    {
        sf::VertexArray  vertexArray;
        const sf::Vertex vertex({1, 2}, {3, 4, 5, 6}, {7, 8});
        vertexArray.append(vertex);
        CHECK(vertexArray.getVertexCount() == 1);
        CHECK(vertexArray[0].position == vertex.position);
        CHECK(vertexArray[0].color == vertex.color);
        CHECK(vertexArray[0].texCoords == vertex.texCoords);
    }

    SECTION("Write to array")
    {
        sf::VertexArray vertexArray;
        vertexArray.resize(10);
        const sf::Vertex otherVertex({2, 3}, {4, 5, 6, 7}, {8, 9});
        vertexArray[9] = otherVertex;
        CHECK(vertexArray[9].position == otherVertex.position);
        CHECK(vertexArray[9].color == otherVertex.color);
        CHECK(vertexArray[9].texCoords == otherVertex.texCoords);
    }

    SECTION("Set primitive type")
    {
        sf::VertexArray vertexArray;
        vertexArray.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
        CHECK(vertexArray.getPrimitiveType() == sf::PrimitiveType::TriangleStrip);
    }

    SECTION("Get bounds")
    {
        sf::VertexArray vertexArray;
        vertexArray.append(sf::Vertex({1, 1}));
        vertexArray.append(sf::Vertex({2, 2}));
        CHECK(vertexArray.getBounds() == sf::FloatRect({1, 1}, {1, 1}));
        vertexArray[0] = sf::Vertex({0, 0});
        CHECK(vertexArray.getBounds() == sf::FloatRect({0, 0}, {2, 2}));
        vertexArray[0] = sf::Vertex({5, 5});
        CHECK(vertexArray.getBounds() == sf::FloatRect({2, 2}, {3, 3}));
        vertexArray.append(sf::Vertex({10, 10}));
        CHECK(vertexArray.getBounds() == sf::FloatRect({2, 2}, {8, 8}));
    }
}
