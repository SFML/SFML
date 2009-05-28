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

#ifndef SFML_SOUNDBUFFERRECORDER_H
#define SFML_SOUNDBUFFERRECORDER_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Audio/Types.h>


////////////////////////////////////////////////////////////
/// Construct a new sound buffer recorder
///
/// \return A new sfSoundBufferRecorder object (NULL if failed)
///
////////////////////////////////////////////////////////////
CSFML_API sfSoundBufferRecorder* sfSoundBufferRecorder_Create();

////////////////////////////////////////////////////////////
/// Destroy an existing sound buffer recorder
///
/// \param SoundBufferRecorder : Sound buffer recorder to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundBufferRecorder_Destroy(sfSoundBufferRecorder* SoundBufferRecorder);

////////////////////////////////////////////////////////////
/// Start the capture.
/// Warning : only one capture can happen at the same time
///
/// \param SoundBufferRecorder : Sound bufferrecorder to start
/// \param SampleRate :          Sound frequency (the more samples, the higher the quality)
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundBufferRecorder_Start(sfSoundBufferRecorder* SoundBufferRecorder, unsigned int SampleRate);

////////////////////////////////////////////////////////////
/// Stop the capture
///
/// \param SoundBufferRecorder : Sound buffer recorder to stop
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundBufferRecorder_Stop(sfSoundBufferRecorder* SoundBufferRecorder);

////////////////////////////////////////////////////////////
/// Get the sample rate of a sound buffer recorder
///
/// \param SoundBufferRecorder : Sound buffer recorder to get sample rate from
///
/// \return Frequency, in samples per second
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfSoundBufferRecorder_GetSampleRate(sfSoundBufferRecorder* SoundBufferRecorder);

////////////////////////////////////////////////////////////
/// Get the sound buffer containing the captured audio data
/// of a sound buffer recorder
///
/// \param SoundBufferRecorder : Sound buffer recorder to get the sound buffer from
///
/// \return Pointer to the sound buffer (you don't need to destroy it after use)
///
////////////////////////////////////////////////////////////
CSFML_API sfSoundBuffer* sfSoundBufferRecorder_GetBuffer(sfSoundBufferRecorder* SoundBufferRecorder);


#endif // SFML_SOUNDBUFFERRECORDER_H
