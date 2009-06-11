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
sfMusic* sfMusic_CreateFromFile(const char* Filename)
{
    sfMusic* Music = new sfMusic;

    if (!Music->This.OpenFromFile(Filename))
    {
        delete Music;
        Music = NULL;
    }

    return Music;
}


////////////////////////////////////////////////////////////
/// Create a new music and load it from a file in memory
////////////////////////////////////////////////////////////
sfMusic* sfMusic_CreateFromMemory(const char* Data, size_t SizeInBytes)
{
    sfMusic* Music = new sfMusic;

    if (!Music->This.OpenFromMemory(Data, SizeInBytes))
    {
        delete Music;
        Music = NULL;
    }

    return Music;
}


////////////////////////////////////////////////////////////
/// Destroy an existing music
////////////////////////////////////////////////////////////
void sfMusic_Destroy(sfMusic* Music)
{
    delete Music;
}


////////////////////////////////////////////////////////////
/// Set a music loop state
////////////////////////////////////////////////////////////
void sfMusic_SetLoop(sfMusic* Music, sfBool Loop)
{
    CSFML_CALL(Music, SetLoop(Loop != 0));
}


////////////////////////////////////////////////////////////
/// Tell whether or not a music is looping
////////////////////////////////////////////////////////////
sfBool sfMusic_GetLoop(sfMusic* Music)
{
    CSFML_CALL_RETURN(Music, GetLoop(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get a music duration
////////////////////////////////////////////////////////////
float sfMusic_GetDuration(sfMusic* Music)
{
    CSFML_CALL_RETURN(Music, GetDuration(), 0.f);
}


////////////////////////////////////////////////////////////
/// Start playing a music
////////////////////////////////////////////////////////////
void sfMusic_Play(sfMusic* Music)
{
    CSFML_CALL(Music, Play());
}


////////////////////////////////////////////////////////////
/// Pause a music
////////////////////////////////////////////////////////////
void sfMusic_Pause(sfMusic* Music)
{
    CSFML_CALL(Music, Pause());
}


////////////////////////////////////////////////////////////
/// Stop playing a music
////////////////////////////////////////////////////////////
void sfMusic_Stop(sfMusic* Music)
{
    CSFML_CALL(Music, Stop());
}


////////////////////////////////////////////////////////////
/// Return the number of channels of a music (1 = mono, 2 = stereo)
////////////////////////////////////////////////////////////
unsigned int sfMusic_GetChannelsCount(sfMusic* Music)
{
    CSFML_CALL_RETURN(Music, GetChannelsCount(), 0);
}


////////////////////////////////////////////////////////////
/// Get the stream sample rate of a music
////////////////////////////////////////////////////////////
unsigned int sfMusic_GetSampleRate(sfMusic* Music)
{
    CSFML_CALL_RETURN(Music, GetSampleRate(), 0);
}


////////////////////////////////////////////////////////////
/// Get the status of a music (stopped, paused, playing)
////////////////////////////////////////////////////////////
sfSoundStatus sfMusic_GetStatus(sfMusic* Music)
{
    CSFML_CHECK_RETURN(Music, sfStopped);

    return static_cast<sfSoundStatus>(Music->This.GetStatus());
}


////////////////////////////////////////////////////////////
/// Get the current playing position of a music
////////////////////////////////////////////////////////////
float sfMusic_GetPlayingOffset(sfMusic* Music)
{
    CSFML_CALL_RETURN(Music, GetPlayingOffset(), 0.f);
}


////////////////////////////////////////////////////////////
/// Set the pitch of a music
////////////////////////////////////////////////////////////
void sfMusic_SetPitch(sfMusic* Music, float Pitch)
{
    CSFML_CALL(Music, SetPitch(Pitch));
}


////////////////////////////////////////////////////////////
/// Set the volume of a music
////////////////////////////////////////////////////////////
void sfMusic_SetVolume(sfMusic* Music, float Volume)
{
    CSFML_CALL(Music, SetVolume(Volume));
}


////////////////////////////////////////////////////////////
/// Set the position of a music
////////////////////////////////////////////////////////////
void sfMusic_SetPosition(sfMusic* Music, float X, float Y, float Z)
{
    CSFML_CALL(Music, SetPosition(sf::Vector3f(X, Y, Z)));
}


////////////////////////////////////////////////////////////
/// Make the music's position relative to the listener's
/// position, or absolute.
/// The default value is false (absolute)
////////////////////////////////////////////////////////////
void sfMusic_SetRelativeToListener(sfMusic* music, sfBool Relative)
{
    CSFML_CALL(music, SetRelativeToListener(Relative == sfTrue));
}


////////////////////////////////////////////////////////////
/// Set the minimum distance - closer than this distance,
/// the listener will hear the music at its maximum volume.
/// The default minimum distance is 1.0
////////////////////////////////////////////////////////////
void sfMusic_SetMinDistance(sfMusic* Music, float MinDistance)
{
    CSFML_CALL(Music, SetMinDistance(MinDistance));
}


////////////////////////////////////////////////////////////
/// Set the attenuation factor - the higher the attenuation, the
/// more the sound will be attenuated with distance from listener.
/// The default attenuation factor 1.0
////////////////////////////////////////////////////////////
void sfMusic_SetAttenuation(sfMusic* Music, float Attenuation)
{
    CSFML_CALL(Music, SetAttenuation(Attenuation));
}


////////////////////////////////////////////////////////////
/// Set the current playing position of a stream
////////////////////////////////////////////////////////////
void sfMusic_SetPlayingOffset(sfMusic* Music, float TimeOffset)
{
    CSFML_CALL(Music, SetPlayingOffset(TimeOffset));
}


////////////////////////////////////////////////////////////
/// Get the pitch of a music
////////////////////////////////////////////////////////////
float sfMusic_GetPitch(sfMusic* Music)
{
    CSFML_CALL_RETURN(Music, GetPitch(), 0.f);
}


////////////////////////////////////////////////////////////
/// Get the volume of a music
////////////////////////////////////////////////////////////
float sfMusic_GetVolume(sfMusic* Music)
{
    CSFML_CALL_RETURN(Music, GetVolume(), 0.f);
}


////////////////////////////////////////////////////////////
/// Get the position of a music
////////////////////////////////////////////////////////////
void sfMusic_GetPosition(sfMusic* Music, float* X, float* Y, float* Z)
{
    CSFML_CHECK(Music);

    if (X && Y && Z)
    {
        sf::Vector3f Position = Music->This.GetPosition();
        *X = Position.x;
        *Y = Position.y;
        *Z = Position.z;
    }
}


////////////////////////////////////////////////////////////
/// Tell if the music's position is relative to the listener's
/// position, or if it's absolute
////////////////////////////////////////////////////////////
CSFML_API sfBool sfMusic_IsRelativeToListener(sfMusic* Music)
{
    CSFML_CALL_RETURN(Music, IsRelativeToListener(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the minimum distance of a music
////////////////////////////////////////////////////////////
float sfMusic_GetMinDistance(sfMusic* Music)
{
    CSFML_CALL_RETURN(Music, GetMinDistance(), 0.f);
}


////////////////////////////////////////////////////////////
/// Get the attenuation factor of a a
////////////////////////////////////////////////////////////
float sfMusic_GetAttenuation(sfMusic* Music)
{
    CSFML_CALL_RETURN(Music, GetAttenuation(), 0.f);
}
