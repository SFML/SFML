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
#include <SFML/Audio/ReverbEffect.hpp>

#define AL_ALEXT_PROTOTYPES
#include <AL/efx.h>
#include <AL/alext.h>

#include <cassert>
#include <algorithm>

namespace
{
    //these are defined so that they can be shared between EAX/non-EAX
    //versions of the reverb effect. See applyParameterNames()
    sf::Uint32 REVERB_DENSITY              = 0;
    sf::Uint32 REVERB_DIFFUSION            = 0;
    sf::Uint32 REVERB_GAIN                 = 0;
    sf::Uint32 REVERB_DECAY_TIME           = 0;
    sf::Uint32 REVERB_REFLECTIONS_GAIN     = 0;
    sf::Uint32 REVERB_REFLECTIONS_DELAY    = 0;
    sf::Uint32 REVERB_LATE_REVERB_GAIN     = 0;
    sf::Uint32 REVERB_LATE_REVERB_DELAY    = 0;
    sf::Uint32 REVERB_ROOM_ROLLOFF_FACTOR  = 0;
}

namespace sf
{
//values are initialised according to OpenAL extension guide p.66
////////////////////////////////////////////////////////////
ReverbEffect::ReverbEffect()
    : SoundEffect       (alGetEnumValue("AL_EFFECT_EAXREVERB") == 0 ? AL_EFFECT_REVERB : AL_EFFECT_EAXREVERB),
    m_density           (1.f),
    m_diffusion         (1.f),
    m_gain              (0.32f),
    m_decayTime         (1.4f),
    m_reflectionGain    (0.05f),
    m_reflectionDelay   (0.007f),
    m_lateReverbGain    (1.26f),
    m_lateReverbDelay   (0.011f),
    m_roomRolloff       (0.f)
{
    applyParameterNames();
}


////////////////////////////////////////////////////////////
ReverbEffect::ReverbEffect(const ReverbEffect& copy)
    : SoundEffect       (copy),
    m_density           (1.f),
    m_diffusion         (1.f),
    m_gain              (0.32f),
    m_decayTime         (1.4f),
    m_reflectionGain    (0.05f),
    m_reflectionDelay   (0.007f),
    m_lateReverbGain    (1.26f),
    m_lateReverbDelay   (0.011f),
    m_roomRolloff       (0.f)
{
    applyParameterNames();

    setDensity(copy.getDensity());
    setDiffusion(copy.getDiffusion());
    setGain(copy.getGain());
    setDecayTime(copy.getDecayTime());
    setReflectionGain(copy.getReflectionGain());
    setReflectionDelay(copy.getReflectionDelay());
    setLateReverbGain(copy.getLateReverbGain());
    setLateReverbDelay(copy.getLateReverbDelay());
    setRoomRolloff(copy.getRoomRolloff());
}


////////////////////////////////////////////////////////////
void ReverbEffect::setDensity(float density)
{
    m_density = std::min(1.f, std::max(0.f, density));
    setParameter(REVERB_DENSITY, m_density);
}


////////////////////////////////////////////////////////////
void ReverbEffect::setDiffusion(float diffusion)
{
    m_diffusion = std::min(1.f, std::max(0.f, diffusion));
    setParameter(REVERB_DIFFUSION, m_density);
}


////////////////////////////////////////////////////////////
void ReverbEffect::setGain(float gain)
{
    m_gain = std::min(1.f, std::max(0.f, gain));
    setParameter(REVERB_GAIN, m_gain);
}


////////////////////////////////////////////////////////////
void ReverbEffect::setDecayTime(float decayTime)
{
    m_decayTime = std::min(20.f, std::max(0.1f, decayTime));
    setParameter(REVERB_DECAY_TIME, m_decayTime);
}


////////////////////////////////////////////////////////////
void ReverbEffect::setReflectionGain(float gain)
{
    m_reflectionGain = std::min(1.f, std::max(0.f, gain));
    setParameter(REVERB_REFLECTIONS_GAIN, m_reflectionGain);
}


////////////////////////////////////////////////////////////
void ReverbEffect::setReflectionDelay(float delay)
{
    m_reflectionDelay = std::min(0.3f, std::max(0.f, delay));
    setParameter(REVERB_REFLECTIONS_DELAY, m_reflectionDelay);
}


////////////////////////////////////////////////////////////
void ReverbEffect::setLateReverbGain(float gain)
{
    m_lateReverbGain = std::min(10.f, std::max(0.f, gain));
    setParameter(REVERB_LATE_REVERB_GAIN, m_lateReverbGain);
}


////////////////////////////////////////////////////////////
void ReverbEffect::setLateReverbDelay(float delay)
{
    m_lateReverbDelay = std::min(0.1f, std::max(0.f, delay));
    setParameter(REVERB_LATE_REVERB_DELAY, m_lateReverbDelay);
}


////////////////////////////////////////////////////////////
void ReverbEffect::setRoomRolloff(float rolloff)
{
    m_roomRolloff = std::min(10.f, std::max(0.f, rolloff));
    setParameter(REVERB_ROOM_ROLLOFF_FACTOR, m_roomRolloff);
}


////////////////////////////////////////////////////////////
float ReverbEffect::getDensity() const
{
    return m_density;
}


////////////////////////////////////////////////////////////
float ReverbEffect::getDiffusion() const
{
    return m_diffusion;
}


////////////////////////////////////////////////////////////
float ReverbEffect::getGain() const
{
    return m_gain;
}


////////////////////////////////////////////////////////////
float ReverbEffect::getDecayTime() const
{
    return m_decayTime;
}


////////////////////////////////////////////////////////////
float ReverbEffect::getReflectionGain() const
{
    return m_reflectionGain;
}


////////////////////////////////////////////////////////////
float ReverbEffect::getReflectionDelay() const
{
    return m_reflectionDelay;
}


////////////////////////////////////////////////////////////
float ReverbEffect::getLateReverbGain() const
{
    return m_lateReverbGain;
}


////////////////////////////////////////////////////////////
float ReverbEffect::getLateReverbDelay() const
{
    return m_lateReverbDelay;
}


////////////////////////////////////////////////////////////
float ReverbEffect::getRoomRolloff() const
{
    return m_roomRolloff;
}


////////////////////////////////////////////////////////////
ReverbEffect& ReverbEffect::operator= (const ReverbEffect& right)
{
    SoundEffect::operator=(right);

    setDensity(right.getDensity());
    setDiffusion(right.getDiffusion());
    setGain(right.getGain());
    setDecayTime(right.getDecayTime());
    setReflectionGain(right.getReflectionGain());
    setReflectionDelay(right.getReflectionDelay());
    setLateReverbGain(right.getLateReverbGain());
    setLateReverbDelay(right.getLateReverbDelay());
    setRoomRolloff(right.getRoomRolloff());

    return *this;
}


////////////////////////////////////////////////////////////
void ReverbEffect::applyParameterNames()
{
    //sets the correct values depending on if this
    //is an EAX reverb or not
    if (alGetEnumValue("AL_EFFECT_EAXREVERB") != 0)
    {
        REVERB_DENSITY = AL_EAXREVERB_DENSITY;
        REVERB_DIFFUSION = AL_EAXREVERB_DIFFUSION;
        REVERB_GAIN = AL_EAXREVERB_GAIN;
        REVERB_DECAY_TIME = AL_EAXREVERB_DECAY_TIME;
        REVERB_REFLECTIONS_GAIN = AL_EAXREVERB_REFLECTIONS_GAIN;
        REVERB_REFLECTIONS_DELAY = AL_EAXREVERB_REFLECTIONS_DELAY;
        REVERB_LATE_REVERB_GAIN = AL_EAXREVERB_LATE_REVERB_GAIN;
        REVERB_LATE_REVERB_DELAY = AL_EAXREVERB_LATE_REVERB_DELAY;
        REVERB_ROOM_ROLLOFF_FACTOR = AL_EAXREVERB_ROOM_ROLLOFF_FACTOR;
    }
    else
    {
        REVERB_DENSITY = AL_REVERB_DENSITY;
        REVERB_DIFFUSION = AL_REVERB_DIFFUSION;
        REVERB_GAIN = AL_REVERB_GAIN;
        REVERB_DECAY_TIME = AL_REVERB_DECAY_TIME;
        REVERB_REFLECTIONS_GAIN = AL_REVERB_REFLECTIONS_GAIN;
        REVERB_REFLECTIONS_DELAY = AL_REVERB_REFLECTIONS_DELAY;
        REVERB_LATE_REVERB_GAIN = AL_REVERB_LATE_REVERB_GAIN;
        REVERB_LATE_REVERB_DELAY = AL_REVERB_LATE_REVERB_DELAY;
        REVERB_ROOM_ROLLOFF_FACTOR = AL_REVERB_ROOM_ROLLOFF_FACTOR;
    }
}
} // namespace sf
