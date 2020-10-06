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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#ifndef SFML_CHORUS_EFFECT_HPP
#define SFML_CHORUS_EFFECT_HPP

#include <SFML/Audio/SoundEffect.hpp>

namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Implements a chorus sound effect
///
////////////////////////////////////////////////////////////
class SFML_AUDIO_API ChorusEffect : public SoundEffect
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    ChorusEffect();

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    /// \param copy Instance to copy
    ///
    ////////////////////////////////////////////////////////////
    ChorusEffect(const ChorusEffect& copy);


    ////////////////////////////////////////////////////////////
    /// \brief Waveform enum
    ///
    /// \see setWaveform()
    ////////////////////////////////////////////////////////////
    enum Waveform
    {
        Sine,
        Triangle
    };


    ////////////////////////////////////////////////////////////
    /// \brief Set the current waveform value
    ///
    /// This property sets the waveform shape of the LFO that controls
    /// the delay time of the delayed signals.
    /// 
    /// \param waveform new Waveform to set. Defaults to Waveform::Triangle
    ////////////////////////////////////////////////////////////
    void setWaveform(Waveform waveform);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current phase value
    ///
    /// This property controls the phase difference between the left and
    /// right LFOs. At zero degrees the two LFOs are synchronized. Use
    /// this parameter to create the illusion of an expanded stereo field
    /// of the output signal.
    /// 
    /// \param angle The angle of phase to set, between -180 and 180. Defaults to 90
    ////////////////////////////////////////////////////////////
    void setPhase(sf::Int32 angle);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current rate value
    ///
    /// This property sets the modulation rate of the LFO that controls
    /// the delay time of the delayed signals.
    /// 
    /// \param rate new rate to set in hz, between 0.f and 10.f. Defaults to 1.1f
    ////////////////////////////////////////////////////////////
    void setRate(float rate);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current depth value
    ///
    /// This property controls the amount by which the delay time is modulated by the LFO
    /// 
    /// \param depth new depth to set, between 0.f and 1.f. Defaults to 0.1f
    ////////////////////////////////////////////////////////////
    void setDepth(float depth);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current feedback value
    ///
    /// This property controls the amount of processed signal that is fed back to the
    /// input of the chorus effect. Negative values will reverse the phase of the feedback
    /// signal. At full magnitude the identical sample will repeat endlessly. At lower
    /// magnitudes the sample will repeat and fade out over time. Use this parameter to
    /// create a "cascading" chorus effect.
    /// 
    /// \param amount new feedback amount to set, between -1.f and 1.f. Defaults to 0.25f
    ////////////////////////////////////////////////////////////
    void setFeedback(float amount);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current delay value
    ///
    /// This property controls the average amount of time the sample is delayed before
    /// it is played back, and with feedback, the amount of time between iterations of
    /// the sample.Larger values lower the pitch. Smaller values make the chorus sound
    /// like a flanger, but with different frequency characteristics.
    /// 
    /// \param delay new delay to set in second, between 0.f and 0.016f. Defaults to 0.016f
    ////////////////////////////////////////////////////////////
    void setDelay(float delay);

    ////////////////////////////////////////////////////////////
    /// \brief Returns the current waveform value
    /// \see setWaveform()
    ////////////////////////////////////////////////////////////
    Waveform getWaveform() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the current phase value
    /// \see setPhase()
    ////////////////////////////////////////////////////////////
    sf::Int32 getPhase() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the current rate value
    /// \see setRate()
    ////////////////////////////////////////////////////////////
    float getRate() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the current depth value
    /// \see setDepth()
    ////////////////////////////////////////////////////////////
    float getDepth() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the current feedback value
    /// \see setFeedback()
    ////////////////////////////////////////////////////////////
    float getFeedback() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the current delay value
    /// \see setDelay()
    ////////////////////////////////////////////////////////////
    float getDelay() const;

    ////////////////////////////////////////////////////////////
    /// \brief Overload of assignment operator
    ///
    /// \param right Instance to assign
    ///
    /// \return Reference to self
    ///
    ////////////////////////////////////////////////////////////
    ChorusEffect& operator =(const ChorusEffect& right);

private:

    Waveform m_waveform;
    sf::Int32 m_phase;
    float m_rate;
    float m_depth;
    float m_feedback;
    float m_delay;
};
}

#endif //SFML_CHORUS_EFFECT_HPP

////////////////////////////////////////////////////////////
/// \class sf::ChorusEffect
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
/// long as the SoundSource using it.
///
/// This class implements a chorus effect which is used to create
/// 'thicker' sounds by doubling the input via a small delay.
///
/// Usage example:
/// \code
/// //create an effect
/// sf::ChorusEffect chorus;
/// chorus.setPhase(120);
/// chorus.setFeedback(0.2f);
///
/// //load some music
/// sf::Music music;
/// if(music.loadFromFile("sound.ogg"))
/// {
///     //apply the effect to the music
///     music.setEffect(&chorus);
///     music.play();
/// }
/// \endcode
/// 
/// \see sf::SoundSource, sf::SoundEffect
///
////////////////////////////////////////////////////////////
