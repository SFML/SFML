#include <SFML/Window/Cursor.hpp>

#include <catch2/catch_test_macros.hpp>

#include <WindowUtil.hpp>
#include <array>
#include <type_traits>

TEST_CASE("[Window] sf::Cursor", runDisplayTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_default_constructible_v<sf::Cursor>);
        STATIC_CHECK(!std::is_copy_constructible_v<sf::Cursor>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::Cursor>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Cursor>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Cursor>);
    }

    SECTION("loadFromPixels()")
    {
        static constexpr std::array<std::uint8_t, 4> pixels{};

        CHECK(!sf::Cursor::loadFromPixels(nullptr, {}, {}));
        CHECK(!sf::Cursor::loadFromPixels(pixels.data(), {0, 1}, {}));
        CHECK(!sf::Cursor::loadFromPixels(pixels.data(), {1, 0}, {}));
        CHECK(sf::Cursor::loadFromPixels(pixels.data(), {1, 1}, {}));
    }

    SECTION("loadFromSystem()")
    {
        CHECK(sf::Cursor::loadFromSystem(sf::Cursor::Type::Hand));
        CHECK(sf::Cursor::loadFromSystem(sf::Cursor::Type::SizeHorizontal));
        CHECK(sf::Cursor::loadFromSystem(sf::Cursor::Type::SizeVertical));
        CHECK(sf::Cursor::loadFromSystem(sf::Cursor::Type::SizeLeft));
        CHECK(sf::Cursor::loadFromSystem(sf::Cursor::Type::SizeRight));
        CHECK(sf::Cursor::loadFromSystem(sf::Cursor::Type::SizeTop));
        CHECK(sf::Cursor::loadFromSystem(sf::Cursor::Type::SizeBottom));
        CHECK(sf::Cursor::loadFromSystem(sf::Cursor::Type::SizeTopLeft));
        CHECK(sf::Cursor::loadFromSystem(sf::Cursor::Type::SizeTopRight));
        CHECK(sf::Cursor::loadFromSystem(sf::Cursor::Type::SizeBottomLeft));
        CHECK(sf::Cursor::loadFromSystem(sf::Cursor::Type::SizeBottomRight));
        CHECK(sf::Cursor::loadFromSystem(sf::Cursor::Type::Cross));
        CHECK(sf::Cursor::loadFromSystem(sf::Cursor::Type::Help));
        CHECK(sf::Cursor::loadFromSystem(sf::Cursor::Type::NotAllowed));
    }
}
