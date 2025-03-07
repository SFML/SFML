#include <SFML/Window/Window.hpp>

// Other 1st party headers
#include <SFML/Window/VideoMode.hpp>

#include <SFML/System/String.hpp>

#include <catch2/catch_test_macros.hpp>

#include <WindowUtil.hpp>
#include <type_traits>

TEST_CASE("[Window] sf::Window", runDisplayTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::has_virtual_destructor_v<sf::Window>);
        STATIC_CHECK(!std::is_copy_constructible_v<sf::Window>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::Window>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Window>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Window>);
    }

    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            const sf::Window window;
            CHECK(!window.isOpen());
            CHECK(window.getPosition() == sf::Vector2i());
            CHECK(window.getSize() == sf::Vector2u());
            CHECK(!window.hasFocus());
            CHECK(window.getNativeHandle() == sf::WindowHandle());
            CHECK(window.getSettings().depthBits == 0);
            CHECK(window.getSettings().stencilBits == 0);
            CHECK(window.getSettings().antiAliasingLevel == 0);
            CHECK(window.getSettings().majorVersion == 1);
            CHECK(window.getSettings().minorVersion == 1);
            CHECK(window.getSettings().attributeFlags == sf::ContextSettings::Default);
            CHECK(!window.getSettings().sRgbCapable);
        }

        SECTION("Mode and title constructor")
        {
            const sf::Window window(sf::VideoMode({360, 240}), "Window Tests");
            CHECK(window.isOpen());
            CHECK(window.getSize() == sf::Vector2u(360, 240));
            CHECK(window.getNativeHandle() != sf::WindowHandle());
            CHECK(window.getSettings().attributeFlags == sf::ContextSettings::Default);
        }

        SECTION("Mode, title, and style constructor")
        {
            const sf::Window window(sf::VideoMode({360, 240}), "Window Tests", sf::Style::Resize);
            CHECK(window.isOpen());
            CHECK(window.getSize() == sf::Vector2u(360, 240));
            CHECK(window.getNativeHandle() != sf::WindowHandle());
            CHECK(window.getSettings().attributeFlags == sf::ContextSettings::Default);
        }

        SECTION("Mode, title, style, and state constructor")
        {
            const sf::Window window(sf::VideoMode({360, 240}), "Window Tests", sf::Style::Resize, sf::State::Windowed);
            CHECK(window.isOpen());
            CHECK(window.getSize() == sf::Vector2u(360, 240));
            CHECK(window.getNativeHandle() != sf::WindowHandle());
            CHECK(window.getSettings().attributeFlags == sf::ContextSettings::Default);
        }

        SECTION("Mode, title, style, state, and context settings constructor")
        {
            const sf::Window window(sf::VideoMode({360, 240}),
                                    "Window Tests",
                                    sf::Style::Resize,
                                    sf::State::Windowed,
                                    sf::ContextSettings{/* depthBits*/ 1, /* stencilBits */ 1, /* antiAliasingLevel */ 1});
            CHECK(window.isOpen());
            CHECK(window.getSize() == sf::Vector2u(360, 240));
            CHECK(window.getNativeHandle() != sf::WindowHandle());
            CHECK(window.getSettings().depthBits >= 1);
            CHECK(window.getSettings().stencilBits >= 1);
            CHECK(window.getSettings().antiAliasingLevel >= 1);
        }

        SECTION("Mode, title, and state")
        {
            const sf::Window window(sf::VideoMode({360, 240}), "Window Tests", sf::State::Windowed);
            CHECK(window.isOpen());
            CHECK(window.getSize() == sf::Vector2u(360, 240));
            CHECK(window.getNativeHandle() != sf::WindowHandle());
            CHECK(window.getSettings().attributeFlags == sf::ContextSettings::Default);
        }

        SECTION("Mode, title, state, and context settings constructor")
        {
            const sf::Window window(sf::VideoMode({360, 240}),
                                    "Window Tests",
                                    sf::State::Windowed,
                                    sf::ContextSettings{/* depthBits*/ 1, /* stencilBits */ 1, /* antiAliasingLevel */ 1});
            CHECK(window.isOpen());
            CHECK(window.getSize() == sf::Vector2u(360, 240));
            CHECK(window.getNativeHandle() != sf::WindowHandle());
            CHECK(window.getSettings().depthBits >= 1);
            CHECK(window.getSettings().stencilBits >= 1);
            CHECK(window.getSettings().antiAliasingLevel >= 1);
        }
    }

    SECTION("create()")
    {
        sf::Window window;

        SECTION("Mode and title")
        {
            window.create(sf::VideoMode({240, 360}), "Window Tests");
            CHECK(window.isOpen());
            CHECK(window.getSize() == sf::Vector2u(240, 360));
            CHECK(window.getNativeHandle() != sf::WindowHandle());
            CHECK(window.getSettings().attributeFlags == sf::ContextSettings::Default);
        }

        SECTION("Mode, title, and style")
        {
            window.create(sf::VideoMode({240, 360}), "Window Tests", sf::Style::Resize);
            CHECK(window.isOpen());
            CHECK(window.getSize() == sf::Vector2u(240, 360));
            CHECK(window.getNativeHandle() != sf::WindowHandle());
            CHECK(window.getSettings().attributeFlags == sf::ContextSettings::Default);
        }

        SECTION("Mode, title, style, and state")
        {
            window.create(sf::VideoMode({240, 360}), "Window Tests", sf::Style::Resize, sf::State::Windowed);
            CHECK(window.isOpen());
            CHECK(window.getSize() == sf::Vector2u(240, 360));
            CHECK(window.getNativeHandle() != sf::WindowHandle());
            CHECK(window.getSettings().attributeFlags == sf::ContextSettings::Default);
        }

        SECTION("Mode, title, style, state, and context settings")
        {
            window.create(sf::VideoMode({240, 360}),
                          "Window Tests",
                          sf::Style::Resize,
                          sf::State::Windowed,
                          sf::ContextSettings{/* depthBits*/ 1, /* stencilBits */ 1, /* antiAliasingLevel */ 1});
            CHECK(window.isOpen());
            CHECK(window.getSize() == sf::Vector2u(240, 360));
            CHECK(window.getNativeHandle() != sf::WindowHandle());
            CHECK(window.getSettings().depthBits >= 1);
            CHECK(window.getSettings().stencilBits >= 1);
            CHECK(window.getSettings().antiAliasingLevel >= 1);
        }

        SECTION("Mode, title, and state")
        {
            window.create(sf::VideoMode({240, 360}), "Window Tests", sf::State::Windowed);
            CHECK(window.isOpen());
            CHECK(window.getSize() == sf::Vector2u(240, 360));
            CHECK(window.getNativeHandle() != sf::WindowHandle());
            CHECK(window.getSettings().attributeFlags == sf::ContextSettings::Default);
        }

        SECTION("Mode, title, state, and context settings")
        {
            window.create(sf::VideoMode({240, 360}),
                          "Window Tests",
                          sf::State::Windowed,
                          sf::ContextSettings{/* depthBits*/ 1, /* stencilBits */ 1, /* antiAliasingLevel */ 1});
            CHECK(window.isOpen());
            CHECK(window.getSize() == sf::Vector2u(240, 360));
            CHECK(window.getNativeHandle() != sf::WindowHandle());
            CHECK(window.getSettings().depthBits >= 1);
            CHECK(window.getSettings().stencilBits >= 1);
            CHECK(window.getSettings().antiAliasingLevel >= 1);
        }
    }
}
