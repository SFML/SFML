////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Audio/SoundFileReaderOgg.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/MemoryInputStream.hpp>

#include <ostream>

#include <cassert>
#include <cctype>


namespace
{
std::size_t read(void* ptr, std::size_t size, std::size_t nmemb, void* data)
{
    auto* stream = static_cast<sf::InputStream*>(data);
    return static_cast<std::size_t>(stream->read(ptr, static_cast<std::int64_t>(size * nmemb)));
}

int seek(void* data, ogg_int64_t offset, int whence)
{
    auto* stream = static_cast<sf::InputStream*>(data);
    switch (whence)
    {
        case SEEK_SET:
            break;
        case SEEK_CUR:
            offset += stream->tell();
            break;
        case SEEK_END:
            offset = stream->getSize() - offset;
    }
    return static_cast<int>(stream->seek(offset));
}

long tell(void* data)
{
    auto* stream = static_cast<sf::InputStream*>(data);
    return static_cast<long>(stream->tell());
}

ov_callbacks callbacks = {&read, &seek, nullptr, &tell};
} // namespace

namespace sf::priv
{
////////////////////////////////////////////////////////////
bool SoundFileReaderOgg::check(InputStream& stream)
{
    OggVorbis_File file;
    if (ov_test_callbacks(&stream, &file, nullptr, 0, callbacks) == 0)
    {
        ov_clear(&file);
        return true;
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
SoundFileReaderOgg::SoundFileReaderOgg()
{
    m_vorbis.datasource = nullptr;
}


////////////////////////////////////////////////////////////
SoundFileReaderOgg::~SoundFileReaderOgg()
{
    close();
}


////////////////////////////////////////////////////////////
bool SoundFileReaderOgg::open(InputStream& stream, Info& info)
{
    // Open the Vorbis stream
    const int status = ov_open_callbacks(&stream, &m_vorbis, nullptr, 0, callbacks);
    if (status < 0)
    {
        err() << "Failed to open Vorbis file for reading" << std::endl;
        return false;
    }

    // Retrieve the music attributes
    vorbis_info* vorbisInfo = ov_info(&m_vorbis, -1);
    info.channelCount       = static_cast<unsigned int>(vorbisInfo->channels);
    info.sampleRate         = static_cast<unsigned int>(vorbisInfo->rate);
    info.sampleCount        = static_cast<std::size_t>(ov_pcm_total(&m_vorbis, -1) * vorbisInfo->channels);

    // We must keep the channel count for the seek function
    m_channelCount = info.channelCount;

    return true;
}


////////////////////////////////////////////////////////////
void SoundFileReaderOgg::seek(std::uint64_t sampleOffset)
{
    assert(m_vorbis.datasource);

    ov_pcm_seek(&m_vorbis, static_cast<ogg_int64_t>(sampleOffset / m_channelCount));
}


////////////////////////////////////////////////////////////
std::uint64_t SoundFileReaderOgg::read(std::int16_t* samples, std::uint64_t maxCount)
{
    assert(m_vorbis.datasource);

    // Try to read the requested number of samples, stop only on error or end of file
    std::uint64_t count = 0;
    while (count < maxCount)
    {
        const int  bytesToRead = static_cast<int>(maxCount - count) * static_cast<int>(sizeof(std::int16_t));
        const long bytesRead   = ov_read(&m_vorbis, reinterpret_cast<char*>(samples), bytesToRead, 0, 2, 1, nullptr);
        if (bytesRead > 0)
        {
            const long samplesRead = bytesRead / static_cast<long>(sizeof(std::int16_t));
            count += static_cast<std::uint64_t>(samplesRead);
            samples += samplesRead;
        }
        else
        {
            // error or end of file
            break;
        }
    }

    return count;
}


////////////////////////////////////////////////////////////
void SoundFileReaderOgg::close()
{
    if (m_vorbis.datasource)
    {
        ov_clear(&m_vorbis);
        m_vorbis.datasource = nullptr;
        m_channelCount      = 0;
    }
}

} // namespace sf::priv
