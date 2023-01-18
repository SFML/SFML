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
            CHECK(renderStates.transform == sf::Transform());
            CHECK(renderStates.texture == nullptr);
            CHECK(renderStates.shader == nullptr);
        }

        SECTION("BlendMode constructor")
        {
            const sf::BlendMode    blendMode(sf::BlendMode::Zero,
                                          sf::BlendMode::SrcColor,
                                          sf::BlendMode::ReverseSubtract,
                                          sf::BlendMode::OneMinusDstAlpha,
                                          sf::BlendMode::DstAlpha,
                                          sf::BlendMode::Max);
            const sf::RenderStates renderStates(blendMode);
            CHECK(renderStates.blendMode == blendMode);
            CHECK(renderStates.transform == sf::Transform());
            CHECK(renderStates.texture == nullptr);
            CHECK(renderStates.shader == nullptr);
        }

        SECTION("Transform constructor")
        {
            const sf::Transform    transform(10, 9, 8, 7, 6, 5, 4, 3, 2);
            const sf::RenderStates renderStates(transform);
            CHECK(renderStates.blendMode == sf::BlendMode());
            CHECK(renderStates.transform == transform);
            CHECK(renderStates.texture == nullptr);
            CHECK(renderStates.shader == nullptr);
        }

        SECTION("Texture constructor")
        {
            const sf::Texture*     texture = nullptr;
            const sf::RenderStates renderStates(texture);
            CHECK(renderStates.blendMode == sf::BlendMode());
            CHECK(renderStates.transform == sf::Transform());
            CHECK(renderStates.texture == texture);
            CHECK(renderStates.shader == nullptr);
        }

        SECTION("Shader constructor")
        {
            const sf::Shader*      shader = nullptr;
            const sf::RenderStates renderStates(shader);
            CHECK(renderStates.blendMode == sf::BlendMode());
            CHECK(renderStates.transform == sf::Transform());
            CHECK(renderStates.texture == nullptr);
            CHECK(renderStates.shader == shader);
        }

        SECTION("Verbose constructor")
        {
            const sf::BlendMode    blendMode(sf::BlendMode::One,
                                          sf::BlendMode::SrcColor,
                                          sf::BlendMode::ReverseSubtract,
                                          sf::BlendMode::OneMinusDstAlpha,
                                          sf::BlendMode::DstAlpha,
                                          sf::BlendMode::Max);
            const sf::Transform    transform(10, 2, 3, 4, 50, 40, 30, 20, 10);
            const sf::RenderStates renderStates(blendMode, transform, nullptr, nullptr);
            CHECK(renderStates.blendMode == blendMode);
            CHECK(renderStates.transform == transform);
            CHECK(renderStates.texture == nullptr);
            CHECK(renderStates.shader == nullptr);
        }
    }

    SECTION("Default constant")
    {
        CHECK(sf::RenderStates::Default.blendMode == sf::BlendMode());
        CHECK(sf::RenderStates::Default.transform == sf::Transform());
        CHECK(sf::RenderStates::Default.texture == nullptr);
        CHECK(sf::RenderStates::Default.shader == nullptr);
    }
}
