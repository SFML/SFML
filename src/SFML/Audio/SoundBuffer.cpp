////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Audio/AudioDevice.hpp>
#include <SFML/Audio/OpenAL.hpp>
#include <iostream>
#include <memory>


namespace sf
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
SoundBuffer::SoundBuffer() :
myBuffer  (0),
myDuration(0.f)
{
    // Create the buffer
    ALCheck(alGenBuffers(1, &myBuffer));
}


////////////////////////////////////////////////////////////
/// Copy constructor
////////////////////////////////////////////////////////////
SoundBuffer::SoundBuffer(const SoundBuffer& copy) :
Resource<SoundBuffer>(copy),
myBuffer             (0),
mySamples            (copy.mySamples),
myDuration           (copy.myDuration)
{
    // Create the buffer
    ALCheck(alGenBuffers(1, &myBuffer));

    // Update the internal buffer with the new samples
    Update(copy.GetChannelsCount(), copy.GetSampleRate());
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
SoundBuffer::~SoundBuffer()
{
    if (myBuffer)
        ALCheck(alDeleteBuffers(1, &myBuffer));
}


////////////////////////////////////////////////////////////
/// Load the sound buffer from a file
////////////////////////////////////////////////////////////
bool SoundBuffer::LoadFromFile(const std::string& filename)
{
    // Open the sound file
    priv::SoundFile file;
    if (file.OpenRead(filename))
    {
        // Get the sound parameters
        std::size_t  nbSamples     = file.GetSamplesCount();
        unsigned int channelsCount = file.GetChannelsCount();
        unsigned int sampleRate    = file.GetSampleRate();

        // Read the samples from the opened file
        mySamples.resize(nbSamples);
        if (file.Read(&mySamples[0], nbSamples) == nbSamples)
        {
            // Update the internal buffer with the new samples
            return Update(channelsCount, sampleRate);
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
/// Load the sound buffer from a file in memory
////////////////////////////////////////////////////////////
bool SoundBuffer::LoadFromMemory(const char* data, std::size_t sizeInBytes)
{
    // Open the sound file
    priv::SoundFile file;
    if (file.OpenRead(data, sizeInBytes))
    {
        // Get the sound parameters
        std::size_t  nbSamples     = file.GetSamplesCount();
        unsigned int channelsCount = file.GetChannelsCount();
        unsigned int sampleRate    = file.GetSampleRate();

        // Read the samples from the opened file
        mySamples.resize(nbSamples);
        if (file.Read(&mySamples[0], nbSamples) == nbSamples)
        {
            // Update the internal buffer with the new samples
            return Update(channelsCount, sampleRate);
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
/// Load the sound buffer from an array of samples  - assumed format for
/// samples is 16 bits signed integer
////////////////////////////////////////////////////////////
bool SoundBuffer::LoadFromSamples(const Int16* samples, std::size_t samplesCount, unsigned int channelsCount, unsigned int sampleRate)
{
    if (samples && samplesCount && channelsCount && sampleRate)
    {
        // Copy the new audio samples
        mySamples.assign(samples, samples + samplesCount);

        // Update the internal buffer with the new samples
        return Update(channelsCount, sampleRate);
    }
    else
    {
        // Error...
        std::cerr << "Failed to load sound buffer from memory ("
                  << "Samples : "        << samples       << ", "
                  << "Samples count : "  << samplesCount  << ", "
                  << "Channels count : " << channelsCount << ", "
                  << "Sample rate : "    << sampleRate    << ")"
                  << std::endl;

        return false;
    }
}


////////////////////////////////////////////////////////////
/// Save the sound buffer to a file
////////////////////////////////////////////////////////////
bool SoundBuffer::SaveToFile(const std::string& filename) const
{
    // Create the sound file in write mode
    priv::SoundFile file;
    if (file.OpenWrite(filename, GetChannelsCount(), GetSampleRate()))
    {
        // Write the samples to the opened file
        file.Write(&mySamples[0], mySamples.size());

        return true;
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
/// Return the sound samples
////////////////////////////////////////////////////////////
const Int16* SoundBuffer::GetSamples() const
{
    return mySamples.empty() ? NULL : &mySamples[0];
}


////////////////////////////////////////////////////////////
/// Return the samples count
////////////////////////////////////////////////////////////
std::size_t SoundBuffer::GetSamplesCount() const
{
    return mySamples.size();
}


////////////////////////////////////////////////////////////
/// Get the sample rate
////////////////////////////////////////////////////////////
unsigned int SoundBuffer::GetSampleRate() const
{
    ALint sampleRate;
    ALCheck(alGetBufferi(myBuffer, AL_FREQUENCY, &sampleRate));

    return sampleRate;
}


////////////////////////////////////////////////////////////
/// Return the number of channels (1 = mono, 2 = stereo, ...)
////////////////////////////////////////////////////////////
unsigned int SoundBuffer::GetChannelsCount() const
{
    ALint channelsCount;
    ALCheck(alGetBufferi(myBuffer, AL_CHANNELS, &channelsCount));

    return channelsCount;
}


////////////////////////////////////////////////////////////
/// Get the sound duration
////////////////////////////////////////////////////////////
float SoundBuffer::GetDuration() const
{
    return myDuration;
}


////////////////////////////////////////////////////////////
/// Assignment operator
////////////////////////////////////////////////////////////
SoundBuffer& SoundBuffer::operator =(const SoundBuffer& other)
{
    SoundBuffer temp(other);

    mySamples.swap(temp.mySamples);
    std::swap(myBuffer,   temp.myBuffer);
    std::swap(myDuration, temp.myDuration);

    return *this;
}


////////////////////////////////////////////////////////////
/// Update the internal buffer with the audio samples
////////////////////////////////////////////////////////////
bool SoundBuffer::Update(unsigned int channelsCount, unsigned int sampleRate)
{
    // Check parameters
    if (!channelsCount || !sampleRate || mySamples.empty())
        return false;

    // Find the good format according to the number of channels
    ALenum format = priv::AudioDevice::GetInstance().GetFormatFromChannelsCount(channelsCount);

    // Check if the format is valid
    if (format == 0)
    {
        std::cerr << "Unsupported number of channels (" << channelsCount << ")" << std::endl;
        return false;
    }

    // Fill the buffer
    ALsizei size = static_cast<ALsizei>(mySamples.size()) * sizeof(Int16);
    ALCheck(alBufferData(myBuffer, format, &mySamples[0], size, sampleRate));

    // Compute the duration
    myDuration = static_cast<float>(mySamples.size()) / sampleRate / channelsCount;

    return true;
}

} // namespace sf
