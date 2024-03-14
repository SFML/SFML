////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Config.hpp>

#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Sensor.hpp>

#include <SFML/System/Vector2.hpp>

#include <variant>

#include <cassert>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Defines a system event and its parameters
///
////////////////////////////////////////////////////////////
class SFML_WINDOW_API Event
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Empty event
    ///
    ////////////////////////////////////////////////////////////
    struct Empty
    {
    };

    ////////////////////////////////////////////////////////////
    /// \brief Closed event
    ///
    ////////////////////////////////////////////////////////////
    struct Closed
    {
    };

    ////////////////////////////////////////////////////////////
    /// \brief Resized event
    ///
    ////////////////////////////////////////////////////////////
    struct Resized
    {
        Vector2u size; //!< New size, in pixels
    };

    ////////////////////////////////////////////////////////////
    /// \brief Lost focus event
    ///
    ////////////////////////////////////////////////////////////
    struct FocusLost
    {
    };

    ////////////////////////////////////////////////////////////
    /// \brief Gained focus event
    ///
    ////////////////////////////////////////////////////////////
    struct FocusGained
    {
    };

    ////////////////////////////////////////////////////////////
    /// \brief Text event
    ///
    ////////////////////////////////////////////////////////////
    struct TextEntered
    {
        std::uint32_t unicode{}; //!< UTF-32 Unicode value of the character
    };

    ////////////////////////////////////////////////////////////
    /// \brief KeyChanged events
    ///
    ////////////////////////////////////////////////////////////
    struct KeyChanged
    {
        Keyboard::Key      code{};     //!< Code of the key
        Keyboard::Scancode scancode{}; //!< Physical code of the key
        bool               alt{};      //!< Is the Alt key pressed?
        bool               control{};  //!< Is the Control key pressed?
        bool               shift{};    //!< Is the Shift key pressed?
        bool               system{};   //!< Is the System key pressed?
    };
    struct KeyPressed : KeyChanged
    {
    };
    struct KeyReleased : KeyChanged
    {
    };

    ////////////////////////////////////////////////////////////
    /// \brief Mouse wheel scrolled event
    ///
    ////////////////////////////////////////////////////////////
    struct MouseWheelScrolled
    {
        Mouse::Wheel wheel{}; //!< Which wheel (for mice with multiple ones)
        float        delta{}; //!< Wheel offset (positive is up/left, negative is down/right). High-precision mice may use non-integral offsets.
        Vector2i     position; //!< Position of the mouse pointer, relative to the top left of the owner window
    };

    ////////////////////////////////////////////////////////////
    /// \brief Mouse button changed events
    ///
    ////////////////////////////////////////////////////////////
    struct MouseButtonChanged
    {
        Mouse::Button button{}; //!< Code of the button that has been pressed
        Vector2i      position; //!< Position of the mouse pointer, relative to the top left of the owner window
    };
    struct MouseButtonPressed : MouseButtonChanged
    {
    };
    struct MouseButtonReleased : MouseButtonChanged
    {
    };

    ////////////////////////////////////////////////////////////
    /// \brief Mouse move event
    ///
    ////////////////////////////////////////////////////////////
    struct MouseMoved
    {
        Vector2i position; //!< Position of the mouse pointer, relative to the top left of the owner window
    };

    ////////////////////////////////////////////////////////////
    /// \brief Mouse entered event
    ///
    ////////////////////////////////////////////////////////////
    struct MouseEntered
    {
    };

    ////////////////////////////////////////////////////////////
    /// \brief Mouse left event
    ///
    ////////////////////////////////////////////////////////////
    struct MouseLeft
    {
    };

    ////////////////////////////////////////////////////////////
    /// \brief Joystick button events
    ///
    ////////////////////////////////////////////////////////////
    struct JoystickButtonChanged
    {
        unsigned int joystickId{}; //!< Index of the joystick (in range [0 .. Joystick::Count - 1])
        unsigned int button{}; //!< Index of the button that has been pressed (in range [0 .. Joystick::ButtonCount - 1])
    };
    struct JoystickButtonPressed : JoystickButtonChanged
    {
    };
    struct JoystickButtonReleased : JoystickButtonChanged
    {
    };

    ////////////////////////////////////////////////////////////
    /// \brief Joystick axis move event
    ///
    ////////////////////////////////////////////////////////////
    struct JoystickMoved
    {
        unsigned int   joystickId{}; //!< Index of the joystick (in range [0 .. Joystick::Count - 1])
        Joystick::Axis axis{};       //!< Axis on which the joystick moved
        float          position{};   //!< New position on the axis (in range [-100 .. 100])
    };

    ////////////////////////////////////////////////////////////
    /// \brief Joystick connection events
    ///
    ////////////////////////////////////////////////////////////
    struct JoystickChanged
    {
        unsigned int joystickId{}; //!< Index of the joystick (in range [0 .. Joystick::Count - 1])
    };
    struct JoystickConnected : JoystickChanged
    {
    };
    struct JoystickDisconnected : JoystickChanged
    {
    };

    ////////////////////////////////////////////////////////////
    /// \brief Touch events
    ///
    ////////////////////////////////////////////////////////////
    struct TouchChanged
    {
        unsigned int finger{}; //!< Index of the finger in case of multi-touch events
        Vector2i     position; //!< Position of the touch, relative to the top left of the owner window
    };
    struct TouchBegan : TouchChanged
    {
    };
    struct TouchMoved : TouchChanged
    {
    };
    struct TouchEnded : TouchChanged
    {
    };

    ////////////////////////////////////////////////////////////
    /// \brief Sensor event
    ///
    ////////////////////////////////////////////////////////////
    struct SensorChanged
    {
        Sensor::Type type{}; //!< Type of the sensor
        Vector3f     value;  //!< Current value of the sensor on the X, Y, and Z axes
    };

    ////////////////////////////////////////////////////////////
    /// \brief Enumeration of the different types of events
    ///
    /// Each event type maps 1-to-1 to a specific structure of
    /// the same name.
    ///
    ////////////////////////////////////////////////////////////
    enum class Type
    {
        Empty,                  //!< Default type
        Closed,                 //!< The window requested to be closed
        Resized,                //!< The window was resized
        FocusLost,              //!< The window lost the focus
        FocusGained,            //!< The window gained the focus
        TextEntered,            //!< A character was entered
        KeyPressed,             //!< A key was pressed
        KeyReleased,            //!< A key was released
        MouseWheelScrolled,     //!< The mouse wheel was scrolled
        MouseButtonPressed,     //!< A mouse button was pressed
        MouseButtonReleased,    //!< A mouse button was released
        MouseMoved,             //!< The mouse cursor moved
        MouseEntered,           //!< The mouse cursor entered the area of the window
        MouseLeft,              //!< The mouse cursor left the area of the window
        JoystickButtonPressed,  //!< A joystick button was pressed
        JoystickButtonReleased, //!< A joystick button was released
        JoystickMoved,          //!< The joystick moved along an axis
        JoystickConnected,      //!< A joystick was connected
        JoystickDisconnected,   //!< A joystick was disconnected
        TouchBegan,             //!< A touch event began
        TouchMoved,             //!< A touch moved
        TouchEnded,             //!< A touch event ended
        SensorChanged,          //!< A sensor value changed
    };

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Sets the event to Type::Empty
    ///
    ////////////////////////////////////////////////////////////
    Event() = default;

    ////////////////////////////////////////////////////////////
    /// \brief Construct from a given sf::Event subtype
    ///
    /// \param t Event subtype
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    Event(const T& t);

    ////////////////////////////////////////////////////////////
    /// \brief Get current event type
    ///
    /// \return Current event type
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Type getType() const;

    ////////////////////////////////////////////////////////////
    /// \brief Check current event type
    ///
    /// \return True if template parameter is current event type
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    [[nodiscard]] bool is() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get particular event type
    ///
    /// \return Reference to current event type
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    [[nodiscard]] const T& get() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get particular event type
    ///
    /// \return Address of current event type, otherwise nullptr
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    [[nodiscard]] const T* getIf() const;

private:
    ////////////////////////////////////////////////////////////
    /// \brief Alias for std::variant template specialization
    ///
    ////////////////////////////////////////////////////////////
    using VariantType = std::variant<
        Empty,
        Closed,
        Resized,
        FocusLost,
        FocusGained,
        TextEntered,
        KeyPressed,
        KeyReleased,
        MouseWheelScrolled,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseEntered,
        MouseLeft,
        JoystickButtonPressed,
        JoystickButtonReleased,
        JoystickMoved,
        JoystickConnected,
        JoystickDisconnected,
        TouchBegan,
        TouchMoved,
        TouchEnded,
        SensorChanged>;

    ////////////////////////////////////////////////////////////
    // Helper functions
    ////////////////////////////////////////////////////////////
    template <typename T, typename... Ts>
    static constexpr bool isInParameterPack(const std::variant<Ts...>&)
    {
        return (std::is_same_v<T, Ts> || ...);
    }

    template <typename T>
    static constexpr bool isEventType = isInParameterPack<T>(VariantType());

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    VariantType m_data; //!< Event data
};

#include <SFML/Window/Event.inl>

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::Event
/// \ingroup window
///
/// sf::Event holds all the information about a system event
/// that just happened. Events are retrieved using the
/// sf::Window::pollEvent and sf::Window::waitEvent functions.
///
/// A sf::Event instance contains the type of the event
/// (mouse moved, key pressed, window closed, ...) as well
/// as the details about this particular event. Each event
/// corresponds to a different struct which contains the data
/// required to process that event.
///
/// Various member functions are provided to inspect the current
/// active event and access it's data. These functions give you
/// multiple options for how you choose to process events.
///
/// sf::Event::getType returns an enumeration where each value
/// matches the name of the struct that holds the corresponding
/// data. sf::Event::get can be used to retrieve that particular
/// struct. This combination works well if you want to use a switch
/// statement to process events.
///
/// \code
/// for (sf::Event event; window.pollEvent(event);)
/// {
///     switch (event.getType())
///     {
///         // Request for closing the window
///         case sf::Event::Type::Closed:
///             window.close();
///             break;
///
///         // The escape key was pressed
///         case sf::Event::Type::KeyPressed:
///             if (event.get<sf::Event::KeyPressed>().code == sf::Keyboard::Escape)
///                 window.close();
///             break;
///
///         // The window was resized
///         case sf::Event::Type::Resized:
///             doSomethingWithTheNewSize(event.get<sf::Event::Resized>().size);
///             break;
///
///         // etc ...
///     }
/// }
/// \endcode
///
/// An alternative means of accessing the current active event is
/// to use sf::Event::getIf which returns the address of the event
/// struct if it's active or a nullptr otherwise. sf::Event::is is
/// used to check the active event type without actually reading any
/// of the corresponding event data. These are useful if you need
/// to check only a single possible active event or prefer to
/// process events in a series of if/else if blocks.
///
/// \code
/// for (sf::Event event; window.pollEvent(event);)
/// {
///     // Request for closing the window
///     if (event.is<sf::Event::Closed>())
///         window.close();
///
///     // The escape key was pressed
///     if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
///         if (keyPressed->code == sf::Keyboard::Key::Escape)
///             window.close();
///
///     // The window was resized
///     if (const auto* resized = event.getIf<sf::Event::Resized>())
///         doSomethingWithTheNewSize(resized->size);
///
///     // etc ...
/// }
/// \endcode
///
////////////////////////////////////////////////////////////
