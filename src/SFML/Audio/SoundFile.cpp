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
#include <SFML/System/Err.hpp>
#include <cstring>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
SoundFile::SoundFile() :
myFile         (NULL),
myNbSamples    (0),
myChannelsCount(0),
mySampleRate   (0)
{

}


////////////////////////////////////////////////////////////
SoundFile::~SoundFile()
{
    if (myFile)
        sf_close(myFile);
}


////////////////////////////////////////////////////////////
std::size_t SoundFile::GetSamplesCount() const
{
    return myNbSamples;
}


////////////////////////////////////////////////////////////
unsigned int SoundFile::GetChannelsCount() const
{
    return myChannelsCount;
}


////////////////////////////////////////////////////////////
unsigned int SoundFile::GetSampleRate() const
{
    return mySampleRate;
}


////////////////////////////////////////////////////////////
bool SoundFile::OpenRead(const std::string& filename)
{
    // If the file is already opened, first close it
    if (myFile)
        sf_close(myFile);

    // Open the sound file
    SF_INFO fileInfos;
    myFile = sf_open(filename.c_str(), SFM_READ, &fileInfos);
    if (!myFile)
    {
        Err() << "Failed to read sound file \"" << filename << "\" (" << sf_strerror(myFile) << ")" << std::endl;
        return false;
    }

    // Set the sound parameters
    myChannelsCount = fileInfos.channels;
    mySampleRate    = fileInfos.samplerate;
    myNbSamples     = static_cast<std::size_t>(fileInfos.frames) * myChannelsCount;

    return true;
}


////////////////////////////////////////////////////////////
bool SoundFile::OpenRead(const void* data, std::size_t sizeInBytes)
{
    // If the file is already opened, first close it
    if (myFile)
        sf_close(myFile);

    // Prepare the memory I/O structure
    SF_VIRTUAL_IO io = myMemoryIO.Prepare(data, sizeInBytes);

    // Open the sound file
    SF_INFO fileInfos;
    myFile = sf_open_virtual(&io, SFM_READ, &fileInfos, &myMemoryIO);
    if (!myFile)
    {
        Err() << "Failed to read sound file from memory (" << sf_strerror(myFile) << ")" << std::endl;
        return false;
    }

    // Set the sound parameters
    myChannelsCount = fileInfos.channels;
    mySampleRate    = fileInfos.samplerate;
    myNbSamples     = static_cast<std::size_t>(fileInfos.frames) * myChannelsCount;

    return true;
}


////////////////////////////////////////////////////////////
bool SoundFile::OpenWrite(const std::string& filename, unsigned int channelsCount, unsigned int sampleRate)
{
    // If the file is already opened, first close it
    if (myFile)
        sf_close(myFile);

    // Find the right format according to the file extension
    int format = GetFormatFromFilename(filename);
    if (format == -1)
    {
        // Error : unrecognized extension
        Err() << "Failed to create sound file \"" << filename << "\" (unknown format)" << std::endl;
        return false;
    }

    // Fill the sound infos with parameters
    SF_INFO fileInfos;
    fileInfos.channels   = channelsCount;
    fileInfos.samplerate = sampleRate;
    fileInfos.format     = format | (format == SF_FORMAT_OGG ? SF_FORMAT_VORBIS : SF_FORMAT_PCM_16);

    // Open the sound file for writing
    myFile = sf_open(filename.c_str(), SFM_WRITE, &fileInfos);
    if (!myFile)
    {
        Err() << "Failed to create sound file \"" << filename << "\" (" << sf_strerror(myFile) << ")" << std::endl;
        return false;
    }

    // Set the sound parameters
    myChannelsCount = channelsCount;
    mySampleRate    = sampleRate;
    myNbSamples     = 0;

    return true;
}


////////////////////////////////////////////////////////////
std::size_t SoundFile::Read(Int16* data, std::size_t nbSamples)
{
    if (myFile && data && nbSamples)
        return static_cast<std::size_t>(sf_read_short(myFile, data, nbSamples));
    else
        return 0;
}


////////////////////////////////////////////////////////////
void SoundFile::Write(const Int16* data, std::size_t nbSamples)
{
    if (myFile && data && nbSamples)
    {
        // Write small chunks instead of everything at once,
        // to avoid a stack overflow in libsndfile (happens only with OGG format)
        while (nbSamples > 0)
        {
            std::size_t count = nbSamples > 10000 ? 10000 : nbSamples;
            sf_write_short(myFile, data, count);
            data += count;
            nbSamples -= count;
        }
    }
}


////////////////////////////////////////////////////////////
void SoundFile::Seek(float timeOffset)
{
    if (myFile)
    {
        sf_count_t frameOffset = static_cast<sf_count_t>(timeOffset * mySampleRate);
        sf_seek(myFile, frameOffset, SEEK_SET);
    }
}


////////////////////////////////////////////////////////////
int SoundFile::GetFormatFromFilename(const std::string& filename)
{
    // Extract the extension
    std::string ext = "wav";
    std::string::size_type pos = filename.find_last_of(".");
    if (pos != std::string::npos)
        ext = filename.substr(pos + 1);

    // Match every supported extension with its format constant
    if (ext == "wav"   || ext == "WAV" )  return SF_FORMAT_WAV;
    if (ext == "aif"   || ext == "AIF" )  return SF_FORMAT_AIFF;
    if (ext == "aiff"  || ext == "AIFF")  return SF_FORMAT_AIFF;
    if (ext == "au"    || ext == "AU"  )  return SF_FORMAT_AU;
    if (ext == "raw"   || ext == "RAW" )  return SF_FORMAT_RAW;
    if (ext == "paf"   || ext == "PAF" )  return SF_FORMAT_PAF;
    if (ext == "svx"   || ext == "SVX" )  return SF_FORMAT_SVX;
    if (ext == "nist"  || ext == "NIST")  return SF_FORMAT_NIST;
    if (ext == "voc"   || ext == "VOC" )  return SF_FORMAT_VOC;
    if (ext == "sf"    || ext == "SF"  )  return SF_FORMAT_IRCAM;
    if (ext == "w64"   || ext == "W64" )  return SF_FORMAT_W64;
    if (ext == "mat4"  || ext == "MAT4")  return SF_FORMAT_MAT4;
    if (ext == "mat5"  || ext == "MAT5")  return SF_FORMAT_MAT5;
    if (ext == "pvf"   || ext == "PVF" )  return SF_FORMAT_PVF;
    if (ext == "xi"    || ext == "XI" )   return SF_FORMAT_XI;
    if (ext == "htk"   || ext == "HTK" )  return SF_FORMAT_HTK;
    if (ext == "sds"   || ext == "SDS" )  return SF_FORMAT_SDS;
    if (ext == "avr"   || ext == "AVR" )  return SF_FORMAT_AVR;
    if (ext == "sd2"   || ext == "SD2" )  return SF_FORMAT_SD2;
    if (ext == "flac"  || ext == "FLAC")  return SF_FORMAT_FLAC;
    if (ext == "caf"   || ext == "CAF" )  return SF_FORMAT_CAF;
    if (ext == "wve"   || ext == "WVE" )  return SF_FORMAT_WVE;
    if (ext == "ogg"   || ext == "OGG")   return SF_FORMAT_OGG;
    if (ext == "mpc2k" || ext == "MPC2K") return SF_FORMAT_MPC2K;
    if (ext == "rf64"  || ext == "RF64")  return SF_FORMAT_RF64;

    return -1;
}


////////////////////////////////////////////////////////////
SF_VIRTUAL_IO SoundFile::MemoryIO::Prepare(const void* data, std::size_t sizeInBytes)
{
    // Setup the I/O functions
    SF_VIRTUAL_IO io;
    io.get_filelen = &SoundFile::MemoryIO::GetLength;
    io.read        = &SoundFile::MemoryIO::Read;
    io.seek        = &SoundFile::MemoryIO::Seek;
    io.tell        = &SoundFile::MemoryIO::Tell;
    io.write       = &SoundFile::MemoryIO::Write;

    // Initialize the memory data
    myDataStart = static_cast<const char*>(data);
    myDataPtr   = myDataStart;
    myTotalSize = sizeInBytes;

    return io;
}


////////////////////////////////////////////////////////////
sf_count_t SoundFile::MemoryIO::GetLength(void* userData)
{
    MemoryIO* self = static_cast<MemoryIO*>(userData);

    return self->myTotalSize;
}


////////////////////////////////////////////////////////////
sf_count_t SoundFile::MemoryIO::Read(void* ptr, sf_count_t count, void* userData)
{
    MemoryIO* self = static_cast<MemoryIO*>(userData);

    sf_count_t position = self->myDataPtr - self->myDataStart;
    if (position + count >= self->myTotalSize)
        count = self->myTotalSize - position;

    std::memcpy(ptr, self->myDataPtr, static_cast<std::size_t>(count));

    self->myDataPtr += count;

    return count;
}


////////////////////////////////////////////////////////////
sf_count_t SoundFile::MemoryIO::Seek(sf_count_t offset, int whence, void* userData)
{
    MemoryIO* self = static_cast<MemoryIO*>(userData);

    sf_count_t position = 0;
    switch (whence)
    {
        case SEEK_SET : position = offset;                                       break;
        case SEEK_CUR : position = self->myDataPtr - self->myDataStart + offset; break;
        case SEEK_END : position = self->myTotalSize - offset;                   break;
        default       : position = 0;                                            break;
    }

    if (position >= self->myTotalSize)
        position = self->myTotalSize - 1;
    else if (position < 0)
        position = 0;

    self->myDataPtr = self->myDataStart + position;

    return position;
}


////////////////////////////////////////////////////////////
sf_count_t SoundFile::MemoryIO::Tell(void* userData)
{
    MemoryIO* self = static_cast<MemoryIO*>(userData);

    return self->myDataPtr - self->myDataStart;
}


////////////////////////////////////////////////////////////
sf_count_t SoundFile::MemoryIO::Write(const void*, sf_count_t, void*)
{
    return 0;
}

} // namespace priv

} // namespace sf
