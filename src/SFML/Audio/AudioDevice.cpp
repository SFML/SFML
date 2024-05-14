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
#include <SFML/System/Vector3.hpp>

#include <miniaudio.h>

#include <algorithm>
#include <array>
#include <memory>
#include <optional>
#include <ostream>

namespace sf::priv
{
////////////////////////////////////////////////////////////
struct AudioDevice::ListenerProperties
{
    float          volume{100.f};
    Vector3f       position{0, 0, 0};
    Vector3f       direction{0, 0, -1};
    Vector3f       velocity{0, 0, 0};
    Listener::Cone cone{degrees(360.f), degrees(360.f), 1};
    Vector3f       upVector{0, 1, 0};
};


////////////////////////////////////////////////////////////
struct AudioDevice::Impl
{
    std::optional<ma_log>     log;            //!< The miniaudio log
    std::optional<ma_context> context;        //!< The miniaudio context
    std::optional<ma_device>  playbackDevice; //!< The miniaudio playback device
    std::optional<ma_engine>  engine;         //!< The miniaudio engine (used for effects and spatialisation)
};


////////////////////////////////////////////////////////////
AudioDevice::AudioDevice() : m_impl(std::make_unique<Impl>())
{
    // Ensure we only ever have a single AudioDevice instance
    assert(getInstance() == nullptr);
    getInstance() = this;

    // Create the log
    m_impl->log.emplace();

    if (const auto result = ma_log_init(nullptr, &*m_impl->log); result != MA_SUCCESS)
    {
        m_impl->log.reset();
        err() << "Failed to initialize the audio log: " << ma_result_description(result) << std::endl;
        return;
    }

    // Register our logging callback to output any warning/error messages
    if (const auto result = ma_log_register_callback(&*m_impl->log,
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
    m_impl->context.emplace();

    auto contextConfig                                 = ma_context_config_init();
    contextConfig.pLog                                 = &*m_impl->log;
    ma_uint32                              deviceCount = 0;
    const auto                             nullBackend = ma_backend_null;
    const std::array<const ma_backend*, 2> backendLists{nullptr, &nullBackend};

    for (const auto* backendList : backendLists)
    {
        // We can set backendCount to 1 since it is ignored when backends is set to nullptr
        if (const auto result = ma_context_init(backendList, 1, &contextConfig, &*m_impl->context); result != MA_SUCCESS)
        {
            m_impl->context.reset();
            err() << "Failed to initialize the audio playback context: " << ma_result_description(result) << std::endl;
            return;
        }

        // Count the playback devices
        if (const auto result = ma_context_get_devices(&*m_impl->context, nullptr, &deviceCount, nullptr, nullptr);
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
        ma_context_uninit(&*m_impl->context);
    }

    // If the NULL audio backend also doesn't provide a device we give up
    if (deviceCount == 0)
    {
        m_impl->context.reset();
        return;
    }

    if (m_impl->context->backend == ma_backend_null)
        err() << "Using NULL audio backend for playback" << std::endl;

    // Create the playback device
    m_impl->playbackDevice.emplace();

    auto playbackDeviceConfig         = ma_device_config_init(ma_device_type_playback);
    playbackDeviceConfig.dataCallback = [](ma_device* device, void* output, const void*, ma_uint32 frameCount)
    {
        auto& audioDevice = *static_cast<AudioDevice*>(device->pUserData);

        if (audioDevice.m_impl->engine)
        {
            if (const auto result = ma_engine_read_pcm_frames(&*audioDevice.m_impl->engine, output, frameCount, nullptr);
                result != MA_SUCCESS)
                err() << "Failed to read PCM frames from audio engine: " << ma_result_description(result) << std::endl;
        }
    };
    playbackDeviceConfig.pUserData       = this;
    playbackDeviceConfig.playback.format = ma_format_f32;

    if (const auto result = ma_device_init(&*m_impl->context, &playbackDeviceConfig, &*m_impl->playbackDevice);
        result != MA_SUCCESS)
    {
        m_impl->playbackDevice.reset();
        err() << "Failed to initialize the audio playback device: " << ma_result_description(result) << std::endl;
        return;
    }

    // Create the engine
    auto engineConfig          = ma_engine_config_init();
    engineConfig.pContext      = &*m_impl->context;
    engineConfig.pDevice       = &*m_impl->playbackDevice;
    engineConfig.listenerCount = 1;

    m_impl->engine.emplace();

    if (const auto result = ma_engine_init(&engineConfig, &*m_impl->engine); result != MA_SUCCESS)
    {
        m_impl->engine.reset();
        err() << "Failed to initialize the audio engine: " << ma_result_description(result) << std::endl;
        return;
    }

    // Set master volume, position, velocity, cone and world up vector
    ListenerProperties& listenerProperties = getListenerProperties();

    if (const auto result = ma_device_set_master_volume(ma_engine_get_device(&*m_impl->engine),
                                                        listenerProperties.volume * 0.01f);
        result != MA_SUCCESS)
        err() << "Failed to set audio device master volume: " << ma_result_description(result) << std::endl;

    ma_engine_listener_set_position(&*m_impl->engine,
                                    0,
                                    listenerProperties.position.x,
                                    listenerProperties.position.y,
                                    listenerProperties.position.z);

    ma_engine_listener_set_velocity(&*m_impl->engine,
                                    0,
                                    listenerProperties.velocity.x,
                                    listenerProperties.velocity.y,
                                    listenerProperties.velocity.z);

    ma_engine_listener_set_cone(&*m_impl->engine,
                                0,
                                listenerProperties.cone.innerAngle.asRadians(),
                                listenerProperties.cone.outerAngle.asRadians(),
                                listenerProperties.cone.outerGain);

    ma_engine_listener_set_world_up(&*m_impl->engine,
                                    0,
                                    listenerProperties.upVector.x,
                                    listenerProperties.upVector.y,
                                    listenerProperties.upVector.z);
}


////////////////////////////////////////////////////////////
AudioDevice::~AudioDevice()
{
    // Destroy the engine
    if (m_impl->engine)
        ma_engine_uninit(&*m_impl->engine);

    // Destroy the playback device
    if (m_impl->playbackDevice)
        ma_device_uninit(&*m_impl->playbackDevice);

    // Destroy the context
    if (m_impl->context)
        ma_context_uninit(&*m_impl->context);

    // Destroy the log
    if (m_impl->log)
        ma_log_uninit(&*m_impl->log);

    // Ensure we only ever have a single AudioDevice instance
    assert(getInstance() != nullptr);
    getInstance() = nullptr;
}


////////////////////////////////////////////////////////////
ma_engine* AudioDevice::getEngine()
{
    auto* instance = getInstance();

    if (instance && instance->m_impl->engine)
        return &*instance->m_impl->engine;

    return nullptr;
}


////////////////////////////////////////////////////////////
void AudioDevice::setGlobalVolume(float volume)
{
    // Store the volume in case no audio device exists yet
    getListenerProperties().volume = volume;

    auto* instance = getInstance();

    if (!instance || !instance->m_impl->engine)
        return;

    if (const auto result = ma_device_set_master_volume(ma_engine_get_device(&*instance->m_impl->engine), volume * 0.01f);
        result != MA_SUCCESS)
        err() << "Failed to set audio device master volume: " << ma_result_description(result) << std::endl;
}


////////////////////////////////////////////////////////////
float AudioDevice::getGlobalVolume()
{
    return getListenerProperties().volume;
}


////////////////////////////////////////////////////////////
void AudioDevice::setPosition(const Vector3f& position)
{
    // Store the position in case no audio device exists yet
    getListenerProperties().position = position;

    auto* instance = getInstance();

    if (!instance || !instance->m_impl->engine)
        return;

    ma_engine_listener_set_position(&*instance->m_impl->engine, 0, position.x, position.y, position.z);
}


////////////////////////////////////////////////////////////
Vector3f AudioDevice::getPosition()
{
    return getListenerProperties().position;
}


////////////////////////////////////////////////////////////
void AudioDevice::setDirection(const Vector3f& direction)
{
    // Store the direction in case no audio device exists yet
    getListenerProperties().direction = direction;

    auto* instance = getInstance();

    if (!instance || !instance->m_impl->engine)
        return;

    ma_engine_listener_set_direction(&*instance->m_impl->engine, 0, direction.x, direction.y, direction.z);
}


////////////////////////////////////////////////////////////
Vector3f AudioDevice::getDirection()
{
    return getListenerProperties().direction;
}


////////////////////////////////////////////////////////////
void AudioDevice::setVelocity(const Vector3f& velocity)
{
    // Store the velocity in case no audio device exists yet
    getListenerProperties().velocity = velocity;

    auto* instance = getInstance();

    if (!instance || !instance->m_impl->engine)
        return;

    ma_engine_listener_set_velocity(&*instance->m_impl->engine, 0, velocity.x, velocity.y, velocity.z);
}


////////////////////////////////////////////////////////////
Vector3f AudioDevice::getVelocity()
{
    return getListenerProperties().velocity;
}


////////////////////////////////////////////////////////////
void AudioDevice::setCone(const Listener::Cone& cone)
{
    // Store the cone in case no audio device exists yet
    getListenerProperties().cone = cone;

    auto* instance = getInstance();

    if (!instance || !instance->m_impl->engine)
        return;

    ma_engine_listener_set_cone(&*instance->m_impl->engine,
                                0,
                                std::clamp(cone.innerAngle, Angle::Zero, degrees(360.f)).asRadians(),
                                std::clamp(cone.outerAngle, Angle::Zero, degrees(360.f)).asRadians(),
                                cone.outerGain);
}


////////////////////////////////////////////////////////////
Listener::Cone AudioDevice::getCone()
{
    return getListenerProperties().cone;
}


////////////////////////////////////////////////////////////
void AudioDevice::setUpVector(const Vector3f& upVector)
{
    // Store the up vector in case no audio device exists yet
    getListenerProperties().upVector = upVector;

    auto* instance = getInstance();

    if (!instance || !instance->m_impl->engine)
        return;

    ma_engine_listener_set_world_up(&*instance->m_impl->engine, 0, upVector.x, upVector.y, upVector.z);
}


////////////////////////////////////////////////////////////
Vector3f AudioDevice::getUpVector()
{
    return getListenerProperties().upVector;
}


////////////////////////////////////////////////////////////
AudioDevice*& AudioDevice::getInstance()
{
    static AudioDevice* instance{};
    return instance;
}


////////////////////////////////////////////////////////////
AudioDevice::ListenerProperties& AudioDevice::getListenerProperties()
{
    static ListenerProperties properties;
    return properties;
}

} // namespace sf::priv
