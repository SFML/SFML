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

#ifndef SFML_JOYSTICKIMPL_HPP
#define SFML_JOYSTICKIMPL_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <SFML/Window/Joystick.hpp>
#include <algorithm>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Structure holding a joystick's capabilities
///
////////////////////////////////////////////////////////////
struct JoystickCaps
{
    JoystickCaps()
    {
        ButtonCount = 0;
        std::fill(Axes, Axes + Joystick::AxisCount, false);
    }

    unsigned int ButtonCount;               ///< Number of buttons supported by the joystick
    bool         Axes[Joystick::AxisCount]; ///< Support for each axis
};


////////////////////////////////////////////////////////////
/// \brief Structure holding a joystick's state
///
////////////////////////////////////////////////////////////
struct JoystickState
{
    JoystickState()
    {
        Connected = false;
        std::fill(Axes, Axes + Joystick::AxisCount, 0.f);
        std::fill(Buttons, Buttons + Joystick::ButtonCount, false);
    }

    bool  Connected;                      ///< Is the joystick currently connected?
    float Axes[Joystick::AxisCount];      ///< Position of each axis, in range [-100, 100]
    bool  Buttons[Joystick::ButtonCount]; ///< Status of each button (true = pressed)
};

} // namespace priv

} // namespace sf


#if defined(SFML_SYSTEM_WINDOWS)

    #include <SFML/Window/Win32/JoystickImpl.hpp>

#elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD)

    #include <SFML/Window/Linux/JoystickImpl.hpp>

#elif defined(SFML_SYSTEM_MACOS)

    #include <SFML/Window/OSX/JoystickImpl.hpp>

#endif


#endif // SFML_JOYSTICKIMPL_HPP
