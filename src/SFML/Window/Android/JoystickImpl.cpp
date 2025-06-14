////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2013 Jonathan De Wachter (dewachter.jonathan@gmail.com)
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
#include <SFML/Window/Android/JniHelper.hpp>
#include <SFML/Window/JoystickImpl.hpp>

#include <SFML/System/Android/Activity.hpp>
#include <SFML/System/Err.hpp>

namespace
{
[[nodiscard]] std::optional<sf::Joystick::Axis> androidAxisToSf(const int axisCode)
{
    switch (axisCode)
    {
        case AMOTION_EVENT_AXIS_X:
            return sf::Joystick::Axis::X;
        case AMOTION_EVENT_AXIS_Y:
            return sf::Joystick::Axis::Y;
        case AMOTION_EVENT_AXIS_Z:
            return sf::Joystick::Axis::Z;
        case AMOTION_EVENT_AXIS_RZ:
            return sf::Joystick::Axis::R;
        case AMOTION_EVENT_AXIS_LTRIGGER:
            return sf::Joystick::Axis::U;
        case AMOTION_EVENT_AXIS_RTRIGGER:
            return sf::Joystick::Axis::V;
        case AMOTION_EVENT_AXIS_HAT_X:
            return sf::Joystick::Axis::PovX;
        case AMOTION_EVENT_AXIS_HAT_Y:
            return sf::Joystick::Axis::PovY;
    }
    return std::nullopt;
}

[[nodiscard]] std::optional<sf::priv::JoystickCaps> getCapabilitiesFromJni(const sf::priv::JniInputDevice& inputDevice)
{
    auto motionRanges = inputDevice.getMotionRanges();
    if (!motionRanges)
    {
        sf::err() << "Gamepad was found, but its capabilities couldn't be read, skipping" << std::endl;
        return std::nullopt;
    }

    sf::priv::JoystickCaps capabilities = {sf::Joystick::ButtonCount, {false}};

    for (ssize_t axisIdx = 0; axisIdx < motionRanges->getSize(); ++axisIdx)
    {
        auto motionRange = (*motionRanges)[axisIdx];
        if (!motionRange)
        {
            sf::err() << "Gamepad was found, but its capabilities couldn't be read, skipping" << std::endl;
            return std::nullopt;
        }

        if (const auto axis = androidAxisToSf(motionRange->getAxis()))
            capabilities.axes[*axis] = true;
    }

    return capabilities;
}
} // namespace

namespace sf::priv
{
////////////////////////////////////////////////////////////
void JoystickImpl::initialize()
{
}


////////////////////////////////////////////////////////////
void JoystickImpl::cleanup()
{
}


////////////////////////////////////////////////////////////
bool JoystickImpl::isConnected(unsigned int index)
{
    // This is called as a prefilter before open, but it
    // would just duplicate the logic of open.
    return index < Joystick::Count;
}


////////////////////////////////////////////////////////////
bool JoystickImpl::open(unsigned int joyIndex)
{
    if (joyIndex >= Joystick::Count)
        return false;

    // Retrieve activity states
    ActivityStates&       states = getActivity();
    const std::lock_guard lock(states.mutex);

    JNIEnv* env = states.activity->env;
    if (!Jni::attachCurrentThread(states.activity->vm, &env))
    {
        err() << "Failed to initialize JNI" << std::endl;
        return false;
    }

    auto inputDeviceClass = JniInputDeviceClass::findClass(*env);
    if (!inputDeviceClass)
        return false;

    auto deviceIds = inputDeviceClass->getDeviceIds();
    if (!deviceIds)
        return false;

    unsigned foundGamepadsSoFar = 0;
    for (jsize i = 0; i < deviceIds->getLength(); ++i)
    {
        const auto deviceId    = (*deviceIds)[i];
        auto       inputDevice = inputDeviceClass->getDevice(deviceId);
        if (!inputDevice)
            continue;

        if (!inputDevice->supportsSource(AINPUT_SOURCE_GAMEPAD | AINPUT_SOURCE_JOYSTICK))
            continue;

        if (foundGamepadsSoFar < joyIndex)
        {
            ++foundGamepadsSoFar;
            continue;
        }

        // Found device might be already registered. It might be even registered for
        // another joystick index. So skip that and search for some other.
        if (states.joystickStates.find(deviceId) != states.joystickStates.end())
            continue;

        if (const auto capabilities = getCapabilitiesFromJni(*inputDevice))
            m_capabilities = *capabilities;
        else
            return false;

        m_identification = Joystick::Identification{
            inputDevice->getName(),
            inputDevice->getVendorId(),
            inputDevice->getProductId(),
        };

        m_currentDeviceIdx = deviceId;

        states.joystickStates[deviceId] = {};

        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////
void JoystickImpl::close() const
{
    ActivityStates&       states = getActivity();
    const std::lock_guard lock(states.mutex);

    if (m_currentDeviceIdx)
        states.joystickStates.erase(*m_currentDeviceIdx);
}


////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::getCapabilities() const
{
    return m_capabilities;
}


////////////////////////////////////////////////////////////
Joystick::Identification JoystickImpl::getIdentification() const
{
    return m_identification;
}


////////////////////////////////////////////////////////////
JoystickState JoystickImpl::update() const
{
    // Retrieve activity states
    ActivityStates&       states = getActivity();
    const std::lock_guard lock(states.mutex);

    JNIEnv* env = states.activity->env;
    if (!Jni::attachCurrentThread(states.activity->vm, &env))
    {
        err() << "Failed to initialize JNI" << std::endl;
        return {false};
    }

    auto inputDeviceClass = JniInputDeviceClass::findClass(*env);
    if (!inputDeviceClass)
    {
        return {false};
    }

    if (!m_currentDeviceIdx)
    {
        // Should never happen
        return {false};
    }

    const bool isConnected = inputDeviceClass->getDevice(*m_currentDeviceIdx).has_value();
    if (states.joystickStates.find(*m_currentDeviceIdx) == states.joystickStates.end())
    {
        // This technically shouldn't happen, but when I have connect physical gamepad
        // and then connect/disconnect a bluetooth one, states for the physical one
        // disappears for a frame and then it reconnects.
        return {false};
    }

    return {isConnected, states.joystickStates[*m_currentDeviceIdx].axes, states.joystickStates[*m_currentDeviceIdx].buttons};
}


////////////////////////////////////////////////////////////
int JoystickImpl::sfAxisToAndroid(Joystick::Axis axis)
{
    switch (axis)
    {
        case Joystick::Axis::X:
            return AMOTION_EVENT_AXIS_X;
        case Joystick::Axis::Y:
            return AMOTION_EVENT_AXIS_Y;
        case Joystick::Axis::Z:
            return AMOTION_EVENT_AXIS_Z;
        case Joystick::Axis::R:
            return AMOTION_EVENT_AXIS_RZ;
        case Joystick::Axis::U:
            return AMOTION_EVENT_AXIS_LTRIGGER;
        case Joystick::Axis::V:
            return AMOTION_EVENT_AXIS_RTRIGGER;
        case Joystick::Axis::PovX:
            return AMOTION_EVENT_AXIS_HAT_X;
        case Joystick::Axis::PovY:
            return AMOTION_EVENT_AXIS_HAT_Y;
    }

    return -1;
}

} // namespace sf::priv
