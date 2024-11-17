////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Audio/Export.hpp>

#include <SFML/Audio/SoundChannel.hpp>
#include <SFML/Audio/SoundFileWriter.hpp>

#include <filesystem>
#include <memory>
#include <vector>

#include <cstdint>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Provide write access to sound files
///
////////////////////////////////////////////////////////////
class SFML_AUDIO_API OutputSoundFile
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Construct an output sound file that is not associated
    /// with a file to write.
    ///
    ////////////////////////////////////////////////////////////
    OutputSoundFile() = default;

    ////////////////////////////////////////////////////////////
    /// \brief Construct the sound file from the disk for writing
    ///
    /// The supported audio formats are: WAV, OGG/Vorbis, FLAC.
    ///
    /// \param filename     Path of the sound file to write
    /// \param sampleRate   Sample rate of the sound
    /// \param channelCount Number of channels in the sound
    /// \param channelMap   Map of position in sample frame to sound channel
    ///
    /// \throws sf::Exception if the file could not be opened successfully
    ///
    ////////////////////////////////////////////////////////////
    OutputSoundFile(const std::filesystem::path&     filename,
                    unsigned int                     sampleRate,
                    unsigned int                     channelCount,
                    const std::vector<SoundChannel>& channelMap);

    ////////////////////////////////////////////////////////////
    /// \brief Open the sound file from the disk for writing
    ///
    /// The supported audio formats are: WAV, OGG/Vorbis, FLAC.
    ///
    /// \param filename     Path of the sound file to write
    /// \param sampleRate   Sample rate of the sound
    /// \param channelCount Number of channels in the sound
    /// \param channelMap   Map of position in sample frame to sound channel
    ///
    /// \return `true` if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool openFromFile(const std::filesystem::path&     filename,
                                    unsigned int                     sampleRate,
                                    unsigned int                     channelCount,
                                    const std::vector<SoundChannel>& channelMap);

    ////////////////////////////////////////////////////////////
    /// \brief Write audio samples to the file
    ///
    /// \param samples     Pointer to the sample array to write
    /// \param count       Number of samples to write
    ///
    ////////////////////////////////////////////////////////////
    void write(const std::int16_t* samples, std::uint64_t count);

    ////////////////////////////////////////////////////////////
    /// \brief Close the current file
    ///
    ////////////////////////////////////////////////////////////
    void close();

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::unique_ptr<SoundFileWriter> m_writer; //!< Writer that handles I/O on the file's format
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::OutputSoundFile
/// \ingroup audio
///
/// This class encodes audio samples to a sound file. It is
/// used internally by higher-level classes such as `sf::SoundBuffer`,
/// but can also be useful if you want to create audio files from
/// custom data sources, like generated audio samples.
///
/// Usage example:
/// \code
/// // Create a sound file, ogg/vorbis format, 44100 Hz, stereo
/// sf::OutputSoundFile file("music.ogg", 44100, 2, {sf::SoundChannel::FrontLeft, sf::SoundChannel::FrontRight});
///
/// while (...)
/// {
///     // Read or generate audio samples from your custom source
///     std::vector<std::int16_t> samples = ...;
///
///     // Write them to the file
///     file.write(samples.data(), samples.size());
/// }
/// \endcode
///
/// \see `sf::SoundFileWriter`, `sf::InputSoundFile`
///
////////////////////////////////////////////////////////////
