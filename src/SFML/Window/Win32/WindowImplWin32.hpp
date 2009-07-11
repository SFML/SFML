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
    /// Construct the window implementation from an existing control
    ///
    /// \param handle : Platform-specific handle of the control
    ///
    ////////////////////////////////////////////////////////////
    WindowImplWin32(WindowHandle handle);

    ////////////////////////////////////////////////////////////
    /// Create the window implementation
    ///
    /// \param mode :  Video mode to use
    /// \param title : Title of the window
    /// \param style : Window style
    ///
    ////////////////////////////////////////////////////////////
    WindowImplWin32(VideoMode mode, const std::string& title, unsigned long style);

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~WindowImplWin32();

private :

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::GetHandle
    ///
    ////////////////////////////////////////////////////////////
    virtual WindowHandle GetHandle() const;

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::ProcessEvents
    ///
    ////////////////////////////////////////////////////////////
    virtual void ProcessEvents();

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::ShowMouseCursor
    ///
    ////////////////////////////////////////////////////////////
    virtual void ShowMouseCursor(bool show);

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::SetCursorPosition
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetCursorPosition(unsigned int left, unsigned int top);

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::SetPosition
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetPosition(int left, int top);

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::SetSize
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetSize(unsigned int width, unsigned int height);

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::Show
    ///
    ////////////////////////////////////////////////////////////
    virtual void Show(bool show);

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::EnableKeyRepeat
    ///
    ////////////////////////////////////////////////////////////
    virtual void EnableKeyRepeat(bool enabled);

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::SetIcon
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetIcon(unsigned int width, unsigned int height, const Uint8* pixels);

    ////////////////////////////////////////////////////////////
    /// Register the window class
    ///
    ////////////////////////////////////////////////////////////
    void RegisterWindowClass();

    ////////////////////////////////////////////////////////////
    /// Switch to fullscreen mode
    ///
    /// \param mode : video mode to switch to
    ///
    ////////////////////////////////////////////////////////////
    void SwitchToFullscreen(const VideoMode& mode);

    ////////////////////////////////////////////////////////////
    /// Free all the graphical resources attached to the window
    ///
    ////////////////////////////////////////////////////////////
    void Cleanup();

    ////////////////////////////////////////////////////////////
    /// Process a Win32 event
    ///
    /// \param message : Message to process
    /// \param wParam :  First parameter of the event
    /// \param lParam :  Second parameter of the event
    ///
    ////////////////////////////////////////////////////////////
    void ProcessEvent(UINT message, WPARAM wParam, LPARAM lParam);

    ////////////////////////////////////////////////////////////
    /// Check the state of the shift keys on a key event,
    /// and return the corresponding SF key code
    ///
    /// \param keyDown : True for a keydown event, false for a keyup event
    ///
    /// \return SFML key code corresponding to the shift key
    ///
    ////////////////////////////////////////////////////////////
    static Key::Code GetShiftState(bool keyDown);

    ////////////////////////////////////////////////////////////
    /// Convert a Win32 virtual key code to a SFML key code
    ///
    /// \param key :   Virtual key code to convert
    /// \param flags : Additional flags
    ///
    /// \return SFML key code corresponding to the key
    ///
    ////////////////////////////////////////////////////////////
    static Key::Code VirtualKeyCodeToSF(WPARAM key, LPARAM flags);

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
    /// \param handle :  Win32 handle of the window
    /// \param message : Message received
    /// \param wParam :  First parameter of the message
    /// \param lParam :  Second parameter of the message
    ///
    /// \return Something...
    ///
    ////////////////////////////////////////////////////////////
    static LRESULT CALLBACK GlobalOnEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

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
    HWND    myHandle;           ///< Win32 handle of the window
    long    myCallback;         ///< Stores the original event callback function of the control
    HCURSOR myCursor;           ///< The system cursor to display into the window
    HICON   myIcon;             ///< Custom icon assigned to the window
    bool    myKeyRepeatEnabled; ///< Automatic key-repeat state for keydown events
    bool    myIsCursorIn;       ///< Is the mouse cursor in the window's area ?
};

} // namespace priv

} // namespace sf

#endif // SFML_WINDOWIMPLWIN32_HPP
