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
#include <SFML/Audio/OpenAL.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
Sound::Sound()
{
    ALCheck(alGenSources(1, &mySource));
    ALCheck(alSourcei(mySource, AL_BUFFER, 0));
}


////////////////////////////////////////////////////////////
/// Construct the sound from its parameters
////////////////////////////////////////////////////////////
Sound::Sound(const SoundBuffer& Buffer, bool Loop, float Pitch, float Volume, const Vector3f& Position) :
myBuffer(&Buffer)
{
    ALCheck(alGenSources(1, &mySource));

    ALCheck(alSourcei (mySource, AL_BUFFER,   Buffer.myBuffer));
    ALCheck(alSourcei (mySource, AL_LOOPING,  Loop));
    ALCheck(alSourcef (mySource, AL_PITCH,    Pitch));
    ALCheck(alSourcef (mySource, AL_GAIN,     Volume * 0.01f));
    ALCheck(alSource3f(mySource, AL_POSITION, Position.x, Position.y, Position.z));
}


////////////////////////////////////////////////////////////
/// Copy constructor
////////////////////////////////////////////////////////////
Sound::Sound(const Sound& Copy) :
AudioResource(Copy),
myBuffer     (Copy.myBuffer)
{
    ALCheck(alGenSources(1, &mySource));

    ALCheck(alSourcei (mySource, AL_BUFFER,   myBuffer ? myBuffer->myBuffer : 0));
    ALCheck(alSourcei (mySource, AL_LOOPING,  Copy.GetLoop()));
    ALCheck(alSourcef (mySource, AL_PITCH,    Copy.GetPitch()));
    ALCheck(alSourcef (mySource, AL_GAIN,     Copy.GetVolume() * 0.01f));
    ALCheck(alSource3f(mySource, AL_POSITION, Copy.GetPosition().x, Copy.GetPosition().y, Copy.GetPosition().z));
}


////////////////////////////////////////////////////////////
/// Destructor
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
/// Play the sound
////////////////////////////////////////////////////////////
void Sound::Play()
{
    ALCheck(alSourcePlay(mySource));
}


////////////////////////////////////////////////////////////
/// Pause the sound
////////////////////////////////////////////////////////////
void Sound::Pause()
{
    ALCheck(alSourcePause(mySource));
}


////////////////////////////////////////////////////////////
/// Stop the sound
////////////////////////////////////////////////////////////
void Sound::Stop()
{
    ALCheck(alSourceStop(mySource));
}


////////////////////////////////////////////////////////////
/// Set the source buffer
////////////////////////////////////////////////////////////
void Sound::SetBuffer(const SoundBuffer& Buffer)
{
    myBuffer = &Buffer;
    ALCheck(alSourcei(mySource, AL_BUFFER, myBuffer ? myBuffer->myBuffer : 0));
}


////////////////////////////////////////////////////////////
/// Set the sound loop state
////////////////////////////////////////////////////////////
void Sound::SetLoop(bool Loop)
{
    ALCheck(alSourcei(mySource, AL_LOOPING, Loop));
}


////////////////////////////////////////////////////////////
/// Set the sound pitch
////////////////////////////////////////////////////////////
void Sound::SetPitch(float Pitch)
{
    ALCheck(alSourcef(mySource, AL_PITCH, Pitch));
}


////////////////////////////////////////////////////////////
/// Set the sound volume
////////////////////////////////////////////////////////////
void Sound::SetVolume(float Volume)
{
    ALCheck(alSourcef(mySource, AL_GAIN, Volume * 0.01f));
}

////////////////////////////////////////////////////////////
/// Set the sound position (take 3 values).
/// The default position is (0, 0, 0)
////////////////////////////////////////////////////////////
void Sound::SetPosition(float X, float Y, float Z)
{
    ALCheck(alSource3f(mySource, AL_POSITION, X, Y, Z));
}


////////////////////////////////////////////////////////////
/// Set the sound position (take a 3D vector).
/// The default position is (0, 0, 0)
////////////////////////////////////////////////////////////
void Sound::SetPosition(const Vector3f& Position)
{
    SetPosition(Position.x, Position.y, Position.z);
}


////////////////////////////////////////////////////////////
/// Make the sound's position relative to the listener's
/// position, or absolute.
/// The default value is false (absolute)
////////////////////////////////////////////////////////////
void Sound::SetRelativeToListener(bool Relative)
{
    ALCheck(alSourcei(mySource, AL_SOURCE_RELATIVE, Relative));
}


////////////////////////////////////////////////////////////
/// Set the minimum distance - closer than this distance,
/// the listener will hear the sound at its maximum volume.
/// The default minimum distance is 1.0
////////////////////////////////////////////////////////////
void Sound::SetMinDistance(float MinDistance)
{
    ALCheck(alSourcef(mySource, AL_REFERENCE_DISTANCE, MinDistance));
}


////////////////////////////////////////////////////////////
/// Set the attenuation factor - the higher the attenuation, the
/// more the sound will be attenuated with distance from listener.
/// The default attenuation factor 1.0
////////////////////////////////////////////////////////////
void Sound::SetAttenuation(float Attenuation)
{
    ALCheck(alSourcef(mySource, AL_ROLLOFF_FACTOR, Attenuation));
}


////////////////////////////////////////////////////////////
/// Set the current playing position of the sound
////////////////////////////////////////////////////////////
void Sound::SetPlayingOffset(float TimeOffset)
{
    ALCheck(alSourcef(mySource, AL_SEC_OFFSET, TimeOffset));
}


////////////////////////////////////////////////////////////
/// Get the source buffer
////////////////////////////////////////////////////////////
const SoundBuffer* Sound::GetBuffer() const
{
    return myBuffer;
}


////////////////////////////////////////////////////////////
/// Tell whether or not the sound is looping
////////////////////////////////////////////////////////////
bool Sound::GetLoop() const
{
    ALint Loop;
    ALCheck(alGetSourcei(mySource, AL_LOOPING, &Loop));

    return Loop != 0;
}


////////////////////////////////////////////////////////////
/// Get the pitch
////////////////////////////////////////////////////////////
float Sound::GetPitch() const
{
    ALfloat Pitch;
    ALCheck(alGetSourcef(mySource, AL_PITCH, &Pitch));

    return Pitch;
}


////////////////////////////////////////////////////////////
/// Get the volume
////////////////////////////////////////////////////////////
float Sound::GetVolume() const
{
    ALfloat Gain;
    ALCheck(alGetSourcef(mySource, AL_GAIN, &Gain));

    return Gain * 100.f;
}


////////////////////////////////////////////////////////////
/// Get the sound position
////////////////////////////////////////////////////////////
Vector3f Sound::GetPosition() const
{
    Vector3f Position;
    ALCheck(alGetSource3f(mySource, AL_POSITION, &Position.x, &Position.y, &Position.z));

    return Position;
}


////////////////////////////////////////////////////////////
/// Tell if the sound's position is relative to the listener's
/// position, or if it's absolute
////////////////////////////////////////////////////////////
bool Sound::IsRelativeToListener() const
{
    ALint Relative;
    ALCheck(alGetSourcei(mySource, AL_SOURCE_RELATIVE, &Relative));

    return Relative != 0;
}


////////////////////////////////////////////////////////////
/// Get the minimum distance
////////////////////////////////////////////////////////////
float Sound::GetMinDistance() const
{
    ALfloat MinDistance;
    ALCheck(alGetSourcef(mySource, AL_REFERENCE_DISTANCE, &MinDistance));

    return MinDistance;
}


////////////////////////////////////////////////////////////
/// Get the attenuation factor
////////////////////////////////////////////////////////////
float Sound::GetAttenuation() const
{
    ALfloat Attenuation;
    ALCheck(alGetSourcef(mySource, AL_ROLLOFF_FACTOR, &Attenuation));

    return Attenuation;
}


////////////////////////////////////////////////////////////
/// Get the current playing position of the sound
////////////////////////////////////////////////////////////
float Sound::GetPlayingOffset() const
{
    ALfloat Seconds = 0.f;
    ALCheck(alGetSourcef(mySource, AL_SEC_OFFSET, &Seconds));

    return Seconds;
}


////////////////////////////////////////////////////////////
/// Get the status of the sound (stopped, paused, playing)
////////////////////////////////////////////////////////////
Sound::Status Sound::GetStatus() const
{
    ALint State;
    ALCheck(alGetSourcei(mySource, AL_SOURCE_STATE, &State));

    switch (State)
    {
        case AL_INITIAL :
        case AL_STOPPED : return Stopped;
        case AL_PAUSED :  return Paused;
        case AL_PLAYING : return Playing;
    }

    return Stopped;
}


////////////////////////////////////////////////////////////
/// Assignment operator
////////////////////////////////////////////////////////////
Sound& Sound::operator =(const Sound& Other)
{
    Sound Temp(Other);

    std::swap(mySource, Temp.mySource);
    std::swap(myBuffer, Temp.myBuffer);

    return *this;
}

} // namespace sf
