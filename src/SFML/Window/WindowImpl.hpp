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

#ifndef SFML_WINDOWIMPL_HPP
#define SFML_WINDOWIMPL_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowHandle.hpp>
#include <SFML/Window/WindowSettings.hpp>
#include <set>
#include <string>


namespace sf
{
class Event;
class WindowListener;

namespace priv
{
////////////////////////////////////////////////////////////
/// Abstract base class for OS-specific window implementation
////////////////////////////////////////////////////////////
class WindowImpl : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// Create a new window depending on the current OS
    ///
    /// \return Pointer to the created window
    ///
    ////////////////////////////////////////////////////////////
    static WindowImpl* New();

    ////////////////////////////////////////////////////////////
    /// Create a new window depending on the current OS
    ///
    /// \param Mode :        Video mode to use
    /// \param Title :       Title of the window
    /// \param WindowStyle : Window style
    /// \param Params :      Creation parameters
    ///
    /// \return Pointer to the created window
    ///
    ////////////////////////////////////////////////////////////
    static WindowImpl* New(VideoMode Mode, const std::string& Title, unsigned long WindowStyle, WindowSettings& Params);

    ////////////////////////////////////////////////////////////
    /// Create a new window depending on to the current OS
    ///
    /// \param Handle : Platform-specific handle of the control
    /// \param Params : Creation parameters
    ///
    /// \return Pointer to the created window
    ///
    ////////////////////////////////////////////////////////////
    static WindowImpl* New(WindowHandle Handle, WindowSettings& Params);

public :

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~WindowImpl();

    ////////////////////////////////////////////////////////////
    /// Add a listener to the window
    ///
    /// \param Listener : Listener to add
    ///
    ////////////////////////////////////////////////////////////
    void AddListener(WindowListener* Listener);

    ////////////////////////////////////////////////////////////
    /// Remove a listener from the window
    ///
    /// \param Listener : Listener to remove
    ///
    ////////////////////////////////////////////////////////////
    void RemoveListener(WindowListener* Listener);

    ////////////////////////////////////////////////////////////
    /// Initialize window's states that can't be done at construction
    ///
    ////////////////////////////////////////////////////////////
    void Initialize();

    ////////////////////////////////////////////////////////////
    /// Get the client width of the window
    ///
    /// \return Width of the window in pixels
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetWidth() const;

    ////////////////////////////////////////////////////////////
    /// Get the client height of the window
    ///
    /// \return Height of the window in pixels
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetHeight() const;

    ////////////////////////////////////////////////////////////
    /// Activate of deactivate the window as the current target
    /// for rendering
    ///
    /// \param Active : True to activate, false to deactivate (true by default)
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetActive(bool Active = true) const = 0;

    ////////////////////////////////////////////////////////////
    /// Change the joystick threshold, ie. the value below which
    /// no move event will be generated
    ///
    /// \param Threshold : New threshold, in range [0, 100]
    ///
    ////////////////////////////////////////////////////////////
    void SetJoystickThreshold(float Threshold);

    ////////////////////////////////////////////////////////////
    /// Process incoming events from operating system
    ///
    ////////////////////////////////////////////////////////////
    void DoEvents();

    ////////////////////////////////////////////////////////////
    /// Check if there's an active context on the current thread
    ///
    /// \return True if there's a context bound to the current thread
    ///
    ////////////////////////////////////////////////////////////
    static bool IsContextActive();

    ////////////////////////////////////////////////////////////
    /// Display the window on screen
    ///
    ////////////////////////////////////////////////////////////
    virtual void Display() = 0;

    ////////////////////////////////////////////////////////////
    /// Enable / disable vertical synchronization
    ///
    /// \param Enabled : True to enable v-sync, false to deactivate
    ///
    ////////////////////////////////////////////////////////////
    virtual void UseVerticalSync(bool Enabled) = 0;

    ////////////////////////////////////////////////////////////
    /// Show or hide the mouse cursor
    ///
    /// \param Show : True to show, false to hide
    ///
    ////////////////////////////////////////////////////////////
    virtual void ShowMouseCursor(bool Show) = 0;

    ////////////////////////////////////////////////////////////
    /// Change the position of the mouse cursor
    ///
    /// \param Left : Left coordinate of the cursor, relative to the window
    /// \param Top :  Top coordinate of the cursor, relative to the window
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetCursorPosition(unsigned int Left, unsigned int Top) = 0;

    ////////////////////////////////////////////////////////////
    /// Change the position of the window on screen
    ///
    /// \param Left : Left position
    /// \param Top :  Top position
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetPosition(int Left, int Top) = 0;

    ////////////////////////////////////////////////////////////
    /// Change the size of the rendering region of the window
    ///
    /// \param Width :  New width
    /// \param Height : New height
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetSize(unsigned int Width, unsigned int Height) = 0;

    ////////////////////////////////////////////////////////////
    /// Show or hide the window
    ///
    /// \param State : True to show, false to hide
    ///
    ////////////////////////////////////////////////////////////
    virtual void Show(bool State) = 0;

    ////////////////////////////////////////////////////////////
    /// Enable or disable automatic key-repeat
    ///
    /// \param Enabled : True to enable, false to disable
    ///
    ////////////////////////////////////////////////////////////
    virtual void EnableKeyRepeat(bool Enabled) = 0;

    ////////////////////////////////////////////////////////////
    /// Change the window's icon
    ///
    /// \param Width :  Icon's width, in pixels
    /// \param Height : Icon's height, in pixels
    /// \param Pixels : Pointer to the pixels in memory, format must be RGBA 32 bits
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetIcon(unsigned int Width, unsigned int Height, const Uint8* Pixels) = 0;

protected :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    WindowImpl();

    ////////////////////////////////////////////////////////////
    /// Send an event to listeners (for derived classes only)
    ///
    /// \param EventToSend : Event to send
    ///
    ////////////////////////////////////////////////////////////
    void SendEvent(const Event& EventToSend);

    ////////////////////////////////////////////////////////////
    /// Evaluate a pixel format configuration.
    /// This functions can be used by implementations that have
    /// several valid formats and want to get the best one
    ///
    /// \param Mode :         Requested video mode
    /// \param Settings :     Requested additionnal settings
    /// \param ColorBits :    Color bits of the configuration to evaluate
    /// \param DepthBits :    Depth bits of the configuration to evaluate
    /// \param StencilBits :  Stencil bits of the configuration to evaluate
    /// \param Antialiasing : Antialiasing level of the configuration to evaluate
    ///
    /// \return Score of the configuration : the lower the better
    ///
    ////////////////////////////////////////////////////////////
    static int EvaluateConfig(const VideoMode& Mode, const WindowSettings& Settings, int ColorBits, int DepthBits, int StencilBits, int Antialiasing);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int myWidth;  ///< Internal width of the window
    unsigned int myHeight; ///< Internal height of the window

private :

    ////////////////////////////////////////////////////////////
    /// Read the joysticks state and generate the appropriate events
    ///
    ////////////////////////////////////////////////////////////
    void ProcessJoystickEvents();

    ////////////////////////////////////////////////////////////
    /// Process incoming events from operating system
    ///
    ////////////////////////////////////////////////////////////
    virtual void ProcessEvents() = 0;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::set<WindowListener*> myListeners;             ///< Array of listeners connected to the window
    Joystick                  myJoysticks[Joy::Count]; ///< Joysticks to observe
    JoystickState             myJoyStates[Joy::Count]; ///< Current states of the joysticks
    float                     myJoyThreshold;          ///< Joystick threshold (minimum motion for MOVE event to be generated)
};

} // namespace priv

} // namespace sf


#endif // SFML_WINDOWIMPL_HPP
