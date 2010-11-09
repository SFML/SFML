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

#ifndef SFML_MUSIC_H
#define SFML_MUSIC_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Audio/SoundStatus.h>
#include <SFML/Audio/Types.h>


////////////////////////////////////////////////////////////
/// Create a new music and load it from a file
///
/// \param filename : Path of the music file to open
///
/// \return A new sfMusic object (NULL if failed)
///
////////////////////////////////////////////////////////////
CSFML_API sfMusic* sfMusic_CreateFromFile(const char* filename);

////////////////////////////////////////////////////////////
/// Create a new music and load it from a file in memory
///
/// \param data :        Pointer to the file data in memory
/// \param sizeInBytes : Size of the data to load, in bytes
///
/// \return A new sfMusic object (NULL if failed)
///
////////////////////////////////////////////////////////////
CSFML_API sfMusic* sfMusic_CreateFromMemory(const void* data, size_t sizeInBytes);

////////////////////////////////////////////////////////////
/// Destroy an existing music
///
/// \param music : Music to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_Destroy(sfMusic* music);

////////////////////////////////////////////////////////////
/// Set a music loop state
///
/// \param music : Music to set the loop state
/// \param loop :  sfTrue to play in loop, sfFalse to play once
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_SetLoop(sfMusic* music, sfBool loop);

////////////////////////////////////////////////////////////
/// Tell whether or not a music is looping
///
/// \param music : Music to get the loop state from
///
/// \return sfTrue if the music is looping, sfFalse otherwise
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfMusic_GetLoop(const sfMusic* music);

////////////////////////////////////////////////////////////
/// Get a music duration
///
/// \param music : Music to get the duration from
///
/// \return Music duration, in seconds
///
////////////////////////////////////////////////////////////
CSFML_API float sfMusic_GetDuration(const sfMusic* music);

////////////////////////////////////////////////////////////
/// Start playing a music
///
/// \param music : Music to play
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_Play(sfMusic* music);

////////////////////////////////////////////////////////////
/// Pause a music
///
/// \param music : Music to pause
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_Pause(sfMusic* music);

////////////////////////////////////////////////////////////
/// Stop playing a music
///
/// \param music : Music to stop
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_Stop(sfMusic* music);

////////////////////////////////////////////////////////////
/// Return the number of channels of a music (1 = mono, 2 = stereo)
///
/// \param music : Music to get the channels count from
///
/// \return Number of channels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfMusic_GetChannelsCount(const sfMusic* music);

////////////////////////////////////////////////////////////
/// Get the stream sample rate of a music
///
/// \param music : Music to get the sample rate from
///
/// \return Stream frequency (number of samples per second)
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfMusic_GetSampleRate(const sfMusic* music);

////////////////////////////////////////////////////////////
/// Get the status of a music (stopped, paused, playing)
///
/// \param music : Music to get the status from
///
/// \return Current status of the sound
///
////////////////////////////////////////////////////////////
CSFML_API sfSoundStatus sfMusic_GetStatus(const sfMusic* music);

////////////////////////////////////////////////////////////
/// Get the current playing position of a music
///
/// \param music : Music to get the position from
///
/// \return Current playing position, expressed in seconds
///
////////////////////////////////////////////////////////////
CSFML_API float sfMusic_GetPlayingOffset(const sfMusic* music);

////////////////////////////////////////////////////////////
/// Set the pitch of a music
///
/// \param music : Music to modify
/// \param pitch : New pitch
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_SetPitch(sfMusic* music, float pitch);

////////////////////////////////////////////////////////////
/// Set the volume of a music
///
/// \param music :  Music to modify
/// \param volume : Volume (in range [0, 100])
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_SetVolume(sfMusic* music, float volume);

////////////////////////////////////////////////////////////
/// Set the position of a music
///
/// \param music : Music to modify
/// \param x :     X position of the sound in the world
/// \param y :     Y position of the sound in the world
/// \param z :     Z position of the sound in the world
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_SetPosition(sfMusic* music, float x, float y, float z);

////////////////////////////////////////////////////////////
/// Make the music's position relative to the listener's
/// position, or absolute.
/// The default value is false (absolute)
///
/// \param music :    Music to modify
/// \param relative : True to set the position relative, false to set it absolute
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_SetRelativeToListener(sfMusic* music, sfBool relative);

////////////////////////////////////////////////////////////
/// Set the minimum distance - closer than this distance,
/// the listener will hear the music at its maximum volume.
/// The default minimum distance is 1.0
///
/// \param music :    Music to modify
/// \param distance : New minimum distance for the music
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_SetMinDistance(sfMusic* music, float distance);

////////////////////////////////////////////////////////////
/// Set the attenuation factor - the higher the attenuation, the
/// more the sound will be attenuated with distance from listener.
/// The default attenuation factor 1.0
///
/// \param music :       Music to modify
/// \param attenuation : New attenuation factor for the sound
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_SetAttenuation(sfMusic* music, float attenuation);

////////////////////////////////////////////////////////////
/// Set the current playing position of a music
///
/// \param music :      Music to modify
/// \param timeOffset : New playing position, expressed in seconds
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_SetPlayingOffset(sfMusic* music, float timeOffset);

////////////////////////////////////////////////////////////
/// Get the pitch of a music
///
/// \param music : Music to get the pitch from
///
/// \return Pitch value
///
////////////////////////////////////////////////////////////
CSFML_API float sfMusic_GetPitch(const sfMusic* music);

////////////////////////////////////////////////////////////
/// Get the volume of a music
///
/// \param music : Music to get the volume from
///
/// \return Volume value (in range [1, 100])
///
////////////////////////////////////////////////////////////
CSFML_API float sfMusic_GetVolume(const sfMusic* music);

////////////////////////////////////////////////////////////
/// Get the position of a music
///
/// \param music : Music to get the position from
/// \param x :     X position of the sound in the world
/// \param y :     Y position of the sound in the world
/// \param z :     Z position of the sound in the world
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_GetPosition(const sfMusic* music, float* x, float* y, float* z);

////////////////////////////////////////////////////////////
/// Tell if the music's position is relative to the listener's
/// position, or if it's absolute
///
/// \param music : Music to check
///
/// \return sfTrue if the position is relative, sfFalse if it's absolute
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfMusic_IsRelativeToListener(const sfMusic* music);

////////////////////////////////////////////////////////////
/// Get the minimum distance of a music
///
/// \param music : Music to get the minimum distance from
///
/// \return Minimum distance for the music
///
////////////////////////////////////////////////////////////
CSFML_API float sfMusic_GetMinDistance(const sfMusic* music);

////////////////////////////////////////////////////////////
/// Get the attenuation factor of a music
///
/// \param music : Music to get the attenuation factor from
///
/// \return Attenuation factor for the a music
///
////////////////////////////////////////////////////////////
CSFML_API float sfMusic_GetAttenuation(const sfMusic* music);


#endif // SFML_MUSIC_H
