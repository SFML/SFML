#include <SFML/Graphics/Shader.hpp>

#include <catch2/catch_test_macros.hpp>

#include <type_traits>

namespace
{
constexpr auto vertexSource = R"(
uniform vec2 storm_position;
uniform float storm_total_radius;
uniform float storm_inner_radius;

void main()
{
    vec4 vertex = gl_ModelViewMatrix * gl_Vertex;
    vec2 offset = vertex.xy - storm_position;
    float len = length(offset);
    if (len < storm_total_radius)
    {
        float push_distance = storm_inner_radius + len / storm_total_radius * (storm_total_radius - storm_inner_radius);
        vertex.xy = storm_position + normalize(offset) * push_distance;
    }

    gl_Position = gl_ProjectionMatrix * vertex;
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;
}
)";

constexpr auto geometrySource = R"(
#version 150

// The render target's resolution (used for scaling)
uniform vec2 resolution;

// The billboards' size
uniform vec2 size;

// Input is the passed point cloud
layout (points) in;

// The output will consist of triangle strips with four vertices each
layout (triangle_strip, max_vertices = 4) out;

// Output texture coordinates
out vec2 tex_coord;

// Main entry point
void main()
{
    // Calculate the half width/height of the billboards
    vec2 half_size = size / 2.f;

    // Scale the size based on resolution (1 would be full width/height)
    half_size /= resolution;

    // Iterate over all vertices
    for (int i = 0; i < gl_in.length(); ++i)
    {
        // Retrieve the passed vertex position
        vec2 pos = gl_in[i].gl_Position.xy;

        // Bottom left vertex
        gl_Position = vec4(pos - half_size, 0.f, 1.f);
        tex_coord = vec2(1.f, 1.f);
        EmitVertex();

        // Bottom right vertex
        gl_Position = vec4(pos.x + half_size.x, pos.y - half_size.y, 0.f, 1.f);
        tex_coord = vec2(0.f, 1.f);
        EmitVertex();

        // Top left vertex
        gl_Position = vec4(pos.x - half_size.x, pos.y + half_size.y, 0.f, 1.f);
        tex_coord = vec2(1.f, 0.f);
        EmitVertex();

        // Top right vertex
        gl_Position = vec4(pos + half_size, 0.f, 1.f);
        tex_coord = vec2(0.f, 0.f);
        EmitVertex();

        // And finalize the primitive
        EndPrimitive();
    }
}
)";

constexpr auto fragmentSource = R"(
uniform sampler2D texture;
uniform float blink_alpha;

void main()
{
    vec4 pixel = gl_Color;
    pixel.a = blink_alpha;
    gl_FragColor = pixel;
}
)";

#ifdef SFML_RUN_DISPLAY_TESTS
#ifdef SFML_OPENGL_ES
constexpr bool skipShaderDummyTest = false;
constexpr bool skipShaderFullTest  = true;
#else
constexpr bool skipShaderDummyTest = true;
constexpr bool skipShaderFullTest  = false;
#endif
#else
constexpr bool skipShaderDummyTest = true;
constexpr bool skipShaderFullTest  = true;
#endif

std::string skipShaderDummyTests()
{
    if constexpr (skipShaderDummyTest)
        // https://github.com/catchorg/Catch2/blob/devel/docs/test-cases-and-sections.md#special-tags
        // This tag tells Catch2 to not run a given TEST_CASE
        return "[.shaderDummy]";
    else
        return "";
}

std::string skipShaderFullTests()
{
    if constexpr (skipShaderFullTest)
        // https://github.com/catchorg/Catch2/blob/devel/docs/test-cases-and-sections.md#special-tags
        // This tag tells Catch2 to not run a given TEST_CASE
        return "[.shaderFull]";
    else
        return "";
}

} // namespace

TEST_CASE("[Graphics] sf::Shader (Dummy Implementation)", skipShaderDummyTests())
{
    SECTION("Available")
    {
        CHECK_FALSE(sf::Shader::isAvailable());
        CHECK_FALSE(sf::Shader::isGeometryAvailable());
    }

    SECTION("Load")
    {
        sf::Shader shader;
        CHECK_FALSE(shader.loadFromMemory(vertexSource, sf::Shader::Type::Vertex));
        CHECK_FALSE(shader.loadFromMemory(geometrySource, sf::Shader::Type::Geometry));
        CHECK_FALSE(shader.loadFromMemory(fragmentSource, sf::Shader::Type::Fragment));
        CHECK_FALSE(shader.loadFromMemory(vertexSource, fragmentSource));
        CHECK_FALSE(shader.loadFromMemory(vertexSource, geometrySource, fragmentSource));
    }
}

TEST_CASE("[Graphics] sf::Shader", skipShaderFullTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_copy_constructible_v<sf::Shader>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::Shader>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Shader>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Shader>);
    }

    if (!sf::Shader::isAvailable())
        return;

    SECTION("Load")
    {
        sf::Shader shader;
        CHECK(shader.loadFromMemory(vertexSource, sf::Shader::Type::Vertex));
        CHECK_FALSE(shader.loadFromMemory(geometrySource, sf::Shader::Type::Geometry));
        CHECK(shader.loadFromMemory(fragmentSource, sf::Shader::Type::Fragment));
        CHECK(shader.loadFromMemory(vertexSource, fragmentSource));
        CHECK(sf::Shader::isGeometryAvailable() == shader.loadFromMemory(vertexSource, geometrySource, fragmentSource));
        CHECK(shader.getNativeHandle() != 0);
    }
}
