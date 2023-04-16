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
#include <SFML/Audio/Sound.hpp>

#include <SFML/Audio/AudioDevice.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/Err.hpp>

#include <algorithm>
#include <cassert>
#include <limits>
#include <miniaudio.h>
#include <ostream>


namespace sf
{
struct Sound::Impl
{
    Impl()
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

        soundConfig             = ma_sound_config_init();
        soundConfig.pDataSource = this;
        soundConfig.endCallback = [](void*, ma_sound* sound)
        {
            // Seek back to the start of the sound when it finishes playing
            if (auto result = ma_sound_seek_to_pcm_frame(sound, 0); result != MA_SUCCESS)
                err() << "Failed to seek sound to frame 0: " << ma_result_description(result) << std::endl;
        };

        if (auto result = ma_sound_init_ex(engine, &soundConfig, &m_sound); result != MA_SUCCESS)
            err() << "Failed to initialize sound: " << ma_result_description(result) << std::endl;

        // Because we are providing a custom data source, we have to provide the channel map ourselves
        if (m_buffer && !m_buffer->getChannelMap().empty())
        {
            m_channelMap.clear();

            for (auto channel : m_buffer->getChannelMap())
            {
                switch (channel)
                {
                    case SoundChannel::Unspecified:
                        m_channelMap.push_back(MA_CHANNEL_NONE);
                        break;
                    case SoundChannel::Mono:
                        m_channelMap.push_back(MA_CHANNEL_MONO);
                        break;
                    case SoundChannel::FrontLeft:
                        m_channelMap.push_back(MA_CHANNEL_FRONT_LEFT);
                        break;
                    case SoundChannel::FrontRight:
                        m_channelMap.push_back(MA_CHANNEL_FRONT_RIGHT);
                        break;
                    case SoundChannel::FrontCenter:
                        m_channelMap.push_back(MA_CHANNEL_FRONT_CENTER);
                        break;
                    case SoundChannel::FrontLeftOfCenter:
                        m_channelMap.push_back(MA_CHANNEL_FRONT_LEFT_CENTER);
                        break;
                    case SoundChannel::FrontRightOfCenter:
                        m_channelMap.push_back(MA_CHANNEL_FRONT_RIGHT_CENTER);
                        break;
                    case SoundChannel::LowFrequencyEffects:
                        m_channelMap.push_back(MA_CHANNEL_LFE);
                        break;
                    case SoundChannel::BackLeft:
                        m_channelMap.push_back(MA_CHANNEL_BACK_LEFT);
                        break;
                    case SoundChannel::BackRight:
                        m_channelMap.push_back(MA_CHANNEL_BACK_RIGHT);
                        break;
                    case SoundChannel::BackCenter:
                        m_channelMap.push_back(MA_CHANNEL_BACK_CENTER);
                        break;
                    case SoundChannel::SideLeft:
                        m_channelMap.push_back(MA_CHANNEL_SIDE_LEFT);
                        break;
                    case SoundChannel::SideRight:
                        m_channelMap.push_back(MA_CHANNEL_SIDE_RIGHT);
                        break;
                    case SoundChannel::TopCenter:
                        m_channelMap.push_back(MA_CHANNEL_TOP_CENTER);
                        break;
                    case SoundChannel::TopFrontLeft:
                        m_channelMap.push_back(MA_CHANNEL_TOP_FRONT_LEFT);
                        break;
                    case SoundChannel::TopFrontRight:
                        m_channelMap.push_back(MA_CHANNEL_TOP_FRONT_RIGHT);
                        break;
                    case SoundChannel::TopFrontCenter:
                        m_channelMap.push_back(MA_CHANNEL_TOP_FRONT_CENTER);
                        break;
                    case SoundChannel::TopBackLeft:
                        m_channelMap.push_back(MA_CHANNEL_TOP_BACK_LEFT);
                        break;
                    case SoundChannel::TopBackRight:
                        m_channelMap.push_back(MA_CHANNEL_TOP_BACK_RIGHT);
                        break;
                    case SoundChannel::TopBackCenter:
                        m_channelMap.push_back(MA_CHANNEL_TOP_BACK_CENTER);
                        break;
                    default:
                        break;
                }
            }

            m_sound.engineNode.spatializer.pChannelMapIn = m_channelMap.data();
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
        auto&       impl   = *static_cast<Impl*>(dataSource);
        const auto* buffer = impl.m_buffer;

        if (buffer == nullptr)
            return MA_NO_DATA_AVAILABLE;

        // Determine how many frames we can read
        *framesRead = std::min(frameCount, (buffer->getSampleCount() - impl.m_cursor) / buffer->getChannelCount());

        // Copy the samples to the output
        const auto sampleCount = *framesRead * buffer->getChannelCount();

        std::memcpy(framesOut, buffer->getSamples() + impl.m_cursor, sampleCount * sizeof(buffer->getSamples()[0]));

        impl.m_cursor += sampleCount;

        // If we are looping and at the end of the sound, set the cursor back to the start
        if (impl.m_looping && (impl.m_cursor >= buffer->getSampleCount()))
            impl.m_cursor = 0;

        return MA_SUCCESS;
    }

    static ma_result seek(ma_data_source* dataSource, ma_uint64 frameIndex)
    {
        auto&       impl   = *static_cast<Impl*>(dataSource);
        const auto* buffer = impl.m_buffer;

        if (buffer == nullptr)
            return MA_NO_DATA_AVAILABLE;

        impl.m_cursor = frameIndex * buffer->getChannelCount();

        return MA_SUCCESS;
    }

    static ma_result getFormat(ma_data_source* dataSource,
                               ma_format*      format,
                               ma_uint32*      channels,
                               ma_uint32*      sampleRate,
                               ma_channel*,
                               size_t)
    {
        const auto& impl   = *static_cast<const Impl*>(dataSource);
        const auto* buffer = impl.m_buffer;

        // If we don't have valid values yet, initialize with defaults so sound creation doesn't fail
        *format     = ma_format_s16;
        *channels   = buffer && buffer->getChannelCount() ? buffer->getChannelCount() : 1;
        *sampleRate = buffer && buffer->getSampleRate() ? buffer->getSampleRate() : 44100;

        return MA_SUCCESS;
    }

    static ma_result getCursor(ma_data_source* dataSource, ma_uint64* cursor)
    {
        const auto& impl   = *static_cast<const Impl*>(dataSource);
        const auto* buffer = impl.m_buffer;

        if (buffer == nullptr)
            return MA_NO_DATA_AVAILABLE;

        *cursor = impl.m_cursor / buffer->getChannelCount();

        return MA_SUCCESS;
    }

    static ma_result getLength(ma_data_source* dataSource, ma_uint64* length)
    {
        const auto& impl   = *static_cast<const Impl*>(dataSource);
        const auto* buffer = impl.m_buffer;

        if (buffer == nullptr)
            return MA_NO_DATA_AVAILABLE;

        *length = buffer->getSampleCount() / buffer->getChannelCount();

        return MA_SUCCESS;
    }

    static ma_result setLooping(ma_data_source* dataSource, ma_bool32 looping)
    {
        auto& impl = *static_cast<Impl*>(dataSource);

        impl.m_looping = (looping == MA_TRUE);

        return MA_SUCCESS;
    }

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    ma_data_source_base     m_dataSourceBase{}; //!< The struct that makes this object a miniaudio data source (must be first member)
    std::vector<ma_channel> m_channelMap;       //!< The map of position in sample frame to sound channel (miniaudio channels)
    ma_sound                m_sound{};          //!< The sound
    std::size_t             m_cursor{};         //!< The current playing position
    bool                    m_looping{};        //!< True if we are looping the sound
    const SoundBuffer*      m_buffer{};         //!< Sound buffer bound to the source
};


////////////////////////////////////////////////////////////
Sound::Sound() : m_impl(std::make_unique<Impl>())
{
}


////////////////////////////////////////////////////////////
Sound::Sound(const SoundBuffer& buffer) : m_impl(std::make_unique<Impl>())
{
    setBuffer(buffer);
}


////////////////////////////////////////////////////////////
Sound::Sound(const Sound& copy) : m_impl(std::make_unique<Impl>())
{
    if (copy.m_impl->m_buffer)
        setBuffer(*copy.m_impl->m_buffer);
    setLoop(copy.getLoop());
}


////////////////////////////////////////////////////////////
Sound::~Sound()
{
    stop();
    if (m_impl->m_buffer)
        m_impl->m_buffer->detachSound(this);
}


////////////////////////////////////////////////////////////
void Sound::play()
{
    if (auto result = ma_sound_start(&m_impl->m_sound); result != MA_SUCCESS)
        err() << "Failed to start playing sound: " << ma_result_description(result) << std::endl;
}


////////////////////////////////////////////////////////////
void Sound::pause()
{
    if (auto result = ma_sound_stop(&m_impl->m_sound); result != MA_SUCCESS)
        err() << "Failed to stop playing sound: " << ma_result_description(result) << std::endl;
}


////////////////////////////////////////////////////////////
void Sound::stop()
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
void Sound::setBuffer(const SoundBuffer& buffer)
{
    // First detach from the previous buffer
    if (m_impl->m_buffer)
    {
        stop();

        // Reset cursor
        m_impl->m_cursor = 0;
        m_impl->m_buffer->detachSound(this);
    }

    // Assign and use the new buffer
    m_impl->m_buffer = &buffer;
    m_impl->m_buffer->attachSound(this);

    m_impl->reinitialize();
}


////////////////////////////////////////////////////////////
void Sound::setLoop(bool loop)
{
    ma_sound_set_looping(&m_impl->m_sound, loop ? MA_TRUE : MA_FALSE);
}


////////////////////////////////////////////////////////////
void Sound::setPlayingOffset(Time timeOffset)
{
    ma_uint32 sampleRate{};

    if (auto result = ma_sound_get_data_format(&m_impl->m_sound, nullptr, nullptr, &sampleRate, nullptr, 0);
        result != MA_SUCCESS)
        err() << "Failed to get sound data format: " << ma_result_description(result) << std::endl;

    const auto frameIndex = static_cast<ma_uint64>(timeOffset.asSeconds() * static_cast<double>(sampleRate));

    if (auto result = ma_sound_seek_to_pcm_frame(&m_impl->m_sound, frameIndex); result != MA_SUCCESS)
        err() << "Failed to seek sound to pcm frame: " << ma_result_description(result) << std::endl;

    if (m_impl->m_buffer)
        m_impl->m_cursor = frameIndex * m_impl->m_buffer->getChannelCount();
}


////////////////////////////////////////////////////////////
const SoundBuffer* Sound::getBuffer() const
{
    return m_impl->m_buffer;
}


////////////////////////////////////////////////////////////
bool Sound::getLoop() const
{
    return ma_sound_is_looping(&m_impl->m_sound) == MA_TRUE;
}


////////////////////////////////////////////////////////////
Time Sound::getPlayingOffset() const
{
    if (!m_impl->m_buffer || m_impl->m_buffer->getChannelCount() == 0 || m_impl->m_buffer->getSampleRate() == 0)
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
Sound::Status Sound::getStatus() const
{
    return SoundSource::getStatus();
}


////////////////////////////////////////////////////////////
Sound& Sound::operator=(const Sound& right)
{
    // Here we don't use the copy-and-swap idiom, because it would mess up
    // the list of sound instances contained in the buffers

    // Handle self-assignment here, as no copy-and-swap idiom is being used
    if (this == &right)
        return *this;

    // Delegate to base class, which copies all the sound attributes
    SoundSource::operator=(right);

    // Detach the sound instance from the previous buffer (if any)
    if (m_impl->m_buffer)
    {
        stop();
        m_impl->m_buffer->detachSound(this);
        m_impl->m_buffer = nullptr;
    }

    // Copy the remaining sound attributes
    if (right.m_impl->m_buffer)
        setBuffer(*right.m_impl->m_buffer);
    setLoop(right.getLoop());

    return *this;
}


////////////////////////////////////////////////////////////
void Sound::resetBuffer()
{
    // First stop the sound in case it is playing
    stop();

    // Detach the buffer
    if (m_impl->m_buffer)
    {
        // alCheck(alSourcei(m_source, AL_BUFFER, 0));
        m_impl->m_buffer->detachSound(this);
        m_impl->m_buffer = nullptr;
    }
}


////////////////////////////////////////////////////////////
void* Sound::getSound() const
{
    return &m_impl->m_sound;
}

} // namespace sf
