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

class NativeWindow
{
public:
    NativeWindow()
    {
        m_classInfo.lpfnWndProc   = wndProc;
        m_classInfo.hInstance     = GetModuleHandleW(nullptr);
        m_classInfo.lpszClassName = L"sfml_WindowBaseTests";

        m_winClassId = RegisterClassW(&m_classInfo);
        assert(m_winClassId);

        m_handle = CreateWindowW(reinterpret_cast<LPWSTR>(static_cast<ULONG_PTR>(m_winClassId)),
                                 L"WindowBase Tests",
                                 WS_OVERLAPPEDWINDOW,
                                 CW_USEDEFAULT,
                                 CW_USEDEFAULT,
                                 640,
                                 480,
                                 nullptr,
                                 nullptr,
                                 m_classInfo.hInstance,
                                 nullptr);
        assert(m_handle);
    }

    ~NativeWindow()
    {
        DestroyWindow(m_handle);
        UnregisterClassW(m_classInfo.lpszClassName, m_classInfo.hInstance);
        gotWmShowWindow = false;
    }

    [[nodiscard]] HWND getHandle() const
    {
        return m_handle;
    }

private:
    static LRESULT WINAPI wndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
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

    WNDCLASSW m_classInfo{};
    ATOM      m_winClassId{};
    HWND      m_handle{};
};
} // namespace

TEST_CASE("[Window] sf::WindowBase (Win32)")
{
    std::optional<NativeWindow>   nativeWindow(std::in_place);
    std::optional<sf::WindowBase> windowBase;

    const HWND handle = nativeWindow->getHandle();

    SECTION("WindowHandle constructor")
    {
        windowBase.emplace(handle);
    }

    SECTION("create(WindowHandle)")
    {
        windowBase.emplace();
        windowBase->create(handle);
    }

    RECT rect{};
    GetClientRect(handle, &rect);
    CHECK(windowBase->isOpen());
    CHECK(windowBase->getSize() == sf::Vector2u(sf::Vector2(rect.right - rect.left, rect.bottom - rect.top)));
    CHECK(windowBase->getNativeHandle() == handle);

    CHECK(gotWmShowWindow);
    CHECK(IsWindow(handle));

    windowBase.reset();
    CHECK(IsWindow(handle)); // The window is not destroyed.

    nativeWindow.reset();
    CHECK(!IsWindow(handle)); // Now it is gone
}
