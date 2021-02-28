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

#ifndef SFML_SOUNDEFFECT_HPP
#define SFML_SOUNDEFFECT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/Export.hpp>
#include <SFML/Audio/AlResource.hpp>
#include <set>

namespace sf
{
class SoundSource;

////////////////////////////////////////////////////////////
/// \brief Base class for implementing audio effects
///
////////////////////////////////////////////////////////////
class SFML_AUDIO_API SoundEffect : AlResource
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Returns true if audio effects are available on the current platform
    ///
    ////////////////////////////////////////////////////////////
    static bool isAvailable();

    ////////////////////////////////////////////////////////////
    /// \brief Sets the volume for this effect
    ///
    /// The volume is in the range 0 - 1, multiplying the output volume
    /// of any SoundSource which uses this effect. A value of 0 will
    /// effectively mute all sounds that have this effect applied
    ///
    /// \param volume Floating point value automatically clamped between 0 and 1
    ///
    ////////////////////////////////////////////////////////////
    void setVolume(float volume);

    ////////////////////////////////////////////////////////////
    /// \brief Returns the current volume of this effect
    ///
    ////////////////////////////////////////////////////////////
    float getVolume() const;

protected:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Protected so that it may only be called by derived classes
    ///
    /// \param alEffectType AL_EFFECT_X enum type to declare which effect
    /// should be used.
    ///
    ////////////////////////////////////////////////////////////
    explicit SoundEffect(int alEffectType);

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    /// Protected so that it may only be called by derived classes
    ///
    /// \param copy Instance to copy
    ///
    ////////////////////////////////////////////////////////////
    SoundEffect(const SoundEffect& copy);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~SoundEffect();

    ////////////////////////////////////////////////////////////
    /// \brief Sets an effect parameter
    ///
    /// \param parameter AL_EFFECTNAME_PARAM enum declaraing
    /// parameter name to be set
    ///
    /// \param value Value to set the parameter to
    ///
    ////////////////////////////////////////////////////////////
    void setParameter(int parameter, float value);

    ////////////////////////////////////////////////////////////
    /// \brief Sets an effect parameter
    ///
    /// \param parameter AL_EFFECTNAME_PARAM enum declaraing
    /// parameter name to be set
    ///
    /// \param value Value to set the parameter to
    ///
    ////////////////////////////////////////////////////////////
    void setParameter(int parameter, int value);

    ////////////////////////////////////////////////////////////
    /// \brief Overload of assignment operator
    ///
    /// \param right Instance to assign
    ///
    /// \return Reference to self
    ///
    ////////////////////////////////////////////////////////////
    SoundEffect& operator =(const SoundEffect& right);

private:
    friend class SoundSource;

    ////////////////////////////////////////////////////////////
    /// \brief Add a sound to the list of sounds that use this effect
    ///
    /// \param sound SoundSource instance to attach
    ///
    ////////////////////////////////////////////////////////////
    void attachSoundSource(SoundSource* source) const;

    ////////////////////////////////////////////////////////////
    /// \brief Remove a sound from the list of sounds that use this effect
    ///
    /// \param sound SoundSource instance to detach
    ///
    ////////////////////////////////////////////////////////////
    void detachSoundSource(SoundSource* source) const;

    sf::Uint32 m_effectSlot;                    //!< OpenAL Effects slot handle
    sf::Uint32 m_effect;                        //!< OpenAL Effect handle assigned to this slot
    int m_type;                                 //!< OpenAL Effect type used to track reference counting

    float m_volume;                             //!< Current volume of this slot

    mutable std::set<SoundSource*> m_soundlist; //!< List of SoundSources using this effect
    void ensureEffect(int type);                //!< Used to ensure that there is only one instance of OpenAL effect object for each type
};
}

#endif // SFML_SOUNDEFFECT_HPP

////////////////////////////////////////////////////////////
/// \class sf::SoundEffect
/// \ingroup audio
///
/// Base class for creating OpenAL sound effects. This class is
/// used to wrap an OpenAL EffectsSlot and assign an Effect object
/// to it. It is up to derived classes to implemented the interface
/// for adjusting parameters a specific effect type.
///
/// 
/// \see sf::ReverbEffect, sf::ChorusEffect, sf::DelayEffect
///
////////////////////////////////////////////////////////////
