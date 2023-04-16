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
#include <SFML/Audio/InputSoundFile.hpp>
#include <SFML/Audio/OutputSoundFile.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Time.hpp>

#include <memory>
#include <ostream>
#include <string>
#include <unordered_set>
#include <vector>


namespace sf
{
////////////////////////////////////////////////////////////
struct SoundBuffer::Impl
{
    ////////////////////////////////////////////////////////////
    // Types
    ////////////////////////////////////////////////////////////
    using SoundList = std::unordered_set<Sound*>; //!< Set of unique sound instances

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::vector<std::int16_t> m_samples;                        //!< Samples buffer
    unsigned int              m_channelCount{1};                //!< Number of channels of the sound
    unsigned int              m_sampleRate{44100};              //!< Number of samples per second
    std::vector<SoundChannel> m_channelMap{SoundChannel::Mono}; //!< The map of position in sample frame to sound channel
    Time                      m_duration;                       //!< Sound duration
    mutable SoundList         m_sounds;                         //!< List of sounds that are using this buffer
};


////////////////////////////////////////////////////////////
SoundBuffer::SoundBuffer() : m_impl(std::make_unique<Impl>())
{
}


////////////////////////////////////////////////////////////
SoundBuffer::SoundBuffer(const SoundBuffer& copy) : SoundBuffer()
{
    // don't copy the attached sounds
    m_impl->m_samples  = copy.m_impl->m_samples;
    m_impl->m_duration = copy.m_impl->m_duration;

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
    Impl::SoundList sounds;
    sounds.swap(m_impl->m_sounds);

    // Detach the buffer from the sounds that use it
    for (Sound* soundPtr : sounds)
        soundPtr->resetBuffer();
}


////////////////////////////////////////////////////////////
bool SoundBuffer::loadFromFile(const std::filesystem::path& filename)
{
    InputSoundFile file;
    if (file.openFromFile(filename))
        return initialize(file);
    else
        return false;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::loadFromMemory(const void* data, std::size_t sizeInBytes)
{
    InputSoundFile file;
    if (file.openFromMemory(data, sizeInBytes))
        return initialize(file);
    else
        return false;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::loadFromStream(InputStream& stream)
{
    InputSoundFile file;
    if (file.openFromStream(stream))
        return initialize(file);
    else
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
        m_impl->m_samples.assign(samples, samples + sampleCount);

        // Update the internal buffer with the new samples
        return update(channelCount, sampleRate, channelMap);
    }
    else
    {
        // Error...
        err() << "Failed to load sound buffer from samples ("
              << "array: " << samples << ", "
              << "count: " << sampleCount << ", "
              << "channels: " << channelCount << ", "
              << "samplerate: " << sampleRate << ")" << std::endl;

        return false;
    }
}


////////////////////////////////////////////////////////////
bool SoundBuffer::saveToFile(const std::filesystem::path& filename) const
{
    // Create the sound file in write mode
    OutputSoundFile file;
    if (file.openFromFile(filename, getSampleRate(), getChannelCount(), getChannelMap()))
    {
        // Write the samples to the opened file
        file.write(m_impl->m_samples.data(), m_impl->m_samples.size());

        return true;
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
const std::int16_t* SoundBuffer::getSamples() const
{
    return m_impl->m_samples.empty() ? nullptr : m_impl->m_samples.data();
}


////////////////////////////////////////////////////////////
std::uint64_t SoundBuffer::getSampleCount() const
{
    return m_impl->m_samples.size();
}


////////////////////////////////////////////////////////////
unsigned int SoundBuffer::getSampleRate() const
{
    return m_impl->m_sampleRate;
}


////////////////////////////////////////////////////////////
unsigned int SoundBuffer::getChannelCount() const
{
    return m_impl->m_channelCount;
}


////////////////////////////////////////////////////////////
std::vector<SoundChannel> SoundBuffer::getChannelMap() const
{
    return m_impl->m_channelMap;
}


////////////////////////////////////////////////////////////
Time SoundBuffer::getDuration() const
{
    return m_impl->m_duration;
}


////////////////////////////////////////////////////////////
SoundBuffer& SoundBuffer::operator=(const SoundBuffer& right)
{
    SoundBuffer temp(right);

    std::swap(m_impl->m_samples, temp.m_impl->m_samples);
    std::swap(m_impl->m_channelCount, temp.m_impl->m_channelCount);
    std::swap(m_impl->m_sampleRate, temp.m_impl->m_sampleRate);
    std::swap(m_impl->m_channelMap, temp.m_impl->m_channelMap);
    std::swap(m_impl->m_duration, temp.m_impl->m_duration);
    std::swap(m_impl->m_sounds, temp.m_impl->m_sounds); // swap sounds too, so that they are detached when temp is destroyed

    return *this;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::initialize(InputSoundFile& file)
{
    // Retrieve the sound parameters
    std::uint64_t sampleCount = file.getSampleCount();

    // Read the samples from the provided file
    m_impl->m_samples.resize(static_cast<std::size_t>(sampleCount));
    if (file.read(m_impl->m_samples.data(), sampleCount) == sampleCount)
    {
        // Update the internal buffer with the new samples
        return update(file.getChannelCount(), file.getSampleRate(), file.getChannelMap());
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
bool SoundBuffer::update(unsigned int channelCount, unsigned int sampleRate, const std::vector<SoundChannel>& channelMap)
{
    // Check parameters
    if (!channelCount || !sampleRate || (channelMap.size() != channelCount))
        return false;

    m_impl->m_channelCount = channelCount;
    m_impl->m_sampleRate   = sampleRate;
    m_impl->m_channelMap   = channelMap;

    // First make a copy of the list of sounds so we can reattach later
    Impl::SoundList sounds(m_impl->m_sounds);

    // Detach the buffer from the sounds that use it
    for (Sound* soundPtr : sounds)
        soundPtr->resetBuffer();

    // Compute the duration
    m_impl->m_duration = seconds(
        static_cast<float>(m_impl->m_samples.size()) / static_cast<float>(sampleRate) / static_cast<float>(channelCount));

    // Now reattach the buffer to the sounds that use it
    for (Sound* soundPtr : sounds)
        soundPtr->setBuffer(*this);

    return true;
}


////////////////////////////////////////////////////////////
void SoundBuffer::attachSound(Sound* sound) const
{
    m_impl->m_sounds.insert(sound);
}


////////////////////////////////////////////////////////////
void SoundBuffer::detachSound(Sound* sound) const
{
    m_impl->m_sounds.erase(sound);
}

} // namespace sf
