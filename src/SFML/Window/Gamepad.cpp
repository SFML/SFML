////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2026 Laurent Gomila (laurent@sfml-dev.org)
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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Gamepad.hpp>
#include <SFML/Window/JoystickManager.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Utils.hpp>

#include <fstream>
#include <iterator>
#include <ostream>


namespace sf::Gamepad
{
////////////////////////////////////////////////////////////
bool isConnected(unsigned int joystick)
{
    return priv::JoystickManager::getInstance().getGamepadState(joystick).connected;
}


////////////////////////////////////////////////////////////
bool hasButton(unsigned int joystick, Button button)
{
    return priv::JoystickManager::getInstance().getGamepadBindings(joystick).hasButton(button);
}


////////////////////////////////////////////////////////////
bool hasAxis(unsigned int joystick, Axis axis)
{
    return priv::JoystickManager::getInstance().getGamepadBindings(joystick).hasAxis(axis);
}


////////////////////////////////////////////////////////////
bool isButtonPressed(unsigned int joystick, Button button)
{
    return priv::JoystickManager::getInstance().getGamepadState(joystick).buttons[button];
}


////////////////////////////////////////////////////////////
float getAxisPosition(unsigned int joystick, Axis axis)
{
    return priv::JoystickManager::getInstance().getGamepadState(joystick).axes[axis];
}


////////////////////////////////////////////////////////////
String getName(unsigned int joystick)
{
    if (const priv::GamepadMapping* mapping = priv::JoystickManager::getInstance().getGamepadMapping(joystick))
        return String::fromUtf8(mapping->getName().begin(), mapping->getName().end());

    return {};
}


////////////////////////////////////////////////////////////
std::string getMapping(unsigned int joystick)
{
    if (const priv::GamepadMapping* mapping = priv::JoystickManager::getInstance().getGamepadMapping(joystick))
        return mapping->getString();

    return {};
}


////////////////////////////////////////////////////////////
std::size_t addMappings(std::string_view mappings)
{
    return priv::JoystickManager::getInstance().addGamepadMappings(mappings);
}


////////////////////////////////////////////////////////////
std::size_t addMappingsFromFile(const std::filesystem::path& filename)
{
    std::ifstream file(filename, std::ios_base::binary);
    if (!file)
    {
        err() << "Failed to open gamepad mapping file\n" << formatDebugPathInfo(filename) << std::endl;
        return 0;
    }

    const std::string mappings(std::istreambuf_iterator<char>(file), {});
    return addMappings(mappings);
}

} // namespace sf::Gamepad
