#include <SFML/Window/WindowBase.hpp>

// Other 1st party headers
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <SFML/System/String.hpp>

#include <catch2/catch_test_macros.hpp>

#include <WindowUtil.hpp>
#include <chrono>
#include <memory>
#include <type_traits>
#include <utility>

TEST_CASE("[Window] sf::WindowBase", runDisplayTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::has_virtual_destructor_v<sf::WindowBase>);
        STATIC_CHECK(!std::is_copy_constructible_v<sf::WindowBase>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::WindowBase>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::WindowBase>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::WindowBase>);
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

        SECTION("Mode, title, style, and state constructor")
        {
            const sf::WindowBase windowBase(sf::VideoMode({360, 240}), "WindowBase Tests", sf::Style::Resize, sf::State::Windowed);
            CHECK(windowBase.isOpen());
            CHECK(windowBase.getSize() == sf::Vector2u(360, 240));
            CHECK(windowBase.getNativeHandle() != sf::WindowHandle());
        }

        SECTION("Mode, title, and state constructor")
        {
            const sf::WindowBase windowBase(sf::VideoMode({360, 240}), "WindowBase Tests", sf::State::Windowed);
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

        SECTION("Mode, title, and state")
        {
            windowBase.create(sf::VideoMode({240, 360}), "WindowBase Tests", sf::State::Windowed);
            CHECK(windowBase.isOpen());
            CHECK(windowBase.getSize() == sf::Vector2u(240, 360));
            CHECK(windowBase.getNativeHandle() != sf::WindowHandle());
        }

        SECTION("Mode, title, style, and state")
        {
            windowBase.create(sf::VideoMode({240, 360}), "WindowBase Tests", sf::Style::Resize, sf::State::Windowed);
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
        SECTION("Uninitialized window")
        {
            sf::WindowBase windowBase;
            CHECK(!windowBase.pollEvent());
        }
    }

    SECTION("waitEvent()")
    {
        SECTION("Uninitialized window")
        {
            sf::WindowBase windowBase;
            CHECK(!windowBase.waitEvent());
        }

        SECTION("Initialized window")
        {
            sf::WindowBase windowBase(sf::VideoMode({360, 240}), "WindowBase Tests");

            constexpr auto timeout = sf::milliseconds(100);

            const auto startTime = std::chrono::steady_clock::now();
            const auto event     = windowBase.waitEvent(timeout);
            const auto elapsed   = std::chrono::steady_clock::now() - startTime;

            REQUIRE(elapsed < (timeout + sf::milliseconds(100)).toDuration());

            if (elapsed <= timeout.toDuration())
                CHECK(event);
            else
                CHECK(!event);
        }
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

    // Test for compilation but do not run. This code sometimes hangs indefinitely
    // when running on the BuildBot CI pipeline. Because it contains no
    // assertions we have nothing to gain by running it anyways
    (void)[]
    {
        sf::WindowBase windowBase(sf::VideoMode({360, 240}), "WindowBase Tests");

        // Should compile if user provides only a specific handler
        windowBase.handleEvents([](const sf::Event::Closed&) {});

        // Should compile if user provides only a catch-all
        windowBase.handleEvents([](const auto&) {});

        // Should compile if user provides both a specific handler and a catch-all
        windowBase.handleEvents([](const sf::Event::Closed&) {}, [](const auto&) {});

        // Should compile if user provides a handler taking an event subtype by value or reference,
        // but not rvalue reference because it would never be called.
        windowBase.handleEvents([](sf::Event::Closed) {});
        windowBase.handleEvents([](const sf::Event::Closed) {});
        windowBase.handleEvents([](sf::Event::Closed&) {});
        windowBase.handleEvents([](const sf::Event::Closed&) {});

        // Should compile if user provides a move-only handler
        windowBase.handleEvents([p = std::make_unique<int>()](const sf::Event::Closed&) {});

        // Should compile if user provides a handler with deleted rvalue ref-qualified call operator
        struct LvalueOnlyHandler
        {
            void operator()(const sf::Event::Closed&) &
            {
            }
            void operator()(const sf::Event::Closed&) && = delete;
        };
        windowBase.handleEvents(LvalueOnlyHandler{});

        // Should compile if user provides a reference to a handler
        auto handler = [](const sf::Event::Closed&) {};
        windowBase.handleEvents(handler);
        windowBase.handleEvents(std::as_const(handler));

        // Should compile if user provides a function pointer
        windowBase.handleEvents(+[](const sf::Event::Closed&) {});
    };
}
