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
#include <SFML/Audio/SoundFileWriter.hpp>

#include <filesystem>
#include <fstream>
#include <optional>
#include <string_view>
#include <vector>

#include <cstdint>


namespace sf::priv
{
namespace qoaFile
{
struct LmsState;
} // namespace qoaFile

////////////////////////////////////////////////////////////
/// \brief Implementation of sound file writer that handles qoa files
///
////////////////////////////////////////////////////////////
class SoundFileWriterQoa : public SoundFileWriter
{
public:
    ////////////////////////////////////////////////////////////
    ///
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~SoundFileWriterQoa() override;

    ////////////////////////////////////////////////////////////
    /// \brief Check if this writer can handle a file on disk
    ///
    /// \param filename Path of the sound file to check
    ///
    /// \return `true` if the file can be written by this writer
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static bool check(const std::filesystem::path& filename);

    ////////////////////////////////////////////////////////////
    /// \brief Open a sound file for writing
    ///
    /// \param filename     Path of the file to open
    /// \param sampleRate   Sample rate of the sound
    /// \param channelCount Number of channels of the sound
    /// \param channelMap   Map of position in sample frame to sound channel
    ///
    /// \return `true` if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool open(const std::filesystem::path&     filename,
                            unsigned int                     sampleRate,
                            unsigned int                     channelCount,
                            const std::vector<SoundChannel>& channelMap) override;

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
    /// \brief Fill the header of the file
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::optional<std::string_view> seekAndWriteHeader(std::uint32_t              samplesPerChannel,
                                                                     std::vector<std::uint8_t>& reusedContainer);

    ////////////////////////////////////////////////////////////
    /// \brief Write the whole content of a single frame
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::optional<std::string_view> writeFrame(const std::int16_t*        samples,
                                                             std::uint16_t              count,
                                                             qoaFile::LmsState&         lmsState,
                                                             std::vector<std::uint8_t>& reusedContainer);

    struct FrameSharedData
    {
        std::uint8_t  numChannels;
        std::uint32_t sampleRate;

        [[nodiscard]] std::optional<std::string_view> writeFrameHeader(std::uint16_t  samplesPerChannel,
                                                                       std::ofstream& stream,
                                                                       std::vector<std::uint8_t>& reusedContainer) const;
    };

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::ofstream                  m_file;            //!< File stream to write to
    std::vector<std::uint8_t>      m_remapTable;      //!< Mapping from input channel order to QOA channel order
    std::optional<FrameSharedData> m_frameSharedData; //!< Common data of frames, used for header
};

} // namespace sf::priv
