////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2026 Laurent Gomila (laurent@sfml-dev.org)
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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/SoundFileReader.hpp>

#include <string_view>

#include <cstdint>


namespace sf
{
class InputStream;
}

namespace sf::priv
{

////////////////////////////////////////////////////////////
/// \brief Implementation of sound file reader that handles qoa files
///
////////////////////////////////////////////////////////////
class SoundFileReaderQoa : public SoundFileReader
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Check if this reader can handle a file given by an input stream
    ///
    /// \param stream Source stream to check
    ///
    /// \return `true` if the file is supported by this reader
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static bool check(InputStream& stream);

    ////////////////////////////////////////////////////////////
    /// \brief Open a sound file for reading
    ///
    /// \param stream Stream to open
    ///
    /// \return Properties of the loaded sound if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::optional<Info> open(InputStream& stream) override;

    ////////////////////////////////////////////////////////////
    /// \brief Change the current read position to the given sample offset
    ///
    /// The sample offset takes the channels into account.
    /// If you have a time offset instead, you can easily find
    /// the corresponding sample offset with the following formula:
    /// `timeInSeconds * sampleRate * channelCount`
    /// If the given offset exceeds to total number of samples,
    /// this function must jump to the end of the file.
    ///
    /// \param sampleOffset Index of the sample to jump to, relative to the beginning
    ///
    ////////////////////////////////////////////////////////////
    void seek(std::uint64_t sampleOffset) override;

    ////////////////////////////////////////////////////////////
    /// \brief Read audio samples from the open file
    ///
    /// \param samples  Pointer to the sample array to fill
    /// \param maxCount Maximum number of samples to read
    ///
    /// \return Number of samples actually read (may be less than \a maxCount)
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::uint64_t read(std::int16_t* samples, std::uint64_t maxCount) override;

private:
    ////////////////////////////////////////////////////////////
    /// \brief Read audio samples from last decoded frame
    ///
    /// \param samples  Pointer to the sample array to fill
    /// \param maxCount Maximum number of samples to read
    ///
    /// \return Number of samples actually read (may be less than \a maxCount)
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::uint16_t readCurrentDecodedFrame(std::int16_t* samples, std::uint32_t maxCount);

    ////////////////////////////////////////////////////////////
    /// \brief Decode the next frame in the stream
    ///
    /// \return An error message on failure and std::nullopt on success
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::optional<std::string_view> decodeNextFrame();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::uint8_t              m_channelCount{};             //!< Number of channels
    std::uint32_t             m_decodedSamplesPerChannel{}; //!< Number of total samples decoded per channel
    std::uint32_t             m_sampleRate{};               //!< Number of samples rate per channel
    std::uint32_t             m_samplesPerChannel{}; //!< Number of total samples per channel or zero if unspecified
    std::vector<std::int16_t> m_currentFrameSamples; //!< Decoded samples of last processed frame
    std::uint16_t m_currentFrameNextSampleIndex{};   //!< Index of next unread sample in the last processed frame
    InputStream*  m_inputStream{};                   //!< The input stream received in the open method
    std::size_t   m_streamFirstFramePosition{};
};

} // namespace sf::priv
