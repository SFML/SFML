#include <SFML/Graphics/RenderTarget.hpp>

#include <catch2/catch_test_macros.hpp>

#include <type_traits>

class RenderTargetTest : public sf::RenderTarget
{
public:
    Vector2u getSize() const override
    {
        return {0, 0};
    }

    bool isSrgb() const override
    {
        return true;
    }

    bool setActive(bool) override
    {
        return true;
    }
};

TEST_CASE("[Graphics] sf::RenderTarget")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_constructible_v<sf::RenderTarget>);
        STATIC_CHECK(!std::is_copy_constructible_v<sf::RenderTarget>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::RenderTarget>);
        STATIC_CHECK(!std::is_nothrow_move_constructible_v<sf::RenderTarget>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::RenderTarget>);
    }

    SECTION("Move semantics")
    {
        SECTION("Move constructor")
        {
            RenderTargetTest       moveRenderTarget;
            const RenderTargetTest renderTarget(std::move(moveRenderTarget));
        }

        SECTION("Move assignment")
        {
            RenderTargetTest moveRenderTarget;
            RenderTargetTest renderTarget;
            renderTarget = std::move(moveRenderTarget);
        }
    }
}