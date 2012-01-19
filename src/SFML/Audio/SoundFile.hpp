////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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
    std::size_t GetSampleCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the number of channels used by the sound
    ///
    /// \return Number of channels (1 = mono, 2 = stereo)
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetChannelCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the sample rate of the sound
    ///
    /// \return Sample rate, in samples per second
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetSampleRate() const;

    ////////////////////////////////////////////////////////////
    /// \brief Open a sound file for reading
    ///
    /// \param filename Path of the sound file to load
    ///
    /// \return True if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////
    bool OpenRead(const std::string& filename);

    ////////////////////////////////////////////////////////////
    /// \brief Open a sound file in memory for reading
    ///
    /// \param data        Pointer to the file data in memory
    /// \param sizeInBytes Size of the data to load, in bytes
    ///
    /// \return True if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////
    bool OpenRead(const void* data, std::size_t sizeInBytes);

    ////////////////////////////////////////////////////////////
    /// \brief Open a sound file from a custom stream for reading
    ///
    /// \param stream Source stream to read from
    ///
    /// \return True if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////
    bool OpenRead(InputStream& stream);

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
    bool OpenWrite(const std::string& filename, unsigned int channelCount, unsigned int sampleRate);

    ////////////////////////////////////////////////////////////
    /// \brief Read audio samples from the loaded sound
    ///
    /// \param data        Pointer to the sample array to fill
    /// \param sampleCount Number of samples to read
    ///
    /// \return Number of samples actually read (may be less than \a sampleCount)
    ///
    ////////////////////////////////////////////////////////////
    std::size_t Read(Int16* data, std::size_t sampleCount);

    ////////////////////////////////////////////////////////////
    /// \brief Write audio samples to the file
    ///
    /// \param data        Pointer to the sample array to write
    /// \param sampleCount Number of samples to write
    ///
    ////////////////////////////////////////////////////////////
    void Write(const Int16* data, std::size_t sampleCount);

    ////////////////////////////////////////////////////////////
    /// \brief Change the current read position in the file
    ///
    /// \param timeOffset New playing position, from the beginning of the file
    ///
    ////////////////////////////////////////////////////////////
    void Seek(Time timeOffset);

private :

    ////////////////////////////////////////////////////////////
    /// \brief Get the internal format of an audio file according to
    ///        its filename extension
    ///
    /// \param filename Filename to check
    ///
    /// \return Internal format matching the filename (-1 if no match)
    ///
    ////////////////////////////////////////////////////////////
    static int GetFormatFromFilename(const std::string& filename);

    ////////////////////////////////////////////////////////////
    /// \brief Data and callbacks for opening from memory
    ///
    ////////////////////////////////////////////////////////////
    struct Memory
    {
        const char* DataStart;
        const char* DataPtr;
        sf_count_t  TotalSize;

        static sf_count_t GetLength(void* user);
        static sf_count_t Read(void* ptr, sf_count_t count, void* user);
        static sf_count_t Seek(sf_count_t offset, int whence, void* user);
        static sf_count_t Tell(void* user);
    };

    ////////////////////////////////////////////////////////////
    /// \brief Callbacks for opening from stream
    ///
    ////////////////////////////////////////////////////////////
    struct Stream
    {
        static sf_count_t GetLength(void* user);
        static sf_count_t Read(void* ptr, sf_count_t count, void* user);
        static sf_count_t Seek(sf_count_t offset, int whence, void* user);
        static sf_count_t Tell(void* user);
    };

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    SNDFILE*     myFile;         ///< File descriptor
    Memory       myMemory;       ///< Memory reading info
    std::size_t  mySampleCount;  ///< Total number of samples in the file
    unsigned int myChannelCount; ///< Number of channels used by the sound
    unsigned int mySampleRate;   ///< Number of samples per second
};

} // namespace priv

} // namespace sf


#endif // SFML_SOUNDFILE_HPP
