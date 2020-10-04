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

#include <SFML/Audio/AudioDevice.hpp>
#include <SFML/Audio/SoundEffect.hpp>
#include <SFML/Audio/SoundSource.hpp>
#include <SFML/System/Err.hpp>

#include "ALCheck.hpp"

#define AL_ALEXT_PROTOTYPES
#include <AL/efx.h>
#include <AL/alext.h>

#include <map>

namespace
{
    //reference counted effects object handle.
    //this is to enables sharing of objects between effects slots
    //see ensureEffect()
    struct CountedEffect
    {
        std::uint32_t handle = 0;
        std::uint32_t count = 0;
    };
    std::map<sf::SoundEffect::Type, CountedEffect> effects;
}

namespace sf
{
////////////////////////////////////////////////////////////
SoundEffect::SoundEffect()
    : m_effectSlot  (0),
    m_effect        (0),
    m_type          (Null),
    m_volume        (1.f)
{
    alCheck(alGenAuxiliaryEffectSlots(1, &m_effectSlot));
}


////////////////////////////////////////////////////////////
SoundEffect::SoundEffect(const SoundEffect& copy)
    : m_effectSlot  (0),
    m_effect        (0),
    m_type          (Null),
    m_volume        (1.f)
{
    alCheck(alGenAuxiliaryEffectSlots(1, &m_effectSlot));

    //copy properties from copy
    setVolume(copy.getVolume());
}


////////////////////////////////////////////////////////////
SoundEffect::~SoundEffect()
{
    std::set<SoundSource*> sounds;
    m_soundlist.swap(sounds);

    for (std::set<SoundSource*>::const_iterator it = sounds.begin(); it != sounds.end(); ++it)
    {
        (*it)->resetEffect();
    }

    alCheck(alAuxiliaryEffectSloti(m_effectSlot, AL_EFFECTSLOT_EFFECT, AL_EFFECT_NULL));
    alCheck(alDeleteAuxiliaryEffectSlots(1, &m_effectSlot));

    CountedEffect& effect = effects.at(m_type);
    effect.count--;

    if (effect.count == 0)
    {
        alCheck(alDeleteEffects(1, &effect.handle));
        effects.erase(m_type);
    }
}


////////////////////////////////////////////////////////////
bool SoundEffect::isAvailable()
{
    return priv::AudioDevice::isExtensionSupported("ALC_EXT_EFX");
}


////////////////////////////////////////////////////////////
SoundEffect::Type SoundEffect::getType() const
{
    return m_type;
}


////////////////////////////////////////////////////////////
void SoundEffect::setVolume(float vol)
{
    m_volume = std::min(1.f, std::max(0.f, vol));

    alCheck(alAuxiliaryEffectSlotf(m_effectSlot, AL_EFFECTSLOT_GAIN, m_volume));
}


////////////////////////////////////////////////////////////
float SoundEffect::getVolume() const
{
    return m_volume;
}


////////////////////////////////////////////////////////////
std::uint32_t SoundEffect::setType(SoundEffect::Type type)
{
    switch (type)
    {
    default:
        err() << type << ": not a known effect type." << std::endl;
        return 0;
    case Reverb:
        ensureEffect(type);

        //check if EAX reverb is  available
        if (alGetEnumValue("AL_EFFECT_EAXREVERB") != 0)
        {
            alCheck(alEffecti(m_effect, AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB));
        }
        else
        {
            alCheck(alEffecti(m_effect, AL_EFFECT_TYPE, AL_EFFECT_REVERB));
        }
        break;
    case Chorus:
        ensureEffect(type);
        alCheck(alEffecti(m_effect, AL_EFFECT_TYPE, AL_EFFECT_CHORUS));
        break;
    }

    m_type = type;
    alCheck(alAuxiliaryEffectSloti(m_effectSlot, AL_EFFECTSLOT_EFFECT, m_effect));
    return m_effect;
}


////////////////////////////////////////////////////////////
void SoundEffect::applyEffect()
{
    alCheck(alAuxiliaryEffectSloti(m_effectSlot, AL_EFFECTSLOT_EFFECT, m_effect));
}


////////////////////////////////////////////////////////////
void SoundEffect::attachSoundSource(SoundSource* sound) const
{
    m_soundlist.insert(sound);
}


////////////////////////////////////////////////////////////
void SoundEffect::detachSoundSource(SoundSource* sound) const
{
    m_soundlist.erase(sound);
}


////////////////////////////////////////////////////////////
void SoundEffect::ensureEffect(Type type)
{
    if (effects.count(type) == 0)
    {
        effects.insert(std::make_pair(type, CountedEffect()));

        CountedEffect& effect = effects.at(type);
        alCheck(alGenEffects(1, &effect.handle));
    }

    CountedEffect& effect = effects.at(type);
    effect.count++;

    m_effect = effect.handle;
}
} // namespace sf
