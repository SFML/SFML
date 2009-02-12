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

#ifndef SFML_SOUNDRECORDER_H
#define SFML_SOUNDRECORDER_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Audio/Types.h>


typedef sfBool (*sfSoundRecorderStartCallback)(void*);                           ///< Type of the callback used when starting a capture
typedef sfBool (*sfSoundRecorderProcessCallback)(const sfInt16*, size_t, void*); ///< Type of the callback used to process audio data
typedef void   (*sfSoundRecorderStopCallback)(void*);                            ///< Type of the callback used when stopping a capture


////////////////////////////////////////////////////////////
/// Construct a new sound recorder with callback functions
/// for processing captured samples
///
/// \param OnStart :   Callback function which will be called when a new capture starts (can be NULL)
/// \param OnProcess : Callback function which will be called each time there's audio data to process
/// \param OnStop :    Callback function which will be called when the current capture stops (can be NULL)
/// \param UserData :  Data to pass to the callback function (can be NULL)
///
/// \return A new sfSoundRecorder object (NULL if failed)
///
////////////////////////////////////////////////////////////
CSFML_API sfSoundRecorder* sfSoundRecorder_Create(sfSoundRecorderStartCallback   OnStart,
                                                  sfSoundRecorderProcessCallback OnProcess,
                                                  sfSoundRecorderStopCallback    OnStop,
                                                  void*                          UserData);

////////////////////////////////////////////////////////////
/// Destroy an existing sound recorder
///
/// \param SoundRecorder : Sound recorder to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundRecorder_Destroy(sfSoundRecorder* SoundRecorder);

////////////////////////////////////////////////////////////
/// Start the capture.
/// Warning : only one capture can happen at the same time
///
/// \param SoundRecorder : Sound recorder to start
/// \param SampleRate :    Sound frequency (the more samples, the higher the quality)
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundRecorder_Start(sfSoundRecorder* SoundRecorder, unsigned int SampleRate);

////////////////////////////////////////////////////////////
/// Stop the capture
///
/// \param SoundRecorder : Sound recorder to stop
///
////////////////////////////////////////////////////////////
CSFML_API void sfSoundRecorder_Stop(sfSoundRecorder* SoundRecorder);

////////////////////////////////////////////////////////////
/// Get the sample rate of a sound recorder
///
/// \param SoundRecorder : Sound recorder to get sample rate from
///
/// \return Frequency, in samples per second
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfSoundRecorder_GetSampleRate(sfSoundRecorder* SoundRecorder);

////////////////////////////////////////////////////////////
/// Tell if the system supports sound capture.
/// If not, this class won't be usable
///
/// \return sfTrue if audio capture is supported
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfSoundRecorder_CanCapture();


#endif // SFML_SOUNDRECORDER_H
