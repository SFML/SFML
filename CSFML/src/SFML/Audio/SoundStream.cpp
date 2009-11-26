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
#include <SFML/Audio/SoundStream.h>
#include <SFML/Audio/SoundStreamStruct.h>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Construct a new sound stream
////////////////////////////////////////////////////////////
sfSoundStream* sfSoundStream_Create(sfSoundStreamGetDataCallback onGetData,
                                    sfSoundStreamSeekCallback    onSeek,
                                    unsigned int                 channelsCount,
                                    unsigned int                 sampleRate,
                                    void*                        userData)
{
    return new sfSoundStream(onGetData, onSeek, channelsCount, sampleRate, userData);
}


////////////////////////////////////////////////////////////
/// Destroy an existing sound stream
////////////////////////////////////////////////////////////
void sfSoundStream_Destroy(sfSoundStream* soundStream)
{
    delete soundStream;
}


////////////////////////////////////////////////////////////
/// Start playing a sound stream
////////////////////////////////////////////////////////////
void sfSoundStream_Play(sfSoundStream* soundStream)
{
    CSFML_CALL(soundStream, Play());
}


////////////////////////////////////////////////////////////
/// Pause a sound stream
////////////////////////////////////////////////////////////
void sfSoundStream_Pause(sfSoundStream* soundStream)
{
    CSFML_CALL(soundStream, Pause());
}


////////////////////////////////////////////////////////////
/// Stop playing a sound stream
////////////////////////////////////////////////////////////
void sfSoundStream_Stop(sfSoundStream* soundStream)
{
    CSFML_CALL(soundStream, Stop());
}


////////////////////////////////////////////////////////////
/// Get the status of a sound stream (stopped, paused, playing)
////////////////////////////////////////////////////////////
sfSoundStatus sfSoundStream_GetStatus(const sfSoundStream* soundStream)
{
    CSFML_CHECK_RETURN(soundStream, sfStopped);

    return static_cast<sfSoundStatus>(soundStream->This.GetStatus());
}


////////////////////////////////////////////////////////////
/// Return the number of channels of a sound stream
/// (1 = mono, 2 = stereo)
////////////////////////////////////////////////////////////
unsigned int sfSoundStream_GetChannelsCount(const sfSoundStream* soundStream)
{
    CSFML_CALL_RETURN(soundStream, GetChannelsCount(), 0);
}


////////////////////////////////////////////////////////////
/// Get the sample rate of a sound stream
////////////////////////////////////////////////////////////
unsigned int sfSoundStream_GetSampleRate(const sfSoundStream* soundStream)
{
    CSFML_CALL_RETURN(soundStream, GetSampleRate(), 0);
}


////////////////////////////////////////////////////////////
/// Set the pitch of a sound stream
////////////////////////////////////////////////////////////
void sfSoundStream_SetPitch(sfSoundStream* soundStream, float pitch)
{
    CSFML_CALL(soundStream, SetPitch(pitch));
}


////////////////////////////////////////////////////////////
/// Set the volume of a sound stream
////////////////////////////////////////////////////////////
void sfSoundStream_SetVolume(sfSoundStream* soundStream, float volume)
{
    CSFML_CALL(soundStream, SetVolume(volume));
}


////////////////////////////////////////////////////////////
/// Set the position of a sound stream
////////////////////////////////////////////////////////////
void sfSoundStream_SetPosition(sfSoundStream* soundStream, float x, float y, float z)
{
    CSFML_CALL(soundStream, SetPosition(x, y, z));
}


////////////////////////////////////////////////////////////
/// Make the sound stream's position relative to the listener's
/// position, or absolute.
/// The default value is false (absolute)
////////////////////////////////////////////////////////////
void sfSoundStream_SetRelativeToListener(sfSoundStream* soundStream, sfBool relative)
{
    CSFML_CALL(soundStream, SetRelativeToListener(relative == sfTrue));
}


////////////////////////////////////////////////////////////
/// Set the minimum distance - closer than this distance,
/// the listener will hear the sound stream at its maximum volume.
/// The default minimum distance is 1.0
////////////////////////////////////////////////////////////
void sfSoundStream_SetMinDistance(sfSoundStream* soundStream, float distance)
{
    CSFML_CALL(soundStream, SetMinDistance(distance));
}


////////////////////////////////////////////////////////////
/// Set the attenuation factor - the higher the attenuation, the
/// more the sound stream will be attenuated with distance from listener.
/// The default attenuation factor 1.0
////////////////////////////////////////////////////////////
void sfSoundStream_SetAttenuation(sfSoundStream* soundStream, float attenuation)
{
    CSFML_CALL(soundStream, SetAttenuation(attenuation));
}


////////////////////////////////////////////////////////////
/// Set the current playing position of a stream
////////////////////////////////////////////////////////////
void sfSoundStream_SetPlayingOffset(sfSoundStream* soundStream, float timeOffset)
{
    CSFML_CALL(soundStream, SetPlayingOffset(timeOffset));
}


////////////////////////////////////////////////////////////
/// Set a stream loop state
////////////////////////////////////////////////////////////
void sfSoundStream_SetLoop(sfSoundStream* soundStream, sfBool loop)
{
    CSFML_CALL(soundStream, SetLoop(loop == sfTrue));
}


////////////////////////////////////////////////////////////
/// Get the pitch of a sound stream
////////////////////////////////////////////////////////////
float sfSoundStream_GetPitch(const sfSoundStream* soundStream)
{
    CSFML_CALL_RETURN(soundStream, GetPitch(), 0.f);
}


////////////////////////////////////////////////////////////
/// Get the volume of a sound stream
////////////////////////////////////////////////////////////
float sfSoundStream_GetVolume(const sfSoundStream* soundStream)
{
    CSFML_CALL_RETURN(soundStream, GetVolume(), 0.f);
}


////////////////////////////////////////////////////////////
/// Get the position of a sound stream
////////////////////////////////////////////////////////////
void sfSoundStream_GetPosition(const sfSoundStream* soundStream, float* x, float* y, float* z)
{
    CSFML_CHECK(soundStream);

    sf::Vector3f position = soundStream->This.GetPosition();
    if (x) *x = position.x;
    if (y) *y = position.y;
    if (z) *z = position.z;
}


////////////////////////////////////////////////////////////
/// Tell if the sound stream's position is relative to the listener's
/// position, or if it's absolute
////////////////////////////////////////////////////////////
CSFML_API sfBool sfSoundStream_IsRelativeToListener(const sfSoundStream* soundStream)
{
    CSFML_CALL_RETURN(soundStream, IsRelativeToListener(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the minimum distance of a sound stream
////////////////////////////////////////////////////////////
float sfSoundStream_GetMinDistance(const sfSoundStream* soundStream)
{
    CSFML_CALL_RETURN(soundStream, GetMinDistance(), 0.f);
}


////////////////////////////////////////////////////////////
/// Get the attenuation factor of a sound stream
////////////////////////////////////////////////////////////
float sfSoundStream_GetAttenuation(const sfSoundStream* soundStream)
{
    CSFML_CALL_RETURN(soundStream, GetAttenuation(), 0.f);
}


////////////////////////////////////////////////////////////
/// Tell whether or not a stream is looping
////////////////////////////////////////////////////////////
sfBool sfSoundStream_GetLoop(const sfSoundStream* soundStream)
{
    CSFML_CALL_RETURN(soundStream, GetLoop(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the current playing position of a sound stream
////////////////////////////////////////////////////////////
float sfSoundStream_GetPlayingOffset(const sfSoundStream* soundStream)
{
    CSFML_CALL_RETURN(soundStream, GetPlayingOffset(), 0.f);
}
