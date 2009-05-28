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
#include <GL/glx.h>
#include <set>
#include <string>


namespace sf
{
namespace priv
{
class VideoModeSupport;

////////////////////////////////////////////////////////////
/// WindowImplX11 is the Linux (X11) implementation of WindowImpl
////////////////////////////////////////////////////////////
class WindowImplX11 : public WindowImpl
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    /// (creates a dummy window to provide a valid OpenGL context)
    ///
    ////////////////////////////////////////////////////////////
    WindowImplX11();

    ////////////////////////////////////////////////////////////
    /// Construct the window implementation from an existing control
    ///
    /// \param Handle : Platform-specific handle of the control
    /// \param Params : Creation settings
    ///
    ////////////////////////////////////////////////////////////
    WindowImplX11(WindowHandle Handle, WindowSettings& Params);

    ////////////////////////////////////////////////////////////
    /// Create the window implementation
    ///
    /// \param Mode :        Video mode to use
    /// \param Title :       Title of the window
    /// \param WindowStyle : Window style (resizable, fixed, or fullscren)
    /// \param Params :      Creation settings
    ///
    ////////////////////////////////////////////////////////////
    WindowImplX11(VideoMode Mode, const std::string& Title, unsigned long WindowStyle, WindowSettings& Params);

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~WindowImplX11();

    ////////////////////////////////////////////////////////////
    /// Check if there's an active context on the current thread
    ///
    /// \return True if there's a context bound to the current thread
    ///
    ////////////////////////////////////////////////////////////
    static bool IsContextActive();

private :

    friend class VideoModeSupport;

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::Display
    ///
    ////////////////////////////////////////////////////////////
    virtual void Display();

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::ProcessEvents
    ///
    ////////////////////////////////////////////////////////////
    virtual void ProcessEvents();

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
    /// /see sfWindowImpl::SetCursorPosition
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetCursorPosition(unsigned int Left, unsigned int Top);

    ////////////////////////////////////////////////////////////
    /// /see sfWindowImpl::SetPosition
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetPosition(int Left, int Top);

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::SetSize
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetSize(unsigned int Width, unsigned int Height);

    ////////////////////////////////////////////////////////////
    /// /see sfWindowImpl::Show
    ///
    ////////////////////////////////////////////////////////////
    virtual void Show(bool State);

    ////////////////////////////////////////////////////////////
    /// /see sfWindowImpl::EnableKeyRepeat
    ///
    ////////////////////////////////////////////////////////////
    virtual void EnableKeyRepeat(bool Enabled);

    ////////////////////////////////////////////////////////////
    /// /see WindowImpl::SetIcon
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetIcon(unsigned int Width, unsigned int Height, const Uint8* Pixels);

    ////////////////////////////////////////////////////////////
    /// Switch to fullscreen mode
    ///
    /// \param Mode : video mode to switch to
    ///
    ////////////////////////////////////////////////////////////
    void SwitchToFullscreen(const VideoMode& Mode);

    ////////////////////////////////////////////////////////////
    /// Create the OpenGL rendering context
    ///
    /// \param Mode :         Video mode to use
    /// \param ChosenVisual : Visual that has been chosen for creating the contexte
    /// \param Params :       Creation parameters
    /// \param Template :     Visual infos to match
    /// \param Mask :         Visual attributes to check in Template
    ///
    /// \return True on success, false on error
    ///
    ////////////////////////////////////////////////////////////
    bool CreateContext(const VideoMode& Mode, XVisualInfo& ChosenVisual, WindowSettings& Params, XVisualInfo Template = XVisualInfo(), unsigned long Mask = 0);

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
    /// \param WinEvent : Event which has been received
    ///
    ////////////////////////////////////////////////////////////
    void ProcessEvent(XEvent WinEvent);

    ////////////////////////////////////////////////////////////
    /// Convert a X11 keysym to SFML key code
    ///
    /// \param Sym : keysym to convert
    ///
    /// \return Corrsponding SFML key code
    ///
    ////////////////////////////////////////////////////////////
    static Key::Code KeysymToSF(KeySym Sym);

    ////////////////////////////////////////////////////////////
    /// Open the display (if not already done)
    ///
    /// \param AddWindow : Tell whether or not we must increase the windows count
    ///
    /// \return True if the display is properly opened
    ///
    ////////////////////////////////////////////////////////////
    static bool OpenDisplay(bool AddWindow = true);

    ////////////////////////////////////////////////////////////
    /// Close the display
    ///
    ////////////////////////////////////////////////////////////
    static void CloseDisplay();

    ////////////////////////////////////////////////////////////
    // Static member data
    ////////////////////////////////////////////////////////////
    static ::Display*     ourDisplay;          ///< Current opened display
    static int            ourScreen;           ///< Default screen on the opened display
    static WindowImplX11* ourFullscreenWindow; ///< Keep track of the active fullscreen window
    static unsigned int   ourWindowsCount;     ///< Number of windows created
    static unsigned long  ourEventMask;        ///< Mask defining the events that will be caught by our windows
    static XIM            ourInputMethod;      ///< Input object used to get unicode characters from keypress messages

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    ::Window   myWindow;              ///< X11 structure defining our window
    bool       myIsExternal;          ///< Tell whether the window has been created externally or by SFML
    GLXContext myGLContext;           ///< OpenGL context attached to the window
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
