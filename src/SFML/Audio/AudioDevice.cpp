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
#include <SFML/Audio/AudioDevice.hpp>
#include <SFML/Audio/AudioResource.hpp>
#include <SFML/Audio/Listener.hpp>
#include <algorithm>
#include <iostream>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
// Static member data
////////////////////////////////////////////////////////////
AudioDevice* AudioDevice::ourInstance;


////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
AudioDevice::AudioDevice() :
myRefCount(0)
{
    // Create the device
    myDevice = alcOpenDevice(NULL);

    if (myDevice)
    {
        // Create the context
        myContext = alcCreateContext(myDevice, NULL);

        if (myContext)
        {
            // Set the context as the current one (we'll only need one)
            alcMakeContextCurrent(myContext);

            // Initialize the listener, located at the origin and looking along the Z axis
            Listener::SetPosition(0.f, 0.f, 0.f);
            Listener::SetTarget(0.f, 0.f, -1.f);
        }
        else
        {
            std::cerr << "Failed to create the audio context" << std::endl;
        }
    }
    else
    {
        std::cerr << "Failed to open the audio device" << std::endl;
    }
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
AudioDevice::~AudioDevice()
{
    // Destroy the context
    alcMakeContextCurrent(NULL);
    if (myContext)
        alcDestroyContext(myContext);
    
    // Destroy the device
    if (myDevice)
        alcCloseDevice(myDevice);
}


////////////////////////////////////////////////////////////
/// Get the unique instance of the class
////////////////////////////////////////////////////////////
AudioDevice& AudioDevice::GetInstance()
{
    // Create the audio device if it doesn't exist
    if (!ourInstance)
        ourInstance = new AudioDevice;

    return *ourInstance;
}


////////////////////////////////////////////////////////////
/// Add a reference to the audio device
////////////////////////////////////////////////////////////
void AudioDevice::AddReference()
{
    // Create the audio device if it doesn't exist
    if (!ourInstance)
        ourInstance = new AudioDevice;

    // Increase the references count
    ourInstance->myRefCount++;
}


////////////////////////////////////////////////////////////
/// Remove a reference to the audio device
////////////////////////////////////////////////////////////
void AudioDevice::RemoveReference()
{
    // Decrease the references count
    ourInstance->myRefCount--;

    // Destroy the audio device if the references count reaches 0
    if (ourInstance->myRefCount == 0)
    {
        delete ourInstance;
        ourInstance = NULL;
    }
}


////////////////////////////////////////////////////////////
/// Get the OpenAL audio device
////////////////////////////////////////////////////////////
ALCdevice* AudioDevice::GetDevice() const
{
    return myDevice;
}


////////////////////////////////////////////////////////////
/// Get the OpenAL format that matches the given number of channels
////////////////////////////////////////////////////////////
ALenum AudioDevice::GetFormatFromChannelsCount(unsigned int ChannelsCount) const
{
    // Find the good format according to the number of channels
    switch (ChannelsCount)
    {
        case 1 : return AL_FORMAT_MONO16;
        case 2 : return AL_FORMAT_STEREO16;
        case 4 : return alGetEnumValue("AL_FORMAT_QUAD16");
        case 6 : return alGetEnumValue("AL_FORMAT_51CHN16");
        case 7 : return alGetEnumValue("AL_FORMAT_61CHN16");
        case 8 : return alGetEnumValue("AL_FORMAT_71CHN16");
    }

    return 0;
}

} // namespace priv

} // namespace sf
