#include <SFML/Window/Clipboard.hpp>

// Other 1st party headers
#include <SFML/System/String.hpp>

#include <catch2/catch_test_macros.hpp>

#include <WindowUtil.hpp>

TEST_CASE("[Window] sf::Clipboard", runDisplayTests())
{
    // Capture current clipboard state
    const auto currentClipboard = sf::Clipboard::getString();

    sf::String string;

    SECTION("ASCII")
    {
        string = "Snail";
    }

    SECTION("Latin1")
    {
        string = U"Limacé";
    }

    SECTION("Basic Multilingual Plane")
    {
        string = U"カタツムリ";
    }

    SECTION("Emoji")
    {
        string = U"🐌";
    }

    INFO("String: " << reinterpret_cast<const char*>(string.toUtf8().c_str()));
    sf::Clipboard::setString(string);
    CHECK(sf::Clipboard::getString() == string);

    // Restore clipboard
    sf::Clipboard::setString(currentClipboard);

    // We rely on getString triggering clipboard event processing on X11 to make
    // setString work, but note that the way setString is guaranteed to work is
    // by having an open window for which events are being handled.
    CHECK(sf::Clipboard::getString() == currentClipboard);
}
