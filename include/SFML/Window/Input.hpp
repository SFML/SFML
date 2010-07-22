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

#ifndef SFML_INPUT_HPP
#define SFML_INPUT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>


namespace sf
{
class Window;

////////////////////////////////////////////////////////////
/// \brief Give access to the real-time states of keyboard,
///        mouse and joysticks
///
////////////////////////////////////////////////////////////
class SFML_API Input : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Input();

    ////////////////////////////////////////////////////////////
    /// \brief Get the current state of a key (pressed or released)
    ///
    /// \param key Code of the key to test
    ///
    /// \return True if key is down, false if key is up
    ///
    ////////////////////////////////////////////////////////////
    bool IsKeyDown(Key::Code key) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the current state of a mouse button (pressed or released)
    ///
    /// \param button Code of the mouse button to check
    ///
    /// \return True if button is down, false if button is up
    ///
    ////////////////////////////////////////////////////////////
    bool IsMouseButtonDown(Mouse::Button button) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the current state of a joystick button (pressed or released)
    ///
    /// \param joystick Index of the joystick to test (0 or 1)
    /// \param button   Index of the button to test
    ///
    /// \return True if button is down, false if button is up
    ///
    ////////////////////////////////////////////////////////////
    bool IsJoystickButtonDown(unsigned int joystick, unsigned int button) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the current mouse X position
    ///
    /// The returned position is relative to the left border
    /// of the owner window.
    ///
    /// \return Current mouse left position
    ///
    ////////////////////////////////////////////////////////////
    int GetMouseX() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the current mouse Y position
    ///
    /// The returned position is relative to the top border
    /// of the owner window.
    ///
    /// \return Current mouse top position
    ///
    ////////////////////////////////////////////////////////////
    int GetMouseY() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the current position of a joystick axis
    ///
    /// The returned position is in the range [-100, 100], except
    /// the POV which is an angle and is thus defined in [0, 360].
    ///
    /// \param joystick Index of the joystick to test (0 or 1)
    /// \param axis     Axis to test
    ///
    /// \return Current axis position
    ///
    ////////////////////////////////////////////////////////////
    float GetJoystickAxis(unsigned int joystick, Joy::Axis axis) const;

private :

    friend class Window;

    ////////////////////////////////////////////////////////////
    /// \brief Notifies the input of a new event
    ///
    /// This function is for internal use only, it is called by
    /// the owner window every time a new event has been triggered.
    ///
    /// \param event Event received
    ///
    ////////////////////////////////////////////////////////////
    void OnEvent(const Event& event);

    ////////////////////////////////////////////////////////////
    /// Reset all the states
    ///
    ////////////////////////////////////////////////////////////
    void ResetStates();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    bool  myKeys[Key::Count];                              ///< Array containing the state of all keyboard keys
    bool  myMouseButtons[Mouse::ButtonCount];              ///< Array containing the state of all mouse buttons
    int   myMouseX;                                        ///< Mouse position on X
    int   myMouseY;                                        ///< Mouse position on Y
    bool  myJoystickButtons[Joy::Count][Joy::ButtonCount]; ///< Array containing the state of all joysticks buttons
    float myJoystickAxis[Joy::Count][Joy::AxisCount];      ///< Joysticks position on each axis
};

} // namespace sf


#endif // SFML_INPUT_HPP


////////////////////////////////////////////////////////////
/// \class sf::Input
/// \ingroup window
///
/// sf::Input provides a way to access the state of keys,
/// mouse buttons, mouse position, joystick buttons and
/// jostick axis.
///
/// sf::Input provides the same informations as the event
/// system, but these informations can be accessed at any time,
/// which is more convenient in many situations.
///
/// For example, to move an entity you can decide to catch the
/// sf::Event::KeyPressed event on arrow keys. But if you do so,
/// you will only receive one event when the key gets pressed
/// (or repeated events if you activated this feature), thus the
/// entity will not move smoothly. The best solution here is to
/// use sf::Input::IsKeyDown so that you can update your entity's
/// position at every iteration of your game loop, not only when you
/// catch a KeyPressed event.
///
/// Note that instances of sf::Input cannot be created directly,
/// they must be retrieved from a window (sf::Window) with its
/// GetInput() function.
///
/// Usage example:
/// \code
/// // Retrieve the input object attached to our window
/// const sf::Input& input = window.GetInput();
///
/// // Move an entity according to the current keys state
/// float offset = 5 * window.GetFrameTime(); // 5 pixels/sec
/// if (input.IsKeyDown(sf::Key::Left))  entity.Move(-offset, 0);
/// if (input.IsKeyDown(sf::Key::Right)) entity.Move( offset, 0);
/// if (input.IsKeyDown(sf::Key::Up))    entity.Move(0, -offset);
/// if (input.IsKeyDown(sf::Key::Down))  entity.Move(0,  offset);
/// \endcode
///
////////////////////////////////////////////////////////////
