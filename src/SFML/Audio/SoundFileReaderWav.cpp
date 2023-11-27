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
#include <SFML/Audio/SoundFileReaderWav.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/InputStream.hpp>
#include <SFML/System/Utils.hpp>

#include <algorithm>
#include <ostream>

#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstring>


namespace
{
// The following functions read integers as little endian and
// return them in the host byte order

bool decode(sf::InputStream& stream, std::uint8_t& value)
{
    return static_cast<std::size_t>(stream.read(&value, sizeof(value))) == sizeof(value);
}

bool decode(sf::InputStream& stream, std::int16_t& value)
{
    std::byte bytes[sizeof(value)];
    if (static_cast<std::size_t>(stream.read(bytes, static_cast<std::int64_t>(sizeof(bytes)))) != sizeof(bytes))
        return false;

    value = sf::toInteger<std::int16_t>(bytes[0], bytes[1]);

    return true;
}

bool decode(sf::InputStream& stream, std::uint16_t& value)
{
    std::byte bytes[sizeof(value)];
    if (static_cast<std::size_t>(stream.read(bytes, static_cast<std::int64_t>(sizeof(bytes)))) != sizeof(bytes))
        return false;

    value = sf::toInteger<std::uint16_t>(bytes[0], bytes[1]);

    return true;
}

bool decode24bit(sf::InputStream& stream, std::uint32_t& value)
{
    std::byte bytes[3];
    if (static_cast<std::size_t>(stream.read(bytes, static_cast<std::int64_t>(sizeof(bytes)))) != sizeof(bytes))
        return false;

    value = sf::toInteger<std::uint32_t>(bytes[0], bytes[1], bytes[2]);

    return true;
}

bool decode(sf::InputStream& stream, std::uint32_t& value)
{
    std::byte bytes[sizeof(value)];
    if (static_cast<std::size_t>(stream.read(bytes, static_cast<std::int64_t>(sizeof(bytes)))) != sizeof(bytes))
        return false;

    value = sf::toInteger<std::uint32_t>(bytes[0], bytes[1], bytes[2], bytes[3]);

    return true;
}

const std::uint64_t mainChunkSize = 12;

const std::uint16_t waveFormatPcm = 1;

const std::uint16_t waveFormatExtensible = 65534;

const char* waveSubformatPcm =
    "\x01\x00\x00\x00\x00\x00\x10\x00"
    "\x80\x00\x00\xAA\x00\x38\x9B\x71";
} // namespace

namespace sf::priv
{
////////////////////////////////////////////////////////////
bool SoundFileReaderWav::check(InputStream& stream)
{
    char header[mainChunkSize];
    if (stream.read(header, sizeof(header)) < static_cast<std::int64_t>(sizeof(header)))
        return false;

    return (header[0] == 'R') && (header[1] == 'I') && (header[2] == 'F') && (header[3] == 'F') && (header[8] == 'W') &&
           (header[9] == 'A') && (header[10] == 'V') && (header[11] == 'E');
}


////////////////////////////////////////////////////////////
std::optional<SoundFileReader::Info> SoundFileReaderWav::open(InputStream& stream)
{
    m_stream = &stream;

    const auto info = parseHeader();
    if (!info)
        err() << "Failed to open WAV sound file (invalid or unsupported file)" << std::endl;

    return info;
}


////////////////////////////////////////////////////////////
void SoundFileReaderWav::seek(std::uint64_t sampleOffset)
{
    assert(m_stream && "Input stream cannot be null. Call SoundFileReaderWav::open() to initialize it.");

    if (m_stream->seek(static_cast<std::int64_t>(m_dataStart + sampleOffset * m_bytesPerSample) == -1))
        err() << "Failed to seek WAV sound stream" << std::endl;
}


////////////////////////////////////////////////////////////
std::uint64_t SoundFileReaderWav::read(std::int16_t* samples, std::uint64_t maxCount)
{
    assert(m_stream && "Input stream cannot be null. Call SoundFileReaderWav::open() to initialize it.");

    std::uint64_t count    = 0;
    const auto    startPos = static_cast<std::uint64_t>(m_stream->tell());

    // Tracking of m_dataEnd is important to prevent sf::Music from reading
    // data until EOF, as WAV files may have metadata at the end.
    while ((count < maxCount) && (startPos + count * m_bytesPerSample < m_dataEnd))
    {
        switch (m_bytesPerSample)
        {
            case 1:
            {
                std::uint8_t sample = 0;
                if (decode(*m_stream, sample))
                    *samples++ = static_cast<std::int16_t>((static_cast<std::int16_t>(sample) - 128) << 8);
                else
                    return count;
                break;
            }

            case 2:
            {
                std::int16_t sample = 0;
                if (decode(*m_stream, sample))
                    *samples++ = sample;
                else
                    return count;
                break;
            }

            case 3:
            {
                std::uint32_t sample = 0;
                if (decode24bit(*m_stream, sample))
                    *samples++ = static_cast<std::int16_t>(sample >> 8);
                else
                    return count;
                break;
            }

            case 4:
            {
                std::uint32_t sample = 0;
                if (decode(*m_stream, sample))
                    *samples++ = static_cast<std::int16_t>(sample >> 16);
                else
                    return count;
                break;
            }

            default:
            {
                assert(false && "Invalid bytes per sample. Must be 1, 2, 3, or 4.");
                return 0;
            }
        }

        ++count;
    }

    return count;
}


////////////////////////////////////////////////////////////
std::optional<SoundFileReader::Info> SoundFileReaderWav::parseHeader()
{
    assert(m_stream && "Input stream cannot be null. Call SoundFileReaderWav::open() to initialize it.");

    // If we are here, it means that the first part of the header
    // (the format) has already been checked
    char mainChunk[mainChunkSize];
    if (static_cast<std::size_t>(m_stream->read(mainChunk, static_cast<std::int64_t>(sizeof(mainChunk)))) !=
        sizeof(mainChunk))
        return std::nullopt;

    // Parse all the sub-chunks
    Info info;
    bool dataChunkFound = false;
    while (!dataChunkFound)
    {
        // Parse the sub-chunk id and size
        char subChunkId[4];
        if (static_cast<std::size_t>(m_stream->read(subChunkId, static_cast<std::int64_t>(sizeof(subChunkId)))) !=
            sizeof(subChunkId))
            return std::nullopt;
        std::uint32_t subChunkSize = 0;
        if (!decode(*m_stream, subChunkSize))
            return std::nullopt;
        const std::int64_t subChunkStart = m_stream->tell();
        if (subChunkStart == -1)
            return std::nullopt;

        // Check which chunk it is
        if ((subChunkId[0] == 'f') && (subChunkId[1] == 'm') && (subChunkId[2] == 't') && (subChunkId[3] == ' '))
        {
            // "fmt" chunk

            // Audio format
            std::uint16_t format = 0;
            if (!decode(*m_stream, format))
                return std::nullopt;
            if ((format != waveFormatPcm) && (format != waveFormatExtensible))
                return std::nullopt;

            // Channel count
            std::uint16_t channelCount = 0;
            if (!decode(*m_stream, channelCount))
                return std::nullopt;
            info.channelCount = channelCount;

            // Sample rate
            std::uint32_t sampleRate = 0;
            if (!decode(*m_stream, sampleRate))
                return std::nullopt;
            info.sampleRate = sampleRate;

            // Byte rate
            std::uint32_t byteRate = 0;
            if (!decode(*m_stream, byteRate))
                return std::nullopt;

            // Block align
            std::uint16_t blockAlign = 0;
            if (!decode(*m_stream, blockAlign))
                return std::nullopt;

            // Bits per sample
            std::uint16_t bitsPerSample = 0;
            if (!decode(*m_stream, bitsPerSample))
                return std::nullopt;
            if (bitsPerSample != 8 && bitsPerSample != 16 && bitsPerSample != 24 && bitsPerSample != 32)
            {
                err() << "Unsupported sample size: " << bitsPerSample
                      << " bit (Supported sample sizes are 8/16/24/32 bit)" << std::endl;
                return std::nullopt;
            }
            m_bytesPerSample = bitsPerSample / 8;

            if (format == waveFormatExtensible)
            {
                // Extension size
                std::uint16_t extensionSize = 0;
                if (!decode(*m_stream, extensionSize))
                    return std::nullopt;

                // Valid bits per sample
                std::uint16_t validBitsPerSample = 0;
                if (!decode(*m_stream, validBitsPerSample))
                    return std::nullopt;

                // Channel mask
                std::uint32_t channelMask = 0;
                if (!decode(*m_stream, channelMask))
                    return std::nullopt;

                // Subformat
                char subformat[16];
                if (static_cast<std::size_t>(m_stream->read(subformat, static_cast<std::int64_t>(sizeof(subformat)))) !=
                    sizeof(subformat))
                    return std::nullopt;

                if (std::memcmp(subformat, waveSubformatPcm, sizeof(subformat)) != 0)
                {
                    err() << "Unsupported format: extensible format with non-PCM subformat" << std::endl;
                    return std::nullopt;
                }

                if (validBitsPerSample != bitsPerSample)
                {
                    err() << "Unsupported format: sample size (" << validBitsPerSample
                          << " bits) and "
                             "sample container size ("
                          << bitsPerSample << " bits) differ" << std::endl;
                    return std::nullopt;
                }
            }

            // Skip potential extra information
            if (m_stream->seek(subChunkStart + subChunkSize) == -1)
                return std::nullopt;
        }
        else if ((subChunkId[0] == 'd') && (subChunkId[1] == 'a') && (subChunkId[2] == 't') && (subChunkId[3] == 'a'))
        {
            // "data" chunk

            // Compute the total number of samples
            info.sampleCount = subChunkSize / m_bytesPerSample;

            // Store the start and end position of samples in the file
            m_dataStart = static_cast<std::uint64_t>(subChunkStart);
            m_dataEnd   = m_dataStart + info.sampleCount * m_bytesPerSample;

            dataChunkFound = true;
        }
        else
        {
            // unknown chunk, skip it
            if (m_stream->seek(m_stream->tell() + subChunkSize) == -1)
                return std::nullopt;
        }
    }

    return info;
}

} // namespace sf::priv
