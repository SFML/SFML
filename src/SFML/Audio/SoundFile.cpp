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
#include <SFML/System/InputStream.hpp>
#include <SFML/System/Err.hpp>
#include <cstring>
#include <cctype>


namespace
{
    // Convert a string to lower case
    std::string ToLower(std::string str)
    {
        for (std::string::iterator i = str.begin(); i != str.end(); ++i)
            *i = static_cast<char>(std::tolower(*i));
        return str;
    }
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
SoundFile::SoundFile() :
m_file        (NULL),
m_sampleCount (0),
m_channelCount(0),
m_sampleRate  (0)
{

}


////////////////////////////////////////////////////////////
SoundFile::~SoundFile()
{
    if (m_file)
        sf_close(m_file);
}


////////////////////////////////////////////////////////////
std::size_t SoundFile::GetSampleCount() const
{
    return m_sampleCount;
}


////////////////////////////////////////////////////////////
unsigned int SoundFile::GetChannelCount() const
{
    return m_channelCount;
}


////////////////////////////////////////////////////////////
unsigned int SoundFile::GetSampleRate() const
{
    return m_sampleRate;
}


////////////////////////////////////////////////////////////
bool SoundFile::OpenRead(const std::string& filename)
{
    // If the file is already opened, first close it
    if (m_file)
        sf_close(m_file);

    // Open the sound file
    SF_INFO fileInfos;
    m_file = sf_open(filename.c_str(), SFM_READ, &fileInfos);
    if (!m_file)
    {
        Err() << "Failed to open sound file \"" << filename << "\" (" << sf_strerror(m_file) << ")" << std::endl;
        return false;
    }

    // Set the sound parameters
    m_channelCount = fileInfos.channels;
    m_sampleRate   = fileInfos.samplerate;
    m_sampleCount  = static_cast<std::size_t>(fileInfos.frames) * m_channelCount;

    return true;
}


////////////////////////////////////////////////////////////
bool SoundFile::OpenRead(const void* data, std::size_t sizeInBytes)
{
    // If the file is already opened, first close it
    if (m_file)
        sf_close(m_file);

    // Prepare the memory I/O structure
    SF_VIRTUAL_IO io;
    io.get_filelen = &Memory::GetLength;
    io.read        = &Memory::Read;
    io.seek        = &Memory::Seek;
    io.tell        = &Memory::Tell;

    // Initialize the memory data
    m_memory.DataStart = static_cast<const char*>(data);
    m_memory.DataPtr   = m_memory.DataStart;
    m_memory.TotalSize = sizeInBytes;

    // Open the sound file
    SF_INFO fileInfos;
    m_file = sf_open_virtual(&io, SFM_READ, &fileInfos, &m_memory);
    if (!m_file)
    {
        Err() << "Failed to open sound file from memory (" << sf_strerror(m_file) << ")" << std::endl;
        return false;
    }

    // Set the sound parameters
    m_channelCount = fileInfos.channels;
    m_sampleRate   = fileInfos.samplerate;
    m_sampleCount  = static_cast<std::size_t>(fileInfos.frames) * m_channelCount;

    return true;
}


////////////////////////////////////////////////////////////
bool SoundFile::OpenRead(InputStream& stream)
{
    // If the file is already opened, first close it
    if (m_file)
        sf_close(m_file);

    // Prepare the memory I/O structure
    SF_VIRTUAL_IO io;
    io.get_filelen = &Stream::GetLength;
    io.read        = &Stream::Read;
    io.seek        = &Stream::Seek;
    io.tell        = &Stream::Tell;

    // Open the sound file
    SF_INFO fileInfos;
    m_file = sf_open_virtual(&io, SFM_READ, &fileInfos, &stream);
    if (!m_file)
    {
        Err() << "Failed to open sound file from stream (" << sf_strerror(m_file) << ")" << std::endl;
        return false;
    }

    // Set the sound parameters
    m_channelCount = fileInfos.channels;
    m_sampleRate   = fileInfos.samplerate;
    m_sampleCount  = static_cast<std::size_t>(fileInfos.frames) * m_channelCount;

    return true;
}


////////////////////////////////////////////////////////////
bool SoundFile::OpenWrite(const std::string& filename, unsigned int channelCount, unsigned int sampleRate)
{
    // If the file is already opened, first close it
    if (m_file)
        sf_close(m_file);

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
    fileInfos.channels   = channelCount;
    fileInfos.samplerate = sampleRate;
    fileInfos.format     = format | (format == SF_FORMAT_OGG ? SF_FORMAT_VORBIS : SF_FORMAT_PCM_16);

    // Open the sound file for writing
    m_file = sf_open(filename.c_str(), SFM_WRITE, &fileInfos);
    if (!m_file)
    {
        Err() << "Failed to create sound file \"" << filename << "\" (" << sf_strerror(m_file) << ")" << std::endl;
        return false;
    }

    // Set the sound parameters
    m_channelCount = channelCount;
    m_sampleRate   = sampleRate;
    m_sampleCount  = 0;

    return true;
}


////////////////////////////////////////////////////////////
std::size_t SoundFile::Read(Int16* data, std::size_t sampleCount)
{
    if (m_file && data && sampleCount)
        return static_cast<std::size_t>(sf_read_short(m_file, data, sampleCount));
    else
        return 0;
}


////////////////////////////////////////////////////////////
void SoundFile::Write(const Int16* data, std::size_t sampleCount)
{
    if (m_file && data && sampleCount)
    {
        // Write small chunks instead of everything at once,
        // to avoid a stack overflow in libsndfile (happens only with OGG format)
        while (sampleCount > 0)
        {
            std::size_t count = sampleCount > 10000 ? 10000 : sampleCount;
            sf_write_short(m_file, data, count);
            data += count;
            sampleCount -= count;
        }
    }
}


////////////////////////////////////////////////////////////
void SoundFile::Seek(Time timeOffset)
{
    if (m_file)
    {
        sf_count_t frameOffset = static_cast<sf_count_t>(timeOffset.AsSeconds() * m_sampleRate);
        sf_seek(m_file, frameOffset, SEEK_SET);
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
    if (ToLower(ext) == "wav"  ) return SF_FORMAT_WAV;
    if (ToLower(ext) == "aif"  ) return SF_FORMAT_AIFF;
    if (ToLower(ext) == "aiff" ) return SF_FORMAT_AIFF;
    if (ToLower(ext) == "au"   ) return SF_FORMAT_AU;
    if (ToLower(ext) == "raw"  ) return SF_FORMAT_RAW;
    if (ToLower(ext) == "paf"  ) return SF_FORMAT_PAF;
    if (ToLower(ext) == "svx"  ) return SF_FORMAT_SVX;
    if (ToLower(ext) == "nist" ) return SF_FORMAT_NIST;
    if (ToLower(ext) == "voc"  ) return SF_FORMAT_VOC;
    if (ToLower(ext) == "sf"   ) return SF_FORMAT_IRCAM;
    if (ToLower(ext) == "w64"  ) return SF_FORMAT_W64;
    if (ToLower(ext) == "mat4" ) return SF_FORMAT_MAT4;
    if (ToLower(ext) == "mat5" ) return SF_FORMAT_MAT5;
    if (ToLower(ext) == "pvf"  ) return SF_FORMAT_PVF;
    if (ToLower(ext) == "xi"   ) return SF_FORMAT_XI;
    if (ToLower(ext) == "htk"  ) return SF_FORMAT_HTK;
    if (ToLower(ext) == "sds"  ) return SF_FORMAT_SDS;
    if (ToLower(ext) == "avr"  ) return SF_FORMAT_AVR;
    if (ToLower(ext) == "sd2"  ) return SF_FORMAT_SD2;
    if (ToLower(ext) == "flac" ) return SF_FORMAT_FLAC;
    if (ToLower(ext) == "caf"  ) return SF_FORMAT_CAF;
    if (ToLower(ext) == "wve"  ) return SF_FORMAT_WVE;
    if (ToLower(ext) == "ogg"  ) return SF_FORMAT_OGG;
    if (ToLower(ext) == "mpc2k") return SF_FORMAT_MPC2K;
    if (ToLower(ext) == "rf64" ) return SF_FORMAT_RF64;

    return -1;
}


////////////////////////////////////////////////////////////
sf_count_t SoundFile::Memory::GetLength(void* user)
{
    Memory* memory = static_cast<Memory*>(user);
    return memory->TotalSize;
}


////////////////////////////////////////////////////////////
sf_count_t SoundFile::Memory::Read(void* ptr, sf_count_t count, void* user)
{
    Memory* memory = static_cast<Memory*>(user);

    sf_count_t position = memory->DataPtr - memory->DataStart;
    if (position + count >= memory->TotalSize)
        count = memory->TotalSize - position;

    std::memcpy(ptr, memory->DataPtr, static_cast<std::size_t>(count));
    memory->DataPtr += count;
    return count;
}


////////////////////////////////////////////////////////////
sf_count_t SoundFile::Memory::Seek(sf_count_t offset, int whence, void* user)
{
    Memory* memory = static_cast<Memory*>(user);
    sf_count_t position = 0;
    switch (whence)
    {
        case SEEK_SET : position = offset;                                       break;
        case SEEK_CUR : position = memory->DataPtr - memory->DataStart + offset; break;
        case SEEK_END : position = memory->TotalSize - offset;                   break;
        default       : position = 0;                                            break;
    }

    if (position >= memory->TotalSize)
        position = memory->TotalSize - 1;
    else if (position < 0)
        position = 0;

    memory->DataPtr = memory->DataStart + position;
    return position;
}


////////////////////////////////////////////////////////////
sf_count_t SoundFile::Memory::Tell(void* user)
{
    Memory* memory = static_cast<Memory*>(user);
    return memory->DataPtr - memory->DataStart;
}


////////////////////////////////////////////////////////////
sf_count_t SoundFile::Stream::GetLength(void* userData)
{
    sf::InputStream* stream = static_cast<sf::InputStream*>(userData);
    return stream->GetSize();
}


////////////////////////////////////////////////////////////
sf_count_t SoundFile::Stream::Read(void* ptr, sf_count_t count, void* userData)
{
    sf::InputStream* stream = static_cast<sf::InputStream*>(userData);
    return stream->Read(reinterpret_cast<char*>(ptr), count);
}


////////////////////////////////////////////////////////////
sf_count_t SoundFile::Stream::Seek(sf_count_t offset, int whence, void* userData)
{
    sf::InputStream* stream = static_cast<sf::InputStream*>(userData);
    switch (whence)
    {
        case SEEK_SET : return stream->Seek(offset);
        case SEEK_CUR : return stream->Seek(stream->Tell() + offset);
        case SEEK_END : return stream->Seek(stream->GetSize() - offset);
        default       : return stream->Seek(0);
    }
}


////////////////////////////////////////////////////////////
sf_count_t SoundFile::Stream::Tell(void* userData)
{
    sf::InputStream* stream = static_cast<sf::InputStream*>(userData);
    return stream->Tell();
}

} // namespace priv

} // namespace sf
