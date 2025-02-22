#include <SFML/Window/WindowHandle.hpp>

// Other 1st party headers
#include <SFML/Window/Window.hpp>
#include <SFML/Window/WindowBase.hpp>

#include <catch2/catch_test_macros.hpp>

#include <SystemUtil.hpp>
#include <optional>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

namespace
{
bool gotWmShowWindow = false;

LRESULT WINAPI wndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_SHOWWINDOW:
            gotWmShowWindow = true;
            assert(wParam == TRUE); // If wParam is TRUE, the window is being shown
            assert(lParam == 0); // If lParam is zero, the message was sent because of a call to the ShowWindow function
            break;
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(handle, message, wParam, lParam);
}
} // namespace

TEST_CASE("[Window] sf::WindowHandle (Win32)")
{
    const WNDCLASSW classInfo{{}, wndProc, {}, {}, GetModuleHandleW(nullptr), {}, {}, {}, {}, L"sfml_WindowHandleTests"};

    const ATOM winClassId = RegisterClassW(&classInfo);
    REQUIRE(winClassId);

    const sf::WindowHandle handle = CreateWindowW(reinterpret_cast<LPWSTR>(static_cast<ULONG_PTR>(winClassId)),
                                                  L"WindowHandle Tests",
                                                  WS_OVERLAPPEDWINDOW,
                                                  CW_USEDEFAULT,
                                                  CW_USEDEFAULT,
                                                  640,
                                                  480,
                                                  nullptr,
                                                  nullptr,
                                                  classInfo.hInstance,
                                                  nullptr);
    REQUIRE(handle);
    REQUIRE(!gotWmShowWindow);
    REQUIRE(IsWindow(handle));

    RECT windowRect{};
    REQUIRE(GetWindowRect(handle, &windowRect));
    const auto position = sf::Vector2i(sf::Vector2(windowRect.left, windowRect.top));

    RECT clientRect{};
    REQUIRE(GetClientRect(handle, &clientRect));
    const auto size = sf::Vector2u(sf::Vector2(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top));

    SECTION("sf::WindowBase")
    {
        std::optional<sf::WindowBase> windowBase;

        SECTION("WindowHandle constructor")
        {
            windowBase.emplace(handle);
        }

        SECTION("create(WindowHandle)")
        {
            windowBase.emplace().create(handle);
        }

        CHECK(windowBase->isOpen());
        CHECK(windowBase->getPosition() == position);
        CHECK(windowBase->getSize() == size);
        CHECK(windowBase->getNativeHandle() == handle);
    }

    SECTION("sf::Window")
    {
        std::optional<sf::Window> window;

        SECTION("Default context settings")
        {
            SECTION("WindowHandle constructor")
            {
                window.emplace(handle);
            }

            SECTION("create(WindowHandle)")
            {
                window.emplace().create(handle);
            }

            CHECK(window->getSettings().attributeFlags == sf::ContextSettings::Default);
        }

        SECTION("Custom context settings")
        {
            static constexpr sf::ContextSettings contextSettings{/* depthBits*/ 1, /* stencilBits */ 1, /* antiAliasingLevel */ 1};

            SECTION("WindowHandle constructor")
            {
                window.emplace(handle, contextSettings);
            }

            SECTION("create(WindowHandle)")
            {
                window.emplace().create(handle, contextSettings);
            }

            CHECK(window->getSettings().depthBits >= 1);
            CHECK(window->getSettings().stencilBits >= 1);
            CHECK(window->getSettings().antiAliasingLevel >= 1);
        }

        CHECK(window->isOpen());
        CHECK(window->getPosition() == position);
        CHECK(window->getSize() == size);
        CHECK(window->getNativeHandle() == handle);
    }

    CHECK(gotWmShowWindow);
    CHECK(IsWindow(handle)); // The window is not destroyed

    CHECK(DestroyWindow(handle));
    CHECK(UnregisterClassW(classInfo.lpszClassName, classInfo.hInstance));
    gotWmShowWindow = false;

    CHECK(!IsWindow(handle)); // Now it is gone
}
