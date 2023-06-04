#include <SFML/Graphics/RenderTarget.hpp>

#include <catch2/catch_test_macros.hpp>

#include <type_traits>

TEST_CASE("[Graphics] sf::RenderTarget")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_constructible_v<sf::RenderTarget>);
        STATIC_CHECK(!std::is_copy_constructible_v<sf::RenderTarget>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::RenderTarget>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::RenderTarget>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::RenderTarget>);
    }

    SECTION("Move semantics")
    {
        SECTION("Move constructor")
        {
            sf::RenderTarget       moveRenderTarget;
            const sf::RenderTarget renderTarget(std::move(moveRenderTarget));
        }

        SECTION("Move assignment")
        {
            sf::RenderTarget       moveRenderTarget;
            const sf::RenderTarget renderTarget;
            moveRenderTarget = std::move(renderTarget);
        }
    }
}