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
/// \param Filename : Path of the music file to open
///
/// \return A new sfMusic object (NULL if failed)
///
////////////////////////////////////////////////////////////
CSFML_API sfMusic* sfMusic_CreateFromFile(const char* Filename);

////////////////////////////////////////////////////////////
/// Create a new music and load it from a file in memory
///
/// \param Data :        Pointer to the file data in memory
/// \param SizeInBytes : Size of the data to load, in bytes
///
/// \return A new sfMusic object (NULL if failed)
///
////////////////////////////////////////////////////////////
CSFML_API sfMusic* sfMusic_CreateFromMemory(const char* Data, size_t SizeInBytes);

////////////////////////////////////////////////////////////
/// Destroy an existing music
///
/// \param Music : Music to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_Destroy(sfMusic* Music);

////////////////////////////////////////////////////////////
/// Set a music loop state
///
/// \param Music : Music to set the loop state
/// \param Loop :  sfTrue to play in loop, sfFalse to play once
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_SetLoop(sfMusic* Music, sfBool Loop);

////////////////////////////////////////////////////////////
/// Tell whether or not a music is looping
///
/// \param Music : Music to get the loop state from
///
/// \return sfTrue if the music is looping, sfFalse otherwise
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfMusic_GetLoop(sfMusic* Music);

////////////////////////////////////////////////////////////
/// Get a music duration
///
/// \param Music : Music to get the duration from
///
/// \return Music duration, in seconds
///
////////////////////////////////////////////////////////////
CSFML_API float sfMusic_GetDuration(sfMusic* Music);

////////////////////////////////////////////////////////////
/// Start playing a music
///
/// \param Music : Music to play
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_Play(sfMusic* Music);

////////////////////////////////////////////////////////////
/// Pause a music
///
/// \param Music : Music to pause
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_Pause(sfMusic* Music);

////////////////////////////////////////////////////////////
/// Stop playing a music
///
/// \param Music : Music to stop
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_Stop(sfMusic* Music);

////////////////////////////////////////////////////////////
/// Return the number of channels of a music (1 = mono, 2 = stereo)
///
/// \param Music : Music to get the channels count from
///
/// \return Number of channels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfMusic_GetChannelsCount(sfMusic* Music);

////////////////////////////////////////////////////////////
/// Get the stream sample rate of a music
///
/// \param Music : Music to get the sample rate from
///
/// \return Stream frequency (number of samples per second)
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfMusic_GetSampleRate(sfMusic* Music);

////////////////////////////////////////////////////////////
/// Get the status of a music (stopped, paused, playing)
///
/// \param Music : Music to get the status from
///
/// \return Current status of the sound
///
////////////////////////////////////////////////////////////
CSFML_API sfSoundStatus sfMusic_GetStatus(sfMusic* Music);

////////////////////////////////////////////////////////////
/// Get the current playing position of a music
///
/// \param Music : Music to get the position from
///
/// \return Current playing position, expressed in seconds
///
////////////////////////////////////////////////////////////
CSFML_API float sfMusic_GetPlayingOffset(sfMusic* Music);

////////////////////////////////////////////////////////////
/// Set the pitch of a music
///
/// \param Music : Music to modify
/// \param Pitch : New pitch
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_SetPitch(sfMusic* Music, float Pitch);

////////////////////////////////////////////////////////////
/// Set the volume of a music
///
/// \param Music :  Music to modify
/// \param Volume : Volume (in range [0, 100])
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_SetVolume(sfMusic* Music, float Volume);

////////////////////////////////////////////////////////////
/// Set the position of a music
///
/// \param Music : Music to modify
/// \param X : X   position of the sound in the world
/// \param Y : Y   position of the sound in the world
/// \param Z : Z   position of the sound in the world
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_SetPosition(sfMusic* Music, float X, float Y, float Z);

////////////////////////////////////////////////////////////
/// Make the music's position relative to the listener's
/// position, or absolute.
/// The default value is false (absolute)
///
/// \param Music :    Music to modify
/// \param Relative : True to set the position relative, false to set it absolute
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_SetRelativeToListener(sfMusic* Music, sfBool Relative);

////////////////////////////////////////////////////////////
/// Set the minimum distance - closer than this distance,
/// the listener will hear the music at its maximum volume.
/// The default minimum distance is 1.0
///
/// \param Music :       Music to modify
/// \param MinDistance : New minimum distance for the music
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_SetMinDistance(sfMusic* Music, float MinDistance);

////////////////////////////////////////////////////////////
/// Set the attenuation factor - the higher the attenuation, the
/// more the sound will be attenuated with distance from listener.
/// The default attenuation factor 1.0
///
/// \param Sound :       Sound to modify
/// \param Attenuation : New attenuation factor for the sound
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_SetAttenuation(sfMusic* Music, float Attenuation);

////////////////////////////////////////////////////////////
/// Get the pitch of a music
///
/// \param Music : Music to get the pitch from
///
/// \return Pitch value
///
////////////////////////////////////////////////////////////
CSFML_API float sfMusic_GetPitch(sfMusic* Music);

////////////////////////////////////////////////////////////
/// Get the volume of a music
///
/// \param Music : Music to get the volume from
///
/// \return Volume value (in range [1, 100])
///
////////////////////////////////////////////////////////////
CSFML_API float sfMusic_GetVolume(sfMusic* Music);

////////////////////////////////////////////////////////////
/// Get the position of a music
///
/// \param Music : Music to get the position from
/// \param X :     X position of the sound in the world
/// \param Y :     Y position of the sound in the world
/// \param Z :     Z position of the sound in the world
///
////////////////////////////////////////////////////////////
CSFML_API void sfMusic_GetPosition(sfMusic* Music, float* X, float* Y, float* Z);

////////////////////////////////////////////////////////////
/// Tell if the music's position is relative to the listener's
/// position, or if it's absolute
///
/// \param Music : Music to check
///
/// \return sfTrue if the position is relative, sfFalse if it's absolute
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfMusic_IsRelativeToListener(sfMusic* Music);

////////////////////////////////////////////////////////////
/// Get the minimum distance of a music
///
/// \param Music : Music to get the minimum distance from
///
/// \return Minimum distance for the music
///
////////////////////////////////////////////////////////////
CSFML_API float sfMusic_GetMinDistance(sfMusic* Music);

////////////////////////////////////////////////////////////
/// Get the attenuation factor of a music
///
/// \param Music : Music to get the attenuation factor from
///
/// \return Attenuation factor for the a music
///
////////////////////////////////////////////////////////////
CSFML_API float sfMusic_GetAttenuation(sfMusic* Music);


#endif // SFML_MUSIC_H
