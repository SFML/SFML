////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2026 Marco Antognini (antognini.marco@gmail.com),
//                         Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/macOS/AutoreleasePoolWrapper.hpp>
#include <SFML/Window/macOS/GameControllerManager.hpp>

#import <GameController/GameController.h>
#include <algorithm>
#include <array>

#include <cstring>

namespace
{
using Buttons = std::array<GCControllerButtonInput*, sf::Joystick::ButtonCount>;

////////////////////////////////////////////////////////////
Buttons getButtons(GCExtendedGamepad* gamepad)
{
    if (@available(macOS 11.0, *))
    {
        // Preserve the HID button indices used by Xbox and PlayStation controllers.
        if ([gamepad isKindOfClass:[GCXboxGamepad class]])
        {
            auto* const xbox = static_cast<GCXboxGamepad*>(gamepad);
            Buttons     buttons{};
            buttons[0]  = gamepad.buttonA;
            buttons[1]  = gamepad.buttonB;
            buttons[3]  = gamepad.buttonX;
            buttons[4]  = gamepad.buttonY;
            buttons[6]  = gamepad.leftShoulder;
            buttons[7]  = gamepad.rightShoulder;
            buttons[10] = gamepad.buttonOptions;
            buttons[11] = gamepad.buttonMenu;
            buttons[12] = gamepad.buttonHome;
            buttons[13] = gamepad.leftThumbstickButton;
            buttons[14] = gamepad.rightThumbstickButton;
            if (@available(macOS 12.0, *))
                buttons[15] = xbox.buttonShare;
            buttons[16] = xbox.paddleButton1;
            buttons[17] = xbox.paddleButton2;
            buttons[18] = xbox.paddleButton3;
            buttons[19] = xbox.paddleButton4;
            return buttons;
        }

        GCControllerButtonInput* touchpad = nil;
        if ([gamepad isKindOfClass:[GCDualShockGamepad class]])
            touchpad = static_cast<GCDualShockGamepad*>(gamepad).touchpadButton;
        if (@available(macOS 11.3, *))
        {
            if ([gamepad isKindOfClass:[GCDualSenseGamepad class]])
                touchpad = static_cast<GCDualSenseGamepad*>(gamepad).touchpadButton;
        }
        if (touchpad)
        {
            return {gamepad.buttonX,
                    gamepad.buttonA,
                    gamepad.buttonB,
                    gamepad.buttonY,
                    gamepad.leftShoulder,
                    gamepad.rightShoulder,
                    gamepad.leftTrigger,
                    gamepad.rightTrigger,
                    gamepad.buttonOptions,
                    gamepad.buttonMenu,
                    gamepad.leftThumbstickButton,
                    gamepad.rightThumbstickButton,
                    gamepad.buttonHome,
                    touchpad};
        }

        // Other extended profiles use a dense, profile-independent ordering.
        return {gamepad.buttonA,
                gamepad.buttonB,
                gamepad.buttonX,
                gamepad.buttonY,
                gamepad.leftShoulder,
                gamepad.rightShoulder,
                gamepad.leftTrigger,
                gamepad.rightTrigger,
                gamepad.buttonOptions,
                gamepad.buttonMenu,
                gamepad.leftThumbstickButton,
                gamepad.rightThumbstickButton,
                gamepad.buttonHome};
    }

    return {};
}

////////////////////////////////////////////////////////////
Buttons getButtons(GCController* controller)
{
    if (controller.extendedGamepad)
        return getButtons(controller.extendedGamepad);

    if (@available(macOS 11.0, *))
    {
        if (GCMicroGamepad* const gamepad = controller.microGamepad)
            return {gamepad.buttonA, gamepad.buttonX, gamepad.buttonMenu};
    }

    return {};
}

////////////////////////////////////////////////////////////
void updateRunLoop()
{
    // Joystick::update() must also work without an SFML window pumping events.
    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, true);
}
} // namespace

namespace sf::priv
{
////////////////////////////////////////////////////////////
GameControllerManager& GameControllerManager::getInstance()
{
    static GameControllerManager instance;
    return instance;
}


////////////////////////////////////////////////////////////
GameControllerManager::GameControllerManager()
{
    const AutoreleasePool pool;
    if (@available(macOS 11.3, *))
    {
        // Match SFML's real-time joystick polling when the application is unfocused.
        m_monitoredBackgroundEvents                = GCController.shouldMonitorBackgroundEvents;
        GCController.shouldMonitorBackgroundEvents = YES;
    }
}


////////////////////////////////////////////////////////////
GameControllerManager::~GameControllerManager()
{
    const AutoreleasePool pool;
    for (unsigned int slot = 0; slot < Joystick::Count; ++slot)
        close(slot);
    if (@available(macOS 11.3, *))
        GCController.shouldMonitorBackgroundEvents = m_monitoredBackgroundEvents;
}


////////////////////////////////////////////////////////////
bool GameControllerManager::supportsHIDDevice(IOHIDDeviceRef device)
{
    const AutoreleasePool pool;
    if (@available(macOS 11.0, *))
        return [GCController supportsHIDDevice:device];

    return false;
}


////////////////////////////////////////////////////////////
bool GameControllerManager::isConnected(unsigned int slot)
{
    const AutoreleasePool pool;
    if (@available(macOS 11.0, *))
    {
        updateRunLoop();
        NSArray<GCController*>* const controllers = [GCController controllers];
        if (m_controllers[slot])
        {
            if ([controllers containsObject:static_cast<GCController*>(m_controllers[slot])])
                return true;
            close(slot);
            return false;
        }

        for (GCController* controller in controllers) // NOLINT(cppcoreguidelines-init-variables)
        {
            if (!controller.extendedGamepad && !controller.microGamepad)
                continue;

            if (std::find(m_controllers.begin(), m_controllers.end(), controller) == m_controllers.end())
            {
                m_controllers[slot] = [controller retain];
                return true;
            }
        }
    }

    return false;
}


////////////////////////////////////////////////////////////
bool GameControllerManager::open(unsigned int slot)
{
    return isConnected(slot);
}


////////////////////////////////////////////////////////////
void GameControllerManager::close(unsigned int slot)
{
    [static_cast<GCController*>(m_controllers[slot]) release];
    m_controllers[slot] = nullptr;
}


////////////////////////////////////////////////////////////
JoystickCaps GameControllerManager::getCapabilities(unsigned int slot) const
{
    const AutoreleasePool pool;
    JoystickCaps          caps;
    auto* const           controller = static_cast<GCController*>(m_controllers[slot]);
    const Buttons         buttons    = getButtons(controller);
    for (unsigned int index = 0; index < buttons.size(); ++index)
    {
        if (buttons[index])
            caps.buttonCount = index + 1;
    }

    if (controller.extendedGamepad)
    {
        caps.axes[Joystick::Axis::X] = true;
        caps.axes[Joystick::Axis::Y] = true;
        caps.axes[Joystick::Axis::Z] = true;
        caps.axes[Joystick::Axis::R] = true;
        caps.axes[Joystick::Axis::U] = true;
        caps.axes[Joystick::Axis::V] = true;
    }
    if (controller.extendedGamepad || controller.microGamepad)
    {
        caps.axes[Joystick::Axis::PovX] = true;
        caps.axes[Joystick::Axis::PovY] = true;
    }
    return caps;
}


////////////////////////////////////////////////////////////
Joystick::Identification GameControllerManager::getIdentification(unsigned int slot) const
{
    const AutoreleasePool    pool;
    Joystick::Identification identification;
    if (@available(macOS 11.0, *))
    {
        auto* const     controller = static_cast<GCController*>(m_controllers[slot]);
        NSString*       name       = controller.vendorName;
        NSString* const category   = controller.productCategory;
        if (category.length &&
            (!name.length || [name rangeOfString:category options:NSCaseInsensitiveSearch].location == NSNotFound))
            name = name.length ? [NSString stringWithFormat:@"%@ (%@)", name, category] : category;
        if (const char* utf8Name = [name UTF8String])
            identification.name = String::fromUtf8(utf8Name, utf8Name + std::strlen(utf8Name));
        // GameController does not expose USB vendor/product IDs. Do not pair devices
        // with unrelated HID entries by name or enumeration order to invent them.
    }
    return identification;
}


////////////////////////////////////////////////////////////
JoystickState GameControllerManager::update(unsigned int slot)
{
    const AutoreleasePool pool;
    JoystickState         state;
    if (@available(macOS 11.0, *))
    {
        if (!m_controllers[slot] || !isConnected(slot))
            return state;

        state.connected                = true;
        GCController* const controller = [static_cast<GCController*>(m_controllers[slot]) capture];
        const Buttons       buttons    = getButtons(controller);
        for (unsigned int index = 0; index < buttons.size(); ++index)
            state.buttons[index] = buttons[index].pressed;

        GCControllerDirectionPad* dpad = nil;
        if (GCExtendedGamepad* const gamepad = controller.extendedGamepad)
        {
            state.axes[Joystick::Axis::X] = gamepad.leftThumbstick.xAxis.value * 100.f;
            state.axes[Joystick::Axis::Y] = -gamepad.leftThumbstick.yAxis.value * 100.f;
            state.axes[Joystick::Axis::Z] = gamepad.rightThumbstick.xAxis.value * 100.f;
            state.axes[Joystick::Axis::R] = -gamepad.rightThumbstick.yAxis.value * 100.f;
            state.axes[Joystick::Axis::U] = gamepad.leftTrigger.value * 200.f - 100.f;
            state.axes[Joystick::Axis::V] = gamepad.rightTrigger.value * 200.f - 100.f;
            dpad                          = gamepad.dpad;
        }
        else
        {
            dpad = controller.microGamepad.dpad;
        }
        state.axes[Joystick::Axis::PovX] = dpad.xAxis.value * 100.f;
        state.axes[Joystick::Axis::PovY] = dpad.yAxis.value * 100.f;
    }
    return state;
}

} // namespace sf::priv
