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
#include <SFML/Audio/DelayEffect.hpp>

#include "ALCheck.hpp"

#define AL_ALEXT_PROTOTYPES
#include <AL/efx.h>
#include <AL/alext.h>

#include <cassert>

namespace sf
{
//default values as p108 of OpenAL extensions guide
////////////////////////////////////////////////////////////
DelayEffect::DelayEffect()
    : SoundEffect   (),
    m_delay         (0.1f),
    m_LRDelay       (0.1f),
    m_damping       (0.5f),
    m_feedback      (0.5f),
    m_spread        (-1.f),
    m_effect        (0)
{
    m_effect = setType(Delay);
}


////////////////////////////////////////////////////////////
DelayEffect::DelayEffect(const DelayEffect& copy)
    : SoundEffect   (copy),
    m_delay         (0.1f),
    m_LRDelay       (0.1f),
    m_damping       (0.5f),
    m_feedback      (0.5f),
    m_spread        (-1.f),
    m_effect        (0)
{
    m_effect = setType(Delay);

    setDelay(copy.getDelay());
    setLRDelay(copy.getLRDelay());
    setDamping(copy.getDamping());
    setFeedback(copy.getFeedback());
    setSpread(copy.getSpread());
}


////////////////////////////////////////////////////////////
void DelayEffect::setDelay(float delay)
{
    assert(m_effect != 0);

    m_delay = std::min(0.27f, std::max(0.f, delay));
    alCheck(alEffectf(m_effect, AL_ECHO_DELAY, m_delay));

    applyEffect();
}


////////////////////////////////////////////////////////////
void DelayEffect::setLRDelay(float delay)
{
    assert(m_effect != 0);

    m_LRDelay = std::min(0.404f, std::max(0.f, delay));
    alCheck(alEffectf(m_effect, AL_ECHO_LRDELAY, m_LRDelay));

    applyEffect();
}


////////////////////////////////////////////////////////////
void DelayEffect::setDamping(float damping)
{
    assert(m_effect != 0);

    m_damping = std::min(0.99f, std::max(0.f, damping));
    alCheck(alEffectf(m_effect, AL_ECHO_DAMPING, m_damping));

    applyEffect();
}


////////////////////////////////////////////////////////////
void DelayEffect::setFeedback(float feedback)
{
    assert(m_effect != 0);

    m_feedback = std::min(1.f, std::max(0.f, feedback));
    alCheck(alEffectf(m_effect, AL_ECHO_FEEDBACK, m_feedback));

    applyEffect();
}


////////////////////////////////////////////////////////////
void DelayEffect::setSpread(float spread)
{
    assert(m_effect != 0);

    m_spread = std::min(1.f, std::max(-1.f, spread));
    alCheck(alEffectf(m_effect, AL_ECHO_SPREAD, m_spread));

    applyEffect();
}


////////////////////////////////////////////////////////////
float DelayEffect::getDelay() const
{
    return m_delay;
}


////////////////////////////////////////////////////////////
float DelayEffect::getLRDelay() const
{
    return m_LRDelay;
}


////////////////////////////////////////////////////////////
float DelayEffect::getDamping() const
{
    return m_damping;
}


////////////////////////////////////////////////////////////
float DelayEffect::getFeedback() const
{
    return m_feedback;
}

////////////////////////////////////////////////////////////
float DelayEffect::getSpread() const
{
    return m_spread;
}
}
