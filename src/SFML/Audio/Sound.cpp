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
Sound::Sound() :
m_buffer(NULL)
{
}


////////////////////////////////////////////////////////////
Sound::Sound(const SoundBuffer& buffer) :
m_buffer(NULL)
{
    SetBuffer(buffer);
}


////////////////////////////////////////////////////////////
Sound::Sound(const Sound& copy) :
SoundSource(copy),
m_buffer   (NULL)
{
    if (copy.m_buffer)
        SetBuffer(*copy.m_buffer);
    SetLoop(copy.GetLoop());
}


////////////////////////////////////////////////////////////
Sound::~Sound()
{
    Stop();
    if (m_buffer)
        m_buffer->DetachSound(this);
}


////////////////////////////////////////////////////////////
void Sound::Play()
{
    ALCheck(alSourcePlay(m_source));
}


////////////////////////////////////////////////////////////
void Sound::Pause()
{
    ALCheck(alSourcePause(m_source));
}


////////////////////////////////////////////////////////////
void Sound::Stop()
{
    ALCheck(alSourceStop(m_source));
}


////////////////////////////////////////////////////////////
void Sound::SetBuffer(const SoundBuffer& buffer)
{
    // First detach from the previous buffer
    if (m_buffer)
    {
        Stop();
        m_buffer->DetachSound(this);
    }

    // Assign and use the new buffer
    m_buffer = &buffer;
    m_buffer->AttachSound(this);
    ALCheck(alSourcei(m_source, AL_BUFFER, m_buffer->m_buffer));
}


////////////////////////////////////////////////////////////
void Sound::SetLoop(bool Loop)
{
    ALCheck(alSourcei(m_source, AL_LOOPING, Loop));
}


////////////////////////////////////////////////////////////
void Sound::SetPlayingOffset(Time timeOffset)
{
    ALCheck(alSourcef(m_source, AL_SEC_OFFSET, timeOffset.AsSeconds()));
}


////////////////////////////////////////////////////////////
const SoundBuffer* Sound::GetBuffer() const
{
    return m_buffer;
}


////////////////////////////////////////////////////////////
bool Sound::GetLoop() const
{
    ALint loop;
    ALCheck(alGetSourcei(m_source, AL_LOOPING, &loop));

    return loop != 0;
}


////////////////////////////////////////////////////////////
Time Sound::GetPlayingOffset() const
{
    ALfloat seconds = 0.f;
    ALCheck(alGetSourcef(m_source, AL_SEC_OFFSET, &seconds));

    return Seconds(seconds);
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
    if (m_buffer)
    {
        Stop();
        m_buffer->DetachSound(this);
        m_buffer = NULL;
    }

    // Copy the sound attributes
    if (right.m_buffer)
        SetBuffer(*right.m_buffer);
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
    ALCheck(alSourcei(m_source, AL_BUFFER, 0));
    m_buffer = NULL;
}

} // namespace sf
