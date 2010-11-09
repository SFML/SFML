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
#include <SFML/Audio/SoundRecorder.h>
#include <SFML/Audio/SoundRecorderStruct.h>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Construct a new sound recorder with callback functions
/// for processing captured samples
////////////////////////////////////////////////////////////
sfSoundRecorder* sfSoundRecorder_Create(sfSoundRecorderStartCallback   onStart,
                                        sfSoundRecorderProcessCallback onProcess,
                                        sfSoundRecorderStopCallback    onStop,
                                        void*                          userData)
{
    return new sfSoundRecorder(onStart, onProcess, onStop, userData);
}


////////////////////////////////////////////////////////////
/// Destroy an existing sound recorder
////////////////////////////////////////////////////////////
void sfSoundRecorder_Destroy(sfSoundRecorder* soundRecorder)
{
    delete soundRecorder;
}


////////////////////////////////////////////////////////////
/// Start the capture.
/// Warning : only one capture can happen at the same time
////////////////////////////////////////////////////////////
void sfSoundRecorder_Start(sfSoundRecorder* soundRecorder, unsigned int sampleRate)
{
    CSFML_CALL(soundRecorder, Start(sampleRate));
}


////////////////////////////////////////////////////////////
/// Stop the capture
////////////////////////////////////////////////////////////
void sfSoundRecorder_Stop(sfSoundRecorder* soundRecorder)
{
    CSFML_CALL(soundRecorder, Stop());
}


////////////////////////////////////////////////////////////
/// Get the sample rate of a sound recorder
////////////////////////////////////////////////////////////
unsigned int sfSoundRecorder_GetSampleRate(const sfSoundRecorder* soundRecorder)
{
    CSFML_CALL_RETURN(soundRecorder, GetSampleRate(), 0);
}


////////////////////////////////////////////////////////////
/// Tell if the system supports sound capture.
/// If not, this class won't be usable
////////////////////////////////////////////////////////////
sfBool sfSoundRecorder_IsAvailable(void)
{
    return sf::SoundRecorder::IsAvailable() ? sfTrue : sfFalse;
}
