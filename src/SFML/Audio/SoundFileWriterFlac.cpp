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
#include <SFML/Audio/SoundFileWriterFlac.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Utils.hpp>

#include <algorithm>
#include <ostream>


namespace sf::priv
{
////////////////////////////////////////////////////////////
void SoundFileWriterFlac::FlacStreamEncoderDeleter::operator()(FLAC__StreamEncoder* encoder) const
{
    FLAC__stream_encoder_finish(encoder);
    FLAC__stream_encoder_delete(encoder);
}


////////////////////////////////////////////////////////////
bool SoundFileWriterFlac::check(const std::filesystem::path& filename)
{
    return toLower(filename.extension().string()) == ".flac";
}


////////////////////////////////////////////////////////////
bool SoundFileWriterFlac::open(const std::filesystem::path&     filename,
                               unsigned int                     sampleRate,
                               unsigned int                     channelCount,
                               const std::vector<SoundChannel>& channelMap)
{
    std::vector<SoundChannel> targetChannelMap;

    // For FLAC channel mapping refer to: https://xiph.org/flac/format.html#frame_header
    switch (channelCount)
    {
        case 0:
            err() << "No channels to write to FLAC file" << std::endl;
            return false;
        case 1:
            targetChannelMap = {SoundChannel::Mono};
            break;
        case 2:
            targetChannelMap = {SoundChannel::FrontLeft, SoundChannel::FrontRight};
            break;
        case 3:
            targetChannelMap = {SoundChannel::FrontLeft, SoundChannel::FrontRight, SoundChannel::FrontCenter};
            break;
        case 4:
            targetChannelMap = {SoundChannel::FrontLeft, SoundChannel::FrontRight, SoundChannel::BackLeft, SoundChannel::BackRight};
            break;
        case 5:
            targetChannelMap = {SoundChannel::FrontLeft,
                                SoundChannel::FrontRight,
                                SoundChannel::FrontCenter,
                                SoundChannel::BackLeft,
                                SoundChannel::BackRight};
            break;
        case 6:
            targetChannelMap = {SoundChannel::FrontLeft,
                                SoundChannel::FrontRight,
                                SoundChannel::FrontCenter,
                                SoundChannel::LowFrequencyEffects,
                                SoundChannel::BackLeft,
                                SoundChannel::BackRight};
            break;
        case 7:
            targetChannelMap = {SoundChannel::FrontLeft,
                                SoundChannel::FrontRight,
                                SoundChannel::FrontCenter,
                                SoundChannel::LowFrequencyEffects,
                                SoundChannel::BackCenter,
                                SoundChannel::SideLeft,
                                SoundChannel::SideRight};
            break;
        case 8:
            targetChannelMap = {SoundChannel::FrontLeft,
                                SoundChannel::FrontRight,
                                SoundChannel::FrontCenter,
                                SoundChannel::LowFrequencyEffects,
                                SoundChannel::BackLeft,
                                SoundChannel::BackRight,
                                SoundChannel::SideLeft,
                                SoundChannel::SideRight};
            break;
        default:
            err() << "FLAC files with more than 8 channels not supported" << std::endl;
            return false;
    }

    // Check if the channel map contains channels that we cannot remap to a mapping supported by FLAC
    if (!std::is_permutation(channelMap.begin(), channelMap.end(), targetChannelMap.begin()))
    {
        err() << "Provided channel map cannot be reordered to a channel map supported by FLAC" << std::endl;
        return false;
    }

    // Build the remap rable
    for (auto i = 0u; i < channelCount; ++i)
        m_remapTable[i] = static_cast<std::size_t>(
            std::find(channelMap.begin(), channelMap.end(), targetChannelMap[i]) - channelMap.begin());

    // Create the encoder
    m_encoder.reset(FLAC__stream_encoder_new());
    if (!m_encoder)
    {
        err() << "Failed to write flac file (failed to allocate encoder)\n"
              << formatDebugPathInfo(filename) << std::endl;
        return false;
    }

    // Setup the encoder
    FLAC__stream_encoder_set_channels(m_encoder.get(), channelCount);
    FLAC__stream_encoder_set_bits_per_sample(m_encoder.get(), 16);
    FLAC__stream_encoder_set_sample_rate(m_encoder.get(), sampleRate);

    // Initialize the output stream
    if (FLAC__stream_encoder_init_file(m_encoder.get(), filename.string().c_str(), nullptr, nullptr) !=
        FLAC__STREAM_ENCODER_INIT_STATUS_OK)
    {
        err() << "Failed to write flac file (failed to open the file)\n" << formatDebugPathInfo(filename) << std::endl;
        m_encoder.reset();
        return false;
    }

    // Store the channel count
    m_channelCount = channelCount;

    return true;
}


////////////////////////////////////////////////////////////
void SoundFileWriterFlac::write(const std::int16_t* samples, std::uint64_t count)
{
    while (count > 0)
    {
        // Make sure that we don't process too many samples at once
        const unsigned int frames = std::min(static_cast<unsigned int>(count / m_channelCount), 10000u);

        // Convert the samples to 32-bits and remap the channels
        m_samples32.clear();
        m_samples32.reserve(frames * m_channelCount);

        for (auto frame = 0u; frame < frames; ++frame)
        {
            for (auto channel = 0u; channel < m_channelCount; ++channel)
                m_samples32.push_back(samples[frame * m_channelCount + m_remapTable[channel]]);
        }

        // Write them to the FLAC stream
        FLAC__stream_encoder_process_interleaved(m_encoder.get(), m_samples32.data(), frames);

        // Next chunk
        count -= m_samples32.size();
        samples += m_samples32.size();
    }
}

} // namespace sf::priv
