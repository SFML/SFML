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
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/Input.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowHandle.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/Window/GlResource.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <string>


namespace sf
{
namespace priv
{
    class GlContext;
    class WindowImpl;
}

class Event;

////////////////////////////////////////////////////////////
/// \brief Window that serves as a target for OpenGL rendering
///
////////////////////////////////////////////////////////////
class SFML_API Window : GlResource, NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructor doesn't actually create the window,
    /// use the other constructors or call Create to do so.
    ///
    ////////////////////////////////////////////////////////////
    Window();

    ////////////////////////////////////////////////////////////
    /// \brief Construct a new window
    ///
    /// This constructor creates the window with the size and pixel
    /// depth defined in \a mode. An optional style can be passed to
    /// customize the look and behaviour of the window (borders,
    /// title bar, resizable, closable, ...). If \a style contains
    /// Style::Fullscreen, then \a mode must be a valid video mode.
    ///
    /// The fourth parameter is an optional structure specifying
    /// advanced OpenGL context settings such as antialiasing,
    /// depth-buffer bits, etc.
    ///
    /// \param mode     Video mode to use (defines the width, height and depth of the rendering area of the window)
    /// \param title    Title of the window
    /// \param style    Window style
    /// \param settings Additional settings for the underlying OpenGL context
    ///
    ////////////////////////////////////////////////////////////
    Window(VideoMode mode, const std::string& title, unsigned long style = Style::Default, const ContextSettings& settings = ContextSettings());

    ////////////////////////////////////////////////////////////
    /// \brief Construct the window from an existing control
    ///
    /// Use this constructor if you want to create an OpenGL
    /// rendering area into an already existing control.
    ///
    /// The second parameter is an optional structure specifying
    /// advanced OpenGL context settings such as antialiasing,
    /// depth-buffer bits, etc.
    ///
    /// \param handle   Platform-specific handle of the control
    /// \param settings Additional settings for the underlying OpenGL context
    ///
    ////////////////////////////////////////////////////////////
    explicit Window(WindowHandle handle, const ContextSettings& settings = ContextSettings());

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// Closes the window and free all the resources attached to it.
    ///
    ////////////////////////////////////////////////////////////
    virtual ~Window();

    ////////////////////////////////////////////////////////////
    /// \brief Create (or recreate) the window
    ///
    /// If the window was already created, it closes it first.
    /// If \a style contains Style::Fullscreen, then \a mode
    /// must be a valid video mode.
    ///
    /// \param mode     Video mode to use (defines the width, height and depth of the rendering area of the window)
    /// \param title    Title of the window
    /// \param style    Window style
    /// \param settings Additional settings for the underlying OpenGL context
    ///
    ////////////////////////////////////////////////////////////
    void Create(VideoMode mode, const std::string& title, unsigned long style = Style::Default, const ContextSettings& settings = ContextSettings());

    ////////////////////////////////////////////////////////////
    /// \brief Create (or recreate) the window from an existing control
    ///
    /// Use this function if you want to create an OpenGL
    /// rendering area into an already existing control.
    /// If the window was already created, it closes it first.
    ///
    /// \param handle   Platform-specific handle of the control
    /// \param settings Additional settings for the underlying OpenGL context
    ///
    ////////////////////////////////////////////////////////////
    void Create(WindowHandle handle, const ContextSettings& settings = ContextSettings());

    ////////////////////////////////////////////////////////////
    /// \brief Close the window and destroy all the attached resources
    ///
    /// After calling this function, the sf::Window instance remains
    /// valid and you can call Create() to recreate the window.
    /// All other functions such as GetEvent() or Display() will
    /// still work (i.e. you don't have to test IsOpened() every time),
    /// and will have no effect on closed windows.
    ///
    ////////////////////////////////////////////////////////////
    void Close();

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether or not the window is opened
    ///
    /// This function returns whether or not the window exists.
    /// Note that a hidden window (Show(false)) will return true.
    ///
    /// \return True if the window is opened, false if it has been closed
    ///
    ////////////////////////////////////////////////////////////
    bool IsOpened() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the width of the rendering region of the window
    ///
    /// The width doesn't include the titlebar and borders
    /// of the window.
    ///
    /// \return Width in pixels
    ///
    /// \see GetHeight
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetWidth() const;

    ////////////////////////////////////////////////////////////
    /// Get the height of the rendering region of the window
    ///
    /// The height doesn't include the titlebar and borders
    /// of the window.
    ///
    /// \return Height in pixels
    ///
    /// \see GetWidth
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetHeight() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the settings of the OpenGL context of the window
    ///
    /// Note that these settings may be different from what was
    /// passed to the constructor or the Create() function,
    /// if one or more settings were not supported. In this case,
    /// SFML chose the closest match.
    ///
    /// \return Structure containing the OpenGL context settings
    ///
    ////////////////////////////////////////////////////////////
    const ContextSettings& GetSettings() const;

    ////////////////////////////////////////////////////////////
    /// \brief Pop the event on top of events stack, if any, and return it
    ///
    /// This function is not blocking: if there's no pending event then
    /// it will return false and leave \a event unmodified.
    /// Note that more than event may be present in the events stack,
    /// thus you should always call this function in a loop
    /// to make sure that you process every pending event.
    /// \code
    /// sf::Event event;
    /// while (window.GetEvent(event))
    /// {
    ///    // process event...
    /// }
    /// \endcode
    ///
    /// \param event Event to be returned
    ///
    /// \return True if an event was returned, or false if the events stack was empty
    ///
    /// \see WaitEvent
    ///
    ////////////////////////////////////////////////////////////
    bool GetEvent(Event& event);

    ////////////////////////////////////////////////////////////
    /// \brief Wait for an event and return it
    ///
    /// This function is blocking: if there's no pending event then
    /// it will wait until an event is received.
    /// After this function returns (and no error occured),
    /// the \a event object is always valid and filled properly.
    /// This function is typically used when you have a thread that
    /// is dedicated to events handling: you want to make this thread
    /// sleep as long as no new event is received.
    /// \code
    /// sf::Event event;
    /// if (window.WaitEvent(event))
    /// {
    ///    // process event...
    /// }
    /// \endcode
    ///
    /// \param event Event to be returned
    ///
    /// \return False if any error occured
    ///
    /// \see GetEvent
    ///
    ////////////////////////////////////////////////////////////
    bool WaitEvent(Event& event);

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable vertical synchronization
    ///
    /// Activating vertical synchronization will limit the number
    /// of frames displayed to the refresh rate of the monitor.
    /// This can avoid some visual artifacts, and limit the framerate
    /// to a good value (but not constant across different computers).
    ///
    /// Vertical synchronization is disabled by default.
    ///
    /// \param enabled True to enable v-sync, false to deactivate
    ///
    ////////////////////////////////////////////////////////////
    void EnableVerticalSync(bool enabled);

    ////////////////////////////////////////////////////////////
    /// \brief Show or hide the mouse cursor
    ///
    /// The mouse cursor is shown by default.
    ///
    /// \param show True to show, false to hide
    ///
    ////////////////////////////////////////////////////////////
    void ShowMouseCursor(bool show);

    ////////////////////////////////////////////////////////////
    /// \brief Change the position of the mouse cursor
    ///
    /// \param x Left coordinate of the cursor, relative to the window
    /// \param y Top coordinate of the cursor, relative to the window
    ///
    ////////////////////////////////////////////////////////////
    void SetCursorPosition(unsigned int x, unsigned int y);

    ////////////////////////////////////////////////////////////
    /// \brief Change the position of the window on screen
    ///
    /// This function only works for top-level windows
    /// (i.e. it will be ignored for windows created from
    /// the handle of a child window/control).
    ///
    /// \param x Left position
    /// \param y Top position
    ///
    ////////////////////////////////////////////////////////////
    void SetPosition(int x, int y);

    ////////////////////////////////////////////////////////////
    /// \brief Change the size of the rendering region of the window
    ///
    /// \param width  New width, in pixels
    /// \param height New height, in pixels
    ///
    ////////////////////////////////////////////////////////////
    void SetSize(unsigned int width, unsigned int height);

    ////////////////////////////////////////////////////////////
    /// \brief Change the title of the window
    ///
    /// \param title New title
    ///
    ////////////////////////////////////////////////////////////
    void SetTitle(const std::string& title);

    ////////////////////////////////////////////////////////////
    /// \brief Show or hide the window
    ///
    /// The window is shown by default.
    ///
    /// \param show True to show, false to hide
    ///
    ////////////////////////////////////////////////////////////
    void Show(bool show);

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable automatic key-repeat
    ///
    /// If key repeat is enabled, you will receive repeated
    /// KeyPress events while keeping a key pressed. If it is disabled,
    /// you will only get a single event when the key is pressed.
    ///
    /// Key repeat is enabled by default.
    ///
    /// \param enabled True to enable, false to disable
    ///
    ////////////////////////////////////////////////////////////
    void EnableKeyRepeat(bool enabled);

    ////////////////////////////////////////////////////////////
    /// \brief Change the window's icon
    ///
    /// \a pixels must be an array of \a width x \a height pixels
    /// in 32-bits RGBA format.
    ///
    /// The OS default icon is used by default.
    ///
    /// \param width  Icon's width, in pixels
    /// \param height Icon's height, in pixels
    /// \param pixels Pointer to the array of pixels in memory
    ///
    ////////////////////////////////////////////////////////////
    void SetIcon(unsigned int width, unsigned int height, const Uint8* pixels);

    ////////////////////////////////////////////////////////////
    /// \brief Activate or deactivate the window as the current target
    ///        for OpenGL rendering
    ///
    /// A window is active only on the current thread, if you want to
    /// make it active on another thread you have to deactivate it
    /// on the previous thread first if it was active.
    /// Only one window can be active on a thread at a time, thus
    /// the window previously active (if any) automatically gets deactivated.
    ///
    /// \param active True to activate, false to deactivate
    ///
    /// \return True if operation was successful, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    bool SetActive(bool active = true) const;

    ////////////////////////////////////////////////////////////
    /// \brief Display on screen what has been rendered to the
    ///        window so far
    ///
    /// This function is typically called after all OpenGL rendering
    /// has been done for the current frame, in order to show
    /// it on screen.
    ///
    ////////////////////////////////////////////////////////////
    void Display();

    ////////////////////////////////////////////////////////////
    /// \brief Get the input manager attached the window
    ///
    /// This input gives access to the real-time state of
    /// keyboard, mouse and joysticks for this window.
    ///
    /// \return Read-only reference to the input manager
    ///
    ////////////////////////////////////////////////////////////
    const Input& GetInput() const;

    ////////////////////////////////////////////////////////////
    /// \brief Limit the framerate to a maximum fixed frequency
    ///
    /// If a limit is set, the window will use a small delay after
    /// each call to Display() to ensure that the current frame
    /// lasted long enough to match the framerate limit.
    ///
    /// \param limit Framerate limit, in frames per seconds (use 0 to disable limit)
    ///
    ////////////////////////////////////////////////////////////
    void SetFramerateLimit(unsigned int limit);

    ////////////////////////////////////////////////////////////
    /// \brief Get the duration of the last frame
    ///
    /// This function returns the time elapsed between the last
    /// two calls to Display().
    /// This can be useful for calculating the framerate, or for
    /// updating the application's objects.
    ///
    /// \return Time elapsed in last frame, in seconds
    ///
    ////////////////////////////////////////////////////////////
    float GetFrameTime() const;

    ////////////////////////////////////////////////////////////
    /// \brief Change the joystick threshold
    ///
    /// The joystick threshold is the value below which
    /// no JoyMoved event will be generated.
    ///
    /// The threshold value is 0.1 by default.
    ///
    /// \param threshold New threshold, in the range [0, 100]
    ///
    ////////////////////////////////////////////////////////////
    void SetJoystickThreshold(float threshold);

    ////////////////////////////////////////////////////////////
    /// \brief Get the OS-specific handle of the window
    ///
    /// The type of the returned handle is sf::WindowHandle,
    /// which is a typedef to the handle type defined by the OS.
    /// You shouldn't need to use this function, unless you have
    /// very specific stuff to implement that SFML doesn't support,
    /// or implement a temporary workaround until a bug is fixed.
    ///
    /// \return System handle of the window
    ///
    ////////////////////////////////////////////////////////////
    WindowHandle GetSystemHandle() const;

private :

    ////////////////////////////////////////////////////////////
    /// \brief Function called after the window has been created
    ///
    /// This function is called so that derived classes can
    /// perform their own specific initialization as soon as
    /// the window is created.
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnCreate();

    ////////////////////////////////////////////////////////////
    /// \brief Function called after the window has been resized
    ///
    /// This function is called so that derived classes can
    /// perform custom actions when the size of the window changes.
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnResize();

    ////////////////////////////////////////////////////////////
    /// \brief Processes an event before it is sent to the user
    ///
    /// This function is called every time an event is received
    /// from the internal window (through GetEvent or WaitEvent).
    /// It filters out unwanted events, and performs whatever internal
    /// stuff the window needs before the event is returned to the
    /// user.
    ///
    /// \param event Event to filter
    ///
    ////////////////////////////////////////////////////////////
    bool FilterEvent(const Event& event);

    ////////////////////////////////////////////////////////////
    /// \brief Perform some common internal initializations
    ///
    ////////////////////////////////////////////////////////////
    void Initialize();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    priv::WindowImpl* myWindow;         ///< Platform-specific implementation of the window
    priv::GlContext*  myContext;        ///< Platform-specific implementation of the OpenGL context
    Input             myInput;          ///< Input manager connected to window
    Clock             myClock;          ///< Clock for measuring the elapsed time between frames
    float             myLastFrameTime;  ///< Time elapsed since last frame
    unsigned int      myFramerateLimit; ///< Current framerate limit
    int               mySetCursorPosX;  ///< X coordinate passed to the last call to SetCursorPosition
    int               mySetCursorPosY;  ///< Y coordinate passed to the last call to SetCursorPosition
};

} // namespace sf


#endif // SFML_WINDOW_HPP


////////////////////////////////////////////////////////////
/// \class sf::Window
/// \ingroup window
///
/// sf::Window is the main class of the Window module. It defines
/// an OS window that is able to receive an OpenGL rendering.
///
/// A sf::Window can create its own new window, or be embedded into
/// an already existing control using the Create(handle) function.
/// This can be useful for embedding an OpenGL rendering area into
/// a view which is part of a bigger GUI with existing windows,
/// controls, etc. It can also serve as embedding an OpenGL rendering
/// area into a window created by another (probably richer) GUI library
/// like Qt or wxWidgets.
///
/// The sf::Window class provides a simple interface for manipulating
/// the window: move, resize, show/hide, control mouse cursor, etc.
/// It also provides event handling through its GetEvent() function,
/// and real-time state handling with its attached sf::Input object
/// (see GetInput()).
///
/// Note that OpenGL experts can pass their own parameters (antialiasing
/// level, bits for the depth and stencil buffers, etc.) to the
/// OpenGL context attached to the window, with the sf::ContextSettings
/// structure which is passed as an optional argument when creating the
/// window.
///
/// Usage example:
/// \code
/// // Declare and create a new window
/// sf::Window window(sf::VideoMode(800, 600), "SFML window");
///
/// // Limit the framerate to 60 frames per second (this step is optional)
/// window.SetFramerateLimit(60);
///
/// // The main loop - ends as soon as the window is closed
/// while (window.IsOpened())
/// {
///    // Event processing
///    sf::Event event;
///    while (window.GetEvent(event))
///    {
///        // Request for closing the window
///        if (event.Type == sf::Event::Closed)
///            window.Close();
///    }
///
///    // Activate the window for OpenGL rendering
///    window.SetActive();
///
///    // OpenGL drawing commands go here...
///
///    // End the current frame and display its contents on screen
///    window.Display();
/// }
/// \endcode
///
////////////////////////////////////////////////////////////
