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
#include <sndfile.h>
#include <string>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// SoundFile is used to load and save various sampled
/// sound file formats
////////////////////////////////////////////////////////////
class SoundFile : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    SoundFile();

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~SoundFile();

    ////////////////////////////////////////////////////////////
    /// Get the total number of samples in the file
    ///
    /// \return Number of samples
    ///
    ////////////////////////////////////////////////////////////
    std::size_t GetSamplesCount() const;

    ////////////////////////////////////////////////////////////
    /// Get the number of channels used by the sound
    ///
    /// \return Number of channels (1 = mono, 2 = stereo)
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetChannelsCount() const;

    ////////////////////////////////////////////////////////////
    /// Get the sample rate of the sound
    ///
    /// \return Sample rate, in samples / sec
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetSampleRate() const;

    ////////////////////////////////////////////////////////////
    /// Open the sound file for reading
    ///
    /// \param filename : Path of sound file to load
    ///
    /// \return True if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////
    bool OpenRead(const std::string& filename);

    ////////////////////////////////////////////////////////////
    /// Open the sound file in memory for reading
    ///
    /// \param data :        Pointer to the file data in memory
    /// \param sizeInBytes : Size of the data to load, in bytes
    ///
    /// \return True if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////
    bool OpenRead(const char* data, std::size_t sizeInBytes);

    ////////////////////////////////////////////////////////////
    /// Open the sound file for writing
    ///
    /// \param filename :      Path of sound file to write
    /// \param channelsCount : Number of channels in the sound
    /// \param sampleRate :    Sample rate of the sound
    ///
    /// \return True if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////
    bool OpenWrite(const std::string& filename, unsigned int channelsCount, unsigned int sampleRate);

    ////////////////////////////////////////////////////////////
    /// Read samples from the loaded sound
    ///
    /// \param data :      Pointer to the samples array to fill
    /// \param nbSamples : Number of samples to read
    ///
    /// \return Number of samples read
    ///
    ////////////////////////////////////////////////////////////
    std::size_t Read(Int16* data, std::size_t nbSamples);

    ////////////////////////////////////////////////////////////
    /// Write samples to the file
    ///
    /// \param data :      Pointer to the samples array to write
    /// \param nbSamples : Number of samples to write
    ///
    ////////////////////////////////////////////////////////////
    void Write(const Int16* data, std::size_t nbSamples);

    ////////////////////////////////////////////////////////////
    /// Move the current reading position in the file
    ///
    /// \param timeOffset : New position, expressed in seconds
    ///
    ////////////////////////////////////////////////////////////
    void Seek(float timeOffset);

private :

    ////////////////////////////////////////////////////////////
    /// Get the internal format of an audio file according to
    /// its filename extension
    ///
    /// \param filename : Filename to check
    ///
    /// \return Internal format matching the filename (-1 if no match)
    ///
    ////////////////////////////////////////////////////////////
    static int GetFormatFromFilename(const std::string& filename);

    ////////////////////////////////////////////////////////////
    /// Functions for implementing custom read and write to memory files
    ///
    ////////////////////////////////////////////////////////////
    static sf_count_t MemoryGetLength(void* UserData);
    static sf_count_t MemoryRead(void* Ptr, sf_count_t Count, void* UserData);
    static sf_count_t MemorySeek(sf_count_t Offset, int Whence, void* UserData);
    static sf_count_t MemoryTell(void* UserData);
    static sf_count_t MemoryWrite(const void* Ptr, sf_count_t Count, void* UserData);

    ////////////////////////////////////////////////////////////
    /// Structure holding data related to memory operations
    ////////////////////////////////////////////////////////////
    struct MemoryInfos
    {
        const char* DataStart; ///< Pointer to the begining of the data
        const char* DataPtr;   ///< Pointer to the current read / write position
        sf_count_t  TotalSize; ///< Total size of the data, in bytes
    };

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    SNDFILE*     myFile;          ///< File descriptor
    MemoryInfos  myMemory;        ///< Memory read / write data
    std::size_t  myNbSamples;     ///< Total number of samples in the file
    unsigned int myChannelsCount; ///< Number of channels used by the sound
    unsigned int mySampleRate;    ///< Number of samples per second
};

} // namespace priv

} // namespace sf


#endif // SFML_SOUNDFILE_HPP
