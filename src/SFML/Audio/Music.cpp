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
#include <SFML/Audio/OpenAL.hpp>
#include <SFML/Audio/SoundFile.hpp>
#include <SFML/System/Lock.hpp>
#include <fstream>
#include <iostream>


namespace sf
{
////////////////////////////////////////////////////////////
/// Construct the music with a buffer size
////////////////////////////////////////////////////////////
Music::Music(std::size_t bufferSize) :
myFile    (new priv::SoundFile),
myDuration(0.f),
mySamples (bufferSize)
{

}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
Music::~Music()
{
    // We must stop before destroying the file :)
    Stop();

    delete myFile;
}


////////////////////////////////////////////////////////////
/// Open a music file (doesn't play it -- call Play() for that)
////////////////////////////////////////////////////////////
bool Music::OpenFromFile(const std::string& filename)
{
    // First stop the music if it was already running
    Stop();

    // Create the sound file implementation, and open it in read mode
    if (!myFile->OpenRead(filename))
    {
        std::cerr << "Failed to open \"" << filename << "\" for reading" << std::endl;
        return false;
    }

    // Compute the duration
    myDuration = static_cast<float>(myFile->GetSamplesCount()) / myFile->GetSampleRate() / myFile->GetChannelsCount();

    // Initialize the stream
    Initialize(myFile->GetChannelsCount(), myFile->GetSampleRate());

    return true;
}


////////////////////////////////////////////////////////////
/// Open a music file from memory (doesn't play it -- call Play() for that)
////////////////////////////////////////////////////////////
bool Music::OpenFromMemory(const char* data, std::size_t sizeInBytes)
{
    // First stop the music if it was already running
    Stop();

    // Create the sound file implementation, and open it in read mode
    if (!myFile->OpenRead(data, sizeInBytes))
    {
        std::cerr << "Failed to open music from memory for reading" << std::endl;
        return false;
    }

    // Compute the duration
    myDuration = static_cast<float>(myFile->GetSamplesCount()) / myFile->GetSampleRate();

    // Initialize the stream
    Initialize(myFile->GetChannelsCount(), myFile->GetSampleRate());

    return true;
}


////////////////////////////////////////////////////////////
/// Get the sound duration
////////////////////////////////////////////////////////////
float Music::GetDuration() const
{
    return myDuration;
}


////////////////////////////////////////////////////////////
/// /see SoundStream::OnGetData
////////////////////////////////////////////////////////////
bool Music::OnGetData(SoundStream::Chunk& data)
{
    sf::Lock lock(myMutex);

    // Fill the chunk parameters
    data.Samples   = &mySamples[0];
    data.NbSamples = myFile->Read(&mySamples[0], mySamples.size());

    // Check if we have reached the end of the audio file
    return data.NbSamples == mySamples.size();
}


////////////////////////////////////////////////////////////
/// /see SoundStream::OnSeek
////////////////////////////////////////////////////////////
void Music::OnSeek(float timeOffset)
{
    sf::Lock lock(myMutex);

    myFile->Seek(timeOffset);
}

} // namespace sf
