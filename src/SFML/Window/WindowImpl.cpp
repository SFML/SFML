////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2021 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/WindowImpl.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/JoystickImpl.hpp>
#include <SFML/Window/JoystickManager.hpp>
#include <SFML/Window/SensorManager.hpp>
#include <SFML/System/Sleep.hpp>
#include <algorithm>
#include <memory>
#include <cmath>

#if defined(SFML_SYSTEM_WINDOWS)

    #include <SFML/Window/Win32/WindowImplWin32.hpp>
    using WindowImplType = sf::priv::WindowImplWin32;

    #include <SFML/Window/Win32/VulkanImplWin32.hpp>
    using VulkanImplType = sf::priv::VulkanImplWin32;

#elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD) || defined(SFML_SYSTEM_OPENBSD) || defined(SFML_SYSTEM_NETBSD)

    #include <SFML/Window/Unix/WindowImplX11.hpp>
    using WindowImplType = sf::priv::WindowImplX11;

    #include <SFML/Window/Unix/VulkanImplX11.hpp>
    using VulkanImplType = sf::priv::VulkanImplX11;

#elif defined(SFML_SYSTEM_MACOS)

    #include <SFML/Window/OSX/WindowImplCocoa.hpp>
    using WindowImplType = sf::priv::WindowImplCocoa;

    #define SFML_VULKAN_IMPLEMENTATION_NOT_AVAILABLE

#elif defined(SFML_SYSTEM_IOS)

    #include <SFML/Window/iOS/WindowImplUIKit.hpp>
    using WindowImplType = sf::priv::WindowImplUIKit;

    #define SFML_VULKAN_IMPLEMENTATION_NOT_AVAILABLE

#elif defined(SFML_SYSTEM_ANDROID)

    #include <SFML/Window/Android/WindowImplAndroid.hpp>
    using WindowImplType = sf::priv::WindowImplAndroid;

    #define SFML_VULKAN_IMPLEMENTATION_NOT_AVAILABLE

#endif


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
struct WindowImpl::JoystickStatesImpl
{
    JoystickState m_states[Joystick::Count]; //!< Previous state of the joysticks
};

////////////////////////////////////////////////////////////
std::unique_ptr<WindowImpl> WindowImpl::create(VideoMode mode, const String& title, Uint32 style, const ContextSettings& settings)
{
    return std::make_unique<WindowImplType>(mode, title, style, settings);
}


////////////////////////////////////////////////////////////
std::unique_ptr<WindowImpl> WindowImpl::create(WindowHandle handle)
{
    return std::make_unique<WindowImplType>(handle);
}


////////////////////////////////////////////////////////////
WindowImpl::WindowImpl() :
m_joystickStatesImpl(std::make_unique<JoystickStatesImpl>()),
m_joystickThreshold(0.1f)
{
    // Get the initial joystick states
    JoystickManager::getInstance().update();
    for (unsigned int i = 0; i < Joystick::Count; ++i)
    {
        m_joystickStatesImpl->m_states[i] = JoystickManager::getInstance().getState(i);
        std::fill_n(m_previousAxes[i], static_cast<std::size_t>(Joystick::AxisCount), 0.f);
    }

    // Get the initial sensor states
    for (sf::Vector3f& vec : m_sensorValue)
        vec = Vector3f(0, 0, 0);
}


////////////////////////////////////////////////////////////
WindowImpl::~WindowImpl() = default;


////////////////////////////////////////////////////////////
void WindowImpl::setJoystickThreshold(float threshold)
{
    m_joystickThreshold = threshold;
}


////////////////////////////////////////////////////////////
bool WindowImpl::popEvent(Event& event, bool block)
{
    // If the event queue is empty, let's first check if new events are available from the OS
    if (m_events.empty())
    {
        // Get events from the system
        processJoystickEvents();
        processSensorEvents();
        processEvents();

        // In blocking mode, we must process events until one is triggered
        if (block)
        {
            // Here we use a manual wait loop instead of the optimized
            // wait-event provided by the OS, so that we don't skip joystick
            // events (which require polling)
            while (m_events.empty())
            {
                sleep(milliseconds(10));
                processJoystickEvents();
                processSensorEvents();
                processEvents();
            }
        }
    }

    // Pop the first event of the queue, if it is not empty
    if (!m_events.empty())
    {
        event = m_events.front();
        m_events.pop();

        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////
void WindowImpl::pushEvent(const Event& event)
{
    m_events.push(event);
}


////////////////////////////////////////////////////////////
void WindowImpl::processJoystickEvents()
{
    // First update the global joystick states
    JoystickManager::getInstance().update();

    for (unsigned int i = 0; i < Joystick::Count; ++i)
    {
        // Copy the previous state of the joystick and get the new one
        JoystickState previousState = m_joystickStatesImpl->m_states[i];
        m_joystickStatesImpl->m_states[i] = JoystickManager::getInstance().getState(i);

        // Connection state
        bool connected = m_joystickStatesImpl->m_states[i].connected;
        if (previousState.connected ^ connected)
        {
            Event event;
            event.type = connected ? Event::JoystickConnected : Event::JoystickDisconnected;
            event.joystickButton.joystickId = i;
            pushEvent(event);

            // Clear previous axes positions
            if (connected)
                std::fill_n(m_previousAxes[i], static_cast<std::size_t>(Joystick::AxisCount), 0.f);
        }

        if (connected)
        {
            JoystickCaps caps = JoystickManager::getInstance().getCapabilities(i);

            // Axes
            for (unsigned int j = 0; j < Joystick::AxisCount; ++j)
            {
                if (caps.axes[j])
                {
                    auto axis = static_cast<Joystick::Axis>(j);
                    float prevPos = m_previousAxes[i][axis];
                    float currPos = m_joystickStatesImpl->m_states[i].axes[axis];
                    if (std::abs(currPos - prevPos) >= m_joystickThreshold)
                    {
                        Event event;
                        event.type = Event::JoystickMoved;
                        event.joystickMove.joystickId = i;
                        event.joystickMove.axis = axis;
                        event.joystickMove.position = currPos;
                        pushEvent(event);

                        m_previousAxes[i][axis] = currPos;
                    }
                }
            }

            // Buttons
            for (unsigned int j = 0; j < caps.buttonCount; ++j)
            {
                bool prevPressed = previousState.buttons[j];
                bool currPressed = m_joystickStatesImpl->m_states[i].buttons[j];

                if (prevPressed ^ currPressed)
                {
                    Event event;
                    event.type = currPressed ? Event::JoystickButtonPressed : Event::JoystickButtonReleased;
                    event.joystickButton.joystickId = i;
                    event.joystickButton.button = j;
                    pushEvent(event);
                }
            }
        }
    }
}


////////////////////////////////////////////////////////////
void WindowImpl::processSensorEvents()
{
    // First update the sensor states
    SensorManager::getInstance().update();

    for (unsigned int i = 0; i < Sensor::Count; ++i)
    {
        auto sensor = static_cast<Sensor::Type>(i);

        // Only process enabled sensors
        if (SensorManager::getInstance().isEnabled(sensor))
        {
            // Copy the previous value of the sensor and get the new one
            Vector3f previousValue = m_sensorValue[i];
            m_sensorValue[i] = SensorManager::getInstance().getValue(sensor);

            // If the value has changed, trigger an event
            if (m_sensorValue[i] != previousValue) // @todo use a threshold?
            {
                Event event;
                event.type = Event::SensorChanged;
                event.sensor.type = sensor;
                event.sensor.x = m_sensorValue[i].x;
                event.sensor.y = m_sensorValue[i].y;
                event.sensor.z = m_sensorValue[i].z;
                pushEvent(event);
            }
        }
    }
}


////////////////////////////////////////////////////////////
bool WindowImpl::createVulkanSurface(const VkInstance& instance, VkSurfaceKHR& surface, const VkAllocationCallbacks* allocator)
{
#if defined(SFML_VULKAN_IMPLEMENTATION_NOT_AVAILABLE)

    (void) instance;
    (void) surface;
    (void) allocator;
    return false;

#else

    return VulkanImplType::createVulkanSurface(instance, getSystemHandle(), surface, allocator);

#endif
}

} // namespace priv

} // namespace sf
