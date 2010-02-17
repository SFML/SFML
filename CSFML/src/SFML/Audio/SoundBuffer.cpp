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
#include <SFML/Audio/SoundBuffer.h>
#include <SFML/Audio/SoundBufferStruct.h>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Create a new sound buffer and load it from a file
////////////////////////////////////////////////////////////
sfSoundBuffer* sfSoundBuffer_CreateFromFile(const char* filename)
{
    sfSoundBuffer* buffer = new sfSoundBuffer;

    if (!buffer->This.LoadFromFile(filename))
    {
        delete buffer;
        buffer = NULL;
    }

    return buffer;
}


////////////////////////////////////////////////////////////
/// Create a new sound buffer and load it from a file in memory
////////////////////////////////////////////////////////////
sfSoundBuffer* sfSoundBuffer_CreateFromMemory(const void* data, size_t sizeInBytes)
{
    sfSoundBuffer* buffer = new sfSoundBuffer;

    if (!buffer->This.LoadFromMemory(data, sizeInBytes))
    {
        delete buffer;
        buffer = NULL;
    }

    return buffer;
}


////////////////////////////////////////////////////////////
/// Create a new sound buffer and load it from an array of
/// samples in memory - assumed format for samples is
/// 16 bits signed integer
////////////////////////////////////////////////////////////
sfSoundBuffer* sfSoundBuffer_CreateFromSamples(const sfInt16* samples, size_t samplesCount, unsigned int channelsCount, unsigned int sampleRate)
{
    sfSoundBuffer* buffer = new sfSoundBuffer;

    if (!buffer->This.LoadFromSamples(samples, samplesCount, channelsCount, sampleRate))
    {
        delete buffer;
        buffer = NULL;
    }

    return buffer;
}


////////////////////////////////////////////////////////////
/// Copy an existing sound buffer
////////////////////////////////////////////////////////////
sfSoundBuffer* sfSoundBuffer_Copy(sfSoundBuffer* soundBuffer)
{
    CSFML_CHECK_RETURN(soundBuffer, NULL);

    return new sfSoundBuffer(*soundBuffer);
}


////////////////////////////////////////////////////////////
/// Destroy an existing sound buffer
////////////////////////////////////////////////////////////
void sfSoundBuffer_Destroy(sfSoundBuffer* soundBuffer)
{
    delete soundBuffer;
}


////////////////////////////////////////////////////////////
/// Save a sound buffer to a file
////////////////////////////////////////////////////////////
sfBool sfSoundBuffer_SaveToFile(const sfSoundBuffer* soundBuffer, const char* filename)
{
    CSFML_CALL_RETURN(soundBuffer, SaveToFile(filename), sfFalse)
}


////////////////////////////////////////////////////////////
/// Return the samples contained in a sound buffer
////////////////////////////////////////////////////////////
const sfInt16* sfSoundBuffer_GetSamples(const sfSoundBuffer* soundBuffer)
{
    CSFML_CALL_RETURN(soundBuffer, GetSamples(), NULL)
}


////////////////////////////////////////////////////////////
/// Return the number of samples contained in a sound buffer
////////////////////////////////////////////////////////////
size_t sfSoundBuffer_GetSamplesCount(const sfSoundBuffer* soundBuffer)
{
    CSFML_CALL_RETURN(soundBuffer, GetSamplesCount(), 0)
}


////////////////////////////////////////////////////////////
/// Get the sample rate of a sound buffer
////////////////////////////////////////////////////////////
unsigned int sfSoundBuffer_GetSampleRate(const sfSoundBuffer* soundBuffer)
{
    CSFML_CALL_RETURN(soundBuffer, GetSampleRate(), 0)
}


////////////////////////////////////////////////////////////
/// Return the number of channels of a sound buffer (1 = mono, 2 = stereo, ...)
////////////////////////////////////////////////////////////
unsigned int sfSoundBuffer_GetChannelsCount(const sfSoundBuffer* soundBuffer)
{
    CSFML_CALL_RETURN(soundBuffer, GetChannelsCount(), 0)
}


////////////////////////////////////////////////////////////
/// Get the duration of a sound buffer
////////////////////////////////////////////////////////////
float sfSoundBuffer_GetDuration(const sfSoundBuffer* soundBuffer)
{
    CSFML_CALL_RETURN(soundBuffer, GetDuration(), 0.f)
}
