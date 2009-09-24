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

#ifndef SFML_WINDOW_HPP
#define SFML_WINDOW_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Input.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowHandle.hpp>
#include <SFML/Window/WindowListener.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <queue>
#include <string>


namespace sf
{
namespace priv
{
    class WindowImpl;
    class ContextGL;
}

////////////////////////////////////////////////////////////
/// Window is a rendering window ; it can create a new window
/// or connect to an existing one
////////////////////////////////////////////////////////////
class SFML_API Window : public WindowListener, NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Window();

    ////////////////////////////////////////////////////////////
    /// Construct a new window
    ///
    /// \param mode :     Video mode to use
    /// \param title :    Title of the window
    /// \param style :    Window style (Resize | Close by default)
    /// \param settings : Additional settings for the underlying OpenGL context (see default constructor for default values)
    ///
    ////////////////////////////////////////////////////////////
    Window(VideoMode mode, const std::string& title, unsigned long style = Style::Resize | Style::Close, const ContextSettings& settings = ContextSettings());

    ////////////////////////////////////////////////////////////
    /// Construct the window from an existing control
    ///
    /// \param handle :   Platform-specific handle of the control
    /// \param settings : Additional settings for the underlying OpenGL context (see default constructor for default values)
    ///
    ////////////////////////////////////////////////////////////
    Window(WindowHandle handle, const ContextSettings& settings = ContextSettings());

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~Window();

    ////////////////////////////////////////////////////////////
    /// Create (or recreate) the window
    ///
    /// \param mode :     Video mode to use
    /// \param title :    Title of the window
    /// \param style :    Window style (Resize | Close by default)
    /// \param Settings : Additional settings for the underlying OpenGL context (see default constructor for default values)
    ///
    ////////////////////////////////////////////////////////////
    void Create(VideoMode mode, const std::string& title, unsigned long style = Style::Resize | Style::Close, const ContextSettings& settings = ContextSettings());

    ////////////////////////////////////////////////////////////
    /// Create (or recreate) the window from an existing control
    ///
    /// \param handle :   Platform-specific handle of the control
    /// \param settings : Additional settings for the underlying OpenGL context (see default constructor for default values)
    ///
    ////////////////////////////////////////////////////////////
    void Create(WindowHandle handle, const ContextSettings& settings = ContextSettings());

    ////////////////////////////////////////////////////////////
    /// Close (destroy) the window.
    /// The sf::Window instance remains valid and you can call
    /// Create to recreate the window
    ///
    ////////////////////////////////////////////////////////////
    void Close();

    ////////////////////////////////////////////////////////////
    /// Tell whether or not the window is opened (ie. has been created).
    /// Note that a hidden window (Show(false))
    /// will still return true
    ///
    /// \return True if the window is opened
    ///
    ////////////////////////////////////////////////////////////
    bool IsOpened() const;

    ////////////////////////////////////////////////////////////
    /// Get the width of the rendering region of the window
    ///
    /// \return Width in pixels
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetWidth() const;

    ////////////////////////////////////////////////////////////
    /// Get the height of the rendering region of the window
    ///
    /// \return Height in pixels
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetHeight() const;

    ////////////////////////////////////////////////////////////
    /// Get the settinsg of the OpenGL context of the window
    ///
    /// \return Structure containing the context settings
    ///
    ////////////////////////////////////////////////////////////
    const ContextSettings& GetSettings() const;

    ////////////////////////////////////////////////////////////
    /// Get the event on top of events stack, if any, and pop it
    ///
    /// \param event : Event to fill, if any
    ///
    /// \return True if an event was returned, false if events stack was empty
    ///
    ////////////////////////////////////////////////////////////
    bool GetEvent(Event& event);

    ////////////////////////////////////////////////////////////
    /// Enable / disable vertical synchronization
    ///
    /// \param enabled : True to enable v-sync, false to deactivate
    ///
    ////////////////////////////////////////////////////////////
    void UseVerticalSync(bool enabled);

    ////////////////////////////////////////////////////////////
    /// Show or hide the mouse cursor
    ///
    /// \param show : True to show, false to hide
    ///
    ////////////////////////////////////////////////////////////
    void ShowMouseCursor(bool show);

    ////////////////////////////////////////////////////////////
    /// Change the position of the mouse cursor
    ///
    /// \param left : Left coordinate of the cursor, relative to the window
    /// \param top :  Top coordinate of the cursor, relative to the window
    ///
    ////////////////////////////////////////////////////////////
    void SetCursorPosition(unsigned int left, unsigned int top);

    ////////////////////////////////////////////////////////////
    /// Change the position of the window on screen.
    /// Only works for top-level windows
    ///
    /// \param left : Left position
    /// \param top :  Top position
    ///
    ////////////////////////////////////////////////////////////
    void SetPosition(int left, int top);

    ////////////////////////////////////////////////////////////
    /// Change the size of the rendering region of the window
    ///
    /// \param width :  New width
    /// \param height : New height
    ///
    ////////////////////////////////////////////////////////////
    void SetSize(unsigned int width, unsigned int height);

    ////////////////////////////////////////////////////////////
    /// Show or hide the window
    ///
    /// \param show : True to show, false to hide
    ///
    ////////////////////////////////////////////////////////////
    void Show(bool show);

    ////////////////////////////////////////////////////////////
    /// Enable or disable automatic key-repeat.
    /// Automatic key-repeat is enabled by default
    ///
    /// \param Enabled : True to enable, false to disable
    ///
    ////////////////////////////////////////////////////////////
    void EnableKeyRepeat(bool Enabled);

    ////////////////////////////////////////////////////////////
    /// Change the window's icon
    ///
    /// \param width :  Icon's width, in pixels
    /// \param height : Icon's height, in pixels
    /// \param pixels : Pointer to the pixels in memory, format must be RGBA 32 bits
    ///
    ////////////////////////////////////////////////////////////
    void SetIcon(unsigned int width, unsigned int height, const Uint8* pixels);

    ////////////////////////////////////////////////////////////
    /// Activate or deactivate the window as the current target
    /// for rendering
    ///
    /// \param active : True to activate, false to deactivate (true by default)
    ///
    /// \return True if operation was successful, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    bool SetActive(bool active = true) const;

    ////////////////////////////////////////////////////////////
    /// Display the window on screen
    ///
    ////////////////////////////////////////////////////////////
    void Display();

    ////////////////////////////////////////////////////////////
    /// Get the input manager of the window
    ///
    /// \return Reference to the input
    ///
    ////////////////////////////////////////////////////////////
    const Input& GetInput() const;

    ////////////////////////////////////////////////////////////
    /// Limit the framerate to a maximum fixed frequency
    ///
    /// \param limit : Framerate limit, in frames per seconds (use 0 to disable limit)
    ///
    ////////////////////////////////////////////////////////////
    void SetFramerateLimit(unsigned int limit);

    ////////////////////////////////////////////////////////////
    /// Get time elapsed since last frame
    ///
    /// \return Time elapsed, in seconds
    ///
    ////////////////////////////////////////////////////////////
    float GetFrameTime() const;

    ////////////////////////////////////////////////////////////
    /// Change the joystick threshold, ie. the value below which
    /// no move event will be generated
    ///
    /// \param threshold : New threshold, in range [0, 100]
    ///
    ////////////////////////////////////////////////////////////
    void SetJoystickThreshold(float threshold);

private :

    ////////////////////////////////////////////////////////////
    /// Called after the window has been created
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnCreate();

    ////////////////////////////////////////////////////////////
    /// Called before the window has been displayed
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnDisplay();

    ////////////////////////////////////////////////////////////
    /// /see WindowListener::OnEvent
    ///
    /// \param event : Event received
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnEvent(const Event& event);

    ////////////////////////////////////////////////////////////
    /// Do some common internal initializations
    ///
    ////////////////////////////////////////////////////////////
    void Initialize();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    priv::WindowImpl* myWindow;         ///< Platform-specific implementation of the window
    priv::ContextGL*  myContext;        ///< Platform-specific implementation of the OpenGL context
    std::queue<Event> myEvents;         ///< Queue of received events
    Input             myInput;          ///< Input manager connected to window
    Clock             myClock;          ///< Clock for measuring the elapsed time between frames
    float             myLastFrameTime;  ///< Time elapsed since last frame
    bool              myIsExternal;     ///< Tell whether the window is internal or external (created by SFML or not)
    unsigned int      myFramerateLimit; ///< Current framerate limit
    int               mySetCursorPosX;  ///< X coordinate passed to the last call to SetCursorPosition
    int               mySetCursorPosY;  ///< Y coordinate passed to the last call to SetCursorPosition
};

} // namespace sf


#endif // SFML_WINDOW_HPP
