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
#include <SFML/Audio/Music.h>
#include <SFML/Audio/MusicStruct.h>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Create a new music and load it from a file
////////////////////////////////////////////////////////////
sfMusic* sfMusic_CreateFromFile(const char* filename)
{
    sfMusic* music = new sfMusic;

    if (!music->This.OpenFromFile(filename))
    {
        delete music;
        music = NULL;
    }

    return music;
}


////////////////////////////////////////////////////////////
/// Create a new music and load it from a file in memory
////////////////////////////////////////////////////////////
sfMusic* sfMusic_CreateFromMemory(const void* data, size_t sizeInBytes)
{
    sfMusic* music = new sfMusic;

    if (!music->This.OpenFromMemory(data, sizeInBytes))
    {
        delete music;
        music = NULL;
    }

    return music;
}


////////////////////////////////////////////////////////////
/// Destroy an existing music
////////////////////////////////////////////////////////////
void sfMusic_Destroy(sfMusic* music)
{
    delete music;
}


////////////////////////////////////////////////////////////
/// Set a music loop state
////////////////////////////////////////////////////////////
void sfMusic_SetLoop(sfMusic* music, sfBool loop)
{
    CSFML_CALL(music, SetLoop(loop != 0));
}


////////////////////////////////////////////////////////////
/// Tell whether or not a music is looping
////////////////////////////////////////////////////////////
sfBool sfMusic_GetLoop(const sfMusic* music)
{
    CSFML_CALL_RETURN(music, GetLoop(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get a music duration
////////////////////////////////////////////////////////////
float sfMusic_GetDuration(const sfMusic* music)
{
    CSFML_CALL_RETURN(music, GetDuration(), 0.f);
}


////////////////////////////////////////////////////////////
/// Start playing a music
////////////////////////////////////////////////////////////
void sfMusic_Play(sfMusic* music)
{
    CSFML_CALL(music, Play());
}


////////////////////////////////////////////////////////////
/// Pause a music
////////////////////////////////////////////////////////////
void sfMusic_Pause(sfMusic* music)
{
    CSFML_CALL(music, Pause());
}


////////////////////////////////////////////////////////////
/// Stop playing a music
////////////////////////////////////////////////////////////
void sfMusic_Stop(sfMusic* music)
{
    CSFML_CALL(music, Stop());
}


////////////////////////////////////////////////////////////
/// Return the number of channels of a music (1 = mono, 2 = stereo)
////////////////////////////////////////////////////////////
unsigned int sfMusic_GetChannelsCount(const sfMusic* music)
{
    CSFML_CALL_RETURN(music, GetChannelsCount(), 0);
}


////////////////////////////////////////////////////////////
/// Get the stream sample rate of a music
////////////////////////////////////////////////////////////
unsigned int sfMusic_GetSampleRate(const sfMusic* music)
{
    CSFML_CALL_RETURN(music, GetSampleRate(), 0);
}


////////////////////////////////////////////////////////////
/// Get the status of a music (stopped, paused, playing)
////////////////////////////////////////////////////////////
sfSoundStatus sfMusic_GetStatus(const sfMusic* music)
{
    CSFML_CHECK_RETURN(music, sfStopped);

    return static_cast<sfSoundStatus>(music->This.GetStatus());
}


////////////////////////////////////////////////////////////
/// Get the current playing position of a music
////////////////////////////////////////////////////////////
float sfMusic_GetPlayingOffset(const sfMusic* music)
{
    CSFML_CALL_RETURN(music, GetPlayingOffset(), 0.f);
}


////////////////////////////////////////////////////////////
/// Set the pitch of a music
////////////////////////////////////////////////////////////
void sfMusic_SetPitch(sfMusic* music, float pitch)
{
    CSFML_CALL(music, SetPitch(pitch));
}


////////////////////////////////////////////////////////////
/// Set the volume of a music
////////////////////////////////////////////////////////////
void sfMusic_SetVolume(sfMusic* music, float volume)
{
    CSFML_CALL(music, SetVolume(volume));
}


////////////////////////////////////////////////////////////
/// Set the position of a music
////////////////////////////////////////////////////////////
void sfMusic_SetPosition(sfMusic* music, float x, float y, float z)
{
    CSFML_CALL(music, SetPosition(sf::Vector3f(x, y, z)));
}


////////////////////////////////////////////////////////////
/// Make the music's position relative to the listener's
/// position, or absolute.
/// The default value is false (absolute)
////////////////////////////////////////////////////////////
void sfMusic_SetRelativeToListener(sfMusic* music, sfBool relative)
{
    CSFML_CALL(music, SetRelativeToListener(relative == sfTrue));
}


////////////////////////////////////////////////////////////
/// Set the minimum distance - closer than this distance,
/// the listener will hear the music at its maximum volume.
/// The default minimum distance is 1.0
////////////////////////////////////////////////////////////
void sfMusic_SetMinDistance(sfMusic* music, float distance)
{
    CSFML_CALL(music, SetMinDistance(distance));
}


////////////////////////////////////////////////////////////
/// Set the attenuation factor - the higher the attenuation, the
/// more the sound will be attenuated with distance from listener.
/// The default attenuation factor 1.0
////////////////////////////////////////////////////////////
void sfMusic_SetAttenuation(sfMusic* music, float attenuation)
{
    CSFML_CALL(music, SetAttenuation(attenuation));
}


////////////////////////////////////////////////////////////
/// Set the current playing position of a stream
////////////////////////////////////////////////////////////
void sfMusic_SetPlayingOffset(sfMusic* music, float timeOffset)
{
    CSFML_CALL(music, SetPlayingOffset(timeOffset));
}


////////////////////////////////////////////////////////////
/// Get the pitch of a music
////////////////////////////////////////////////////////////
float sfMusic_GetPitch(const sfMusic* music)
{
    CSFML_CALL_RETURN(music, GetPitch(), 0.f);
}


////////////////////////////////////////////////////////////
/// Get the volume of a music
////////////////////////////////////////////////////////////
float sfMusic_GetVolume(const sfMusic* music)
{
    CSFML_CALL_RETURN(music, GetVolume(), 0.f);
}


////////////////////////////////////////////////////////////
/// Get the position of a music
////////////////////////////////////////////////////////////
void sfMusic_GetPosition(const sfMusic* music, float* x, float* y, float* z)
{
    CSFML_CHECK(music);

    if (x && y && z)
    {
        sf::Vector3f position = music->This.GetPosition();
        *x = position.x;
        *y = position.y;
        *z = position.z;
    }
}


////////////////////////////////////////////////////////////
/// Tell if the music's position is relative to the listener's
/// position, or if it's absolute
////////////////////////////////////////////////////////////
CSFML_API sfBool sfMusic_IsRelativeToListener(const sfMusic* music)
{
    CSFML_CALL_RETURN(music, IsRelativeToListener(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the minimum distance of a music
////////////////////////////////////////////////////////////
float sfMusic_GetMinDistance(const sfMusic* music)
{
    CSFML_CALL_RETURN(music, GetMinDistance(), 0.f);
}


////////////////////////////////////////////////////////////
/// Get the attenuation factor of a music
////////////////////////////////////////////////////////////
float sfMusic_GetAttenuation(const sfMusic* music)
{
    CSFML_CALL_RETURN(music, GetAttenuation(), 0.f);
}
