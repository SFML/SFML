#include <SFML/Window/Cursor.hpp>

#include <catch2/catch_test_macros.hpp>

#include <WindowUtil.hpp>
#include <array>
#include <type_traits>

TEST_CASE("[Window] sf::Cursor", runDisplayTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_copy_constructible_v<sf::Cursor>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::Cursor>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Cursor>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Cursor>);
    }

    SECTION("Construction")
    {
        const sf::Cursor cursor;
    }

    SECTION("Move semantics")
    {
        SECTION("Construction")
        {
            sf::Cursor       movedCursor;
            const sf::Cursor cursor(std::move(movedCursor));
        }

        SECTION("Assignment")
        {
            sf::Cursor movedCursor;
            sf::Cursor cursor;
            cursor = std::move(movedCursor);
        }
    }

    SECTION("loadFromPixels()")
    {
        sf::Cursor                  cursor;
        std::array<std::uint8_t, 4> pixels{};

        CHECK(!cursor.loadFromPixels(nullptr, {}, {}));
        CHECK(!cursor.loadFromPixels(pixels.data(), {0, 1}, {}));
        CHECK(!cursor.loadFromPixels(pixels.data(), {1, 0}, {}));
        CHECK(cursor.loadFromPixels(pixels.data(), {1, 1}, {}));
    }

    SECTION("loadFromSystem()")
    {
        sf::Cursor cursor;
        CHECK(cursor.loadFromSystem(sf::Cursor::Hand));
        CHECK(cursor.loadFromSystem(sf::Cursor::SizeHorizontal));
        CHECK(cursor.loadFromSystem(sf::Cursor::SizeVertical));
        CHECK(cursor.loadFromSystem(sf::Cursor::SizeLeft));
        CHECK(cursor.loadFromSystem(sf::Cursor::SizeRight));
        CHECK(cursor.loadFromSystem(sf::Cursor::SizeTop));
        CHECK(cursor.loadFromSystem(sf::Cursor::SizeBottom));
        CHECK(cursor.loadFromSystem(sf::Cursor::SizeTopLeft));
        CHECK(cursor.loadFromSystem(sf::Cursor::SizeTopRight));
        CHECK(cursor.loadFromSystem(sf::Cursor::SizeBottomLeft));
        CHECK(cursor.loadFromSystem(sf::Cursor::SizeBottomRight));
        CHECK(cursor.loadFromSystem(sf::Cursor::Cross));
        CHECK(cursor.loadFromSystem(sf::Cursor::Help));
        CHECK(cursor.loadFromSystem(sf::Cursor::NotAllowed));
    }
}
