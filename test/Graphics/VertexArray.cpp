#include <SFML/Graphics/VertexArray.hpp>
#include "GraphicsUtil.hpp"

#include <doctest.h>

TEST_CASE("sf::VertexArray class - [graphics]")
{
    SUBCASE("Construction")
    {
        SUBCASE("Default constructor")
        {
            const sf::VertexArray vertexArray;
            CHECK(vertexArray.getVertexCount() == 0);
            CHECK(vertexArray.getPrimitiveType() == sf::PrimitiveType::Points);
            CHECK(vertexArray.getBounds() == sf::FloatRect({0.f, 0.f}, {0.f, 0.f}));
        }

        SUBCASE("Explicit constructor with default argument")
        {
            const sf::VertexArray vertexArray(sf::PrimitiveType::Lines);
            CHECK(vertexArray.getVertexCount() == 0);
            CHECK(vertexArray.getPrimitiveType() == sf::PrimitiveType::Lines);
            CHECK(vertexArray.getBounds() == sf::FloatRect({0.f, 0.f}, {0.f, 0.f}));
        }

        SUBCASE("Explicit constructor")
        {
            const sf::VertexArray vertexArray(sf::PrimitiveType::Lines, 10);
            CHECK(vertexArray.getVertexCount() == 10);
            CHECK(vertexArray.getPrimitiveType() == sf::PrimitiveType::Lines);
            CHECK(vertexArray.getBounds() == sf::FloatRect({0.f, 0.f}, {0.f, 0.f}));
            for (std::size_t i = 0; i < vertexArray.getVertexCount(); ++i)
            {
                CHECK(vertexArray[i].position == sf::Vertex().position);
                CHECK(vertexArray[i].color == sf::Vertex().color);
                CHECK(vertexArray[i].texCoords == sf::Vertex().texCoords);
            }
        }
    }

    SUBCASE("Resize array")
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

    SUBCASE("Clear array")
    {
        sf::VertexArray vertexArray;
        vertexArray.append({});
        vertexArray.clear();
        CHECK(vertexArray.getVertexCount() == 0);
    }

    SUBCASE("Append to array")
    {
        sf::VertexArray vertexArray;
        const sf::Vertex vertex({1.f, 2.f}, {3, 4, 5, 6}, {7.f, 8.f});
        vertexArray.append(vertex);
        CHECK(vertexArray.getVertexCount() == 1);
        CHECK(vertexArray[0].position == vertex.position);
        CHECK(vertexArray[0].color == vertex.color);
        CHECK(vertexArray[0].texCoords == vertex.texCoords);
    }

    SUBCASE("Write to array")
    {
        sf::VertexArray vertexArray;
        vertexArray.resize(10);
        const sf::Vertex otherVertex({2.f, 3.f}, {4, 5, 6, 7}, {8.f, 9.f});
        vertexArray[9] = otherVertex;
        CHECK(vertexArray[9].position == otherVertex.position);
        CHECK(vertexArray[9].color == otherVertex.color);
        CHECK(vertexArray[9].texCoords == otherVertex.texCoords);
    }

    SUBCASE("Set primitive type")
    {
        sf::VertexArray vertexArray;
        vertexArray.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
        CHECK(vertexArray.getPrimitiveType() == sf::PrimitiveType::TriangleStrip);
    }

    SUBCASE("Get bounds")
    {
        sf::VertexArray vertexArray;
        vertexArray.append(sf::Vertex({1.f, 1.f}));
        vertexArray.append(sf::Vertex({2.f, 2.f}));
        CHECK(vertexArray.getBounds() == sf::FloatRect({1.f, 1.f}, {1.f, 1.f}));
        vertexArray[0] = sf::Vertex({0.f, 0.f});
        CHECK(vertexArray.getBounds() == sf::FloatRect({0.f, 0.f}, {2.f, 2.f}));
        vertexArray[0] = sf::Vertex({5.f, 5.f});
        CHECK(vertexArray.getBounds() == sf::FloatRect({2.f, 2.f}, {3.f, 3.f}));
        vertexArray.append(sf::Vertex({10.f, 10.f}));
        CHECK(vertexArray.getBounds() == sf::FloatRect({2.f, 2.f}, {8.f, 8.f}));
    }
}
