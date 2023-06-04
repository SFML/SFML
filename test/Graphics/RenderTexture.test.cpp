#include <SFML/Graphics/RenderTexture.hpp>

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
        SUBCASE("Move constructor")
        {
            sf::RenderTexture        moveRenderTexture;
            const sf::RenderTexture  RenderTexture(std::move(moveRenderTexture));
        }

        SUBCASE("Move assignment")
        {
            sf::RenderTexture        moveRenderTexture;
            const sf::RenderTexture  RenderTexture;
            moveRenderTexture = std::move(RenderTexture);
        }
    }
}