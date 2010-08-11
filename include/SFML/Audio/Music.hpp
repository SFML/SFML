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
/// \brief Streamed music played from an audio file
///
////////////////////////////////////////////////////////////
class SFML_API Music : public SoundStream
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Music();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~Music();

    ////////////////////////////////////////////////////////////
    /// \brief Open a music from an audio file
    ///
    /// This function doesn't start playing the music (call Play()
    /// to do so).
    /// Here is a complete list of all the supported audio formats:
    /// ogg, wav, flac, aiff, au, raw, paf, svx, nist, voc, ircam,
    /// w64, mat4, mat5 pvf, htk, sds, avr, sd2, caf, wve, mpc2k, rf64.
    ///
    /// \param filename Path of the music file to open
    ///
    /// \return True if loading succeeded, false if it failed
    ///
    /// \see OpenFromMemory
    ///
    ////////////////////////////////////////////////////////////
    bool OpenFromFile(const std::string& filename);

    ////////////////////////////////////////////////////////////
    /// \brief Open a music from an audio file in memory
    ///
    /// This function doesn't start playing the music (call Play()
    /// to do so).
    /// Here is a complete list of all the supported audio formats:
    /// ogg, wav, flac, aiff, au, raw, paf, svx, nist, voc, ircam,
    /// w64, mat4, mat5 pvf, htk, sds, avr, sd2, caf, wve, mpc2k, rf64.
    ///
    /// \param data        Pointer to the file data in memory
    /// \param sizeInBytes Size of the data to load, in bytes
    ///
    /// \return True if loading succeeded, false if it failed
    ///
    /// \see OpenFromFile
    ///
    ////////////////////////////////////////////////////////////
    bool OpenFromMemory(const void* data, std::size_t sizeInBytes);

    ////////////////////////////////////////////////////////////
    /// \brief Get the total duration of the music
    ///
    /// \return Music duration, in seconds
    ///
    ////////////////////////////////////////////////////////////
    float GetDuration() const;

protected :

    ////////////////////////////////////////////////////////////
    /// \brief Request a new chunk of audio samples from the stream source
    ///
    /// This function fills the chunk from the next samples
    /// to read from the audio file.
    ///
    /// \param data Chunk of data to fill
    ///
    /// \return True to continue playback, false to stop
    ///
    ////////////////////////////////////////////////////////////
    virtual bool OnGetData(Chunk& data);

    ////////////////////////////////////////////////////////////
    /// \brief Change the current playing position in the stream source
    ///
    /// \param timeOffset New playing position, in seconds
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnSeek(float timeOffset);

private :

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


////////////////////////////////////////////////////////////
/// \class sf::Music
/// \ingroup audio
///
/// Musics are sounds that are streamed rather than completely
/// loaded in memory. This is especially useful for compressed
/// musics that usually take hundreds of MB when they are
/// uncompressed: by streaming it instead of loading it entirely,
/// you avoid saturating the memory and have almost no loading delay.
///
/// Apart from that, a sf::Music has almost the same features as
/// the sf::SoundBuffer / sf::Sound pair: you can play/pause/stop
/// it, request its parameters (channels, sample rate), change
/// the way it is played (pitch, volume, 3D position, ...), etc.
///
/// As a sound stream, a music is played in its own thread in order
/// not to block the rest of the program. This means that you can
/// leave the music alone after calling Play(), it will manage itself
/// very well.
///
/// Usage example:
/// \code
/// // Declare a new music
/// sf::Music music;
///
/// // Open it from an audio file
/// if (!music.OpenFromFile("music.ogg"))
/// {
///     // error...
/// }
///
/// // Change some parameters
/// music.SetPosition(0, 1, 10); // change its 3D position
/// music.SetPitch(2);           // increase the pitch
/// music.SetVolume(50);         // reduce the volume
/// music.SetLoop(true);         // make it loop
///
/// // Play it
/// music.Play();
/// \endcode
///
/// \see sf::Sound, sf::SoundStream
///
////////////////////////////////////////////////////////////
