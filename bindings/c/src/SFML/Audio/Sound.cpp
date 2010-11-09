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
#include <SFML/Audio/Sound.h>
#include <SFML/Audio/SoundStruct.h>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Construct a new sound
////////////////////////////////////////////////////////////
sfSound* sfSound_Create(void)
{
    return new sfSound;
}


////////////////////////////////////////////////////////////
/// Copy an existing sound
////////////////////////////////////////////////////////////
sfSound* sfSound_Copy(sfSound* sound)
{
    CSFML_CHECK_RETURN(sound, NULL);

    return new sfSound(*sound);
}


////////////////////////////////////////////////////////////
/// Destroy an existing sound
////////////////////////////////////////////////////////////
void sfSound_Destroy(sfSound* sound)
{
    delete sound;
}


////////////////////////////////////////////////////////////
/// Start playing a sound
////////////////////////////////////////////////////////////
void sfSound_Play(sfSound* sound)
{
    CSFML_CALL(sound, Play())
}


////////////////////////////////////////////////////////////
/// Pause a sound
////////////////////////////////////////////////////////////
void sfSound_Pause(sfSound* sound)
{
    CSFML_CALL(sound, Pause())
}


////////////////////////////////////////////////////////////
/// Stop playing a sound
////////////////////////////////////////////////////////////
void sfSound_Stop(sfSound* sound)
{
    CSFML_CALL(sound, Stop())
}


////////////////////////////////////////////////////////////
/// Bind a sound buffer to a sound
////////////////////////////////////////////////////////////
void sfSound_SetBuffer(sfSound* sound, const sfSoundBuffer* buffer)
{
    if (buffer)
    {
        CSFML_CALL(sound, SetBuffer(buffer->This))
        sound->Buffer = buffer;
    }
}


////////////////////////////////////////////////////////////
/// Get the sound buffer bound to a sound
////////////////////////////////////////////////////////////
const sfSoundBuffer* sfSound_GetBuffer(const sfSound* sound)
{
    CSFML_CHECK_RETURN(sound, NULL)

    return sound->Buffer;
}


////////////////////////////////////////////////////////////
/// Set a sound loop state
////////////////////////////////////////////////////////////
void sfSound_SetLoop(sfSound* sound, sfBool loop)
{
    CSFML_CALL(sound, SetLoop(loop == sfTrue))
}


////////////////////////////////////////////////////////////
/// Tell whether or not a sound is looping
////////////////////////////////////////////////////////////
sfBool sfSound_GetLoop(const sfSound* sound)
{
    CSFML_CALL_RETURN(sound, GetLoop(), sfFalse)
}


////////////////////////////////////////////////////////////
/// Get the status of a sound (stopped, paused, playing)
////////////////////////////////////////////////////////////
sfSoundStatus sfSound_GetStatus(const sfSound* sound)
{
    CSFML_CHECK_RETURN(sound, sfStopped);

    return static_cast<sfSoundStatus>(sound->This.GetStatus());
}


////////////////////////////////////////////////////////////
/// Set the pitch of a sound
////////////////////////////////////////////////////////////
void sfSound_SetPitch(sfSound* sound, float pitch)
{
    CSFML_CALL(sound, SetPitch(pitch))
}


////////////////////////////////////////////////////////////
/// Set the volume of a sound
////////////////////////////////////////////////////////////
void sfSound_SetVolume(sfSound* sound, float volume)
{
    CSFML_CALL(sound, SetVolume(volume))
}


////////////////////////////////////////////////////////////
/// Set the position of a sound
////////////////////////////////////////////////////////////
void sfSound_SetPosition(sfSound* sound, float x, float y, float z)
{
    CSFML_CALL(sound, SetPosition(sf::Vector3f(x, y, z)))
}


////////////////////////////////////////////////////////////
/// Make the sound's position relative to the listener's
/// position, or absolute.
/// The default value is false (absolute)
////////////////////////////////////////////////////////////
void sfSound_SetRelativeToListener(sfSound* sound, sfBool relative)
{
    CSFML_CALL(sound, SetRelativeToListener(relative == sfTrue));
}


////////////////////////////////////////////////////////////
/// Set the minimum distance - closer than this distance,
/// the listener will hear the sound at its maximum volume.
/// The default minimum distance is 1.0
////////////////////////////////////////////////////////////
void sfSound_SetMinDistance(sfSound* sound, float distance)
{
    CSFML_CALL(sound, SetMinDistance(distance));
}


////////////////////////////////////////////////////////////
/// Set the attenuation factor - the higher the attenuation, the
/// more the sound will be attenuated with distance from listener.
/// The default attenuation factor is 1.0
////////////////////////////////////////////////////////////
void sfSound_SetAttenuation(sfSound* sound, float attenuation)
{
    CSFML_CALL(sound, SetAttenuation(attenuation));
}


////////////////////////////////////////////////////////////
/// Set the current playing position of a sound
////////////////////////////////////////////////////////////
void sfSound_SetPlayingOffset(sfSound* sound, float timeOffset)
{
    CSFML_CALL(sound, SetPlayingOffset(timeOffset));
}


////////////////////////////////////////////////////////////
/// Get the pitch of a sound
////////////////////////////////////////////////////////////
float sfSound_GetPitch(const sfSound* sound)
{
    CSFML_CALL_RETURN(sound, GetPitch(), 0.f)
}


////////////////////////////////////////////////////////////
/// Get the volume of a sound
////////////////////////////////////////////////////////////
float sfSound_GetVolume(const sfSound* sound)
{
    CSFML_CALL_RETURN(sound, GetVolume(), 0.f)
}


////////////////////////////////////////////////////////////
/// Get the position of a sound
////////////////////////////////////////////////////////////
void sfSound_GetPosition(const sfSound* sound, float* x, float* y, float* z)
{
    CSFML_CHECK(sound);

    sf::Vector3f position = sound->This.GetPosition();
    if (x) *x = position.x;
    if (y) *y = position.y;
    if (z) *z = position.z;
}


////////////////////////////////////////////////////////////
/// Tell if the sound's position is relative to the listener's
/// position, or if it's absolute
////////////////////////////////////////////////////////////
CSFML_API sfBool sfSound_IsRelativeToListener(const sfSound* sound)
{
    CSFML_CALL_RETURN(sound, IsRelativeToListener(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the minimum distance of a sound
////////////////////////////////////////////////////////////
float sfSound_GetMinDistance(const sfSound* sound)
{
    CSFML_CALL_RETURN(sound, GetMinDistance(), 0.f);
}


////////////////////////////////////////////////////////////
/// Get the attenuation factor of a sound
////////////////////////////////////////////////////////////
float sfSound_GetAttenuation(const sfSound* sound)
{
    CSFML_CALL_RETURN(sound, GetAttenuation(), 0.f);
}


////////////////////////////////////////////////////////////
/// Get the current playing position of a sound
////////////////////////////////////////////////////////////
float sfSound_GetPlayingOffset(const sfSound* sound)
{
    CSFML_CALL_RETURN(sound, GetPlayingOffset(), 0.f)
}
