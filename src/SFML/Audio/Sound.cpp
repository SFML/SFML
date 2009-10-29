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
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/ALCheck.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
Sound::Sound()
{
    ALCheck(alGenSources(1, &mySource));
    ALCheck(alSourcei(mySource, AL_BUFFER, 0));
}


////////////////////////////////////////////////////////////
Sound::Sound(const SoundBuffer& buffer, bool loop, float pitch, float volume, const Vector3f& position) :
myBuffer(NULL)
{
    ALCheck(alGenSources(1, &mySource));

    SetBuffer(buffer);
    SetLoop(loop);
    SetPitch(pitch);
    SetVolume(volume);
    SetPosition(position);
}


////////////////////////////////////////////////////////////
Sound::Sound(const Sound& copy) :
myBuffer(NULL)
{
    ALCheck(alGenSources(1, &mySource));

    if (copy.myBuffer)
        SetBuffer(*copy.myBuffer);
    SetLoop(copy.GetLoop());
    SetPitch(copy.GetPitch());
    SetVolume(copy.GetVolume());
    SetPosition(copy.GetPosition());
    SetRelativeToListener(copy.IsRelativeToListener());
    SetMinDistance(copy.GetMinDistance());
    SetAttenuation(copy.GetAttenuation());
}


////////////////////////////////////////////////////////////
Sound::~Sound()
{
    if (mySource)
    {
        if (myBuffer)
        {
            Stop();
            ALCheck(alSourcei(mySource, AL_BUFFER, 0));
        }
        ALCheck(alDeleteSources(1, &mySource));
    }
}


////////////////////////////////////////////////////////////
void Sound::Play()
{
    ALCheck(alSourcePlay(mySource));
}


////////////////////////////////////////////////////////////
void Sound::Pause()
{
    ALCheck(alSourcePause(mySource));
}


////////////////////////////////////////////////////////////
void Sound::Stop()
{
    ALCheck(alSourceStop(mySource));
}


////////////////////////////////////////////////////////////
void Sound::SetBuffer(const SoundBuffer& buffer)
{
    myBuffer = &buffer;
    ALCheck(alSourcei(mySource, AL_BUFFER, myBuffer->myBuffer));
}


////////////////////////////////////////////////////////////
void Sound::SetLoop(bool Loop)
{
    ALCheck(alSourcei(mySource, AL_LOOPING, Loop));
}


////////////////////////////////////////////////////////////
void Sound::SetPitch(float pitch)
{
    ALCheck(alSourcef(mySource, AL_PITCH, pitch));
}


////////////////////////////////////////////////////////////
void Sound::SetVolume(float volume)
{
    ALCheck(alSourcef(mySource, AL_GAIN, volume * 0.01f));
}

////////////////////////////////////////////////////////////
void Sound::SetPosition(float x, float y, float z)
{
    ALCheck(alSource3f(mySource, AL_POSITION, x, y, z));
}


////////////////////////////////////////////////////////////
void Sound::SetPosition(const Vector3f& position)
{
    SetPosition(position.x, position.y, position.z);
}


////////////////////////////////////////////////////////////
void Sound::SetRelativeToListener(bool relative)
{
    ALCheck(alSourcei(mySource, AL_SOURCE_RELATIVE, relative));
}


////////////////////////////////////////////////////////////
void Sound::SetMinDistance(float distance)
{
    ALCheck(alSourcef(mySource, AL_REFERENCE_DISTANCE, distance));
}


////////////////////////////////////////////////////////////
void Sound::SetAttenuation(float attenuation)
{
    ALCheck(alSourcef(mySource, AL_ROLLOFF_FACTOR, attenuation));
}


////////////////////////////////////////////////////////////
void Sound::SetPlayingOffset(float timeOffset)
{
    ALCheck(alSourcef(mySource, AL_SEC_OFFSET, timeOffset));
}


////////////////////////////////////////////////////////////
const SoundBuffer* Sound::GetBuffer() const
{
    return myBuffer;
}


////////////////////////////////////////////////////////////
bool Sound::GetLoop() const
{
    ALint loop;
    ALCheck(alGetSourcei(mySource, AL_LOOPING, &loop));

    return loop != 0;
}


////////////////////////////////////////////////////////////
float Sound::GetPitch() const
{
    ALfloat pitch;
    ALCheck(alGetSourcef(mySource, AL_PITCH, &pitch));

    return pitch;
}


////////////////////////////////////////////////////////////
float Sound::GetVolume() const
{
    ALfloat gain;
    ALCheck(alGetSourcef(mySource, AL_GAIN, &gain));

    return gain * 100.f;
}


////////////////////////////////////////////////////////////
Vector3f Sound::GetPosition() const
{
    Vector3f position;
    ALCheck(alGetSource3f(mySource, AL_POSITION, &position.x, &position.y, &position.z));

    return position;
}


////////////////////////////////////////////////////////////
bool Sound::IsRelativeToListener() const
{
    ALint relative;
    ALCheck(alGetSourcei(mySource, AL_SOURCE_RELATIVE, &relative));

    return relative != 0;
}


////////////////////////////////////////////////////////////
float Sound::GetMinDistance() const
{
    ALfloat distance;
    ALCheck(alGetSourcef(mySource, AL_REFERENCE_DISTANCE, &distance));

    return distance;
}


////////////////////////////////////////////////////////////
float Sound::GetAttenuation() const
{
    ALfloat attenuation;
    ALCheck(alGetSourcef(mySource, AL_ROLLOFF_FACTOR, &attenuation));

    return attenuation;
}


////////////////////////////////////////////////////////////
float Sound::GetPlayingOffset() const
{
    ALfloat seconds = 0.f;
    ALCheck(alGetSourcef(mySource, AL_SEC_OFFSET, &seconds));

    return seconds;
}


////////////////////////////////////////////////////////////
Sound::Status Sound::GetStatus() const
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


////////////////////////////////////////////////////////////
Sound& Sound::operator =(const Sound& other)
{
    Sound temp(other);

    std::swap(mySource, temp.mySource);
    std::swap(myBuffer, temp.myBuffer);

    return *this;
}

} // namespace sf
