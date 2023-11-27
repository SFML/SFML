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
#include <SFML/Audio/SoundFileWriterFlac.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Utils.hpp>

#include <algorithm>
#include <ostream>

#include <cassert>
#include <cctype>


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
bool SoundFileWriterFlac::open(const std::filesystem::path& filename, unsigned int sampleRate, unsigned int channelCount)
{
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

        // Convert the samples to 32-bits
        m_samples32.assign(samples, samples + frames * m_channelCount);

        // Write them to the FLAC stream
        FLAC__stream_encoder_process_interleaved(m_encoder.get(), m_samples32.data(), frames);

        // Next chunk
        count -= m_samples32.size();
        samples += m_samples32.size();
    }
}

} // namespace sf::priv
