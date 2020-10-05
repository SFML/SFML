////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2020 Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef SFML_REVERB_EFFECT_HPP
#define SFML_REVERB_EFFECT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/SoundEffect.hpp>

namespace sf
{

////////////////////////////////////////////////////////////
/// \brief Implements a reverb sound effect
///
////////////////////////////////////////////////////////////
class SFML_AUDIO_API ReverbEffect : public SoundEffect
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    ReverbEffect();

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    /// \param copy Instance to copy
    ///
    ////////////////////////////////////////////////////////////
    ReverbEffect(const ReverbEffect& copy);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current density value
    ///
    /// Reverb Modal Density controls the coloration of the late reverb.
    /// Lowering the value adds more coloration to the late reverb.
    /// 
    /// \param density new value to set between 0.f and 1.f. Defaults to 1.f
    ////////////////////////////////////////////////////////////
    void setDensity(float density);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current diffusion value
    ///
    /// The Reverb Diffusion property controls the echo density in
    /// the reverberation decay. It's set by default to 1.0, which
    /// provides the highest density.Reducing diffusion gives the
    /// reverberation a more 'grainy' character that is especially
    /// noticeable with percussive sound sources. If you set a
    /// diffusion value of 0.0, the later reverberation sounds like
    /// a succession of distinct echoes.
    /// 
    /// \param diffusion new value to set between 0.f and 1.f. Defaults to 1.f
    ////////////////////////////////////////////////////////////
    void setDiffusion(float diffusion);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current gain value
    ///
    /// The Reverb Gain property is the master volume control for
    /// the reflected sound (both early reflections and reverberation)
    /// that the reverb effect adds to all sound sources. It sets the
    /// maximum amount of reflections and reverberation added to the
    /// final sound mix. The value of the Reverb Gain property ranges
    /// from 1.0 (0db) (the maximum amount) to 0.0 (-100db)
    /// (no reflected sound at all).
    /// 
    /// \param gain new value to set between 0.f and 1.f. Defaults to 0.32f
    ////////////////////////////////////////////////////////////
    void setGain(float gain);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current decay value
    ///
    /// The Decay Time property sets the reverberation decay time.
    /// It ranges from 0.1 (typically a small room with very dead
    /// surfaces) to 20.0 (typically a large room with very live surfaces).
    /// 
    /// \param decay new value to set between 0.1f and 20.f seconds. Defaults to 1.49f
    ////////////////////////////////////////////////////////////
    void setDecayTime(float decay);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current reflection gain value
    ///
    /// The Reflections Gain property controls the overall amount
    /// of initial reflections relative to the Gain property.
    /// (The Gain property sets the overall amount of reflected
    /// sound : both initial reflections and later reverberation.)
    /// The value of Reflections Gain ranges from a maximum of
    /// 3.16 (+10 dB) to a minimum of 0.0 (-100 dB) (no initial
    /// reflections at all), and is corrected by the value of the
    /// Gain property.The Reflections Gain property does not affect
    /// the subsequent reverberation decay.
    ///
    /// You can increase the amount of initial reflections to simulate
    /// a more narrow space or closer walls, especially effective if
    /// you associate the initial reflections increase with a reduction
    /// in reflections delays by lowering the value of the Reflection
    /// Delay property.To simulate open or semi - open environments,
    /// you can maintain the amount of early reflections while reducing
    /// the value of the Late Reverb Gain property, which controls later
    /// reflections.
    /// 
    /// \param gain new value to set between 0.f and 1.f. Defaults to 0.05f
    ////////////////////////////////////////////////////////////
    void setReflectionGain(float gain);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current reflection delay value
    ///
    /// The Reflections Delay property is the amount of delay
    /// between the arrival time of the direct path from the source
    /// to the first reflection from the source.It ranges from 0 to
    /// 300 milliseconds.You can reduce or increase Reflections Delay
    /// to simulate closer or more distant reflective surfaces — and
    /// therefore control the perceived size of the room.
    /// 
    /// \param delay new value to set between 0.f and 0.3f. Defaults to 0.007f
    ////////////////////////////////////////////////////////////
    void setReflectionDelay(float delay);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current late reverb delay value
    ///
    /// The Late Reverb Gain property controls the overall amount
    /// of later reverberation relative to the Gain property.
    /// (The Gain property sets the overall amount of both initial
    /// reflectionsand later reverberation.) The value of Late Reverb
    /// Gain ranges from a maximum of 10.0 (+20 dB) to a minimum of
    /// 0.0 (-100 dB) (no late reverberation at all). Note that Late
    /// Reverb Gain and Decay Time are independent properties :
    /// If you adjust Decay Time without changing Late Reverb Gain,
    /// the total intensity(the averaged square of the amplitude)
    /// of the late reverberation remains constant.
    /// 
    /// \param delay new value to set between 0.f and 10.f. Defaults to 1.26f
    ////////////////////////////////////////////////////////////
    void setLateReverbGain(float delay);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current late reverb delay value
    ///
    /// The Late Reverb Delay property defines the begin time of the
    /// late reverberation relative to the time of the initial
    /// reflection(the first of the early reflections).It ranges from
    /// 0 to 100 milliseconds. Reducing or increasing Late Reverb Delay
    /// is useful for simulating a smaller or larger room.
    /// 
    /// \param delay new value to set between 0.f and 0.1f. Defaults to 0.011f
    ////////////////////////////////////////////////////////////
    void setLateReverbDelay(float delay);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current room rolloff value
    ///
    /// The Room Rolloff Factor property is one of two methods available
    /// to attenuate the reflected sound (containing both reflections and
    /// reverberation) according to source - listener distance. It's defined
    /// the same way as sf::SoundSource's Attenuation Factor, but operates
    /// on reverb sound instead of direct - path sound. Setting the Room
    /// Rolloff Factor value to 1.0 specifies that the reflected sound will
    /// decay by 6 dB every time the distance doubles. Any value other than
    /// 1.0 is equivalent to a scaling factor.
    /// 
    /// \param rolloff new value to set between 0.f and 10.f. Defaults to 0.f
    ////////////////////////////////////////////////////////////
    void setRoomRolloff(float rolloff);


    ////////////////////////////////////////////////////////////
    /// \brief Returns the current density value
    /// \see setDensity()
    ////////////////////////////////////////////////////////////
    float getDensity() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the current diffusion value
    /// \see setDiffusion()
    ////////////////////////////////////////////////////////////
    float getDiffusion() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the current gain value
    /// \see setGain()
    ////////////////////////////////////////////////////////////
    float getGain() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the current decay time value
    /// \see setDecayTime()
    ////////////////////////////////////////////////////////////
    float getDecayTime() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the current reflection gain value
    /// \see setReflectionGain()
    ////////////////////////////////////////////////////////////
    float getReflectionGain() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the current reflection delay value
    /// \see setReflectionDelay()
    ////////////////////////////////////////////////////////////
    float getReflectionDelay() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the current late reverb gain value
    /// \see setLateReverbGain()
    ////////////////////////////////////////////////////////////
    float getLateReverbGain() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the current late reverb delay value
    /// \see setLateReverbDelay()
    ////////////////////////////////////////////////////////////
    float getLateReverbDelay() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the current room rolloff value
    /// \see setRoomRolloff()
    ////////////////////////////////////////////////////////////
    float getRoomRolloff() const;

    ////////////////////////////////////////////////////////////
    /// \brief Overload of assignment operator
    ///
    /// \param right Instance to assign
    ///
    /// \return Reference to self
    ///
    ////////////////////////////////////////////////////////////
    ReverbEffect& operator =(const ReverbEffect& right);

private:
    float m_density;
    float m_diffusion;
    float m_gain;
    float m_decayTime;
    float m_reflectionGain;
    float m_reflectionDelay;
    float m_lateReverbGain;
    float m_lateReverbDelay;
    float m_roomRolloff;

    //OpenAL handle if setType() is successful
    std::uint32_t m_effect; 

    ////////////////////////////////////////////////////////////
    /// \brief Used to apply the correct paramter names depending
    /// on whether the currnt system supports EAX or regular reverb
    ////////////////////////////////////////////////////////////
    void applyParameterNames();
};
}
#endif //SFML_REVERB_EFFECT_HPP

////////////////////////////////////////////////////////////
/// \class sf::ReverbEffect
/// \ingroup audio
///
/// Sound effect objects are applied to any SoundSource by
/// creating an instance of that effect and then using
/// sf::SoundSource::setEffect() to attach it. An effect
/// may be applied to multiple SourceSources at the same time
/// and to different types such as sf::Sound or sf::Music.
/// However a SoundSource may be only attached to a single effect
/// at any one time.
/// Much like an sf::SoundBuffer which supplies audio data to
/// an sf::Sound, an effect must be kept alive for at least as
/// long as the SoundSource using it. Potentially reverb (and
/// other delay type effects) will need to be kept alive longer
/// else the trailing audio effect (the audible 'echo') will be
/// cut short once an effect object is destroyed.
///
/// This class implements a reverb effect which is used to create
/// echo-like sounds such as being in a cave, cathedral or, more
/// subtly, in a small dense room.
///
/// Usage example:
/// \code
/// //create an effect
/// sf::ReverbEffect reverb;
/// reverb.setDecayTime(12.f);
/// reverb.setDensity(0.2f);
///
/// //load some music
/// sf::Music music;
/// if(music.loadFromFile("sound.ogg"))
/// {
///     //apply the effect to the music
///     music.setEffect(&reverb);
///     music.play();
/// }
///
/// \endcode
/// 
/// \see sf::SoundSource, sf::SoundEffect
///
////////////////////////////////////////////////////////////
