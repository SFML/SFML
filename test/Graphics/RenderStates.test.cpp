#include <SFML/Graphics/RenderStates.hpp>

#include <catch2/catch_test_macros.hpp>

#include <GraphicsUtil.hpp>
#include <type_traits>

TEST_CASE("[Graphics] sf::RenderStates")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::RenderStates>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::RenderStates>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::RenderStates>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::RenderStates>);
    }

    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            const sf::RenderStates renderStates;
            CHECK(renderStates.blendMode == sf::BlendMode());
            CHECK(renderStates.stencilMode == sf::StencilMode{});
            CHECK(renderStates.transform == sf::Transform());
            CHECK(renderStates.coordinateType == sf::CoordinateType::Pixels);
            CHECK(renderStates.texture == nullptr);
            CHECK(renderStates.shader == nullptr);
        }

        SECTION("BlendMode constructor")
        {
            const sf::BlendMode    blendMode(sf::BlendMode::Factor::Zero,
                                          sf::BlendMode::Factor::SrcColor,
                                          sf::BlendMode::Equation::ReverseSubtract,
                                          sf::BlendMode::Factor::OneMinusDstAlpha,
                                          sf::BlendMode::Factor::DstAlpha,
                                          sf::BlendMode::Equation::Max);
            const sf::RenderStates renderStates(blendMode);
            CHECK(renderStates.blendMode == blendMode);
            CHECK(renderStates.stencilMode == sf::StencilMode{});
            CHECK(renderStates.transform == sf::Transform());
            CHECK(renderStates.coordinateType == sf::CoordinateType::Pixels);
            CHECK(renderStates.texture == nullptr);
            CHECK(renderStates.shader == nullptr);
        }

        SECTION("StencilMode constructor")
        {
            const sf::StencilMode stencilMode{sf::StencilComparison::Equal, sf::StencilUpdateOperation::Replace, 1, 0u, true};
            const sf::RenderStates renderStates(stencilMode);
            CHECK(renderStates.blendMode == sf::BlendMode());
            CHECK(renderStates.stencilMode == stencilMode);
            CHECK(renderStates.transform == sf::Transform());
            CHECK(renderStates.texture == nullptr);
            CHECK(renderStates.shader == nullptr);
        }

        SECTION("Transform constructor")
        {
            const sf::Transform    transform(10, 9, 8, 7, 6, 5, 4, 3, 2);
            const sf::RenderStates renderStates(transform);
            CHECK(renderStates.blendMode == sf::BlendMode());
            CHECK(renderStates.stencilMode == sf::StencilMode{});
            CHECK(renderStates.transform == transform);
            CHECK(renderStates.coordinateType == sf::CoordinateType::Pixels);
            CHECK(renderStates.texture == nullptr);
            CHECK(renderStates.shader == nullptr);
        }

        SECTION("Texture constructor")
        {
            const sf::Texture*     texture = nullptr;
            const sf::RenderStates renderStates(texture);
            CHECK(renderStates.blendMode == sf::BlendMode());
            CHECK(renderStates.stencilMode == sf::StencilMode{});
            CHECK(renderStates.transform == sf::Transform());
            CHECK(renderStates.coordinateType == sf::CoordinateType::Pixels);
            CHECK(renderStates.texture == texture);
            CHECK(renderStates.shader == nullptr);
        }

        SECTION("Shader constructor")
        {
            const sf::Shader*      shader = nullptr;
            const sf::RenderStates renderStates(shader);
            CHECK(renderStates.blendMode == sf::BlendMode());
            CHECK(renderStates.stencilMode == sf::StencilMode{});
            CHECK(renderStates.transform == sf::Transform());
            CHECK(renderStates.coordinateType == sf::CoordinateType::Pixels);
            CHECK(renderStates.texture == nullptr);
            CHECK(renderStates.shader == shader);
        }

        SECTION("Verbose constructor")
        {
            const sf::BlendMode blendMode(sf::BlendMode::Factor::One,
                                          sf::BlendMode::Factor::SrcColor,
                                          sf::BlendMode::Equation::ReverseSubtract,
                                          sf::BlendMode::Factor::OneMinusDstAlpha,
                                          sf::BlendMode::Factor::DstAlpha,
                                          sf::BlendMode::Equation::Max);
            const sf::StencilMode stencilMode{sf::StencilComparison::Equal, sf::StencilUpdateOperation::Replace, 1, 0u, true};
            const sf::Transform transform(10, 2, 3, 4, 50, 40, 30, 20, 10);
            const sf::RenderStates renderStates(blendMode, stencilMode, transform, sf::CoordinateType::Normalized, nullptr, nullptr);
            CHECK(renderStates.blendMode == blendMode);
            CHECK(renderStates.stencilMode == stencilMode);
            CHECK(renderStates.transform == transform);
            CHECK(renderStates.coordinateType == sf::CoordinateType::Normalized);
            CHECK(renderStates.texture == nullptr);
            CHECK(renderStates.shader == nullptr);
        }
    }

    SECTION("Default constant")
    {
        CHECK(sf::RenderStates::Default.blendMode == sf::BlendMode());
        CHECK(sf::RenderStates::Default.stencilMode == sf::StencilMode{});
        CHECK(sf::RenderStates::Default.transform == sf::Transform());
        CHECK(sf::RenderStates::Default.coordinateType == sf::CoordinateType::Pixels);
        CHECK(sf::RenderStates::Default.texture == nullptr);
        CHECK(sf::RenderStates::Default.shader == nullptr);
    }
}
