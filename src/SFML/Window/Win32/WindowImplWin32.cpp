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
#include <SFML/Window/WindowSettings.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <GL/gl.h>
#include <SFML/Window/glext/wglext.h>
#include <SFML/Window/glext/glext.h>
#include <iostream>
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


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
// Static member data
////////////////////////////////////////////////////////////
unsigned int     WindowImplWin32::ourWindowCount      = 0;
const char*      WindowImplWin32::ourClassNameA       = "SFML_Window";
const wchar_t*   WindowImplWin32::ourClassNameW       = L"SFML_Window";
WindowImplWin32* WindowImplWin32::ourFullscreenWindow = NULL;


////////////////////////////////////////////////////////////
/// Default constructor
/// (creates a dummy window to provide a valid OpenGL context)
////////////////////////////////////////////////////////////
WindowImplWin32::WindowImplWin32() :
myHandle          (NULL),
myCallback        (0),
myCursor          (NULL),
myIcon            (NULL),
myKeyRepeatEnabled(true),
myIsCursorIn      (false)
{
    // Register the window class at first call
    if (ourWindowCount == 0)
        RegisterWindowClass();

    // Use small dimensions
    myWidth  = 1;
    myHeight = 1;

    // Create a dummy window (disabled and hidden)
    if (HasUnicodeSupport())
    {
        myHandle = CreateWindowW(ourClassNameW, L"", WS_POPUP | WS_DISABLED, 0, 0, myWidth, myHeight, NULL, NULL, GetModuleHandle(NULL), NULL);
    }
    else
    {
        myHandle = CreateWindowA(ourClassNameA, "", WS_POPUP | WS_DISABLED, 0, 0, myWidth, myHeight, NULL, NULL, GetModuleHandle(NULL), NULL);
    }
    ShowWindow(myHandle, SW_HIDE);

    // Create the rendering context
    if (myHandle)
    {
        WindowSettings Params(0, 0, 0);
        CreateContext(VideoMode(myWidth, myHeight, 32), Params);

        // Don't activate by default
        SetActive(false);
    }
}


////////////////////////////////////////////////////////////
/// Create the window implementation from an existing control
////////////////////////////////////////////////////////////
WindowImplWin32::WindowImplWin32(WindowHandle Handle, WindowSettings& Params) :
myHandle          (NULL),
myCallback        (0),
myCursor          (NULL),
myIcon            (NULL),
myKeyRepeatEnabled(true),
myIsCursorIn      (false)
{
    // Save window handle
    myHandle = static_cast<HWND>(Handle);

    if (myHandle)
    {
        // Get window client size
        RECT Rect;
        GetClientRect(myHandle, &Rect);
        myWidth  = Rect.right - Rect.left;
        myHeight = Rect.bottom - Rect.top;

        // Create the rendering context
        VideoMode Mode(myWidth, myHeight, VideoMode::GetDesktopMode().BitsPerPixel);
        CreateContext(Mode, Params);

        // We change the event procedure of the control (it is important to save the old one)
        SetWindowLongPtr(myHandle, GWLP_USERDATA, reinterpret_cast<long>(this));
        myCallback = SetWindowLongPtr(myHandle, GWLP_WNDPROC, reinterpret_cast<long>(&WindowImplWin32::GlobalOnEvent));
    }
}


////////////////////////////////////////////////////////////
/// Create the window implementation
////////////////////////////////////////////////////////////
WindowImplWin32::WindowImplWin32(VideoMode Mode, const std::string& Title, unsigned long WindowStyle, WindowSettings& Params) :
myHandle          (NULL),
myCallback        (0),
myCursor          (NULL),
myIcon            (NULL),
myKeyRepeatEnabled(true),
myIsCursorIn      (false)
{
    // Register the window class at first call
    if (ourWindowCount == 0)
        RegisterWindowClass();

    // Compute position and size
    HDC ScreenDC = GetDC(NULL);
    int Left   = (GetDeviceCaps(ScreenDC, HORZRES) - Mode.Width)  / 2;
    int Top    = (GetDeviceCaps(ScreenDC, VERTRES) - Mode.Height) / 2;
    int Width  = myWidth  = Mode.Width;
    int Height = myHeight = Mode.Height;
    ReleaseDC(NULL, ScreenDC);

    // Choose the window style according to the Style parameter
    DWORD Win32Style = WS_VISIBLE;
    if (WindowStyle == Style::None)
    {
        Win32Style |= WS_POPUP;
    }
    else
    {
        if (WindowStyle & Style::Titlebar) Win32Style |= WS_CAPTION | WS_MINIMIZEBOX;
        if (WindowStyle & Style::Resize)   Win32Style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
        if (WindowStyle & Style::Close)    Win32Style |= WS_SYSMENU;
    }

    // In windowed mode, adjust width and height so that window will have the requested client area
    bool Fullscreen = (WindowStyle & Style::Fullscreen) != 0;
    if (!Fullscreen)
    {
        RECT Rect = {0, 0, Width, Height};
        AdjustWindowRect(&Rect, Win32Style, false);
        Width  = Rect.right - Rect.left;
        Height = Rect.bottom - Rect.top;
    }

    // Create the window
    if (HasUnicodeSupport())
    {
        wchar_t WTitle[256];
        int NbChars = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Title.c_str(), static_cast<int>(Title.size()), WTitle, sizeof(WTitle) / sizeof(*WTitle));
        WTitle[NbChars] = L'\0';
        myHandle = CreateWindowW(ourClassNameW, WTitle, Win32Style, Left, Top, Width, Height, NULL, NULL, GetModuleHandle(NULL), this);
    }
    else
    {
        myHandle = CreateWindowA(ourClassNameA, Title.c_str(), Win32Style, Left, Top, Width, Height, NULL, NULL, GetModuleHandle(NULL), this);
    }

    // Switch to fullscreen if requested
    if (Fullscreen)
        SwitchToFullscreen(Mode);

    // Create the rendering context
    if (myHandle)
        CreateContext(Mode, Params);

    // Increment window count
    ourWindowCount++;

    // Get the actual size of the window, which can be smaller even after the call to AdjustWindowRect
    // This happens when the window is bigger than the desktop
    RECT ActualRect;
    GetClientRect(myHandle, &ActualRect);
    myWidth  = ActualRect.right - ActualRect.left;
    myHeight = ActualRect.bottom - ActualRect.top;
}


////////////////////////////////////////////////////////////
/// Destructor
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
        ourWindowCount--;

        // Unregister window class if we were the last window
        if (ourWindowCount == 0)
        {
            if (HasUnicodeSupport())
            {
                UnregisterClassW(ourClassNameW, GetModuleHandle(NULL));
            }
            else
            {
                UnregisterClassA(ourClassNameA, GetModuleHandle(NULL));
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
/// Check if there's an active context on the current thread
////////////////////////////////////////////////////////////
bool WindowImplWin32::IsContextActive()
{
    return wglGetCurrentContext() != NULL;
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::ProcessEvents
////////////////////////////////////////////////////////////
void WindowImplWin32::ProcessEvents()
{
    // We update the window only if we own it
    if (!myCallback)
    {
        MSG Message;
        while (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }
    }
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::Display
////////////////////////////////////////////////////////////
void WindowImplWin32::Display()
{
    if (myDeviceContext && myGLContext)
        SwapBuffers(myDeviceContext);
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetActive
////////////////////////////////////////////////////////////
void WindowImplWin32::SetActive(bool Active) const
{
    if (Active)
    {
        if (myDeviceContext && myGLContext && (wglGetCurrentContext() != myGLContext))
            wglMakeCurrent(myDeviceContext, myGLContext);
    }
    else
    {
        if (wglGetCurrentContext() == myGLContext)
            wglMakeCurrent(NULL, NULL);
    }
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::UseVerticalSync
////////////////////////////////////////////////////////////
void WindowImplWin32::UseVerticalSync(bool Enabled)
{
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
    if (wglSwapIntervalEXT)
        wglSwapIntervalEXT(Enabled ? 1 : 0);
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::ShowMouseCursor
////////////////////////////////////////////////////////////
void WindowImplWin32::ShowMouseCursor(bool Show)
{
    if (Show)
        myCursor = LoadCursor(NULL, IDC_ARROW);
    else
        myCursor = NULL;

    SetCursor(myCursor);
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetCursorPosition
////////////////////////////////////////////////////////////
void WindowImplWin32::SetCursorPosition(unsigned int Left, unsigned int Top)
{
    POINT Pos = {Left, Top};
    ClientToScreen(myHandle, &Pos);
    SetCursorPos(Pos.x, Pos.y);
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetPosition
////////////////////////////////////////////////////////////
void WindowImplWin32::SetPosition(int Left, int Top)
{
    SetWindowPos(myHandle, NULL, Left, Top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetSize
////////////////////////////////////////////////////////////
void WindowImplWin32::SetSize(unsigned int Width, unsigned int Height)
{
    // SetWindowPos wants the total size of the window (including title bar and borders),
    // so we have to compute it
    RECT Rect = {0, 0, Width, Height};
    AdjustWindowRect(&Rect, GetWindowLong(myHandle, GWL_STYLE), false);
    Width  = Rect.right - Rect.left;
    Height = Rect.bottom - Rect.top;

    SetWindowPos(myHandle, NULL, 0, 0, Width, Height, SWP_NOMOVE | SWP_NOZORDER);
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::Show
////////////////////////////////////////////////////////////
void WindowImplWin32::Show(bool State)
{
    ShowWindow(myHandle, State ? SW_SHOW : SW_HIDE);
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::EnableKeyRepeat
////////////////////////////////////////////////////////////
void WindowImplWin32::EnableKeyRepeat(bool Enabled)
{
    myKeyRepeatEnabled = Enabled;
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetIcon
////////////////////////////////////////////////////////////
void WindowImplWin32::SetIcon(unsigned int Width, unsigned int Height, const Uint8* Pixels)
{
    // First destroy the previous one
    if (myIcon)
        DestroyIcon(myIcon);

    // Windows wants BGRA pixels : swap red and blue channels
    std::vector<Uint8> IconPixels(Width * Height * 4);
    for (std::size_t i = 0; i < IconPixels.size() / 4; ++i)
    {
        IconPixels[i * 4 + 0] = Pixels[i * 4 + 2];
        IconPixels[i * 4 + 1] = Pixels[i * 4 + 1];
        IconPixels[i * 4 + 2] = Pixels[i * 4 + 0];
        IconPixels[i * 4 + 3] = Pixels[i * 4 + 3];
    }

    // Create the icon from the pixels array
    myIcon = CreateIcon(GetModuleHandle(NULL), Width, Height, 1, 32, NULL, &IconPixels[0]);

    // Set it as both big and small icon of the window
    if (myIcon)
    {
        SendMessage(myHandle, WM_SETICON, ICON_BIG,   (LPARAM)myIcon);
        SendMessage(myHandle, WM_SETICON, ICON_SMALL, (LPARAM)myIcon);
    }
    else
    {
        std::cerr << "Failed to set the window's icon" << std::endl;
    }
}


////////////////////////////////////////////////////////////
/// Register the window class
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
        WindowClass.lpszClassName = ourClassNameW;
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
        WindowClass.lpszClassName = ourClassNameA;
        RegisterClassA(&WindowClass);
    }
}


////////////////////////////////////////////////////////////
/// Switch to fullscreen mode
////////////////////////////////////////////////////////////
void WindowImplWin32::SwitchToFullscreen(const VideoMode& Mode)
{
    DEVMODE DevMode;
    DevMode.dmSize       = sizeof(DEVMODE);
    DevMode.dmPelsWidth  = Mode.Width;
    DevMode.dmPelsHeight = Mode.Height;
    DevMode.dmBitsPerPel = Mode.BitsPerPixel;
    DevMode.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

    // Apply fullscreen mode
    if (ChangeDisplaySettings(&DevMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
    {
        std::cerr << "Failed to change display mode for fullscreen" << std::endl;
        return;
    }

    // Make the window flags compatible with fullscreen mode
    SetWindowLong(myHandle, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
    SetWindowLong(myHandle, GWL_EXSTYLE, WS_EX_APPWINDOW);

    // Resize the window so that it fits the entire screen
    SetWindowPos(myHandle, HWND_TOP, 0, 0, Mode.Width, Mode.Height, SWP_FRAMECHANGED);
    ShowWindow(myHandle, SW_SHOW);

    // Set "this" as the current fullscreen window
    ourFullscreenWindow = this;
}


////////////////////////////////////////////////////////////
/// Construct the context from graphics settings
////////////////////////////////////////////////////////////
void WindowImplWin32::CreateContext(const VideoMode& Mode, WindowSettings& Params)
{
    // Get the device context attached to the window
    myDeviceContext = GetDC(myHandle);
    if (myDeviceContext == NULL)
    {
        std::cerr << "Failed to get device context of window -- cannot create OpenGL context" << std::endl;
        return;
    }

    // Let's find a suitable pixel format -- first try with antialiasing
    int BestFormat = 0;
    if (Params.AntialiasingLevel > 0)
    {
        // Get the wglChoosePixelFormatARB function (it is an extension)
        PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
        if (wglChoosePixelFormatARB)
        {
            // Define the basic attributes we want for our window
            int IntAttributes[] =
            {
                WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		        WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
		        WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
                WGL_SAMPLE_BUFFERS_ARB, (Params.AntialiasingLevel ? GL_TRUE : GL_FALSE),
		        WGL_SAMPLES_ARB,        Params.AntialiasingLevel,
		        0,                      0
            };

            // Let's check how many formats are supporting our requirements
            int   Formats[128];
	        UINT  NbFormats;
	        float FloatAttributes[] = {0, 0};
	        bool  IsValid = wglChoosePixelFormatARB(myDeviceContext, IntAttributes, FloatAttributes, sizeof(Formats) / sizeof(*Formats), Formats, &NbFormats) != 0;
            if (!IsValid || (NbFormats == 0))
            {
                if (Params.AntialiasingLevel > 2)
                {
                    // No format matching our needs : reduce the multisampling level
                    std::cerr << "Failed to find a pixel format supporting "
                              << Params.AntialiasingLevel << " antialiasing levels ; trying with 2 levels" << std::endl;

                    Params.AntialiasingLevel = IntAttributes[11] = 2;
	                IsValid = wglChoosePixelFormatARB(myDeviceContext, IntAttributes, FloatAttributes, sizeof(Formats) / sizeof(*Formats), Formats, &NbFormats) != 0;
                }

                if (!IsValid || (NbFormats == 0))
                {
                    // Cannot find any pixel format supporting multisampling ; disabling antialiasing
                    std::cerr << "Failed to find a pixel format supporting antialiasing ; antialiasing will be disabled" << std::endl;
                    Params.AntialiasingLevel = 0;
                }
            }

            // Get the best format among the returned ones
            if (IsValid && (NbFormats > 0))
            {
                int BestScore = 0xFFFF;
                for (UINT i = 0; i < NbFormats; ++i)
                {
                    // Get the current format's attributes
                    PIXELFORMATDESCRIPTOR Attribs;
                    Attribs.nSize    = sizeof(PIXELFORMATDESCRIPTOR);
                    Attribs.nVersion = 1;
                    DescribePixelFormat(myDeviceContext, Formats[i], sizeof(PIXELFORMATDESCRIPTOR), &Attribs);

                    // Evaluate the current configuration
                    int Color = Attribs.cRedBits + Attribs.cGreenBits + Attribs.cBlueBits + Attribs.cAlphaBits;
                    int Score = EvaluateConfig(Mode, Params, Color, Attribs.cDepthBits, Attribs.cStencilBits, Params.AntialiasingLevel);

                    // Keep it if it's better than the current best
                    if (Score < BestScore)
                    {
                        BestScore  = Score;
                        BestFormat = Formats[i];
                    }
                }
            }
        }
        else
        {
            // wglChoosePixelFormatARB not supported ; disabling antialiasing
            std::cerr << "Antialiasing is not supported ; it will be disabled" << std::endl;
            Params.AntialiasingLevel = 0;
        }
    }

    // Find a pixel format with no antialiasing, if not needed or not supported
    if (BestFormat == 0)
    {
        // Setup a pixel format descriptor from the rendering settings
        PIXELFORMATDESCRIPTOR PixelDescriptor;
        ZeroMemory(&PixelDescriptor, sizeof(PIXELFORMATDESCRIPTOR));
        PixelDescriptor.nSize        = sizeof(PIXELFORMATDESCRIPTOR);
        PixelDescriptor.nVersion     = 1;
        PixelDescriptor.iLayerType   = PFD_MAIN_PLANE;
        PixelDescriptor.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        PixelDescriptor.iPixelType   = PFD_TYPE_RGBA;
        PixelDescriptor.cColorBits   = static_cast<BYTE>(Mode.BitsPerPixel);
        PixelDescriptor.cDepthBits   = static_cast<BYTE>(Params.DepthBits);
        PixelDescriptor.cStencilBits = static_cast<BYTE>(Params.StencilBits);
        PixelDescriptor.cAlphaBits   = Mode.BitsPerPixel == 32 ? 8 : 0;

        // Get the pixel format that best matches our requirements
        BestFormat = ChoosePixelFormat(myDeviceContext, &PixelDescriptor);
        if (BestFormat == 0)
        {
            std::cerr << "Failed to find a suitable pixel format for device context -- cannot create OpenGL context" << std::endl;
            return;
        }
    }

    // Extract the depth and stencil bits from the chosen format
    PIXELFORMATDESCRIPTOR ActualFormat;
    ActualFormat.nSize    = sizeof(PIXELFORMATDESCRIPTOR);
    ActualFormat.nVersion = 1;
    DescribePixelFormat(myDeviceContext, BestFormat, sizeof(PIXELFORMATDESCRIPTOR), &ActualFormat);
    Params.DepthBits   = ActualFormat.cDepthBits;
    Params.StencilBits = ActualFormat.cStencilBits;

    // Set the chosen pixel format
    if (!SetPixelFormat(myDeviceContext, BestFormat, &ActualFormat))
    {
        std::cerr << "Failed to set pixel format for device context -- cannot create OpenGL context" << std::endl;
        return;
    }

    // Create the OpenGL context from the device context
    myGLContext = wglCreateContext(myDeviceContext);
    if (myGLContext == NULL)
    {
        std::cerr << "Failed to create an OpenGL context for this window" << std::endl;
        return;
    }

    // Share display lists with other contexts
    HGLRC CurrentContext = wglGetCurrentContext();
    if (CurrentContext)
        wglShareLists(CurrentContext, myGLContext);

    // Activate the context
    SetActive(true);

    // Enable multisampling
    if (Params.AntialiasingLevel > 0)
        glEnable(GL_MULTISAMPLE_ARB);
}


////////////////////////////////////////////////////////////
/// Free all the graphical resources attached to the window
////////////////////////////////////////////////////////////
void WindowImplWin32::Cleanup()
{
    // Restore the previous video mode (in case we were running in fullscreen)
    if (ourFullscreenWindow == this)
    {
        ChangeDisplaySettings(NULL, 0);
        ourFullscreenWindow = NULL;
    }

    // Unhide the mouse cursor (in case it was hidden)
    ShowMouseCursor(true);

    // Destroy the OpenGL context
    if (myGLContext)
    {
        // Unbind the context before destroying it
        SetActive(false);

        wglDeleteContext(myGLContext);
        myGLContext = NULL;
    }
    if (myDeviceContext)
    {
        ReleaseDC(myHandle, myDeviceContext);
        myDeviceContext = NULL;
    }
}


////////////////////////////////////////////////////////////
/// Process a Win32 event
////////////////////////////////////////////////////////////
void WindowImplWin32::ProcessEvent(UINT Message, WPARAM WParam, LPARAM LParam)
{
    // Don't process any message until window is created
    if (myHandle == NULL)
        return;

    switch (Message)
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
            if (LOWORD(LParam) == HTCLIENT)
                SetCursor(myCursor);

            break;
        }

        // Close event
        case WM_CLOSE :
        {
            Event Evt;
            Evt.Type = Event::Closed;
            SendEvent(Evt);
            break;
        }

        // Resize event
        case WM_SIZE :
        {
            // Update window size
            RECT Rect;
            GetClientRect(myHandle, &Rect);
            myWidth  = Rect.right - Rect.left;
            myHeight = Rect.bottom - Rect.top;

            Event Evt;
            Evt.Type        = Event::Resized;
            Evt.Size.Width  = myWidth;
            Evt.Size.Height = myHeight;
            SendEvent(Evt);
            break;
        }

        // Gain focus event
        case WM_SETFOCUS :
        {
            Event Evt;
            Evt.Type = Event::GainedFocus;
            SendEvent(Evt);
            break;
        }

        // Lost focus event
        case WM_KILLFOCUS :
        {
            Event Evt;
            Evt.Type = Event::LostFocus;
            SendEvent(Evt);
            break;
        }

        // Text event
        case WM_CHAR :
        {
            if (myKeyRepeatEnabled || ((LParam & (1 << 30)) == 0))
            {
                Event Evt;
                Evt.Type = Event::TextEntered;
                Evt.Text.Unicode = static_cast<Uint32>(WParam);
                SendEvent(Evt);
            }
            break;
        }

        // Keydown event
        case WM_KEYDOWN :
        case WM_SYSKEYDOWN :
        {
            if (myKeyRepeatEnabled || ((HIWORD(LParam) & KF_REPEAT) == 0))
            {
                Event Evt;
                Evt.Type        = Event::KeyPressed;
                Evt.Key.Alt     = HIWORD(GetAsyncKeyState(VK_MENU))    != 0;
                Evt.Key.Control = HIWORD(GetAsyncKeyState(VK_CONTROL)) != 0;
                Evt.Key.Shift   = HIWORD(GetAsyncKeyState(VK_SHIFT))   != 0;
                Evt.Key.Code    = VirtualKeyCodeToSF(WParam, LParam);
                SendEvent(Evt);
            }
            break;
        }

        // Keyup event
        case WM_KEYUP :
        case WM_SYSKEYUP :
        {
            Event Evt;
            Evt.Type        = Event::KeyReleased;
            Evt.Key.Alt     = HIWORD(GetAsyncKeyState(VK_MENU))    != 0;
            Evt.Key.Control = HIWORD(GetAsyncKeyState(VK_CONTROL)) != 0;
            Evt.Key.Shift   = HIWORD(GetAsyncKeyState(VK_SHIFT))   != 0;
            Evt.Key.Code    = VirtualKeyCodeToSF(WParam, LParam);
            SendEvent(Evt);

            break;
        }

        // Mouse wheel event
        case WM_MOUSEWHEEL :
        {
            Event Evt;
            Evt.Type = Event::MouseWheelMoved;
            Evt.MouseWheel.Delta = static_cast<Int16>(HIWORD(WParam)) / 120;
            SendEvent(Evt);
            break;
        }

        // Mouse left button down event
        case WM_LBUTTONDOWN :
        {
            Event Evt;
            Evt.Type               = Event::MouseButtonPressed;
            Evt.MouseButton.Button = Mouse::Left;
            Evt.MouseButton.X      = LOWORD(LParam);
            Evt.MouseButton.Y      = HIWORD(LParam);
            SendEvent(Evt);
            break;
        }

        // Mouse left button up event
        case WM_LBUTTONUP :
        {
            Event Evt;
            Evt.Type               = Event::MouseButtonReleased;
            Evt.MouseButton.Button = Mouse::Left;
            Evt.MouseButton.X      = LOWORD(LParam);
            Evt.MouseButton.Y      = HIWORD(LParam);
            SendEvent(Evt);
            break;
        }

        // Mouse right button down event
        case WM_RBUTTONDOWN :
        {
            Event Evt;
            Evt.Type               = Event::MouseButtonPressed;
            Evt.MouseButton.Button = Mouse::Right;
            Evt.MouseButton.X      = LOWORD(LParam);
            Evt.MouseButton.Y      = HIWORD(LParam);
            SendEvent(Evt);
            break;
        }

        // Mouse right button up event
        case WM_RBUTTONUP :
        {
            Event Evt;
            Evt.Type               = Event::MouseButtonReleased;
            Evt.MouseButton.Button = Mouse::Right;
            Evt.MouseButton.X      = LOWORD(LParam);
            Evt.MouseButton.Y      = HIWORD(LParam);
            SendEvent(Evt);
            break;
        }

        // Mouse wheel button down event
        case WM_MBUTTONDOWN :
        {
            Event Evt;
            Evt.Type               = Event::MouseButtonPressed;
            Evt.MouseButton.Button = Mouse::Middle;
            Evt.MouseButton.X      = LOWORD(LParam);
            Evt.MouseButton.Y      = HIWORD(LParam);
            SendEvent(Evt);
            break;
        }

        // Mouse wheel button up event
        case WM_MBUTTONUP :
        {
            Event Evt;
            Evt.Type               = Event::MouseButtonReleased;
            Evt.MouseButton.Button = Mouse::Middle;
            Evt.MouseButton.X      = LOWORD(LParam);
            Evt.MouseButton.Y      = HIWORD(LParam);
            SendEvent(Evt);
            break;
        }

        // Mouse X button down event
        case WM_XBUTTONDOWN :
        {
            Event Evt;
            Evt.Type               = Event::MouseButtonPressed;
            Evt.MouseButton.Button = HIWORD(WParam) == XBUTTON1 ? Mouse::XButton1 : Mouse::XButton2;
            Evt.MouseButton.X      = LOWORD(LParam);
            Evt.MouseButton.Y      = HIWORD(LParam);
            SendEvent(Evt);
            break;
        }

        // Mouse X button up event
        case WM_XBUTTONUP :
        {
            Event Evt;
            Evt.Type               = Event::MouseButtonReleased;
            Evt.MouseButton.Button = HIWORD(WParam) == XBUTTON1 ? Mouse::XButton1 : Mouse::XButton2;
            Evt.MouseButton.X      = LOWORD(LParam);
            Evt.MouseButton.Y      = HIWORD(LParam);
            SendEvent(Evt);
            break;
        }

        // Mouse move event
        case WM_MOUSEMOVE :
        {
            // Check if we need to generate a MouseEntered event
            if (!myIsCursorIn)
            {
                TRACKMOUSEEVENT MouseEvent;
                MouseEvent.cbSize    = sizeof(TRACKMOUSEEVENT);
                MouseEvent.hwndTrack = myHandle;
                MouseEvent.dwFlags   = TME_LEAVE;
                TrackMouseEvent(&MouseEvent);

                myIsCursorIn = true;

                Event Evt;
                Evt.Type = Event::MouseEntered;
                SendEvent(Evt);
            }

            Event Evt;
            Evt.Type        = Event::MouseMoved;
            Evt.MouseMove.X = LOWORD(LParam);
            Evt.MouseMove.Y = HIWORD(LParam);
            SendEvent(Evt);
            break;
        }

        // Mouse leave event
        case WM_MOUSELEAVE :
        {
            myIsCursorIn = false;

            Event Evt;
            Evt.Type = Event::MouseLeft;
            SendEvent(Evt);
            break;
        }
    }
}


////////////////////////////////////////////////////////////
/// Convert a Win32 virtual key code to a SFML key code
////////////////////////////////////////////////////////////
Key::Code WindowImplWin32::VirtualKeyCodeToSF(WPARAM VirtualKey, LPARAM Flags)
{
    switch (VirtualKey)
    {
        // Check the scancode to distinguish between left and right shift
        case VK_SHIFT :
        {
            static UINT LShift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
            UINT scancode = (Flags & (0xFF << 16)) >> 16;
            return scancode == LShift ? Key::LShift : Key::RShift;
        }

        // Check the "extended" flag to distinguish between left and right alt
        case VK_MENU : return (HIWORD(Flags) & KF_EXTENDED) ? Key::RAlt : Key::LAlt;

        // Check the "extended" flag to distinguish between left and right control
        case VK_CONTROL : return (HIWORD(Flags) & KF_EXTENDED) ? Key::RControl : Key::LControl;

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
/// Check if the current version of the OS supports unicode
/// messages and functions ; Windows 95/98/Me may not support
/// it, whereas Windows NT/2000/XP/Vista will
////////////////////////////////////////////////////////////
bool WindowImplWin32::HasUnicodeSupport()
{
    OSVERSIONINFO VersionInfo;
    ZeroMemory(&VersionInfo, sizeof(VersionInfo));
    VersionInfo.dwOSVersionInfoSize = sizeof(VersionInfo);

    if (GetVersionEx(&VersionInfo))
    {
        return VersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT;
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
/// Function called whenever one of our windows receives a message
////////////////////////////////////////////////////////////
LRESULT CALLBACK WindowImplWin32::GlobalOnEvent(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam)
{
    // Associate handle and Window instance when the creation message is received
    if (Message == WM_CREATE)
    {
        // Get WindowImplWin32 instance (it was passed as the last argument of CreateWindow)
        long This = reinterpret_cast<long>(reinterpret_cast<CREATESTRUCT*>(LParam)->lpCreateParams);

        // Set as the "user data" parameter of the window
        SetWindowLongPtr(Handle, GWLP_USERDATA, This);
    }

    // Get the WindowImpl instance corresponding to the window handle
    WindowImplWin32* Window = reinterpret_cast<WindowImplWin32*>(GetWindowLongPtr(Handle, GWLP_USERDATA));

    // Forward the event to the appropriate function
    if (Window)
    {
        Window->ProcessEvent(Message, WParam, LParam);

        if (Window->myCallback)
            return CallWindowProc(reinterpret_cast<WNDPROC>(Window->myCallback), Handle, Message, WParam, LParam);
    }

    // We don't forward the WM_CLOSE message to prevent the OS from automatically destroying the window
    if (Message == WM_CLOSE)
        return 0;

    static const bool HasUnicode = HasUnicodeSupport();
    return HasUnicode ? DefWindowProcW(Handle, Message, WParam, LParam) :
                        DefWindowProcA(Handle, Message, WParam, LParam);
}

} // namespace priv

} // namespace sf
