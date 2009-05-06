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
#include <SFML/Audio/SoundFile.hpp>
#include <SFML/Audio/SoundFileDefault.hpp>
#include <SFML/Audio/SoundFileOgg.hpp>
#include <iostream>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Create a new sound from a file, for reading
////////////////////////////////////////////////////////////
SoundFile* SoundFile::CreateRead(const std::string& Filename)
{
    // Create the file according to its type
    SoundFile* File = NULL;
    if      (SoundFileOgg::IsFileSupported(Filename, true))     File = new SoundFileOgg;
    else if (SoundFileDefault::IsFileSupported(Filename, true)) File = new SoundFileDefault;

    // Open it for reading
    if (File)
    {
        std::size_t  SamplesCount;
        unsigned int ChannelsCount;
        unsigned int SampleRate;

        if (File->OpenRead(Filename, SamplesCount, ChannelsCount, SampleRate))
        {
            File->myFilename      = Filename;
            File->myData          = NULL;
            File->mySize          = 0;
            File->myNbSamples     = SamplesCount;
            File->myChannelsCount = ChannelsCount;
            File->mySampleRate    = SampleRate;
        }
        else
        {
            delete File;
            File = NULL;
        }
    }

    return File;
}


////////////////////////////////////////////////////////////
/// Create a new sound from a file in memory, for reading
////////////////////////////////////////////////////////////
SoundFile* SoundFile::CreateRead(const char* Data, std::size_t SizeInMemory)
{
    // Create the file according to its type
    SoundFile* File = NULL;
    if      (SoundFileOgg::IsFileSupported(Data, SizeInMemory))     File = new SoundFileOgg;
    else if (SoundFileDefault::IsFileSupported(Data, SizeInMemory)) File = new SoundFileDefault;

    // Open it for reading
    if (File)
    {
        std::size_t  SamplesCount;
        unsigned int ChannelsCount;
        unsigned int SampleRate;

        if (File->OpenRead(Data, SizeInMemory, SamplesCount, ChannelsCount, SampleRate))
        {
            File->myFilename      = "";
            File->myData          = Data;
            File->mySize          = SizeInMemory;
            File->myNbSamples     = SamplesCount;
            File->myChannelsCount = ChannelsCount;
            File->mySampleRate    = SampleRate;
        }
        else
        {
            delete File;
            File = NULL;
        }
    }

    return File;
}


////////////////////////////////////////////////////////////
/// Create a new sound from a file, for writing
////////////////////////////////////////////////////////////
SoundFile* SoundFile::CreateWrite(const std::string& Filename, unsigned int ChannelsCount, unsigned int SampleRate)
{
    // Create the file according to its type
    SoundFile* File = NULL;
    if      (SoundFileOgg::IsFileSupported(Filename, false))     File = new SoundFileOgg;
    else if (SoundFileDefault::IsFileSupported(Filename, false)) File = new SoundFileDefault;

    // Open it for writing
    if (File)
    {
        if (File->OpenWrite(Filename, ChannelsCount, SampleRate))
        {
            File->myFilename      = "";
            File->myData          = NULL;
            File->mySize          = 0;
            File->myNbSamples     = 0;
            File->myChannelsCount = ChannelsCount;
            File->mySampleRate    = SampleRate;
        }
        else
        {
            delete File;
            File = NULL;
        }
    }

    return File;
}


////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
SoundFile::SoundFile() :
myNbSamples    (0),
myChannelsCount(0),
mySampleRate   (0)
{

}


////////////////////////////////////////////////////////////
/// Virtual destructor
////////////////////////////////////////////////////////////
SoundFile::~SoundFile()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
/// Get the total number of samples in the file
////////////////////////////////////////////////////////////
std::size_t SoundFile::GetSamplesCount() const
{
    return myNbSamples;
}


////////////////////////////////////////////////////////////
/// Get the number of channels used by the sound
////////////////////////////////////////////////////////////
unsigned int SoundFile::GetChannelsCount() const
{
    return myChannelsCount;
}


////////////////////////////////////////////////////////////
/// Get the sample rate of the sound
////////////////////////////////////////////////////////////
unsigned int SoundFile::GetSampleRate() const
{
    return mySampleRate;
}


////////////////////////////////////////////////////////////
/// Restart the sound from the beginning
////////////////////////////////////////////////////////////
bool SoundFile::Restart()
{
    if (myData)
    {
        // Reopen from memory
        return OpenRead(myData, mySize, myNbSamples, myChannelsCount, mySampleRate);
    }
    else if (myFilename != "")
    {
        // Reopen from file
        return OpenRead(myFilename, myNbSamples, myChannelsCount, mySampleRate);
    }
    else
    {
        // Trying to reopen a file opened in write mode... error
        std::cerr << "Warning : trying to restart a sound opened in write mode, which is not allowed" << std::endl;
        return false;
    }
}


////////////////////////////////////////////////////////////
/// Open the sound file for reading
////////////////////////////////////////////////////////////
bool SoundFile::OpenRead(const std::string& Filename, std::size_t&, unsigned int&, unsigned int&)
{
    std::cerr << "Failed to open sound file \"" << Filename << "\", format is not supported by SFML" << std::endl;

    return false;
}


////////////////////////////////////////////////////////////
/// Open the sound file in memory for reading
////////////////////////////////////////////////////////////
bool SoundFile::OpenRead(const char*, std::size_t, std::size_t&, unsigned int&, unsigned int&)
{
    std::cerr << "Failed to open sound file from memory, format is not supported by SFML" << std::endl;

    return false;
}


////////////////////////////////////////////////////////////
/// Open the sound file for writing
////////////////////////////////////////////////////////////
bool SoundFile::OpenWrite(const std::string& Filename, unsigned int, unsigned int)
{
    std::cerr << "Failed to open sound file \"" << Filename << "\", format is not supported by SFML" << std::endl;

    return false;
}


////////////////////////////////////////////////////////////
/// Read samples from the loaded sound
////////////////////////////////////////////////////////////
std::size_t SoundFile::Read(Int16*, std::size_t)
{
    std::cerr << "Failed to read from sound file (not supported)" << std::endl;

    return 0;
}


////////////////////////////////////////////////////////////
/// Write samples to the file
////////////////////////////////////////////////////////////
void SoundFile::Write(const Int16*, std::size_t)
{
    std::cerr << "Failed to write to sound file (not supported)" << std::endl;
}

} // namespace priv

} // namespace sf
