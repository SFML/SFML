////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Audio/Export.hpp>

#include <SFML/Audio/SoundSource.hpp>

#include <memory>

#include <cstdlib>

namespace sf
{
class Time;
class SoundBuffer;

////////////////////////////////////////////////////////////
/// \brief Regular sound that can be played in the audio environment
///
////////////////////////////////////////////////////////////
class SFML_AUDIO_API Sound : public SoundSource
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Construct the sound with a buffer
    ///
    /// \param buffer Sound buffer containing the audio data to play with the sound
    ///
    ////////////////////////////////////////////////////////////
    explicit Sound(const SoundBuffer& buffer);

    ////////////////////////////////////////////////////////////
    /// \brief Disallow construction from a temporary sound buffer
    ///
    ////////////////////////////////////////////////////////////
    Sound(const SoundBuffer&& buffer) = delete;

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
    ~Sound() override;

    ////////////////////////////////////////////////////////////
    /// \brief Start or resume playing the sound
    ///
    /// This function starts the stream if it was stopped, resumes
    /// it if it was paused, and restarts it from beginning if it
    /// was it already playing.
    /// This function uses its own thread so that it doesn't block
    /// the rest of the program while the sound is played.
    ///
    /// \see `pause`, `stop`
    ///
    ////////////////////////////////////////////////////////////
    void play() override;

    ////////////////////////////////////////////////////////////
    /// \brief Pause the sound
    ///
    /// This function pauses the sound if it was playing,
    /// otherwise (sound already paused or stopped) it has no effect.
    ///
    /// \see `play`, `stop`
    ///
    ////////////////////////////////////////////////////////////
    void pause() override;

    ////////////////////////////////////////////////////////////
    /// \brief stop playing the sound
    ///
    /// This function stops the sound if it was playing or paused,
    /// and does nothing if it was already stopped.
    /// It also resets the playing position (unlike `pause()`).
    ///
    /// \see `play`, `pause`
    ///
    ////////////////////////////////////////////////////////////
    void stop() override;

    ////////////////////////////////////////////////////////////
    /// \brief Set the source buffer containing the audio data to play
    ///
    /// It is important to note that the sound buffer is not copied,
    /// thus the `sf::SoundBuffer` instance must remain alive as long
    /// as it is attached to the sound.
    ///
    /// \param buffer Sound buffer to attach to the sound
    ///
    /// \see `getBuffer`
    ///
    ////////////////////////////////////////////////////////////
    void setBuffer(const SoundBuffer& buffer);

    ////////////////////////////////////////////////////////////
    /// \brief Disallow setting from a temporary sound buffer
    ///
    ////////////////////////////////////////////////////////////
    void setBuffer(const SoundBuffer&& buffer) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Set whether or not the sound should loop after reaching the end
    ///
    /// If set, the sound will restart from beginning after
    /// reaching the end and so on, until it is stopped or
    /// `setLooping(false)` is called.
    /// The default looping state for sound is `false`.
    ///
    /// \param loop `true` to play in loop, `false` to play once
    ///
    /// \see `isLooping`
    ///
    ////////////////////////////////////////////////////////////
    void setLooping(bool loop);

    ////////////////////////////////////////////////////////////
    /// \brief Change the current playing position of the sound
    ///
    /// The playing position can be changed when the sound is
    /// either paused or playing. Changing the playing position
    /// when the sound is stopped has no effect, since playing
    /// the sound will reset its position.
    ///
    /// \param timeOffset New playing position, from the beginning of the sound
    ///
    /// \see `getPlayingOffset`
    ///
    ////////////////////////////////////////////////////////////
    void setPlayingOffset(Time timeOffset);

    ////////////////////////////////////////////////////////////
    /// \brief Set the effect processor to be applied to the sound
    ///
    /// The effect processor is a callable that will be called
    /// with sound data to be processed.
    ///
    /// \param effectProcessor The effect processor to attach to this sound, attach an empty processor to disable processing
    ///
    ////////////////////////////////////////////////////////////
    void setEffectProcessor(EffectProcessor effectProcessor) override;

    ////////////////////////////////////////////////////////////
    /// \brief Get the audio buffer attached to the sound
    ///
    /// \return Sound buffer attached to the sound
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] const SoundBuffer& getBuffer() const;

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether or not the sound is in loop mode
    ///
    /// \return `true` if the sound is looping, `false` otherwise
    ///
    /// \see `setLooping`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool isLooping() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the current playing position of the sound
    ///
    /// \return Current playing position, from the beginning of the sound
    ///
    /// \see `setPlayingOffset`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Time getPlayingOffset() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the current status of the sound (stopped, paused, playing)
    ///
    /// \return Current status of the sound
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Status getStatus() const override;

    ////////////////////////////////////////////////////////////
    /// \brief Overload of assignment operator
    ///
    /// \param right Instance to assign
    ///
    /// \return Reference to self
    ///
    ////////////////////////////////////////////////////////////
    Sound& operator=(const Sound& right);

private:
    friend class SoundBuffer;

    ////////////////////////////////////////////////////////////
    /// \brief Detach sound from its internal buffer
    ///
    /// This allows the sound buffer to temporarily detach the
    /// sounds that use it when the sound buffer gets updated.
    ///
    ////////////////////////////////////////////////////////////
    void detachBuffer();

    ////////////////////////////////////////////////////////////
    /// \brief Get the sound object
    ///
    /// \return The sound object
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] void* getSound() const override;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    struct Impl;
    const std::unique_ptr<Impl> m_impl; //!< Implementation details
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::Sound
/// \ingroup audio
///
/// `sf::Sound` is the class to use to play sounds.
/// It provides:
/// \li Control (play, pause, stop)
/// \li Ability to modify output parameters in real-time (pitch, volume, ...)
/// \li 3D spatial features (position, attenuation, ...).
///
/// `sf::Sound` is perfect for playing short sounds that can
/// fit in memory and require no latency, like foot steps or
/// gun shots. For longer sounds, like background musics
/// or long speeches, rather see `sf::Music` (which is based
/// on streaming).
///
/// In order to work, a sound must be given a buffer of audio
/// data to play. Audio data (samples) is stored in `sf::SoundBuffer,`
/// and attached to a sound when it is created or with the `setBuffer()` function.
/// The buffer object attached to a sound must remain alive
/// as long as the sound uses it. Note that multiple sounds
/// can use the same sound buffer at the same time.
///
/// Usage example:
/// \code
/// const sf::SoundBuffer buffer("sound.wav");
/// sf::Sound sound(buffer);
/// sound.play();
/// \endcode
///
/// \see `sf::SoundBuffer`, `sf::Music`
///
////////////////////////////////////////////////////////////
