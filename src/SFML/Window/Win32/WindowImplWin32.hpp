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

#ifndef SFML_WINDOWIMPLWIN32_HPP
#define SFML_WINDOWIMPLWIN32_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Event.hpp>
#include <SFML/Window/WindowImpl.hpp>
#include <windows.h>
#include <string>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// WindowImplWin32 is the Win32 implementation of WindowImpl
////////////////////////////////////////////////////////////
class WindowImplWin32 : public WindowImpl
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    /// (creates a dummy window to provide a valid OpenGL context)
    ///
    ////////////////////////////////////////////////////////////
    WindowImplWin32();

    ////////////////////////////////////////////////////////////
    /// Construct the window implementation from an existing control
    ///
    /// \param Handle : Platform-specific handle of the control
    /// \param Params : Creation settings
    ///
    ////////////////////////////////////////////////////////////
    WindowImplWin32(WindowHandle Handle, WindowSettings& Params);

    ////////////////////////////////////////////////////////////
    /// Create the window implementation
    ///
    /// \param Mode :        Video mode to use
    /// \param Title :       Title of the window
    /// \param WindowStyle : Window style
    /// \param Params :      Creation settings
    ///
    ////////////////////////////////////////////////////////////
    WindowImplWin32(VideoMode Mode, const std::string& Title, unsigned long WindowStyle, WindowSettings& Params);

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~WindowImplWin32();

    ////////////////////////////////////////////////////////////
    /// Check if there's an active context on the current thread
    ///
    /// \return True if there's a context bound to the current thread
    ///
    ////////////////////////////////////////////////////////////
    static bool IsContextActive();

private :

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::ProcessEvents
    ///
    ////////////////////////////////////////////////////////////
    virtual void ProcessEvents();

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::Display
    ///
    ////////////////////////////////////////////////////////////
    virtual void Display();

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::SetActive
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetActive(bool Active = true) const;

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::UseVerticalSync
    ///
    ////////////////////////////////////////////////////////////
    virtual void UseVerticalSync(bool Enabled);

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::ShowMouseCursor
    ///
    ////////////////////////////////////////////////////////////
    virtual void ShowMouseCursor(bool Show);

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::SetCursorPosition
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetCursorPosition(unsigned int Left, unsigned int Top);

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::SetPosition
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetPosition(int Left, int Top);

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::SetSize
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetSize(unsigned int Width, unsigned int Height);

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::Show
    ///
    ////////////////////////////////////////////////////////////
    virtual void Show(bool State);

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::EnableKeyRepeat
    ///
    ////////////////////////////////////////////////////////////
    virtual void EnableKeyRepeat(bool Enabled);

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::SetIcon
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetIcon(unsigned int Width, unsigned int Height, const Uint8* Pixels);

    ////////////////////////////////////////////////////////////
    /// Register the window class
    ///
    ////////////////////////////////////////////////////////////
    void RegisterWindowClass();

    ////////////////////////////////////////////////////////////
    /// Switch to fullscreen mode
    ///
    /// \param Mode : video mode to switch to
    ///
    ////////////////////////////////////////////////////////////
    void SwitchToFullscreen(const VideoMode& Mode);

    ////////////////////////////////////////////////////////////
    /// Construct the context from graphics settings
    ///
    /// \param Mode :   Video mode
    /// \param Params : Creation settings
    ///
    ////////////////////////////////////////////////////////////
    void CreateContext(const VideoMode& Mode, WindowSettings& Params);

    ////////////////////////////////////////////////////////////
    /// Free all the graphical resources attached to the window
    ///
    ////////////////////////////////////////////////////////////
    void Cleanup();

    ////////////////////////////////////////////////////////////
    /// Process a Win32 event
    ///
    /// \param Message : Message to process
    /// \param WParam :  First parameter of the event
    /// \param LParam :  Second parameter of the event
    ///
    ////////////////////////////////////////////////////////////
    void ProcessEvent(UINT Message, WPARAM WParam, LPARAM LParam);

    ////////////////////////////////////////////////////////////
    /// Convert a Win32 virtual key code to a SFML key code
    ///
    /// \param VirtualKey : Virtual key code to convert
    /// \param Flags :      Additional flags
    ///
    /// \return SFML key code corresponding to VirtualKey
    ///
    ////////////////////////////////////////////////////////////
    static Key::Code VirtualKeyCodeToSF(WPARAM VirtualKey, LPARAM Flags);

    ////////////////////////////////////////////////////////////
    /// Check if the current version of the OS supports unicode
    /// messages and functions ; Windows 95/98/Me may not support
    /// it, whereas Windows NT/2000/XP/Vista will
    ///
    /// \return True if the OS supports unicode
    ///
    ////////////////////////////////////////////////////////////
    static bool HasUnicodeSupport();

    ////////////////////////////////////////////////////////////
    /// Function called whenever one of our windows receives a message
    ///
    /// \param Handle :  Win32 handle of the window
    /// \param Message : Message received
    /// \param WParam :  First parameter of the message
    /// \param LParam :  Second parameter of the message
    ///
    /// \return Something...
    ///
    ////////////////////////////////////////////////////////////
    static LRESULT CALLBACK GlobalOnEvent(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam);

    ////////////////////////////////////////////////////////////
    // Static member data
    ////////////////////////////////////////////////////////////
    static unsigned int     ourWindowCount;      ///< Number of windows that we own
    static const char*      ourClassNameA;       ///< Win32 window class name (ANSI version)
    static const wchar_t*   ourClassNameW;       ///< Win32 window class name (unicode version)
    static WindowImplWin32* ourFullscreenWindow; ///< Window currently in fullscreen

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    HWND          myHandle;           ///< Win32 handle of the window
    long          myCallback;         ///< Stores the original event callback function of the control
    HCURSOR       myCursor;           ///< The system cursor to display into the window
    HICON         myIcon;             ///< Custom icon assigned to the window
    bool          myKeyRepeatEnabled; ///< Automatic key-repeat state for keydown events
    bool          myIsCursorIn;       ///< Is the mouse cursor in the window's area ?
    HDC           myDeviceContext;    ///< HDC associated to the window
    HGLRC         myGLContext;        ///< OpenGL rendering context associated to the HDC
};

} // namespace priv

} // namespace sf

#endif // SFML_WINDOWIMPLWIN32_HPP
