#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include <catch2/catch_test_macros.hpp>

#include <GraphicsUtil.hpp>
#include <type_traits>

class DrawableTest : public sf::Drawable
{
public:
    int callCount() const
    {
        return m_callCount;
    }

private:
    void draw(sf::RenderTarget&, const sf::RenderStates&) const override
    {
        ++m_callCount;
    }

    mutable int m_callCount{};
};

TEST_CASE("[Graphics] sf::Drawable", runDisplayTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_constructible_v<sf::Drawable>);
        STATIC_CHECK(!std::is_copy_constructible_v<sf::Drawable>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Drawable>);
        STATIC_CHECK(!std::is_nothrow_move_constructible_v<sf::Drawable>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Drawable>);
        STATIC_CHECK(std::is_abstract_v<sf::Drawable>);
        STATIC_CHECK(std::has_virtual_destructor_v<sf::Drawable>);
    }

    SECTION("Construction")
    {
        const DrawableTest drawableTest;
        CHECK(drawableTest.callCount() == 0);
    }

    SECTION("draw()")
    {
        const DrawableTest drawableTest;
        sf::RenderTexture  renderTexture;
        CHECK(renderTexture.create({32, 32}));
        CHECK(drawableTest.callCount() == 0);
        renderTexture.draw(drawableTest);
        CHECK(drawableTest.callCount() == 1);
    }
}
