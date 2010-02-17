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

#ifndef SFML_SOUNDBUFFER_H
#define SFML_SOUNDBUFFER_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Audio/Types.h>


////////////////////////////////////////////////////////////
/// Create a new sound buffer and load it from a file
///
/// \param filename : Path of the music file to open
///
/// \return A new sfSoundBuffer object (NULL if failed)
///
////////////////////////////////////////////////////////////
CSFML_API sfSoundBuffer* sfSoundBuffer_CreateFromFile(const char* filename);

////////////////////////////////////////////////////////////
/// Create a new sound buffer and load it from a file in memory
///
/// \param data :        Pointer to the file data in memory
/// \param sizeInBytes : Size of the data to load, in bytes
///
/// \return A new sfSoundBuffer object (NULL if failed)
///
////////////////////////////////////////////////////////////
CSFML_API sfSoundBuffer* sfSoundBuffer_CreateFromMemory(const void* data, size_t sizeInBytes);

////////////////////////////////////////////////////////////
/// Create a new sound buffer and load it from an array of
/// samples in memory - assumed format for samples is
/// 16 bits signed integer
///
/// \param samples :       Pointer to the samples in memory
/// \param samplesCount :  Number of samples pointed by Samples
/// \param channelsCount : Number of channels (1 = mono, 2 = stereo, ...)
/// \param sampleRate :    Frequency (number of samples to play per second)
///
/// \return A new sfSoundBuffer object (NULL if failed)
///
////////////////////////////////////////////////////////////
CSFML_API sfSoundBuffer* sfSoundBuffer_CreateFromSamples(const sfInt16* samples, size_t samplesCount, unsigned int channelsCount, unsigned int sampleRate);

////////////////////////////////////////////////////////////
/// Copy an existing sound buffer
///
/// \param soundBuffer : Sound buffer to copy
///
/// \return Copied object
///
////////////////////////////////////////////////////////////
CSFML_API sfSoundBuffer* sfSoundBuffer_Copy(sfSoundBuffer* soundBuffer);

////////////////////////////////////////////////////////////
/// Destroy an existing sound buffer
///
/// \param soundBuffer : Sound buffer to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundBuffer_Destroy(sfSoundBuffer* soundBuffer);

////////////////////////////////////////////////////////////
/// Save a sound buffer to a file
///
/// \param soundBuffer : Sound buffer to save
/// \param filename :    Path of the sound file to write
///
/// \return sfTrue if saving has been successful
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfSoundBuffer_SaveToFile(const sfSoundBuffer* soundBuffer, const char* filename);

////////////////////////////////////////////////////////////
/// Return the samples contained in a sound buffer
///
/// \param soundBuffer : Sound buffer to get samples from
///
/// \return Pointer to the array of sound samples, in 16 bits signed integer format
///
////////////////////////////////////////////////////////////
CSFML_API const sfInt16* sfSoundBuffer_GetSamples(const sfSoundBuffer* soundBuffer);

////////////////////////////////////////////////////////////
/// Return the number of samples contained in a sound buffer
///
/// \param soundBuffer : Sound buffer to get samples count from
///
/// \return Number of samples
///
////////////////////////////////////////////////////////////
CSFML_API size_t sfSoundBuffer_GetSamplesCount(const sfSoundBuffer* soundBuffer);

////////////////////////////////////////////////////////////
/// Get the sample rate of a sound buffer
///
/// \param soundBuffer : Sound buffer to get sample rate from
///
/// \return Sound frequency (number of samples per second)
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfSoundBuffer_GetSampleRate(const sfSoundBuffer* soundBuffer);

////////////////////////////////////////////////////////////
/// Return the number of channels of a sound buffer (1 = mono, 2 = stereo, ...)
///
/// \param soundBuffer : Sound buffer to get channels count from
///
/// \return Number of channels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfSoundBuffer_GetChannelsCount(const sfSoundBuffer* soundBuffer);

////////////////////////////////////////////////////////////
/// Get the duration of a sound buffer
///
/// \param soundBuffer : Sound buffer to get duration from
///
/// \return Sound duration, in seconds
///
////////////////////////////////////////////////////////////
CSFML_API float sfSoundBuffer_GetDuration(const sfSoundBuffer* soundBuffer);


#endif // SFML_SOUNDBUFFER_H
