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
#include <iostream>
#include <string.h>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
SoundFile::SoundFile() :
myFile         (NULL),
myNbSamples    (0),
myChannelsCount(0),
mySampleRate   (0)
{

}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
SoundFile::~SoundFile()
{
    if (myFile)
        sf_close(myFile);
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
/// Open the sound file for reading
////////////////////////////////////////////////////////////
bool SoundFile::OpenRead(const std::string& Filename)
{
    // If the file is already opened, first close it
    if (myFile)
        sf_close(myFile);

    // Open the sound file
    SF_INFO FileInfos;
    myFile = sf_open(Filename.c_str(), SFM_READ, &FileInfos);
    if (!myFile)
    {
        std::cerr << "Failed to read sound file \"" << Filename << "\" (" << sf_strerror(myFile) << ")" << std::endl;
        return false;
    }

    // Set the sound parameters
    myChannelsCount = FileInfos.channels;
    mySampleRate    = FileInfos.samplerate;
    myNbSamples     = static_cast<std::size_t>(FileInfos.frames) * myChannelsCount;

    return true;
}


////////////////////////////////////////////////////////////
/// Open the sound file in memory for reading
////////////////////////////////////////////////////////////
bool SoundFile::OpenRead(const char* Data, std::size_t SizeInBytes)
{
    // If the file is already opened, first close it
    if (myFile)
        sf_close(myFile);

    // Define the I/O custom functions for reading from memory
    SF_VIRTUAL_IO VirtualIO;
    VirtualIO.get_filelen = &SoundFile::MemoryGetLength;
    VirtualIO.read        = &SoundFile::MemoryRead;
    VirtualIO.seek        = &SoundFile::MemorySeek;
    VirtualIO.tell        = &SoundFile::MemoryTell;
    VirtualIO.write       = &SoundFile::MemoryWrite;

    // Initialize the memory data
    myMemory.DataStart = Data;
    myMemory.DataPtr   = Data;
    myMemory.TotalSize = SizeInBytes;

    // Open the sound file
    SF_INFO FileInfos;
    myFile = sf_open_virtual(&VirtualIO, SFM_READ, &FileInfos, &myMemory);
    if (!myFile)
    {
        std::cerr << "Failed to read sound file from memory (" << sf_strerror(myFile) << ")" << std::endl;
        return false;
    }

    // Set the sound parameters
    myChannelsCount = FileInfos.channels;
    mySampleRate    = FileInfos.samplerate;
    myNbSamples     = static_cast<std::size_t>(FileInfos.frames) * myChannelsCount;

    return true;
}


////////////////////////////////////////////////////////////
/// Open the sound file for writing
////////////////////////////////////////////////////////////
bool SoundFile::OpenWrite(const std::string& Filename, unsigned int ChannelsCount, unsigned int SampleRate)
{
    // If the file is already opened, first close it
    if (myFile)
        sf_close(myFile);

    // Find the right format according to the file extension
    int Format = GetFormatFromFilename(Filename);
    if (Format == -1)
    {
        // Error : unrecognized extension
        std::cerr << "Failed to create sound file \"" << Filename << "\" (unknown format)" << std::endl;
        return false;
    }

    // Fill the sound infos with parameters
    SF_INFO FileInfos;
    FileInfos.channels   = ChannelsCount;
    FileInfos.samplerate = SampleRate;
    FileInfos.format     = Format | (Format == SF_FORMAT_OGG ? SF_FORMAT_VORBIS : SF_FORMAT_PCM_16);

    // Open the sound file for writing
    myFile = sf_open(Filename.c_str(), SFM_WRITE, &FileInfos);
    if (!myFile)
    {
        std::cerr << "Failed to create sound file \"" << Filename << "\" (" << sf_strerror(myFile) << ")" << std::endl;
        return false;
    }

    // Set the sound parameters
    myChannelsCount = ChannelsCount;
    mySampleRate    = SampleRate;
    myNbSamples     = 0;

    return true;
}


////////////////////////////////////////////////////////////
/// Read samples from the loaded sound
////////////////////////////////////////////////////////////
std::size_t SoundFile::Read(Int16* Data, std::size_t NbSamples)
{
    if (myFile && Data && NbSamples)
        return static_cast<std::size_t>(sf_read_short(myFile, Data, NbSamples));
    else
        return 0;
}


////////////////////////////////////////////////////////////
/// Write samples to the file
////////////////////////////////////////////////////////////
void SoundFile::Write(const Int16* Data, std::size_t NbSamples)
{
    if (myFile && Data && NbSamples)
    {
        // Write small chunks instead of everything at once,
        // to avoid a stack overflow in libsndfile (happens only with OGG format)
        while (NbSamples > 0)
        {
            std::size_t Count = NbSamples > 10000 ? 10000 : NbSamples;
            sf_write_short(myFile, Data, Count);
            Data += Count;
            NbSamples -= Count;
        }
    }
}


////////////////////////////////////////////////////////////
/// Move the current reading position in the file
////////////////////////////////////////////////////////////
void SoundFile::Seek(float TimeOffset)
{
    if (myFile)
    {
        sf_count_t FrameOffset = static_cast<sf_count_t>(TimeOffset * mySampleRate);
        sf_seek(myFile, FrameOffset, SEEK_SET);
    }
}


////////////////////////////////////////////////////////////
/// Get the internal format of an audio file according to
/// its filename extension
////////////////////////////////////////////////////////////
int SoundFile::GetFormatFromFilename(const std::string& Filename)
{
    // Extract the extension
    std::string Ext = "wav";
    std::string::size_type Pos = Filename.find_last_of(".");
    if (Pos != std::string::npos)
        Ext = Filename.substr(Pos + 1);

    // Match every supported extension with its format constant
    if (Ext == "wav"   || Ext == "WAV" )  return SF_FORMAT_WAV;
    if (Ext == "aif"   || Ext == "AIF" )  return SF_FORMAT_AIFF;
    if (Ext == "aiff"  || Ext == "AIFF")  return SF_FORMAT_AIFF;
    if (Ext == "au"    || Ext == "AU"  )  return SF_FORMAT_AU;
    if (Ext == "raw"   || Ext == "RAW" )  return SF_FORMAT_RAW;
    if (Ext == "paf"   || Ext == "PAF" )  return SF_FORMAT_PAF;
    if (Ext == "svx"   || Ext == "SVX" )  return SF_FORMAT_SVX;
    if (Ext == "nist"  || Ext == "NIST")  return SF_FORMAT_NIST;
    if (Ext == "voc"   || Ext == "VOC" )  return SF_FORMAT_VOC;
    if (Ext == "sf"    || Ext == "SF"  )  return SF_FORMAT_IRCAM;
    if (Ext == "w64"   || Ext == "W64" )  return SF_FORMAT_W64;
    if (Ext == "mat4"  || Ext == "MAT4")  return SF_FORMAT_MAT4;
    if (Ext == "mat5"  || Ext == "MAT5")  return SF_FORMAT_MAT5;
    if (Ext == "pvf"   || Ext == "PVF" )  return SF_FORMAT_PVF;
    if (Ext == "xi"    || Ext == "XI" )   return SF_FORMAT_XI;
    if (Ext == "htk"   || Ext == "HTK" )  return SF_FORMAT_HTK;
    if (Ext == "sds"   || Ext == "SDS" )  return SF_FORMAT_SDS;
    if (Ext == "avr"   || Ext == "AVR" )  return SF_FORMAT_AVR;
    if (Ext == "sd2"   || Ext == "SD2" )  return SF_FORMAT_SD2;
    if (Ext == "flac"  || Ext == "FLAC")  return SF_FORMAT_FLAC;
    if (Ext == "caf"   || Ext == "CAF" )  return SF_FORMAT_CAF;
    if (Ext == "wve"   || Ext == "WVE" )  return SF_FORMAT_WVE;
    if (Ext == "ogg"   || Ext == "OGG")   return SF_FORMAT_OGG;
    if (Ext == "mpc2k" || Ext == "MPC2K") return SF_FORMAT_MPC2K;
    if (Ext == "rf64"  || Ext == "RF64")  return SF_FORMAT_RF64;

    return -1;
}


////////////////////////////////////////////////////////////
/// Functions for implementing custom read and write to memory files
////////////////////////////////////////////////////////////
sf_count_t SoundFile::MemoryGetLength(void* UserData)
{
    MemoryInfos* Memory = static_cast<MemoryInfos*>(UserData);

    return Memory->TotalSize;
}
sf_count_t SoundFile::MemoryRead(void* Ptr, sf_count_t Count, void* UserData)
{
    MemoryInfos* Memory = static_cast<MemoryInfos*>(UserData);

    sf_count_t Position = Memory->DataPtr - Memory->DataStart;
    if (Position + Count >= Memory->TotalSize)
        Count = Memory->TotalSize - Position;

    memcpy(Ptr, Memory->DataPtr, static_cast<std::size_t>(Count));

    Memory->DataPtr += Count;

    return Count;
}
sf_count_t SoundFile::MemorySeek(sf_count_t Offset, int Whence, void* UserData)
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
sf_count_t SoundFile::MemoryTell(void* UserData)
{
    MemoryInfos* Memory = static_cast<MemoryInfos*>(UserData);

    return Memory->DataPtr - Memory->DataStart;
}
sf_count_t SoundFile::MemoryWrite(const void*, sf_count_t, void*)
{
    return 0;
}

} // namespace priv

} // namespace sf
