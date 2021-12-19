////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2021 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/System/MemoryInputStream.hpp>
#include <SFML/System/Err.hpp>
#include <cctype>
#include <cassert>


namespace
{
    size_t read(void* ptr, size_t size, size_t nmemb, void* data)
    {
        auto* stream = static_cast<sf::InputStream*>(data);
        return static_cast<std::size_t>(stream->read(ptr, static_cast<sf::Int64>(size * nmemb)));
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

    static ov_callbacks callbacks = {&read, &seek, nullptr, &tell};
}

namespace sf
{
namespace priv
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
SoundFileReaderOgg::SoundFileReaderOgg() :
m_vorbis      (),
m_channelCount(0)
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
    int status = ov_open_callbacks(&stream, &m_vorbis, nullptr, 0, callbacks);
    if (status < 0)
    {
        err() << "Failed to open Vorbis file for reading" << std::endl;
        return false;
    }

    // Retrieve the music attributes
    vorbis_info* vorbisInfo = ov_info(&m_vorbis, -1);
    info.channelCount = static_cast<unsigned int>(vorbisInfo->channels);
    info.sampleRate = static_cast<unsigned int>(vorbisInfo->rate);
    info.sampleCount = static_cast<std::size_t>(ov_pcm_total(&m_vorbis, -1) * vorbisInfo->channels);

    // We must keep the channel count for the seek function
    m_channelCount = info.channelCount;

    return true;
}


////////////////////////////////////////////////////////////
void SoundFileReaderOgg::seek(Uint64 sampleOffset)
{
    assert(m_vorbis.datasource);

    ov_pcm_seek(&m_vorbis, static_cast<ogg_int64_t>(sampleOffset / m_channelCount));
}


////////////////////////////////////////////////////////////
Uint64 SoundFileReaderOgg::read(Int16* samples, Uint64 maxCount)
{
    assert(m_vorbis.datasource);

    // Try to read the requested number of samples, stop only on error or end of file
    Uint64 count = 0;
    while (count < maxCount)
    {
        int bytesToRead = static_cast<int>(maxCount - count) * static_cast<int>(sizeof(Int16));
        long bytesRead = ov_read(&m_vorbis, reinterpret_cast<char*>(samples), bytesToRead, 0, 2, 1, nullptr);
        if (bytesRead > 0)
        {
            long samplesRead = bytesRead / static_cast<long>(sizeof(Int16));
            count += static_cast<Uint64>(samplesRead);
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
        m_channelCount = 0;
    }
}

} // namespace priv

} // namespace sf
