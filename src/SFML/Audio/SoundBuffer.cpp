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
#include <SFML/Audio/InputSoundFile.hpp>
#include <SFML/Audio/OutputSoundFile.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Exception.hpp>

#include <exception>
#include <ostream>
#include <utility>


namespace sf
{
////////////////////////////////////////////////////////////
SoundBuffer::SoundBuffer(const std::filesystem::path& filename)
{
    if (!loadFromFile(filename))
        throw sf::Exception("Failed to open sound buffer from file");
}


////////////////////////////////////////////////////////////
SoundBuffer::SoundBuffer(const void* data, std::size_t sizeInBytes)
{
    if (!loadFromMemory(data, sizeInBytes))
        throw sf::Exception("Failed to open sound buffer from memory");
}


////////////////////////////////////////////////////////////
SoundBuffer::SoundBuffer(InputStream& stream)
{
    if (!loadFromStream(stream))
        throw sf::Exception("Failed to open sound buffer from stream");
}


////////////////////////////////////////////////////////////
SoundBuffer::SoundBuffer(const std::int16_t*              samples,
                         std::uint64_t                    sampleCount,
                         unsigned int                     channelCount,
                         unsigned int                     sampleRate,
                         const std::vector<SoundChannel>& channelMap)
{
    if (!loadFromSamples(samples, sampleCount, channelCount, sampleRate, channelMap))
        throw sf::Exception("Failed to open sound buffer from samples");
}


////////////////////////////////////////////////////////////
SoundBuffer::SoundBuffer(const SoundBuffer& copy)
{
    // don't copy the attached sounds
    m_samples  = copy.m_samples;
    m_duration = copy.m_duration;

    // Update the internal buffer with the new samples
    if (!update(copy.getChannelCount(), copy.getSampleRate(), copy.getChannelMap()))
        err() << "Failed to update copy-constructed sound buffer" << std::endl;
}


////////////////////////////////////////////////////////////
SoundBuffer::~SoundBuffer()
{
    // To prevent the iterator from becoming invalid, move the entire buffer to another
    // container. Otherwise calling resetBuffer would result in detachSound being
    // called which removes the sound from the internal list.
    SoundList sounds;
    sounds.swap(m_sounds);

    // Detach the buffer from the sounds that use it
    for (Sound* soundPtr : sounds)
        soundPtr->detachBuffer();
}


////////////////////////////////////////////////////////////
bool SoundBuffer::loadFromFile(const std::filesystem::path& filename)
{
    InputSoundFile file;
    if (file.openFromFile(filename))
        return initialize(file);

    err() << "Failed to open sound buffer from file" << std::endl;
    return false;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::loadFromMemory(const void* data, std::size_t sizeInBytes)
{
    InputSoundFile file;
    if (file.openFromMemory(data, sizeInBytes))
        return initialize(file);

    err() << "Failed to open sound buffer from memory" << std::endl;
    return false;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::loadFromStream(InputStream& stream)
{
    InputSoundFile file;
    if (file.openFromStream(stream))
        return initialize(file);

    err() << "Failed to open sound buffer from stream" << std::endl;
    return false;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::loadFromSamples(const std::int16_t*              samples,
                                  std::uint64_t                    sampleCount,
                                  unsigned int                     channelCount,
                                  unsigned int                     sampleRate,
                                  const std::vector<SoundChannel>& channelMap)
{
    if (samples && sampleCount && channelCount && sampleRate && !channelMap.empty())
    {
        // Copy the new audio samples
        m_samples.assign(samples, samples + sampleCount);

        // Update the internal buffer with the new samples
        return update(channelCount, sampleRate, channelMap);
    }

    // Error...
    err() << "Failed to load sound buffer from samples ("
          << "array: " << samples << ", "
          << "count: " << sampleCount << ", "
          << "channels: " << channelCount << ", "
          << "samplerate: " << sampleRate << ")" << std::endl;

    return false;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::saveToFile(const std::filesystem::path& filename) const
{
    // Create the sound file in write mode
    OutputSoundFile file;
    if (file.openFromFile(filename, getSampleRate(), getChannelCount(), getChannelMap()))
    {
        // Write the samples to the opened file
        file.write(m_samples.data(), m_samples.size());

        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////
const std::int16_t* SoundBuffer::getSamples() const
{
    return m_samples.empty() ? nullptr : m_samples.data();
}


////////////////////////////////////////////////////////////
std::uint64_t SoundBuffer::getSampleCount() const
{
    return m_samples.size();
}


////////////////////////////////////////////////////////////
unsigned int SoundBuffer::getSampleRate() const
{
    return m_sampleRate;
}


////////////////////////////////////////////////////////////
unsigned int SoundBuffer::getChannelCount() const
{
    return static_cast<unsigned int>(m_channelMap.size());
}


////////////////////////////////////////////////////////////
std::vector<SoundChannel> SoundBuffer::getChannelMap() const
{
    return m_channelMap;
}


////////////////////////////////////////////////////////////
Time SoundBuffer::getDuration() const
{
    return m_duration;
}


////////////////////////////////////////////////////////////
SoundBuffer& SoundBuffer::operator=(const SoundBuffer& right)
{
    SoundBuffer temp(right);

    std::swap(m_samples, temp.m_samples);
    std::swap(m_sampleRate, temp.m_sampleRate);
    std::swap(m_channelMap, temp.m_channelMap);
    std::swap(m_duration, temp.m_duration);
    std::swap(m_sounds, temp.m_sounds); // swap sounds too, so that they are detached when temp is destroyed

    return *this;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::initialize(InputSoundFile& file)
{
    // Retrieve the sound parameters
    const std::uint64_t sampleCount = file.getSampleCount();

    // Read the samples from the provided file
    m_samples.resize(static_cast<std::size_t>(sampleCount));
    if (file.read(m_samples.data(), sampleCount) == sampleCount)
    {
        // Update the internal buffer with the new samples
        if (!update(file.getChannelCount(), file.getSampleRate(), file.getChannelMap()))
        {
            err() << "Failed to initialize sound buffer (internal update failure)" << std::endl;
            return false;
        }

        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::update(unsigned int channelCount, unsigned int sampleRate, const std::vector<SoundChannel>& channelMap)
{
    // Check parameters
    if (!channelCount || !sampleRate || (channelMap.size() != channelCount))
        return false;

    m_sampleRate = sampleRate;
    m_channelMap = channelMap;

    // First make a copy of the list of sounds so we can reattach later
    const SoundList sounds(m_sounds);

    // Detach the buffer from the sounds that use it
    for (Sound* soundPtr : sounds)
        soundPtr->detachBuffer();

    // Compute the duration
    m_duration = seconds(
        static_cast<float>(m_samples.size()) / static_cast<float>(sampleRate) / static_cast<float>(channelCount));

    // Now reattach the buffer to the sounds that use it
    for (Sound* soundPtr : sounds)
        soundPtr->setBuffer(*this);

    return true;
}


////////////////////////////////////////////////////////////
void SoundBuffer::attachSound(Sound* sound) const
{
    m_sounds.insert(sound);
}


////////////////////////////////////////////////////////////
void SoundBuffer::detachSound(Sound* sound) const
{
    m_sounds.erase(sound);
}

} // namespace sf
