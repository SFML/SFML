#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include <doctest/doctest.h>

#include <GraphicsUtil.hpp>
#include <type_traits>

static_assert(!std::is_constructible_v<sf::Drawable>);
static_assert(!std::is_copy_constructible_v<sf::Drawable>);
static_assert(std::is_copy_assignable_v<sf::Drawable>);
static_assert(!std::is_nothrow_move_constructible_v<sf::Drawable>);
static_assert(std::is_nothrow_move_assignable_v<sf::Drawable>);
static_assert(std::is_abstract_v<sf::Drawable>);
static_assert(std::has_virtual_destructor_v<sf::Drawable>);

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

TEST_CASE("[Graphics] sf::Drawable" * doctest::skip(skipDisplayTests))
{
    SUBCASE("Construction")
    {
        const DrawableTest drawableTest;
        CHECK(drawableTest.callCount() == 0);
    }

    SUBCASE("draw()")
    {
        const DrawableTest drawableTest;
        sf::RenderTexture  renderTexture;
        CHECK(renderTexture.create({32, 32}));
        CHECK(drawableTest.callCount() == 0);
        renderTexture.draw(drawableTest);
        CHECK(drawableTest.callCount() == 1);
    }
}
