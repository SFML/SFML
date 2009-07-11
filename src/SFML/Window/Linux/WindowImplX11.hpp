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
#include <SFML/Window/Linux/DisplayRef.hpp>
#include <X11/Xlib.h>
#include <set>
#include <string>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// WindowImplX11 is the Linux (X11) implementation of WindowImpl
////////////////////////////////////////////////////////////
class WindowImplX11 : public WindowImpl
{
public :

    ////////////////////////////////////////////////////////////
    /// Construct the window implementation from an existing control
    ///
    /// \param handle : Platform-specific handle of the control
    ///
    ////////////////////////////////////////////////////////////
    WindowImplX11(WindowHandle handle);

    ////////////////////////////////////////////////////////////
    /// Create the window implementation
    ///
    /// \param mode :  Video mode to use
    /// \param title : Title of the window
    /// \param style : Window style (resizable, fixed, or fullscren)
    ///
    ////////////////////////////////////////////////////////////
    WindowImplX11(VideoMode mode, const std::string& title, unsigned long style);

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~WindowImplX11();

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
    /// /see sfWindowImpl::SetCursorPosition
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetCursorPosition(unsigned int left, unsigned int top);

    ////////////////////////////////////////////////////////////
    /// /see sfWindowImpl::SetPosition
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetPosition(int left, int top);

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::SetSize
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetSize(unsigned int width, unsigned int height);

    ////////////////////////////////////////////////////////////
    /// /see sfWindowImpl::Show
    ///
    ////////////////////////////////////////////////////////////
    virtual void Show(bool show);

    ////////////////////////////////////////////////////////////
    /// /see sfWindowImpl::EnableKeyRepeat
    ///
    ////////////////////////////////////////////////////////////
    virtual void EnableKeyRepeat(bool enabled);

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::SetIcon
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetIcon(unsigned int width, unsigned int height, const Uint8* pixels);

    ////////////////////////////////////////////////////////////
    /// Switch to fullscreen mode
    ///
    /// \param Mode : video mode to switch to
    ///
    ////////////////////////////////////////////////////////////
    void SwitchToFullscreen(const VideoMode& mode);

    ////////////////////////////////////////////////////////////
    /// Do some common initializations after the window has been created
    ///
    ////////////////////////////////////////////////////////////
    void Initialize();

    ////////////////////////////////////////////////////////////
    /// Create a transparent mouse cursor
    ///
    ////////////////////////////////////////////////////////////
    void CreateHiddenCursor();

    ////////////////////////////////////////////////////////////
    /// Cleanup graphical resources attached to the window
    ///
    ////////////////////////////////////////////////////////////
    void CleanUp();

    ////////////////////////////////////////////////////////////
    /// Process an incoming event from the window
    ///
    /// \param windowEvent : Event which has been received
    ///
    ////////////////////////////////////////////////////////////
    void ProcessEvent(XEvent windowEvent);

    ////////////////////////////////////////////////////////////
    /// Convert a X11 keysym to SFML key code
    ///
    /// \param symbol : keysym to convert
    ///
    /// \return Corrsponding SFML key code
    ///
    ////////////////////////////////////////////////////////////
    static Key::Code KeysymToSF(KeySym symbol);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    DisplayRef myDisplayRef;          ///< Connection to the X server
    ::Window   myWindow;              ///< X11 structure defining our window
    ::Display* myDisplay;             ///< Pointer to the display
    int        myScreen;              ///< Screen identifier
    bool       myIsExternal;          ///< Tell whether the window has been created externally or by SFML
    Atom       myAtomClose;           ///< Atom used to identify the close event
    int        myOldVideoMode;        ///< Video mode in use before we switch to fullscreen
    Cursor     myHiddenCursor;        ///< As X11 doesn't provide cursor hidding, we must create a transparent one
    XIC        myInputContext;        ///< Input context used to get unicode input in our window
    bool       myKeyRepeat;           ///< Is the KeyRepeat feature enabled ?
    XEvent     myLastKeyReleaseEvent; ///< Last key release event we received (needed for discarding repeated key events)
};

} // namespace priv

} // namespace sf


#endif // SFML_WINDOWIMPLX11_HPP
