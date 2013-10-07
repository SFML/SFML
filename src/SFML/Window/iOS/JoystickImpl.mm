////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Window/iOS/SFAppDelegate.hpp>


namespace
{
    enum
    {
        Accelerometer,
        Gyroscope,
        Magnetometer,
        UserAcceleration,
        AbsoluteOrientation
    };
}

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
void JoystickImpl::initialize()
{
    static const NSTimeInterval updateInterval = 1. / 60.;

    [SFAppDelegate getInstance].motionManager.accelerometerUpdateInterval = updateInterval;
    [[SFAppDelegate getInstance].motionManager startAccelerometerUpdates];

    [SFAppDelegate getInstance].motionManager.gyroUpdateInterval = updateInterval;
    [[SFAppDelegate getInstance].motionManager startGyroUpdates];

    [SFAppDelegate getInstance].motionManager.magnetometerUpdateInterval = updateInterval;
    [[SFAppDelegate getInstance].motionManager startMagnetometerUpdates];

    [SFAppDelegate getInstance].motionManager.deviceMotionUpdateInterval = updateInterval;
    [[SFAppDelegate getInstance].motionManager startDeviceMotionUpdates];
}


////////////////////////////////////////////////////////////
void JoystickImpl::cleanup()
{
    if ([SFAppDelegate getInstance].motionManager.accelerometerActive)
        [[SFAppDelegate getInstance].motionManager stopAccelerometerUpdates];

    if ([SFAppDelegate getInstance].motionManager.gyroActive)
        [[SFAppDelegate getInstance].motionManager stopGyroUpdates];

    if ([SFAppDelegate getInstance].motionManager.magnetometerActive)
        [[SFAppDelegate getInstance].motionManager stopMagnetometerUpdates];

    if ([SFAppDelegate getInstance].motionManager.deviceMotionActive)
        [[SFAppDelegate getInstance].motionManager stopDeviceMotionUpdates];
}


////////////////////////////////////////////////////////////
bool JoystickImpl::isConnected(unsigned int index)
{
    switch (index)
    {
        case Accelerometer:
            return [SFAppDelegate getInstance].motionManager.accelerometerAvailable;

        case Gyroscope:
            return [SFAppDelegate getInstance].motionManager.gyroAvailable;

        case Magnetometer:
            return [SFAppDelegate getInstance].motionManager.magnetometerAvailable;

        case UserAcceleration:
        case AbsoluteOrientation:
            return [SFAppDelegate getInstance].motionManager.deviceMotionAvailable;

        default:
            return false;
    }
}


////////////////////////////////////////////////////////////
bool JoystickImpl::open(unsigned int index)
{
    // Save the index
    m_index = index;

    return true;
}


////////////////////////////////////////////////////////////
void JoystickImpl::close()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::getCapabilities() const
{
    JoystickCaps caps;

    // All the connected joysticks have (X, Y, Z) axes
    caps.axes[Joystick::X] = true;
    caps.axes[Joystick::Y] = true;
    caps.axes[Joystick::Z] = true;

    return caps;
}


////////////////////////////////////////////////////////////
JoystickState JoystickImpl::update()
{
    JoystickState state;

    // Always connected
    state.connected = true;

    CMMotionManager* manager = [SFAppDelegate getInstance].motionManager;

    switch (m_index)
    {
        case Accelerometer:
            // Acceleration is given in G (reminder: 1G is regular earth gravity, 9.81 m/s^2), in range [-2, 2] (could be more on newer devices?)
            state.axes[Joystick::X] = manager.accelerometerData.acceleration.x;
            state.axes[Joystick::Y] = manager.accelerometerData.acceleration.y;
            state.axes[Joystick::Z] = manager.accelerometerData.acceleration.z;
            break;

        case Gyroscope:
            // Rotation rates are given in rad/s, in range [?, ?]
            state.axes[Joystick::X] = manager.gyroData.rotationRate.x;
            state.axes[Joystick::Y] = manager.gyroData.rotationRate.y;
            state.axes[Joystick::Z] = manager.gyroData.rotationRate.z;
            break;

        case Magnetometer:
            // Magnetic field is given in microteslas, in range [?, ?]
            state.axes[Joystick::X] = manager.magnetometerData.magneticField.x;
            state.axes[Joystick::Y] = manager.magnetometerData.magneticField.y;
            state.axes[Joystick::Z] = manager.magnetometerData.magneticField.z;
            break;

        case UserAcceleration:
            // User acceleration (gravity removed), same unit and range as raw accelerometer values
            state.axes[Joystick::X] = manager.deviceMotion.userAcceleration.x;
            state.axes[Joystick::Y] = manager.deviceMotion.userAcceleration.y;
            state.axes[Joystick::Z] = manager.deviceMotion.userAcceleration.z;
            break;

        case AbsoluteOrientation:
            // Absolute rotation (Euler) angles are given in radians, in range [-PI, PI]
            state.axes[Joystick::X] = manager.deviceMotion.attitude.yaw;
            state.axes[Joystick::Y] = manager.deviceMotion.attitude.pitch;
            state.axes[Joystick::Z] = manager.deviceMotion.attitude.roll;
            break;

        default:
            break;
    }

    return state;
}

} // namespace priv

} // namespace sf
