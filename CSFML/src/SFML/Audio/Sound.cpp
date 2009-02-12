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
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Internal.h>


// WARNING : this structure must always be the SAME as in Audio/SoundBuffer.h
struct sfSoundBuffer
{
    sf::SoundBuffer This;
};
struct sfSound
{
    sf::Sound      This;
    sfSoundBuffer* Buffer;
};


////////////////////////////////////////////////////////////
/// Construct a new sound
////////////////////////////////////////////////////////////
sfSound* sfSound_Create()
{
    return new sfSound;
}


////////////////////////////////////////////////////////////
/// Destroy an existing sound
////////////////////////////////////////////////////////////
void sfSound_Destroy(sfSound* Sound)
{
    delete Sound;
}


////////////////////////////////////////////////////////////
/// Start playing a sound
////////////////////////////////////////////////////////////
void sfSound_Play(sfSound* Sound)
{
    CSFML_CALL(Sound, Play())
}


////////////////////////////////////////////////////////////
/// Pause a sound
////////////////////////////////////////////////////////////
void sfSound_Pause(sfSound* Sound)
{
    CSFML_CALL(Sound, Pause())
}


////////////////////////////////////////////////////////////
/// Stop playing a sound
////////////////////////////////////////////////////////////
void sfSound_Stop(sfSound* Sound)
{
    CSFML_CALL(Sound, Stop())
}


////////////////////////////////////////////////////////////
/// Bind a sound buffer to a sound
////////////////////////////////////////////////////////////
void sfSound_SetBuffer(sfSound* Sound, sfSoundBuffer* Buffer)
{
    if (Buffer)
    {
        CSFML_CALL(Sound, SetBuffer(Buffer->This))
        Sound->Buffer = Buffer;
    }
}


////////////////////////////////////////////////////////////
/// Get the sound buffer bound to a sound
////////////////////////////////////////////////////////////
sfSoundBuffer* sfSound_GetBuffer(sfSound* Sound)
{
    CSFML_CHECK_RETURN(Sound, NULL)

    return Sound->Buffer;
}


////////////////////////////////////////////////////////////
/// Set a sound loop state
////////////////////////////////////////////////////////////
void sfSound_SetLoop(sfSound* Sound, sfBool Loop)
{
    CSFML_CALL(Sound, SetLoop(Loop == sfTrue))
}


////////////////////////////////////////////////////////////
/// Tell whether or not a sound is looping
////////////////////////////////////////////////////////////
sfBool sfSound_GetLoop(sfSound* Sound)
{
    CSFML_CALL_RETURN(Sound, GetLoop(), sfFalse)
}


////////////////////////////////////////////////////////////
/// Get the status of a sound (stopped, paused, playing)
////////////////////////////////////////////////////////////
sfSoundStatus sfSound_GetStatus(sfSound* Sound)
{
    CSFML_CHECK_RETURN(Sound, sfStopped);

    return static_cast<sfSoundStatus>(Sound->This.GetStatus());
}


////////////////////////////////////////////////////////////
/// Set the pitch of a sound
////////////////////////////////////////////////////////////
void sfSound_SetPitch(sfSound* Sound, float Pitch)
{
    CSFML_CALL(Sound, SetPitch(Pitch))
}


////////////////////////////////////////////////////////////
/// Set the volume of a sound
////////////////////////////////////////////////////////////
void sfSound_SetVolume(sfSound* Sound, float Volume)
{
    CSFML_CALL(Sound, SetVolume(Volume))
}


////////////////////////////////////////////////////////////
/// Set the position of a sound
////////////////////////////////////////////////////////////
void sfSound_SetPosition(sfSound* Sound, float X, float Y, float Z)
{
    CSFML_CALL(Sound, SetPosition(sf::Vector3f(X, Y, Z)))
}


////////////////////////////////////////////////////////////
/// Set the minimum distance - closer than this distance,
/// the listener will hear the sound at its maximum volume.
/// The default minimum distance is 1.0
////////////////////////////////////////////////////////////
void sfSound_SetMinDistance(sfSound* Sound, float MinDistance)
{
    CSFML_CALL(Sound, SetMinDistance(MinDistance));
}


////////////////////////////////////////////////////////////
/// Set the attenuation factor - the higher the attenuation, the
/// more the sound will be attenuated with distance from listener.
/// The default attenuation factor is 1.0
////////////////////////////////////////////////////////////
void sfSound_SetAttenuation(sfSound* Sound, float Attenuation)
{
    CSFML_CALL(Sound, SetAttenuation(Attenuation));
}


////////////////////////////////////////////////////////////
/// Set the current playing position of a sound
////////////////////////////////////////////////////////////
void sfSound_SetPlayingOffset(sfSound* Sound, float TimeOffset)
{
    CSFML_CALL(Sound, SetPlayingOffset(TimeOffset));
}


////////////////////////////////////////////////////////////
/// Get the pitch of a sound
////////////////////////////////////////////////////////////
float sfSound_GetPitch(sfSound* Sound)
{
    CSFML_CALL_RETURN(Sound, GetPitch(), 0.f)
}


////////////////////////////////////////////////////////////
/// Get the volume of a sound
////////////////////////////////////////////////////////////
float sfSound_GetVolume(sfSound* Sound)
{
    CSFML_CALL_RETURN(Sound, GetVolume(), 0.f)
}


////////////////////////////////////////////////////////////
/// Get the position of a sound
////////////////////////////////////////////////////////////
void sfSound_GetPosition(sfSound* Sound, float* X, float* Y, float* Z)
{
    CSFML_CHECK(Sound);

    sf::Vector3f Position = Sound->This.GetPosition();
    if (X) *X = Position.x;
    if (Y) *Y = Position.y;
    if (Z) *Z = Position.z;
}


////////////////////////////////////////////////////////////
/// Get the minimum distance of a sound
////////////////////////////////////////////////////////////
float sfSound_GetMinDistance(sfSound* Sound)
{
    CSFML_CALL_RETURN(Sound, GetMinDistance(), 0.f);
}


////////////////////////////////////////////////////////////
/// Get the attenuation factor of a sound
////////////////////////////////////////////////////////////
float sfSound_GetAttenuation(sfSound* Sound)
{
    CSFML_CALL_RETURN(Sound, GetAttenuation(), 0.f);
}


////////////////////////////////////////////////////////////
/// Get the current playing position of a sound
////////////////////////////////////////////////////////////
float sfSound_GetPlayingOffset(sfSound* Sound)
{
    CSFML_CALL_RETURN(Sound, GetPlayingOffset(), 0.f)
}
