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
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/ALCheck.hpp>
#include <SFML/Audio/SoundFile.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Err.hpp>
#include <fstream>


namespace sf
{
////////////////////////////////////////////////////////////
Music::Music() :
myFile    (new priv::SoundFile),
myDuration()
{

}


////////////////////////////////////////////////////////////
Music::~Music()
{
    // We must stop before destroying the file :)
    Stop();

    delete myFile;
}


////////////////////////////////////////////////////////////
bool Music::OpenFromFile(const std::string& filename)
{
    // First stop the music if it was already running
    Stop();

    // Open the underlying sound file
    if (!myFile->OpenRead(filename))
        return false;

    // Perform common initializations
    Initialize();

    return true;
}


////////////////////////////////////////////////////////////
bool Music::OpenFromMemory(const void* data, std::size_t sizeInBytes)
{
    // First stop the music if it was already running
    Stop();

    // Open the underlying sound file
    if (!myFile->OpenRead(data, sizeInBytes))
        return false;

    // Perform common initializations
    Initialize();

    return true;
}


////////////////////////////////////////////////////////////
bool Music::OpenFromStream(InputStream& stream)
{
    // First stop the music if it was already running
    Stop();

    // Open the underlying sound file
    if (!myFile->OpenRead(stream))
        return false;

    // Perform common initializations
    Initialize();

    return true;
}


////////////////////////////////////////////////////////////
Time Music::GetDuration() const
{
    return myDuration;
}


////////////////////////////////////////////////////////////
bool Music::OnGetData(SoundStream::Chunk& data)
{
    Lock lock(myMutex);

    // Fill the chunk parameters
    data.Samples     = &mySamples[0];
    data.SampleCount = myFile->Read(&mySamples[0], mySamples.size());

    // Check if we have reached the end of the audio file
    return data.SampleCount == mySamples.size();
}


////////////////////////////////////////////////////////////
void Music::OnSeek(Time timeOffset)
{
    Lock lock(myMutex);

    myFile->Seek(timeOffset);
}


////////////////////////////////////////////////////////////
void Music::Initialize()
{
    // Compute the music duration
    myDuration = Seconds(static_cast<float>(myFile->GetSampleCount()) / myFile->GetSampleRate() / myFile->GetChannelCount());

    // Resize the internal buffer so that it can contain 1 second of audio samples
    mySamples.resize(myFile->GetSampleRate() * myFile->GetChannelCount());

    // Initialize the stream
    SoundStream::Initialize(myFile->GetChannelCount(), myFile->GetSampleRate());
}

} // namespace sf
