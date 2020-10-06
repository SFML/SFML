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

#define AL_ALEXT_PROTOTYPES
#include <AL/efx.h>
#include <AL/alext.h>

#include <cassert>
#include <algorithm>

namespace sf
{
//default values as p108 of OpenAL extensions guide
////////////////////////////////////////////////////////////
DelayEffect::DelayEffect()
    : SoundEffect   (AL_EFFECT_ECHO),
    m_delay         (0.1f),
    m_LRDelay       (0.1f),
    m_damping       (0.5f),
    m_feedback      (0.5f),
    m_spread        (-1.f)
{

}


////////////////////////////////////////////////////////////
DelayEffect::DelayEffect(const DelayEffect& copy)
    : SoundEffect   (copy),
    m_delay         (0.1f),
    m_LRDelay       (0.1f),
    m_damping       (0.5f),
    m_feedback      (0.5f),
    m_spread        (-1.f)
{
    setDelay(copy.getDelay());
    setLRDelay(copy.getLRDelay());
    setDamping(copy.getDamping());
    setFeedback(copy.getFeedback());
    setSpread(copy.getSpread());
}


////////////////////////////////////////////////////////////
void DelayEffect::setDelay(float delay)
{
    m_delay = std::min(0.27f, std::max(0.f, delay));
    setParameter(AL_ECHO_DELAY, m_delay);
}


////////////////////////////////////////////////////////////
void DelayEffect::setLRDelay(float delay)
{
    m_LRDelay = std::min(0.404f, std::max(0.f, delay));
    setParameter(AL_ECHO_LRDELAY, m_LRDelay);
}


////////////////////////////////////////////////////////////
void DelayEffect::setDamping(float damping)
{
    m_damping = std::min(0.99f, std::max(0.f, damping));
    setParameter(AL_ECHO_DAMPING, m_damping);
}


////////////////////////////////////////////////////////////
void DelayEffect::setFeedback(float feedback)
{
    m_feedback = std::min(1.f, std::max(0.f, feedback));
    setParameter(AL_ECHO_FEEDBACK, m_feedback);
}


////////////////////////////////////////////////////////////
void DelayEffect::setSpread(float spread)
{
    m_spread = std::min(1.f, std::max(-1.f, spread));
    setParameter(AL_ECHO_SPREAD, m_spread);
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


////////////////////////////////////////////////////////////
DelayEffect& DelayEffect::operator= (const DelayEffect& right)
{
    SoundEffect::operator=(right);

    setDelay(right.getDelay());
    setLRDelay(right.getLRDelay());
    setDamping(right.getDamping());
    setFeedback(right.getFeedback());
    setSpread(right.getSpread());

    return *this;
}

} //naemspace sf
