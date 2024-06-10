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
#include <SFML/Audio/SoundFileFactory.hpp>
#include <SFML/Audio/SoundFileReader.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/FileInputStream.hpp>
#include <SFML/System/InputStream.hpp>
#include <SFML/System/MemoryInputStream.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Utils.hpp>

#include <ostream>
#include <utility>

#include <cassert>
#include <cstdint>


namespace sf
{
////////////////////////////////////////////////////////////
InputSoundFile::StreamDeleter::StreamDeleter(bool theOwned) : owned(theOwned)
{
}


////////////////////////////////////////////////////////////
template <typename T>
InputSoundFile::StreamDeleter::StreamDeleter(const std::default_delete<T>&)
{
}


////////////////////////////////////////////////////////////
void InputSoundFile::StreamDeleter::operator()(InputStream* ptr) const
{
    if (owned)
        delete ptr;
}


////////////////////////////////////////////////////////////
std::optional<InputSoundFile> InputSoundFile::openFromFile(const std::filesystem::path& filename)
{
    // Find a suitable reader for the file type
    auto reader = SoundFileFactory::createReaderFromFilename(filename);
    if (!reader)
    {
        // Error message generated in called function.
        return std::nullopt;
    }

    // Open the file
    auto fileInputStream = FileInputStream::open(filename);
    if (!fileInputStream)
    {
        err() << "Failed to open input sound file from file (couldn't open file input stream)\n"
              << formatDebugPathInfo(filename) << std::endl;

        return std::nullopt;
    }

    // Wrap the file into a stream
    auto file = std::make_unique<FileInputStream>(std::move(*fileInputStream));

    // Pass the stream to the reader
    auto info = reader->open(*file);
    if (!info)
    {
        err() << "Failed to open input sound file from file (reader open failure)\n"
              << formatDebugPathInfo(filename) << std::endl;

        return std::nullopt;
    }

    return InputSoundFile(std::move(reader), std::move(file), info->sampleCount, info->sampleRate, std::move(info->channelMap));
}


////////////////////////////////////////////////////////////
std::optional<InputSoundFile> InputSoundFile::openFromMemory(const void* data, std::size_t sizeInBytes)
{
    // Find a suitable reader for the file type
    auto reader = SoundFileFactory::createReaderFromMemory(data, sizeInBytes);
    if (!reader)
    {
        // Error message generated in called function.
        return std::nullopt;
    }

    // Wrap the memory file into a stream
    auto memory = std::make_unique<MemoryInputStream>(data, sizeInBytes);

    // Pass the stream to the reader
    auto info = reader->open(*memory);
    if (!info)
    {
        err() << "Failed to open input sound file from memory (reader open failure)" << std::endl;
        return std::nullopt;
    }

    return InputSoundFile(std::move(reader), std::move(memory), info->sampleCount, info->sampleRate, std::move(info->channelMap));
}


////////////////////////////////////////////////////////////
std::optional<InputSoundFile> InputSoundFile::openFromStream(InputStream& stream)
{
    // Find a suitable reader for the file type
    auto reader = SoundFileFactory::createReaderFromStream(stream);
    if (!reader)
    {
        // Error message generated in called function.
        return std::nullopt;
    }

    // Don't forget to reset the stream to its beginning before re-opening it
    if (stream.seek(0) != 0)
    {
        err() << "Failed to open sound file from stream (cannot restart stream)" << std::endl;
        return std::nullopt;
    }

    // Pass the stream to the reader
    auto info = reader->open(stream);
    if (!info)
    {
        err() << "Failed to open input sound file from stream (reader open failure)" << std::endl;
        return std::nullopt;
    }

    return InputSoundFile(std::move(reader), {&stream, false}, info->sampleCount, info->sampleRate, std::move(info->channelMap));
}


////////////////////////////////////////////////////////////
std::uint64_t InputSoundFile::getSampleCount() const
{
    return m_sampleCount;
}


////////////////////////////////////////////////////////////
unsigned int InputSoundFile::getChannelCount() const
{
    return static_cast<unsigned int>(m_channelMap.size());
}


////////////////////////////////////////////////////////////
unsigned int InputSoundFile::getSampleRate() const
{
    return m_sampleRate;
}


////////////////////////////////////////////////////////////
const std::vector<SoundChannel>& InputSoundFile::getChannelMap() const
{
    return m_channelMap;
}


////////////////////////////////////////////////////////////
Time InputSoundFile::getDuration() const
{
    // Make sure we don't divide by 0
    if (m_channelMap.empty() || m_sampleRate == 0)
        return Time::Zero;

    return seconds(
        static_cast<float>(m_sampleCount) / static_cast<float>(m_channelMap.size()) / static_cast<float>(m_sampleRate));
}


////////////////////////////////////////////////////////////
Time InputSoundFile::getTimeOffset() const
{
    // Make sure we don't divide by 0
    if (m_channelMap.empty() || m_sampleRate == 0)
        return Time::Zero;

    return seconds(
        static_cast<float>(m_sampleOffset) / static_cast<float>(m_channelMap.size()) / static_cast<float>(m_sampleRate));
}


////////////////////////////////////////////////////////////
std::uint64_t InputSoundFile::getSampleOffset() const
{
    return m_sampleOffset;
}


////////////////////////////////////////////////////////////
void InputSoundFile::seek(std::uint64_t sampleOffset)
{
    assert(m_reader);

    if (!m_channelMap.empty())
    {
        // The reader handles an overrun gracefully, but we
        // pre-check to keep our known position consistent
        m_sampleOffset = std::min(sampleOffset / m_channelMap.size() * m_channelMap.size(), m_sampleCount);
        m_reader->seek(m_sampleOffset);
    }
}


////////////////////////////////////////////////////////////
void InputSoundFile::seek(Time timeOffset)
{
    seek(static_cast<std::size_t>(timeOffset.asSeconds() * static_cast<float>(m_sampleRate)) * m_channelMap.size());
}


////////////////////////////////////////////////////////////
std::uint64_t InputSoundFile::read(std::int16_t* samples, std::uint64_t maxCount)
{
    assert(m_reader);

    std::uint64_t readSamples = 0;
    if (samples && maxCount)
        readSamples = m_reader->read(samples, maxCount);
    m_sampleOffset += readSamples;
    return readSamples;
}


////////////////////////////////////////////////////////////
void InputSoundFile::close()
{
    *this = {};
    m_channelMap.clear();
}


////////////////////////////////////////////////////////////
InputSoundFile::InputSoundFile(std::unique_ptr<SoundFileReader>&&            reader,
                               std::unique_ptr<InputStream, StreamDeleter>&& stream,
                               std::uint64_t                                 sampleCount,
                               unsigned int                                  sampleRate,
                               std::vector<SoundChannel>&&                   channelMap) :
m_reader(std::move(reader)),
m_stream(std::move(stream)),
m_sampleCount(sampleCount),
m_sampleRate(sampleRate),
m_channelMap(std::move(channelMap))
{
}

} // namespace sf
