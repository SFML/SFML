////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/System/InputStream.hpp>
#include <SFML/System/Err.hpp>
#include <algorithm>
#include <cctype>
#include <cassert>
#include <cstring>


namespace
{
    // The following functions read integers as little endian and
    // return them in the host byte order

    bool decode(sf::InputStream& stream, sf::Uint8& value)
    {
         return static_cast<std::size_t>(stream.read(&value, sizeof(value))) == sizeof(value);
    }

    bool decode(sf::InputStream& stream, sf::Int16& value)
    {
        unsigned char bytes[sizeof(value)];
        if (static_cast<std::size_t>(stream.read(bytes, static_cast<sf::Int64>(sizeof(bytes)))) != sizeof(bytes))
            return false;

        value = static_cast<sf::Uint8>(bytes[0] | (bytes[1] << 8));

        return true;
    }

    bool decode(sf::InputStream& stream, sf::Uint16& value)
    {
        unsigned char bytes[sizeof(value)];
        if (static_cast<std::size_t>(stream.read(bytes, static_cast<sf::Int64>(sizeof(bytes)))) != sizeof(bytes))
            return false;

        value = static_cast<sf::Uint16>(bytes[0] | (bytes[1] << 8));

        return true;
    }

    bool decode24bit(sf::InputStream& stream, sf::Uint32& value)
    {
        unsigned char bytes[3];
        if (static_cast<std::size_t>(stream.read(bytes, static_cast<sf::Int64>(sizeof(bytes)))) != sizeof(bytes))
            return false;

        value = static_cast<sf::Uint32>(bytes[0] | (bytes[1] << 8) | (bytes[2] << 16));

        return true;
    }

    bool decode(sf::InputStream& stream, sf::Uint32& value)
    {
        unsigned char bytes[sizeof(value)];
        if (static_cast<std::size_t>(stream.read(bytes, static_cast<sf::Int64>(sizeof(bytes)))) != sizeof(bytes))
            return false;

        value = static_cast<sf::Uint32>(bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24));

        return true;
    }

    const sf::Uint64 mainChunkSize = 12;

    const sf::Uint16 waveFormatPcm = 1;

    const sf::Uint16 waveFormatExtensible= 65534;

    const char* waveSubformatPcm =
        "\x01\x00\x00\x00\x00\x00\x10\x00"
        "\x80\x00\x00\xAA\x00\x38\x9B\x71";
}

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
bool SoundFileReaderWav::check(InputStream& stream)
{
    char header[mainChunkSize];
    if (stream.read(header, sizeof(header)) < static_cast<Int64>(sizeof(header)))
        return false;

    return (header[0] == 'R') && (header[1] == 'I') && (header[2] == 'F') && (header[3] == 'F')
        && (header[8] == 'W') && (header[9] == 'A') && (header[10] == 'V') && (header[11] == 'E');
}


////////////////////////////////////////////////////////////
SoundFileReaderWav::SoundFileReaderWav() :
m_stream        (nullptr),
m_bytesPerSample(0),
m_dataStart     (0),
m_dataEnd       (0)
{
}


////////////////////////////////////////////////////////////
bool SoundFileReaderWav::open(InputStream& stream, Info& info)
{
    m_stream = &stream;

    if (!parseHeader(info))
    {
        err() << "Failed to open WAV sound file (invalid or unsupported file)" << std::endl;
        return false;
    }

    return true;
}


////////////////////////////////////////////////////////////
void SoundFileReaderWav::seek(Uint64 sampleOffset)
{
    assert(m_stream);

    if (m_stream->seek(static_cast<Int64>(m_dataStart + sampleOffset * m_bytesPerSample) == -1))
        err() << "Failed to seek WAV sound stream" << std::endl;
}


////////////////////////////////////////////////////////////
Uint64 SoundFileReaderWav::read(Int16* samples, Uint64 maxCount)
{
    assert(m_stream);

    Uint64 count = 0;
    auto startPos = static_cast<Uint64>(m_stream->tell());

    // Tracking of m_dataEnd is important to prevent sf::Music from reading
    // data until EOF, as WAV files may have metadata at the end.
    while ((count < maxCount) && (startPos + count * m_bytesPerSample < m_dataEnd))
    {
        switch (m_bytesPerSample)
        {
            case 1:
            {
                Uint8 sample = 0;
                if (decode(*m_stream, sample))
                    *samples++ = static_cast<Int16>((static_cast<Int16>(sample) - 128) << 8);
                else
                    return count;
                break;
            }

            case 2:
            {
                Int16 sample = 0;
                if (decode(*m_stream, sample))
                    *samples++ = sample;
                else
                    return count;
                break;
            }

            case 3:
            {
                Uint32 sample = 0;
                if (decode24bit(*m_stream, sample))
                    *samples++ = static_cast<Int16>(sample >> 8);
                else
                    return count;
                break;
            }

            case 4:
            {
                Uint32 sample = 0;
                if (decode(*m_stream, sample))
                    *samples++ = static_cast<Int16>(sample >> 16);
                else
                    return count;
                break;
            }

            default:
            {
                assert(false);
                return 0;
            }
        }

        ++count;
    }

    return count;
}


////////////////////////////////////////////////////////////
bool SoundFileReaderWav::parseHeader(Info& info)
{
    assert(m_stream);

    // If we are here, it means that the first part of the header
    // (the format) has already been checked
    char mainChunk[mainChunkSize];
    if (static_cast<std::size_t>(m_stream->read(mainChunk, static_cast<Int64>(sizeof(mainChunk)))) != sizeof(mainChunk))
        return false;

    // Parse all the sub-chunks
    bool dataChunkFound = false;
    while (!dataChunkFound)
    {
        // Parse the sub-chunk id and size
        char subChunkId[4];
        if (static_cast<std::size_t>(m_stream->read(subChunkId, static_cast<Int64>(sizeof(subChunkId)))) != sizeof(subChunkId))
            return false;
        Uint32 subChunkSize = 0;
        if (!decode(*m_stream, subChunkSize))
            return false;
        Int64 subChunkStart = m_stream->tell();
        if (subChunkStart == -1)
            return false;

        // Check which chunk it is
        if ((subChunkId[0] == 'f') && (subChunkId[1] == 'm') && (subChunkId[2] == 't') && (subChunkId[3] == ' '))
        {
            // "fmt" chunk

            // Audio format
            Uint16 format = 0;
            if (!decode(*m_stream, format))
                return false;
            if ((format != waveFormatPcm) && (format != waveFormatExtensible))
                return false;

            // Channel count
            Uint16 channelCount = 0;
            if (!decode(*m_stream, channelCount))
                return false;
            info.channelCount = channelCount;

            // Sample rate
            Uint32 sampleRate = 0;
            if (!decode(*m_stream, sampleRate))
                return false;
            info.sampleRate = sampleRate;

            // Byte rate
            Uint32 byteRate = 0;
            if (!decode(*m_stream, byteRate))
                return false;

            // Block align
            Uint16 blockAlign = 0;
            if (!decode(*m_stream, blockAlign))
                return false;

            // Bits per sample
            Uint16 bitsPerSample = 0;
            if (!decode(*m_stream, bitsPerSample))
                return false;
            if (bitsPerSample != 8 && bitsPerSample != 16 && bitsPerSample != 24 && bitsPerSample != 32)
            {
                err() << "Unsupported sample size: " << bitsPerSample << " bit (Supported sample sizes are 8/16/24/32 bit)" << std::endl;
                return false;
            }
            m_bytesPerSample = bitsPerSample / 8;

            if (format == waveFormatExtensible)
            {
                // Extension size
                Uint16 extensionSize = 0;
                if (!decode(*m_stream, extensionSize))
                    return false;

                // Valid bits per sample
                Uint16 validBitsPerSample = 0;
                if (!decode(*m_stream, validBitsPerSample))
                    return false;

                // Channel mask
                Uint32 channelMask = 0;
                if (!decode(*m_stream, channelMask))
                    return false;

                // Subformat
                char subformat[16];
                if (static_cast<std::size_t>(m_stream->read(subformat, static_cast<Int64>(sizeof(subformat)))) != sizeof(subformat))
                    return false;

                if (std::memcmp(subformat, waveSubformatPcm, sizeof(subformat)) != 0)
                {
                    err() << "Unsupported format: extensible format with non-PCM subformat" << std::endl;
                    return false;
                }

                if (validBitsPerSample != bitsPerSample)
                {
                    err() << "Unsupported format: sample size (" << validBitsPerSample << " bits) and "
                            "sample container size (" << bitsPerSample << " bits) differ" << std::endl;
                    return false;
                }
            }

            // Skip potential extra information
            if (m_stream->seek(subChunkStart + subChunkSize) == -1)
                return false;
        }
        else if ((subChunkId[0] == 'd') && (subChunkId[1] == 'a') && (subChunkId[2] == 't') && (subChunkId[3] == 'a'))
        {
            // "data" chunk

            // Compute the total number of samples
            info.sampleCount = subChunkSize / m_bytesPerSample;

            // Store the start and end position of samples in the file
            m_dataStart = static_cast<Uint64>(subChunkStart);
            m_dataEnd = m_dataStart + info.sampleCount * m_bytesPerSample;

            dataChunkFound = true;
        }
        else
        {
            // unknown chunk, skip it
            if (m_stream->seek(m_stream->tell() + subChunkSize) == -1)
                return false;
        }
    }

    return true;
}

} // namespace priv

} // namespace sf
