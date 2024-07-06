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

        CHECK(!sf::Cursor::createFromPixels(nullptr, {}, {}));
        CHECK(!sf::Cursor::createFromPixels(pixels.data(), {0, 1}, {}));
        CHECK(!sf::Cursor::createFromPixels(pixels.data(), {1, 0}, {}));
        CHECK(sf::Cursor::createFromPixels(pixels.data(), {1, 1}, {}));
    }

    SECTION("loadFromSystem()")
    {
        CHECK(sf::Cursor::createFromSystem(sf::Cursor::Type::Hand));
        CHECK(sf::Cursor::createFromSystem(sf::Cursor::Type::SizeHorizontal));
        CHECK(sf::Cursor::createFromSystem(sf::Cursor::Type::SizeVertical));
        CHECK(sf::Cursor::createFromSystem(sf::Cursor::Type::SizeLeft));
        CHECK(sf::Cursor::createFromSystem(sf::Cursor::Type::SizeRight));
        CHECK(sf::Cursor::createFromSystem(sf::Cursor::Type::SizeTop));
        CHECK(sf::Cursor::createFromSystem(sf::Cursor::Type::SizeBottom));
        CHECK(sf::Cursor::createFromSystem(sf::Cursor::Type::SizeTopLeft));
        CHECK(sf::Cursor::createFromSystem(sf::Cursor::Type::SizeTopRight));
        CHECK(sf::Cursor::createFromSystem(sf::Cursor::Type::SizeBottomLeft));
        CHECK(sf::Cursor::createFromSystem(sf::Cursor::Type::SizeBottomRight));
        CHECK(sf::Cursor::createFromSystem(sf::Cursor::Type::Cross));
        CHECK(sf::Cursor::createFromSystem(sf::Cursor::Type::Help));
        CHECK(sf::Cursor::createFromSystem(sf::Cursor::Type::NotAllowed));
    }
}
