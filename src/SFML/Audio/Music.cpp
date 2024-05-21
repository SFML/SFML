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
#include <SFML/Audio/Music.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Time.hpp>

#include <algorithm>
#include <mutex>
#include <ostream>

#include <cassert>


namespace sf
{
////////////////////////////////////////////////////////////
Music::~Music()
{
    // We must stop before destroying the file
    stop();
}


////////////////////////////////////////////////////////////
bool Music::openFromFile(const std::filesystem::path& filename)
{
    // First stop the music if it was already running
    stop();

    // Open the underlying sound file
    m_file = sf::InputSoundFile::openFromFile(filename);
    if (!m_file)
        return false;

    // Perform common initializations
    initialize();

    return true;
}


////////////////////////////////////////////////////////////
bool Music::openFromMemory(const void* data, std::size_t sizeInBytes)
{
    // First stop the music if it was already running
    stop();

    // Open the underlying sound file
    m_file = sf::InputSoundFile::openFromMemory(data, sizeInBytes);
    if (!m_file)
        return false;

    // Perform common initializations
    initialize();

    return true;
}


////////////////////////////////////////////////////////////
bool Music::openFromStream(InputStream& stream)
{
    // First stop the music if it was already running
    stop();

    // Open the underlying sound file
    m_file = sf::InputSoundFile::openFromStream(stream);
    if (!m_file)
        return false;

    // Perform common initializations
    initialize();

    return true;
}


////////////////////////////////////////////////////////////
Time Music::getDuration() const
{
    assert(m_file && "Music::getDuration() Cannot get duration until music is opened");
    return m_file->getDuration();
}


////////////////////////////////////////////////////////////
Music::TimeSpan Music::getLoopPoints() const
{
    return TimeSpan{samplesToTime(m_loopSpan.offset), samplesToTime(m_loopSpan.length)};
}


////////////////////////////////////////////////////////////
void Music::setLoopPoints(TimeSpan timePoints)
{
    Span<std::uint64_t> samplePoints{timeToSamples(timePoints.offset), timeToSamples(timePoints.length)};

    // Check our state. This averts a divide-by-zero. GetChannelCount() is cheap enough to use often
    assert(m_file && "Music::setLoopPoints() Cannot set loop points unit music is opened");
    if (getChannelCount() == 0 || m_file->getSampleCount() == 0)
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
    if (samplePoints.offset >= m_file->getSampleCount())
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
    samplePoints.length = std::min(samplePoints.length, m_file->getSampleCount() - samplePoints.offset);

    // If this change has no effect, we can return without touching anything
    if (samplePoints.offset == m_loopSpan.offset && samplePoints.length == m_loopSpan.length)
        return;

    // When we apply this change, we need to "reset" this instance and its buffer

    // Get old playing status and position
    const Status oldStatus = getStatus();
    const Time   oldPos    = getPlayingOffset();

    // Unload
    stop();

    // Set
    m_loopSpan = samplePoints;

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
    assert(m_file && "Music::onGetData() Cannot perform operation until music is opened");

    const std::lock_guard lock(m_mutex);

    std::size_t         toFill        = m_samples.size();
    std::uint64_t       currentOffset = m_file->getSampleOffset();
    const std::uint64_t loopEnd       = m_loopSpan.offset + m_loopSpan.length;

    // If the loop end is enabled and imminent, request less data.
    // This will trip an "onLoop()" call from the underlying SoundStream,
    // and we can then take action.
    if (getLoop() && (m_loopSpan.length != 0) && (currentOffset <= loopEnd) && (currentOffset + toFill > loopEnd))
        toFill = static_cast<std::size_t>(loopEnd - currentOffset);

    // Fill the chunk parameters
    data.samples     = m_samples.data();
    data.sampleCount = static_cast<std::size_t>(m_file->read(m_samples.data(), toFill));
    currentOffset += data.sampleCount;

    // Check if we have stopped obtaining samples or reached either the EOF or the loop end point
    return (data.sampleCount != 0) && (currentOffset < m_file->getSampleCount()) &&
           (currentOffset != loopEnd || m_loopSpan.length == 0);
}


////////////////////////////////////////////////////////////
void Music::onSeek(Time timeOffset)
{
    assert(m_file && "Music::onSeek() Cannot perform operation until music is opened");

    const std::lock_guard lock(m_mutex);
    m_file->seek(timeOffset);
}


////////////////////////////////////////////////////////////
std::optional<std::uint64_t> Music::onLoop()
{
    assert(m_file && "Music::onLoop() Cannot perform operation until music is opened");

    // Called by underlying SoundStream so we can determine where to loop.
    const std::lock_guard lock(m_mutex);
    const std::uint64_t   currentOffset = m_file->getSampleOffset();
    if (getLoop() && (m_loopSpan.length != 0) && (currentOffset == m_loopSpan.offset + m_loopSpan.length))
    {
        // Looping is enabled, and either we're at the loop end, or we're at the EOF
        // when it's equivalent to the loop end (loop end takes priority). Send us to loop begin
        m_file->seek(m_loopSpan.offset);
        return static_cast<std::int64_t>(m_file->getSampleOffset());
    }
    else if (getLoop() && (currentOffset >= m_file->getSampleCount()))
    {
        // If we're at the EOF, reset to 0
        m_file->seek(0);
        return 0;
    }
    return std::nullopt;
}


////////////////////////////////////////////////////////////
void Music::initialize()
{
    // Compute the music positions
    m_loopSpan.offset = 0;
    m_loopSpan.length = m_file->getSampleCount();

    // Resize the internal buffer so that it can contain 1 second of audio samples
    m_samples.resize(m_file->getSampleRate() * m_file->getChannelCount());

    // Initialize the stream
    SoundStream::initialize(m_file->getChannelCount(), m_file->getSampleRate(), m_file->getChannelMap());
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
