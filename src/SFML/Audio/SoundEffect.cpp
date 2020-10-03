#include <SFML/Audio/AudioDevice.hpp>
#include <SFML/Audio/SoundEffect.hpp>
#include <SFML/Audio/SoundSource.hpp>
#include <SFML/System/Err.hpp>

#include "ALCheck.hpp"

#define AL_ALEXT_PROTOTYPES
#include <AL/efx.h>
#include <AL/alext.h>

#include <unordered_map>

namespace
{
    //reference counted effects object handle.
    //this is to enables sharing of objects between effects slots
    struct CountedEffect
    {
        std::uint32_t handle = 0;
        std::uint32_t count = 0;
    };
    std::unordered_map<sf::SoundEffect::Type, CountedEffect> effects;
}

namespace sf
{
SoundEffect::SoundEffect()
    : m_effectSlot  (0),
    m_effect        (0),
    m_type          (Null),
    m_volume        (1.f)
{
    alCheck(alGenAuxiliaryEffectSlots(1, &m_effectSlot));
}

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

SoundEffect::~SoundEffect()
{
    std::set<SoundSource*> sounds;
    m_soundlist.swap(sounds);

    for (auto* sound : sounds)
    {
        sound->resetEffect();
    }

    alCheck(alAuxiliaryEffectSloti(m_effectSlot, AL_EFFECTSLOT_EFFECT, AL_EFFECT_NULL));
    alCheck(alDeleteAuxiliaryEffectSlots(1, &m_effectSlot));

    auto& effect = effects.at(m_type);
    effect.count--;

    if (effect.count == 0)
    {
        alCheck(alDeleteEffects(1, &effect.handle));
    }
}

//public
bool SoundEffect::isAvailable()
{
    return priv::AudioDevice::isExtensionSupported("ALC_EXT_EFX");
}

SoundEffect::Type SoundEffect::getType() const
{
    return m_type;
}

void SoundEffect::setVolume(float vol)
{
    m_volume = std::min(1.f, std::max(0.f, vol));

    alCheck(alAuxiliaryEffectSlotf(m_effectSlot, AL_EFFECTSLOT_GAIN, m_volume));
}

float SoundEffect::getVolume() const
{
    return m_volume;
}

//protected
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

void SoundEffect::applyEffect()
{
    alCheck(alAuxiliaryEffectSloti(m_effectSlot, AL_EFFECTSLOT_EFFECT, m_effect));
}

//private
void SoundEffect::attachSoundSource(SoundSource* sound) const
{
    m_soundlist.insert(sound);
}

void SoundEffect::detachSoundSource(SoundSource* sound) const
{
    m_soundlist.erase(sound);
}

void SoundEffect::ensureEffect(Type type)
{
    if (effects.count(type) == 0)
    {
        effects.insert(std::make_pair(type, CountedEffect()));

        auto& effect = effects.at(type);
        alCheck(alGenEffects(1, &effect.handle));
    }

    auto& effect = effects.at(type);
    effect.count++;

    m_effect = effect.handle;
}
} // namespace sf
