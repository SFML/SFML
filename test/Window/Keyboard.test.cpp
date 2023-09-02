#include <SFML/Window/Keyboard.hpp>

// Other 1st party headers
#include <SFML/System/String.hpp>

#include <catch2/catch_test_macros.hpp>

#include <WindowUtil.hpp>

// We're limited on what can be tested. Without control over the hardware and the
// configuration of the operating system, certain things cannot be tested. In
// general, the mapping between keys and scancodes is a user configuration. Our
// tests cannot assume any particular configuration.
//
// Regardless this test case represents a best faith effort to cover some of this
// code in a way that is hopefully not prone to fail on different machines.

TEST_CASE("[Window] sf::Keyboard", runDisplayTests())
{
    SECTION("isKeyPressed(Key)")
    {
        CHECK(!sf::Keyboard::isKeyPressed(sf::Keyboard::W));
        CHECK(!sf::Keyboard::isKeyPressed(sf::Keyboard::A));
        CHECK(!sf::Keyboard::isKeyPressed(sf::Keyboard::S));
        CHECK(!sf::Keyboard::isKeyPressed(sf::Keyboard::D));
    }

    SECTION("isKeyPressed(Scancode)")
    {
        CHECK(!sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W));
        CHECK(!sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A));
        CHECK(!sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S));
        CHECK(!sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D));
    }

    SECTION("localize(Scancode)")
    {
        CHECK(sf::Keyboard::localize(sf::Keyboard::Scan::Space) == sf::Keyboard::Space);
    }

    SECTION("delocalize(Key)")
    {
        CHECK(sf::Keyboard::delocalize(sf::Keyboard::Space) == sf::Keyboard::Scan::Space);
    }

    SECTION("getDescription(Scancode)")
    {
        CHECK(sf::Keyboard::getDescription(sf::Keyboard::Scan::F1) == "F1");
        CHECK(sf::Keyboard::getDescription(sf::Keyboard::Scan::F2) == "F2");
        CHECK(sf::Keyboard::getDescription(sf::Keyboard::Scan::F3) == "F3");
        CHECK(sf::Keyboard::getDescription(sf::Keyboard::Scan::F4) == "F4");
        CHECK(sf::Keyboard::getDescription(sf::Keyboard::Scan::F5) == "F5");
        CHECK(sf::Keyboard::getDescription(sf::Keyboard::Scan::F6) == "F6");
        CHECK(sf::Keyboard::getDescription(sf::Keyboard::Scan::F7) == "F7");
        CHECK(sf::Keyboard::getDescription(sf::Keyboard::Scan::F8) == "F8");
        CHECK(sf::Keyboard::getDescription(sf::Keyboard::Scan::F9) == "F9");
        CHECK(sf::Keyboard::getDescription(sf::Keyboard::Scan::F10) == "F10");
        CHECK(sf::Keyboard::getDescription(sf::Keyboard::Scan::F11) == "F11");
        CHECK(sf::Keyboard::getDescription(sf::Keyboard::Scan::F12) == "F12");
    }
}
