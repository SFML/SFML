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
#include <SFML/Window/WindowListener.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// Input handles real-time input from keyboard and mouse.
/// Use it instead of events to handle continuous moves and more
/// game-friendly inputs
////////////////////////////////////////////////////////////
class SFML_API Input : public WindowListener, NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Input();

    ////////////////////////////////////////////////////////////
    /// Get the state of a key
    ///
    /// \param KeyCode : Key to check
    ///
    /// \return True if key is down, false if key is up
    ///
    ////////////////////////////////////////////////////////////
    bool IsKeyDown(Key::Code KeyCode) const;

    ////////////////////////////////////////////////////////////
    /// Get the state of a mouse button
    ///
    /// \param Button : Button to check
    ///
    /// \return True if button is down, false if button is up
    ///
    ////////////////////////////////////////////////////////////
    bool IsMouseButtonDown(Mouse::Button Button) const;

    ////////////////////////////////////////////////////////////
    /// Get the state of a joystick button
    ///
    /// \param JoyId :  Identifier of the joystick to check (0 or 1)
    /// \param Button : Button to check
    ///
    /// \return True if button is down, false if button is up
    ///
    ////////////////////////////////////////////////////////////
    bool IsJoystickButtonDown(unsigned int JoyId, unsigned int Button) const;

    ////////////////////////////////////////////////////////////
    /// Get the mouse X position
    ///
    /// \return Current mouse left position, relative to owner window
    ///
    ////////////////////////////////////////////////////////////
    int GetMouseX() const;

    ////////////////////////////////////////////////////////////
    /// Get the mouse Y position
    ///
    /// \return Current mouse top position, relative to owner window
    ///
    ////////////////////////////////////////////////////////////
    int GetMouseY() const;

    ////////////////////////////////////////////////////////////
    /// Get a joystick axis position
    ///
    /// \param JoyId : Identifier of the joystick to check (0 or 1)
    /// \param Axis :  Axis to get
    ///
    /// \return Current axis position, in the range [-100, 100] (except for POV, which is [0, 360])
    ///
    ////////////////////////////////////////////////////////////
    float GetJoystickAxis(unsigned int JoyId, Joy::Axis Axis) const;

private :

    ////////////////////////////////////////////////////////////
    /// /see WindowListener::OnEvent
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnEvent(const Event& EventReceived);

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
