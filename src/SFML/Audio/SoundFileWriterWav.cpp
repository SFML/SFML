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
#include <SFML/Audio/SoundFileWriterWav.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Utils.hpp>

#include <algorithm>
#include <ostream>

#include <cstdint>
#include <cassert>
#include <cstddef>


namespace
{
// The following functions takes integers in host byte order
// and writes them to a stream as little endian

void encode(std::ostream& stream, std::int16_t value)
{
    const std::byte bytes[] = {static_cast<std::byte>(value & 0xFF), static_cast<std::byte>(value >> 8)};
    stream.write(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

void encode(std::ostream& stream, std::uint16_t value)
{
    const std::byte bytes[] = {static_cast<std::byte>(value & 0xFF), static_cast<std::byte>(value >> 8)};
    stream.write(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

void encode(std::ostream& stream, std::uint32_t value)
{
    const std::byte bytes[] = {
        static_cast<std::byte>(value & 0x000000FF),
        static_cast<std::byte>((value & 0x0000FF00) >> 8),
        static_cast<std::byte>((value & 0x00FF0000) >> 16),
        static_cast<std::byte>((value & 0xFF000000) >> 24),
    };
    stream.write(reinterpret_cast<const char*>(bytes), sizeof(bytes));
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
SoundFileWriterWav::SoundFileWriterWav() = default;


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
    else if (channelCount == 1)
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
        static constexpr auto SPEAKER_FRONT_LEFT            = 0x1u;
        static constexpr auto SPEAKER_FRONT_RIGHT           = 0x2u;
        static constexpr auto SPEAKER_FRONT_CENTER          = 0x4u;
        static constexpr auto SPEAKER_LOW_FREQUENCY         = 0x8u;
        static constexpr auto SPEAKER_BACK_LEFT             = 0x10u;
        static constexpr auto SPEAKER_BACK_RIGHT            = 0x20u;
        static constexpr auto SPEAKER_FRONT_LEFT_OF_CENTER  = 0x40u;
        static constexpr auto SPEAKER_FRONT_RIGHT_OF_CENTER = 0x80u;
        static constexpr auto SPEAKER_BACK_CENTER           = 0x100u;
        static constexpr auto SPEAKER_SIDE_LEFT             = 0x200u;
        static constexpr auto SPEAKER_SIDE_RIGHT            = 0x400u;
        static constexpr auto SPEAKER_TOP_CENTER            = 0x800u;
        static constexpr auto SPEAKER_TOP_FRONT_LEFT        = 0x1000u;
        static constexpr auto SPEAKER_TOP_FRONT_CENTER      = 0x2000u;
        static constexpr auto SPEAKER_TOP_FRONT_RIGHT       = 0x4000u;
        static constexpr auto SPEAKER_TOP_BACK_LEFT         = 0x8000u;
        static constexpr auto SPEAKER_TOP_BACK_CENTER       = 0x10000u;
        static constexpr auto SPEAKER_TOP_BACK_RIGHT        = 0x20000u;

        struct SupportedChannel
        {
            std::uint32_t bit;
            SoundChannel  channel;
        };

        std::vector<SupportedChannel> targetChannelMap;

        targetChannelMap.push_back(SupportedChannel{SPEAKER_FRONT_LEFT, SoundChannel::FrontLeft});
        targetChannelMap.push_back(SupportedChannel{SPEAKER_FRONT_RIGHT, SoundChannel::FrontRight});
        targetChannelMap.push_back(SupportedChannel{SPEAKER_FRONT_CENTER, SoundChannel::FrontCenter});
        targetChannelMap.push_back(SupportedChannel{SPEAKER_LOW_FREQUENCY, SoundChannel::LowFrequencyEffects});
        targetChannelMap.push_back(SupportedChannel{SPEAKER_BACK_LEFT, SoundChannel::BackLeft});
        targetChannelMap.push_back(SupportedChannel{SPEAKER_BACK_RIGHT, SoundChannel::BackRight});
        targetChannelMap.push_back(SupportedChannel{SPEAKER_FRONT_LEFT_OF_CENTER, SoundChannel::FrontLeftOfCenter});
        targetChannelMap.push_back(SupportedChannel{SPEAKER_FRONT_RIGHT_OF_CENTER, SoundChannel::FrontRightOfCenter});
        targetChannelMap.push_back(SupportedChannel{SPEAKER_BACK_CENTER, SoundChannel::BackCenter});
        targetChannelMap.push_back(SupportedChannel{SPEAKER_SIDE_LEFT, SoundChannel::SideLeft});
        targetChannelMap.push_back(SupportedChannel{SPEAKER_SIDE_RIGHT, SoundChannel::SideRight});
        targetChannelMap.push_back(SupportedChannel{SPEAKER_TOP_CENTER, SoundChannel::TopCenter});
        targetChannelMap.push_back(SupportedChannel{SPEAKER_TOP_FRONT_LEFT, SoundChannel::TopFrontLeft});
        targetChannelMap.push_back(SupportedChannel{SPEAKER_TOP_FRONT_CENTER, SoundChannel::TopFrontCenter});
        targetChannelMap.push_back(SupportedChannel{SPEAKER_TOP_FRONT_RIGHT, SoundChannel::TopFrontRight});
        targetChannelMap.push_back(SupportedChannel{SPEAKER_TOP_BACK_LEFT, SoundChannel::TopBackLeft});
        targetChannelMap.push_back(SupportedChannel{SPEAKER_TOP_BACK_CENTER, SoundChannel::TopBackCenter});
        targetChannelMap.push_back(SupportedChannel{SPEAKER_TOP_BACK_RIGHT, SoundChannel::TopBackRight});

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
        for (auto channel : channelMap)
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
            m_remapTable[i] = std::find(channelMap.begin(), channelMap.end(), targetChannelMap[i].channel) -
                              channelMap.begin();

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
    if (!writeHeader(sampleRate, channelCount, channelMask))
    {
        err() << "Failed to write header of WAV sound file\n" << formatDebugPathInfo(filename) << std::endl;
        return false;
    }

    return true;
}


////////////////////////////////////////////////////////////
void SoundFileWriterWav::write(const std::int16_t* samples, std::uint64_t count)
{
    assert(m_file.good());
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
bool SoundFileWriterWav::writeHeader(unsigned int sampleRate, unsigned int channelCount, unsigned int channelMask)
{
    assert(m_file.good());

    // Write the main chunk ID
    char mainChunkId[4] = {'R', 'I', 'F', 'F'};
    m_file.write(mainChunkId, sizeof(mainChunkId));

    // Write the main chunk header
    encode(m_file, static_cast<std::uint32_t>(0)); // 0 is a placeholder, will be written later
    char mainChunkFormat[4] = {'W', 'A', 'V', 'E'};
    m_file.write(mainChunkFormat, sizeof(mainChunkFormat));

    // Write the sub-chunk 1 ("format") id and size
    char fmtChunkId[4] = {'f', 'm', 't', ' '};
    m_file.write(fmtChunkId, sizeof(fmtChunkId));

    if (channelCount > 2)
    {
        std::uint32_t fmtChunkSize = 40;
        encode(m_file, fmtChunkSize);

        // Write the format (Extensible)
        std::uint16_t format = 65534;
        encode(m_file, format);
    }
    else
    {
        std::uint32_t fmtChunkSize = 16;
        encode(m_file, fmtChunkSize);

        // Write the format (PCM)
        std::uint16_t format = 1;
        encode(m_file, format);
    }

    // Write the sound attributes
    encode(m_file, static_cast<std::uint16_t>(channelCount));
    encode(m_file, sampleRate);
    std::uint32_t byteRate = sampleRate * channelCount * 2;
    encode(m_file, byteRate);
    auto blockAlign = static_cast<std::uint16_t>(channelCount * 2);
    encode(m_file, blockAlign);
    std::uint16_t bitsPerSample = 16;
    encode(m_file, bitsPerSample);

    if (channelCount > 2)
    {
        std::uint16_t extensionSize = 16;
        encode(m_file, extensionSize);
        encode(m_file, bitsPerSample);
        encode(m_file, channelMask);
        // Write the subformat (PCM)
        char subformat[16] =
            {'\x01', '\x00', '\x00', '\x00', '\x00', '\x00', '\x10', '\x00', '\x80', '\x00', '\x00', '\xAA', '\x00', '\x38', '\x9B', '\x71'};
        m_file.write(subformat, sizeof(subformat));
    }

    // Write the sub-chunk 2 ("data") id and size
    char dataChunkId[4] = {'d', 'a', 't', 'a'};
    m_file.write(dataChunkId, sizeof(dataChunkId));
    std::uint32_t dataChunkSize = 0; // placeholder, will be written later
    encode(m_file, dataChunkSize);

    return true;
}


////////////////////////////////////////////////////////////
void SoundFileWriterWav::close()
{
    // If the file is open, finalize the header and close it
    if (m_file.is_open())
    {
        m_file.flush();

        // Update the main chunk size and data sub-chunk size
        std::uint32_t fileSize = static_cast<std::uint32_t>(m_file.tellp());
        m_file.seekp(4);
        encode(m_file, fileSize - 8); // 8 bytes RIFF header
        m_file.seekp(40);
        encode(m_file, fileSize - 44); // 44 bytes RIFF + WAVE headers

        m_file.close();
    }
}

} // namespace sf::priv
