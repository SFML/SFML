#include <SFML/Graphics/Sprite.hpp>

#include <SFML/System/LifetimeTracking.hpp>

// Other 1st party headers
#include <SFML/Graphics/Texture.hpp>

#include <catch2/catch_test_macros.hpp>

#include <WindowUtil.hpp>
#include <optional>
#include <type_traits>

TEST_CASE("[Graphics] sf::Sprite", runDisplayTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_constructible_v<sf::Sprite, sf::Texture&&>);
        STATIC_CHECK(std::is_copy_constructible_v<sf::Sprite>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Sprite>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Sprite>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Sprite>);
    }

    const auto texture = sf::Texture::create({64, 64}).value();

    SECTION("Construction")
    {
        SECTION("Texture constructor")
        {
            const sf::Sprite sprite(texture);
            CHECK(&sprite.getTexture() == &texture);
            CHECK(sprite.getTextureRect() == sf::IntRect({}, {64, 64}));
            CHECK(sprite.getColor() == sf::Color::White);
            CHECK(sprite.getLocalBounds() == sf::FloatRect({}, {64, 64}));
            CHECK(sprite.getGlobalBounds() == sf::FloatRect({}, {64, 64}));
        }

        SECTION("Texture and rectangle constructor")
        {
            const sf::Sprite sprite(texture, {{0, 0}, {40, 60}});
            CHECK(&sprite.getTexture() == &texture);
            CHECK(sprite.getTextureRect() == sf::IntRect({0, 0}, {40, 60}));
            CHECK(sprite.getColor() == sf::Color::White);
            CHECK(sprite.getLocalBounds() == sf::FloatRect({0, 0}, {40, 60}));
            CHECK(sprite.getGlobalBounds() == sf::FloatRect({0, 0}, {40, 60}));
        }

        SECTION("Negative-size texture rectangle")
        {
            const sf::Sprite sprite(texture, {{0, 0}, {-40, -60}});
            CHECK(&sprite.getTexture() == &texture);
            CHECK(sprite.getTextureRect() == sf::IntRect({0, 0}, {-40, -60}));
            CHECK(sprite.getColor() == sf::Color::White);
            CHECK(sprite.getLocalBounds() == sf::FloatRect({0, 0}, {40, 60}));
            CHECK(sprite.getGlobalBounds() == sf::FloatRect({0, 0}, {40, 60}));
        }
    }

    SECTION("Set/get texture")
    {
        const sf::Texture otherTexture = sf::Texture::create({64, 64}).value();

        sf::Sprite sprite(texture);
        sprite.setTexture(otherTexture);
        CHECK(&sprite.getTexture() == &otherTexture);
    }

    SECTION("Set/get texture rect")
    {
        sf::Sprite sprite(texture);
        sprite.setTextureRect({{1, 2}, {3, 4}});
        CHECK(sprite.getTextureRect() == sf::IntRect({1, 2}, {3, 4}));
    }

    SECTION("Set/get color")
    {
        sf::Sprite sprite(texture);
        sprite.setColor(sf::Color::Red);
        CHECK(sprite.getColor() == sf::Color::Red);
    }

#ifdef SFML_ENABLE_LIFETIME_TRACKING
    SECTION("Lifetime tracking")
    {
        SECTION("Return local from function")
        {
            const auto badFunction = []
            {
                const auto localTexture = sf::Texture::create({64, 64}).value();
                return sf::Sprite(localTexture);
            };

            const sf::priv::LifetimeDependee::TestingModeGuard guard;
            CHECK(!guard.fatalErrorTriggered());

            badFunction();

            CHECK(guard.fatalErrorTriggered());
        }

        SECTION("Move struct holding both dependee and dependant")
        {
            struct BadStruct
            {
                sf::Texture memberTexture{sf::Texture::create({64, 64}).value()};
                sf::Sprite  memberSprite{memberTexture};
            };

            const sf::priv::LifetimeDependee::TestingModeGuard guard;
            CHECK(!guard.fatalErrorTriggered());

            std::optional<BadStruct> badStruct0;
            badStruct0.emplace();
            CHECK(!guard.fatalErrorTriggered());

            const BadStruct badStruct1 = std::move(badStruct0.value());
            CHECK(!guard.fatalErrorTriggered());

            badStruct0.reset();
            CHECK(guard.fatalErrorTriggered());
        }
    }
#endif
}
