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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/SoundFileWriter.hpp>

#include <FLAC/stream_encoder.h>
#include <filesystem>
#include <vector>


namespace sf::priv
{
////////////////////////////////////////////////////////////
/// \brief Implementation of sound file writer that handles FLAC files
///
////////////////////////////////////////////////////////////
class SoundFileWriterFlac : public SoundFileWriter
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Check if this writer can handle a file on disk
    ///
    /// \param filename Path of the sound file to check
    ///
    /// \return True if the file can be written by this writer
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static bool check(const std::filesystem::path& filename);

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    SoundFileWriterFlac();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~SoundFileWriterFlac() override;

    ////////////////////////////////////////////////////////////
    /// \brief Open a sound file for writing
    ///
    /// \param filename     Path of the file to open
    /// \param sampleRate   Sample rate of the sound
    /// \param channelCount Number of channels of the sound
    ///
    /// \return True if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool open(const std::filesystem::path& filename, unsigned int sampleRate, unsigned int channelCount) override;

    ////////////////////////////////////////////////////////////
    /// \brief Write audio samples to the open file
    ///
    /// \param samples Pointer to the sample array to write
    /// \param count   Number of samples to write
    ///
    ////////////////////////////////////////////////////////////
    void write(const std::int16_t* samples, std::uint64_t count) override;

private:
    ////////////////////////////////////////////////////////////
    /// \brief Close the file
    ///
    ////////////////////////////////////////////////////////////
    void close();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    FLAC__StreamEncoder*      m_encoder{};      //!< FLAC stream encoder
    unsigned int              m_channelCount{}; //!< Number of channels
    std::vector<std::int32_t> m_samples32;      //!< Conversion buffer
};

} // namespace sf::priv
