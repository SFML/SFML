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

#ifndef SFML_SOUND_H
#define SFML_SOUND_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Audio/SoundStatus.h>
#include <SFML/Audio/Types.h>


////////////////////////////////////////////////////////////
/// Construct a new sound
///
/// \return A new sfSound object (NULL if failed)
///
////////////////////////////////////////////////////////////
CSFML_API sfSound* sfSound_Create();

////////////////////////////////////////////////////////////
/// Destroy an existing sound
///
/// \param Sound : Sound to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_Destroy(sfSound* Sound);

////////////////////////////////////////////////////////////
/// Start playing a sound
///
/// \param Sound : Sound to play
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_Play(sfSound* Sound);

////////////////////////////////////////////////////////////
/// Pause a sound
///
/// \param Sound : Sound to pause
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_Pause(sfSound* Sound);

////////////////////////////////////////////////////////////
/// Stop playing a sound
///
/// \param Sound : Sound to stop
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_Stop(sfSound* Sound);

////////////////////////////////////////////////////////////
/// Bind a sound buffer to a sound
///
/// \param Sound :  Sound to set the loop state
/// \param Buffer : Buffer to bind
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_SetBuffer(sfSound* Sound, sfSoundBuffer* Buffer);

////////////////////////////////////////////////////////////
/// Get the sound buffer bound to a sound
///
/// \param Sound : Sound to get the buffer from
///
/// \return Pointer to the sound's buffer
///
////////////////////////////////////////////////////////////
CSFML_API sfSoundBuffer* sfSound_GetBuffer(sfSound* Sound);

////////////////////////////////////////////////////////////
/// Set a sound loop state
///
/// \param Sound : Sound to set the loop state
/// \param Loop :  sfTrue to play in loop, sfFalse to play once
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_SetLoop(sfSound* Sound, sfBool Loop);

////////////////////////////////////////////////////////////
/// Tell whether or not a sound is looping
///
/// \param Sound : Sound to get the loop state from
///
/// \return sfTrue if the sound is looping, sfFalse otherwise
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfSound_GetLoop(sfSound* Sound);

////////////////////////////////////////////////////////////
/// Get the status of a sound (stopped, paused, playing)
///
/// \param Sound : Sound to get the status from
///
/// \return Current status of the sound
///
////////////////////////////////////////////////////////////
CSFML_API sfSoundStatus sfSound_GetStatus(sfSound* Sound);

////////////////////////////////////////////////////////////
/// Set the pitch of a sound
///
/// \param Sound : Sound to modify
/// \param Pitch : New pitch
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_SetPitch(sfSound* Sound, float Pitch);

////////////////////////////////////////////////////////////
/// Set the volume of a sound
///
/// \param Sound :  Sound to modify
/// \param Volume : Volume (in range [0, 100])
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_SetVolume(sfSound* Sound, float Volume);

////////////////////////////////////////////////////////////
/// Set the position of a sound
///
/// \param Sound : Sound to modify
/// \param X : X   position of the sound in the world
/// \param Y : Y   position of the sound in the world
/// \param Z : Z   position of the sound in the world
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_SetPosition(sfSound* Sound, float X, float Y, float Z);

////////////////////////////////////////////////////////////
/// Make the sound's position relative to the listener's
/// position, or absolute.
/// The default value is false (absolute)
///
/// \param Sound :    Sound to modify
/// \param Relative : True to set the position relative, false to set it absolute
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_SetRelativeToListener(sfSound* Sound, sfBool Relative);

////////////////////////////////////////////////////////////
/// Set the minimum distance - closer than this distance,
/// the listener will hear the sound at its maximum volume.
/// The default minimum distance is 1.0
///
/// \param Sound :       Sound to modify
/// \param MinDistance : New minimum distance for the sound
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_SetMinDistance(sfSound* Sound, float MinDistance);

////////////////////////////////////////////////////////////
/// Set the attenuation factor - the higher the attenuation, the
/// more the sound will be attenuated with distance from listener.
/// The default attenuation factor is 1.0
///
/// \param Sound :       Sound to modify
/// \param Attenuation : New attenuation factor for the sound
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_SetAttenuation(sfSound* Sound, float Attenuation);

////////////////////////////////////////////////////////////
/// Set the current playing position of a sound
///
/// \param Sound :      Sound to modify
/// \param TimeOffset : New playing position, expressed in seconds
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_SetPlayingOffset(sfSound* Sound, float TimeOffset);

////////////////////////////////////////////////////////////
/// Get the pitch of a sound
///
/// \param Sound : Sound to get the pitch from
///
/// \return Pitch value
///
////////////////////////////////////////////////////////////
CSFML_API float sfSound_GetPitch(sfSound* Sound);

////////////////////////////////////////////////////////////
/// Get the volume of a sound
///
/// \param Sound : Sound to get the volume from
///
/// \return Volume value (in range [1, 100])
///
////////////////////////////////////////////////////////////
CSFML_API float sfSound_GetVolume(sfSound* Sound);

////////////////////////////////////////////////////////////
/// Get the position of a sound
///
/// \param Sound : Sound to get the position from
/// \param X :     X position of the sound in the world
/// \param Y :     Y position of the sound in the world
/// \param Z :     Z position of the sound in the world
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_GetPosition(sfSound* Sound, float* X, float* Y, float* Z);

////////////////////////////////////////////////////////////
/// Tell if the sound's position is relative to the listener's
/// position, or if it's absolute
///
/// \param Sound : Sound to check
///
/// \return sfTrue if the position is relative, sfFalse if it's absolute
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfSound_IsRelativeToListener(sfSound* Sound);

////////////////////////////////////////////////////////////
/// Get the minimum distance of a sound
///
/// \param Sound : Sound to get the minimum distance from
///
/// \return Minimum distance for the sound
///
////////////////////////////////////////////////////////////
CSFML_API float sfSound_GetMinDistance(sfSound* Sound);

////////////////////////////////////////////////////////////
/// Get the attenuation factor of a sound
///
/// \param Sound : Sound to get the attenuation factor from
///
/// \return Attenuation factor for the sound
///
////////////////////////////////////////////////////////////
CSFML_API float sfSound_GetAttenuation(sfSound* Sound);

////////////////////////////////////////////////////////////
/// Get the current playing position of a sound
///
/// \param Sound : Sound to get the position from
///
/// \return Current playing position, expressed in seconds
///
////////////////////////////////////////////////////////////
CSFML_API float sfSound_GetPlayingOffset(sfSound* Sound);


#endif // SFML_SOUND_H
