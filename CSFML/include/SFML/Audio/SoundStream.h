////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2008 Laurent Gomila (laurent.gom@gmail.com)
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


////////////////////////////////////////////////////////////
/// sfSoundStream is a streamed sound, ie samples are acquired
/// while the sound is playing. Use it for big sound that would
/// require hundreds of MB in memory (see sfMusic),
/// or for streaming sound from the network
////////////////////////////////////////////////////////////
typedef struct sfSoundStream sfSoundStream;

////////////////////////////////////////////////////////////
/// sfSoundStreamChunk defines the data to fill by the
/// OnGetData callback
////////////////////////////////////////////////////////////
typedef struct
{
    sfInt16*     Samples;   ///< Pointer to the audio samples
    unsigned int NbSamples; ///< Number of samples pointed by Samples
} sfSoundStreamChunk;

typedef sfBool (*sfSoundStreamStartCallback)(void*);                        ///< Type of the callback used to start a sound stream
typedef sfBool (*sfSoundStreamGetDataCallback)(sfSoundStreamChunk*, void*); ///< Type of the callback used to get a sound stream data


////////////////////////////////////////////////////////////
/// Construct a new sound stream
///
/// \param OnStart :       Function called when the stream starts (can be NULL)
/// \param OnGetData :     Function called when the stream needs more data (can't be NULL)
/// \param ChannelsCount : Number of channels to use (1 = mono, 2 = stereo)
/// \param SampleRate :    Sample rate of the sound (44100 = CD quality)
/// \param UserData :      Data to pass to the callback functions
///
/// \return A new sfSoundStream object (NULL if failed)
///
////////////////////////////////////////////////////////////
CSFML_API sfSoundStream* sfSoundStream_Create(sfSoundStreamStartCallback   OnStart,
                                              sfSoundStreamGetDataCallback OnGetData,
                                              unsigned int                 ChannelsCount,
                                              unsigned int                 SampleRate,
                                              void*                        UserData);

////////////////////////////////////////////////////////////
/// Destroy an existing sound stream
///
/// \param SoundStream : Sound stream to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_Destroy(sfSoundStream* SoundStreamStream);

////////////////////////////////////////////////////////////
/// Start playing a sound stream
///
/// \param SoundStream : Sound stream to play
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_Play(sfSoundStream* SoundStream);

////////////////////////////////////////////////////////////
/// Pause a sound stream
///
/// \param SoundStream : Sound stream to pause
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_Pause(sfSoundStream* SoundStream);

////////////////////////////////////////////////////////////
/// Stop playing a sound stream
///
/// \param SoundStream : Sound stream to stop
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_Stop(sfSoundStream* SoundStream);

////////////////////////////////////////////////////////////
/// Get the status of a sound stream (stopped, paused, playing)
///
/// \param SoundStream : Sound stream to get the status from
///
/// \return Current status of the sound stream
///
////////////////////////////////////////////////////////////
CSFML_API sfSoundStatus sfSoundStream_GetStatus(sfSoundStream* SoundStream);

////////////////////////////////////////////////////////////
/// Return the number of channels of a sound stream
/// (1 = mono, 2 = stereo)
///
/// \param SoundStream : Sound stream to get the channels count from
///
/// \return Number of channels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfSoundStream_GetChannelsCount(sfSoundStream* SoundStream);

////////////////////////////////////////////////////////////
/// Get the sample rate of a sound stream
///
/// \param SoundStream : Sound stream to get the sample rate from
///
/// \return Stream frequency (number of samples per second)
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfSoundStream_GetSampleRate(sfSoundStream* SoundStream);

////////////////////////////////////////////////////////////
/// Set the pitch of a sound stream
///
/// \param SoundStream : Sound stream to modify
/// \param Pitch :       New pitch
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_SetPitch(sfSoundStream* SoundStream, float Pitch);

////////////////////////////////////////////////////////////
/// Set the volume of a sound stream
///
/// \param SoundStream : Sound stream to modify
/// \param Volume :      Volume (in range [0, 100])
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_SetVolume(sfSoundStream* SoundStream, float Volume);

////////////////////////////////////////////////////////////
/// Set the position of a sound stream
///
/// \param SoundStream : Sound stream to modify
/// \param X :           X position of the sound stream in the world
/// \param Y :           Y position of the sound stream in the world
/// \param Z :           Z position of the sound stream in the world
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_SetPosition(sfSoundStream* SoundStream, float X, float Y, float Z);

////////////////////////////////////////////////////////////
/// Set the minimum distance - closer than this distance,
/// the listener will hear the sound stream at its maximum volume.
/// The default minimum distance is 1.0
///
/// \param SoundStream : Sound stream to modify
/// \param MinDistance : New minimum distance for the sound stream
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_SetMinDistance(sfSoundStream* SoundStream, float MinDistance);

////////////////////////////////////////////////////////////
/// Set the attenuation factor - the higher the attenuation, the
/// more the sound stream will be attenuated with distance from listener.
/// The default attenuation factor 1.0
///
/// \param SoundStream : Sound stream to modify
/// \param Attenuation : New attenuation factor for the sound stream
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_SetAttenuation(sfSoundStream* SoundStream, float Attenuation);

////////////////////////////////////////////////////////////
/// Set a stream loop state
///
/// \param SoundStream : Stream to set the loop state
/// \param Loop :        sfTrue to play in loop, sfFalse to play once
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_SetLoop(sfSoundStream* SoundStream, sfBool Loop);

////////////////////////////////////////////////////////////
/// Get the pitch of a sound stream
///
/// \param SoundStream : Sound stream to get the pitch from
///
/// \return Pitch value
///
////////////////////////////////////////////////////////////
CSFML_API float sfSoundStream_GetPitch(sfSoundStream* SoundStream);

////////////////////////////////////////////////////////////
/// Get the volume of a sound stream
///
/// \param SoundStream : Sound stream to get the volume from
///
/// \return Volume value (in range [1, 100])
///
////////////////////////////////////////////////////////////
CSFML_API float sfSoundStream_GetVolume(sfSoundStream* SoundStream);

////////////////////////////////////////////////////////////
/// Get the position of a sound stream
///
/// \param SoundStream : Sound stream to get the position from
/// \param X :           X position of the sound stream in the world
/// \param Y :           Y position of the sound stream in the world
/// \param Z :           Z position of the sound stream in the world
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundStream_GetPosition(sfSoundStream* SoundStream, float* X, float* Y, float* Z);

////////////////////////////////////////////////////////////
/// Get the minimum distance of a sound stream
///
/// \param SoundStream : Sound stream to get the minimum distance from
///
/// \return Minimum distance for the sound stream
///
////////////////////////////////////////////////////////////
CSFML_API float sfSoundStream_GetMinDistance(sfSoundStream* SoundStream);

////////////////////////////////////////////////////////////
/// Get the attenuation factor of a sound stream
///
/// \param SoundStream : Sound stream to get the attenuation factor from
///
/// \return Attenuation factor for the sound stream
///
////////////////////////////////////////////////////////////
CSFML_API float sfSoundStream_GetAttenuation(sfSoundStream* SoundStream);

////////////////////////////////////////////////////////////
/// Tell whether or not a stream is looping
///
/// \param SoundStream : Soundstream to get the loop state from
///
/// \return sfTrue if the stream is looping, sfFalse otherwise
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfSoundStream_GetLoop(sfSoundStream* SoundStream);

////////////////////////////////////////////////////////////
/// Get the current playing position of a sound stream
///
/// \param SoundStream : Sound stream to get the position from
///
/// \return Current playing position, expressed in seconds
///
////////////////////////////////////////////////////////////
CSFML_API float sfSoundStream_GetPlayingOffset(sfSoundStream* SoundStream);


#endif // SFML_SOUNDSTREAM_H
