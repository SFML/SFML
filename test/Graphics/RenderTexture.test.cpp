#include <SFML/Graphics/RenderTexture.hpp>

#include <catch2/catch_test_macros.hpp>

#include <GraphicsUtil.hpp>
#include <type_traits>

TEST_CASE("[Graphics] sf::RenderTexture", runDisplayTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_constructible_v<sf::RenderTexture>);
        STATIC_CHECK(!std::is_copy_constructible_v<sf::RenderTexture>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::RenderTexture>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::RenderTexture>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::RenderTexture>);
    }

    SECTION("Move semantics")
    {
        SECTION("Move constructor")
        {
            sf::RenderTexture       moveRenderTexture;
            const sf::RenderTexture renderTexture(std::move(moveRenderTexture));
        }

        SECTION("Move assignment")
        {
            sf::RenderTexture moveRenderTexture;
            sf::RenderTexture renderTexture;
            renderTexture = std::move(moveRenderTexture);
        }
    }
}