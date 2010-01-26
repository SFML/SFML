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
}


////////////////////////////////////////////////////////////
Sound::Sound(const SoundBuffer& buffer, bool loop, float pitch, float volume, const Vector3f& position) :
myBuffer(NULL)
{
    SetBuffer(buffer);
    SetLoop(loop);
    SetPitch(pitch);
    SetVolume(volume);
    SetPosition(position);
}


////////////////////////////////////////////////////////////
Sound::Sound(const Sound& copy) :
SoundSource(copy),
myBuffer   (NULL)
{
    if (copy.myBuffer)
        SetBuffer(*copy.myBuffer);
    SetLoop(copy.GetLoop());
}


////////////////////////////////////////////////////////////
Sound::~Sound()
{
    Stop();
    if (myBuffer)
        myBuffer->DetachSound(this);
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
    // First detach from the previous buffer
    if (myBuffer)
    {
        Stop();
        myBuffer->DetachSound(this);
    }

    // Assign and use the new buffer
    myBuffer = &buffer;
    myBuffer->AttachSound(this);
    ALCheck(alSourcei(mySource, AL_BUFFER, myBuffer->myBuffer));
}


////////////////////////////////////////////////////////////
void Sound::SetLoop(bool Loop)
{
    ALCheck(alSourcei(mySource, AL_LOOPING, Loop));
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
float Sound::GetPlayingOffset() const
{
    ALfloat seconds = 0.f;
    ALCheck(alGetSourcef(mySource, AL_SEC_OFFSET, &seconds));

    return seconds;
}


////////////////////////////////////////////////////////////
Sound::Status Sound::GetStatus() const
{
    return SoundSource::GetStatus();
}


////////////////////////////////////////////////////////////
Sound& Sound::operator =(const Sound& right)
{
    // Here we don't use the copy-and-swap idiom, because it would mess up
    // the list of sound instances contained in the buffers

    // Detach the sound instance from the previous buffer (if any)
    if (myBuffer)
    {
        Stop();
        myBuffer->DetachSound(this);
        myBuffer = NULL;
    }

    // Copy the sound attributes
    if (right.myBuffer)
        SetBuffer(*right.myBuffer);
    SetLoop(right.GetLoop());
    SetPitch(right.GetPitch());
    SetVolume(right.GetVolume());
    SetPosition(right.GetPosition());
    SetRelativeToListener(right.IsRelativeToListener());
    SetMinDistance(right.GetMinDistance());
    SetAttenuation(right.GetAttenuation());

    return *this;
}


////////////////////////////////////////////////////////////
void Sound::ResetBuffer()
{
    // First stop the sound in case it is playing
    Stop();

    // Detach the buffer
    ALCheck(alSourcei(mySource, AL_BUFFER, 0));
    myBuffer = NULL;
}

} // namespace sf
