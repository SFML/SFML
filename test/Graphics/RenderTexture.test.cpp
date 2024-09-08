#include <SFML/Graphics/RenderTexture.hpp>

// Other 1st party headers
#include <SFML/System/Exception.hpp>

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
        SECTION("Default constructor")
        {
            const sf::RenderTexture renderTexture;
            CHECK(!renderTexture.isSmooth());
            CHECK(!renderTexture.isRepeated());
            CHECK(renderTexture.getSize() == sf::Vector2u(0, 0));
        }

        SECTION("2 parameter constructor")
        {
            CHECK_THROWS_AS(sf::RenderTexture({1'000'000, 1'000'000}), sf::Exception);

            CHECK_NOTHROW(sf::RenderTexture({100, 100}, sf::ContextSettings{8 /* depthBits */, 0 /* stencilBits */}));
            CHECK_NOTHROW(sf::RenderTexture({100, 100}, sf::ContextSettings{0 /* depthBits */, 8 /* stencilBits */}));

            const sf::RenderTexture renderTexture({360, 480});
            CHECK(renderTexture.getSize() == sf::Vector2u(360, 480));
            CHECK(!renderTexture.isSmooth());
            CHECK(!renderTexture.isRepeated());
            CHECK(!renderTexture.isSrgb());

            const auto& texture = renderTexture.getTexture();
            CHECK(texture.getSize() == sf::Vector2u(360, 480));
            CHECK(!texture.isSmooth());
            CHECK(!texture.isSrgb());
            CHECK(!texture.isRepeated());
            CHECK(texture.getNativeHandle() != 0);
        }
    }

    SECTION("resize()")
    {
        sf::RenderTexture renderTexture;
        CHECK(!renderTexture.resize({1'000'000, 1'000'000}));

        CHECK(renderTexture.resize({100, 100}, sf::ContextSettings{8 /* depthBits */, 0 /* stencilBits */}));
        CHECK(renderTexture.resize({100, 100}, sf::ContextSettings{0 /* depthBits */, 8 /* stencilBits */}));

        REQUIRE(renderTexture.resize({360, 480}));
        CHECK(renderTexture.getSize() == sf::Vector2u(360, 480));
        CHECK(!renderTexture.isSmooth());
        CHECK(!renderTexture.isRepeated());
        CHECK(!renderTexture.isSrgb());

        const auto& texture = renderTexture.getTexture();
        CHECK(texture.getSize() == sf::Vector2u(360, 480));
        CHECK(!texture.isSmooth());
        CHECK(!texture.isSrgb());
        CHECK(!texture.isRepeated());
        CHECK(texture.getNativeHandle() != 0);
    }

    SECTION("resize()")
    {
        sf::RenderTexture renderTexture;
        CHECK(!renderTexture.resize({1'000'000, 1'000'000}));
        CHECK(renderTexture.resize({480, 360}));
        CHECK(!renderTexture.isSmooth());
        CHECK(!renderTexture.isRepeated());
        CHECK(renderTexture.getSize() == sf::Vector2u(480, 360));
        CHECK(!renderTexture.isSrgb());
        CHECK(renderTexture.resize({360, 480}));
        CHECK(renderTexture.getSize() == sf::Vector2u(360, 480));
        CHECK(renderTexture.resize({100, 100}, sf::ContextSettings{8 /* depthBits */, 0 /* stencilBits */}));
        CHECK(renderTexture.resize({100, 100}, sf::ContextSettings{0 /* depthBits */, 8 /* stencilBits */}));
    }

    SECTION("getMaximumAntiAliasingLevel()")
    {
        CHECK(sf::RenderTexture::getMaximumAntiAliasingLevel() <= 64);
    }

    SECTION("Set/get smooth")
    {
        sf::RenderTexture renderTexture({64, 64});
        renderTexture.setSmooth(true);
        CHECK(renderTexture.isSmooth());
    }

    SECTION("Set/get repeated")
    {
        sf::RenderTexture renderTexture({64, 64});
        renderTexture.setRepeated(true);
        CHECK(renderTexture.isRepeated());
    }

    SECTION("generateMipmap()")
    {
        sf::RenderTexture renderTexture({64, 64});
        CHECK(renderTexture.generateMipmap());
    }

    SECTION("setActive()")
    {
        sf::RenderTexture renderTexture({64, 64});
        CHECK(renderTexture.setActive());
        CHECK(renderTexture.setActive(false));
        CHECK(renderTexture.setActive(true));
    }

    SECTION("getTexture()")
    {
        const sf::RenderTexture renderTexture({64, 64});
        CHECK(renderTexture.getTexture().getSize() == sf::Vector2u(64, 64));
    }
}
