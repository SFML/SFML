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

#ifndef SFML_MUSIC_HPP
#define SFML_MUSIC_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/SoundStream.hpp>
#include <SFML/System/Mutex.hpp>
#include <string>
#include <vector>


namespace sf
{
namespace priv
{
    class SoundFile;
}

////////////////////////////////////////////////////////////
/// Music defines a big sound played using streaming,
/// so usually what we call a music :)
////////////////////////////////////////////////////////////
class SFML_API Music : public SoundStream
{
public :

    ////////////////////////////////////////////////////////////
    /// Construct the music with a buffer size
    ///
    /// \param bufferSize : Size of the internal buffer, expressed in number of samples
    ///                     (ie. size taken by the music in memory) (44100 by default)
    ///
    ////////////////////////////////////////////////////////////
    Music(std::size_t bufferSize = 44100);

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~Music();

    ////////////////////////////////////////////////////////////
    /// Open a music file (doesn't play it -- call Play() for that)
    ///
    /// \param filename : Path of the music file to open
    ///
    /// \return True if loading has been successful
    ///
    ////////////////////////////////////////////////////////////
    bool OpenFromFile(const std::string& filename);

    ////////////////////////////////////////////////////////////
    /// Open a music file from memory (doesn't play it -- call Play() for that)
    ///
    /// \param data :        Pointer to the file data in memory
    /// \param sizeInBytes : Size of the data to load, in bytes
    ///
    /// \return True if loading has been successful
    ///
    ////////////////////////////////////////////////////////////
    bool OpenFromMemory(const char* data, std::size_t sizeInBytes);

    ////////////////////////////////////////////////////////////
    /// Get the music duration
    ///
    /// \return Music duration, in seconds
    ///
    ////////////////////////////////////////////////////////////
    float GetDuration() const;

private :

    ////////////////////////////////////////////////////////////
    /// /see SoundStream::OnGetData
    ///
    ////////////////////////////////////////////////////////////
    virtual bool OnGetData(Chunk& data);

    ////////////////////////////////////////////////////////////
    /// /see SoundStream::OnSeek
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnSeek(float timeOffset);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    priv::SoundFile*   myFile;     ///< Sound file
    float              myDuration; ///< Music duration, in seconds
    std::vector<Int16> mySamples;  ///< Temporary buffer of samples
    Mutex              myMutex;    ///< Mutex protecting the data
};

} // namespace sf


#endif // SFML_MUSIC_HPP
