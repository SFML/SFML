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

#ifndef SFML_SOUNDFILEDEFAULT_HPP
#define SFML_SOUNDFILEDEFAULT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/SoundFile.hpp>
#include <sndfile.h>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Specialization of SoundFile that can handle a lot of
/// sound formats (see libsndfile homepage for a complete list)
////////////////////////////////////////////////////////////
class SoundFileDefault : public SoundFile
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    SoundFileDefault();

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~SoundFileDefault();

    ////////////////////////////////////////////////////////////
    /// Check if a given file is supported by this loader
    ///
    /// \param Filename : Path of the file to check
    /// \param Read :     Is the file opened for reading or writing ?
    ///
    /// \param return True if the loader can handle this file
    ///
    ////////////////////////////////////////////////////////////
    static bool IsFileSupported(const std::string& Filename, bool Read);

    ////////////////////////////////////////////////////////////
    /// Check if a given file in memory is supported by this loader
    ///
    /// \param Data :        Pointer to the file data in memory
    /// \param SizeInBytes : Size of the data to load, in bytes
    ///
    /// \param return True if the loader can handle this file
    ///
    ////////////////////////////////////////////////////////////
    static bool IsFileSupported(const char* Data, std::size_t SizeInBytes);

    ////////////////////////////////////////////////////////////
    /// /see sf::SoundFile::Read
    ///
    ////////////////////////////////////////////////////////////
    virtual std::size_t Read(Int16* Data, std::size_t NbSamples);

    ////////////////////////////////////////////////////////////
    /// /see sf::SoundFile::Write
    ///
    ////////////////////////////////////////////////////////////
    virtual void Write(const Int16* Data, std::size_t NbSamples);

private :

    ////////////////////////////////////////////////////////////
    /// /see sf::SoundFile::OpenRead
    ///
    ////////////////////////////////////////////////////////////
    virtual bool OpenRead(const std::string& Filename, std::size_t& NbSamples, unsigned int& ChannelsCount, unsigned int& SampleRate);

    ////////////////////////////////////////////////////////////
    /// /see sf::SoundFile::OpenRead
    ///
    ////////////////////////////////////////////////////////////
    virtual bool OpenRead(const char* Data, std::size_t SizeInBytes, std::size_t& NbSamples, unsigned int& ChannelsCount, unsigned int& SampleRate);

    ////////////////////////////////////////////////////////////
    /// /see sf::SoundFile::OpenWrite
    ///
    ////////////////////////////////////////////////////////////
    virtual bool OpenWrite(const std::string& Filename, unsigned int ChannelsCount, unsigned int SampleRate);

    ////////////////////////////////////////////////////////////
    /// Get the internal format of an audio file according to
    /// its filename extension
    ///
    /// \param Filename : Filename to check
    ///
    /// \return Internal format matching the filename (-1 if no match)
    ///
    ////////////////////////////////////////////////////////////
    static int GetFormatFromFilename(const std::string& Filename);

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
    SNDFILE*    myFile;   ///< File descriptor
    MemoryInfos myMemory; ///< Memory read / write data
};

} // namespace priv

} // namespace sf


#endif // SFML_SOUNDFILEDEFAULT_HPP
