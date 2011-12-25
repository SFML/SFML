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
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/AudioDevice.hpp>
#include <SFML/Audio/ALCheck.hpp>
#include <SFML/System/Err.hpp>
#include <memory>


namespace sf
{
////////////////////////////////////////////////////////////
SoundBuffer::SoundBuffer() :
myBuffer  (0),
myDuration(0)
{
    priv::EnsureALInit();

    // Create the buffer
    ALCheck(alGenBuffers(1, &myBuffer));
}


////////////////////////////////////////////////////////////
SoundBuffer::SoundBuffer(const SoundBuffer& copy) :
myBuffer  (0),
mySamples (copy.mySamples),
myDuration(copy.myDuration),
mySounds  () // don't copy the attached sounds
{
    // Create the buffer
    ALCheck(alGenBuffers(1, &myBuffer));

    // Update the internal buffer with the new samples
    Update(copy.GetChannelCount(), copy.GetSampleRate());
}


////////////////////////////////////////////////////////////
SoundBuffer::~SoundBuffer()
{
    // First detach the buffer from the sounds that use it (to avoid OpenAL errors)
    for (SoundList::const_iterator it = mySounds.begin(); it != mySounds.end(); ++it)
        (*it)->ResetBuffer();

    // Destroy the buffer
    if (myBuffer)
        ALCheck(alDeleteBuffers(1, &myBuffer));
}


////////////////////////////////////////////////////////////
bool SoundBuffer::LoadFromFile(const std::string& filename)
{
    priv::SoundFile file;
    if (file.OpenRead(filename))
        return Initialize(file);
    else
        return false;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::LoadFromMemory(const void* data, std::size_t sizeInBytes)
{
    priv::SoundFile file;
    if (file.OpenRead(data, sizeInBytes))
        return Initialize(file);
    else
        return false;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::LoadFromStream(InputStream& stream)
{
    priv::SoundFile file;
    if (file.OpenRead(stream))
        return Initialize(file);
    else
        return false;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::LoadFromSamples(const Int16* samples, std::size_t sampleCount, unsigned int channelCount, unsigned int sampleRate)
{
    if (samples && sampleCount && channelCount && sampleRate)
    {
        // Copy the new audio samples
        mySamples.assign(samples, samples + sampleCount);

        // Update the internal buffer with the new samples
        return Update(channelCount, sampleRate);
    }
    else
    {
        // Error...
        Err() << "Failed to load sound buffer from samples ("
              << "array: "      << samples      << ", "
              << "count: "      << sampleCount  << ", "
              << "channels: "   << channelCount << ", "
              << "samplerate: " << sampleRate   << ")"
              << std::endl;

        return false;
    }
}


////////////////////////////////////////////////////////////
bool SoundBuffer::SaveToFile(const std::string& filename) const
{
    // Create the sound file in write mode
    priv::SoundFile file;
    if (file.OpenWrite(filename, GetChannelCount(), GetSampleRate()))
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
const Int16* SoundBuffer::GetSamples() const
{
    return mySamples.empty() ? NULL : &mySamples[0];
}


////////////////////////////////////////////////////////////
std::size_t SoundBuffer::GetSampleCount() const
{
    return mySamples.size();
}


////////////////////////////////////////////////////////////
unsigned int SoundBuffer::GetSampleRate() const
{
    ALint sampleRate;
    ALCheck(alGetBufferi(myBuffer, AL_FREQUENCY, &sampleRate));

    return sampleRate;
}


////////////////////////////////////////////////////////////
unsigned int SoundBuffer::GetChannelCount() const
{
    ALint channelCount;
    ALCheck(alGetBufferi(myBuffer, AL_CHANNELS, &channelCount));

    return channelCount;
}


////////////////////////////////////////////////////////////
Uint32 SoundBuffer::GetDuration() const
{
    return myDuration;
}


////////////////////////////////////////////////////////////
SoundBuffer& SoundBuffer::operator =(const SoundBuffer& right)
{
    SoundBuffer temp(right);

    std::swap(mySamples,  temp.mySamples);
    std::swap(myBuffer,   temp.myBuffer);
    std::swap(myDuration, temp.myDuration);
    std::swap(mySounds,   temp.mySounds); // swap sounds too, so that they are detached when temp is destroyed

    return *this;
}


////////////////////////////////////////////////////////////
bool SoundBuffer::Initialize(priv::SoundFile& file)
{
    // Retrieve the sound parameters
    std::size_t  nbSamples    = file.GetSampleCount();
    unsigned int channelCount = file.GetChannelCount();
    unsigned int sampleRate   = file.GetSampleRate();

    // Read the samples from the provided file
    mySamples.resize(nbSamples);
    if (file.Read(&mySamples[0], nbSamples) == nbSamples)
    {
        // Update the internal buffer with the new samples
        return Update(channelCount, sampleRate);
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
bool SoundBuffer::Update(unsigned int channelCount, unsigned int sampleRate)
{
    // Check parameters
    if (!channelCount || !sampleRate || mySamples.empty())
        return false;

    // Find the good format according to the number of channels
    ALenum format = priv::AudioDevice::GetFormatFromChannelCount(channelCount);

    // Check if the format is valid
    if (format == 0)
    {
        Err() << "Failed to load sound buffer (unsupported number of channels: " << channelCount << ")" << std::endl;
        return false;
    }

    // Fill the buffer
    ALsizei size = static_cast<ALsizei>(mySamples.size()) * sizeof(Int16);
    ALCheck(alBufferData(myBuffer, format, &mySamples[0], size, sampleRate));

    // Compute the duration
    myDuration = static_cast<Uint32>(1000 * mySamples.size() / sampleRate / channelCount);

    return true;
}


////////////////////////////////////////////////////////////
void SoundBuffer::AttachSound(Sound* sound) const
{
    mySounds.insert(sound);
}


////////////////////////////////////////////////////////////
void SoundBuffer::DetachSound(Sound* sound) const
{
    mySounds.erase(sound);
}

} // namespace sf
