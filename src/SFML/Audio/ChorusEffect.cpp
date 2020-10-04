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
#include <SFML/Audio/ChorusEffect.hpp>

#include "ALCheck.hpp"

#define AL_ALEXT_PROTOTYPES
#include <AL/efx.h>
#include <AL/alext.h>

#include <cassert>

namespace sf
{
//defaults are set as p67 of the effect extension guide
////////////////////////////////////////////////////////////
ChorusEffect::ChorusEffect()
    : SoundEffect   (),
    m_waveform      (Triangle),
    m_phase         (90),
    m_rate          (1.1f),
    m_depth         (0.1f),
    m_feedback      (0.25f),
    m_delay         (0.016f),
    m_effect        (0)
{
    m_effect = setType(Chorus);
}


////////////////////////////////////////////////////////////
ChorusEffect::ChorusEffect(const ChorusEffect& copy)
    : SoundEffect   (copy),
    m_waveform      (Triangle),
    m_phase         (90),
    m_rate          (1.1f),
    m_depth         (0.1f),
    m_feedback      (0.25f),
    m_delay         (0.016f),
    m_effect        (0)
{
    m_effect = setType(Chorus);
}


////////////////////////////////////////////////////////////
void ChorusEffect::setWaveform(Waveform waveform)
{
    assert(m_effect != 0);

    m_waveform = waveform;
    alCheck(alEffecti(m_effect, AL_CHORUS_WAVEFORM, waveform == Triangle ? AL_CHORUS_WAVEFORM_TRIANGLE : AL_CHORUS_WAVEFORM_SINUSOID));

    applyEffect();
}


////////////////////////////////////////////////////////////
void ChorusEffect::setPhase(sf::Int32 phase)
{
    assert(m_effect != 0);

    m_phase = std::min(180, std::max(-180, phase));
    alCheck(alEffecti(m_effect, AL_CHORUS_PHASE, m_phase));

    applyEffect();
}


////////////////////////////////////////////////////////////
void ChorusEffect::setRate(float rate)
{
    assert(m_effect != 0);

    m_rate = std::min(10.f, std::max(0.f, rate));
    alCheck(alEffectf(m_effect, AL_CHORUS_RATE, m_rate));

    applyEffect();
}


////////////////////////////////////////////////////////////
void ChorusEffect::setDepth(float depth)
{
    assert(m_effect != 0);

    m_depth = std::min(1.f, std::max(0.f, depth));
    alCheck(alEffectf(m_effect, AL_CHORUS_DEPTH, m_depth));

    applyEffect();
}


////////////////////////////////////////////////////////////
void ChorusEffect::setFeedback(float feedback)
{
    assert(m_effect != 0);

    m_feedback = std::min(1.f, std::max(-1.f, feedback));
    alCheck(alEffectf(m_effect, AL_CHORUS_FEEDBACK, m_feedback));

    applyEffect();
}


////////////////////////////////////////////////////////////
void ChorusEffect::setDelay(float delay)
{
    assert(m_effect != 0);

    m_delay = std::min(0.016f, std::max(0.f, delay));
    alCheck(alEffectf(m_effect, AL_CHORUS_DELAY, m_delay));

    applyEffect();
}


////////////////////////////////////////////////////////////
ChorusEffect::Waveform ChorusEffect::getWaveform() const
{
    return m_waveform;
}


////////////////////////////////////////////////////////////
sf::Int32 ChorusEffect::getPhase() const
{
    return m_phase;
}

float ChorusEffect::getRate() const
{
    return m_rate;
}


////////////////////////////////////////////////////////////
float ChorusEffect::getDepth() const
{
    return m_depth;
}


////////////////////////////////////////////////////////////
float ChorusEffect::getFeedback() const
{
    return m_feedback;
}


////////////////////////////////////////////////////////////
float ChorusEffect::getDelay() const
{
    return m_delay;
}
}
