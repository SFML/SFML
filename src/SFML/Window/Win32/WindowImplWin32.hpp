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
/// \brief Windows implementation of WindowImpl
///
////////////////////////////////////////////////////////////
class WindowImplWin32 : public WindowImpl
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Construct the window implementation from an existing control
    ///
    /// \param handle Platform-specific handle of the control
    ///
    ////////////////////////////////////////////////////////////
    WindowImplWin32(WindowHandle handle);

    ////////////////////////////////////////////////////////////
    /// \brief Create the window implementation
    ///
    /// \param mode  Video mode to use
    /// \param title Title of the window
    /// \param style Window style
    ///
    ////////////////////////////////////////////////////////////
    WindowImplWin32(VideoMode mode, const std::string& title, unsigned long style);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~WindowImplWin32();

private :

    ////////////////////////////////////////////////////////////
    /// \brief Get the OS-specific handle of the window
    ///
    /// \return Handle of the window
    ///
    ////////////////////////////////////////////////////////////
    virtual WindowHandle GetSystemHandle() const;

    ////////////////////////////////////////////////////////////
    /// \brief Process incoming events from the operating system
    ///
    /// \param block Use true to block the thread until an event arrives
    ///
    ////////////////////////////////////////////////////////////
    virtual void ProcessEvents(bool block);

    ////////////////////////////////////////////////////////////
    /// \brief Show or hide the mouse cursor
    ///
    /// \param show True to show, false to hide
    ///
    ////////////////////////////////////////////////////////////
    virtual void ShowMouseCursor(bool show);

    ////////////////////////////////////////////////////////////
    /// \brief Change the position of the mouse cursor
    ///
    /// \param x Left coordinate of the cursor, relative to the window
    /// \param y Top coordinate of the cursor, relative to the window
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetCursorPosition(unsigned int x, unsigned int y);

    ////////////////////////////////////////////////////////////
    /// \brief Change the position of the window on screen
    ///
    /// \param x Left position
    /// \param y Top position
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetPosition(int x, int y);

    ////////////////////////////////////////////////////////////
    /// \brief Change the size of the rendering region of the window
    ///
    /// \param width  New width
    /// \param height New height
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetSize(unsigned int width, unsigned int height);

    ////////////////////////////////////////////////////////////
    /// \brief Change the title of the window
    ///
    /// \param title New title
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetTitle(const std::string& title);

    ////////////////////////////////////////////////////////////
    /// \brief Show or hide the window
    ///
    /// \param show True to show, false to hide
    ///
    ////////////////////////////////////////////////////////////
    virtual void Show(bool show);

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable automatic key-repeat
    ///
    /// \param enabled True to enable, false to disable
    ///
    ////////////////////////////////////////////////////////////
    virtual void EnableKeyRepeat(bool enabled);

    ////////////////////////////////////////////////////////////
    /// \brief Change the window's icon
    ///
    /// \param width  Icon's width, in pixels
    /// \param height Icon's height, in pixels
    /// \param pixels Pointer to the pixels in memory, format must be RGBA 32 bits
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetIcon(unsigned int width, unsigned int height, const Uint8* pixels);

    ////////////////////////////////////////////////////////////
    /// Register the window class
    ///
    ////////////////////////////////////////////////////////////
    void RegisterWindowClass();

    ////////////////////////////////////////////////////////////
    /// \brief Switch to fullscreen mode
    ///
    /// \param mode Video mode to switch to
    ///
    ////////////////////////////////////////////////////////////
    void SwitchToFullscreen(const VideoMode& mode);

    ////////////////////////////////////////////////////////////
    /// \brief Free all the graphical resources attached to the window
    ///
    ////////////////////////////////////////////////////////////
    void Cleanup();

    ////////////////////////////////////////////////////////////
    /// \brief Process a Win32 event
    ///
    /// \param message Message to process
    /// \param wParam  First parameter of the event
    /// \param lParam  Second parameter of the event
    ///
    ////////////////////////////////////////////////////////////
    void ProcessEvent(UINT message, WPARAM wParam, LPARAM lParam);

    ////////////////////////////////////////////////////////////
    /// \brief Convert a Win32 virtual key code to a SFML key code
    ///
    /// \param key   Virtual key code to convert
    /// \param flags Additional flags
    ///
    /// \return SFML key code corresponding to the key
    ///
    ////////////////////////////////////////////////////////////
    static Key::Code VirtualKeyCodeToSF(WPARAM key, LPARAM flags);

    ////////////////////////////////////////////////////////////
    /// \brief Check if the current version of the OS supports
    ///        unicode messages and functions ; Windows 95/98/Me
    ///        may not support it, whereas Windows NT/2000/XP/Vista
    ///        will
    ///
    /// \return True if the OS supports unicode
    ///
    ////////////////////////////////////////////////////////////
    static bool HasUnicodeSupport();

    ////////////////////////////////////////////////////////////
    /// \brief Function called whenever one of our windows receives a message
    ///
    /// \param handle  Win32 handle of the window
    /// \param message Message received
    /// \param wParam  First parameter of the message
    /// \param lParam  Second parameter of the message
    ///
    /// \return True to discard the event after it has been processed
    ///
    ////////////////////////////////////////////////////////////
    static LRESULT CALLBACK GlobalOnEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

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
