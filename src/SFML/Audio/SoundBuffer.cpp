////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/SoundFile.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/AudioDevice.hpp>
#include <SFML/Audio/ALCheck.hpp>
#include <SFML/System/Err.hpp>
#include <memory>


namespace sf
{
////////////////////////////////////////////////////////////
SoundBuffer::SoundBuffer() :
m_buffer  (0),
m_duration()
{
    priv::ensureALInit();

    // Create the buffer
    alCheck(alGenBuffers(1, &m_buffer));
}


////////////////////////////////////////////////////////////
SoundBuffer::SoundBuffer(const SoundBuffer& copy) :
m_buffer  (0),
m_samples (copy.m_samples),
m_duration(copy.m_duration),
m_sounds  () // don't copy the attached sounds
{
    // Create the buffer
    alCheck(alGenBuffers(1, &m_buffer));

    // Update the internal buffer with the new samples
    update(copy.getChannelCount(), copy.getSampleRate());
}


////////////////////////////////////////////////////////////
SoundBuffer::~SoundBuffer()
{
    // First detach the buffer from the sounds that use it (to avoid OpenAL errors)
    for (SoundList::const_iterator it = m_sounds.begin(); it != m_sounds.end(); ++it)
        (*it)->resetBuffer();

    // Destroy the buffer
    if (m_buffer)
        alCheck(alDeleteBuffers(1, &m_buffer));
}


////////////////////////////////////////////////////////////
bool SoundBuffer::loadFromFile(const std::string& filename)
{
    priv::SoundFile file;
    if (file.openRead(filename))
        return initialize(file);
    else
        return false;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::loadFromMemory(const void* data, std::size_t sizeInBytes)
{
    priv::SoundFile file;
    if (file.openRead(data, sizeInBytes))
        return initialize(file);
    else
        return false;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::loadFromStream(InputStream& stream)
{
    priv::SoundFile file;
    if (file.openRead(stream))
        return initialize(file);
    else
        return false;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::loadFromSamples(const Int16* samples, std::size_t sampleCount, unsigned int channelCount, unsigned int sampleRate)
{
    if (samples && sampleCount && channelCount && sampleRate)
    {
        // Copy the new audio samples
        m_samples.assign(samples, samples + sampleCount);

        // Update the internal buffer with the new samples
        return update(channelCount, sampleRate);
    }
    else
    {
        // Error...
        err() << "Failed to load sound buffer from samples ("
              << "array: "      << samples      << ", "
              << "count: "      << sampleCount  << ", "
              << "channels: "   << channelCount << ", "
              << "samplerate: " << sampleRate   << ")"
              << std::endl;

        return false;
    }
}


////////////////////////////////////////////////////////////
bool SoundBuffer::saveToFile(const std::string& filename) const
{
    // Create the sound file in write mode
    priv::SoundFile file;
    if (file.openWrite(filename, getChannelCount(), getSampleRate()))
    {
        // Write the samples to the opened file
        file.write(&m_samples[0], m_samples.size());

        return true;
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
const Int16* SoundBuffer::getSamples() const
{
    return m_samples.empty() ? NULL : &m_samples[0];
}


////////////////////////////////////////////////////////////
std::size_t SoundBuffer::getSampleCount() const
{
    return m_samples.size();
}


////////////////////////////////////////////////////////////
unsigned int SoundBuffer::getSampleRate() const
{
    ALint sampleRate;
    alCheck(alGetBufferi(m_buffer, AL_FREQUENCY, &sampleRate));

    return sampleRate;
}


////////////////////////////////////////////////////////////
unsigned int SoundBuffer::getChannelCount() const
{
    ALint channelCount;
    alCheck(alGetBufferi(m_buffer, AL_CHANNELS, &channelCount));

    return channelCount;
}


////////////////////////////////////////////////////////////
Time SoundBuffer::getDuration() const
{
    return m_duration;
}


////////////////////////////////////////////////////////////
SoundBuffer& SoundBuffer::operator =(const SoundBuffer& right)
{
    SoundBuffer temp(right);

    std::swap(m_samples,  temp.m_samples);
    std::swap(m_buffer,   temp.m_buffer);
    std::swap(m_duration, temp.m_duration);
    std::swap(m_sounds,   temp.m_sounds); // swap sounds too, so that they are detached when temp is destroyed

    return *this;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::initialize(priv::SoundFile& file)
{
    // Retrieve the sound parameters
    std::size_t  sampleCount  = file.getSampleCount();
    unsigned int channelCount = file.getChannelCount();
    unsigned int sampleRate   = file.getSampleRate();

    // Read the samples from the provided file
    m_samples.resize(sampleCount);
    if (file.read(&m_samples[0], sampleCount) == sampleCount)
    {
        // Update the internal buffer with the new samples
        return update(channelCount, sampleRate);
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
bool SoundBuffer::update(unsigned int channelCount, unsigned int sampleRate)
{
    // Check parameters
    if (!channelCount || !sampleRate || m_samples.empty())
        return false;

    // Find the good format according to the number of channels
    ALenum format = priv::AudioDevice::getFormatFromChannelCount(channelCount);

    // Check if the format is valid
    if (format == 0)
    {
        err() << "Failed to load sound buffer (unsupported number of channels: " << channelCount << ")" << std::endl;
        return false;
    }

    // Fill the buffer
    ALsizei size = static_cast<ALsizei>(m_samples.size()) * sizeof(Int16);
    alCheck(alBufferData(m_buffer, format, &m_samples[0], size, sampleRate));

    // Compute the duration
    m_duration = milliseconds(1000 * m_samples.size() / sampleRate / channelCount);

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
