////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/System/InputStream.hpp>

#include <ostream>

#include <cassert>
#include <cstdio>


namespace
{
std::size_t read(void* ptr, std::size_t size, std::size_t nmemb, void* data)
{
    auto* stream = static_cast<sf::InputStream*>(data);
    return stream->read(ptr, size * nmemb).value_or(-1);
}

int seek(void* data, ogg_int64_t signedOffset, int whence)
{
    auto* stream = static_cast<sf::InputStream*>(data);
    auto  offset = static_cast<std::size_t>(signedOffset);
    switch (whence)
    {
        case SEEK_SET:
            break;
        case SEEK_CUR:
            offset += stream->tell().value();
            break;
        case SEEK_END:
            offset = stream->getSize().value() - offset;
    }
    const std::optional position = stream->seek(offset);
    return position ? static_cast<int>(*position) : -1;
}

long tell(void* data)
{
    auto*               stream   = static_cast<sf::InputStream*>(data);
    const std::optional position = stream->tell();
    return position ? static_cast<long>(*position) : -1;
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

    return false;
}


////////////////////////////////////////////////////////////
SoundFileReaderOgg::~SoundFileReaderOgg()
{
    close();
}


////////////////////////////////////////////////////////////
std::optional<SoundFileReader::Info> SoundFileReaderOgg::open(InputStream& stream)
{
    // Open the Vorbis stream
    const int status = ov_open_callbacks(&stream, &m_vorbis, nullptr, 0, callbacks);
    if (status < 0)
    {
        err() << "Failed to open Vorbis file for reading" << std::endl;
        return std::nullopt;
    }

    // Retrieve the music attributes
    vorbis_info* vorbisInfo = ov_info(&m_vorbis, -1);
    Info         info;
    info.channelCount = static_cast<unsigned int>(vorbisInfo->channels);
    info.sampleRate   = static_cast<unsigned int>(vorbisInfo->rate);
    info.sampleCount  = static_cast<std::size_t>(ov_pcm_total(&m_vorbis, -1) * vorbisInfo->channels);

    // For Vorbis channel mapping refer to: https://xiph.org/vorbis/doc/Vorbis_I_spec.html#x1-810004.3.9
    switch (info.channelCount)
    {
        case 0:
            err() << "No channels in Vorbis file" << std::endl;
            break;
        case 1:
            info.channelMap = {SoundChannel::Mono};
            break;
        case 2:
            info.channelMap = {SoundChannel::FrontLeft, SoundChannel::FrontRight};
            break;
        case 3:
            info.channelMap = {SoundChannel::FrontLeft, SoundChannel::FrontCenter, SoundChannel::FrontRight};
            break;
        case 4:
            info.channelMap = {SoundChannel::FrontLeft, SoundChannel::FrontRight, SoundChannel::BackLeft, SoundChannel::BackRight};
            break;
        case 5:
            info.channelMap = {SoundChannel::FrontLeft,
                               SoundChannel::FrontCenter,
                               SoundChannel::FrontRight,
                               SoundChannel::BackLeft,
                               SoundChannel::BackRight};
            break;
        case 6:
            info.channelMap = {SoundChannel::FrontLeft,
                               SoundChannel::FrontCenter,
                               SoundChannel::FrontRight,
                               SoundChannel::BackLeft,
                               SoundChannel::BackRight,
                               SoundChannel::LowFrequencyEffects};
            break;
        case 7:
            info.channelMap = {SoundChannel::FrontLeft,
                               SoundChannel::FrontCenter,
                               SoundChannel::FrontRight,
                               SoundChannel::SideLeft,
                               SoundChannel::SideRight,
                               SoundChannel::BackCenter,
                               SoundChannel::LowFrequencyEffects};
            break;
        case 8:
            info.channelMap = {SoundChannel::FrontLeft,
                               SoundChannel::FrontCenter,
                               SoundChannel::FrontRight,
                               SoundChannel::SideLeft,
                               SoundChannel::SideRight,
                               SoundChannel::BackLeft,
                               SoundChannel::BackRight,
                               SoundChannel::LowFrequencyEffects};
            break;
        default:
            err() << "Vorbis files with more than 8 channels not supported" << std::endl;
            assert(false);
            break;
    }

    // We must keep the channel count for the seek function
    m_channelCount = info.channelCount;

    return info;
}


////////////////////////////////////////////////////////////
void SoundFileReaderOgg::seek(std::uint64_t sampleOffset)
{
    assert(m_vorbis.datasource && "Vorbis datasource is missing. Call SoundFileReaderOgg::open() to initialize it.");

    ov_pcm_seek(&m_vorbis, static_cast<ogg_int64_t>(sampleOffset / m_channelCount));
}


////////////////////////////////////////////////////////////
std::uint64_t SoundFileReaderOgg::read(std::int16_t* samples, std::uint64_t maxCount)
{
    assert(m_vorbis.datasource && "Vorbis datasource is missing. Call SoundFileReaderOgg::open() to initialize it.");

    // Try to read the requested number of samples, stop only on error or end of file
    std::uint64_t count = 0;
    while (count < maxCount)
    {
        const int bytesToRead = static_cast<int>(maxCount - count) * static_cast<int>(sizeof(std::int16_t));
        const long bytesRead = ov_read(&m_vorbis, reinterpret_cast<char*>(samples), bytesToRead, SFML_IS_BIG_ENDIAN, 2, 1, nullptr);
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
