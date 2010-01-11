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

#ifndef SFML_JOYSTICKWIN32_HPP
#define SFML_JOYSTICKWIN32_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Windows implementation of Joystick
///
////////////////////////////////////////////////////////////
class Joystick
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Initialize the instance and bind it to a physical joystick
    ///
    /// \param index Index of the physical joystick to bind to
    ///
    ////////////////////////////////////////////////////////////
    void Initialize(unsigned int index);

    ////////////////////////////////////////////////////////////
    /// \brief Update the current joystick and return its new state
    ///
    /// \return Current state of the joystick
    ///
    ////////////////////////////////////////////////////////////
    JoystickState UpdateState();

    ////////////////////////////////////////////////////////////
    /// \brief Check if the joystick supports the given axis
    ///
    /// \param Axis : Axis to check
    ///
    /// \return True of the axis is supported, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    bool HasAxis(Joy::Axis Axis) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the number of buttons supported by the joystick
    ///
    /// \return Number of buttons
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetButtonsCount() const;

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    bool         myIsConnected;          ///< Is there a joystick connected?
    unsigned int myIndex;                ///< Windows ID of the joystick
    unsigned int myNbButtons;            ///< Number of buttons supported by the joystick
    bool         myAxes[Joy::AxisCount]; ///< Supported axes
    bool         myHasContinuousPOV;     ///< True if the driver supports continuous values for the POV
};

} // namespace priv

} // namespace sf


#endif // SFML_JOYSTICKWIN32_HPP
