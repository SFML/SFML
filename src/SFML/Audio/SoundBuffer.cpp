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
SoundBuffer::SoundBuffer(const SoundBuffer& Copy) :
AudioResource        (Copy),
Resource<SoundBuffer>(Copy),
myBuffer             (0),
mySamples            (Copy.mySamples),
myDuration           (Copy.myDuration)
{
    // Create the buffer
    ALCheck(alGenBuffers(1, &myBuffer));

    // Update the internal buffer with the new samples
    Update(Copy.GetChannelsCount(), Copy.GetSampleRate());
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
bool SoundBuffer::LoadFromFile(const std::string& Filename)
{
    // Create the sound file
    std::auto_ptr<priv::SoundFile> File(priv::SoundFile::CreateRead(Filename));

    // Open the sound file
    if (File.get())
    {
        // Get the sound parameters
        std::size_t  NbSamples     = File->GetSamplesCount();
        unsigned int ChannelsCount = File->GetChannelsCount();
        unsigned int SampleRate    = File->GetSampleRate();

        // Read the samples from the opened file
        mySamples.resize(NbSamples);
        if (File->Read(&mySamples[0], NbSamples) == NbSamples)
        {
            // Update the internal buffer with the new samples
            return Update(ChannelsCount, SampleRate);
        }
        else
        {
            // Error...
            std::cerr << "Failed to read audio data from file \"" << Filename << "\"" << std::endl;

            return false;
        }
    }
    else
    {
        // Error...
        std::cerr << "Failed to load sound buffer from file \"" << Filename << "\"" << std::endl;

        return false;
    }
}


////////////////////////////////////////////////////////////
/// Load the sound buffer from a file in memory
////////////////////////////////////////////////////////////
bool SoundBuffer::LoadFromMemory(const char* Data, std::size_t SizeInBytes)
{
    // Create the sound file
    std::auto_ptr<priv::SoundFile> File(priv::SoundFile::CreateRead(Data, SizeInBytes));

    // Open the sound file
    if (File.get())
    {
        // Get the sound parameters
        std::size_t  NbSamples     = File->GetSamplesCount();
        unsigned int ChannelsCount = File->GetChannelsCount();
        unsigned int SampleRate    = File->GetSampleRate();

        // Read the samples from the opened file
        mySamples.resize(NbSamples);
        if (File->Read(&mySamples[0], NbSamples) == NbSamples)
        {
            // Update the internal buffer with the new samples
            return Update(ChannelsCount, SampleRate);
        }
        else
        {
            // Error...
            std::cerr << "Failed to read audio data from file in memory" << std::endl;

            return false;
        }
    }
    else
    {
        // Error...
        std::cerr << "Failed to load sound buffer from file in memory" << std::endl;

        return false;
    }
}


////////////////////////////////////////////////////////////
/// Load the sound buffer from an array of samples  - assumed format for
/// samples is 16 bits signed integer
////////////////////////////////////////////////////////////
bool SoundBuffer::LoadFromSamples(const Int16* Samples, std::size_t SamplesCount, unsigned int ChannelsCount, unsigned int SampleRate)
{
    if (Samples && SamplesCount && ChannelsCount && SampleRate)
    {
        // Copy the new audio samples
        mySamples.assign(Samples, Samples + SamplesCount);

        // Update the internal buffer with the new samples
        return Update(ChannelsCount, SampleRate);
    }
    else
    {
        // Error...
        std::cerr << "Failed to load sound buffer from memory ("
                  << "Samples : "        << Samples       << ", "
                  << "Samples count : "  << SamplesCount  << ", "
                  << "Channels count : " << ChannelsCount << ", "
                  << "Sample rate : "    << SampleRate    << ")"
                  << std::endl;

        return false;
    }
}


////////////////////////////////////////////////////////////
/// Save the sound buffer to a file
////////////////////////////////////////////////////////////
bool SoundBuffer::SaveToFile(const std::string& Filename) const
{
    // Create the sound file in write mode
    std::auto_ptr<priv::SoundFile> File(priv::SoundFile::CreateWrite(Filename, GetChannelsCount(), GetSampleRate()));
    if (File.get())
    {
        // Write the samples to the opened file
        File->Write(&mySamples[0], mySamples.size());

        return true;
    }
    else
    {
        // Error...
        std::cerr << "Failed to save sound buffer to file \"" << Filename << "\"" << std::endl;

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
    ALint SampleRate;
    ALCheck(alGetBufferi(myBuffer, AL_FREQUENCY, &SampleRate));

    return SampleRate;
}


////////////////////////////////////////////////////////////
/// Return the number of channels (1 = mono, 2 = stereo, ...)
////////////////////////////////////////////////////////////
unsigned int SoundBuffer::GetChannelsCount() const
{
    ALint ChannelsCount;
    ALCheck(alGetBufferi(myBuffer, AL_CHANNELS, &ChannelsCount));

    return ChannelsCount;
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
SoundBuffer& SoundBuffer::operator =(const SoundBuffer& Other)
{
    SoundBuffer Temp(Other);

    mySamples.swap(Temp.mySamples);
    std::swap(myBuffer,   Temp.myBuffer);
    std::swap(myDuration, Temp.myDuration);

    return *this;
}


////////////////////////////////////////////////////////////
/// Update the internal buffer with the audio samples
////////////////////////////////////////////////////////////
bool SoundBuffer::Update(unsigned int ChannelsCount, unsigned int SampleRate)
{
    // Check parameters
    if (!SampleRate || !ChannelsCount || mySamples.empty())
        return false;

    // Find the good format according to the number of channels
    ALenum Format = priv::AudioDevice::GetInstance().GetFormatFromChannelsCount(ChannelsCount);

    // Check if the format is valid
    if (Format == 0)
    {
        std::cerr << "Unsupported number of channels (" << ChannelsCount << ")" << std::endl;
        return false;
    }

    // Fill the buffer
    ALsizei Size = static_cast<ALsizei>(mySamples.size()) * sizeof(Int16);
    ALCheck(alBufferData(myBuffer, Format, &mySamples[0], Size, SampleRate));

    // Compute the duration
    myDuration = static_cast<float>(mySamples.size()) / SampleRate / ChannelsCount;

    return true;
}

} // namespace sf
