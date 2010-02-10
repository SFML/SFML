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

#ifndef SFML_JOYSTICKLINUX_HPP
#define SFML_JOYSTICKLINUX_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#if defined(SFML_SYSTEM_LINUX)
    #include <linux/joystick.h>
    #include <fcntl.h>
#elif defined(SFML_SYSTEM_FREEBSD)
    // #include <sys/joystick.h> ?
    #define ABS_MAX 1
#endif


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Linux implementation of Joystick
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
    /// \param axis Axis to check
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
    int           myDescriptor;               ///< Linux descriptor of the joystick
    unsigned int  myNbButtons;                ///< Number of buttons supported by the joystick
    bool          myAxes[Joy::AxisCount];     ///< Supported axes
    JoystickState myState;                    ///< Current state of the joystick
    int           myPovX;                     ///< Last X position of the POV
    int           myPovY;                     ///< Last Y position of the POV
    char          myAxesMapping[ABS_MAX + 1]; ///< Axes mapping (index --> axis id)
};

} // namespace priv

} // namespace sf


#endif // SFML_JOYSTICKLINUX_HPP
