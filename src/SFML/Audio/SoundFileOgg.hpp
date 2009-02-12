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

#ifndef SFML_SOUNDFILEOGG_HPP
#define SFML_SOUNDFILEOGG_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/SoundFile.hpp>

struct stb_vorbis;


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Specialization of SoundFile that handles ogg-vorbis files (.ogg)
/// (does not support variable bitrate / channels and writing)
////////////////////////////////////////////////////////////
class SoundFileOgg : public SoundFile
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    SoundFileOgg();

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~SoundFileOgg();

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
    // Member data
    ////////////////////////////////////////////////////////////
    stb_vorbis*  myStream;        ///< Vorbis stream
    unsigned int myChannelsCount; ///< Number of channels (1 = mono, 2 = stereo)
};

} // namespace priv

} // namespace sf


#endif // SFML_SOUNDFILEOGG_HPP
