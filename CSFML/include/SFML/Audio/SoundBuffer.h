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

#ifndef SFML_SOUNDBUFFER_H
#define SFML_SOUNDBUFFER_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>


////////////////////////////////////////////////////////////
/// sfSoundBuffer is the low-level for loading and manipulating
/// sound buffers
////////////////////////////////////////////////////////////
typedef struct sfSoundBuffer sfSoundBuffer;


////////////////////////////////////////////////////////////
/// Create a new sound buffer and load it from a file
///
/// \param Filename : Path of the music file to open
///
/// \return A new sfSoundBuffer object (NULL if failed)
///
////////////////////////////////////////////////////////////
CSFML_API sfSoundBuffer* sfSoundBuffer_CreateFromFile(const char* Filename);

////////////////////////////////////////////////////////////
/// Create a new sound buffer and load it from a file in memory
///
/// \param Data :        Pointer to the file data in memory
/// \param SizeInBytes : Size of the data to load, in bytes
///
/// \return A new sfSoundBuffer object (NULL if failed)
///
////////////////////////////////////////////////////////////
CSFML_API sfSoundBuffer* sfSoundBuffer_CreateFromMemory(const char* Data, size_t SizeInBytes);

////////////////////////////////////////////////////////////
/// Create a new sound buffer and load it from an array of
/// samples in memory - assumed format for samples is
/// 16 bits signed integer
///
/// \param Samples :       Pointer to the samples in memory
/// \param SamplesCount :  Number of samples pointed by Samples
/// \param ChannelsCount : Number of channels (1 = mono, 2 = stereo, ...)
/// \param SampleRate :    Frequency (number of samples to play per second)
///
/// \return A new sfSoundBuffer object (NULL if failed)
///
////////////////////////////////////////////////////////////
CSFML_API sfSoundBuffer* sfSoundBuffer_CreateFromSamples(const sfInt16* Samples, size_t SamplesCount, unsigned int ChannelsCount, unsigned int SampleRate);

////////////////////////////////////////////////////////////
/// Destroy an existing sound buffer
///
/// \param SoundBuffer : Sound buffer to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundBuffer_Destroy(sfSoundBuffer* SoundBuffer);

////////////////////////////////////////////////////////////
/// Save a sound buffer to a file
///
/// \param SoundBuffer : Sound buffer to save
/// \param Filename :    Path of the sound file to write
///
/// \return sfTrue if saving has been successful
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfSoundBuffer_SaveToFile(sfSoundBuffer* SoundBuffer, const char* Filename);

////////////////////////////////////////////////////////////
/// Return the samples contained in a sound buffer
///
/// \param SoundBuffer : Sound buffer to get samples from
///
/// \return Pointer to the array of sound samples, in 16 bits signed integer format
///
////////////////////////////////////////////////////////////
CSFML_API const sfInt16* sfSoundBuffer_GetSamples(sfSoundBuffer* SoundBuffer);

////////////////////////////////////////////////////////////
/// Return the number of samples contained in a sound buffer
///
/// \param SoundBuffer : Sound buffer to get samples count from
///
/// \return Number of samples
///
////////////////////////////////////////////////////////////
CSFML_API size_t sfSoundBuffer_GetSamplesCount(sfSoundBuffer* SoundBuffer);

////////////////////////////////////////////////////////////
/// Get the sample rate of a sound buffer
///
/// \param SoundBuffer : Sound buffer to get sample rate from
///
/// \return Sound frequency (number of samples per second)
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfSoundBuffer_GetSampleRate(sfSoundBuffer* SoundBuffer);

////////////////////////////////////////////////////////////
/// Return the number of channels of a sound buffer (1 = mono, 2 = stereo, ...)
///
/// \param SoundBuffer : Sound buffer to get channels count from
///
/// \return Number of channels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfSoundBuffer_GetChannelsCount(sfSoundBuffer* SoundBuffer);

////////////////////////////////////////////////////////////
/// Get the duration of a sound buffer
///
/// \param SoundBuffer : Sound buffer to get duration from
///
/// \return Sound duration, in seconds
///
////////////////////////////////////////////////////////////
CSFML_API float sfSoundBuffer_GetDuration(sfSoundBuffer* SoundBuffer);


#endif // SFML_SOUNDBUFFER_H
