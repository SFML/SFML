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

#ifndef SFML_SOUND_HPP
#define SFML_SOUND_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/SoundSource.hpp>
#include <SFML/System/Resource.hpp>
#include <cstdlib>


namespace sf
{
class SoundBuffer;

////////////////////////////////////////////////////////////
/// \brief Regular sound that can be played in the audio environment
///
////////////////////////////////////////////////////////////
class SFML_API Sound : public SoundSource
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Sound();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the sound with parameters
    ///
    /// \param buffer   Sound buffer containing the audio data to play with the sound
    /// \param loop     Should the sound loop?
    /// \param pitch    Pitch of the sound
    /// \param volume   Volume of the sound, in the range [0, 100]
    /// \param position 3D position of the sound source in the audio scene
    ///
    ////////////////////////////////////////////////////////////
    Sound(const SoundBuffer& buffer, bool loop = false, float pitch = 1.f, float volume = 100.f, const Vector3f& position = Vector3f(0, 0, 0));

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    /// \param copy Instance to copy
    ///
    ////////////////////////////////////////////////////////////
    Sound(const Sound& copy);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~Sound();

    ////////////////////////////////////////////////////////////
    /// \brief Start or resume playing the sound
    ///
    /// This function starts the stream if it was stopped, resumes
    /// it if it was paused, and restarts it from beginning if it
    /// was it already playing.
    /// This function uses its own thread so that it doesn't block
    /// the rest of the program while the sound is played.
    ///
    /// \see Pause, Stop
    ///
    ////////////////////////////////////////////////////////////
    void Play();

    ////////////////////////////////////////////////////////////
    /// \brief Pause the sound
    ///
    /// This function pauses the sound if it was playing,
    /// otherwise (sound already paused or stopped) it has no effect.
    ///
    /// \see Play, Stop
    ///
    ////////////////////////////////////////////////////////////
    void Pause();

    ////////////////////////////////////////////////////////////
    /// \brief Stop playing the sound
    ///
    /// This function stops the sound if it was playing or paused,
    /// and does nothing if it was already stopped.
    /// It also resets the playing position (unlike Pause()).
    ///
    /// \see Play, Pause
    ///
    ////////////////////////////////////////////////////////////
    void Stop();

    ////////////////////////////////////////////////////////////
    /// \brief Set the source buffer containing the audio data to play
    ///
    /// It is important to note that the sound buffer is not copied,
    /// thus the sf::SoundBuffer instance must remain alive as long
    /// as it is attached to the sound.
    ///
    /// \param buffer Sound buffer to attach to the sound
    ///
    /// \see GetBuffer
    ///
    ////////////////////////////////////////////////////////////
    void SetBuffer(const SoundBuffer& buffer);

    ////////////////////////////////////////////////////////////
    /// \brief Set whether or not the sound should loop after reaching the end
    ///
    /// If set, the sound will restart from beginning after
    /// reaching the end and so on, until it is stopped or
    /// SetLoop(false) is called.
    /// The default looping state for sound is false.
    ///
    /// \param loop True to play in loop, false to play once
    ///
    /// \see GetLoop
    ///
    ////////////////////////////////////////////////////////////
    void SetLoop(bool loop);

    ////////////////////////////////////////////////////////////
    /// \brief Change the current playing position of the sound
    ///
    /// The playing position can be changed when the sound is
    /// either paused or playing.
    ///
    /// \param timeOffset New playing position, in seconds
    ///
    /// \see GetPlayingOffset
    ///
    ////////////////////////////////////////////////////////////
    void SetPlayingOffset(float timeOffset);

    ////////////////////////////////////////////////////////////
    /// \brief Get the audio buffer attached to the sound
    ///
    /// \return Sound buffer attached to the sound (can be NULL)
    ///
    ////////////////////////////////////////////////////////////
    const SoundBuffer* GetBuffer() const;

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether or not the sound is in loop mode
    ///
    /// \return True if the sound is looping, false otherwise
    ///
    /// \see SetLoop
    ///
    ////////////////////////////////////////////////////////////
    bool GetLoop() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the current playing position of the sound
    ///
    /// \return Current playing position, in seconds
    ///
    /// \see SetPlayingOffset
    ///
    ////////////////////////////////////////////////////////////
    float GetPlayingOffset() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the current status of the sound (stopped, paused, playing)
    ///
    /// \return Current status of the sound
    ///
    ////////////////////////////////////////////////////////////
    Status GetStatus() const;

    ////////////////////////////////////////////////////////////
    /// \brief Overload of assignment operator
    ///
    /// \param right Instance to assign
    ///
    /// \return Reference to self
    ///
    ////////////////////////////////////////////////////////////
    Sound& operator =(const Sound& right);

    ////////////////////////////////////////////////////////////
    /// \brief Reset the internal buffer of the sound
    ///
    /// This function is for internal use only, you don't have
    /// to use it. It is called by the sf::SoundBuffer that
    /// this sound uses, when it is destroyed in order to prevent
    /// the sound from using a dead buffer.
    ///
    ////////////////////////////////////////////////////////////
    void ResetBuffer();

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    ResourcePtr<SoundBuffer> myBuffer; ///< Sound buffer bound to the source
};

} // namespace sf


#endif // SFML_SOUND_HPP


////////////////////////////////////////////////////////////
/// \class sf::Sound
/// \ingroup audio
///
/// sf::Sound is the class to use to play sounds.
/// It provides:
/// \li Control (play, pause, stop)
/// \li Ability to modify output parameters in real-time (pitch, volume, ...)
/// \li 3D spatial features (position, attenuation, ...).
///
/// sf::Sound is perfect for playing short sounds that can
/// fit in memory and require no latency, like foot steps or
/// gun shots. For longer sounds, like background musics
/// or long speeches, rather see sf::Music (which is based
/// on streaming).
///
/// In order to work, a sound must be given a buffer of audio
/// data to play. Audio data (samples) is stored in sf::SoundBuffer,
/// and attached to a sound with the SetBuffer() function.
/// The buffer object attached to a sound must remain alive
/// as long as the sound uses it. Note that multiple sounds
/// can use the same sound buffer at the same time.
///
/// Usage example:
/// \code
/// sf::SoundBuffer buffer;
/// buffer.LoadFromFile("sound.wav");
///
/// sf::Sound sound;
/// sound.SetBuffer(buffer);
/// sound.Play();
/// \endcode
///
/// \see sf::SoundBuffer, sf::Music
///
////////////////////////////////////////////////////////////
