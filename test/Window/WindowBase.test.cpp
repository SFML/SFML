#include <SFML/Window/WindowBase.hpp>

// Other 1st party headers
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <SFML/System/String.hpp>

#include <catch2/catch_test_macros.hpp>

#include <WindowUtil.hpp>
#include <type_traits>

TEST_CASE("[Window] sf::WindowBase", runDisplayTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_copy_constructible_v<sf::WindowBase>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::WindowBase>);
        STATIC_CHECK(!std::is_nothrow_move_constructible_v<sf::WindowBase>);
        STATIC_CHECK(!std::is_nothrow_move_assignable_v<sf::WindowBase>);
    }

    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            const sf::WindowBase windowBase;
            CHECK(!windowBase.isOpen());
            CHECK(windowBase.getPosition() == sf::Vector2i());
            CHECK(windowBase.getSize() == sf::Vector2u());
            CHECK(!windowBase.hasFocus());
            CHECK(windowBase.getNativeHandle() == sf::WindowHandle());
        }

        SECTION("Mode and title constructor")
        {
            const sf::WindowBase windowBase(sf::VideoMode({360, 240}), "WindowBase Tests");
            CHECK(windowBase.isOpen());
            CHECK(windowBase.getSize() == sf::Vector2u(360, 240));
            CHECK(windowBase.getNativeHandle() != sf::WindowHandle());
        }

        SECTION("Mode, title, and style constructor")
        {
            const sf::WindowBase windowBase(sf::VideoMode({360, 240}), "WindowBase Tests", sf::Style::Resize);
            CHECK(windowBase.isOpen());
            CHECK(windowBase.getSize() == sf::Vector2u(360, 240));
            CHECK(windowBase.getNativeHandle() != sf::WindowHandle());
        }
    }

    SECTION("create()")
    {
        sf::WindowBase windowBase;

        SECTION("Mode and title")
        {
            windowBase.create(sf::VideoMode({240, 360}), "WindowBase Tests");
            CHECK(windowBase.isOpen());
            CHECK(windowBase.getSize() == sf::Vector2u(240, 360));
            CHECK(windowBase.getNativeHandle() != sf::WindowHandle());
        }

        SECTION("Mode, title, and style")
        {
            windowBase.create(sf::VideoMode({240, 360}), "WindowBase Tests", sf::Style::Resize);
            CHECK(windowBase.isOpen());
            CHECK(windowBase.getSize() == sf::Vector2u(240, 360));
            CHECK(windowBase.getNativeHandle() != sf::WindowHandle());
        }
    }

    SECTION("close()")
    {
        sf::WindowBase windowBase(sf::VideoMode({360, 240}), "WindowBase Tests");
        windowBase.close();
        CHECK(!windowBase.isOpen());
    }

    SECTION("pollEvent()")
    {
        sf::WindowBase windowBase;
        sf::Event      event;
        CHECK(!windowBase.pollEvent(event));
    }

    SECTION("waitEvent()")
    {
        sf::WindowBase windowBase;
        sf::Event      event;
        CHECK(!windowBase.waitEvent(event));
    }

    SECTION("Set/get position")
    {
        sf::WindowBase windowBase;
        windowBase.setPosition({12, 34});
        CHECK(windowBase.getPosition() == sf::Vector2i());
    }

    SECTION("Set/get size")
    {
        SECTION("Uninitialized window")
        {
            sf::WindowBase windowBase;
            windowBase.setSize({128, 256});
            CHECK(windowBase.getSize() == sf::Vector2u());
        }

        SECTION("Initialized window")
        {
            sf::WindowBase windowBase(sf::VideoMode({360, 240}), "WindowBase Tests");
            windowBase.setSize({128, 256});
            CHECK(windowBase.getSize() == sf::Vector2u(128, 256));
        }

        SECTION("Minimum size")
        {
            sf::WindowBase windowBase(sf::VideoMode({360, 240}), "WindowBase Tests");
            windowBase.setMinimumSize(sf::Vector2u(128, 256));
            windowBase.setSize({100, 100});
            CHECK(windowBase.getSize() == sf::Vector2u(128, 256));
        }

        SECTION("Maximum size")
        {
            sf::WindowBase windowBase(sf::VideoMode({360, 240}), "WindowBase Tests");
            windowBase.setMaximumSize(sf::Vector2u(128, 256));
            windowBase.setSize({400, 400});
            CHECK(windowBase.getSize() == sf::Vector2u(128, 256));
        }
    }

    SECTION("setMinimumSize()")
    {
        sf::WindowBase windowBase(sf::VideoMode({100, 100}), "WindowBase Tests", sf::Style::Default ^ sf::Style::Resize);
        windowBase.setMinimumSize(sf::Vector2u(200, 300));
        CHECK(windowBase.getSize() == sf::Vector2u(200, 300));
        windowBase.setMaximumSize(sf::Vector2u(200, 300));
    }

    SECTION("setMinimumSize()")
    {
        sf::WindowBase windowBase(sf::VideoMode({400, 400}), "WindowBase Tests", sf::Style::Default ^ sf::Style::Resize);
        windowBase.setMaximumSize(sf::Vector2u(200, 300));
        CHECK(windowBase.getSize() == sf::Vector2u(200, 300));
        windowBase.setMinimumSize(sf::Vector2u(200, 300));
    }
}
