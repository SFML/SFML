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

#ifndef SFML_SOUNDRECORDERSTRUCT_H
#define SFML_SOUNDRECORDERSTRUCT_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/SoundRecorder.hpp>
#include <SFML/Audio/SoundRecorder.h>


////////////////////////////////////////////////////////////
// Helper class implementing the callback forwarding from
// C++ to C in sfSoundRecorder
////////////////////////////////////////////////////////////
class sfSoundRecorderImpl : public sf::SoundRecorder
{
public :

    sfSoundRecorderImpl(sfSoundRecorderStartCallback   OnStart,
                        sfSoundRecorderProcessCallback OnProcess,
                        sfSoundRecorderStopCallback    OnStop,
                        void*                          UserData) :
    myStartCallback  (OnStart),
    myProcessCallback(OnProcess),
    myStopCallback   (OnStop),
    myUserData       (UserData)
    {
    }

private :

    virtual bool OnStart()
    {
        if (myStartCallback)
            return myStartCallback(myUserData) == sfTrue;
        else
            return true;
    }

    virtual bool OnProcessSamples(const sf::Int16* Samples, std::size_t SamplesCount)
    {
        if (myProcessCallback)
            return myProcessCallback(Samples, SamplesCount, myUserData) == sfTrue;
        else
            return true;
    }

    virtual void OnStop()
    {
        if (myStopCallback)
            myStopCallback(myUserData);
    }

    sfSoundRecorderStartCallback   myStartCallback;
    sfSoundRecorderProcessCallback myProcessCallback;
    sfSoundRecorderStopCallback    myStopCallback;
    void*                          myUserData;
};


////////////////////////////////////////////////////////////
// Internal structure of sfPacket
////////////////////////////////////////////////////////////
struct sfSoundRecorder
{
    sfSoundRecorder(sfSoundRecorderStartCallback   OnStart,
                    sfSoundRecorderProcessCallback OnProcess,
                    sfSoundRecorderStopCallback    OnStop,
                    void*                          UserData) :
    This(OnStart, OnProcess, OnStop, UserData)
    {
    }

    sfSoundRecorderImpl This;
};


#endif // SFML_SOUNDRECORDERSTRUCT_H
