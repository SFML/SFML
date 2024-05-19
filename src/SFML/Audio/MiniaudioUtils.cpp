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
#include <SFML/Audio/MiniaudioUtils.hpp>
#include <SFML/Audio/SoundChannel.hpp>

#include <SFML/System/Angle.hpp>
#include <SFML/System/Err.hpp>
#include <SFML/System/Time.hpp>

#include <miniaudio.h>

#include <functional>
#include <limits>
#include <ostream>

#include <cassert>


namespace sf::priv
{
namespace
{
////////////////////////////////////////////////////////////
struct SavedSettings
{
    float          pitch{1.f};
    float          pan{0.f};
    float          volume{1.f};
    ma_bool32      spatializationEnabled{MA_TRUE};
    ma_vec3f       position{0.f, 0.f, 0.f};
    ma_vec3f       direction{0.f, 0.f, -1.f};
    float          directionalAttenuationFactor{1.f};
    ma_vec3f       velocity{0.f, 0.f, 0.f};
    float          dopplerFactor{1.f};
    ma_positioning positioning{ma_positioning_absolute};
    float          minDistance{1.f};
    float          maxDistance{std::numeric_limits<float>::max()};
    float          minGain{0.f};
    float          maxGain{1.f};
    float          rollOff{1.f};
    float          innerAngle{degrees(360.f).asRadians()};
    float          outerAngle{degrees(360.f).asRadians()};
    float          outerGain{0.f};
};


////////////////////////////////////////////////////////////
SavedSettings saveSettings(const ma_sound& sound)
{
    float innerAngle = 0;
    float outerAngle = 0;
    float outerGain  = 0;
    ma_sound_get_cone(&sound, &innerAngle, &outerAngle, &outerGain);

    return SavedSettings{ma_sound_get_pitch(&sound),
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
                         innerAngle,
                         outerAngle,
                         outerGain};
}


////////////////////////////////////////////////////////////
void applySettings(ma_sound& sound, const SavedSettings& savedSettings)
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

    ma_sound_set_cone(&sound, savedSettings.innerAngle, savedSettings.outerAngle, savedSettings.outerGain);
}


////////////////////////////////////////////////////////////
void initializeDataSource(ma_data_source_base& dataSourceBase, const ma_data_source_vtable& vtable)
{
    // Set this object up as a miniaudio data source
    ma_data_source_config config = ma_data_source_config_init();
    config.vtable                = &vtable;

    if (const ma_result result = ma_data_source_init(&config, &dataSourceBase); result != MA_SUCCESS)
        err() << "Failed to initialize audio data source: " << ma_result_description(result) << std::endl;
}
} // namespace


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
ma_uint64 MiniaudioUtils::getFrameIndex(ma_sound& sound, Time timeOffset)
{
    ma_uint32 sampleRate{};

    if (const ma_result result = ma_sound_get_data_format(&sound, nullptr, nullptr, &sampleRate, nullptr, 0);
        result != MA_SUCCESS)
        err() << "Failed to get sound data format: " << ma_result_description(result) << std::endl;

    const auto frameIndex = static_cast<ma_uint64>(timeOffset.asSeconds() * static_cast<float>(sampleRate));

    if (const ma_result result = ma_sound_seek_to_pcm_frame(&sound, frameIndex); result != MA_SUCCESS)
        err() << "Failed to seek sound to pcm frame: " << ma_result_description(result) << std::endl;

    return frameIndex;
}


////////////////////////////////////////////////////////////
void MiniaudioUtils::reinitializeSound(ma_sound& sound, const std::function<void()>& initializeFn)
{
    const SavedSettings savedSettings = saveSettings(sound);
    ma_sound_uninit(&sound);

    initializeFn();

    applySettings(sound, savedSettings);
}


////////////////////////////////////////////////////////////
void MiniaudioUtils::initializeSound(const ma_data_source_vtable& vtable,
                                     ma_data_source_base&         dataSourceBase,
                                     ma_sound&                    sound,
                                     const std::function<void()>& initializeFn)
{
    initializeDataSource(dataSourceBase, vtable);

    // Initialize sound structure and set default settings
    initializeFn();
    applySettings(sound, SavedSettings{});
}

} // namespace sf::priv
