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

#ifndef SFML_SOUNDSTREAMSTRUCT_H
#define SFML_SOUNDSTREAMSTRUCT_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/SoundStream.hpp>


////////////////////////////////////////////////////////////
// Helper class implementing the callback forwarding from
// C++ to C in sfSoundStream
////////////////////////////////////////////////////////////
class sfSoundStreamImpl : public sf::SoundStream
{
public :

    sfSoundStreamImpl(sfSoundStreamGetDataCallback OnGetData,
                      sfSoundStreamSeekCallback    OnSeek,
                      unsigned int                 ChannelsCount,
                      unsigned int                 SampleRate,
                      void*                        UserData) :
    myGetDataCallback(OnGetData),
    mySeekCallback   (OnSeek),
    myUserData       (UserData)
    {
        Initialize(ChannelsCount, SampleRate);
    }

private :

    virtual bool OnGetData(Chunk& Data)
    {
        sfSoundStreamChunk Chunk = {NULL, 0};
        bool Continue = (myGetDataCallback(&Chunk, myUserData) == sfTrue);

        Data.Samples   = Chunk.Samples;
        Data.NbSamples = Chunk.NbSamples;

        return Continue;
    }

    virtual void OnSeek(float TimeOffset)
    {
        if (mySeekCallback)
            mySeekCallback(TimeOffset, myUserData);
    }

    sfSoundStreamGetDataCallback myGetDataCallback;
    sfSoundStreamSeekCallback    mySeekCallback;
    void*                        myUserData;
};


////////////////////////////////////////////////////////////
// Internal structure of sfSoundStream
////////////////////////////////////////////////////////////
struct sfSoundStream
{
    sfSoundStream(sfSoundStreamGetDataCallback OnGetData,
                  sfSoundStreamSeekCallback    OnSeek,
                  unsigned int                 ChannelsCount,
                  unsigned int                 SampleRate,
                  void*                        UserData) :
    This(OnGetData, OnSeek, ChannelsCount, SampleRate, UserData)
    {
    }

    sfSoundStreamImpl This;
};


#endif // SFML_SOUNDSTREAMSTRUCT_H
