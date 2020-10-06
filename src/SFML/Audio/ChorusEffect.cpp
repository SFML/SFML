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

#define AL_ALEXT_PROTOTYPES
#include <AL/efx.h>
#include <AL/alext.h>

#include <cassert>
#include <algorithm>

namespace sf
{
//defaults are set as p67 of the effect extension guide
////////////////////////////////////////////////////////////
ChorusEffect::ChorusEffect()
    : SoundEffect   (AL_EFFECT_CHORUS),
    m_waveform      (Triangle),
    m_phase         (90),
    m_rate          (1.1f),
    m_depth         (0.1f),
    m_feedback      (0.25f),
    m_delay         (0.016f)
{

}


////////////////////////////////////////////////////////////
ChorusEffect::ChorusEffect(const ChorusEffect& copy)
    : SoundEffect   (copy),
    m_waveform      (Triangle),
    m_phase         (90),
    m_rate          (1.1f),
    m_depth         (0.1f),
    m_feedback      (0.25f),
    m_delay         (0.016f)
{
    setWaveform(copy.getWaveform());
    setPhase(copy.getPhase());
    setRate(copy.getRate());
    setDepth(copy.getDepth());
    setFeedback(copy.getFeedback());
    setDelay(copy.getDelay());
}


////////////////////////////////////////////////////////////
void ChorusEffect::setWaveform(Waveform waveform)
{
    m_waveform = waveform;
    setParameter(AL_CHORUS_WAVEFORM, waveform == Triangle ? AL_CHORUS_WAVEFORM_TRIANGLE : AL_CHORUS_WAVEFORM_SINUSOID);
}


////////////////////////////////////////////////////////////
void ChorusEffect::setPhase(sf::Int32 phase)
{
    m_phase = std::min(180, std::max(-180, phase));
    setParameter(AL_CHORUS_PHASE, m_phase);
}


////////////////////////////////////////////////////////////
void ChorusEffect::setRate(float rate)
{
    m_rate = std::min(10.f, std::max(0.f, rate));
    setParameter(AL_CHORUS_RATE, m_rate);
}


////////////////////////////////////////////////////////////
void ChorusEffect::setDepth(float depth)
{
    m_depth = std::min(1.f, std::max(0.f, depth));
    setParameter(AL_CHORUS_DEPTH, m_depth);
}


////////////////////////////////////////////////////////////
void ChorusEffect::setFeedback(float feedback)
{
    m_feedback = std::min(1.f, std::max(-1.f, feedback));
    setParameter(AL_CHORUS_FEEDBACK, m_feedback);
}


////////////////////////////////////////////////////////////
void ChorusEffect::setDelay(float delay)
{
    m_delay = std::min(0.016f, std::max(0.f, delay));
    setParameter(AL_CHORUS_DELAY, m_delay);
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


////////////////////////////////////////////////////////////
ChorusEffect& ChorusEffect::operator= (const ChorusEffect& right)
{
    SoundEffect::operator=(right);

    setWaveform(right.getWaveform());
    setPhase(right.getPhase());
    setRate(right.getRate());
    setDepth(right.getDepth());
    setFeedback(right.getFeedback());
    setDelay(right.getDelay());

    return *this;
}
} // namespace sf
