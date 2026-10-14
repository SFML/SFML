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
#include <SFML/Window/JoystickManager.hpp>

#ifdef SFML_EMBED_GAMEPAD_DB
#include <BuiltinGamepadMappings.hpp>
#endif

#include <cassert>
#include <cstdlib>


namespace sf::priv
{
////////////////////////////////////////////////////////////
JoystickManager& JoystickManager::getInstance()
{
    static JoystickManager instance;
    return instance;
}


////////////////////////////////////////////////////////////
const JoystickCaps& JoystickManager::getCapabilities(unsigned int joystick) const
{
    assert(joystick < Joystick::Count && "Joystick index must be less than Joystick::Count");
    return m_joysticks[joystick].capabilities;
}


////////////////////////////////////////////////////////////
const JoystickState& JoystickManager::getState(unsigned int joystick) const
{
    assert(joystick < Joystick::Count && "Joystick index must be less than Joystick::Count");
    return m_joysticks[joystick].state;
}


////////////////////////////////////////////////////////////
const Joystick::Identification& JoystickManager::getIdentification(unsigned int joystick) const
{
    assert(joystick < Joystick::Count && "Joystick index must be less than Joystick::Count");
    return m_joysticks[joystick].identification;
}


////////////////////////////////////////////////////////////
const GamepadState& JoystickManager::getGamepadState(unsigned int joystick) const
{
    assert(joystick < Joystick::Count && "Joystick index must be less than Joystick::Count");
    return m_joysticks[joystick].gamepadState;
}


////////////////////////////////////////////////////////////
const GamepadBindings& JoystickManager::getGamepadBindings(unsigned int joystick) const
{
    assert(joystick < Joystick::Count && "Joystick index must be less than Joystick::Count");
    return m_joysticks[joystick].gamepadBindings;
}


////////////////////////////////////////////////////////////
const GamepadMapping* JoystickManager::getGamepadMapping(unsigned int joystick) const
{
    assert(joystick < Joystick::Count && "Joystick index must be less than Joystick::Count");
    const std::optional<GamepadMapping>& mapping = m_joysticks[joystick].gamepadMapping;
    return mapping ? &*mapping : nullptr;
}


////////////////////////////////////////////////////////////
std::size_t JoystickManager::addGamepadMappings(std::string_view mappings)
{
    const std::size_t count = m_gamepadMappings.add(mappings);

    // Joysticks that are already open pick up the new mappings right away
    for (Item& item : m_joysticks)
        updateGamepadMapping(item);

    return count;
}


////////////////////////////////////////////////////////////
void JoystickManager::update()
{
    for (unsigned int i = 0; i < Joystick::Count; ++i)
    {
        Item& item = m_joysticks[i];

        if (item.state.connected)
        {
            // Get the current state of the joystick
            item.state        = item.joystick.update();
            item.gamepadState = item.gamepadMapping ? item.gamepadBindings.evaluate(item.state) : GamepadState();

            // Check if it's still connected
            if (!item.state.connected)
            {
                item.joystick.close();
                item.capabilities   = JoystickCaps();
                item.state          = JoystickState();
                item.identification = Joystick::Identification();
                item.layout.reset();
                updateGamepadMapping(item);
            }
        }
        else
        {
            // Check if the joystick was connected since last update
            if (JoystickImpl::isConnected(i))
            {
                if (item.joystick.open(i))
                {
                    item.capabilities   = item.joystick.getCapabilities();
                    item.state          = item.joystick.update();
                    item.identification = item.joystick.getIdentification();
                    item.layout         = item.joystick.getGamepadLayout();
                    updateGamepadMapping(item);
                }
            }
        }
    }
}


////////////////////////////////////////////////////////////
void JoystickManager::updateGamepadMapping(Item& item) const
{
    item.gamepadMapping.reset();
    item.gamepadBindings = GamepadBindings();
    item.gamepadState    = GamepadState();

    // Without a layout, there's no way to tell which of the joystick's elements a mapping refers to
    if (!item.state.connected || !item.layout)
        return;

    if (const GamepadMapping* mapping = m_gamepadMappings.find(item.identification, *item.layout))
    {
        item.gamepadMapping  = *mapping;
        item.gamepadBindings = mapping->resolve(item.capabilities, *item.layout);
        item.gamepadState    = item.gamepadBindings.evaluate(item.state);
    }
}


////////////////////////////////////////////////////////////
JoystickManager::JoystickManager()
{
    JoystickImpl::initialize();

#ifdef SFML_EMBED_GAMEPAD_DB
    for (const std::string_view mapping : builtinGamepadMappings)
        m_gamepadMappings.add(mapping);
#endif

    // Like SDL, take additional mappings from the environment, which is how Steam
    // passes on the controller configuration of the user
    if (const char* mappings = std::getenv("SDL_GAMECONTROLLERCONFIG"))
        m_gamepadMappings.add(mappings);
}


////////////////////////////////////////////////////////////
JoystickManager::~JoystickManager()
{
    for (Item& item : m_joysticks)
    {
        if (item.state.connected)
            item.joystick.close();
    }

    JoystickImpl::cleanup();
}

} // namespace sf::priv
