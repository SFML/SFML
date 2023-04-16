////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Audio/SoundStream.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Sleep.hpp>

#include <miniaudio.h>

#include <algorithm>
#include <limits>
#include <ostream>
#include <vector>

#include <cassert>


namespace sf
{
struct SoundStream::Impl
{
    Impl(SoundStream* owner) : m_owner(owner)
    {
        // Set this object up as a miniaudio data source
        ma_data_source_config config = ma_data_source_config_init();

        static ma_data_source_vtable vtable{read, seek, getFormat, getCursor, getLength, setLooping, 0};

        config.vtable = &vtable;

        if (auto result = ma_data_source_init(&config, &m_dataSourceBase); result != MA_SUCCESS)
            err() << "Failed to initialize audio data source: " << ma_result_description(result) << std::endl;

        // Initialize sound structure and set default settings
        initialize();

        ma_sound_set_pitch(&m_sound, 1.f);
        ma_sound_set_pan(&m_sound, 0.f);
        ma_sound_set_volume(&m_sound, 1.f);
        ma_sound_set_spatialization_enabled(&m_sound, MA_TRUE);
        ma_sound_set_position(&m_sound, 0.f, 0.f, 0.f);
        ma_sound_set_direction(&m_sound, 0.f, 0.f, -1.f);
        ma_sound_set_cone(&m_sound, sf::degrees(360).asRadians(), sf::degrees(360).asRadians(), 0.f); // inner = 360 degrees, outer = 360 degrees, gain = 0
        ma_sound_set_directional_attenuation_factor(&m_sound, 1.f);
        ma_sound_set_velocity(&m_sound, 0.f, 0.f, 0.f);
        ma_sound_set_doppler_factor(&m_sound, 1.f);
        ma_sound_set_positioning(&m_sound, ma_positioning_absolute);
        ma_sound_set_min_distance(&m_sound, 1.f);
        ma_sound_set_max_distance(&m_sound, std::numeric_limits<float>::max());
        ma_sound_set_min_gain(&m_sound, 0.f);
        ma_sound_set_max_gain(&m_sound, 1.f);
        ma_sound_set_rolloff(&m_sound, 1.f);
    }

    ~Impl()
    {
        ma_sound_uninit(&m_sound);

        ma_data_source_uninit(&m_dataSourceBase);
    }

    void initialize()
    {
        // Initialize the sound
        auto* engine = static_cast<ma_engine*>(priv::AudioDevice::getEngine());

        assert(engine != nullptr);

        ma_sound_config soundConfig;

        soundConfig                      = ma_sound_config_init();
        soundConfig.pDataSource          = this;
        soundConfig.pEndCallbackUserData = this;
        soundConfig.endCallback          = [](void* userData, ma_sound* sound)
        {
            // Seek back to the start of the sound when it finishes playing
            auto& impl       = *static_cast<Impl*>(userData);
            impl.m_streaming = true;

            if (auto result = ma_sound_seek_to_pcm_frame(sound, 0); result != MA_SUCCESS)
                err() << "Failed to seek sound to frame 0: " << ma_result_description(result) << std::endl;
        };

        if (auto result = ma_sound_init_ex(engine, &soundConfig, &m_sound); result != MA_SUCCESS)
            err() << "Failed to initialize sound: " << ma_result_description(result) << std::endl;

        // Because we are providing a custom data source, we have to provide the channel map ourselves
        if (!m_channelMap.empty())
        {
            m_soundChannelMap.clear();

            for (auto channel : m_channelMap)
            {
                switch (channel)
                {
                    case SoundChannel::Unspecified:
                        m_soundChannelMap.push_back(MA_CHANNEL_NONE);
                        break;
                    case SoundChannel::Mono:
                        m_soundChannelMap.push_back(MA_CHANNEL_MONO);
                        break;
                    case SoundChannel::FrontLeft:
                        m_soundChannelMap.push_back(MA_CHANNEL_FRONT_LEFT);
                        break;
                    case SoundChannel::FrontRight:
                        m_soundChannelMap.push_back(MA_CHANNEL_FRONT_RIGHT);
                        break;
                    case SoundChannel::FrontCenter:
                        m_soundChannelMap.push_back(MA_CHANNEL_FRONT_CENTER);
                        break;
                    case SoundChannel::FrontLeftOfCenter:
                        m_soundChannelMap.push_back(MA_CHANNEL_FRONT_LEFT_CENTER);
                        break;
                    case SoundChannel::FrontRightOfCenter:
                        m_soundChannelMap.push_back(MA_CHANNEL_FRONT_RIGHT_CENTER);
                        break;
                    case SoundChannel::LowFrequencyEffects:
                        m_soundChannelMap.push_back(MA_CHANNEL_LFE);
                        break;
                    case SoundChannel::BackLeft:
                        m_soundChannelMap.push_back(MA_CHANNEL_BACK_LEFT);
                        break;
                    case SoundChannel::BackRight:
                        m_soundChannelMap.push_back(MA_CHANNEL_BACK_RIGHT);
                        break;
                    case SoundChannel::BackCenter:
                        m_soundChannelMap.push_back(MA_CHANNEL_BACK_CENTER);
                        break;
                    case SoundChannel::SideLeft:
                        m_soundChannelMap.push_back(MA_CHANNEL_SIDE_LEFT);
                        break;
                    case SoundChannel::SideRight:
                        m_soundChannelMap.push_back(MA_CHANNEL_SIDE_RIGHT);
                        break;
                    case SoundChannel::TopCenter:
                        m_soundChannelMap.push_back(MA_CHANNEL_TOP_CENTER);
                        break;
                    case SoundChannel::TopFrontLeft:
                        m_soundChannelMap.push_back(MA_CHANNEL_TOP_FRONT_LEFT);
                        break;
                    case SoundChannel::TopFrontRight:
                        m_soundChannelMap.push_back(MA_CHANNEL_TOP_FRONT_RIGHT);
                        break;
                    case SoundChannel::TopFrontCenter:
                        m_soundChannelMap.push_back(MA_CHANNEL_TOP_FRONT_CENTER);
                        break;
                    case SoundChannel::TopBackLeft:
                        m_soundChannelMap.push_back(MA_CHANNEL_TOP_BACK_LEFT);
                        break;
                    case SoundChannel::TopBackRight:
                        m_soundChannelMap.push_back(MA_CHANNEL_TOP_BACK_RIGHT);
                        break;
                    case SoundChannel::TopBackCenter:
                        m_soundChannelMap.push_back(MA_CHANNEL_TOP_BACK_CENTER);
                        break;
                    default:
                        break;
                }
            }

            m_sound.engineNode.spatializer.pChannelMapIn = m_soundChannelMap.data();
        }
        else
        {
            m_sound.engineNode.spatializer.pChannelMapIn = nullptr;
        }
    }

    void reinitialize()
    {
        // Save and re-apply settings
        auto pitch                        = ma_sound_get_pitch(&m_sound);
        auto pan                          = ma_sound_get_pan(&m_sound);
        auto volume                       = ma_sound_get_volume(&m_sound);
        auto spatializationEnabled        = ma_sound_is_spatialization_enabled(&m_sound);
        auto position                     = ma_sound_get_position(&m_sound);
        auto direction                    = ma_sound_get_direction(&m_sound);
        auto directionalAttenuationFactor = ma_sound_get_directional_attenuation_factor(&m_sound);
        auto velocity                     = ma_sound_get_velocity(&m_sound);
        auto dopplerFactor                = ma_sound_get_doppler_factor(&m_sound);
        auto positioning                  = ma_sound_get_positioning(&m_sound);
        auto minDistance                  = ma_sound_get_min_distance(&m_sound);
        auto maxDistance                  = ma_sound_get_max_distance(&m_sound);
        auto minGain                      = ma_sound_get_min_gain(&m_sound);
        auto maxGain                      = ma_sound_get_max_gain(&m_sound);
        auto rollOff                      = ma_sound_get_rolloff(&m_sound);

        float innerAngle;
        float outerAngle;
        float outerGain;
        ma_sound_get_cone(&m_sound, &innerAngle, &outerAngle, &outerGain);

        ma_sound_uninit(&m_sound);

        initialize();

        ma_sound_set_pitch(&m_sound, pitch);
        ma_sound_set_pan(&m_sound, pan);
        ma_sound_set_volume(&m_sound, volume);
        ma_sound_set_spatialization_enabled(&m_sound, spatializationEnabled);
        ma_sound_set_position(&m_sound, position.x, position.y, position.z);
        ma_sound_set_direction(&m_sound, direction.x, direction.y, direction.z);
        ma_sound_set_directional_attenuation_factor(&m_sound, directionalAttenuationFactor);
        ma_sound_set_velocity(&m_sound, velocity.x, velocity.y, velocity.z);
        ma_sound_set_doppler_factor(&m_sound, dopplerFactor);
        ma_sound_set_positioning(&m_sound, positioning);
        ma_sound_set_min_distance(&m_sound, minDistance);
        ma_sound_set_max_distance(&m_sound, maxDistance);
        ma_sound_set_min_gain(&m_sound, minGain);
        ma_sound_set_max_gain(&m_sound, maxGain);
        ma_sound_set_rolloff(&m_sound, rollOff);

        ma_sound_set_cone(&m_sound, innerAngle, outerAngle, outerGain);
    }

    static ma_result read(ma_data_source* dataSource, void* framesOut, ma_uint64 frameCount, ma_uint64* framesRead)
    {
        auto& impl  = *static_cast<Impl*>(dataSource);
        auto* owner = impl.m_owner;

        // Try to fill our buffer with new samples if the source is still willing to stream data
        if (impl.m_sampleBuffer.empty() && impl.m_streaming)
        {
            Chunk chunk;

            impl.m_streaming = owner->onGetData(chunk);

            if (chunk.samples && chunk.sampleCount)
            {
                impl.m_sampleBuffer.assign(chunk.samples, chunk.samples + chunk.sampleCount);
                impl.m_sampleBufferCursor = 0;
            }
        }

        // Push the samples to miniaudio
        if (!impl.m_sampleBuffer.empty())
        {
            // Determine how many frames we can read
            *framesRead = std::min(frameCount,
                                   (impl.m_sampleBuffer.size() - impl.m_sampleBufferCursor) / impl.m_channelCount);

            const auto sampleCount = *framesRead * impl.m_channelCount;

            // Copy the samples to the output
            std::memcpy(framesOut,
                        impl.m_sampleBuffer.data() + impl.m_sampleBufferCursor,
                        sampleCount * sizeof(impl.m_sampleBuffer[0]));

            impl.m_sampleBufferCursor += sampleCount;
            impl.m_samplesProcessed += sampleCount;

            if (impl.m_sampleBufferCursor >= impl.m_sampleBuffer.size())
            {
                impl.m_sampleBuffer.clear();
                impl.m_sampleBufferCursor = 0;

                // If we are looping and at the end of the loop, set the cursor back to the beginning of the loop
                if (!impl.m_streaming && impl.m_loop)
                {
                    if (auto seekPositionAfterLoop = owner->onLoop(); seekPositionAfterLoop != NoLoop)
                    {
                        impl.m_streaming        = true;
                        impl.m_samplesProcessed = seekPositionAfterLoop;
                    }
                }
            }
        }
        else
        {
            *framesRead = 0;
        }

        return MA_SUCCESS;
    }

    static ma_result seek(ma_data_source* dataSource, ma_uint64 frameIndex)
    {
        auto& impl  = *static_cast<Impl*>(dataSource);
        auto* owner = impl.m_owner;

        impl.m_streaming = true;
        impl.m_sampleBuffer.clear();
        impl.m_sampleBufferCursor = 0;
        impl.m_samplesProcessed = frameIndex * impl.m_channelCount;

        if (impl.m_sampleRate != 0)
        {
            owner->onSeek(seconds(static_cast<float>(frameIndex / impl.m_sampleRate)));
        }
        else
        {
            owner->onSeek(Time::Zero);
        }

        return MA_SUCCESS;
    }

    static ma_result getFormat(ma_data_source* dataSource,
                               ma_format*      format,
                               ma_uint32*      channels,
                               ma_uint32*      sampleRate,
                               ma_channel*,
                               size_t)
    {
        const auto& impl = *static_cast<const Impl*>(dataSource);

        // If we don't have valid values yet, initialize with defaults so sound creation doesn't fail
        *format     = ma_format_s16;
        *channels   = impl.m_channelCount ? impl.m_channelCount : 1;
        *sampleRate = impl.m_sampleRate ? impl.m_sampleRate : 44100;

        return MA_SUCCESS;
    }

    static ma_result getCursor(ma_data_source* dataSource, ma_uint64* cursor)
    {
        auto& impl = *static_cast<Impl*>(dataSource);
        *cursor    = impl.m_channelCount ? impl.m_samplesProcessed / impl.m_channelCount : 0;

        return MA_SUCCESS;
    }

    static ma_result getLength(ma_data_source*, ma_uint64* length)
    {
        *length = 0;

        return MA_NOT_IMPLEMENTED;
    }

    static ma_result setLooping(ma_data_source* dataSource, ma_bool32 looping)
    {
        static_cast<Impl*>(dataSource)->m_loop = (looping == MA_TRUE);

        return MA_SUCCESS;
    }

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    ma_data_source_base       m_dataSourceBase{};     //!< The struct that makes this object a miniaudio data source (must be first member)
    SoundStream* const        m_owner;                //!< Owning SoundStream object
    std::vector<ma_channel>   m_soundChannelMap;      //!< The map of position in sample frame to sound channel (miniaudio channels)
    ma_sound                  m_sound{};              //!< The sound
    std::vector<std::int16_t> m_sampleBuffer;         //!< Our temporary sample buffer
    std::size_t               m_sampleBufferCursor{}; //!< The current read position in the temporary sample buffer
    std::uint64_t             m_samplesProcessed{};   //!< Number of samples processed since beginning of the stream
    unsigned int              m_channelCount{};       //!< Number of channels (1 = mono, 2 = stereo, ...)
    unsigned int              m_sampleRate{};         //!< Frequency (samples / second)
    std::vector<SoundChannel> m_channelMap{};         //!< The map of position in sample frame to sound channel
    bool                      m_loop{};               //!< Loop flag (true to loop, false to play once)
    bool                      m_streaming{true};      //!< True if we are still streaming samples from the source
};


////////////////////////////////////////////////////////////
SoundStream::SoundStream() : m_impl(std::make_unique<Impl>(this))
{
}


////////////////////////////////////////////////////////////
SoundStream::~SoundStream() = default;


////////////////////////////////////////////////////////////
void SoundStream::initialize(unsigned int channelCount, unsigned int sampleRate, const std::vector<SoundChannel>& channelMap)
{
    m_impl->m_channelCount     = channelCount;
    m_impl->m_sampleRate       = sampleRate;
    m_impl->m_channelMap       = channelMap;
    m_impl->m_samplesProcessed = 0;

    m_impl->reinitialize();
}


////////////////////////////////////////////////////////////
void SoundStream::play()
{
    if (auto result = ma_sound_start(&m_impl->m_sound); result != MA_SUCCESS)
        err() << "Failed to start playing sound: " << ma_result_description(result) << std::endl;
}


////////////////////////////////////////////////////////////
void SoundStream::pause()
{
    if (auto result = ma_sound_stop(&m_impl->m_sound); result != MA_SUCCESS)
        err() << "Failed to stop playing sound: " << ma_result_description(result) << std::endl;
}


////////////////////////////////////////////////////////////
void SoundStream::stop()
{
    if (auto result = ma_sound_stop(&m_impl->m_sound); result != MA_SUCCESS)
    {
        err() << "Failed to stop playing sound: " << ma_result_description(result) << std::endl;
    }
    else
    {
        setPlayingOffset(Time::Zero);
    }
}


////////////////////////////////////////////////////////////
unsigned int SoundStream::getChannelCount() const
{
    return m_impl->m_channelCount;
}


////////////////////////////////////////////////////////////
unsigned int SoundStream::getSampleRate() const
{
    return m_impl->m_sampleRate;
}


////////////////////////////////////////////////////////////
std::vector<SoundChannel> SoundStream::getChannelMap() const
{
    return m_impl->m_channelMap;
}


////////////////////////////////////////////////////////////
SoundStream::Status SoundStream::getStatus() const
{
    return SoundSource::getStatus();
}


////////////////////////////////////////////////////////////
void SoundStream::setPlayingOffset(Time timeOffset)
{
    if (m_impl->m_sampleRate == 0)
        return;

    ma_uint32 sampleRate{};

    if (auto result = ma_sound_get_data_format(&m_impl->m_sound, nullptr, nullptr, &sampleRate, nullptr, 0);
        result != MA_SUCCESS)
        err() << "Failed to get sound data format: " << ma_result_description(result) << std::endl;

    const auto frameIndex = static_cast<ma_uint64>(timeOffset.asSeconds() * static_cast<double>(sampleRate));

    if (auto result = ma_sound_seek_to_pcm_frame(&m_impl->m_sound, frameIndex); result != MA_SUCCESS)
        err() << "Failed to seek sound to pcm frame: " << ma_result_description(result) << std::endl;

    m_impl->m_streaming = true;
    m_impl->m_sampleBuffer.clear();
    m_impl->m_sampleBufferCursor = 0;
    m_impl->m_samplesProcessed   = frameIndex * m_impl->m_channelCount;

    onSeek(seconds(static_cast<float>(frameIndex / m_impl->m_sampleRate)));
}


////////////////////////////////////////////////////////////
Time SoundStream::getPlayingOffset() const
{
    if (m_impl->m_channelCount == 0 || m_impl->m_sampleRate == 0)
        return Time();

    auto cursor = 0.f;

    if (auto result = ma_sound_get_cursor_in_seconds(&m_impl->m_sound, &cursor); result != MA_SUCCESS)
    {
        err() << "Failed to get sound cursor: " << ma_result_description(result) << std::endl;
        return Time();
    }

    return seconds(cursor);
}


////////////////////////////////////////////////////////////
void SoundStream::setLoop(bool loop)
{
    ma_sound_set_looping(&m_impl->m_sound, loop ? MA_TRUE : MA_FALSE);
}


////////////////////////////////////////////////////////////
bool SoundStream::getLoop() const
{
    return ma_sound_is_looping(&m_impl->m_sound) == MA_TRUE;
}


////////////////////////////////////////////////////////////
std::int64_t SoundStream::onLoop()
{
    onSeek(Time::Zero);
    return 0;
}


////////////////////////////////////////////////////////////
void* SoundStream::getSound() const
{
    return &m_impl->m_sound;
}

} // namespace sf
