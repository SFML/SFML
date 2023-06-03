#include <SFML/Graphics/RenderTarget.hpp>

#include <type_traits>

TEST_CASE("[Graphics] sf::RenderTarget", runDisplayTests())
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
        SUBCASE("Move constructor")
        {
            sf::RenderTarget        moveRenderTarget;
            const sf::RenderTarget  renderTarget(std::move(moveRenderTarget));
        }

        SUBCASE("Move assignment")
        {
            sf::RenderTarget        moveRenderTarget;
            const sf::RenderTarget  renderTarget;
            moveRenderTarget = std::move(renderTarget);
        }
    }
}