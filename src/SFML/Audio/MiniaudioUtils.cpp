////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Audio/MiniaudioUtils.hpp>
#include <SFML/Audio/SoundChannel.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Time.hpp>

#include <miniaudio.h>

#include <ostream>

#include <cassert>
#include <cstring>


namespace
{
////////////////////////////////////////////////////////////
sf::priv::MiniaudioUtils::SavedSettings saveSettings(const ma_sound& sound)
{
    float innerAngle = 0;
    float outerAngle = 0;
    float outerGain  = 0;
    ma_sound_get_cone(&sound, &innerAngle, &outerAngle, &outerGain);

    return sf::priv::MiniaudioUtils::
        SavedSettings{ma_sound_get_pitch(&sound),
                      ma_sound_get_pan(&sound),
                      ma_sound_get_volume(&sound),
                      ma_sound_is_spatialization_enabled(&sound),
                      ma_sound_get_position(&sound),
                      ma_sound_get_direction(&sound),
                      ma_sound_get_directional_attenuation_factor(&sound),
                      ma_sound_get_velocity(&sound),
                      ma_sound_get_doppler_factor(&sound),
                      ma_sound_get_positioning(&sound),
                      ma_sound_get_min_distance(&sound),
                      ma_sound_get_max_distance(&sound),
                      ma_sound_get_min_gain(&sound),
                      ma_sound_get_max_gain(&sound),
                      ma_sound_get_rolloff(&sound),
                      ma_sound_is_playing(&sound),
                      ma_sound_is_looping(&sound),
                      innerAngle,
                      outerAngle,
                      outerGain};
}


////////////////////////////////////////////////////////////
void applySettings(ma_sound& sound, const sf::priv::MiniaudioUtils::SavedSettings& savedSettings)
{
    ma_sound_set_pitch(&sound, savedSettings.pitch);
    ma_sound_set_pan(&sound, savedSettings.pan);
    ma_sound_set_volume(&sound, savedSettings.volume);
    ma_sound_set_spatialization_enabled(&sound, savedSettings.spatializationEnabled);
    ma_sound_set_position(&sound, savedSettings.position.x, savedSettings.position.y, savedSettings.position.z);
    ma_sound_set_direction(&sound, savedSettings.direction.x, savedSettings.direction.y, savedSettings.direction.z);
    ma_sound_set_directional_attenuation_factor(&sound, savedSettings.directionalAttenuationFactor);
    ma_sound_set_velocity(&sound, savedSettings.velocity.x, savedSettings.velocity.y, savedSettings.velocity.z);
    ma_sound_set_doppler_factor(&sound, savedSettings.dopplerFactor);
    ma_sound_set_positioning(&sound, savedSettings.positioning);
    ma_sound_set_min_distance(&sound, savedSettings.minDistance);
    ma_sound_set_max_distance(&sound, savedSettings.maxDistance);
    ma_sound_set_min_gain(&sound, savedSettings.minGain);
    ma_sound_set_max_gain(&sound, savedSettings.maxGain);
    ma_sound_set_rolloff(&sound, savedSettings.rollOff);
    ma_sound_set_looping(&sound, savedSettings.looping);

    ma_sound_set_cone(&sound, savedSettings.innerAngle, savedSettings.outerAngle, savedSettings.outerGain);

    if (savedSettings.playing)
    {
        ma_sound_start(&sound);
    }
    else
    {
        ma_sound_stop(&sound);
    }
}
} // namespace


namespace sf::priv
{
////////////////////////////////////////////////////////////
MiniaudioUtils::SoundBase::SoundBase(const ma_data_source_vtable&     dataSourceVTable,
                                     AudioDevice::ResourceEntry::Func reinitializeFunc)
{
    // Set this object up as a miniaudio data source
    ma_data_source_config config = ma_data_source_config_init();
    config.vtable                = &dataSourceVTable;

    if (const ma_result result = ma_data_source_init(&config, &dataSourceBase); result != MA_SUCCESS)
        err() << "Failed to initialize audio data source: " << ma_result_description(result) << std::endl;

    resourceEntryIter = AudioDevice::registerResource(
        this,
        [](void* ptr) { static_cast<SoundBase*>(ptr)->deinitialize(); },
        reinitializeFunc);
}


////////////////////////////////////////////////////////////
MiniaudioUtils::SoundBase::~SoundBase()
{
    AudioDevice::unregisterResource(resourceEntryIter);
    ma_sound_uninit(&sound);
    ma_node_uninit(&effectNode, nullptr);
    ma_data_source_uninit(&dataSourceBase);
}


////////////////////////////////////////////////////////////
void MiniaudioUtils::SoundBase::initialize(ma_sound_end_proc endCallback)
{
    // Initialize the sound
    auto* engine = AudioDevice::getEngine();

    if (engine == nullptr)
    {
        err() << "Failed to initialize sound: No engine available" << std::endl;
        return;
    }

    ma_sound_config soundConfig;

    soundConfig                      = ma_sound_config_init();
    soundConfig.pDataSource          = this;
    soundConfig.pEndCallbackUserData = this;
    soundConfig.endCallback          = endCallback;

    if (const ma_result result = ma_sound_init_ex(engine, &soundConfig, &sound); result != MA_SUCCESS)
    {
        err() << "Failed to initialize sound: " << ma_result_description(result) << std::endl;
        return;
    }

    // Initialize the custom effect node
    effectNodeVTable.onProcess =
        [](ma_node* node, const float** framesIn, std::uint32_t* frameCountIn, float** framesOut, std::uint32_t* frameCountOut)
    { static_cast<EffectNode*>(node)->impl->processEffect(framesIn, *frameCountIn, framesOut, *frameCountOut); };
    effectNodeVTable.onGetRequiredInputFrameCount = nullptr;
    effectNodeVTable.inputBusCount                = 1;
    effectNodeVTable.outputBusCount               = 1;
    effectNodeVTable.flags                        = MA_NODE_FLAG_CONTINUOUS_PROCESSING | MA_NODE_FLAG_ALLOW_NULL_INPUT;

    const auto     nodeChannelCount = ma_engine_get_channels(engine);
    ma_node_config nodeConfig       = ma_node_config_init();
    nodeConfig.vtable               = &effectNodeVTable;
    nodeConfig.pInputChannels       = &nodeChannelCount;
    nodeConfig.pOutputChannels      = &nodeChannelCount;

    if (const ma_result result = ma_node_init(ma_engine_get_node_graph(engine), &nodeConfig, nullptr, &effectNode);
        result != MA_SUCCESS)
    {
        err() << "Failed to initialize effect node: " << ma_result_description(result) << std::endl;
        return;
    }

    effectNode.impl         = this;
    effectNode.channelCount = nodeChannelCount;

    // Route the sound through the effect node depending on whether an effect processor is set
    connectEffect(bool{effectProcessor});

    applySettings(sound, savedSettings);
}


////////////////////////////////////////////////////////////
void MiniaudioUtils::SoundBase::deinitialize()
{
    savedSettings = saveSettings(sound);
    ma_sound_uninit(&sound);
    ma_node_uninit(&effectNode, nullptr);
}


////////////////////////////////////////////////////////////
void MiniaudioUtils::SoundBase::processEffect(const float**  framesIn,
                                              std::uint32_t& frameCountIn,
                                              float**        framesOut,
                                              std::uint32_t& frameCountOut) const
{
    // If a processor is set, call it
    if (effectProcessor)
    {
        if (!framesIn)
            frameCountIn = 0;

        effectProcessor(framesIn ? framesIn[0] : nullptr, frameCountIn, framesOut[0], frameCountOut, effectNode.channelCount);
        return;
    }

    // Otherwise just pass the data through 1:1
    if (framesIn == nullptr)
    {
        frameCountIn  = 0;
        frameCountOut = 0;
        return;
    }

    const auto toProcess = std::min(frameCountIn, frameCountOut);
    std::memcpy(framesOut[0], framesIn[0], toProcess * effectNode.channelCount * sizeof(float));
    frameCountIn  = toProcess;
    frameCountOut = toProcess;
}


////////////////////////////////////////////////////////////
void MiniaudioUtils::SoundBase::connectEffect(bool connect)
{
    auto* engine = AudioDevice::getEngine();

    if (engine == nullptr)
    {
        err() << "Failed to connect effect: No engine available" << std::endl;
        return;
    }

    if (connect)
    {
        // Attach the custom effect node output to our engine endpoint
        if (const ma_result result = ma_node_attach_output_bus(&effectNode, 0, ma_engine_get_endpoint(engine), 0);
            result != MA_SUCCESS)
        {
            err() << "Failed to attach effect node output to endpoint: " << ma_result_description(result) << std::endl;
            return;
        }
    }
    else
    {
        // Detach the custom effect node output from our engine endpoint
        if (const ma_result result = ma_node_detach_output_bus(&effectNode, 0); result != MA_SUCCESS)
        {
            err() << "Failed to detach effect node output from endpoint: " << ma_result_description(result) << std::endl;
            return;
        }
    }

    // Attach the sound output to the custom effect node or the engine endpoint
    if (const ma_result result = ma_node_attach_output_bus(&sound, 0, connect ? &effectNode : ma_engine_get_endpoint(engine), 0);
        result != MA_SUCCESS)
    {
        err() << "Failed to attach sound node output to effect node: " << ma_result_description(result) << std::endl;
        return;
    }
}


////////////////////////////////////////////////////////////
ma_channel MiniaudioUtils::soundChannelToMiniaudioChannel(SoundChannel soundChannel)
{
    switch (soundChannel)
    {
        case SoundChannel::Unspecified:
            return MA_CHANNEL_NONE;
        case SoundChannel::Mono:
            return MA_CHANNEL_MONO;
        case SoundChannel::FrontLeft:
            return MA_CHANNEL_FRONT_LEFT;
        case SoundChannel::FrontRight:
            return MA_CHANNEL_FRONT_RIGHT;
        case SoundChannel::FrontCenter:
            return MA_CHANNEL_FRONT_CENTER;
        case SoundChannel::FrontLeftOfCenter:
            return MA_CHANNEL_FRONT_LEFT_CENTER;
        case SoundChannel::FrontRightOfCenter:
            return MA_CHANNEL_FRONT_RIGHT_CENTER;
        case SoundChannel::LowFrequencyEffects:
            return MA_CHANNEL_LFE;
        case SoundChannel::BackLeft:
            return MA_CHANNEL_BACK_LEFT;
        case SoundChannel::BackRight:
            return MA_CHANNEL_BACK_RIGHT;
        case SoundChannel::BackCenter:
            return MA_CHANNEL_BACK_CENTER;
        case SoundChannel::SideLeft:
            return MA_CHANNEL_SIDE_LEFT;
        case SoundChannel::SideRight:
            return MA_CHANNEL_SIDE_RIGHT;
        case SoundChannel::TopCenter:
            return MA_CHANNEL_TOP_CENTER;
        case SoundChannel::TopFrontLeft:
            return MA_CHANNEL_TOP_FRONT_LEFT;
        case SoundChannel::TopFrontRight:
            return MA_CHANNEL_TOP_FRONT_RIGHT;
        case SoundChannel::TopFrontCenter:
            return MA_CHANNEL_TOP_FRONT_CENTER;
        case SoundChannel::TopBackLeft:
            return MA_CHANNEL_TOP_BACK_LEFT;
        case SoundChannel::TopBackRight:
            return MA_CHANNEL_TOP_BACK_RIGHT;
        default:
            assert(soundChannel == SoundChannel::TopBackCenter);
            return MA_CHANNEL_TOP_BACK_CENTER;
    }
}


////////////////////////////////////////////////////////////
SoundChannel MiniaudioUtils::miniaudioChannelToSoundChannel(ma_channel soundChannel)
{
    switch (soundChannel)
    {
        case MA_CHANNEL_NONE:
            return SoundChannel::Unspecified;
        case MA_CHANNEL_MONO:
            return SoundChannel::Mono;
        case MA_CHANNEL_FRONT_LEFT:
            return SoundChannel::FrontLeft;
        case MA_CHANNEL_FRONT_RIGHT:
            return SoundChannel::FrontRight;
        case MA_CHANNEL_FRONT_CENTER:
            return SoundChannel::FrontCenter;
        case MA_CHANNEL_FRONT_LEFT_CENTER:
            return SoundChannel::FrontLeftOfCenter;
        case MA_CHANNEL_FRONT_RIGHT_CENTER:
            return SoundChannel::FrontRightOfCenter;
        case MA_CHANNEL_LFE:
            return SoundChannel::LowFrequencyEffects;
        case MA_CHANNEL_BACK_LEFT:
            return SoundChannel::BackLeft;
        case MA_CHANNEL_BACK_RIGHT:
            return SoundChannel::BackRight;
        case MA_CHANNEL_BACK_CENTER:
            return SoundChannel::BackCenter;
        case MA_CHANNEL_SIDE_LEFT:
            return SoundChannel::SideLeft;
        case MA_CHANNEL_SIDE_RIGHT:
            return SoundChannel::SideRight;
        case MA_CHANNEL_TOP_CENTER:
            return SoundChannel::TopCenter;
        case MA_CHANNEL_TOP_FRONT_LEFT:
            return SoundChannel::TopFrontLeft;
        case MA_CHANNEL_TOP_FRONT_RIGHT:
            return SoundChannel::TopFrontRight;
        case MA_CHANNEL_TOP_FRONT_CENTER:
            return SoundChannel::TopFrontCenter;
        case MA_CHANNEL_TOP_BACK_LEFT:
            return SoundChannel::TopBackLeft;
        case MA_CHANNEL_TOP_BACK_RIGHT:
            return SoundChannel::TopBackRight;
        default:
            assert(soundChannel == MA_CHANNEL_TOP_BACK_CENTER);
            return SoundChannel::TopBackCenter;
    }
}


////////////////////////////////////////////////////////////
Time MiniaudioUtils::getPlayingOffset(ma_sound& sound)
{
    float cursor = 0.f;

    if (const ma_result result = ma_sound_get_cursor_in_seconds(&sound, &cursor); result != MA_SUCCESS)
    {
        err() << "Failed to get sound cursor: " << ma_result_description(result) << std::endl;
        return {};
    }

    return seconds(cursor);
}


////////////////////////////////////////////////////////////
std::uint64_t MiniaudioUtils::getFrameIndex(ma_sound& sound, Time timeOffset)
{
    std::uint32_t sampleRate{};

    if (const ma_result result = ma_sound_get_data_format(&sound, nullptr, nullptr, &sampleRate, nullptr, 0);
        result != MA_SUCCESS)
        err() << "Failed to get sound data format: " << ma_result_description(result) << std::endl;

    const auto frameIndex = static_cast<std::uint64_t>(timeOffset.asSeconds() * static_cast<float>(sampleRate));

    if (const ma_result result = ma_sound_seek_to_pcm_frame(&sound, frameIndex); result != MA_SUCCESS)
        err() << "Failed to seek sound to pcm frame: " << ma_result_description(result) << std::endl;

    return frameIndex;
}

} // namespace sf::priv
