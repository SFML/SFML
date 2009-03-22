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
#include <SFML/Audio/SoundStream.h>
#include <SFML/Audio/SoundStream.hpp>
#include <SFML/Internal.h>


class sfSoundStreamImpl : public sf::SoundStream
{
public :

    sfSoundStreamImpl(sfSoundStreamStartCallback   OnStart,
                      sfSoundStreamGetDataCallback OnGetData,
                      unsigned int                 ChannelsCount,
                      unsigned int                 SampleRate,
                      void*                        UserData) :
    myStartCallback  (OnStart),
    myGetDataCallback(OnGetData),
    myUserData       (UserData)
    {
        Initialize(ChannelsCount, SampleRate);
    }

private :

    virtual bool OnStart()
    {
        if (myStartCallback)
            return myStartCallback(myUserData) == sfTrue;
        else
            return true;
    }

    virtual bool OnGetData(Chunk& Data)
    {
        sfSoundStreamChunk Chunk = {NULL, 0};
        bool Continue = (myGetDataCallback(&Chunk, myUserData) == sfTrue);

        Data.Samples   = Chunk.Samples;
        Data.NbSamples = Chunk.NbSamples;

        return Continue;
    }

    sfSoundStreamStartCallback   myStartCallback;
    sfSoundStreamGetDataCallback myGetDataCallback;
    void*                        myUserData;
};


struct sfSoundStream
{

    sfSoundStream(sfSoundStreamStartCallback   OnStart,
                  sfSoundStreamGetDataCallback OnGetData,
                  unsigned int                 ChannelsCount,
                  unsigned int                 SampleRate,
                  void*                        UserData) :
    This(OnStart, OnGetData, ChannelsCount, SampleRate, UserData)
    {
    }

    sfSoundStreamImpl This;
};


////////////////////////////////////////////////////////////
/// Construct a new sound stream
////////////////////////////////////////////////////////////
sfSoundStream* sfSoundStream_Create(sfSoundStreamStartCallback   OnStart,
                                    sfSoundStreamGetDataCallback OnGetData,
                                    unsigned int                 ChannelsCount,
                                    unsigned int                 SampleRate,
                                    void*                        UserData)
{
    return new sfSoundStream(OnStart, OnGetData, ChannelsCount, SampleRate, UserData);
}


////////////////////////////////////////////////////////////
/// Destroy an existing sound stream
////////////////////////////////////////////////////////////
void sfSoundStream_Destroy(sfSoundStream* SoundStream)
{
    delete SoundStream;
}


////////////////////////////////////////////////////////////
/// Start playing a sound stream
////////////////////////////////////////////////////////////
void sfSoundStream_Play(sfSoundStream* SoundStream)
{
    CSFML_CALL(SoundStream, Play());
}


////////////////////////////////////////////////////////////
/// Pause a sound stream
////////////////////////////////////////////////////////////
void sfSoundStream_Pause(sfSoundStream* SoundStream)
{
    CSFML_CALL(SoundStream, Pause());
}


////////////////////////////////////////////////////////////
/// Stop playing a sound stream
////////////////////////////////////////////////////////////
void sfSoundStream_Stop(sfSoundStream* SoundStream)
{
    CSFML_CALL(SoundStream, Stop());
}


////////////////////////////////////////////////////////////
/// Get the status of a sound stream (stopped, paused, playing)
////////////////////////////////////////////////////////////
sfSoundStatus sfSoundStream_GetStatus(sfSoundStream* SoundStream)
{
    CSFML_CHECK_RETURN(SoundStream, sfStopped);

    return static_cast<sfSoundStatus>(SoundStream->This.GetStatus());
}


////////////////////////////////////////////////////////////
/// Return the number of channels of a sound stream
/// (1 = mono, 2 = stereo)
////////////////////////////////////////////////////////////
unsigned int sfSoundStream_GetChannelsCount(sfSoundStream* SoundStream)
{
    CSFML_CALL_RETURN(SoundStream, GetChannelsCount(), 0);
}


////////////////////////////////////////////////////////////
/// Get the sample rate of a sound stream
////////////////////////////////////////////////////////////
unsigned int sfSoundStream_GetSampleRate(sfSoundStream* SoundStream)
{
    CSFML_CALL_RETURN(SoundStream, GetSampleRate(), 0);
}


////////////////////////////////////////////////////////////
/// Set the pitch of a sound stream
////////////////////////////////////////////////////////////
void sfSoundStream_SetPitch(sfSoundStream* SoundStream, float Pitch)
{
    CSFML_CALL(SoundStream, SetPitch(Pitch));
}


////////////////////////////////////////////////////////////
/// Set the volume of a sound stream
////////////////////////////////////////////////////////////
void sfSoundStream_SetVolume(sfSoundStream* SoundStream, float Volume)
{
    CSFML_CALL(SoundStream, SetVolume(Volume));
}


////////////////////////////////////////////////////////////
/// Set the position of a sound stream
////////////////////////////////////////////////////////////
void sfSoundStream_SetPosition(sfSoundStream* SoundStream, float X, float Y, float Z)
{
    CSFML_CALL(SoundStream, SetPosition(X, Y, Z));
}


////////////////////////////////////////////////////////////
/// Make the sound stream's position relative to the listener's
/// position, or absolute.
/// The default value is false (absolute)
////////////////////////////////////////////////////////////
void sfSoundStream_SetRelativeToListener(sfSoundStream* SoundStream, sfBool Relative)
{
    CSFML_CALL(SoundStream, SetRelativeToListener(Relative == sfTrue));
}


////////////////////////////////////////////////////////////
/// Set the minimum distance - closer than this distance,
/// the listener will hear the sound stream at its maximum volume.
/// The default minimum distance is 1.0
////////////////////////////////////////////////////////////
void sfSoundStream_SetMinDistance(sfSoundStream* SoundStream, float MinDistance)
{
    CSFML_CALL(SoundStream, SetMinDistance(MinDistance));
}


////////////////////////////////////////////////////////////
/// Set the attenuation factor - the higher the attenuation, the
/// more the sound stream will be attenuated with distance from listener.
/// The default attenuation factor 1.0
////////////////////////////////////////////////////////////
void sfSoundStream_SetAttenuation(sfSoundStream* SoundStream, float Attenuation)
{
    CSFML_CALL(SoundStream, SetAttenuation(Attenuation));
}


////////////////////////////////////////////////////////////
/// Set a stream loop state
////////////////////////////////////////////////////////////
void sfSoundStream_SetLoop(sfSoundStream* SoundStream, sfBool Loop)
{
    CSFML_CALL(SoundStream, SetLoop(Loop == sfTrue));
}


////////////////////////////////////////////////////////////
/// Get the pitch of a sound stream
////////////////////////////////////////////////////////////
float sfSoundStream_GetPitch(sfSoundStream* SoundStream)
{
    CSFML_CALL_RETURN(SoundStream, GetPitch(), 0.f);
}


////////////////////////////////////////////////////////////
/// Get the volume of a sound stream
////////////////////////////////////////////////////////////
float sfSoundStream_GetVolume(sfSoundStream* SoundStream)
{
    CSFML_CALL_RETURN(SoundStream, GetVolume(), 0.f);
}


////////////////////////////////////////////////////////////
/// Get the position of a sound stream
////////////////////////////////////////////////////////////
void sfSoundStream_GetPosition(sfSoundStream* SoundStream, float* X, float* Y, float* Z)
{
    CSFML_CHECK(SoundStream);

    sf::Vector3f Position = SoundStream->This.GetPosition();
    if (X) *X = Position.x;
    if (Y) *Y = Position.y;
    if (Z) *Z = Position.z;
}


////////////////////////////////////////////////////////////
/// Tell if the sound stream's position is relative to the listener's
/// position, or if it's absolute
////////////////////////////////////////////////////////////
CSFML_API sfBool sfSoundStream_IsRelativeToListener(sfSoundStream* SoundStream)
{
    CSFML_CALL_RETURN(SoundStream, IsRelativeToListener(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the minimum distance of a sound stream
////////////////////////////////////////////////////////////
float sfSoundStream_GetMinDistance(sfSoundStream* SoundStream)
{
    CSFML_CALL_RETURN(SoundStream, GetMinDistance(), 0.f);
}


////////////////////////////////////////////////////////////
/// Get the attenuation factor of a sound stream
////////////////////////////////////////////////////////////
float sfSoundStream_GetAttenuation(sfSoundStream* SoundStream)
{
    CSFML_CALL_RETURN(SoundStream, GetAttenuation(), 0.f);
}


////////////////////////////////////////////////////////////
/// Tell whether or not a stream is looping
////////////////////////////////////////////////////////////
sfBool sfSoundStream_GetLoop(sfSoundStream* SoundStream)
{
    CSFML_CALL_RETURN(SoundStream, GetLoop(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the current playing position of a sound stream
////////////////////////////////////////////////////////////
float sfSoundStream_GetPlayingOffset(sfSoundStream* SoundStream)
{
    CSFML_CALL_RETURN(SoundStream, GetPlayingOffset(), 0.f);
}
