////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Audio/AudioDevice.hpp>

#include <SFML/System/Err.hpp>

#include <algorithm>
#include <array>
#include <ostream>

#include <cassert>
#include <cstdlib>


namespace sf::priv
{
////////////////////////////////////////////////////////////
AudioDevice::AudioDevice()
{
    // Create the log
    m_log.emplace();

    if (const auto result = ma_log_init(nullptr, &*m_log); result != MA_SUCCESS)
    {
        m_log.reset();
        err() << "Failed to initialize the audio log: " << ma_result_description(result) << std::endl;
        return;
    }

    // Register our logging callback to output any warning/error messages
    if (const auto result = ma_log_register_callback(&*m_log,
                                                     ma_log_callback_init(
                                                         [](void*, ma_uint32 level, const char* message)
                                                         {
                                                             if (level <= MA_LOG_LEVEL_WARNING)
                                                                 err() << "miniaudio " << ma_log_level_to_string(level)
                                                                       << ": " << message << std::flush;
                                                         },
                                                         nullptr));
        result != MA_SUCCESS)
        err() << "Failed to register audio log callback: " << ma_result_description(result) << std::endl;

    // Create the context
    m_context.emplace();

    auto contextConfig                                 = ma_context_config_init();
    contextConfig.pLog                                 = &*m_log;
    ma_uint32                              deviceCount = 0;
    const auto                             nullBackend = ma_backend_null;
    const std::array<const ma_backend*, 2> backendLists{nullptr, &nullBackend};

    for (const auto* backendList : backendLists)
    {
        // We can set backendCount to 1 since it is ignored when backends is set to nullptr
        if (const auto result = ma_context_init(backendList, 1, &contextConfig, &*m_context); result != MA_SUCCESS)
        {
            m_context.reset();
            err() << "Failed to initialize the audio playback context: " << ma_result_description(result) << std::endl;
            return;
        }

        // Count the playback devices
        if (const auto result = ma_context_get_devices(&*m_context, nullptr, &deviceCount, nullptr, nullptr);
            result != MA_SUCCESS)
        {
            err() << "Failed to get audio playback devices: " << ma_result_description(result) << std::endl;
            return;
        }

        // Check if there are audio playback devices available on the system
        if (deviceCount > 0)
            break;

        // Warn if no devices were found using the default backend list
        if (backendList == nullptr)
            err() << "No audio playback devices available on the system" << std::endl;

        // Clean up the context if we didn't find any devices
        ma_context_uninit(&*m_context);
    }

    // If the NULL audio backend also doesn't provide a device we give up
    if (deviceCount == 0)
    {
        m_context.reset();
        return;
    }

    if (m_context->backend == ma_backend_null)
        err() << "Using NULL audio backend for playback" << std::endl;

    // Create the playback device
    m_playbackDevice.emplace();

    auto playbackDeviceConfig         = ma_device_config_init(ma_device_type_playback);
    playbackDeviceConfig.dataCallback = [](ma_device* device, void* output, const void*, ma_uint32 frameCount)
    {
        auto& audioDevice = *static_cast<AudioDevice*>(device->pUserData);

        if (audioDevice.m_engine)
        {
            if (const auto result = ma_engine_read_pcm_frames(&*audioDevice.m_engine, output, frameCount, nullptr);
                result != MA_SUCCESS)
                err() << "Failed to read PCM frames from audio engine: " << ma_result_description(result) << std::endl;
        }
    };
    playbackDeviceConfig.pUserData       = this;
    playbackDeviceConfig.playback.format = ma_format_f32;

    if (const auto result = ma_device_init(&*m_context, &playbackDeviceConfig, &*m_playbackDevice); result != MA_SUCCESS)
    {
        m_playbackDevice.reset();
        err() << "Failed to initialize the audio playback device: " << ma_result_description(result) << std::endl;
        return;
    }

    // Create the engine
    auto engineConfig          = ma_engine_config_init();
    engineConfig.pContext      = &*m_context;
    engineConfig.pDevice       = &*m_playbackDevice;
    engineConfig.listenerCount = 1;

    m_engine.emplace();

    if (const auto result = ma_engine_init(&engineConfig, &*m_engine); result != MA_SUCCESS)
    {
        m_engine.reset();
        err() << "Failed to initialize the audio engine: " << ma_result_description(result) << std::endl;
        return;
    }

    // Set master volume, position, velocity, cone and world up vector
    if (const auto result = ma_device_set_master_volume(ma_engine_get_device(&*m_engine), m_listenerProperties.volume * 0.01f);
        result != MA_SUCCESS)
        err() << "Failed to set audio device master volume: " << ma_result_description(result) << std::endl;

    ma_engine_listener_set_position(&*m_engine,
                                    0,
                                    m_listenerProperties.position.x,
                                    m_listenerProperties.position.y,
                                    m_listenerProperties.position.z);

    ma_engine_listener_set_velocity(&*m_engine,
                                    0,
                                    m_listenerProperties.velocity.x,
                                    m_listenerProperties.velocity.y,
                                    m_listenerProperties.velocity.z);

    ma_engine_listener_set_cone(&*m_engine,
                                0,
                                m_listenerProperties.cone.innerAngle.asRadians(),
                                m_listenerProperties.cone.outerAngle.asRadians(),
                                m_listenerProperties.cone.outerGain);

    ma_engine_listener_set_world_up(&*m_engine,
                                    0,
                                    m_listenerProperties.upVector.x,
                                    m_listenerProperties.upVector.y,
                                    m_listenerProperties.upVector.z);
}


////////////////////////////////////////////////////////////
AudioDevice::~AudioDevice()
{
    // Destroy the engine
    if (m_engine)
        ma_engine_uninit(&*m_engine);

    // Destroy the playback device
    if (m_playbackDevice)
        ma_device_uninit(&*m_playbackDevice);

    // Destroy the context
    if (m_context)
        ma_context_uninit(&*m_context);

    // Destroy the log
    if (m_log)
        ma_log_uninit(&*m_log);
}


////////////////////////////////////////////////////////////
ma_engine* AudioDevice::getEngine()
{
    if (m_engine.has_value())
        return &*m_engine;

    return nullptr;
}


////////////////////////////////////////////////////////////
void AudioDevice::setGlobalVolume(float volume)
{
    // Store the volume in case no audio device exists yet
    m_listenerProperties.volume = volume;

    if (!m_engine.has_value())
        return;

    if (const auto result = ma_device_set_master_volume(ma_engine_get_device(&*m_engine), volume * 0.01f);
        result != MA_SUCCESS)
        err() << "Failed to set audio device master volume: " << ma_result_description(result) << std::endl;
}


////////////////////////////////////////////////////////////
float AudioDevice::getGlobalVolume() const
{
    return m_listenerProperties.volume;
}


////////////////////////////////////////////////////////////
void AudioDevice::setPosition(const Vector3f& position)
{
    // Store the position in case no audio device exists yet
    m_listenerProperties.position = position;

    if (!m_engine.has_value())
        return;

    ma_engine_listener_set_position(&*m_engine, 0, position.x, position.y, position.z);
}


////////////////////////////////////////////////////////////
Vector3f AudioDevice::getPosition() const
{
    return m_listenerProperties.position;
}


////////////////////////////////////////////////////////////
void AudioDevice::setDirection(const Vector3f& direction)
{
    // Store the direction in case no audio device exists yet
    m_listenerProperties.direction = direction;

    if (!m_engine.has_value())
        return;

    ma_engine_listener_set_direction(&*m_engine, 0, direction.x, direction.y, direction.z);
}


////////////////////////////////////////////////////////////
Vector3f AudioDevice::getDirection() const
{
    return m_listenerProperties.direction;
}


////////////////////////////////////////////////////////////
void AudioDevice::setVelocity(const Vector3f& velocity)
{
    // Store the velocity in case no audio device exists yet
    m_listenerProperties.velocity = velocity;

    if (!m_engine.has_value())
        return;

    ma_engine_listener_set_velocity(&*m_engine, 0, velocity.x, velocity.y, velocity.z);
}


////////////////////////////////////////////////////////////
Vector3f AudioDevice::getVelocity() const
{
    return m_listenerProperties.velocity;
}


////////////////////////////////////////////////////////////
void AudioDevice::setCone(const Listener::Cone& cone)
{
    // Store the cone in case no audio device exists yet
    m_listenerProperties.cone = cone;

    if (!m_engine.has_value())
        return;

    ma_engine_listener_set_cone(&*m_engine,
                                0,
                                std::clamp(cone.innerAngle, Angle::Zero, degrees(360.f)).asRadians(),
                                std::clamp(cone.outerAngle, Angle::Zero, degrees(360.f)).asRadians(),
                                cone.outerGain);
}


////////////////////////////////////////////////////////////
Listener::Cone AudioDevice::getCone() const
{
    return m_listenerProperties.cone;
}


////////////////////////////////////////////////////////////
void AudioDevice::setUpVector(const Vector3f& upVector)
{
    // Store the up vector in case no audio device exists yet
    m_listenerProperties.upVector = upVector;

    if (!m_engine.has_value())
        return;

    ma_engine_listener_set_world_up(&*m_engine, 0, upVector.x, upVector.y, upVector.z);
}


////////////////////////////////////////////////////////////
Vector3f AudioDevice::getUpVector() const
{
    return m_listenerProperties.upVector;
}


////////////////////////////////////////////////////////////
AudioDevice& AudioDevice::get()
{
    static AudioDevice instance;
    return instance;
}

} // namespace sf::priv
