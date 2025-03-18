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
#include <SFML/Audio/SoundFileWriterWav.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Utils.hpp>

#include <algorithm>
#include <array>
#include <ostream>

#include <cassert>
#include <cstddef>
#include <cstdint>


namespace
{
// The following functions takes integers in host byte order
// and writes them to a stream as little endian

void encode(std::ostream& stream, std::int16_t value)
{
    const std::array bytes = {static_cast<char>(value & 0xFF), static_cast<char>(value >> 8)};
    stream.write(bytes.data(), bytes.size());
}

void encode(std::ostream& stream, std::uint16_t value)
{
    const std::array bytes = {static_cast<char>(value & 0xFF), static_cast<char>(value >> 8)};
    stream.write(bytes.data(), bytes.size());
}

void encode(std::ostream& stream, std::uint32_t value)
{
    const std::array bytes = {
        static_cast<char>(value & 0x000000FF),
        static_cast<char>((value & 0x0000FF00) >> 8),
        static_cast<char>((value & 0x00FF0000) >> 16),
        static_cast<char>((value & 0xFF000000) >> 24),
    };
    stream.write(bytes.data(), bytes.size());
}
} // namespace

namespace sf::priv
{
////////////////////////////////////////////////////////////
bool SoundFileWriterWav::check(const std::filesystem::path& filename)
{
    return toLower(filename.extension().string()) == ".wav";
}


////////////////////////////////////////////////////////////
SoundFileWriterWav::~SoundFileWriterWav()
{
    close();
}


////////////////////////////////////////////////////////////
bool SoundFileWriterWav::open(const std::filesystem::path&     filename,
                              unsigned int                     sampleRate,
                              unsigned int                     channelCount,
                              const std::vector<SoundChannel>& channelMap)
{
    auto channelMask = 0u;

    if (channelCount == 0)
    {
        err() << "WAV sound file channel count 0" << std::endl;
        return false;
    }

    if (channelCount == 1)
    {
        m_remapTable[0] = 0;
    }
    else if (channelCount == 2)
    {
        m_remapTable[0] = 0;
        m_remapTable[1] = 1;
    }
    else
    {
        // For WAVE channel mapping refer to: https://learn.microsoft.com/en-us/previous-versions/windows/hardware/design/dn653308(v=vs.85)#default-channel-ordering
        static constexpr auto speakerFrontLeft          = 0x1u;
        static constexpr auto speakerFrontRight         = 0x2u;
        static constexpr auto speakerFrontCenter        = 0x4u;
        static constexpr auto speakerLowFrequency       = 0x8u;
        static constexpr auto speakerBackLeft           = 0x10u;
        static constexpr auto speakerBackRight          = 0x20u;
        static constexpr auto speakerFrontLeftOfCenter  = 0x40u;
        static constexpr auto speakerFrontRightOfCenter = 0x80u;
        static constexpr auto speakerBackCenter         = 0x100u;
        static constexpr auto speakerSideLeft           = 0x200u;
        static constexpr auto speakerSideRight          = 0x400u;
        static constexpr auto speakerTopCenter          = 0x800u;
        static constexpr auto speakerTopFrontLeft       = 0x1000u;
        static constexpr auto speakerTopFrontCenter     = 0x2000u;
        static constexpr auto speakerTopFrontRight      = 0x4000u;
        static constexpr auto speakerTopBackLeft        = 0x8000u;
        static constexpr auto speakerTopBackCenter      = 0x10000u;
        static constexpr auto speakerTopBackRight       = 0x20000u;

        struct SupportedChannel
        {
            std::uint32_t bit;
            SoundChannel  channel;
        };

        std::vector<SupportedChannel>
            targetChannelMap{{speakerFrontLeft, SoundChannel::FrontLeft},
                             {speakerFrontRight, SoundChannel::FrontRight},
                             {speakerFrontCenter, SoundChannel::FrontCenter},
                             {speakerLowFrequency, SoundChannel::LowFrequencyEffects},
                             {speakerBackLeft, SoundChannel::BackLeft},
                             {speakerBackRight, SoundChannel::BackRight},
                             {speakerFrontLeftOfCenter, SoundChannel::FrontLeftOfCenter},
                             {speakerFrontRightOfCenter, SoundChannel::FrontRightOfCenter},
                             {speakerBackCenter, SoundChannel::BackCenter},
                             {speakerSideLeft, SoundChannel::SideLeft},
                             {speakerSideRight, SoundChannel::SideRight},
                             {speakerTopCenter, SoundChannel::TopCenter},
                             {speakerTopFrontLeft, SoundChannel::TopFrontLeft},
                             {speakerTopFrontCenter, SoundChannel::TopFrontCenter},
                             {speakerTopFrontRight, SoundChannel::TopFrontRight},
                             {speakerTopBackLeft, SoundChannel::TopBackLeft},
                             {speakerTopBackCenter, SoundChannel::TopBackCenter},
                             {speakerTopBackRight, SoundChannel::TopBackRight}};

        // Check for duplicate channel entries
        {
            auto sortedChannelMap = channelMap;
            std::sort(sortedChannelMap.begin(), sortedChannelMap.end());

            if (std::adjacent_find(sortedChannelMap.begin(), sortedChannelMap.end()) != sortedChannelMap.end())
            {
                err() << "Duplicate channels in channel map" << std::endl;
                return false;
            }
        }

        // Construct the target channel map by removing unused channels
        for (auto iter = targetChannelMap.begin(); iter != targetChannelMap.end();)
        {
            if (std::find(channelMap.begin(), channelMap.end(), iter->channel) == channelMap.end())
            {
                iter = targetChannelMap.erase(iter);
            }
            else
            {
                ++iter;
            }
        }

        // Verify that all the input channels exist in the target channel map
        for (const SoundChannel channel : channelMap)
        {
            if (std::find_if(targetChannelMap.begin(),
                             targetChannelMap.end(),
                             [channel](const SupportedChannel& c) { return c.channel == channel; }) ==
                targetChannelMap.end())
            {
                err() << "Could not map all input channels to a channel supported by WAV" << std::endl;
                return false;
            }
        }

        // Build the remap table
        for (auto i = 0u; i < channelCount; ++i)
            m_remapTable[i] = static_cast<std::size_t>(
                std::find(channelMap.begin(), channelMap.end(), targetChannelMap[i].channel) - channelMap.begin());

        // Generate the channel mask
        for (const auto& channel : targetChannelMap)
            channelMask |= channel.bit;
    }

    // Save the channel count
    m_channelCount = channelCount;

    // Open the file
    m_file.open(filename, std::ios::binary);
    if (!m_file)
    {
        err() << "Failed to open WAV sound file for writing\n" << formatDebugPathInfo(filename) << std::endl;
        return false;
    }

    // Write the header
    writeHeader(sampleRate, channelCount, channelMask);

    return true;
}


////////////////////////////////////////////////////////////
void SoundFileWriterWav::write(const std::int16_t* samples, std::uint64_t count)
{
    assert(m_file.good() && "Most recent I/O operation failed");
    assert(count % m_channelCount == 0);

    if (count % m_channelCount != 0)
        err() << "Writing samples to WAV sound file requires writing full frames at a time" << std::endl;

    while (count >= m_channelCount)
    {
        for (auto i = 0u; i < m_channelCount; ++i)
            encode(m_file, samples[m_remapTable[i]]);

        samples += m_channelCount;
        count -= m_channelCount;
    }
}


////////////////////////////////////////////////////////////
void SoundFileWriterWav::writeHeader(unsigned int sampleRate, unsigned int channelCount, unsigned int channelMask)
{
    assert(m_file.good() && "Most recent I/O operation failed");

    // Write the main chunk ID
    static constexpr std::array mainChunkId = {'R', 'I', 'F', 'F'};
    m_file.write(mainChunkId.data(), mainChunkId.size());

    // Write the main chunk header
    encode(m_file, std::uint32_t{0}); // 0 is a placeholder, will be written later
    static constexpr std::array mainChunkFormat = {'W', 'A', 'V', 'E'};
    m_file.write(mainChunkFormat.data(), mainChunkFormat.size());

    // Write the sub-chunk 1 ("format") id and size
    static constexpr std::array fmtChunkId = {'f', 'm', 't', ' '};
    m_file.write(fmtChunkId.data(), fmtChunkId.size());

    if (channelCount > 2)
    {
        const std::uint32_t fmtChunkSize = 40;
        encode(m_file, fmtChunkSize);

        // Write the format (Extensible)
        const std::uint16_t format = 65534;
        encode(m_file, format);
    }
    else
    {
        const std::uint32_t fmtChunkSize = 16;
        encode(m_file, fmtChunkSize);

        // Write the format (PCM)
        const std::uint16_t format = 1;
        encode(m_file, format);
    }

    // Write the sound attributes
    encode(m_file, static_cast<std::uint16_t>(channelCount));
    encode(m_file, sampleRate);
    const std::uint32_t byteRate = sampleRate * channelCount * 2;
    encode(m_file, byteRate);
    const auto blockAlign = static_cast<std::uint16_t>(channelCount * 2);
    encode(m_file, blockAlign);
    const std::uint16_t bitsPerSample = 16;
    encode(m_file, bitsPerSample);

    if (channelCount > 2)
    {
        const std::uint16_t extensionSize = 16;
        encode(m_file, extensionSize);
        encode(m_file, bitsPerSample);
        encode(m_file, channelMask);
        // Write the subformat (PCM)
        static constexpr std::array subformat =
            {'\x01', '\x00', '\x00', '\x00', '\x00', '\x00', '\x10', '\x00', '\x80', '\x00', '\x00', '\xAA', '\x00', '\x38', '\x9B', '\x71'};
        m_file.write(subformat.data(), subformat.size());
    }

    // Write the sub-chunk 2 ("data") id and size
    static constexpr std::array dataChunkId = {'d', 'a', 't', 'a'};
    m_file.write(dataChunkId.data(), dataChunkId.size());
    const std::uint32_t dataChunkSize = 0; // placeholder, will be written later
    encode(m_file, dataChunkSize);
}


////////////////////////////////////////////////////////////
void SoundFileWriterWav::close()
{
    // If the file is open, finalize the header and close it
    if (m_file.is_open())
    {
        m_file.flush();

        // Update the main chunk size and data sub-chunk size
        const std::uint32_t fileSize = static_cast<std::uint32_t>(m_file.tellp());
        m_file.seekp(4);
        encode(m_file, fileSize - 8); // 8 bytes RIFF header
        m_file.seekp(40);
        encode(m_file, fileSize - 44); // 44 bytes RIFF + WAVE headers

        m_file.close();
    }
}

} // namespace sf::priv
