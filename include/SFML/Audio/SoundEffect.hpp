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
#include <cstdint>

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
    /// \brief Used to determine which type of effect has been implemented
    ///
    /// Generally not useful for classes outside of those which implement
    /// a specific type of effect.
    /// 
    ////////////////////////////////////////////////////////////
    enum Type
    {
        Null,
        Reverb,
        Chorus,
        Delay
    };

    ////////////////////////////////////////////////////////////
    /// \brief Returns the type of effect implemented
    ///
    ////////////////////////////////////////////////////////////
    Type getType() const;

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
    /// \brief Sets the type of effect this will be
    ///
    /// When implementing a specific effect type this sould be called on effect
    /// creation so that the underlying effects object is properly configured
    ///
    /// \returns A handle to the OpenAL effects object. This can be used by
    /// effects classes to modify specific effects properties.
    ///
    ////////////////////////////////////////////////////////////
    std::uint32_t setType(Type type);

    ////////////////////////////////////////////////////////////
    /// \brief Reapplies any effects parameters.
    ///
    /// When a derived class updates any effects parameters this must be called
    /// so that the new paramter values take effect.
    ///
    ////////////////////////////////////////////////////////////
    void applyEffect();


    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Protected so that it may only be called by derived classes
    ///
    ////////////////////////////////////////////////////////////
    SoundEffect();

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

    std::uint32_t m_effectSlot; //!< OpenAL Effects slot handle
    std::uint32_t m_effect;     //!< OpenAL Effect handle assigned to this slot

    Type m_type;                //!< Type of effect assigned to this slot
    float m_volume;             //!< Current volume of this slot

    mutable std::set<SoundSource*> m_soundlist; //!< List of SoundSources using this effect
    void ensureEffect(Type);    //!< Used to ensure that there is only one instance of OpenAL effect object for each type
};
}

#endif // SFML_SOUNDEFFECT_HPP

////////////////////////////////////////////////////////////
/// \class sf::SoundEffect
/// \ingroup audio
///
/// Base class for creating OpenAL sound effects. This class is
/// used to wrap an OpenAL EffectsSlot and assign an Effect object
/// to it. It is up to derived classes to implemented specific
/// effect types, the interface for adjusting paramters for that
/// effect, and making sure that this class knows which effect is
/// being implemented.
///
/// This is done by the derived class calling setType() with a
/// given type defined in the SoundEffect::Type enumeration. This
/// class will then return a handle to an effect object of that type
/// if it is successful, else 0 if the type requested was invalid
/// or could not be created for some reason.
/// 
/// \see sf::ReverbEffect
///
////////////////////////////////////////////////////////////
