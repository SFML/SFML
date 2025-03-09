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
#include <SFML/Audio/MiniaudioUtils.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <SFML/System/Err.hpp>

#include <miniaudio.h>

#include <algorithm>
#include <ostream>
#include <vector>

#include <cassert>
#include <cstring>


namespace sf
{
struct Sound::Impl : priv::MiniaudioUtils::SoundBase
{
    Impl() : SoundBase(vtable, [](void* ptr) { static_cast<Impl*>(ptr)->initialize(); })
    {
        // Initialize sound structure and set default settings
        initialize();
    }

    void initialize()
    {
        SoundBase::initialize(onEnd);

        // Because we are providing a custom data source, we have to provide the channel map ourselves
        if (buffer && !buffer->getChannelMap().empty())
        {
            soundChannelMap.clear();

            for (const SoundChannel channel : buffer->getChannelMap())
            {
                soundChannelMap.push_back(priv::MiniaudioUtils::soundChannelToMiniaudioChannel(channel));
            }

            sound.engineNode.spatializer.pChannelMapIn = soundChannelMap.data();
        }
        else
        {
            sound.engineNode.spatializer.pChannelMapIn = nullptr;
        }
    }

    static void onEnd(void* userData, ma_sound* soundPtr)
    {
        auto& impl  = *static_cast<Impl*>(userData);
        impl.status = Status::Stopped;

        // Seek back to the start of the sound when it finishes playing
        if (const ma_result result = ma_sound_seek_to_pcm_frame(soundPtr, 0); result != MA_SUCCESS)
            err() << "Failed to seek sound to frame 0: " << ma_result_description(result) << std::endl;
    }

    static ma_result read(ma_data_source* dataSource, void* framesOut, std::uint64_t frameCount, std::uint64_t* framesRead)
    {
        auto&       impl   = *static_cast<Impl*>(dataSource);
        const auto* buffer = impl.buffer;

        if (buffer == nullptr)
            return MA_NO_DATA_AVAILABLE;

        // Determine how many frames we can read
        *framesRead = std::min(frameCount, (buffer->getSampleCount() - impl.cursor) / buffer->getChannelCount());

        // Copy the samples to the output
        const auto sampleCount = *framesRead * buffer->getChannelCount();

        std::memcpy(framesOut,
                    buffer->getSamples() + impl.cursor,
                    static_cast<std::size_t>(sampleCount) * sizeof(buffer->getSamples()[0]));

        impl.cursor += static_cast<std::size_t>(sampleCount);

        // If we are looping and at the end of the sound, set the cursor back to the start
        if (impl.looping && (impl.cursor >= buffer->getSampleCount()))
            impl.cursor = 0;

        return MA_SUCCESS;
    }

    static ma_result seek(ma_data_source* dataSource, std::uint64_t frameIndex)
    {
        auto&       impl   = *static_cast<Impl*>(dataSource);
        const auto* buffer = impl.buffer;

        if (buffer == nullptr)
            return MA_NO_DATA_AVAILABLE;

        impl.cursor = static_cast<std::size_t>(frameIndex * buffer->getChannelCount());

        return MA_SUCCESS;
    }

    static ma_result getFormat(ma_data_source* dataSource,
                               ma_format*      format,
                               std::uint32_t*  channels,
                               std::uint32_t*  sampleRate,
                               ma_channel*,
                               size_t)
    {
        const auto& impl   = *static_cast<const Impl*>(dataSource);
        const auto* buffer = impl.buffer;

        // If we don't have valid values yet, initialize with defaults so sound creation doesn't fail
        *format     = ma_format_s16;
        *channels   = buffer && buffer->getChannelCount() ? buffer->getChannelCount() : 1;
        *sampleRate = buffer && buffer->getSampleRate() ? buffer->getSampleRate() : 44100;

        return MA_SUCCESS;
    }

    static ma_result getCursor(ma_data_source* dataSource, std::uint64_t* cursor)
    {
        const auto& impl   = *static_cast<const Impl*>(dataSource);
        const auto* buffer = impl.buffer;

        if (buffer == nullptr)
            return MA_NO_DATA_AVAILABLE;

        *cursor = impl.cursor / buffer->getChannelCount();

        return MA_SUCCESS;
    }

    static ma_result getLength(ma_data_source* dataSource, std::uint64_t* length)
    {
        const auto& impl   = *static_cast<const Impl*>(dataSource);
        const auto* buffer = impl.buffer;

        if (buffer == nullptr)
            return MA_NO_DATA_AVAILABLE;

        *length = buffer->getSampleCount() / buffer->getChannelCount();

        return MA_SUCCESS;
    }

    static ma_result setLooping(ma_data_source* dataSource, ma_bool32 looping)
    {
        auto& impl   = *static_cast<Impl*>(dataSource);
        impl.looping = (looping == MA_TRUE);

        return MA_SUCCESS;
    }

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    static constexpr ma_data_source_vtable vtable{read, seek, getFormat, getCursor, getLength, setLooping, 0};
    std::size_t                            cursor{};  //!< The current playing position
    bool                                   looping{}; //!< `true` if we are looping the sound
    const SoundBuffer*                     buffer{};  //!< Sound buffer bound to the source
};


////////////////////////////////////////////////////////////
Sound::Sound(const SoundBuffer& buffer) : m_impl(std::make_unique<Impl>())
{
    setBuffer(buffer);
}


////////////////////////////////////////////////////////////
// NOLINTNEXTLINE(readability-redundant-member-init)
Sound::Sound(const Sound& copy) : SoundSource(copy), m_impl(std::make_unique<Impl>())
{
    SoundSource::operator=(copy);

    if (copy.m_impl->buffer)
        setBuffer(*copy.m_impl->buffer);
    setLooping(copy.isLooping());
}


////////////////////////////////////////////////////////////
Sound::~Sound()
{
    stop();
    if (m_impl->buffer)
        m_impl->buffer->detachSound(this);
}


////////////////////////////////////////////////////////////
void Sound::play()
{
    if (m_impl->status == Status::Playing)
        setPlayingOffset(Time::Zero);

    if (const ma_result result = ma_sound_start(&m_impl->sound); result != MA_SUCCESS)
    {
        err() << "Failed to start playing sound: " << ma_result_description(result) << std::endl;
    }
    else
    {
        m_impl->status = Status::Playing;
    }
}


////////////////////////////////////////////////////////////
void Sound::pause()
{
    if (const ma_result result = ma_sound_stop(&m_impl->sound); result != MA_SUCCESS)
    {
        err() << "Failed to stop playing sound: " << ma_result_description(result) << std::endl;
    }
    else
    {
        if (m_impl->status == Status::Playing)
            m_impl->status = Status::Paused;
    }
}


////////////////////////////////////////////////////////////
void Sound::stop()
{
    if (const ma_result result = ma_sound_stop(&m_impl->sound); result != MA_SUCCESS)
    {
        err() << "Failed to stop playing sound: " << ma_result_description(result) << std::endl;
    }
    else
    {
        setPlayingOffset(Time::Zero);
        m_impl->status = Status::Stopped;
    }
}


////////////////////////////////////////////////////////////
void Sound::setBuffer(const SoundBuffer& buffer)
{
    // First detach from the previous buffer
    if (m_impl->buffer)
    {
        stop();

        // Reset cursor
        m_impl->cursor = 0;
        m_impl->buffer->detachSound(this);
    }

    // Assign and use the new buffer
    m_impl->buffer = &buffer;
    m_impl->buffer->attachSound(this);

    m_impl->deinitialize();
    m_impl->initialize();
}


////////////////////////////////////////////////////////////
void Sound::setLooping(bool loop)
{
    ma_sound_set_looping(&m_impl->sound, loop ? MA_TRUE : MA_FALSE);
}


////////////////////////////////////////////////////////////
void Sound::setPlayingOffset(Time timeOffset)
{
    if (m_impl->sound.pDataSource == nullptr || m_impl->sound.engineNode.pEngine == nullptr)
        return;

    const auto frameIndex = priv::MiniaudioUtils::getFrameIndex(m_impl->sound, timeOffset);

    if (m_impl->buffer)
        m_impl->cursor = static_cast<std::size_t>(frameIndex * m_impl->buffer->getChannelCount());
}


////////////////////////////////////////////////////////////
void Sound::setEffectProcessor(EffectProcessor effectProcessor)
{
    m_impl->effectProcessor = std::move(effectProcessor);
    m_impl->connectEffect(bool{m_impl->effectProcessor});
}


////////////////////////////////////////////////////////////
const SoundBuffer& Sound::getBuffer() const
{
    assert(m_impl && "Sound::getBuffer() Cannot access unset buffer");
    return *m_impl->buffer;
}


////////////////////////////////////////////////////////////
bool Sound::isLooping() const
{
    return ma_sound_is_looping(&m_impl->sound) == MA_TRUE;
}


////////////////////////////////////////////////////////////
Time Sound::getPlayingOffset() const
{
    if (!m_impl->buffer || m_impl->buffer->getChannelCount() == 0 || m_impl->buffer->getSampleRate() == 0)
        return {};

    return priv::MiniaudioUtils::getPlayingOffset(m_impl->sound);
}


////////////////////////////////////////////////////////////
Sound::Status Sound::getStatus() const
{
    return m_impl->status;
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
    if (m_impl->buffer)
    {
        stop();
        m_impl->buffer->detachSound(this);
        m_impl->buffer = nullptr;
    }

    // Copy the remaining sound attributes
    if (right.m_impl->buffer)
        setBuffer(*right.m_impl->buffer);
    setLooping(right.isLooping());

    return *this;
}


////////////////////////////////////////////////////////////
void Sound::detachBuffer()
{
    // First stop the sound in case it is playing
    stop();

    // Detach the buffer
    if (m_impl->buffer)
    {
        m_impl->buffer->detachSound(this);
        m_impl->buffer = nullptr;
    }
}


////////////////////////////////////////////////////////////
void* Sound::getSound() const
{
    return &m_impl->sound;
}

} // namespace sf
