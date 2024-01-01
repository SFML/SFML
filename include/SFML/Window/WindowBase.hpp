////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Export.hpp>

#include <SFML/Window/Vulkan.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Window/WindowHandle.hpp>

#include <SFML/System/Vector2.hpp>

#include <memory>
#include <optional>


namespace sf
{
class Cursor;
class String;
class VideoMode;

namespace priv
{
class WindowImpl;
}

struct Event;

////////////////////////////////////////////////////////////
/// \brief Window that serves as a base for other windows
///
////////////////////////////////////////////////////////////
class SFML_WINDOW_API WindowBase
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructor doesn't actually create the window,
    /// use the other constructors or call create() to do so.
    ///
    ////////////////////////////////////////////////////////////
    WindowBase();

    ////////////////////////////////////////////////////////////
    /// \brief Construct a new window
    ///
    /// This constructor creates the window with the size and pixel
    /// depth defined in \a mode. An optional style can be passed to
    /// customize the look and behavior of the window (borders,
    /// title bar, resizable, closable, ...). An optional state can
    /// be provided. If \a state is State::Fullscreen, then \a mode
    /// must be a valid video mode.
    ///
    /// \param mode  Video mode to use (defines the width, height and depth of the rendering area of the window)
    /// \param title Title of the window
    /// \param style %Window style, a bitwise OR combination of sf::Style enumerators
    /// \param state %Window state
    ///
    ////////////////////////////////////////////////////////////
    WindowBase(VideoMode mode, const String& title, std::uint32_t style = Style::Default, State state = State::Windowed);

    ////////////////////////////////////////////////////////////
    /// \brief Construct a new window
    ///
    /// This constructor creates the window with the size and pixel
    /// depth defined in \a mode.
    ///
    /// \param mode  Video mode to use (defines the width, height and depth of the rendering area of the window)
    /// \param title Title of the window
    /// \param state %Window state
    ///
    ////////////////////////////////////////////////////////////
    WindowBase(VideoMode mode, const String& title, State state);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the window from an existing control
    ///
    /// \param handle Platform-specific handle of the control
    ///
    ////////////////////////////////////////////////////////////
    explicit WindowBase(WindowHandle handle);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// Closes the window and frees all the resources attached to it.
    ///
    ////////////////////////////////////////////////////////////
    virtual ~WindowBase();

    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy constructor
    ///
    ////////////////////////////////////////////////////////////
    WindowBase(const WindowBase&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy assignment
    ///
    ////////////////////////////////////////////////////////////
    WindowBase& operator=(const WindowBase&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Create (or recreate) the window
    ///
    /// If the window was already created, it closes it first.
    /// If \a state is State::Fullscreen, then \a mode must be
    /// a valid video mode.
    ///
    /// \param mode  Video mode to use (defines the width, height and depth of the rendering area of the window)
    /// \param title Title of the window
    /// \param style %Window style, a bitwise OR combination of sf::Style enumerators
    /// \param state %Window state
    ///
    ////////////////////////////////////////////////////////////
    virtual void create(VideoMode mode, const String& title, std::uint32_t style = Style::Default, State state = State::Windowed);

    ////////////////////////////////////////////////////////////
    /// \brief Create (or recreate) the window from an existing control
    ///
    /// \param handle Platform-specific handle of the control
    ///
    ////////////////////////////////////////////////////////////
    virtual void create(WindowHandle handle);

    ////////////////////////////////////////////////////////////
    /// \brief Close the window and destroy all the attached resources
    ///
    /// After calling this function, the sf::Window instance remains
    /// valid and you can call create() to recreate the window.
    /// All other functions such as pollEvent() or display() will
    /// still work (i.e. you don't have to test isOpen() every time),
    /// and will have no effect on closed windows.
    ///
    ////////////////////////////////////////////////////////////
    virtual void close();

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether or not the window is open
    ///
    /// This function returns whether or not the window exists.
    /// Note that a hidden window (setVisible(false)) is open
    /// (therefore this function would return true).
    ///
    /// \return True if the window is open, false if it has been closed
    ///
    ////////////////////////////////////////////////////////////
    bool isOpen() const;

    ////////////////////////////////////////////////////////////
    /// \brief Pop the next event from the front of the FIFO event queue, if any, and return it
    ///
    /// This function is not blocking: if there's no pending event then
    /// it will return false and leave \a event unmodified.
    /// Note that more than one event may be present in the event queue,
    /// thus you should always call this function in a loop
    /// to make sure that you process every pending event.
    /// \code
    /// for (sf::Event event; window.pollEvent(event);)
    /// {
    ///    // process event...
    /// }
    /// \endcode
    ///
    /// \param event Event to be returned
    ///
    /// \return True if an event was returned, or false if the event queue was empty
    ///
    /// \see waitEvent
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool pollEvent(Event& event);

    ////////////////////////////////////////////////////////////
    /// \brief Wait for an event and return it
    ///
    /// This function is blocking: if there's no pending event then
    /// it will wait until an event is received.
    /// After this function returns (and no error occurred),
    /// the \a event object is always valid and filled properly.
    /// This function is typically used when you have a thread that
    /// is dedicated to events handling: you want to make this thread
    /// sleep as long as no new event is received.
    /// \code
    /// sf::Event event;
    /// if (window.waitEvent(event))
    /// {
    ///    // process event...
    /// }
    /// \endcode
    ///
    /// \param event Event to be returned
    ///
    /// \return False if any error occurred
    ///
    /// \see pollEvent
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool waitEvent(Event& event);

    ////////////////////////////////////////////////////////////
    /// \brief Get the position of the window
    ///
    /// \return Position of the window, in pixels
    ///
    /// \see setPosition
    ///
    ////////////////////////////////////////////////////////////
    Vector2i getPosition() const;

    ////////////////////////////////////////////////////////////
    /// \brief Change the position of the window on screen
    ///
    /// This function only works for top-level windows
    /// (i.e. it will be ignored for windows created from
    /// the handle of a child window/control).
    ///
    /// \param position New position, in pixels
    ///
    /// \see getPosition
    ///
    ////////////////////////////////////////////////////////////
    void setPosition(const Vector2i& position);

    ////////////////////////////////////////////////////////////
    /// \brief Get the size of the rendering region of the window
    ///
    /// The size doesn't include the titlebar and borders
    /// of the window.
    ///
    /// \return Size in pixels
    ///
    /// \see setSize
    ///
    ////////////////////////////////////////////////////////////
    Vector2u getSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Change the size of the rendering region of the window
    ///
    /// \param size New size, in pixels
    ///
    /// \see getSize
    ///
    ////////////////////////////////////////////////////////////
    void setSize(const Vector2u& size);

    ////////////////////////////////////////////////////////////
    /// \brief Set the minimum window rendering region size
    ///
    /// Pass std::nullopt to unset the minimum size
    ///
    /// \param minimumSize New minimum size, in pixels
    ///
    ////////////////////////////////////////////////////////////
    void setMinimumSize(const std::optional<Vector2u>& minimumSize);

    ////////////////////////////////////////////////////////////
    /// \brief Set the maximum window rendering region size
    ///
    /// Pass std::nullopt to unset the maximum size
    ///
    /// \param maximumSize New maximum size, in pixels
    ///
    ////////////////////////////////////////////////////////////
    void setMaximumSize(const std::optional<Vector2u>& maximumSize);

    ////////////////////////////////////////////////////////////
    /// \brief Change the title of the window
    ///
    /// \param title New title
    ///
    /// \see setIcon
    ///
    ////////////////////////////////////////////////////////////
    void setTitle(const String& title);

    ////////////////////////////////////////////////////////////
    /// \brief Change the window's icon
    ///
    /// \a pixels must be an array of \a width x \a height pixels
    /// in 32-bits RGBA format.
    ///
    /// The OS default icon is used by default.
    ///
    /// \param size   Icon's width and height, in pixels
    /// \param pixels Pointer to the array of pixels in memory. The
    ///               pixels are copied, so you need not keep the
    ///               source alive after calling this function.
    ///
    /// \see setTitle
    ///
    ////////////////////////////////////////////////////////////
    void setIcon(const Vector2u& size, const std::uint8_t* pixels);

    ////////////////////////////////////////////////////////////
    /// \brief Show or hide the window
    ///
    /// The window is shown by default.
    ///
    /// \param visible True to show the window, false to hide it
    ///
    ////////////////////////////////////////////////////////////
    void setVisible(bool visible);

    ////////////////////////////////////////////////////////////
    /// \brief Show or hide the mouse cursor
    ///
    /// The mouse cursor is visible by default.
    ///
    /// \param visible True to show the mouse cursor, false to hide it
    ///
    ////////////////////////////////////////////////////////////
    void setMouseCursorVisible(bool visible);

    ////////////////////////////////////////////////////////////
    /// \brief Grab or release the mouse cursor
    ///
    /// If set, grabs the mouse cursor inside this window's client
    /// area so it may no longer be moved outside its bounds.
    /// Note that grabbing is only active while the window has
    /// focus.
    ///
    /// \param grabbed True to enable, false to disable
    ///
    ////////////////////////////////////////////////////////////
    void setMouseCursorGrabbed(bool grabbed);

    ////////////////////////////////////////////////////////////
    /// \brief Set the displayed cursor to a native system cursor
    ///
    /// Upon window creation, the arrow cursor is used by default.
    ///
    /// \warning The cursor must not be destroyed while in use by
    ///          the window.
    ///
    /// \warning Features related to Cursor are not supported on
    ///          iOS and Android.
    ///
    /// \param cursor Native system cursor type to display
    ///
    /// \see sf::Cursor::loadFromSystem
    /// \see sf::Cursor::loadFromPixels
    ///
    ////////////////////////////////////////////////////////////
    void setMouseCursor(const Cursor& cursor);

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable automatic key-repeat
    ///
    /// If key repeat is enabled, you will receive repeated
    /// KeyPressed events while keeping a key pressed. If it is disabled,
    /// you will only get a single event when the key is pressed.
    ///
    /// Key repeat is enabled by default.
    ///
    /// \param enabled True to enable, false to disable
    ///
    ////////////////////////////////////////////////////////////
    void setKeyRepeatEnabled(bool enabled);

    ////////////////////////////////////////////////////////////
    /// \brief Change the joystick threshold
    ///
    /// The joystick threshold is the value below which
    /// no JoystickMoved event will be generated.
    ///
    /// The threshold value is 0.1 by default.
    ///
    /// \param threshold New threshold, in the range [0, 100]
    ///
    ////////////////////////////////////////////////////////////
    void setJoystickThreshold(float threshold);

    ////////////////////////////////////////////////////////////
    /// \brief Request the current window to be made the active
    ///        foreground window
    ///
    /// At any given time, only one window may have the input focus
    /// to receive input events such as keystrokes or mouse events.
    /// If a window requests focus, it only hints to the operating
    /// system, that it would like to be focused. The operating system
    /// is free to deny the request.
    /// This is not to be confused with setActive().
    ///
    /// \see hasFocus
    ///
    ////////////////////////////////////////////////////////////
    void requestFocus();

    ////////////////////////////////////////////////////////////
    /// \brief Check whether the window has the input focus
    ///
    /// At any given time, only one window may have the input focus
    /// to receive input events such as keystrokes or most mouse
    /// events.
    ///
    /// \return True if window has focus, false otherwise
    /// \see requestFocus
    ///
    ////////////////////////////////////////////////////////////
    bool hasFocus() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the OS-specific handle of the window
    ///
    /// The type of the returned handle is sf::WindowHandle,
    /// which is a type alias to the handle type defined by the OS.
    /// You shouldn't need to use this function, unless you have
    /// very specific stuff to implement that SFML doesn't support,
    /// or implement a temporary workaround until a bug is fixed.
    ///
    /// \return System handle of the window
    ///
    ////////////////////////////////////////////////////////////
    WindowHandle getNativeHandle() const;

    ////////////////////////////////////////////////////////////
    /// \brief Create a Vulkan rendering surface
    ///
    /// \param instance  Vulkan instance
    /// \param surface   Created surface
    /// \param allocator Allocator to use
    ///
    /// \return True if surface creation was successful, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool createVulkanSurface(const VkInstance&            instance,
                                           VkSurfaceKHR&                surface,
                                           const VkAllocationCallbacks* allocator = nullptr);

protected:
    ////////////////////////////////////////////////////////////
    /// \brief Function called after the window has been created
    ///
    /// This function is called so that derived classes can
    /// perform their own specific initialization as soon as
    /// the window is created.
    ///
    ////////////////////////////////////////////////////////////
    virtual void onCreate();

    ////////////////////////////////////////////////////////////
    /// \brief Function called after the window has been resized
    ///
    /// This function is called so that derived classes can
    /// perform custom actions when the size of the window changes.
    ///
    ////////////////////////////////////////////////////////////
    virtual void onResize();

private:
    friend class Window;

    ////////////////////////////////////////////////////////////
    /// \brief Create (or recreate) the window
    ///
    /// Implementation detail for sharing underlying implementation
    /// with sf::Window
    ///
    /// \param mode  Video mode to use (defines the width, height and depth of the rendering area of the window)
    /// \param style %Window style, a bitwise OR combination of sf::Style enumerators
    /// \param state %Window state
    ///
    ////////////////////////////////////////////////////////////
    void create(VideoMode mode, std::uint32_t& style, State& state);

    ////////////////////////////////////////////////////////////
    /// \brief Processes an event before it is sent to the user
    ///
    /// This function is called every time an event is received
    /// from the internal window (through pollEvent or waitEvent).
    /// It filters out unwanted events, and performs whatever internal
    /// stuff the window needs before the event is returned to the
    /// user.
    ///
    /// \param event Event to filter
    ///
    ////////////////////////////////////////////////////////////
    void filterEvent(const Event& event);

    ////////////////////////////////////////////////////////////
    /// \brief Perform some common internal initializations
    ///
    ////////////////////////////////////////////////////////////
    void initialize();

    ////////////////////////////////////////////////////////////
    /// \brief Get the fullscreen window
    ///
    /// \return The fullscreen window or a null pointer if there is none
    ///
    ////////////////////////////////////////////////////////////
    const WindowBase* getFullscreenWindow();

    ////////////////////////////////////////////////////////////
    /// \brief Set a window as the fullscreen window
    ///
    /// \param window Window to set as fullscreen window
    ///
    ////////////////////////////////////////////////////////////
    void setFullscreenWindow(const WindowBase* window);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::unique_ptr<priv::WindowImpl> m_impl; //!< Platform-specific implementation of the window
    Vector2u                          m_size; //!< Current size of the window
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::WindowBase
/// \ingroup window
///
/// sf::WindowBase serves as the base class for all Windows.
///
/// A sf::WindowBase can create its own new window, or be embedded into
/// an already existing control using the create(handle) function.
///
/// The sf::WindowBase class provides a simple interface for manipulating
/// the window: move, resize, show/hide, control mouse cursor, etc.
/// It also provides event handling through its pollEvent() and waitEvent()
/// functions.
///
/// Usage example:
/// \code
/// // Declare and create a new window
/// sf::WindowBase window(sf::VideoMode({800, 600}), "SFML window");
///
/// // The main loop - ends as soon as the window is closed
/// while (window.isOpen())
/// {
///    // Event processing
///    for (sf::Event event; window.pollEvent(event);)
///    {
///        // Request for closing the window
///        if (event.type == sf::Event::Closed)
///            window.close();
///    }
///
///    // Do things with the window here...
/// }
/// \endcode
///
////////////////////////////////////////////////////////////
