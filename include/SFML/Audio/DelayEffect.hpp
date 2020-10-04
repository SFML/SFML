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

#ifndef SFML_DELAY_EFFECT_HPP
#define SFML_DELAY_EFFECT_HPP

#include <SFML/Audio/SoundEffect.hpp>

namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Implements a delay sound effect
///
////////////////////////////////////////////////////////////
class SFML_AUDIO_API DelayEffect : public SoundEffect
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    DelayEffect();

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    /// \param copy Instance to copy
    ///
    ////////////////////////////////////////////////////////////
    DelayEffect(const DelayEffect& copy);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current delay value
    ///
    /// This property controls the delay between the original sound
    /// and the first 'tap', or echo instance. Subsequently, the
    /// value for Delay is used to determine the time delay
    /// between each 'second tap' and the next 'first tap'.
    /// 
    /// \param delay The delay in seconds to set, between 0.f and 0.207f, Defaults to 0.1
    ////////////////////////////////////////////////////////////
    void setDelay(float delay);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current LR delay value
    ///
    /// This property controls the delay between the first 'tap' and
    /// the second 'tap'. Subsequently, the value for Echo LR Delay
    /// is used to determine the time delay between each 'first tap'
    /// and the next 'second tap'.
    /// 
    /// \param delay The LR delay in seconds to set, between 0.f and 0.404f, Defaults to 0.1
    ////////////////////////////////////////////////////////////
    void setLRDelay(float delay);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current damping value
    ///
    /// This property controls the amount of high frequency damping
    /// applied to each echo. As the sound is subsequently fed back
    /// for further echoes, damping results in an echo which progressively
    /// gets softer in tone as well as intensity.
    /// 
    /// \param damping The damping value to set, between 0.f and 0.99f, Defaults to 0.5
    ////////////////////////////////////////////////////////////
    void setDamping(float damping);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current feedback value
    ///
    /// This property controls the amount of feedback the output signal
    /// fed back into the input. Use this parameter to create "cascading"
    /// echoes. At full magnitude, the identical sample will repeat endlessly.
    /// Below full magnitude, the sample will repeat and fade.
    /// 
    /// \param feedback The feedback value to set, between 0.f and 1.f, Defaults to 0.5
    ////////////////////////////////////////////////////////////
    void setFeedback(float feedback);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current spread value
    ///
    /// This property controls how hard panned the individual echoes are.
    /// With a value of 1.0, the first 'tap' will be panned hard left, and
    /// the second tap hard right. A value of –1.0 gives the opposite result.
    /// Settings nearer to 0.0 result in less emphasized panning.
    /// 
    /// \param spread The spread value to set, between -1.f and 1.f, Defaults to -1.0
    ////////////////////////////////////////////////////////////
    void setSpread(float spread);


    ////////////////////////////////////////////////////////////
    /// \brief Returns the current delay value
    /// \see setDelay()
    ////////////////////////////////////////////////////////////
    float getDelay() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the current LRDelay value
    /// \see setLRDelay()
    ////////////////////////////////////////////////////////////
    float getLRDelay() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the current damping value
    /// \see setDamping()
    ////////////////////////////////////////////////////////////
    float getDamping() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the current feedback value
    /// \see setFeedback()
    ////////////////////////////////////////////////////////////
    float getFeedback() const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns the current spread value
    /// \see setSpread()
    ////////////////////////////////////////////////////////////
    float getSpread() const;

private:

    float m_delay;
    float m_LRDelay;
    float m_damping;
    float m_feedback;
    float m_spread;

    //OpenAL handle returned by setType()
    std::uint32_t m_effect;
};
}

#endif //SFML_DELAY_EFFECT_HPP

////////////////////////////////////////////////////////////
/// \class sf::DelayEffect
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
/// This class implements a delay effect which generates discrete,
/// delayed instances of the input signal. The amount of delay and
/// feedback is controllable. The delay is 'two tap' – you can control
/// the interaction between two separate instances of echoes.
///
/// Usage example:
/// \code
/// //create an effect
/// sf::DelayEffect delay;
/// delay.setDelay(0.2f);
/// delay.setFeedback(0.2f);
///
/// //load some music
/// sf::Music music;
/// if(music.loadFromFile("sound.ogg"))
/// {
///     //apply the effect to the music
///     music.setEffect(&delay);
///     music.play();
/// }
/// \endcode
/// 
/// \see sf::SoundSource, sf::SoundEffect
///
////////////////////////////////////////////////////////////
