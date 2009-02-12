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
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Internal.h>


struct sfSoundBuffer
{
    sf::SoundBuffer This;
};


////////////////////////////////////////////////////////////
/// Create a new sound buffer and load it from a file
////////////////////////////////////////////////////////////
sfSoundBuffer* sfSoundBuffer_CreateFromFile(const char* Filename)
{
    sfSoundBuffer* Buffer = new sfSoundBuffer;

    if (!Buffer->This.LoadFromFile(Filename))
    {
        delete Buffer;
        Buffer = NULL;
    }

    return Buffer;
}


////////////////////////////////////////////////////////////
/// Create a new sound buffer and load it from a file in memory
////////////////////////////////////////////////////////////
sfSoundBuffer* sfSoundBuffer_CreateFromMemory(const char* Data, size_t SizeInBytes)
{
    sfSoundBuffer* Buffer = new sfSoundBuffer;

    if (!Buffer->This.LoadFromMemory(Data, SizeInBytes))
    {
        delete Buffer;
        Buffer = NULL;
    }

    return Buffer;
}


////////////////////////////////////////////////////////////
/// Create a new sound buffer and load it from an array of
/// samples in memory - assumed format for samples is
/// 16 bits signed integer
////////////////////////////////////////////////////////////
sfSoundBuffer* sfSoundBuffer_CreateFromSamples(const sfInt16* Samples, size_t SamplesCount, unsigned int ChannelsCount, unsigned int SampleRate)
{
    sfSoundBuffer* Buffer = new sfSoundBuffer;

    if (!Buffer->This.LoadFromSamples(Samples, SamplesCount, ChannelsCount, SampleRate))
    {
        delete Buffer;
        Buffer = NULL;
    }

    return Buffer;
}


////////////////////////////////////////////////////////////
/// Destroy an existing sound buffer
////////////////////////////////////////////////////////////
void sfSoundBuffer_Destroy(sfSoundBuffer* SoundBuffer)
{
    delete SoundBuffer;
}


////////////////////////////////////////////////////////////
/// Save a sound buffer to a file
////////////////////////////////////////////////////////////
sfBool sfSoundBuffer_SaveToFile(sfSoundBuffer* SoundBuffer, const char* Filename)
{
    CSFML_CALL_RETURN(SoundBuffer, SaveToFile(Filename), sfFalse)
}


////////////////////////////////////////////////////////////
/// Return the samples contained in a sound buffer
////////////////////////////////////////////////////////////
const sfInt16* sfSoundBuffer_GetSamples(sfSoundBuffer* SoundBuffer)
{
    CSFML_CALL_RETURN(SoundBuffer, GetSamples(), NULL)
}


////////////////////////////////////////////////////////////
/// Return the number of samples contained in a sound buffer
////////////////////////////////////////////////////////////
size_t sfSoundBuffer_GetSamplesCount(sfSoundBuffer* SoundBuffer)
{
    CSFML_CALL_RETURN(SoundBuffer, GetSamplesCount(), 0)
}


////////////////////////////////////////////////////////////
/// Get the sample rate of a sound buffer
////////////////////////////////////////////////////////////
unsigned int sfSoundBuffer_GetSampleRate(sfSoundBuffer* SoundBuffer)
{
    CSFML_CALL_RETURN(SoundBuffer, GetSampleRate(), 0)
}


////////////////////////////////////////////////////////////
/// Return the number of channels of a sound buffer (1 = mono, 2 = stereo, ...)
////////////////////////////////////////////////////////////
unsigned int sfSoundBuffer_GetChannelsCount(sfSoundBuffer* SoundBuffer)
{
    CSFML_CALL_RETURN(SoundBuffer, GetChannelsCount(), 0)
}


////////////////////////////////////////////////////////////
/// Get the duration of a sound buffer
////////////////////////////////////////////////////////////
float sfSoundBuffer_GetDuration(sfSoundBuffer* SoundBuffer)
{
    CSFML_CALL_RETURN(SoundBuffer, GetDuration(), 0.f)
}
