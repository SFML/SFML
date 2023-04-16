////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Audio/SoundSource.hpp>

#include <algorithm>
#include <miniaudio.h>


namespace sf
{
// NOLINTBEGIN(readability-make-member-function-const)
////////////////////////////////////////////////////////////
SoundSource::SoundSource(const SoundSource& copy)
{
    setPitch(copy.getPitch());
    setPan(copy.getPan());
    setVolume(copy.getVolume());
    setSpatializationEnabled(copy.isSpatializationEnabled());
    setPosition(copy.getPosition());
    setDirection(copy.getDirection());
    setCone(copy.getCone());
    setVelocity(copy.getVelocity());
    setDopplerFactor(copy.getDopplerFactor());
    setRelativeToListener(copy.isRelativeToListener());
    setMinDistance(copy.getMinDistance());
    setMaxDistance(copy.getMaxDistance());
    setMinGain(copy.getMinGain());
    setMaxGain(copy.getMaxGain());
    setAttenuation(copy.getAttenuation());
}


////////////////////////////////////////////////////////////
void SoundSource::setPitch(float pitch)
{
    if (auto* sound = static_cast<ma_sound*>(getSound()); sound)
        ma_sound_set_pitch(sound, pitch);
}


////////////////////////////////////////////////////////////
void SoundSource::setPan(float pan)
{
    if (auto* sound = static_cast<ma_sound*>(getSound()); sound)
        ma_sound_set_pan(sound, pan);
}


////////////////////////////////////////////////////////////
void SoundSource::setVolume(float volume)
{
    if (auto* sound = static_cast<ma_sound*>(getSound()); sound)
        ma_sound_set_volume(sound, volume * 0.01f);
}


////////////////////////////////////////////////////////////
void SoundSource::setSpatializationEnabled(bool enabled)
{
    if (auto* sound = static_cast<ma_sound*>(getSound()); sound)
        ma_sound_set_spatialization_enabled(sound, enabled ? MA_TRUE : MA_FALSE);
}


////////////////////////////////////////////////////////////
void SoundSource::setPosition(const Vector3f& position)
{
    if (auto* sound = static_cast<ma_sound*>(getSound()); sound)
        ma_sound_set_position(sound, position.x, position.y, position.z);
}


////////////////////////////////////////////////////////////
void SoundSource::setDirection(const Vector3f& direction)
{
    if (auto* sound = static_cast<ma_sound*>(getSound()); sound)
        ma_sound_set_direction(sound, direction.x, direction.y, direction.z);
}


////////////////////////////////////////////////////////////
void SoundSource::setCone(const Cone& cone)
{
    if (auto* sound = static_cast<ma_sound*>(getSound()); sound)
        ma_sound_set_cone(sound,
                          std::clamp(cone.innerAngle.asRadians(), 0.f, sf::degrees(360).asRadians()),
                          std::clamp(cone.outerAngle.asRadians(), 0.f, sf::degrees(360).asRadians()),
                          cone.outerGain);
}


////////////////////////////////////////////////////////////
void SoundSource::setVelocity(const Vector3f& velocity)
{
    if (auto* sound = static_cast<ma_sound*>(getSound()); sound)
        ma_sound_set_velocity(sound, velocity.x, velocity.y, velocity.z);
}


////////////////////////////////////////////////////////////
void SoundSource::setDopplerFactor(float factor)
{
    if (auto* sound = static_cast<ma_sound*>(getSound()); sound)
        ma_sound_set_doppler_factor(sound, factor);
}


////////////////////////////////////////////////////////////
void SoundSource::setDirectionalAttenuationFactor(float factor)
{
    if (auto* sound = static_cast<ma_sound*>(getSound()); sound)
        ma_sound_set_directional_attenuation_factor(sound, factor);
}


////////////////////////////////////////////////////////////
void SoundSource::setRelativeToListener(bool relative)
{
    if (auto* sound = static_cast<ma_sound*>(getSound()); sound)
        ma_sound_set_positioning(sound, relative ? ma_positioning_relative : ma_positioning_absolute);
}


////////////////////////////////////////////////////////////
void SoundSource::setMinDistance(float distance)
{
    if (auto* sound = static_cast<ma_sound*>(getSound()); sound)
        ma_sound_set_min_distance(sound, distance);
}


////////////////////////////////////////////////////////////
void SoundSource::setMaxDistance(float distance)
{
    if (auto* sound = static_cast<ma_sound*>(getSound()); sound)
        ma_sound_set_max_distance(sound, distance);
}


////////////////////////////////////////////////////////////
void SoundSource::setMinGain(float gain)
{
    if (auto* sound = static_cast<ma_sound*>(getSound()); sound)
        ma_sound_set_min_gain(sound, gain);
}


////////////////////////////////////////////////////////////
void SoundSource::setMaxGain(float gain)
{
    if (auto* sound = static_cast<ma_sound*>(getSound()); sound)
        ma_sound_set_max_gain(sound, gain);
}


////////////////////////////////////////////////////////////
void SoundSource::setAttenuation(float attenuation)
{
    if (auto* sound = static_cast<ma_sound*>(getSound()); sound)
        ma_sound_set_rolloff(sound, attenuation);
}


////////////////////////////////////////////////////////////
float SoundSource::getPitch() const
{
    if (const auto* sound = static_cast<const ma_sound*>(getSound()); sound)
        return ma_sound_get_pitch(sound);

    return 0.f;
}


////////////////////////////////////////////////////////////
float SoundSource::getPan() const
{
    if (const auto* sound = static_cast<const ma_sound*>(getSound()); sound)
        return ma_sound_get_pan(sound);

    return 0.f;
}


////////////////////////////////////////////////////////////
float SoundSource::getVolume() const
{
    if (const auto* sound = static_cast<const ma_sound*>(getSound()); sound)
        return ma_sound_get_volume(sound) * 100.f;

    return 0.f;
}


////////////////////////////////////////////////////////////
bool SoundSource::isSpatializationEnabled() const
{
    if (const auto* sound = static_cast<const ma_sound*>(getSound()); sound)
        return ma_sound_is_spatialization_enabled(sound) == MA_TRUE;

    return false;
}


////////////////////////////////////////////////////////////
Vector3f SoundSource::getPosition() const
{
    if (const auto* sound = static_cast<const ma_sound*>(getSound()); sound)
    {
        auto position = ma_sound_get_position(sound);
        return Vector3f(position.x, position.y, position.z);
    }

    return Vector3f();
}


////////////////////////////////////////////////////////////
Vector3f SoundSource::getDirection() const
{
    if (const auto* sound = static_cast<const ma_sound*>(getSound()); sound)
    {
        auto direction = ma_sound_get_direction(sound);
        return Vector3f(direction.x, direction.y, direction.z);
    }

    return Vector3f();
}


////////////////////////////////////////////////////////////
SoundSource::Cone SoundSource::getCone() const
{
    if (const auto* sound = static_cast<const ma_sound*>(getSound()); sound)
    {
        float innerAngle = 0.f;
        float outerAngle = 0.f;
        Cone cone;
        ma_sound_get_cone(sound, &innerAngle, &outerAngle, &cone.outerGain);
        cone.innerAngle = sf::radians(innerAngle);
        cone.outerAngle = sf::radians(outerAngle);
        return cone;
    }

    return Cone{sf::Angle(), sf::Angle(), 0.f};
}


////////////////////////////////////////////////////////////
Vector3f SoundSource::getVelocity() const
{
    if (const auto* sound = static_cast<const ma_sound*>(getSound()); sound)
    {
        auto velocity = ma_sound_get_velocity(sound);
        return Vector3f(velocity.x, velocity.y, velocity.z);
    }

    return Vector3f();
}


////////////////////////////////////////////////////////////
float SoundSource::getDopplerFactor() const
{
    if (const auto* sound = static_cast<const ma_sound*>(getSound()); sound)
        return ma_sound_get_doppler_factor(sound);

    return 0.f;
}


////////////////////////////////////////////////////////////
float SoundSource::getDirectionalAttenuationFactor() const
{
    if (const auto* sound = static_cast<const ma_sound*>(getSound()); sound)
        return ma_sound_get_directional_attenuation_factor(sound);

    return 0.f;
}


////////////////////////////////////////////////////////////
bool SoundSource::isRelativeToListener() const
{
    if (const auto* sound = static_cast<const ma_sound*>(getSound()); sound)
        return ma_sound_get_positioning(sound) == ma_positioning_relative;

    return false;
}


////////////////////////////////////////////////////////////
float SoundSource::getMinDistance() const
{
    if (const auto* sound = static_cast<const ma_sound*>(getSound()); sound)
        return ma_sound_get_min_distance(sound);

    return 0.f;
}


////////////////////////////////////////////////////////////
float SoundSource::getMaxDistance() const
{
    if (const auto* sound = static_cast<const ma_sound*>(getSound()); sound)
        return ma_sound_get_max_distance(sound);

    return 0.f;
}


////////////////////////////////////////////////////////////
float SoundSource::getMinGain() const
{
    if (const auto* sound = static_cast<const ma_sound*>(getSound()); sound)
        return ma_sound_get_min_gain(sound);

    return 0.f;
}


////////////////////////////////////////////////////////////
float SoundSource::getMaxGain() const
{
    if (const auto* sound = static_cast<const ma_sound*>(getSound()); sound)
        return ma_sound_get_max_gain(sound);

    return 0.f;
}


////////////////////////////////////////////////////////////
float SoundSource::getAttenuation() const
{
    if (const auto* sound = static_cast<const ma_sound*>(getSound()); sound)
        return ma_sound_get_rolloff(sound);

    return 0.f;
}


////////////////////////////////////////////////////////////
SoundSource& SoundSource::operator=(const SoundSource& right)
{
    // Assign the sound attributes
    setPitch(right.getPitch());
    setVolume(right.getVolume());
    setPosition(right.getPosition());
    setRelativeToListener(right.isRelativeToListener());
    setMinDistance(right.getMinDistance());
    setAttenuation(right.getAttenuation());

    return *this;
}


////////////////////////////////////////////////////////////
SoundSource::Status SoundSource::getStatus() const
{
    if (const auto* sound = static_cast<const ma_sound*>(getSound()); sound && (ma_sound_is_playing(sound) == MA_TRUE))
        return Playing;

    return Stopped;
}
// NOLINTEND(readability-make-member-function-const)

} // namespace sf
