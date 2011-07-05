////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)//
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

#ifndef SFML_JOYSTICK_H
#define SFML_JOYSTICK_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>


////////////////////////////////////////////////////////////
/// \brief Global joysticks capabilities
///
////////////////////////////////////////////////////////////
enum
{
    sfJoystickCount       = 8,  ///< Maximum number of supported joysticks
    sfJoystickButtonCount = 32, ///< Maximum number of supported buttons
    sfJoystickAxisCount   = 8   ///< Maximum number of supported axes
};


////////////////////////////////////////////////////////////
/// \brief Axes supported by SFML joysticks
///
////////////////////////////////////////////////////////////
typedef enum
{
    sfJoystickX,    ///< The X axis
    sfJoystickY,    ///< The Y axis
    sfJoystickZ,    ///< The Z axis
    sfJoystickR,    ///< The R axis
    sfJoystickU,    ///< The U axis
    sfJoystickV,    ///< The V axis
    sfJoystickPovX, ///< The X axis of the point-of-view hat
    sfJoystickPovY  ///< The Y axis of the point-of-view hat
} sfJoystickAxis;


////////////////////////////////////////////////////////////
/// \brief Check if a joystick is connected
///
/// \param joystick Index of the joystick to check
///
/// \return sfTrue if the joystick is connected, sfFalse otherwise
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfJoystick_IsConnected(unsigned int joystick);

////////////////////////////////////////////////////////////
/// \brief Return the number of buttons supported by a joystick
///
/// If the joystick is not connected, this function returns 0.
///
/// \param joystick Index of the joystick
///
/// \return Number of buttons supported by the joystick
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfJoystick_GetButtonCount(unsigned int joystick);

////////////////////////////////////////////////////////////
/// \brief Check if a joystick supports a given axis
///
/// If the joystick is not connected, this function returns false.
///
/// \param joystick Index of the joystick
/// \param axis     Axis to check
///
/// \return sfTrue if the joystick supports the axis, sfFalse otherwise
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfJoystick_HasAxis(unsigned int joystick, sfJoystickAxis axis);

////////////////////////////////////////////////////////////
/// \brief Check if a joystick button is pressed
///
/// If the joystick is not connected, this function returns false.
///
/// \param joystick Index of the joystick
/// \param button   Button to check
///
/// \return sfTrue if the button is pressed, sfFalse otherwise
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfJoystick_IsButtonPressed(unsigned int joystick, unsigned int button);

////////////////////////////////////////////////////////////
/// \brief Get the current position of a joystick axis
///
/// If the joystick is not connected, this function returns 0.
///
/// \param joystick Index of the joystick
/// \param axis     Axis to check
///
/// \return Current position of the axis, in range [-100 .. 100]
///
////////////////////////////////////////////////////////////
CSFML_API float sfJoystick_GetAxisPosition(unsigned int joystick, sfJoystickAxis axis);

////////////////////////////////////////////////////////////
/// \brief Update the states of all joysticks
///
/// This function is used internally by SFML, so you normally
/// don't have to call it explicitely. However, you may need to
/// call it if you have no window yet (or no window at all):
/// in this case the joysticks states are not updated automatically.
///
////////////////////////////////////////////////////////////
CSFML_API void sfJoystick_Update(void);


#endif // SFML_JOYSTICK_H
