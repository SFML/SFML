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
#include <SFML/Audio/InputSoundFile.hpp>
#include <SFML/Audio/Music.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Time.hpp>

#include <algorithm>
#include <mutex>
#include <ostream>


namespace sf
{
////////////////////////////////////////////////////////////
struct Music::Impl
{
    InputSoundFile            file;     //!< The streamed music file
    std::vector<std::int16_t> samples;  //!< Temporary buffer of samples
    std::recursive_mutex      mutex;    //!< Mutex protecting the data
    Span<std::uint64_t>       loopSpan; //!< Loop Range Specifier

    explicit Impl(InputSoundFile&& theFile) :
    file(std::move(theFile)),

    // Resize the internal buffer so that it can contain 1 second of audio samples
    samples(file.getSampleRate() * file.getChannelCount()),

    // Compute the music positions
    loopSpan{0u, file.getSampleCount()}
    {
    }
};


////////////////////////////////////////////////////////////
Music::~Music()
{
    // We must stop before destroying the file
    if (m_impl != nullptr)
    {
        stop();
    }
}


////////////////////////////////////////////////////////////
Music::Music(Music&&) noexcept = default;


////////////////////////////////////////////////////////////
Music& Music::operator=(Music&&) noexcept = default;


////////////////////////////////////////////////////////////
std::optional<Music> Music::tryOpenFromInputSoundFile(std::optional<InputSoundFile>&& optFile, const char* errorContext)
{
    if (!optFile.has_value())
    {
        err() << "Failed to open music from " << errorContext << std::endl;
        return std::nullopt;
    }

    // TODO: apply RVO here via passkey idiom
    return Music(std::move(*optFile));
}


////////////////////////////////////////////////////////////
std::optional<Music> Music::openFromFile(const std::filesystem::path& filename)
{
    return tryOpenFromInputSoundFile(InputSoundFile::openFromFile(filename), "file");
}


////////////////////////////////////////////////////////////
std::optional<Music> Music::openFromMemory(const void* data, std::size_t sizeInBytes)
{
    return tryOpenFromInputSoundFile(InputSoundFile::openFromMemory(data, sizeInBytes), "memory");
}


////////////////////////////////////////////////////////////
std::optional<Music> Music::openFromStream(InputStream& stream)
{
    return tryOpenFromInputSoundFile(InputSoundFile::openFromStream(stream), "stream");
}


////////////////////////////////////////////////////////////
Time Music::getDuration() const
{
    return m_impl->file.getDuration();
}


////////////////////////////////////////////////////////////
Music::TimeSpan Music::getLoopPoints() const
{
    return TimeSpan{samplesToTime(m_impl->loopSpan.offset), samplesToTime(m_impl->loopSpan.length)};
}


////////////////////////////////////////////////////////////
void Music::setLoopPoints(TimeSpan timePoints)
{
    Span<std::uint64_t> samplePoints{timeToSamples(timePoints.offset), timeToSamples(timePoints.length)};

    // Check our state. This averts a divide-by-zero. GetChannelCount() is cheap enough to use often
    if (getChannelCount() == 0 || m_impl->file.getSampleCount() == 0)
    {
        err() << "Music is not in a valid state to assign Loop Points." << std::endl;
        return;
    }

    // Round up to the next even sample if needed
    samplePoints.offset += (getChannelCount() - 1);
    samplePoints.offset -= (samplePoints.offset % getChannelCount());
    samplePoints.length += (getChannelCount() - 1);
    samplePoints.length -= (samplePoints.length % getChannelCount());

    // Validate
    if (samplePoints.offset >= m_impl->file.getSampleCount())
    {
        err() << "LoopPoints offset val must be in range [0, Duration)." << std::endl;
        return;
    }

    if (samplePoints.length == 0)
    {
        err() << "LoopPoints length val must be nonzero." << std::endl;
        return;
    }

    // Clamp End Point
    samplePoints.length = std::min(samplePoints.length, m_impl->file.getSampleCount() - samplePoints.offset);

    // If this change has no effect, we can return without touching anything
    if (samplePoints.offset == m_impl->loopSpan.offset && samplePoints.length == m_impl->loopSpan.length)
        return;

    // When we apply this change, we need to "reset" this instance and its buffer

    // Get old playing status and position
    const Status oldStatus = getStatus();
    const Time   oldPos    = getPlayingOffset();

    // Unload
    stop();

    // Set
    m_impl->loopSpan = samplePoints;

    // Restore
    if (oldPos != Time::Zero)
        setPlayingOffset(oldPos);

    // Resume
    if (oldStatus == Status::Playing)
        play();
}


////////////////////////////////////////////////////////////
bool Music::onGetData(SoundStream::Chunk& data)
{
    const std::lock_guard lock(m_impl->mutex);

    std::size_t         toFill        = m_impl->samples.size();
    std::uint64_t       currentOffset = m_impl->file.getSampleOffset();
    const std::uint64_t loopEnd       = m_impl->loopSpan.offset + m_impl->loopSpan.length;

    // If the loop end is enabled and imminent, request less data.
    // This will trip an "onLoop()" call from the underlying SoundStream,
    // and we can then take action.
    if (getLoop() && (m_impl->loopSpan.length != 0) && (currentOffset <= loopEnd) && (currentOffset + toFill > loopEnd))
        toFill = static_cast<std::size_t>(loopEnd - currentOffset);

    // Fill the chunk parameters
    data.samples     = m_impl->samples.data();
    data.sampleCount = static_cast<std::size_t>(m_impl->file.read(m_impl->samples.data(), toFill));
    currentOffset += data.sampleCount;

    // Check if we have stopped obtaining samples or reached either the EOF or the loop end point
    return (data.sampleCount != 0) && (currentOffset < m_impl->file.getSampleCount()) &&
           (currentOffset != loopEnd || m_impl->loopSpan.length == 0);
}


////////////////////////////////////////////////////////////
void Music::onSeek(Time timeOffset)
{
    const std::lock_guard lock(m_impl->mutex);
    m_impl->file.seek(timeOffset);
}


////////////////////////////////////////////////////////////
std::optional<std::uint64_t> Music::onLoop()
{
    // Called by underlying SoundStream so we can determine where to loop.
    const std::lock_guard lock(m_impl->mutex);
    const std::uint64_t   currentOffset = m_impl->file.getSampleOffset();

    if (getLoop() && (m_impl->loopSpan.length != 0) && (currentOffset == m_impl->loopSpan.offset + m_impl->loopSpan.length))
    {
        // Looping is enabled, and either we're at the loop end, or we're at the EOF
        // when it's equivalent to the loop end (loop end takes priority). Send us to loop begin
        m_impl->file.seek(m_impl->loopSpan.offset);
        return m_impl->file.getSampleOffset();
    }

    if (getLoop() && (currentOffset >= m_impl->file.getSampleCount()))
    {
        // If we're at the EOF, reset to 0
        m_impl->file.seek(0);
        return 0;
    }

    return std::nullopt;
}


////////////////////////////////////////////////////////////
Music::Music(InputSoundFile&& file) : m_impl(std::make_unique<Impl>(std::move(file)))
{
    // Initialize the stream
    SoundStream::initialize(m_impl->file.getChannelCount(), m_impl->file.getSampleRate(), m_impl->file.getChannelMap());
}


////////////////////////////////////////////////////////////
std::uint64_t Music::timeToSamples(Time position) const
{
    // Always ROUND, no unchecked truncation, hence the addition in the numerator.
    // This avoids most precision errors arising from "samples => Time => samples" conversions
    // Original rounding calculation is ((Micros * Freq * Channels) / 1000000) + 0.5
    // We refactor it to keep std::int64_t as the data type throughout the whole operation.
    return ((static_cast<std::uint64_t>(position.asMicroseconds()) * getSampleRate() * getChannelCount()) + 500000) / 1000000;
}


////////////////////////////////////////////////////////////
Time Music::samplesToTime(std::uint64_t samples) const
{
    Time position = Time::Zero;

    // Make sure we don't divide by 0
    if (getSampleRate() != 0 && getChannelCount() != 0)
        position = microseconds(static_cast<std::int64_t>((samples * 1000000) / (getChannelCount() * getSampleRate())));

    return position;
}

} // namespace sf
