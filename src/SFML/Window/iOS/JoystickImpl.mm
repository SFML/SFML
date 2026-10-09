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
#include <SFML/Window/JoystickImpl.hpp>
#include <SFML/Window/iOS/JoystickRegistryImpl.hpp>

#import <UIKit/UIKit.h>

#include <cstring>


namespace
{
////////////////////////////////////////////////////////////
sf::priv::JoystickRegistryImpl& getRegistry()
{
    static sf::priv::JoystickRegistryImpl registry;
    return registry;
}


////////////////////////////////////////////////////////////
GCControllerButtonInput* getShareButton(GCExtendedGamepad* gamepad)
{
    if (@available(iOS 15.0, *))
    {
        if ([gamepad isKindOfClass:[GCXboxGamepad class]])
        {
            auto* const xboxGamepad = static_cast<GCXboxGamepad*>(gamepad);
            return xboxGamepad.buttonShare;
        }
    }

    return nil;
}
} // namespace


namespace sf::priv
{
////////////////////////////////////////////////////////////
void JoystickRegistryImpl::initialize()
{
    {
        const std::lock_guard lock(m_mutex);
        m_initialized = true;
    }

    NSNotificationCenter* const center = [NSNotificationCenter defaultCenter];
    m_connectObserver                  = [center
        addObserverForName:GCControllerDidConnectNotification
                    object:nil
                     queue:nil
                usingBlock:^(NSNotification* notification) {
                    connect(notification.object);
                }];
    m_disconnectObserver               = [center
        addObserverForName:GCControllerDidDisconnectNotification
                    object:nil
                     queue:nil
                usingBlock:^(NSNotification* notification) {
                    disconnect(notification.object);
                }];

    refresh();
}


////////////////////////////////////////////////////////////
void JoystickRegistryImpl::cleanup()
{
    id connectObserver;
    id disconnectObserver;

    {
        const std::lock_guard lock(m_mutex);
        m_initialized        = false;
        connectObserver      = m_connectObserver;
        disconnectObserver   = m_disconnectObserver;
        m_connectObserver    = nil;
        m_disconnectObserver = nil;
        ++m_revision;

        for (Slot& slot : m_slots)
        {
            slot.controller = nil;
            ++slot.generation;
        }
    }

    NSNotificationCenter* const center = [NSNotificationCenter defaultCenter];
    if (connectObserver)
        [center removeObserver:connectObserver];
    if (disconnectObserver)
        [center removeObserver:disconnectObserver];
}


////////////////////////////////////////////////////////////
void JoystickRegistryImpl::refresh()
{
    std::uint64_t revision{};
    {
        const std::lock_guard lock(m_mutex);
        if (!m_initialized)
            return;

        revision = m_revision;
    }

    NSArray<GCController*>* const controllers = [GCController controllers];
    NSMutableArray<GCController*>* const supportedControllers = [[NSMutableArray alloc] initWithCapacity:controllers.count];
    for (GCController* controller in controllers) // NOLINT(cppcoreguidelines-init-variables)
    {
        if (controller.extendedGamepad)
            [supportedControllers addObject:controller];
    }

    const std::lock_guard lock(m_mutex);

    // A notification may have changed the connections while the list was being obtained.
    if (!m_initialized || revision != m_revision)
        return;

    bool changed = false;
    for (Slot& slot : m_slots)
    {
        if (slot.controller && [supportedControllers indexOfObjectIdenticalTo:slot.controller] == NSNotFound)
        {
            slot.controller = nil;
            ++slot.generation;
            changed = true;
        }
    }

    for (GCController* controller in supportedControllers) // NOLINT(cppcoreguidelines-init-variables)
        changed |= assignController(controller);

    if (changed)
        ++m_revision;
}


////////////////////////////////////////////////////////////
bool JoystickRegistryImpl::isConnected(unsigned int index)
{
    const std::lock_guard lock(m_mutex);
    return index < m_slots.size() && m_slots[index].controller != nil;
}


////////////////////////////////////////////////////////////
bool JoystickRegistryImpl::open(unsigned int index, std::uint64_t& generation)
{
    const std::lock_guard lock(m_mutex);
    if (index >= m_slots.size() || !m_slots[index].controller)
        return false;

    generation = m_slots[index].generation;
    return true;
}


////////////////////////////////////////////////////////////
GCController* JoystickRegistryImpl::getController(unsigned int index, std::uint64_t generation)
{
    const std::lock_guard lock(m_mutex);
    if (index >= m_slots.size() || m_slots[index].generation != generation)
        return nil;

    return m_slots[index].controller;
}


////////////////////////////////////////////////////////////
void JoystickRegistryImpl::connect(GCController* controller)
{
    const bool            supported = controller.extendedGamepad != nil;
    const std::lock_guard lock(m_mutex);
    if (!m_initialized)
        return;

    ++m_revision;
    if (supported)
        (void)assignController(controller);
}


////////////////////////////////////////////////////////////
void JoystickRegistryImpl::disconnect(GCController* controller)
{
    const std::lock_guard lock(m_mutex);
    if (!m_initialized)
        return;

    ++m_revision;
    for (Slot& slot : m_slots)
    {
        if (slot.controller == controller)
        {
            slot.controller = nil;
            ++slot.generation;
            return;
        }
    }
}


////////////////////////////////////////////////////////////
bool JoystickRegistryImpl::assignController(GCController* controller)
{
    for (const Slot& slot : m_slots)
    {
        if (slot.controller == controller)
            return false;
    }

    for (Slot& slot : m_slots)
    {
        if (!slot.controller)
        {
            slot.controller = controller;
            ++slot.generation;
            return true;
        }
    }

    return false;
}


////////////////////////////////////////////////////////////
void JoystickImpl::initialize()
{
    @autoreleasepool
    {
        getRegistry().initialize();
    }
}


////////////////////////////////////////////////////////////
void JoystickImpl::cleanup()
{
    @autoreleasepool
    {
        getRegistry().cleanup();
    }
}


////////////////////////////////////////////////////////////
bool JoystickImpl::isConnected(unsigned int index)
{
    @autoreleasepool
    {
        JoystickRegistryImpl& registry = getRegistry();
        registry.refresh();
        return registry.isConnected(index);
    }
}


////////////////////////////////////////////////////////////
bool JoystickImpl::open(unsigned int index)
{
    @autoreleasepool
    {
        close();

        if (!getRegistry().open(index, m_generation))
            return false;

        m_index = index;
        return true;
    }
}


////////////////////////////////////////////////////////////
void JoystickImpl::close()
{
    m_index      = Joystick::Count;
    m_generation = 0;
}


////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::getCapabilities() const
{
    @autoreleasepool
    {
        GCController* const      controller = getRegistry().getController(m_index, m_generation);
        GCExtendedGamepad* const gamepad    = controller.extendedGamepad;
        if (!gamepad)
            return {};

        JoystickCaps capabilities;
        capabilities.axes.fill(true);
        capabilities.buttonCount = 13;
        if (gamepad.buttonOptions)
            capabilities.buttonCount = 14;

        if (@available(iOS 14.0, *))
        {
            if (gamepad.buttonHome)
                capabilities.buttonCount = 15;
        }

        if (getShareButton(gamepad))
            capabilities.buttonCount = 16;

        return capabilities;
    }
}


////////////////////////////////////////////////////////////
Joystick::Identification JoystickImpl::getIdentification() const
{
    @autoreleasepool
    {
        GCController* const controller = getRegistry().getController(m_index, m_generation);
        if (!controller)
            return {};

        NSString* name = controller.vendorName;
        if (name.length == 0)
            name = controller.productCategory;

        Joystick::Identification identification;
        identification.name = "Game Controller";
        if (const char* utf8Name = name.UTF8String; utf8Name && *utf8Name)
            identification.name = String::fromUtf8(utf8Name, utf8Name + std::strlen(utf8Name));

        return identification;
    }
}


////////////////////////////////////////////////////////////
JoystickState JoystickImpl::update() const
{
    @autoreleasepool
    {
        JoystickRegistryImpl& registry = getRegistry();
        registry.refresh();

        GCController* const      controller = registry.getController(m_index, m_generation);
        GCExtendedGamepad* const gamepad    = controller.extendedGamepad;
        if (!gamepad)
            return {};

        JoystickState state;
        state.connected = true;

        if ([UIApplication sharedApplication].applicationState == UIApplicationStateActive)
        {
            state.axes[Joystick::Axis::X]    = gamepad.leftThumbstick.xAxis.value * 100.f;
            state.axes[Joystick::Axis::Y]    = gamepad.leftThumbstick.yAxis.value * -100.f;
            state.axes[Joystick::Axis::Z]    = gamepad.rightThumbstick.xAxis.value * 100.f;
            state.axes[Joystick::Axis::R]    = gamepad.rightThumbstick.yAxis.value * -100.f;
            state.axes[Joystick::Axis::U]    = gamepad.leftTrigger.value * 100.f;
            state.axes[Joystick::Axis::V]    = gamepad.rightTrigger.value * 100.f;
            state.axes[Joystick::Axis::PovX] = gamepad.dpad.xAxis.value * 100.f;
            state.axes[Joystick::Axis::PovY] = gamepad.dpad.yAxis.value * 100.f;

            state.buttons[0]  = gamepad.buttonA.pressed;
            state.buttons[1]  = gamepad.buttonB.pressed;
            state.buttons[3]  = gamepad.buttonX.pressed;
            state.buttons[4]  = gamepad.buttonY.pressed;
            state.buttons[6]  = gamepad.leftShoulder.pressed;
            state.buttons[7]  = gamepad.rightShoulder.pressed;
            state.buttons[8]  = gamepad.leftTrigger.pressed;
            state.buttons[9]  = gamepad.rightTrigger.pressed;
            state.buttons[10] = gamepad.leftThumbstickButton.pressed;
            state.buttons[11] = gamepad.rightThumbstickButton.pressed;
            state.buttons[12] = gamepad.buttonMenu.pressed;
            state.buttons[13] = gamepad.buttonOptions.pressed;

            if (@available(iOS 14.0, *))
                state.buttons[14] = gamepad.buttonHome.pressed;

            state.buttons[15] = getShareButton(gamepad).pressed;
        }

        // Do not return input from a connection invalidated while its state was being read.
        return registry.getController(m_index, m_generation) == controller ? state : JoystickState{};
    }
}

} // namespace sf::priv
