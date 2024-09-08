////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
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

#include <SFML/Audio/AudioResource.hpp>

#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector3.hpp>

#include <functional>


namespace sf
{
// NOLINTBEGIN(readability-make-member-function-const)
////////////////////////////////////////////////////////////
/// \brief Base class defining a sound's properties
///
////////////////////////////////////////////////////////////
class SFML_AUDIO_API SoundSource : protected AudioResource
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Enumeration of the sound source states
    ///
    ////////////////////////////////////////////////////////////
    enum class Status
    {
        Stopped, //!< Sound is not playing
        Paused,  //!< Sound is paused
        Playing  //!< Sound is playing
    };

    ////////////////////////////////////////////////////////////
    /// \brief Structure defining the properties of a directional cone
    ///
    /// Sounds will play at gain 1 when the listener
    /// is positioned within the inner angle of the cone.
    /// Sounds will play at `outerGain` when the listener is
    /// positioned outside the outer angle of the cone.
    /// The gain declines linearly from 1 to `outerGain` as the
    /// listener moves from the inner angle to the outer angle.
    ///
    ////////////////////////////////////////////////////////////
    struct Cone
    {
        Angle innerAngle;  //!< Inner angle
        Angle outerAngle;  //!< Outer angle
        float outerGain{}; //!< Outer gain
    };

    ////////////////////////////////////////////////////////////
    /// \brief Callable that is provided with sound data for processing
    ///
    /// When the audio engine sources sound data from sound
    /// sources it will pass the data through an effects
    /// processor if one is set. The sound data will already be
    /// converted to the internal floating point format.
    ///
    /// Sound data that is processed this way is provided in
    /// frames. Each frame contains 1 floating point sample per
    /// channel. If e.g. the data source provides stereo data,
    /// each frame will contain 2 floats.
    ///
    /// The effects processor function takes 4 parameters:
    ///   - The input data frames, channels interleaved
    ///   - The number of input data frames available
    ///   - The buffer to write output data frames to, channels interleaved
    ///   - The number of output data frames that the output buffer can hold
    ///   - The channel count
    ///
    /// The input and output frame counts are in/out parameters.
    ///
    /// When this function is called, the input count will
    /// contain the number of frames available in the input
    /// buffer. The output count will contain the size of the
    /// output buffer i.e. the maximum number of frames that
    /// can be written to the output buffer.
    ///
    /// Attempting to read more frames than the input frame
    /// count or write more frames than the output frame count
    /// will result in undefined behaviour.
    ///
    /// It is important to note that the channel count of the
    /// audio engine currently sourcing data from this sound
    /// will always be provided in `frameChannelCount`. This can
    /// be different from the channel count of the sound source
    /// so make sure to size necessary processing buffers
    /// according to the engine channel count and not the sound
    /// source channel count.
    ///
    /// When done processing the frames, the input and output
    /// frame counts must be updated to reflect the actual
    /// number of frames that were read from the input and
    /// written to the output.
    ///
    /// The processing function should always try to process as
    /// much sound data as possible i.e. always try to fill the
    /// output buffer to the maximum. In certain situations for
    /// specific effects it can be possible that the input frame
    /// count and output frame count aren't equal. As long as
    /// the frame counts are updated accordingly this is
    /// perfectly valid.
    ///
    /// If the audio engine determines that no audio data is
    /// available from the data source, the input data frames
    /// pointer is set to `nullptr` and the input frame count is
    /// set to 0. In this case it is up to the function to
    /// decide how to handle the situation. For specific effects
    /// e.g. Echo/Delay buffered data might still be able to be
    /// written to the output buffer even if there is no longer
    /// any input data.
    ///
    /// An important thing to remember is that this function is
    /// directly called by the audio engine. Because the audio
    /// engine runs on an internal thread of its own, make sure
    /// access to shared data is synchronized appropriately.
    ///
    /// Because this function is stored by the `SoundSource`
    /// object it will be able to be called as long as the
    /// `SoundSource` object hasn't yet been destroyed. Make sure
    /// that any data this function references outlives the
    /// SoundSource object otherwise use-after-free errors will
    /// occur.
    ///
    ////////////////////////////////////////////////////////////
    using EffectProcessor = std::function<
        void(const float* inputFrames, unsigned int& inputFrameCount, float* outputFrames, unsigned int& outputFrameCount, unsigned int frameChannelCount)>;

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    ////////////////////////////////////////////////////////////
    SoundSource(const SoundSource&) = default;

    ////////////////////////////////////////////////////////////
    /// \brief Move constructor
    ///
    ////////////////////////////////////////////////////////////
    SoundSource(SoundSource&&) noexcept = default;

    ////////////////////////////////////////////////////////////
    /// \brief Move assignment
    ///
    ////////////////////////////////////////////////////////////
    SoundSource& operator=(SoundSource&&) noexcept = default;

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~SoundSource() = default;

    ////////////////////////////////////////////////////////////
    /// \brief Set the pitch of the sound
    ///
    /// The pitch represents the perceived fundamental frequency
    /// of a sound; thus you can make a sound more acute or grave
    /// by changing its pitch. A side effect of changing the pitch
    /// is to modify the playing speed of the sound as well.
    /// The default value for the pitch is 1.
    ///
    /// \param pitch New pitch to apply to the sound
    ///
    /// \see `getPitch`
    ///
    ////////////////////////////////////////////////////////////
    void setPitch(float pitch);

    ////////////////////////////////////////////////////////////
    /// \brief Set the pan of the sound
    ///
    /// Using panning, a mono sound can be panned between
    /// stereo channels. When the pan is set to -1, the sound
    /// is played only on the left channel, when the pan is set
    /// to +1, the sound is played only on the right channel.
    ///
    /// \param pan New pan to apply to the sound [-1, +1]
    ///
    /// \see `getPan`
    ///
    ////////////////////////////////////////////////////////////
    void setPan(float pan);

    ////////////////////////////////////////////////////////////
    /// \brief Set the volume of the sound
    ///
    /// The volume is a value between 0 (mute) and 100 (full volume).
    /// The default value for the volume is 100.
    ///
    /// \param volume Volume of the sound
    ///
    /// \see `getVolume`
    ///
    ////////////////////////////////////////////////////////////
    void setVolume(float volume);

    ////////////////////////////////////////////////////////////
    /// \brief Set whether spatialization of the sound is enabled
    ///
    /// Spatialization is the application of various effects to
    /// simulate a sound being emitted at a virtual position in
    /// 3D space and exhibiting various physical phenomena such as
    /// directional attenuation and doppler shift.
    ///
    /// \param enabled `true` to enable spatialization, `false` to disable
    ///
    /// \see `isSpatializationEnabled`
    ///
    ////////////////////////////////////////////////////////////
    void setSpatializationEnabled(bool enabled);

    ////////////////////////////////////////////////////////////
    /// \brief Set the 3D position of the sound in the audio scene
    ///
    /// Only sounds with one channel (mono sounds) can be
    /// spatialized.
    /// The default position of a sound is (0, 0, 0).
    ///
    /// \param position Position of the sound in the scene
    ///
    /// \see `getPosition`
    ///
    ////////////////////////////////////////////////////////////
    void setPosition(const Vector3f& position);

    ////////////////////////////////////////////////////////////
    /// \brief Set the 3D direction of the sound in the audio scene
    ///
    /// The direction defines where the sound source is facing
    /// in 3D space. It will affect how the sound is attenuated
    /// if facing away from the listener.
    /// The default direction of a sound is (0, 0, -1).
    ///
    /// \param direction Direction of the sound in the scene
    ///
    /// \see `getDirection`
    ///
    ////////////////////////////////////////////////////////////
    void setDirection(const Vector3f& direction);

    ////////////////////////////////////////////////////////////
    /// \brief Set the cone properties of the sound in the audio scene
    ///
    /// The cone defines how directional attenuation is applied.
    /// The default cone of a sound is (2 * PI, 2 * PI, 1).
    ///
    /// \param cone Cone properties of the sound in the scene
    ///
    /// \see `getCone`
    ///
    ////////////////////////////////////////////////////////////
    void setCone(const Cone& cone);

    ////////////////////////////////////////////////////////////
    /// \brief Set the 3D velocity of the sound in the audio scene
    ///
    /// The velocity is used to determine how to doppler shift
    /// the sound. Sounds moving towards the listener will be
    /// perceived to have a higher pitch and sounds moving away
    /// from the listener will be perceived to have a lower pitch.
    ///
    /// \param velocity Velocity of the sound in the scene
    ///
    /// \see `getVelocity`
    ///
    ////////////////////////////////////////////////////////////
    void setVelocity(const Vector3f& velocity);

    ////////////////////////////////////////////////////////////
    /// \brief Set the doppler factor of the sound
    ///
    /// The doppler factor determines how strong the doppler
    /// shift will be.
    ///
    /// \param factor New doppler factor to apply to the sound
    ///
    /// \see `getDopplerFactor`
    ///
    ////////////////////////////////////////////////////////////
    void setDopplerFactor(float factor);

    ////////////////////////////////////////////////////////////
    /// \brief Set the directional attenuation factor of the sound
    ///
    /// Depending on the virtual position of an output channel
    /// relative to the listener (such as in surround sound
    /// setups), sounds will be attenuated when emitting them
    /// from certain channels. This factor determines how strong
    /// the attenuation based on output channel position
    /// relative to the listener is.
    ///
    /// \param factor New directional attenuation factor to apply to the sound
    ///
    /// \see `getDirectionalAttenuationFactor`
    ///
    ////////////////////////////////////////////////////////////
    void setDirectionalAttenuationFactor(float factor);

    ////////////////////////////////////////////////////////////
    /// \brief Make the sound's position relative to the listener or absolute
    ///
    /// Making a sound relative to the listener will ensure that it will always
    /// be played the same way regardless of the position of the listener.
    /// This can be useful for non-spatialized sounds, sounds that are
    /// produced by the listener, or sounds attached to it.
    /// The default value is `false` (position is absolute).
    ///
    /// \param relative `true` to set the position relative, `false` to set it absolute
    ///
    /// \see `isRelativeToListener`
    ///
    ////////////////////////////////////////////////////////////
    void setRelativeToListener(bool relative);

    ////////////////////////////////////////////////////////////
    /// \brief Set the minimum distance of the sound
    ///
    /// The "minimum distance" of a sound is the maximum
    /// distance at which it is heard at its maximum volume. Further
    /// than the minimum distance, it will start to fade out according
    /// to its attenuation factor. A value of 0 ("inside the head
    /// of the listener") is an invalid value and is forbidden.
    /// The default value of the minimum distance is 1.
    ///
    /// \param distance New minimum distance of the sound
    ///
    /// \see `getMinDistance`, `setAttenuation`
    ///
    ////////////////////////////////////////////////////////////
    void setMinDistance(float distance);

    ////////////////////////////////////////////////////////////
    /// \brief Set the maximum distance of the sound
    ///
    /// The "maximum distance" of a sound is the minimum
    /// distance at which it is heard at its minimum volume. Closer
    /// than the maximum distance, it will start to fade in according
    /// to its attenuation factor.
    /// The default value of the maximum distance is the maximum
    /// value a float can represent.
    ///
    /// \param distance New maximum distance of the sound
    ///
    /// \see `getMaxDistance`, `setAttenuation`
    ///
    ////////////////////////////////////////////////////////////
    void setMaxDistance(float distance);

    ////////////////////////////////////////////////////////////
    /// \brief Set the minimum gain of the sound
    ///
    /// When the sound is further away from the listener than
    /// the "maximum distance" the attenuated gain is clamped
    /// so it cannot go below the minimum gain value.
    ///
    /// \param gain New minimum gain of the sound
    ///
    /// \see `getMinGain`, `setAttenuation`
    ///
    ////////////////////////////////////////////////////////////
    void setMinGain(float gain);

    ////////////////////////////////////////////////////////////
    /// \brief Set the maximum gain of the sound
    ///
    /// When the sound is closer from the listener than
    /// the "minimum distance" the attenuated gain is clamped
    /// so it cannot go above the maximum gain value.
    ///
    /// \param gain New maximum gain of the sound
    ///
    /// \see `getMaxGain`, `setAttenuation`
    ///
    ////////////////////////////////////////////////////////////
    void setMaxGain(float gain);

    ////////////////////////////////////////////////////////////
    /// \brief Set the attenuation factor of the sound
    ///
    /// The attenuation is a multiplicative factor which makes
    /// the sound more or less loud according to its distance
    /// from the listener. An attenuation of 0 will produce a
    /// non-attenuated sound, i.e. its volume will always be the same
    /// whether it is heard from near or from far. On the other hand,
    /// an attenuation value such as 100 will make the sound fade out
    /// very quickly as it gets further from the listener.
    /// The default value of the attenuation is 1.
    ///
    /// \param attenuation New attenuation factor of the sound
    ///
    /// \see `getAttenuation`, `setMinDistance`
    ///
    ////////////////////////////////////////////////////////////
    void setAttenuation(float attenuation);

    ////////////////////////////////////////////////////////////
    /// \brief Set the effect processor to be applied to the sound
    ///
    /// The effect processor is a callable that will be called
    /// with sound data to be processed.
    ///
    /// \param effectProcessor The effect processor to attach to this sound, attach an empty processor to disable processing
    ///
    ////////////////////////////////////////////////////////////
    virtual void setEffectProcessor(EffectProcessor effectProcessor);

    ////////////////////////////////////////////////////////////
    /// \brief Get the pitch of the sound
    ///
    /// \return Pitch of the sound
    ///
    /// \see `setPitch`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] float getPitch() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the pan of the sound
    ///
    /// \return Pan of the sound
    ///
    /// \see `setPan`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] float getPan() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the volume of the sound
    ///
    /// \return Volume of the sound, in the range [0, 100]
    ///
    /// \see `setVolume`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] float getVolume() const;

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether spatialization of the sound is enabled
    ///
    /// \return `true` if spatialization is enabled, `false` if it's disabled
    ///
    /// \see `setSpatializationEnabled`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool isSpatializationEnabled() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the 3D position of the sound in the audio scene
    ///
    /// \return Position of the sound
    ///
    /// \see `setPosition`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Vector3f getPosition() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the 3D direction of the sound in the audio scene
    ///
    /// \return Direction of the sound
    ///
    /// \see `setDirection`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Vector3f getDirection() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the cone properties of the sound in the audio scene
    ///
    /// \return Cone properties of the sound
    ///
    /// \see `setCone`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Cone getCone() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the 3D velocity of the sound in the audio scene
    ///
    /// \return Velocity of the sound
    ///
    /// \see `setVelocity`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Vector3f getVelocity() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the doppler factor of the sound
    ///
    /// \return Doppler factor of the sound
    ///
    /// \see `setDopplerFactor`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] float getDopplerFactor() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the directional attenuation factor of the sound
    ///
    /// \return Directional attenuation factor of the sound
    ///
    /// \see `setDirectionalAttenuationFactor`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] float getDirectionalAttenuationFactor() const;

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether the sound's position is relative to the
    ///        listener or is absolute
    ///
    /// \return `true` if the position is relative, `false` if it's absolute
    ///
    /// \see `setRelativeToListener`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool isRelativeToListener() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the minimum distance of the sound
    ///
    /// \return Minimum distance of the sound
    ///
    /// \see `setMinDistance`, `getAttenuation`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] float getMinDistance() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the maximum distance of the sound
    ///
    /// \return Maximum distance of the sound
    ///
    /// \see `setMaxDistance`, `getAttenuation`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] float getMaxDistance() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the minimum gain of the sound
    ///
    /// \return Minimum gain of the sound
    ///
    /// \see `setMinGain`, `getAttenuation`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] float getMinGain() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the maximum gain of the sound
    ///
    /// \return Maximum gain of the sound
    ///
    /// \see `setMaxGain`, `getAttenuation`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] float getMaxGain() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the attenuation factor of the sound
    ///
    /// \return Attenuation factor of the sound
    ///
    /// \see `setAttenuation`, `getMinDistance`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] float getAttenuation() const;

    ////////////////////////////////////////////////////////////
    /// \brief Overload of assignment operator
    ///
    /// \param right Instance to assign
    ///
    /// \return Reference to self
    ///
    ////////////////////////////////////////////////////////////
    SoundSource& operator=(const SoundSource& right);

    ////////////////////////////////////////////////////////////
    /// \brief Start or resume playing the sound source
    ///
    /// This function starts the source if it was stopped, resumes
    /// it if it was paused, and restarts it from the beginning if
    /// it was already playing.
    ///
    /// \see `pause`, `stop`
    ///
    ////////////////////////////////////////////////////////////
    virtual void play() = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Pause the sound source
    ///
    /// This function pauses the source if it was playing,
    /// otherwise (source already paused or stopped) it has no effect.
    ///
    /// \see `play`, `stop`
    ///
    ////////////////////////////////////////////////////////////
    virtual void pause() = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Stop playing the sound source
    ///
    /// This function stops the source if it was playing or paused,
    /// and does nothing if it was already stopped.
    /// It also resets the playing position (unlike `pause()`).
    ///
    /// \see `play`, `pause`
    ///
    ////////////////////////////////////////////////////////////
    virtual void stop() = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Get the current status of the sound (stopped, paused, playing)
    ///
    /// \return Current status of the sound
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] virtual Status getStatus() const = 0;

protected:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructor is meant to be called by derived classes only.
    ///
    ////////////////////////////////////////////////////////////
    SoundSource() = default;

private:
    ////////////////////////////////////////////////////////////
    /// \brief Get the sound object
    ///
    /// \return The sound object
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] virtual void* getSound() const = 0;
};

// NOLINTEND(readability-make-member-function-const)
} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::SoundSource
/// \ingroup audio
///
/// `sf::SoundSource` is not meant to be used directly, it
/// only serves as a common base for all audio objects
/// that can live in the audio environment.
///
/// It defines several properties for the sound: pitch,
/// volume, position, attenuation, etc. All of them can be
/// changed at any time with no impact on performances.
///
/// \see `sf::Sound`, `sf::SoundStream`
///
////////////////////////////////////////////////////////////
