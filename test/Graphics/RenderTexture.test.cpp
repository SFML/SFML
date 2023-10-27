#include <SFML/Graphics/RenderTexture.hpp>

#include <catch2/catch_test_macros.hpp>

#include <WindowUtil.hpp>
#include <type_traits>

TEST_CASE("[Graphics] sf::RenderTexture", runDisplayTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_copy_constructible_v<sf::RenderTexture>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::RenderTexture>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::RenderTexture>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::RenderTexture>);
    }

    SECTION("Construction")
    {
        const sf::RenderTexture renderTexture;
        CHECK(!renderTexture.isSmooth());
        CHECK(!renderTexture.isRepeated());
        CHECK(renderTexture.getSize() == sf::Vector2u(0, 0));
    }

    SECTION("create()")
    {
        sf::RenderTexture renderTexture;
        CHECK(!renderTexture.create({1'000'000, 1'000'000}));
        CHECK(renderTexture.create({480, 360}));
        CHECK(!renderTexture.isSmooth());
        CHECK(!renderTexture.isRepeated());
        CHECK(renderTexture.getSize() == sf::Vector2u(480, 360));
        CHECK(!renderTexture.isSrgb());
    }

    SECTION("getMaximumAntialiasingLevel()")
    {
        CHECK(sf::RenderTexture::getMaximumAntialiasingLevel() <= 64);
    }

    SECTION("Set/get smooth")
    {
        sf::RenderTexture renderTexture;
        renderTexture.setSmooth(true);
        CHECK(renderTexture.isSmooth());
    }

    SECTION("Set/get repeated")
    {
        sf::RenderTexture renderTexture;
        renderTexture.setRepeated(true);
        CHECK(renderTexture.isRepeated());
    }

    SECTION("generateMipmap()")
    {
        sf::RenderTexture renderTexture;
        CHECK(!renderTexture.generateMipmap());
        CHECK(renderTexture.create({480, 360}));
        CHECK(renderTexture.generateMipmap());
    }

    SECTION("setActive()")
    {
        sf::RenderTexture renderTexture;
        CHECK(!renderTexture.setActive());
        CHECK(renderTexture.create({480, 360}));
        CHECK(renderTexture.setActive());
    }

    SECTION("getTexture()")
    {
        sf::RenderTexture renderTexture;
        CHECK(renderTexture.getTexture().getSize() == sf::Vector2u(0, 0));
        CHECK(renderTexture.create({480, 360}));
        CHECK(renderTexture.getTexture().getSize() == sf::Vector2u(480, 360));
    }
}
