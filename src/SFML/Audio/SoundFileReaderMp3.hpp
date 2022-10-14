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

#ifndef SFML_SOUNDFILEREADERMP3_HPP
#define SFML_SOUNDFILEREADERMP3_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#ifndef NOMINMAX
#define NOMINMAX // To avoid windows.h and std::min issue
#endif
#define MINIMP3_NO_STDIO // Minimp3 control define, eliminate file manipulation code which is useless here

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4242 4244 4267 4456 4706)
#endif

#include <minimp3_ex.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#undef NOMINMAX
#undef MINIMP3_NO_STDIO

#include <SFML/Audio/SoundFileReader.hpp>

#include <vector>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Implementation of sound file reader that handles MP3 files
///
////////////////////////////////////////////////////////////
class SoundFileReaderMp3 : public SoundFileReader
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Check if this reader can handle a file given by an input stream
    ///
    /// \param stream Source stream to check
    ///
    /// \return True if the file is supported by this reader
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static bool check(InputStream& stream);

public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    SoundFileReaderMp3();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~SoundFileReaderMp3() override;

    ////////////////////////////////////////////////////////////
    /// \brief Open a sound file for reading
    ///
    /// \param stream Source stream to read from
    /// \param info   Structure to fill with the properties of the loaded sound
    ///
    /// \return True if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool open(InputStream& stream, Info& info) override;

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
    // Member data
    ////////////////////////////////////////////////////////////
    mp3dec_io_t   m_io;
    mp3dec_ex_t   m_decoder;
    std::uint64_t m_numSamples; // Decompressed audio storage size
    std::uint64_t m_position;   // Position in decompressed audio buffer
};

} // namespace priv

} // namespace sf


#endif // SFML_SOUNDFILEREADERMP3_HPP
