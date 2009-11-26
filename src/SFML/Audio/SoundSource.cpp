////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Audio/ALCheck.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
SoundSource::SoundSource()
{
    priv::EnsureALInit();

    ALCheck(alGenSources(1, &mySource));
    ALCheck(alSourcei(mySource, AL_BUFFER, 0));
}


////////////////////////////////////////////////////////////
SoundSource::SoundSource(const SoundSource& copy)
{
    priv::EnsureALInit();

    ALCheck(alGenSources(1, &mySource));
    ALCheck(alSourcei(mySource, AL_BUFFER, 0));

    SetPitch(copy.GetPitch());
    SetVolume(copy.GetVolume());
    SetPosition(copy.GetPosition());
    SetRelativeToListener(copy.IsRelativeToListener());
    SetMinDistance(copy.GetMinDistance());
    SetAttenuation(copy.GetAttenuation());
}


////////////////////////////////////////////////////////////
SoundSource::~SoundSource()
{
    ALCheck(alSourcei(mySource, AL_BUFFER, 0));
    ALCheck(alDeleteSources(1, &mySource));
}


////////////////////////////////////////////////////////////
void SoundSource::SetPitch(float pitch)
{
    ALCheck(alSourcef(mySource, AL_PITCH, pitch));
}


////////////////////////////////////////////////////////////
void SoundSource::SetVolume(float volume)
{
    ALCheck(alSourcef(mySource, AL_GAIN, volume * 0.01f));
}

////////////////////////////////////////////////////////////
void SoundSource::SetPosition(float x, float y, float z)
{
    ALCheck(alSource3f(mySource, AL_POSITION, x, y, z));
}


////////////////////////////////////////////////////////////
void SoundSource::SetPosition(const Vector3f& position)
{
    SetPosition(position.x, position.y, position.z);
}


////////////////////////////////////////////////////////////
void SoundSource::SetRelativeToListener(bool relative)
{
    ALCheck(alSourcei(mySource, AL_SOURCE_RELATIVE, relative));
}


////////////////////////////////////////////////////////////
void SoundSource::SetMinDistance(float distance)
{
    ALCheck(alSourcef(mySource, AL_REFERENCE_DISTANCE, distance));
}


////////////////////////////////////////////////////////////
void SoundSource::SetAttenuation(float attenuation)
{
    ALCheck(alSourcef(mySource, AL_ROLLOFF_FACTOR, attenuation));
}


////////////////////////////////////////////////////////////
float SoundSource::GetPitch() const
{
    ALfloat pitch;
    ALCheck(alGetSourcef(mySource, AL_PITCH, &pitch));

    return pitch;
}


////////////////////////////////////////////////////////////
float SoundSource::GetVolume() const
{
    ALfloat gain;
    ALCheck(alGetSourcef(mySource, AL_GAIN, &gain));

    return gain * 100.f;
}


////////////////////////////////////////////////////////////
Vector3f SoundSource::GetPosition() const
{
    Vector3f position;
    ALCheck(alGetSource3f(mySource, AL_POSITION, &position.x, &position.y, &position.z));

    return position;
}


////////////////////////////////////////////////////////////
bool SoundSource::IsRelativeToListener() const
{
    ALint relative;
    ALCheck(alGetSourcei(mySource, AL_SOURCE_RELATIVE, &relative));

    return relative != 0;
}


////////////////////////////////////////////////////////////
float SoundSource::GetMinDistance() const
{
    ALfloat distance;
    ALCheck(alGetSourcef(mySource, AL_REFERENCE_DISTANCE, &distance));

    return distance;
}


////////////////////////////////////////////////////////////
float SoundSource::GetAttenuation() const
{
    ALfloat attenuation;
    ALCheck(alGetSourcef(mySource, AL_ROLLOFF_FACTOR, &attenuation));

    return attenuation;
}


////////////////////////////////////////////////////////////
SoundSource::Status SoundSource::GetStatus() const
{
    ALint status;
    ALCheck(alGetSourcei(mySource, AL_SOURCE_STATE, &status));

    switch (status)
    {
        case AL_INITIAL :
        case AL_STOPPED : return Stopped;
        case AL_PAUSED :  return Paused;
        case AL_PLAYING : return Playing;
    }

    return Stopped;
}

} // namespace sf
