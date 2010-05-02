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
#include <SFML/Audio/SoundBufferRecorder.h>
#include <SFML/Audio/SoundBufferRecorderStruct.h>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Construct a new sound buffer recorder
////////////////////////////////////////////////////////////
sfSoundBufferRecorder* sfSoundBufferRecorder_Create(void)
{
    return new sfSoundBufferRecorder;
}


////////////////////////////////////////////////////////////
/// Destroy an existing sound buffer recorder
////////////////////////////////////////////////////////////
void sfSoundBufferRecorder_Destroy(sfSoundBufferRecorder* soundBufferRecorder)
{
    delete soundBufferRecorder;
}


////////////////////////////////////////////////////////////
/// Start the capture.
/// Warning : only one capture can happen at the same time
////////////////////////////////////////////////////////////
void sfSoundBufferRecorder_Start(sfSoundBufferRecorder* soundBufferRecorder, unsigned int sampleRate)
{
    CSFML_CALL(soundBufferRecorder, Start(sampleRate));
}


////////////////////////////////////////////////////////////
/// Stop the capture
////////////////////////////////////////////////////////////
void sfSoundBufferRecorder_Stop(sfSoundBufferRecorder* soundBufferRecorder)
{
    CSFML_CALL(soundBufferRecorder, Stop());
}


////////////////////////////////////////////////////////////
/// Get the sample rate of a sound buffer recorder
////////////////////////////////////////////////////////////
unsigned int sfSoundBufferRecorder_GetSampleRate(const sfSoundBufferRecorder* soundBufferRecorder)
{
    CSFML_CALL_RETURN(soundBufferRecorder, GetSampleRate(), 0);
}


////////////////////////////////////////////////////////////
/// Get the sound buffer containing the captured audio data
/// of a sound buffer recorder
////////////////////////////////////////////////////////////
const sfSoundBuffer* sfSoundBufferRecorder_GetBuffer(const sfSoundBufferRecorder* soundBufferRecorder)
{
    CSFML_CHECK_RETURN(soundBufferRecorder, NULL);

    soundBufferRecorder->SoundBuffer.This = soundBufferRecorder->This.GetBuffer();

    return &soundBufferRecorder->SoundBuffer;
}
