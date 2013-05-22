////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_SOUNDFILE_HPP
#define SFML_SOUNDFILE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <sndfile.h>
#include <string>


namespace sf
{
class InputStream;

namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Provide read and write access to sound files
///
////////////////////////////////////////////////////////////
class SoundFile : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    SoundFile();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~SoundFile();

    ////////////////////////////////////////////////////////////
    /// \brief Get the total number of audio samples in the file
    ///
    /// \return Number of samples
    ///
    ////////////////////////////////////////////////////////////
    std::size_t getSampleCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the number of channels used by the sound
    ///
    /// \return Number of channels (1 = mono, 2 = stereo)
    ///
    ////////////////////////////////////////////////////////////
    unsigned int getChannelCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the sample rate of the sound
    ///
    /// \return Sample rate, in samples per second
    ///
    ////////////////////////////////////////////////////////////
    unsigned int getSampleRate() const;

    ////////////////////////////////////////////////////////////
    /// \brief Open a sound file for reading
    ///
    /// \param filename Path of the sound file to load
    ///
    /// \return True if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////
    bool openRead(const std::string& filename);

    ////////////////////////////////////////////////////////////
    /// \brief Open a sound file in memory for reading
    ///
    /// \param data        Pointer to the file data in memory
    /// \param sizeInBytes Size of the data to load, in bytes
    ///
    /// \return True if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////
    bool openRead(const void* data, std::size_t sizeInBytes);

    ////////////////////////////////////////////////////////////
    /// \brief Open a sound file from a custom stream for reading
    ///
    /// \param stream Source stream to read from
    ///
    /// \return True if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////
    bool openRead(InputStream& stream);

    ////////////////////////////////////////////////////////////
    /// \brief a the sound file for writing
    ///
    /// \param filename     Path of the sound file to write
    /// \param channelCount Number of channels in the sound
    /// \param sampleRate   Sample rate of the sound
    ///
    /// \return True if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////
    bool openWrite(const std::string& filename, unsigned int channelCount, unsigned int sampleRate);

    ////////////////////////////////////////////////////////////
    /// \brief Read audio samples from the loaded sound
    ///
    /// \param data        Pointer to the sample array to fill
    /// \param sampleCount Number of samples to read
    ///
    /// \return Number of samples actually read (may be less than \a sampleCount)
    ///
    ////////////////////////////////////////////////////////////
    std::size_t read(Int16* data, std::size_t sampleCount);

    ////////////////////////////////////////////////////////////
    /// \brief Write audio samples to the file
    ///
    /// \param data        Pointer to the sample array to write
    /// \param sampleCount Number of samples to write
    ///
    ////////////////////////////////////////////////////////////
    void write(const Int16* data, std::size_t sampleCount);

    ////////////////////////////////////////////////////////////
    /// \brief Change the current read position in the file
    ///
    /// \param timeOffset New playing position, from the beginning of the file
    ///
    ////////////////////////////////////////////////////////////
    void seek(Time timeOffset);

private :

    ////////////////////////////////////////////////////////////
    /// \brief Initialize the internal state of the sound file
    ///
    /// This function is called by all the openRead functions.
    ///
    /// \param fileInfo Information about the loaded sound file
    ///
    ////////////////////////////////////////////////////////////
    void initialize(SF_INFO fileInfo);

    ////////////////////////////////////////////////////////////
    /// \brief Get the internal format of an audio file according to
    ///        its filename extension
    ///
    /// \param filename Filename to check
    ///
    /// \return Internal format matching the filename (-1 if no match)
    ///
    ////////////////////////////////////////////////////////////
    static int getFormatFromFilename(const std::string& filename);

    ////////////////////////////////////////////////////////////
    /// \brief Data and callbacks for opening from memory
    ///
    ////////////////////////////////////////////////////////////
    struct Memory
    {
        const char* begin;
        const char* current;
        sf_count_t  size;

        static sf_count_t getLength(void* user);
        static sf_count_t read(void* ptr, sf_count_t count, void* user);
        static sf_count_t seek(sf_count_t offset, int whence, void* user);
        static sf_count_t tell(void* user);
    };

    ////////////////////////////////////////////////////////////
    /// \brief Data and callbacks for opening from stream
    ///
    ////////////////////////////////////////////////////////////
    struct Stream
    {
        InputStream* source;
        Int64 size;

        static sf_count_t getLength(void* user);
        static sf_count_t read(void* ptr, sf_count_t count, void* user);
        static sf_count_t seek(sf_count_t offset, int whence, void* user);
        static sf_count_t tell(void* user);
    };

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    SNDFILE*     m_file;         ///< File descriptor
    Memory       m_memory;       ///< Memory reading info
    Stream       m_stream;       ///< Stream reading info
    std::size_t  m_sampleCount;  ///< Total number of samples in the file
    unsigned int m_channelCount; ///< Number of channels used by the sound
    unsigned int m_sampleRate;   ///< Number of samples per second
};

} // namespace priv

} // namespace sf


#endif // SFML_SOUNDFILE_HPP
