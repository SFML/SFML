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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/SoundBufferRecorder.h>
#include <SFML/Audio/SoundBufferRecorder.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Internal.h>


// WARNING : this structure must always be the SAME as in Audio/SoundBuffer.h
struct sfSoundBuffer
{
    sf::SoundBuffer This;
};

struct sfSoundBufferRecorder
{
    sf::SoundBufferRecorder This;
    sfSoundBuffer           SoundBuffer;
};


////////////////////////////////////////////////////////////
/// Construct a new sound buffer recorder
////////////////////////////////////////////////////////////
sfSoundBufferRecorder* sfSoundBufferRecorder_Create()
{
    return new sfSoundBufferRecorder;
}


////////////////////////////////////////////////////////////
/// Destroy an existing sound buffer recorder
////////////////////////////////////////////////////////////
void sfSoundBufferRecorder_Destroy(sfSoundBufferRecorder* SoundBufferRecorder)
{
    delete SoundBufferRecorder;
}


////////////////////////////////////////////////////////////
/// Start the capture.
/// Warning : only one capture can happen at the same time
////////////////////////////////////////////////////////////
void sfSoundBufferRecorder_Start(sfSoundBufferRecorder* SoundBufferRecorder, unsigned int SampleRate)
{
    CSFML_CALL(SoundBufferRecorder, Start(SampleRate));
}


////////////////////////////////////////////////////////////
/// Stop the capture
////////////////////////////////////////////////////////////
void sfSoundBufferRecorder_Stop(sfSoundBufferRecorder* SoundBufferRecorder)
{
    CSFML_CALL(SoundBufferRecorder, Stop());
}


////////////////////////////////////////////////////////////
/// Get the sample rate of a sound buffer recorder
////////////////////////////////////////////////////////////
unsigned int sfSoundBufferRecorder_GetSampleRate(sfSoundBufferRecorder* SoundBufferRecorder)
{
    CSFML_CALL_RETURN(SoundBufferRecorder, GetSampleRate(), 0);
}


////////////////////////////////////////////////////////////
/// Get the sound buffer containing the captured audio data
/// of a sound buffer recorder
////////////////////////////////////////////////////////////
sfSoundBuffer* sfSoundBufferRecorder_GetBuffer(sfSoundBufferRecorder* SoundBufferRecorder)
{
    CSFML_CHECK_RETURN(SoundBufferRecorder, NULL);

    SoundBufferRecorder->SoundBuffer.This = SoundBufferRecorder->This.GetBuffer();

    return &SoundBufferRecorder->SoundBuffer;
}
