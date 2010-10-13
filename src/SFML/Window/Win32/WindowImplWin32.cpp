////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#define _WIN32_WINDOWS 0x0501
#define _WIN32_WINNT   0x0501
#include <SFML/Window/Win32/WindowImplWin32.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <GL/gl.h>
#include <SFML/Window/glext/wglext.h>
#include <SFML/Window/glext/glext.h>
#include <SFML/System/Err.hpp>
#include <vector>

// MinGW lacks the definition of some Win32 constants
#ifndef XBUTTON1
    #define XBUTTON1 0x0001
#endif
#ifndef XBUTTON2
    #define XBUTTON2 0x0002
#endif
#ifndef MAPVK_VK_TO_VSC
    #define MAPVK_VK_TO_VSC (0)
#endif


////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
    unsigned int               WindowCount      = 0;
    const char*                ClassNameA       = "SFML_Window";
    const wchar_t*             ClassNameW       = L"SFML_Window";
    sf::priv::WindowImplWin32* FullscreenWindow = NULL;
}

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
WindowImplWin32::WindowImplWin32(WindowHandle handle) :
myHandle          (handle),
myCallback        (0),
myCursor          (NULL),
myIcon            (NULL),
myKeyRepeatEnabled(true),
myIsCursorIn      (false)
{
    if (myHandle)
    {
        // Get window client size
        RECT rectangle;
        GetClientRect(myHandle, &rectangle);
        myWidth  = rectangle.right - rectangle.left;
        myHeight = rectangle.bottom - rectangle.top;

        // We change the event procedure of the control (it is important to save the old one)
        SetWindowLongPtr(myHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        myCallback = SetWindowLongPtr(myHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowImplWin32::GlobalOnEvent));
    }
}


////////////////////////////////////////////////////////////
WindowImplWin32::WindowImplWin32(VideoMode mode, const std::string& title, unsigned long style) :
myHandle          (NULL),
myCallback        (0),
myCursor          (NULL),
myIcon            (NULL),
myKeyRepeatEnabled(true),
myIsCursorIn      (false)
{
    // Register the window class at first call
    if (WindowCount == 0)
        RegisterWindowClass();

    // Compute position and size
    HDC screenDC = GetDC(NULL);
    int left   = (GetDeviceCaps(screenDC, HORZRES) - mode.Width)  / 2;
    int top    = (GetDeviceCaps(screenDC, VERTRES) - mode.Height) / 2;
    int width  = myWidth  = mode.Width;
    int height = myHeight = mode.Height;
    ReleaseDC(NULL, screenDC);

    // Choose the window style according to the Style parameter
    DWORD win32Style = WS_VISIBLE;
    if (style == Style::None)
    {
        win32Style |= WS_POPUP;
    }
    else
    {
        if (style & Style::Titlebar) win32Style |= WS_CAPTION | WS_MINIMIZEBOX;
        if (style & Style::Resize)   win32Style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
        if (style & Style::Close)    win32Style |= WS_SYSMENU;
    }

    // In windowed mode, adjust width and height so that window will have the requested client area
    bool fullscreen = (style & Style::Fullscreen) != 0;
    if (!fullscreen)
    {
        RECT rectangle = {0, 0, width, height};
        AdjustWindowRect(&rectangle, win32Style, false);
        width  = rectangle.right - rectangle.left;
        height = rectangle.bottom - rectangle.top;
    }

    // Create the window
    if (HasUnicodeSupport())
    {
        wchar_t wTitle[256];
        int count = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, title.c_str(), static_cast<int>(title.size()), wTitle, sizeof(wTitle) / sizeof(*wTitle));
        wTitle[count] = L'\0';
        myHandle = CreateWindowW(ClassNameW, wTitle, win32Style, left, top, width, height, NULL, NULL, GetModuleHandle(NULL), this);
    }
    else
    {
        myHandle = CreateWindowA(ClassNameA, title.c_str(), win32Style, left, top, width, height, NULL, NULL, GetModuleHandle(NULL), this);
    }

    // Switch to fullscreen if requested
    if (fullscreen)
        SwitchToFullscreen(mode);

    // Increment window count
    WindowCount++;

    // Get the actual size of the window, which can be smaller even after the call to AdjustWindowRect
    // This happens when the window is bigger than the desktop
    RECT actualRectangle;
    GetClientRect(myHandle, &actualRectangle);
    myWidth  = actualRectangle.right - actualRectangle.left;
    myHeight = actualRectangle.bottom - actualRectangle.top;
}


////////////////////////////////////////////////////////////
WindowImplWin32::~WindowImplWin32()
{
    // Destroy the custom icon, if any
    if (myIcon)
        DestroyIcon(myIcon);

    if (!myCallback)
    {
        // Destroy the window
        if (myHandle)
            DestroyWindow(myHandle);

        // Decrement the window count
        WindowCount--;

        // Unregister window class if we were the last window
        if (WindowCount == 0)
        {
            if (HasUnicodeSupport())
            {
                UnregisterClassW(ClassNameW, GetModuleHandle(NULL));
            }
            else
            {
                UnregisterClassA(ClassNameA, GetModuleHandle(NULL));
            }
        }
    }
    else
    {
        // The window is external : remove the hook on its message callback
        SetWindowLongPtr(myHandle, GWLP_WNDPROC, myCallback);
    }
}


////////////////////////////////////////////////////////////
WindowHandle WindowImplWin32::GetSystemHandle() const
{
    return myHandle;
}


////////////////////////////////////////////////////////////
void WindowImplWin32::ProcessEvents(bool block)
{
    // We process the window events only if we own it
    if (!myCallback)
    {
        if (block)
            WaitMessage();

        MSG message;
        while (PeekMessage(&message, myHandle, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    }
}


////////////////////////////////////////////////////////////
void WindowImplWin32::ShowMouseCursor(bool show)
{
    if (show)
        myCursor = LoadCursor(NULL, IDC_ARROW);
    else
        myCursor = NULL;

    SetCursor(myCursor);
}


////////////////////////////////////////////////////////////
void WindowImplWin32::SetCursorPosition(unsigned int x, unsigned int y)
{
    POINT position = {x, y};
    ClientToScreen(myHandle, &position);
    SetCursorPos(position.x, position.y);
}


////////////////////////////////////////////////////////////
void WindowImplWin32::SetPosition(int x, int y)
{
    SetWindowPos(myHandle, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}


////////////////////////////////////////////////////////////
void WindowImplWin32::SetSize(unsigned int width, unsigned int height)
{
    // SetWindowPos wants the total size of the window (including title bar and borders),
    // so we have to compute it
    RECT rectangle = {0, 0, width, height};
    AdjustWindowRect(&rectangle, GetWindowLong(myHandle, GWL_STYLE), false);
    width  = rectangle.right - rectangle.left;
    height = rectangle.bottom - rectangle.top;

    SetWindowPos(myHandle, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}


////////////////////////////////////////////////////////////
void WindowImplWin32::SetTitle(const std::string& title)
{
    SetWindowText(myHandle, title.c_str());
}


////////////////////////////////////////////////////////////
void WindowImplWin32::Show(bool show)
{
    ShowWindow(myHandle, show ? SW_SHOW : SW_HIDE);
}


////////////////////////////////////////////////////////////
void WindowImplWin32::EnableKeyRepeat(bool enabled)
{
    myKeyRepeatEnabled = enabled;
}


////////////////////////////////////////////////////////////
void WindowImplWin32::SetIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
    // First destroy the previous one
    if (myIcon)
        DestroyIcon(myIcon);

    // Windows wants BGRA pixels : swap red and blue channels
    std::vector<Uint8> iconPixels(width * height * 4);
    for (std::size_t i = 0; i < iconPixels.size() / 4; ++i)
    {
        iconPixels[i * 4 + 0] = pixels[i * 4 + 2];
        iconPixels[i * 4 + 1] = pixels[i * 4 + 1];
        iconPixels[i * 4 + 2] = pixels[i * 4 + 0];
        iconPixels[i * 4 + 3] = pixels[i * 4 + 3];
    }

    // Create the icon from the pixels array
    myIcon = CreateIcon(GetModuleHandle(NULL), width, height, 1, 32, NULL, &iconPixels[0]);

    // Set it as both big and small icon of the window
    if (myIcon)
    {
        SendMessage(myHandle, WM_SETICON, ICON_BIG,   (LPARAM)myIcon);
        SendMessage(myHandle, WM_SETICON, ICON_SMALL, (LPARAM)myIcon);
    }
    else
    {
        Err() << "Failed to set the window's icon" << std::endl;
    }
}


////////////////////////////////////////////////////////////
void WindowImplWin32::RegisterWindowClass()
{
    if (HasUnicodeSupport())
    {
        WNDCLASSW WindowClass;
        WindowClass.style         = 0;
        WindowClass.lpfnWndProc   = &WindowImplWin32::GlobalOnEvent;
        WindowClass.cbClsExtra    = 0;
        WindowClass.cbWndExtra    = 0;
        WindowClass.hInstance     = GetModuleHandle(NULL);
        WindowClass.hIcon         = NULL;
        WindowClass.hCursor       = 0;
        WindowClass.hbrBackground = 0;
        WindowClass.lpszMenuName  = NULL;
        WindowClass.lpszClassName = ClassNameW;
        RegisterClassW(&WindowClass);
    }
    else
    {
        WNDCLASSA WindowClass;
        WindowClass.style         = 0;
        WindowClass.lpfnWndProc   = &WindowImplWin32::GlobalOnEvent;
        WindowClass.cbClsExtra    = 0;
        WindowClass.cbWndExtra    = 0;
        WindowClass.hInstance     = GetModuleHandle(NULL);
        WindowClass.hIcon         = NULL;
        WindowClass.hCursor       = 0;
        WindowClass.hbrBackground = 0;
        WindowClass.lpszMenuName  = NULL;
        WindowClass.lpszClassName = ClassNameA;
        RegisterClassA(&WindowClass);
    }
}


////////////////////////////////////////////////////////////
void WindowImplWin32::SwitchToFullscreen(const VideoMode& mode)
{
    DEVMODE devMode;
    devMode.dmSize       = sizeof(devMode);
    devMode.dmPelsWidth  = mode.Width;
    devMode.dmPelsHeight = mode.Height;
    devMode.dmBitsPerPel = mode.BitsPerPixel;
    devMode.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

    // Apply fullscreen mode
    if (ChangeDisplaySettings(&devMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
    {
        Err() << "Failed to change display mode for fullscreen" << std::endl;
        return;
    }

    // Make the window flags compatible with fullscreen mode
    SetWindowLong(myHandle, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
    SetWindowLong(myHandle, GWL_EXSTYLE, WS_EX_APPWINDOW);

    // Resize the window so that it fits the entire screen
    SetWindowPos(myHandle, HWND_TOP, 0, 0, mode.Width, mode.Height, SWP_FRAMECHANGED);
    ShowWindow(myHandle, SW_SHOW);

    // Set "this" as the current fullscreen window
    FullscreenWindow = this;
}


////////////////////////////////////////////////////////////
void WindowImplWin32::Cleanup()
{
    // Restore the previous video mode (in case we were running in fullscreen)
    if (FullscreenWindow == this)
    {
        ChangeDisplaySettings(NULL, 0);
        FullscreenWindow = NULL;
    }

    // Unhide the mouse cursor (in case it was hidden)
    ShowMouseCursor(true);
}


////////////////////////////////////////////////////////////
void WindowImplWin32::ProcessEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    // Don't process any message until window is created
    if (myHandle == NULL)
        return;

    switch (message)
    {
        // Destroy event
        case WM_DESTROY :
        {
            // Here we must cleanup resources !
            Cleanup();
            break;
        }

        // Set cursor event
        case WM_SETCURSOR :
        {
            // The mouse has moved, if the cursor is in our window we must refresh the cursor
            if (LOWORD(lParam) == HTCLIENT)
                SetCursor(myCursor);

            break;
        }

        // Close event
        case WM_CLOSE :
        {
            Event event;
            event.Type = Event::Closed;
            PushEvent(event);
            break;
        }

        // Resize event
        case WM_SIZE :
        {
            // Ignore size events triggered by a minimize (size == 0 in this case)
            if (wParam != SIZE_MINIMIZED)
            {
                // Update window size
                RECT rectangle;
                GetClientRect(myHandle, &rectangle);
                myWidth  = rectangle.right - rectangle.left;
                myHeight = rectangle.bottom - rectangle.top;

                Event event;
                event.Type        = Event::Resized;
                event.Size.Width  = myWidth;
                event.Size.Height = myHeight;
                PushEvent(event);
                break;
            }
        }

        // Gain focus event
        case WM_SETFOCUS :
        {
            Event event;
            event.Type = Event::GainedFocus;
            PushEvent(event);
            break;
        }

        // Lost focus event
        case WM_KILLFOCUS :
        {
            Event event;
            event.Type = Event::LostFocus;
            PushEvent(event);
            break;
        }

        // Text event
        case WM_CHAR :
        {
            if (myKeyRepeatEnabled || ((lParam & (1 << 30)) == 0))
            {
                Event event;
                event.Type = Event::TextEntered;
                event.Text.Unicode = static_cast<Uint32>(wParam);
                PushEvent(event);
            }
            break;
        }

        // Keydown event
        case WM_KEYDOWN :
        case WM_SYSKEYDOWN :
        {
            if (myKeyRepeatEnabled || ((HIWORD(lParam) & KF_REPEAT) == 0))
            {
                Event event;
                event.Type        = Event::KeyPressed;
                event.Key.Alt     = HIWORD(GetAsyncKeyState(VK_MENU))    != 0;
                event.Key.Control = HIWORD(GetAsyncKeyState(VK_CONTROL)) != 0;
                event.Key.Shift   = HIWORD(GetAsyncKeyState(VK_SHIFT))   != 0;
                event.Key.Code    = VirtualKeyCodeToSF(wParam, lParam);
                PushEvent(event);
            }
            break;
        }

        // Keyup event
        case WM_KEYUP :
        case WM_SYSKEYUP :
        {
            Event event;
            event.Type        = Event::KeyReleased;
            event.Key.Alt     = HIWORD(GetAsyncKeyState(VK_MENU))    != 0;
            event.Key.Control = HIWORD(GetAsyncKeyState(VK_CONTROL)) != 0;
            event.Key.Shift   = HIWORD(GetAsyncKeyState(VK_SHIFT))   != 0;
            event.Key.Code    = VirtualKeyCodeToSF(wParam, lParam);
            PushEvent(event);
            break;
        }

        // Mouse wheel event
        case WM_MOUSEWHEEL :
        {
            // Mouse position is in screen coordinates, convert it to window coordinates
            POINT position;
            position.x = static_cast<Int16>(LOWORD(lParam));
            position.y = static_cast<Int16>(HIWORD(lParam));
            ScreenToClient(myHandle, &position);

            Event event;
            event.Type = Event::MouseWheelMoved;
            event.MouseWheel.Delta = static_cast<Int16>(HIWORD(wParam)) / 120;
            event.MouseButton.X    = position.x;
            event.MouseButton.Y    = position.y;
            PushEvent(event);
            break;
        }

        // Mouse left button down event
        case WM_LBUTTONDOWN :
        {
            Event event;
            event.Type               = Event::MouseButtonPressed;
            event.MouseButton.Button = Mouse::Left;
            event.MouseButton.X      = static_cast<Int16>(LOWORD(lParam));
            event.MouseButton.Y      = static_cast<Int16>(HIWORD(lParam));
            PushEvent(event);
            break;
        }

        // Mouse left button up event
        case WM_LBUTTONUP :
        {
            Event event;
            event.Type               = Event::MouseButtonReleased;
            event.MouseButton.Button = Mouse::Left;
            event.MouseButton.X      = static_cast<Int16>(LOWORD(lParam));
            event.MouseButton.Y      = static_cast<Int16>(HIWORD(lParam));
            PushEvent(event);
            break;
        }

        // Mouse right button down event
        case WM_RBUTTONDOWN :
        {
            Event event;
            event.Type               = Event::MouseButtonPressed;
            event.MouseButton.Button = Mouse::Right;
            event.MouseButton.X      = static_cast<Int16>(LOWORD(lParam));
            event.MouseButton.Y      = static_cast<Int16>(HIWORD(lParam));
            PushEvent(event);
            break;
        }

        // Mouse right button up event
        case WM_RBUTTONUP :
        {
            Event event;
            event.Type               = Event::MouseButtonReleased;
            event.MouseButton.Button = Mouse::Right;
            event.MouseButton.X      = static_cast<Int16>(LOWORD(lParam));
            event.MouseButton.Y      = static_cast<Int16>(HIWORD(lParam));
            PushEvent(event);
            break;
        }

        // Mouse wheel button down event
        case WM_MBUTTONDOWN :
        {
            Event event;
            event.Type               = Event::MouseButtonPressed;
            event.MouseButton.Button = Mouse::Middle;
            event.MouseButton.X      = static_cast<Int16>(LOWORD(lParam));
            event.MouseButton.Y      = static_cast<Int16>(HIWORD(lParam));
            PushEvent(event);
            break;
        }

        // Mouse wheel button up event
        case WM_MBUTTONUP :
        {
            Event event;
            event.Type               = Event::MouseButtonReleased;
            event.MouseButton.Button = Mouse::Middle;
            event.MouseButton.X      = static_cast<Int16>(LOWORD(lParam));
            event.MouseButton.Y      = static_cast<Int16>(HIWORD(lParam));
            PushEvent(event);
            break;
        }

        // Mouse X button down event
        case WM_XBUTTONDOWN :
        {
            Event event;
            event.Type               = Event::MouseButtonPressed;
            event.MouseButton.Button = HIWORD(wParam) == XBUTTON1 ? Mouse::XButton1 : Mouse::XButton2;
            event.MouseButton.X      = static_cast<Int16>(LOWORD(lParam));
            event.MouseButton.Y      = static_cast<Int16>(HIWORD(lParam));
            PushEvent(event);
            break;
        }

        // Mouse X button up event
        case WM_XBUTTONUP :
        {
            Event event;
            event.Type               = Event::MouseButtonReleased;
            event.MouseButton.Button = HIWORD(wParam) == XBUTTON1 ? Mouse::XButton1 : Mouse::XButton2;
            event.MouseButton.X      = static_cast<Int16>(LOWORD(lParam));
            event.MouseButton.Y      = static_cast<Int16>(HIWORD(lParam));
            PushEvent(event);
            break;
        }

        // Mouse move event
        case WM_MOUSEMOVE :
        {
            // Check if we need to generate a MouseEntered event
            if (!myIsCursorIn)
            {
                TRACKMOUSEEVENT mouseEvent;
                mouseEvent.cbSize    = sizeof(TRACKMOUSEEVENT);
                mouseEvent.hwndTrack = myHandle;
                mouseEvent.dwFlags   = TME_LEAVE;
                TrackMouseEvent(&mouseEvent);

                myIsCursorIn = true;

                Event event;
                event.Type = Event::MouseEntered;
                PushEvent(event);
            }

            Event event;
            event.Type        = Event::MouseMoved;
            event.MouseMove.X = static_cast<Int16>(LOWORD(lParam));
            event.MouseMove.Y = static_cast<Int16>(HIWORD(lParam));
            PushEvent(event);
            break;
        }

        // Mouse leave event
        case WM_MOUSELEAVE :
        {
            myIsCursorIn = false;

            Event event;
            event.Type = Event::MouseLeft;
            PushEvent(event);
            break;
        }
    }
}


////////////////////////////////////////////////////////////
Key::Code WindowImplWin32::VirtualKeyCodeToSF(WPARAM key, LPARAM flags)
{
    switch (key)
    {
        // Check the scancode to distinguish between left and right shift
        case VK_SHIFT :
        {
            static UINT lShift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
            UINT scancode = static_cast<UINT>((flags & (0xFF << 16)) >> 16);
            return scancode == lShift ? Key::LShift : Key::RShift;
        }

        // Check the "extended" flag to distinguish between left and right alt
        case VK_MENU : return (HIWORD(flags) & KF_EXTENDED) ? Key::RAlt : Key::LAlt;

        // Check the "extended" flag to distinguish between left and right control
        case VK_CONTROL : return (HIWORD(flags) & KF_EXTENDED) ? Key::RControl : Key::LControl;

        // Other keys are reported properly
        case VK_LWIN :       return Key::LSystem;
        case VK_RWIN :       return Key::RSystem;
        case VK_APPS :       return Key::Menu;
        case VK_OEM_1 :      return Key::SemiColon;
        case VK_OEM_2 :      return Key::Slash;
        case VK_OEM_PLUS :   return Key::Equal;
        case VK_OEM_MINUS :  return Key::Dash;
        case VK_OEM_4 :      return Key::LBracket;
        case VK_OEM_6 :      return Key::RBracket;
        case VK_OEM_COMMA :  return Key::Comma;
        case VK_OEM_PERIOD : return Key::Period;
        case VK_OEM_7 :      return Key::Quote;
        case VK_OEM_5 :      return Key::BackSlash;
        case VK_OEM_3 :      return Key::Tilde;
        case VK_ESCAPE :     return Key::Escape;
        case VK_SPACE :      return Key::Space;
        case VK_RETURN :     return Key::Return;
        case VK_BACK :       return Key::Back;
        case VK_TAB :        return Key::Tab;
        case VK_PRIOR :      return Key::PageUp;
        case VK_NEXT :       return Key::PageDown;
        case VK_END :        return Key::End;
        case VK_HOME :       return Key::Home;
        case VK_INSERT :     return Key::Insert;
        case VK_DELETE :     return Key::Delete;
        case VK_ADD :        return Key::Add;
        case VK_SUBTRACT :   return Key::Subtract;
        case VK_MULTIPLY :   return Key::Multiply;
        case VK_DIVIDE :     return Key::Divide;
        case VK_PAUSE :      return Key::Pause;
        case VK_F1 :         return Key::F1;
        case VK_F2 :         return Key::F2;
        case VK_F3 :         return Key::F3;
        case VK_F4 :         return Key::F4;
        case VK_F5 :         return Key::F5;
        case VK_F6 :         return Key::F6;
        case VK_F7 :         return Key::F7;
        case VK_F8 :         return Key::F8;
        case VK_F9 :         return Key::F9;
        case VK_F10 :        return Key::F10;
        case VK_F11 :        return Key::F11;
        case VK_F12 :        return Key::F12;
        case VK_F13 :        return Key::F13;
        case VK_F14 :        return Key::F14;
        case VK_F15 :        return Key::F15;
        case VK_LEFT :       return Key::Left;
        case VK_RIGHT :      return Key::Right;
        case VK_UP :         return Key::Up;
        case VK_DOWN :       return Key::Down;
        case VK_NUMPAD0 :    return Key::Numpad0;
        case VK_NUMPAD1 :    return Key::Numpad1;
        case VK_NUMPAD2 :    return Key::Numpad2;
        case VK_NUMPAD3 :    return Key::Numpad3;
        case VK_NUMPAD4 :    return Key::Numpad4;
        case VK_NUMPAD5 :    return Key::Numpad5;
        case VK_NUMPAD6 :    return Key::Numpad6;
        case VK_NUMPAD7 :    return Key::Numpad7;
        case VK_NUMPAD8 :    return Key::Numpad8;
        case VK_NUMPAD9 :    return Key::Numpad9;
        case 'A' :           return Key::A;
        case 'Z' :           return Key::Z;
        case 'E' :           return Key::E;
        case 'R' :           return Key::R;
        case 'T' :           return Key::T;
        case 'Y' :           return Key::Y;
        case 'U' :           return Key::U;
        case 'I' :           return Key::I;
        case 'O' :           return Key::O;
        case 'P' :           return Key::P;
        case 'Q' :           return Key::Q;
        case 'S' :           return Key::S;
        case 'D' :           return Key::D;
        case 'F' :           return Key::F;
        case 'G' :           return Key::G;
        case 'H' :           return Key::H;
        case 'J' :           return Key::J;
        case 'K' :           return Key::K;
        case 'L' :           return Key::L;
        case 'M' :           return Key::M;
        case 'W' :           return Key::W;
        case 'X' :           return Key::X;
        case 'C' :           return Key::C;
        case 'V' :           return Key::V;
        case 'B' :           return Key::B;
        case 'N' :           return Key::N;
        case '0' :           return Key::Num0;
        case '1' :           return Key::Num1;
        case '2' :           return Key::Num2;
        case '3' :           return Key::Num3;
        case '4' :           return Key::Num4;
        case '5' :           return Key::Num5;
        case '6' :           return Key::Num6;
        case '7' :           return Key::Num7;
        case '8' :           return Key::Num8;
        case '9' :           return Key::Num9;
    }

    return Key::Code(0);
}


////////////////////////////////////////////////////////////
bool WindowImplWin32::HasUnicodeSupport()
{
    OSVERSIONINFO version;
    ZeroMemory(&version, sizeof(version));
    version.dwOSVersionInfoSize = sizeof(version);

    if (GetVersionEx(&version))
    {
        return version.dwPlatformId == VER_PLATFORM_WIN32_NT;
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
LRESULT CALLBACK WindowImplWin32::GlobalOnEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Associate handle and Window instance when the creation message is received
    if (message == WM_CREATE)
    {
        // Get WindowImplWin32 instance (it was passed as the last argument of CreateWindow)
        LONG_PTR window = (LONG_PTR)reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams;

        // Set as the "user data" parameter of the window
        SetWindowLongPtr(handle, GWLP_USERDATA, window);
    }

    // Get the WindowImpl instance corresponding to the window handle
    WindowImplWin32* window = reinterpret_cast<WindowImplWin32*>(GetWindowLongPtr(handle, GWLP_USERDATA));

    // Forward the event to the appropriate function
    if (window)
    {
        window->ProcessEvent(message, wParam, lParam);

        if (window->myCallback)
            return CallWindowProc(reinterpret_cast<WNDPROC>(window->myCallback), handle, message, wParam, lParam);
    }

    // We don't forward the WM_CLOSE message to prevent the OS from automatically destroying the window
    if (message == WM_CLOSE)
        return 0;

    static const bool hasUnicode = HasUnicodeSupport();
    return hasUnicode ? DefWindowProcW(handle, message, wParam, lParam) :
                        DefWindowProcA(handle, message, wParam, lParam);
}

} // namespace priv

} // namespace sf
