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

#ifndef SFML_SOUNDSTREAM_H
#define SFML_SOUNDSTREAM_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Audio/SoundStatus.h>
#include <SFML/Audio/Types.h>


////////////////////////////////////////////////////////////
/// sfSoundStreamChunk defines the data to fill by the
/// OnGetData callback
////////////////////////////////////////////////////////////
typedef struct
{
    sfInt16*     Samples;   ///< Pointer to the audio samples
    unsigned int NbSamples; ///< Number of samples pointed by Samples
} sfSoundStreamChunk;

typedef sfBool (*sfSoundStreamGetDataCallback)(sfSoundStreamChunk*, void*); ///< Type of the callback used to get a sound stream data
typedef void   (*sfSoundStreamSeekCallback)(float, void*);                  ///< Type of the callback used to seek in a sound stream


////////////////////////////////////////////////////////////
/// Construct a new sound stream
///
/// \param onGetData :     Function called when the stream needs more data (can't be NULL)
/// \param onSeek :        Function called when the stream seeks (can't be NULL)
/// \param channelsCount : Number of channels to use (1 = mono, 2 = stereo)
/// \param sampleRate :    Sample rate of the sound (44100 = CD quality)
/// \param userData :      Data to pass to the callback functions
///
/// \return A new sfSoundStream object (NULL if failed)
///
////////////////////////////////////////////////////////////
CSFML_API sfSoundStream* sfSoundStream_Create(sfSoundStreamGetDataCallback onGetData,
                                              sfSoundStreamSeekCallback    onSeek,
                                              unsigned int                 channelsCount,
                                              unsigned int                 sampleRate,
                                              void*                        userData);

////////////////////////////////////////////////////////////
/// Destroy an existing sound stream
///
/// \param soundStream : Sound stream to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_Destroy(sfSoundStream* soundStream);

////////////////////////////////////////////////////////////
/// Start playing a sound stream
///
/// \param soundStream : Sound stream to play
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_Play(sfSoundStream* soundStream);

////////////////////////////////////////////////////////////
/// Pause a sound stream
///
/// \param soundStream : Sound stream to pause
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_Pause(sfSoundStream* soundStream);

////////////////////////////////////////////////////////////
/// Stop playing a sound stream
///
/// \param soundStream : Sound stream to stop
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_Stop(sfSoundStream* soundStream);

////////////////////////////////////////////////////////////
/// Get the status of a sound stream (stopped, paused, playing)
///
/// \param soundStream : Sound stream to get the status from
///
/// \return Current status of the sound stream
///
////////////////////////////////////////////////////////////
CSFML_API sfSoundStatus sfSoundStream_GetStatus(const sfSoundStream* soundStream);

////////////////////////////////////////////////////////////
/// Return the number of channels of a sound stream
/// (1 = mono, 2 = stereo)
///
/// \param soundStream : Sound stream to get the channels count from
///
/// \return Number of channels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfSoundStream_GetChannelsCount(const sfSoundStream* soundStream);

////////////////////////////////////////////////////////////
/// Get the sample rate of a sound stream
///
/// \param soundStream : Sound stream to get the sample rate from
///
/// \return Stream frequency (number of samples per second)
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfSoundStream_GetSampleRate(const sfSoundStream* soundStream);

////////////////////////////////////////////////////////////
/// Set the pitch of a sound stream
///
/// \param soundStream : Sound stream to modify
/// \param pitch :       New pitch
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_SetPitch(sfSoundStream* soundStream, float pitch);

////////////////////////////////////////////////////////////
/// Set the volume of a sound stream
///
/// \param soundStream : Sound stream to modify
/// \param volume :      Volume (in range [0, 100])
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_SetVolume(sfSoundStream* soundStream, float volume);

////////////////////////////////////////////////////////////
/// Set the position of a sound stream
///
/// \param soundStream : Sound stream to modify
/// \param x :           X position of the sound stream in the world
/// \param y :           Y position of the sound stream in the world
/// \param z :           Z position of the sound stream in the world
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_SetPosition(sfSoundStream* soundStream, float x, float y, float z);

////////////////////////////////////////////////////////////
/// Make the sound stream's position relative to the listener's
/// position, or absolute.
/// The default value is false (absolute)
///
/// \param soundStream : Sound stream to modify
/// \param relative :    True to set the position relative, false to set it absolute
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_SetRelativeToListener(sfSoundStream* soundStream, sfBool relative);

////////////////////////////////////////////////////////////
/// Set the minimum distance - closer than this distance,
/// the listener will hear the sound stream at its maximum volume.
/// The default minimum distance is 1.0
///
/// \param soundStream : Sound stream to modify
/// \param distance :    New minimum distance for the sound stream
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_SetMinDistance(sfSoundStream* soundStream, float distance);

////////////////////////////////////////////////////////////
/// Set the attenuation factor - the higher the attenuation, the
/// more the sound stream will be attenuated with distance from listener.
/// The default attenuation factor 1.0
///
/// \param soundStream : Sound stream to modify
/// \param attenuation : New attenuation factor for the sound stream
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_SetAttenuation(sfSoundStream* soundStream, float attenuation);

////////////////////////////////////////////////////////////
/// Set the current playing position of a stream
///
/// \param soundStream : Sound stream to modify
/// \param timeOffset :  New playing position, expressed in seconds
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_SetPlayingOffset(sfSoundStream* soundStream, float timeOffset);

////////////////////////////////////////////////////////////
/// Set a stream loop state
///
/// \param soundStream : Stream to set the loop state
/// \param loop :        sfTrue to play in loop, sfFalse to play once
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_SetLoop(sfSoundStream* soundStream, sfBool loop);

////////////////////////////////////////////////////////////
/// Get the pitch of a sound stream
///
/// \param soundStream : Sound stream to get the pitch from
///
/// \return Pitch value
///
////////////////////////////////////////////////////////////
CSFML_API float sfSoundStream_GetPitch(const sfSoundStream* soundStream);

////////////////////////////////////////////////////////////
/// Get the volume of a sound stream
///
/// \param soundStream : Sound stream to get the volume from
///
/// \return Volume value (in range [1, 100])
///
////////////////////////////////////////////////////////////
CSFML_API float sfSoundStream_GetVolume(const sfSoundStream* soundStream);

////////////////////////////////////////////////////////////
/// Get the position of a sound stream
///
/// \param soundStream : Sound stream to get the position from
/// \param x :           X position of the sound stream in the world
/// \param y :           Y position of the sound stream in the world
/// \param z :           Z position of the sound stream in the world
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_GetPosition(const sfSoundStream* soundStream, float* x, float* y, float* z);

////////////////////////////////////////////////////////////
/// Tell if the sound stream's position is relative to the listener's
/// position, or if it's absolute
///
/// \param soundStream : Sound stream to check
///
/// \return sfTrue if the position is relative, sfFalse if it's absolute
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfSoundStream_IsRelativeToListener(const sfSoundStream* soundStream);

////////////////////////////////////////////////////////////
/// Get the minimum distance of a sound stream
///
/// \param soundStream : Sound stream to get the minimum distance from
///
/// \return Minimum distance for the sound stream
///
////////////////////////////////////////////////////////////
CSFML_API float sfSoundStream_GetMinDistance(const sfSoundStream* soundStream);

////////////////////////////////////////////////////////////
/// Get the attenuation factor of a sound stream
///
/// \param soundStream : Sound stream to get the attenuation factor from
///
/// \return Attenuation factor for the sound stream
///
////////////////////////////////////////////////////////////
CSFML_API float sfSoundStream_GetAttenuation(const sfSoundStream* soundStream);

////////////////////////////////////////////////////////////
/// Tell whether or not a stream is looping
///
/// \param soundStream : Soundstream to get the loop state from
///
/// \return sfTrue if the stream is looping, sfFalse otherwise
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfSoundStream_GetLoop(const sfSoundStream* soundStream);

////////////////////////////////////////////////////////////
/// Get the current playing position of a sound stream
///
/// \param soundStream : Sound stream to get the position from
///
/// \return Current playing position, expressed in seconds
///
////////////////////////////////////////////////////////////
CSFML_API float sfSoundStream_GetPlayingOffset(const sfSoundStream* soundStream);


#endif // SFML_SOUNDSTREAM_H
