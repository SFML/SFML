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

#include <SFML/Audio/SoundEffect.hpp>

#ifndef SFML_SYSTEM_IOS

#include <SFML/Audio/AudioDevice.hpp>
#include <SFML/Audio/SoundSource.hpp>

#include "ALCheck.hpp"

#define AL_ALEXT_PROTOTYPES
#include <AL/efx.h>
#include <AL/alext.h>

#include <map>
#include <algorithm>
#include <memory>

namespace
{
    //reference counted effects object handle.
    //this is to enable sharing of objects between effects slots
    //see ensureEffect()
    struct CountedEffect
    {
        CountedEffect() : handle(0), count(0) {}

        sf::Uint32 handle;
        sf::Uint32 count;
    };
    std::map<int, CountedEffect> effects;
}

namespace sf
{
////////////////////////////////////////////////////////////
SoundEffect::SoundEffect(int effectType)
    : m_effectSlot      (0),
    m_effect            (0),
    m_type              (effectType),
    m_volumeMultiplier  (1.f)
{
    alCheck(alGenAuxiliaryEffectSlots(1, &m_effectSlot));

    ensureEffect(effectType);
    alCheck(alEffecti(m_effect, AL_EFFECT_TYPE, effectType));
    alCheck(alAuxiliaryEffectSloti(m_effectSlot, AL_EFFECTSLOT_EFFECT, static_cast<ALint>(m_effect)));
}


////////////////////////////////////////////////////////////
SoundEffect::SoundEffect(const SoundEffect& copy)
    : m_effectSlot      (0),
    m_effect            (0),
    m_type              (copy.m_type),
    m_volumeMultiplier  (1.f)
{
    alCheck(alGenAuxiliaryEffectSlots(1, &m_effectSlot));

    //make sure we properly reference count our handle
    ensureEffect(m_type);
    alCheck(alAuxiliaryEffectSloti(m_effectSlot, AL_EFFECTSLOT_EFFECT, static_cast<ALint>(m_effect)));

    //copy properties from copy
    setVolumeMultiplier(copy.getVolumeMultiplier());
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
void SoundEffect::setVolumeMultiplier(float value)
{
    m_volumeMultiplier = std::min(1.f, std::max(0.f, value));

    alCheck(alAuxiliaryEffectSlotf(m_effectSlot, AL_EFFECTSLOT_GAIN, m_volumeMultiplier));
}


////////////////////////////////////////////////////////////
float SoundEffect::getVolumeMultiplier() const
{
    return m_volumeMultiplier;
}


////////////////////////////////////////////////////////////
void SoundEffect::setParameter(int parameter, float value)
{
    alCheck(alEffectf(m_effect, parameter, value));
    alCheck(alAuxiliaryEffectSloti(m_effectSlot, AL_EFFECTSLOT_EFFECT, static_cast<ALint>(m_effect)));
}


////////////////////////////////////////////////////////////
void SoundEffect::setParameter(int parameter, int value)
{
    alCheck(alEffecti(m_effect, parameter, value));
    alCheck(alAuxiliaryEffectSloti(m_effectSlot, AL_EFFECTSLOT_EFFECT, static_cast<ALint>(m_effect)));
}

////////////////////////////////////////////////////////////
SoundEffect& SoundEffect::operator=(const SoundEffect& right)
{
    SoundEffect temp(right);
    std::swap(m_soundlist, temp.m_soundlist);

    return *this;
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
void SoundEffect::ensureEffect(int type)
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


#else

//empty implementation for IOS
#include <SFML/System/Err.hpp>

namespace
{
    bool hasWarned = false;
}

namespace sf
{
////////////////////////////////////////////////////////////
SoundEffect::SoundEffect(int effectType)
    : m_effectSlot      (0),
    m_effect            (0),
    m_type              (effectType),
    m_volumeMultiplier  (1.f)
{
    if (!hasWarned)
    {
        sf::err() << "Sound effects are unavailable on IOS\n";
        hasWarned = true;
    }
}


////////////////////////////////////////////////////////////
SoundEffect::SoundEffect(const SoundEffect& copy)
    : m_effectSlot      (0),
    m_effect            (0),
    m_type              (copy.m_type),
    m_volumeMultiplier  (1.f)
{

}


////////////////////////////////////////////////////////////
SoundEffect::~SoundEffect()
{

}


////////////////////////////////////////////////////////////
bool SoundEffect::isAvailable()
{
    return false;
}


////////////////////////////////////////////////////////////
void SoundEffect::setVolumeMultiplier(float)
{

}


////////////////////////////////////////////////////////////
float SoundEffect::getVolumeMultiplier() const
{
    return m_volumeMultiplier;
}


////////////////////////////////////////////////////////////
void SoundEffect::setParameter(int, float)
{

}


////////////////////////////////////////////////////////////
void SoundEffect::setParameter(int, int)
{

}

////////////////////////////////////////////////////////////
SoundEffect& SoundEffect::operator=(const SoundEffect&)
{
    return *this;
}


////////////////////////////////////////////////////////////
void SoundEffect::attachSoundSource(SoundSource*) const
{

}


////////////////////////////////////////////////////////////
void SoundEffect::detachSoundSource(SoundSource*) const
{

}


////////////////////////////////////////////////////////////
void SoundEffect::ensureEffect(int)
{

}
} // namespace sf

#endif //SFML_SYSTEM_IOS
