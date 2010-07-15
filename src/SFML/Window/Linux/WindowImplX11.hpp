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

#ifndef SFML_WINDOWIMPLX11_HPP
#define SFML_WINDOWIMPLX11_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Event.hpp>
#include <SFML/Window/WindowImpl.hpp>
#include <X11/Xlib.h>
#include <set>
#include <string>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Linux (X11) implementation of WindowImpl
///
////////////////////////////////////////////////////////////
class WindowImplX11 : public WindowImpl
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Construct the window implementation from an existing control
    ///
    /// \param handle Platform-specific handle of the control
    ///
    ////////////////////////////////////////////////////////////
    WindowImplX11(WindowHandle handle);

    ////////////////////////////////////////////////////////////
    /// \brief Create the window implementation
    ///
    /// \param mode  Video mode to use
    /// \param title Title of the window
    /// \param style Window style (resizable, fixed, or fullscren)
    ///
    ////////////////////////////////////////////////////////////
    WindowImplX11(VideoMode mode, const std::string& title, unsigned long style);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~WindowImplX11();

    ////////////////////////////////////////////////////////////
    /// \brief Get the display used by the window
    ///
    /// This functions is meant to be used internally by ContextGLX.
    ///
    /// \return Pointer to the X display of the window
    ///
    ////////////////////////////////////////////////////////////
    ::Display* GetDisplay() const;

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
    /// \brief Switch to fullscreen mode
    ///
    /// \param Mode video mode to switch to
    ///
    ////////////////////////////////////////////////////////////
    void SwitchToFullscreen(const VideoMode& mode);

    ////////////////////////////////////////////////////////////
    /// \brief Do some common initializations after the window has been created
    ///
    ////////////////////////////////////////////////////////////
    void Initialize();

    ////////////////////////////////////////////////////////////
    /// \brief Create a transparent mouse cursor
    ///
    ////////////////////////////////////////////////////////////
    void CreateHiddenCursor();

    ////////////////////////////////////////////////////////////
    /// \brief Cleanup graphical resources attached to the window
    ///
    ////////////////////////////////////////////////////////////
    void CleanUp();

    ////////////////////////////////////////////////////////////
    /// \brief Process an incoming event from the window
    ///
    /// \param windowEvent Event which has been received
    ///
    /// \return True if the event was processed, false if it was discarded
    ///
    ////////////////////////////////////////////////////////////
    bool ProcessEvent(XEvent windowEvent);

    ////////////////////////////////////////////////////////////
    /// \brief Convert a X11 keysym to SFML key code
    ///
    /// \param symbol Key symbol to convert
    ///
    /// \return Corrsponding SFML key code
    ///
    ////////////////////////////////////////////////////////////
    static Key::Code KeysymToSF(KeySym symbol);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    ::Window   myWindow;              ///< X11 structure defining our window
    ::Display* myDisplay;             ///< Pointer to the display
    int        myScreen;              ///< Screen identifier
    XIM        myInputMethod;         ///< Input method linked to the X display
    XIC        myInputContext;        ///< Input context used to get unicode input in our window
    bool       myIsExternal;          ///< Tell whether the window has been created externally or by SFML
    Atom       myAtomClose;           ///< Atom used to identify the close event
    int        myOldVideoMode;        ///< Video mode in use before we switch to fullscreen
    Cursor     myHiddenCursor;        ///< As X11 doesn't provide cursor hidding, we must create a transparent one
    bool       myKeyRepeat;           ///< Is the KeyRepeat feature enabled ?
    XEvent     myLastKeyReleaseEvent; ///< Last key release event we received (needed for discarding repeated key events)
};

} // namespace priv

} // namespace sf


#endif // SFML_WINDOWIMPLX11_HPP
