////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2012 Laurent Gomila (laurent.gom@gmail.com)
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
    /// \brief Get the OS-specific handle of the window
    ///
    /// \return Handle of the window
    ///
    ////////////////////////////////////////////////////////////
    virtual WindowHandle getSystemHandle() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the position of the window
    ///
    /// \return Position of the window, in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual Vector2i getPosition() const;

    ////////////////////////////////////////////////////////////
    /// \brief Change the position of the window on screen
    ///
    /// \param position New position of the window, in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual void setPosition(const Vector2i& position);

    ////////////////////////////////////////////////////////////
    /// \brief Get the client size of the window
    ///
    /// \return Size of the window, in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual Vector2u getSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Change the size of the rendering region of the window
    ///
    /// \param size New size, in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual void setSize(const Vector2u& size);

    ////////////////////////////////////////////////////////////
    /// \brief Change the title of the window
    ///
    /// \param title New title
    ///
    ////////////////////////////////////////////////////////////
    virtual void setTitle(const std::string& title);

    ////////////////////////////////////////////////////////////
    /// \brief Change the window's icon
    ///
    /// \param width  Icon's width, in pixels
    /// \param height Icon's height, in pixels
    /// \param pixels Pointer to the pixels in memory, format must be RGBA 32 bits
    ///
    ////////////////////////////////////////////////////////////
    virtual void setIcon(unsigned int width, unsigned int height, const Uint8* pixels);

    ////////////////////////////////////////////////////////////
    /// \brief Show or hide the window
    ///
    /// \param visible True to show, false to hide
    ///
    ////////////////////////////////////////////////////////////
    virtual void setVisible(bool visible);

    ////////////////////////////////////////////////////////////
    /// \brief Show or hide the mouse cursor
    ///
    /// \param visible True to show, false to hide
    ///
    ////////////////////////////////////////////////////////////
    virtual void setMouseCursorVisible(bool visible);

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable automatic key-repeat
    ///
    /// \param enabled True to enable, false to disable
    ///
    ////////////////////////////////////////////////////////////
    virtual void setKeyRepeatEnabled(bool enabled);

protected :

    ////////////////////////////////////////////////////////////
    /// \brief Process incoming events from the operating system
    ///
    ////////////////////////////////////////////////////////////
    virtual void processEvents();

private :

    ////////////////////////////////////////////////////////////
    /// \brief Switch to fullscreen mode
    ///
    /// \param Mode video mode to switch to
    ///
    ////////////////////////////////////////////////////////////
    void switchToFullscreen(const VideoMode& mode);

    ////////////////////////////////////////////////////////////
    /// \brief Do some common initializations after the window has been created
    ///
    ////////////////////////////////////////////////////////////
    void initialize();

    ////////////////////////////////////////////////////////////
    /// \brief Create a transparent mouse cursor
    ///
    ////////////////////////////////////////////////////////////
    void createHiddenCursor();

    ////////////////////////////////////////////////////////////
    /// \brief Cleanup graphical resources attached to the window
    ///
    ////////////////////////////////////////////////////////////
    void cleanup();

    ////////////////////////////////////////////////////////////
    /// \brief Process an incoming event from the window
    ///
    /// \param windowEvent Event which has been received
    ///
    /// \return True if the event was processed, false if it was discarded
    ///
    ////////////////////////////////////////////////////////////
    bool processEvent(XEvent windowEvent);

    ////////////////////////////////////////////////////////////
    /// \brief Convert a X11 keysym to SFML key code
    ///
    /// \param symbol Key symbol to convert
    ///
    /// \return Corrsponding SFML key code
    ///
    ////////////////////////////////////////////////////////////
    static Keyboard::Key keysymToSF(KeySym symbol);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    ::Window   m_window;              ///< X11 structure defining our window
    ::Display* m_display;             ///< Pointer to the display
    int        m_screen;              ///< Screen identifier
    XIM        m_inputMethod;         ///< Input method linked to the X display
    XIC        m_inputContext;        ///< Input context used to get unicode input in our window
    bool       m_isExternal;          ///< Tell whether the window has been created externally or by SFML
    Atom       m_atomClose;           ///< Atom used to identify the close event
    int        m_oldVideoMode;        ///< Video mode in use before we switch to fullscreen
    Cursor     m_hiddenCursor;        ///< As X11 doesn't provide cursor hidding, we must create a transparent one
    bool       m_keyRepeat;           ///< Is the KeyRepeat feature enabled ?
    XEvent     m_lastKeyReleaseEvent; ///< Last key release event we received (needed for discarding repeated key events)
};

} // namespace priv

} // namespace sf


#endif // SFML_WINDOWIMPLX11_HPP
