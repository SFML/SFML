#include <SFML/Window/Cursor.hpp>

// Other 1st party headers
#include <SFML/System/Exception.hpp>

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

    SECTION("Constructor")
    {
        SECTION("Pixels")
        {
            static constexpr std::array<std::uint8_t, 4> pixels{};

            CHECK_THROWS_AS(sf::Cursor(nullptr, {}, {}), sf::Exception);
            CHECK_THROWS_AS(sf::Cursor(pixels.data(), {0, 1}, {}), sf::Exception);
            CHECK_THROWS_AS(sf::Cursor(pixels.data(), {1, 0}, {}), sf::Exception);
            CHECK_NOTHROW(sf::Cursor(pixels.data(), {1, 1}, {}));
        }

        SECTION("System")
        {
            CHECK_NOTHROW(sf::Cursor(sf::Cursor::Type::Hand));
            CHECK_NOTHROW(sf::Cursor(sf::Cursor::Type::SizeHorizontal));
            CHECK_NOTHROW(sf::Cursor(sf::Cursor::Type::SizeVertical));
            CHECK_NOTHROW(sf::Cursor(sf::Cursor::Type::SizeLeft));
            CHECK_NOTHROW(sf::Cursor(sf::Cursor::Type::SizeRight));
            CHECK_NOTHROW(sf::Cursor(sf::Cursor::Type::SizeTop));
            CHECK_NOTHROW(sf::Cursor(sf::Cursor::Type::SizeBottom));
            CHECK_NOTHROW(sf::Cursor(sf::Cursor::Type::SizeTopLeft));
            CHECK_NOTHROW(sf::Cursor(sf::Cursor::Type::SizeTopRight));
            CHECK_NOTHROW(sf::Cursor(sf::Cursor::Type::SizeBottomLeft));
            CHECK_NOTHROW(sf::Cursor(sf::Cursor::Type::SizeBottomRight));
            CHECK_NOTHROW(sf::Cursor(sf::Cursor::Type::Cross));
            CHECK_NOTHROW(sf::Cursor(sf::Cursor::Type::Help));
            CHECK_NOTHROW(sf::Cursor(sf::Cursor::Type::NotAllowed));
        }
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
