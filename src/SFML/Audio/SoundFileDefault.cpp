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
#include <SFML/Audio/SoundFileDefault.hpp>
#include <iostream>
#include <string.h>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
SoundFileDefault::SoundFileDefault() :
myFile(NULL)
{

}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
SoundFileDefault::~SoundFileDefault()
{
    if (myFile)
        sf_close(myFile);
}


////////////////////////////////////////////////////////////
/// Check if a given file is supported by this loader
////////////////////////////////////////////////////////////
bool SoundFileDefault::IsFileSupported(const std::string& Filename, bool Read)
{
    if (Read)
    {
        // Open the sound file
        SF_INFO FileInfos;
        SNDFILE* File = sf_open(Filename.c_str(), SFM_READ, &FileInfos);

        if (File)
        {
            sf_close(File);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        // Check the extension
        return GetFormatFromFilename(Filename) != -1;
    }
}


////////////////////////////////////////////////////////////
/// Check if a given file in memory is supported by this loader
////////////////////////////////////////////////////////////
bool SoundFileDefault::IsFileSupported(const char* Data, std::size_t SizeInBytes)
{
    // Define the I/O custom functions for reading from memory
    SF_VIRTUAL_IO VirtualIO;
    VirtualIO.get_filelen = &SoundFileDefault::MemoryGetLength;
    VirtualIO.read        = &SoundFileDefault::MemoryRead;
    VirtualIO.seek        = &SoundFileDefault::MemorySeek;
    VirtualIO.tell        = &SoundFileDefault::MemoryTell;
    VirtualIO.write       = &SoundFileDefault::MemoryWrite;

    // Initialize the memory data
    MemoryInfos Memory;
    Memory.DataStart = Data;
    Memory.DataPtr   = Data;
    Memory.TotalSize = SizeInBytes;

    // Open the sound file
    SF_INFO FileInfos;
    SNDFILE* File = sf_open_virtual(&VirtualIO, SFM_READ, &FileInfos, &Memory);

    if (File)
    {
        sf_close(File);
        return true;
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
/// Open the sound file for reading
////////////////////////////////////////////////////////////
bool SoundFileDefault::OpenRead(const std::string& Filename, std::size_t& NbSamples, unsigned int& ChannelsCount, unsigned int& SampleRate)
{
    // If the file is already opened, first close it
    if (myFile)
        sf_close(myFile);

    // Open the sound file
    SF_INFO FileInfos;
    myFile = sf_open(Filename.c_str(), SFM_READ, &FileInfos);
    if (!myFile)
    {
        std::cerr << "Failed to read sound file \"" << Filename << "\"" << std::endl;
        return false;
    }

    // Set the sound parameters
    ChannelsCount = FileInfos.channels;
    SampleRate    = FileInfos.samplerate;
    NbSamples     = static_cast<std::size_t>(FileInfos.frames) * ChannelsCount;

    return true;
}


////////////////////////////////////////////////////////////
/// /see sf::SoundFile::OpenRead
////////////////////////////////////////////////////////////
bool SoundFileDefault::OpenRead(const char* Data, std::size_t SizeInBytes, std::size_t& NbSamples, unsigned int& ChannelsCount, unsigned int& SampleRate)
{
    // If the file is already opened, first close it
    if (myFile)
        sf_close(myFile);

    // Define the I/O custom functions for reading from memory
    SF_VIRTUAL_IO VirtualIO;
    VirtualIO.get_filelen = &SoundFileDefault::MemoryGetLength;
    VirtualIO.read        = &SoundFileDefault::MemoryRead;
    VirtualIO.seek        = &SoundFileDefault::MemorySeek;
    VirtualIO.tell        = &SoundFileDefault::MemoryTell;
    VirtualIO.write       = &SoundFileDefault::MemoryWrite;

    // Initialize the memory data
    myMemory.DataStart = Data;
    myMemory.DataPtr   = Data;
    myMemory.TotalSize = SizeInBytes;

    // Open the sound file
    SF_INFO FileInfos;
    myFile = sf_open_virtual(&VirtualIO, SFM_READ, &FileInfos, &myMemory);
    if (!myFile)
    {
        std::cerr << "Failed to read sound file from memory" << std::endl;
        return false;
    }

    // Set the sound parameters
    ChannelsCount = FileInfos.channels;
    SampleRate    = FileInfos.samplerate;
    NbSamples     = static_cast<std::size_t>(FileInfos.frames) * ChannelsCount;

    return true;
}


////////////////////////////////////////////////////////////
/// Open the sound file for writing
////////////////////////////////////////////////////////////
bool SoundFileDefault::OpenWrite(const std::string& Filename, unsigned int ChannelsCount, unsigned int SampleRate)
{
    // If the file is already opened, first close it
    if (myFile)
        sf_close(myFile);

    // Find the right format according to the file extension
    int Format = GetFormatFromFilename(Filename);
    if (Format == -1)
    {
        // Error : unrecognized extension
        std::cerr << "Failed to create sound file \"" << Filename << "\" : unknown format" << std::endl;
        return false;
    }

    // Fill the sound infos with parameters
    SF_INFO FileInfos;
    FileInfos.channels   = ChannelsCount;
    FileInfos.samplerate = SampleRate;
    FileInfos.format     = Format | SF_FORMAT_PCM_16;

    // Open the sound file for writing
    myFile = sf_open(Filename.c_str(), SFM_WRITE, &FileInfos);
    if (!myFile)
    {
        std::cerr << "Failed to create sound file \"" << Filename << "\"" << std::endl;
        return false;
    }

    return true;
}


////////////////////////////////////////////////////////////
/// Read samples from the loaded sound
////////////////////////////////////////////////////////////
std::size_t SoundFileDefault::Read(Int16* Data, std::size_t NbSamples)
{
    if (myFile && Data && NbSamples)
        return static_cast<std::size_t>(sf_read_short(myFile, Data, NbSamples));
    else
        return 0;
}


////////////////////////////////////////////////////////////
/// Write samples to the file
////////////////////////////////////////////////////////////
void SoundFileDefault::Write(const Int16* Data, std::size_t NbSamples)
{
    if (myFile && Data && NbSamples)
        sf_write_short(myFile, Data, NbSamples);
}


////////////////////////////////////////////////////////////
/// Get the internal format of an audio file according to
/// its filename extension
////////////////////////////////////////////////////////////
int SoundFileDefault::GetFormatFromFilename(const std::string& Filename)
{
    // Extract the extension
    std::string Ext = "wav";
    std::string::size_type Pos = Filename.find_last_of(".");
    if (Pos != std::string::npos)
        Ext = Filename.substr(Pos + 1);

    // Match every supported extension with its format constant
    if (Ext == "wav"  || Ext == "WAV" ) return SF_FORMAT_WAV;
    if (Ext == "aif"  || Ext == "AIF" ) return SF_FORMAT_AIFF;
    if (Ext == "aiff" || Ext == "AIFF") return SF_FORMAT_AIFF;
    if (Ext == "au"   || Ext == "AU"  ) return SF_FORMAT_AU;
    if (Ext == "raw"  || Ext == "RAW" ) return SF_FORMAT_RAW;
    if (Ext == "paf"  || Ext == "PAF" ) return SF_FORMAT_PAF;
    if (Ext == "svx"  || Ext == "SVX" ) return SF_FORMAT_SVX;
    if (Ext == "voc"  || Ext == "VOC" ) return SF_FORMAT_VOC;
    if (Ext == "sf"   || Ext == "SF"  ) return SF_FORMAT_IRCAM;
    if (Ext == "w64"  || Ext == "W64" ) return SF_FORMAT_W64;
    if (Ext == "mat4" || Ext == "MAT4") return SF_FORMAT_MAT4;
    if (Ext == "mat5" || Ext == "MAT5") return SF_FORMAT_MAT5;
    if (Ext == "pvf"  || Ext == "PVF" ) return SF_FORMAT_PVF;
    if (Ext == "htk"  || Ext == "HTK" ) return SF_FORMAT_HTK;
    if (Ext == "caf"  || Ext == "CAF" ) return SF_FORMAT_CAF;
    if (Ext == "nist" || Ext == "NIST") return SF_FORMAT_NIST; // SUPPORTED ?
    if (Ext == "sds"  || Ext == "SDS" ) return SF_FORMAT_SDS;  // SUPPORTED ?
    if (Ext == "avr"  || Ext == "AVR" ) return SF_FORMAT_AVR;  // SUPPORTED ?
    if (Ext == "sd2"  || Ext == "SD2" ) return SF_FORMAT_SD2;  // SUPPORTED ?
    if (Ext == "flac" || Ext == "FLAC") return SF_FORMAT_FLAC; // SUPPORTED ?

    return -1;
}


////////////////////////////////////////////////////////////
/// Functions for implementing custom read and write to memory files
///
////////////////////////////////////////////////////////////
sf_count_t SoundFileDefault::MemoryGetLength(void* UserData)
{
    MemoryInfos* Memory = static_cast<MemoryInfos*>(UserData);

    return Memory->TotalSize;
}
sf_count_t SoundFileDefault::MemoryRead(void* Ptr, sf_count_t Count, void* UserData)
{
    MemoryInfos* Memory = static_cast<MemoryInfos*>(UserData);

    sf_count_t Position = Memory->DataPtr - Memory->DataStart;
    if (Position + Count >= Memory->TotalSize)
        Count = Memory->TotalSize - Position;

    memcpy(Ptr, Memory->DataPtr, static_cast<std::size_t>(Count));

    Memory->DataPtr += Count;

    return Count;
}
sf_count_t SoundFileDefault::MemorySeek(sf_count_t Offset, int Whence, void* UserData)
{
    MemoryInfos* Memory = static_cast<MemoryInfos*>(UserData);

    sf_count_t Position = 0;
    switch (Whence)
    {
        case SEEK_SET :
            Position = Offset;
            break;
        case SEEK_CUR :
            Position = Memory->DataPtr - Memory->DataStart + Offset;
            break;
        case SEEK_END :
            Position = Memory->TotalSize - Offset;
            break;
        default :
            Position = 0;
            break;
    }

    if (Position >= Memory->TotalSize)
        Position = Memory->TotalSize - 1;
    else if (Position < 0)
        Position = 0;

    Memory->DataPtr = Memory->DataStart + Position;

    return Position;
}
sf_count_t SoundFileDefault::MemoryTell(void* UserData)
{
    MemoryInfos* Memory = static_cast<MemoryInfos*>(UserData);

    return Memory->DataPtr - Memory->DataStart;
}
sf_count_t SoundFileDefault::MemoryWrite(const void*, sf_count_t, void*)
{
    return 0;
}


} // namespace priv

} // namespace sf
