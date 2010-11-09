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
CSFML_API sfSound* sfSound_Create(void);

////////////////////////////////////////////////////////////
/// Copy an existing sound
///
/// \param sound : Sound to copy
///
/// \return Copied object
///
////////////////////////////////////////////////////////////
CSFML_API sfSound* sfSound_Copy(sfSound* sound);

////////////////////////////////////////////////////////////
/// Destroy an existing sound
///
/// \param sound : Sound to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_Destroy(sfSound* sound);

////////////////////////////////////////////////////////////
/// Start playing a sound
///
/// \param sound : Sound to play
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_Play(sfSound* sound);

////////////////////////////////////////////////////////////
/// Pause a sound
///
/// \param sound : Sound to pause
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_Pause(sfSound* sound);

////////////////////////////////////////////////////////////
/// Stop playing a sound
///
/// \param sound : Sound to stop
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_Stop(sfSound* sound);

////////////////////////////////////////////////////////////
/// Bind a sound buffer to a sound
///
/// \param sound :  Sound to set the loop state
/// \param buffer : Buffer to bind
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_SetBuffer(sfSound* sound, const sfSoundBuffer* buffer);

////////////////////////////////////////////////////////////
/// Get the sound buffer bound to a sound
///
/// \param sound : Sound to get the buffer from
///
/// \return Pointer to the sound's buffer
///
////////////////////////////////////////////////////////////
CSFML_API const sfSoundBuffer* sfSound_GetBuffer(const sfSound* sound);

////////////////////////////////////////////////////////////
/// Set a sound loop state
///
/// \param sound : Sound to set the loop state
/// \param loop :  sfTrue to play in loop, sfFalse to play once
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_SetLoop(sfSound* sound, sfBool loop);

////////////////////////////////////////////////////////////
/// Tell whether or not a sound is looping
///
/// \param sound : Sound to get the loop state from
///
/// \return sfTrue if the sound is looping, sfFalse otherwise
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfSound_GetLoop(const sfSound* sound);

////////////////////////////////////////////////////////////
/// Get the status of a sound (stopped, paused, playing)
///
/// \param sound : Sound to get the status from
///
/// \return Current status of the sound
///
////////////////////////////////////////////////////////////
CSFML_API sfSoundStatus sfSound_GetStatus(const sfSound* sound);

////////////////////////////////////////////////////////////
/// Set the pitch of a sound
///
/// \param sound : Sound to modify
/// \param pitch : New pitch
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_SetPitch(sfSound* sound, float pitch);

////////////////////////////////////////////////////////////
/// Set the volume of a sound
///
/// \param sound :  Sound to modify
/// \param volume : Volume (in range [0, 100])
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_SetVolume(sfSound* sound, float volume);

////////////////////////////////////////////////////////////
/// Set the position of a sound
///
/// \param sound : Sound to modify
/// \param x :     X position of the sound in the world
/// \param y :     Y position of the sound in the world
/// \param z :     Z position of the sound in the world
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_SetPosition(sfSound* sound, float x, float y, float z);

////////////////////////////////////////////////////////////
/// Make the sound's position relative to the listener's
/// position, or absolute.
/// The default value is false (absolute)
///
/// \param sound :    Sound to modify
/// \param relative : True to set the position relative, false to set it absolute
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_SetRelativeToListener(sfSound* sound, sfBool relative);

////////////////////////////////////////////////////////////
/// Set the minimum distance - closer than this distance,
/// the listener will hear the sound at its maximum volume.
/// The default minimum distance is 1.0
///
/// \param sound :    Sound to modify
/// \param distance : New minimum distance for the sound
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_SetMinDistance(sfSound* sound, float distance);

////////////////////////////////////////////////////////////
/// Set the attenuation factor - the higher the attenuation, the
/// more the sound will be attenuated with distance from listener.
/// The default attenuation factor is 1.0
///
/// \param sound :       Sound to modify
/// \param attenuation : New attenuation factor for the sound
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_SetAttenuation(sfSound* sound, float attenuation);

////////////////////////////////////////////////////////////
/// Set the current playing position of a sound
///
/// \param sound :      Sound to modify
/// \param timeOffset : New playing position, expressed in seconds
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_SetPlayingOffset(sfSound* sound, float timeOffset);

////////////////////////////////////////////////////////////
/// Get the pitch of a sound
///
/// \param sound : Sound to get the pitch from
///
/// \return Pitch value
///
////////////////////////////////////////////////////////////
CSFML_API float sfSound_GetPitch(const sfSound* sound);

////////////////////////////////////////////////////////////
/// Get the volume of a sound
///
/// \param sound : Sound to get the volume from
///
/// \return Volume value (in range [1, 100])
///
////////////////////////////////////////////////////////////
CSFML_API float sfSound_GetVolume(const sfSound* sound);

////////////////////////////////////////////////////////////
/// Get the position of a sound
///
/// \param sound : Sound to get the position from
/// \param x :     X position of the sound in the world
/// \param y :     Y position of the sound in the world
/// \param z :     Z position of the sound in the world
///
////////////////////////////////////////////////////////////
CSFML_API void sfSound_GetPosition(const sfSound* sound, float* x, float* y, float* z);

////////////////////////////////////////////////////////////
/// Tell if the sound's position is relative to the listener's
/// position, or if it's absolute
///
/// \param sound : Sound to check
///
/// \return sfTrue if the position is relative, sfFalse if it's absolute
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfSound_IsRelativeToListener(const sfSound* sound);

////////////////////////////////////////////////////////////
/// Get the minimum distance of a sound
///
/// \param sound : Sound to get the minimum distance from
///
/// \return Minimum distance for the sound
///
////////////////////////////////////////////////////////////
CSFML_API float sfSound_GetMinDistance(const sfSound* sound);

////////////////////////////////////////////////////////////
/// Get the attenuation factor of a sound
///
/// \param sound : Sound to get the attenuation factor from
///
/// \return Attenuation factor for the sound
///
////////////////////////////////////////////////////////////
CSFML_API float sfSound_GetAttenuation(const sfSound* sound);

////////////////////////////////////////////////////////////
/// Get the current playing position of a sound
///
/// \param sound : Sound to get the position from
///
/// \return Current playing position, expressed in seconds
///
////////////////////////////////////////////////////////////
CSFML_API float sfSound_GetPlayingOffset(const sfSound* sound);


#endif // SFML_SOUND_H
