#include <SFML/Window/WindowHandle.hpp>

// Other 1st party headers
#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Window/Window.hpp>
#include <SFML/Window/WindowBase.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

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

sf::WindowHandle createWindowWrapper(LPWSTR className, HINSTANCE hInstance, DWORD dwExStyle, bool withMenu)
{
    HMENU hMenu = nullptr;
    if (withMenu)
    {
        hMenu = CreateMenu();
        AppendMenuW(hMenu, MF_STRING, 1, L"Test");
    }
    return CreateWindowExW(dwExStyle,
                           className,
                           L"WindowHandle Tests",
                           WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT,
                           CW_USEDEFAULT,
                           640,
                           480,
                           nullptr,
                           hMenu,
                           hInstance,
                           nullptr);
}

} // namespace

TEST_CASE("[Window] sf::WindowHandle (Win32)")
{
    const auto exStyle  = GENERATE(DWORD{0}, WS_EX_TOOLWINDOW | WS_EX_CLIENTEDGE);
    const auto withMenu = GENERATE(false, true);
    INFO("ExStyle: " << std::hex << exStyle << ", withMenu: " << withMenu);

    const WNDCLASSW classInfo{{}, wndProc, {}, {}, GetModuleHandleW(nullptr), {}, {}, {}, {}, L"sfml_WindowHandleTests"};

    const ATOM winClassId = RegisterClassW(&classInfo);
    REQUIRE(winClassId);

    const sf::WindowHandle handle = createWindowWrapper(reinterpret_cast<LPWSTR>(static_cast<ULONG_PTR>(winClassId)),
                                                        classInfo.hInstance,
                                                        exStyle,
                                                        withMenu);
    REQUIRE(handle);
    REQUIRE(!gotWmShowWindow);
    REQUIRE(IsWindow(handle));

    RECT windowRect{};
    REQUIRE(GetWindowRect(handle, &windowRect));
    const auto position = sf::Vector2i(sf::Vector2(windowRect.left, windowRect.top));

    RECT clientRect{};
    REQUIRE(GetClientRect(handle, &clientRect));
    const auto initialSize = sf::Vector2u(
        sf::Vector2(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top));
    constexpr sf::Vector2u newSize(640, 480);

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

        INFO("ExStyle: " << std::hex << exStyle << ", withMenu: " << withMenu);
        CHECK(windowBase->isOpen());
        CHECK(windowBase->getPosition() == position);
        CHECK(windowBase->getSize() == initialSize);
        CHECK(windowBase->getNativeHandle() == handle);

        CHECK(windowBase->getSize() != newSize);
        windowBase->setSize(newSize);

        REQUIRE(GetClientRect(handle, &clientRect));
        const auto size = sf::Vector2u(sf::Vector2(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top));
        CHECK(size == newSize);               // Validate that the actual client rect is indeed what we asked for
        CHECK(windowBase->getSize() == size); // Validate that the `getSize` also returns the _actual_ client size
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

            INFO("ExStyle: " << std::hex << exStyle << ", withMenu: " << withMenu);
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

            INFO("ExStyle: " << std::hex << exStyle << ", withMenu: " << withMenu);
            CHECK(window->getSettings().depthBits >= 1);
            CHECK(window->getSettings().stencilBits >= 1);
            CHECK(window->getSettings().antiAliasingLevel >= 1);
        }

        INFO("ExStyle: " << std::hex << exStyle << ", withMenu: " << withMenu);
        CHECK(window->isOpen());
        CHECK(window->getPosition() == position);
        CHECK(window->getSize() == initialSize);
        CHECK(window->getNativeHandle() == handle);

        CHECK(window->getSize() != newSize);
        window->setSize(newSize);

        REQUIRE(GetClientRect(handle, &clientRect));
        const auto size = sf::Vector2u(sf::Vector2(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top));
        CHECK(size == newSize);           // Validate that the actual client rect is indeed what we asked for
        CHECK(window->getSize() == size); // Validate that the `getSize` also returns the _actual_ client size
    }

    SECTION("sf::RenderWindow")
    {
        std::optional<sf::RenderWindow> renderWindow;

        SECTION("Default context settings")
        {
            SECTION("WindowHandle constructor")
            {
                renderWindow.emplace(handle);
            }

            SECTION("create(WindowHandle)")
            {
                renderWindow.emplace().create(handle);
            }

            INFO("ExStyle: " << std::hex << exStyle << ", withMenu: " << withMenu);
            CHECK(renderWindow->getSettings().attributeFlags == sf::ContextSettings::Default);
        }

        SECTION("Custom context settings")
        {
            static constexpr sf::ContextSettings contextSettings{/* depthBits*/ 1, /* stencilBits */ 1, /* antiAliasingLevel */ 1};

            SECTION("WindowHandle constructor")
            {
                renderWindow.emplace(handle, contextSettings);
            }

            SECTION("create(WindowHandle)")
            {
                renderWindow.emplace().create(handle, contextSettings);
            }

            INFO("ExStyle: " << std::hex << exStyle << ", withMenu: " << withMenu);
            CHECK(renderWindow->getSettings().depthBits >= 1);
            CHECK(renderWindow->getSettings().stencilBits >= 1);
            CHECK(renderWindow->getSettings().antiAliasingLevel >= 1);
        }

        INFO("ExStyle: " << std::hex << exStyle << ", withMenu: " << withMenu);
        CHECK(renderWindow->isOpen());
        CHECK(renderWindow->getPosition() == position);
        CHECK(renderWindow->getSize() == initialSize);
        CHECK(renderWindow->getNativeHandle() == handle);

        CHECK(renderWindow->getSize() != newSize);
        renderWindow->setSize(newSize);

        REQUIRE(GetClientRect(handle, &clientRect));
        const auto size = sf::Vector2u(sf::Vector2(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top));
        CHECK(size == newSize);                 // Validate that the actual client rect is indeed what we asked for
        CHECK(renderWindow->getSize() == size); // Validate that the `getSize` also returns the _actual_ client size
    }

    INFO("ExStyle: " << std::hex << exStyle << ", withMenu: " << withMenu);

    CHECK(gotWmShowWindow);
    CHECK(IsWindow(handle)); // The window is not destroyed

    CHECK(DestroyWindow(handle));
    CHECK(UnregisterClassW(classInfo.lpszClassName, classInfo.hInstance));
    gotWmShowWindow = false;

    CHECK(!IsWindow(handle)); // Now it is gone
}
