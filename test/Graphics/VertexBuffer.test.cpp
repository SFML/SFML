#include <SFML/Graphics/VertexBuffer.hpp>

// Other first party headers
#include <SFML/Graphics/Vertex.hpp>

#include <doctest/doctest.h>

#include <GraphicsUtil.hpp>
#include <array>
#include <type_traits>

static_assert(std::is_copy_constructible_v<sf::VertexBuffer>);
static_assert(std::is_copy_assignable_v<sf::VertexBuffer>);
static_assert(std::is_move_constructible_v<sf::VertexBuffer>);
static_assert(!std::is_nothrow_move_constructible_v<sf::VertexBuffer>);
static_assert(std::is_move_assignable_v<sf::VertexBuffer>);
static_assert(!std::is_nothrow_move_assignable_v<sf::VertexBuffer>);
static_assert(std::is_nothrow_swappable_v<sf::VertexBuffer>);

// Skip these tests because they produce flakey failures in CI when using xvfb-run
TEST_CASE("[Graphics] sf::VertexBuffer" * doctest::skip(true))
{
    // Skip tests if vertex buffers aren't available
    if (!sf::VertexBuffer::isAvailable())
        return;

    SUBCASE("Construction")
    {
        SUBCASE("Default constructor")
        {
            const sf::VertexBuffer vertexBuffer;
            CHECK(vertexBuffer.getVertexCount() == 0);
            CHECK(vertexBuffer.getNativeHandle() == 0);
            CHECK(vertexBuffer.getPrimitiveType() == sf::PrimitiveType::Points);
            CHECK(vertexBuffer.getUsage() == sf::VertexBuffer::Stream);
        }

        SUBCASE("Primitive type constructor")
        {
            const sf::VertexBuffer vertexBuffer(sf::PrimitiveType::Triangles);
            CHECK(vertexBuffer.getVertexCount() == 0);
            CHECK(vertexBuffer.getNativeHandle() == 0);
            CHECK(vertexBuffer.getPrimitiveType() == sf::PrimitiveType::Triangles);
            CHECK(vertexBuffer.getUsage() == sf::VertexBuffer::Stream);
        }

        SUBCASE("Usage constructor")
        {
            const sf::VertexBuffer vertexBuffer(sf::VertexBuffer::Static);
            CHECK(vertexBuffer.getVertexCount() == 0);
            CHECK(vertexBuffer.getNativeHandle() == 0);
            CHECK(vertexBuffer.getPrimitiveType() == sf::PrimitiveType::Points);
            CHECK(vertexBuffer.getUsage() == sf::VertexBuffer::Static);
        }

        SUBCASE("Primitive type and usage constructor")
        {
            const sf::VertexBuffer vertexBuffer(sf::PrimitiveType::LineStrip, sf::VertexBuffer::Dynamic);
            CHECK(vertexBuffer.getVertexCount() == 0);
            CHECK(vertexBuffer.getNativeHandle() == 0);
            CHECK(vertexBuffer.getPrimitiveType() == sf::PrimitiveType::LineStrip);
            CHECK(vertexBuffer.getUsage() == sf::VertexBuffer::Dynamic);
        }
    }

    SUBCASE("Copy semantics")
    {
        const sf::VertexBuffer vertexBuffer(sf::PrimitiveType::LineStrip, sf::VertexBuffer::Dynamic);

        SUBCASE("Construction")
        {
            const sf::VertexBuffer vertexBufferCopy(vertexBuffer); // NOLINT(performance-unnecessary-copy-initialization)
            CHECK(vertexBufferCopy.getVertexCount() == 0);
            CHECK(vertexBufferCopy.getNativeHandle() == 0);
            CHECK(vertexBufferCopy.getPrimitiveType() == sf::PrimitiveType::LineStrip);
            CHECK(vertexBufferCopy.getUsage() == sf::VertexBuffer::Dynamic);
        }

        SUBCASE("Assignment")
        {
            sf::VertexBuffer vertexBufferCopy;
            vertexBufferCopy = vertexBuffer;
            CHECK(vertexBufferCopy.getVertexCount() == 0);
            CHECK(vertexBufferCopy.getNativeHandle() == 0);
            CHECK(vertexBufferCopy.getPrimitiveType() == sf::PrimitiveType::LineStrip);
            CHECK(vertexBufferCopy.getUsage() == sf::VertexBuffer::Dynamic);
        }
    }

    SUBCASE("create()")
    {
        sf::VertexBuffer vertexBuffer;
        CHECK(vertexBuffer.create(100));
        CHECK(vertexBuffer.getVertexCount() == 100);
    }

    SUBCASE("update()")
    {
        sf::VertexBuffer            vertexBuffer;
        std::array<sf::Vertex, 128> vertices{};

        SUBCASE("Vertices")
        {
            SUBCASE("Uninitialized buffer")
            {
                CHECK(!vertexBuffer.update(vertices.data()));
            }

            CHECK(vertexBuffer.create(128));

            SUBCASE("Null vertices")
            {
                CHECK(!vertexBuffer.update(nullptr));
            }

            CHECK(vertexBuffer.update(vertices.data()));
            CHECK(vertexBuffer.getVertexCount() == 128);
            CHECK(vertexBuffer.getNativeHandle() != 0);
        }

        SUBCASE("Vertices, count, and offset")
        {
            CHECK(vertexBuffer.create(128));

            SUBCASE("Count + offset too large")
            {
                CHECK(!vertexBuffer.update(vertices.data(), 100, 100));
            }

            CHECK(vertexBuffer.update(vertices.data(), 128, 0));
            CHECK(vertexBuffer.getVertexCount() == 128);
        }

        SUBCASE("Another buffer")
        {
            sf::VertexBuffer otherVertexBuffer;

            CHECK(!vertexBuffer.update(otherVertexBuffer));
            CHECK(otherVertexBuffer.create(42));
            CHECK(!vertexBuffer.update(otherVertexBuffer));
        }
    }

    SUBCASE("swap()")
    {
        sf::VertexBuffer vertexBuffer1(sf::PrimitiveType::LineStrip, sf::VertexBuffer::Dynamic);
        CHECK(vertexBuffer1.create(50));

        sf::VertexBuffer vertexBuffer2(sf::PrimitiveType::TriangleStrip, sf::VertexBuffer::Stream);
        CHECK(vertexBuffer2.create(60));

        std::swap(vertexBuffer1, vertexBuffer2);

        CHECK(vertexBuffer1.getVertexCount() == 60);
        CHECK(vertexBuffer1.getNativeHandle() != 0);
        CHECK(vertexBuffer1.getPrimitiveType() == sf::PrimitiveType::TriangleStrip);
        CHECK(vertexBuffer1.getUsage() == sf::VertexBuffer::Stream);

        CHECK(vertexBuffer2.getVertexCount() == 50);
        CHECK(vertexBuffer2.getNativeHandle() != 0);
        CHECK(vertexBuffer2.getPrimitiveType() == sf::PrimitiveType::LineStrip);
        CHECK(vertexBuffer2.getUsage() == sf::VertexBuffer::Dynamic);
    }

    SUBCASE("Set/get primitive type")
    {
        sf::VertexBuffer vertexBuffer;
        vertexBuffer.setPrimitiveType(sf::PrimitiveType::TriangleFan);
        CHECK(vertexBuffer.getPrimitiveType() == sf::PrimitiveType::TriangleFan);
    }

    SUBCASE("Set/get usage")
    {
        sf::VertexBuffer vertexBuffer;
        vertexBuffer.setUsage(sf::VertexBuffer::Dynamic);
        CHECK(vertexBuffer.getUsage() == sf::VertexBuffer::Dynamic);
    }
}
