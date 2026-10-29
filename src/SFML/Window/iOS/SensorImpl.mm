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
#include <SFML/Window/SensorImpl.hpp>
#include <SFML/Window/iOS/SFAppDelegate.hpp>

#include <SFML/System/Angle.hpp>


namespace
{
unsigned int deviceMotionEnabledCount = 0;
bool         magnetometerEnabled      = false;

////////////////////////////////////////////////////////////
void restartDeviceMotion()
{
    CMMotionManager* const manager = [SFAppDelegate getInstance].motionManager;

    [manager stopDeviceMotionUpdates];

    if (deviceMotionEnabledCount == 0)
        return;

    // The calibrated magnetic field is only provided when the reference frame uses the magnetometer
    if (magnetometerEnabled)
        [manager startDeviceMotionUpdatesUsingReferenceFrame:CMAttitudeReferenceFrameXArbitraryCorrectedZVertical];
    else
        [manager startDeviceMotionUpdates];
}
} // namespace


namespace sf::priv
{
////////////////////////////////////////////////////////////
void SensorImpl::initialize()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
void SensorImpl::cleanup()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
bool SensorImpl::isAvailable(Sensor::Type sensor)
{
    switch (sensor)
    {
        case Sensor::Type::Accelerometer:
            return [SFAppDelegate getInstance].motionManager.accelerometerAvailable;

        case Sensor::Type::Gyroscope:
            return [SFAppDelegate getInstance].motionManager.gyroAvailable;

        case Sensor::Type::Magnetometer:
            return [SFAppDelegate getInstance].motionManager.deviceMotionAvailable &&
                   ([CMMotionManager availableAttitudeReferenceFrames] &
                    CMAttitudeReferenceFrameXArbitraryCorrectedZVertical) != 0;

        case Sensor::Type::Gravity:
        case Sensor::Type::UserAcceleration:
        case Sensor::Type::Orientation:
            return [SFAppDelegate getInstance].motionManager.deviceMotionAvailable;

        default:
            return false;
    }
}


////////////////////////////////////////////////////////////
bool SensorImpl::open(Sensor::Type sensor)
{
    // Store the sensor type
    m_sensor = sensor;

    // The sensor is disabled by default
    m_enabled = false;

    // Set the refresh rate (use the maximum allowed)
    constexpr NSTimeInterval updateInterval = 1. / 60.;
    switch (sensor)
    {
        case Sensor::Type::Accelerometer:
            [SFAppDelegate getInstance].motionManager.accelerometerUpdateInterval = updateInterval;
            break;

        case Sensor::Type::Gyroscope:
            [SFAppDelegate getInstance].motionManager.gyroUpdateInterval = updateInterval;
            break;

        case Sensor::Type::Magnetometer:
        case Sensor::Type::Gravity:
        case Sensor::Type::UserAcceleration:
        case Sensor::Type::Orientation:
            [SFAppDelegate getInstance].motionManager.deviceMotionUpdateInterval = updateInterval;
            break;

        default:
            break;
    }

    return true;
}


////////////////////////////////////////////////////////////
void SensorImpl::close()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
Vector3f SensorImpl::update()
{
    CMMotionManager* const manager = [SFAppDelegate getInstance].motionManager;

    switch (m_sensor)
    {
        case Sensor::Type::Accelerometer:
            // Acceleration is given in G, convert to m/s^2
            return Vector3f(9.81 * Vector3(manager.accelerometerData.acceleration.x,
                                           manager.accelerometerData.acceleration.y,
                                           manager.accelerometerData.acceleration.z));

        case Sensor::Type::Gyroscope:
            // Rotation rates are given in rad/s
            return Vector3f(
                Vector3(manager.gyroData.rotationRate.x, manager.gyroData.rotationRate.y, manager.gyroData.rotationRate.z));

        case Sensor::Type::Magnetometer:
            // Calibrated magnetic field is given in microteslas
            return Vector3f(Vector3(manager.deviceMotion.magneticField.field.x,
                                    manager.deviceMotion.magneticField.field.y,
                                    manager.deviceMotion.magneticField.field.z));

        case Sensor::Type::Gravity:
            // Gravity is given in G, convert to m/s^2
            return Vector3f(
                9.81 *
                Vector3(manager.deviceMotion.gravity.x, manager.deviceMotion.gravity.y, manager.deviceMotion.gravity.z));

        case Sensor::Type::UserAcceleration:
            // User acceleration is given in G, convert to m/s^2
            return Vector3f(9.81 * Vector3(manager.deviceMotion.userAcceleration.x,
                                           manager.deviceMotion.userAcceleration.y,
                                           manager.deviceMotion.userAcceleration.z));

        case Sensor::Type::Orientation:
            // Absolute rotation (Euler) angles are given in radians
            return Vector3f(Vector3<double>(manager.deviceMotion.attitude.yaw,
                                            manager.deviceMotion.attitude.pitch,
                                            manager.deviceMotion.attitude.roll));

        default:
            return {};
    }
}


////////////////////////////////////////////////////////////
void SensorImpl::setEnabled(bool enabled)
{
    // Don't do anything if the state is the same
    if (enabled == m_enabled)
        return;

    switch (m_sensor)
    {
        case Sensor::Type::Accelerometer:
            if (enabled)
                [[SFAppDelegate getInstance].motionManager startAccelerometerUpdates];
            else
                [[SFAppDelegate getInstance].motionManager stopAccelerometerUpdates];
            break;

        case Sensor::Type::Gyroscope:
            if (enabled)
                [[SFAppDelegate getInstance].motionManager startGyroUpdates];
            else
                [[SFAppDelegate getInstance].motionManager stopGyroUpdates];
            break;

        case Sensor::Type::Magnetometer:
        case Sensor::Type::Gravity:
        case Sensor::Type::UserAcceleration:
        case Sensor::Type::Orientation:
        {
            // these sensors all share device motion, which is restarted to change
            // its reference frame when the magnetometer is toggled
            const bool wasRunning           = deviceMotionEnabledCount > 0;
            const bool wasUsingMagnetometer = magnetometerEnabled;

            if (enabled)
                ++deviceMotionEnabledCount;
            else
                --deviceMotionEnabledCount;

            if (m_sensor == Sensor::Type::Magnetometer)
                magnetometerEnabled = enabled;

            if (wasRunning != (deviceMotionEnabledCount > 0) || wasUsingMagnetometer != magnetometerEnabled)
                restartDeviceMotion();
            break;
        }

        default:
            break;
    }

    // Update the enable state
    m_enabled = enabled;
}

} // namespace sf::priv
