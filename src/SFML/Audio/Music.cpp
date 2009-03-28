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
#include <fstream>
#include <iostream>


namespace sf
{
////////////////////////////////////////////////////////////
/// Construct the music with a buffer size
////////////////////////////////////////////////////////////
Music::Music(std::size_t BufferSize) :
myFile    (NULL),
myDuration(0.f),
mySamples (BufferSize)
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
bool Music::OpenFromFile(const std::string& Filename)
{
    // First stop the music if it was already running
    Stop();

    // Create the sound file implementation, and open it in read mode
    delete myFile;
    myFile = priv::SoundFile::CreateRead(Filename);
    if (!myFile)
    {
        std::cerr << "Failed to open \"" << Filename << "\" for reading" << std::endl;
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
bool Music::OpenFromMemory(const char* Data, std::size_t SizeInBytes)
{
    // First stop the music if it was already running
    Stop();

    // Create the sound file implementation, and open it in read mode
    delete myFile;
    myFile = priv::SoundFile::CreateRead(Data, SizeInBytes);
    if (!myFile)
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
/// /see SoundStream::OnStart
////////////////////////////////////////////////////////////
bool Music::OnStart()
{
    return myFile && myFile->Restart();
}


////////////////////////////////////////////////////////////
/// /see SoundStream::OnGetData
////////////////////////////////////////////////////////////
bool Music::OnGetData(SoundStream::Chunk& Data)
{
    if (myFile)
    {
        // Fill the chunk parameters
        Data.Samples   = &mySamples[0];
        Data.NbSamples = myFile->Read(&mySamples[0], mySamples.size());

        // Check if we have reached the end of the audio file
        return Data.NbSamples == mySamples.size();
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
/// Get the sound duration
////////////////////////////////////////////////////////////
float Music::GetDuration() const
{
    return myDuration;
}

} // namespace sf
