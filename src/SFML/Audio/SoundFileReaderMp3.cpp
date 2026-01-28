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
#define DR_MP3_IMPLEMENTATION
#define DR_MP3_NO_STDIO
#include <dr_mp3.h>
#undef DR_MP3_NO_STDIO

#include <SFML/Audio/SoundFileReaderMp3.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/InputStream.hpp>

#include <algorithm>
#include <array>
#include <ostream>

#include <cassert>
#include <cstdint>
#include <cstring>


namespace
{
std::size_t readCallback(void* userData, void* bufferOut, std::size_t bytesToRead)
{
    auto* stream = static_cast<sf::InputStream*>(userData);
    return stream->read(bufferOut, bytesToRead).value_or(0);
}

drmp3_bool32 seekCallback(void* userData, int offset, drmp3_seek_origin origin)
{
    auto* stream = static_cast<sf::InputStream*>(userData);

    if (origin == DRMP3_SEEK_CUR)
    {
        auto currentPosition = stream->tell();

        if (!currentPosition.has_value())
            return false;

        offset = static_cast<int>(currentPosition.value()) + offset;
    }
    else if (origin == DRMP3_SEEK_END)
    {
        auto currentSize = stream->getSize();

        if (!currentSize.has_value())
            return false;

        offset = static_cast<int>(currentSize.value()) + offset;
    }

    const std::optional position = stream->seek(static_cast<std::size_t>(offset));
    return position.has_value();
}

drmp3_bool32 tellCallback(void* userData, drmp3_int64* pCursor)
{
    auto* stream = static_cast<sf::InputStream*>(userData);

    auto position = stream->tell();

    if (!position.has_value())
        return false;

    *pCursor = static_cast<drmp3_int64>(position.value());
    return *pCursor != -1;
}

bool hasValidId3Tag(const std::uint8_t* header)
{
    return std::memcmp(header, "ID3", 3) == 0 &&
           !((header[5] & 15) || (header[6] & 0x80) || (header[7] & 0x80) || (header[8] & 0x80) || (header[9] & 0x80));
}
} // namespace

namespace sf::priv
{
////////////////////////////////////////////////////////////
bool SoundFileReaderMp3::check(InputStream& stream)
{
    std::array<std::uint8_t, 10> header{};

    if (stream.read(header.data(), header.size()) != header.size())
        return false;

    if (hasValidId3Tag(header.data()))
        return true;

    if (drmp3_hdr_valid(header.data()))
        return true;

    return false;
}


////////////////////////////////////////////////////////////
SoundFileReaderMp3::SoundFileReaderMp3() = default;


////////////////////////////////////////////////////////////
SoundFileReaderMp3::~SoundFileReaderMp3()
{
    drmp3_uninit(&m_decoder);
}


////////////////////////////////////////////////////////////
std::optional<SoundFileReader::Info> SoundFileReaderMp3::open(InputStream& stream)
{
    // Init mp3 decoder
    if (drmp3_init(&m_decoder, readCallback, seekCallback, tellCallback, nullptr, &stream, nullptr) != 1)
        return std::nullopt;

    // Retrieve the music attributes
    Info info;
    info.channelCount = static_cast<unsigned int>(m_decoder.channels);
    info.sampleRate   = static_cast<unsigned int>(m_decoder.sampleRate);
    info.sampleCount  = drmp3_get_pcm_frame_count(&m_decoder) * m_decoder.channels;

    // MP3 only supports mono/stereo channels
    switch (info.channelCount)
    {
        case 0:
            err() << "No channels in MP3 file" << std::endl;
            break;
        case 1:
            info.channelMap = {SoundChannel::Mono};
            break;
        case 2:
            info.channelMap = {SoundChannel::SideLeft, SoundChannel::SideRight};
            break;
        default:
            err() << "MP3 files with more than 2 channels not supported" << std::endl;
            assert(false);
            break;
    }

    m_numSamples = info.sampleCount;
    return info;
}


////////////////////////////////////////////////////////////
void SoundFileReaderMp3::seek(std::uint64_t sampleOffset)
{
    m_position = std::min(sampleOffset, m_numSamples);
    drmp3_seek_to_pcm_frame(&m_decoder, m_position / m_decoder.channels);
}


////////////////////////////////////////////////////////////
std::uint64_t SoundFileReaderMp3::read(std::int16_t* samples, std::uint64_t maxCount)
{
    // dr_mp3's PCM frame contains one sample per channel
    std::uint64_t toRead = std::min(maxCount, m_numSamples - m_position) / m_decoder.channels;
    toRead               = std::uint64_t{drmp3_read_pcm_frames_s16(&m_decoder, toRead, samples)} * m_decoder.channels;
    m_position += toRead;
    return toRead;
}

} // namespace sf::priv
