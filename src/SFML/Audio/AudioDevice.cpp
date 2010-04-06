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
#include <SFML/Audio/ALCheck.hpp>
#include <SFML/Audio/Listener.hpp>
#include <SFML/System/Err.hpp>


////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace 
{
    ALCdevice*  audioDevice  = NULL;
    ALCcontext* audioContext = NULL;
}

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
AudioDevice::AudioDevice()
{
    // Create the device
    audioDevice = alcOpenDevice(NULL);

    if (audioDevice)
    {
        // Create the context
        audioContext = alcCreateContext(audioDevice, NULL);

        if (audioContext)
        {
            // Set the context as the current one (we'll only need one)
            alcMakeContextCurrent(audioContext);
        }
        else
        {
            Err() << "Failed to create the audio context" << std::endl;
        }
    }
    else
    {
        Err() << "Failed to open the audio device" << std::endl;
    }
}


////////////////////////////////////////////////////////////
AudioDevice::~AudioDevice()
{
    // Destroy the context
    alcMakeContextCurrent(NULL);
    if (audioContext)
        alcDestroyContext(audioContext);

    // Destroy the device
    if (audioDevice)
        alcCloseDevice(audioDevice);
}


////////////////////////////////////////////////////////////
bool AudioDevice::IsExtensionSupported(const std::string& extension)
{
    EnsureALInit();

    if ((extension.length() > 2) && (extension.substr(0, 3) == "ALC"))
        return alcIsExtensionPresent(audioDevice, extension.c_str()) != AL_FALSE;
    else
        return alIsExtensionPresent(extension.c_str()) != AL_FALSE;
}


////////////////////////////////////////////////////////////
int AudioDevice::GetFormatFromChannelsCount(unsigned int channelsCount)
{
    EnsureALInit();

    // Find the good format according to the number of channels
    switch (channelsCount)
    {
        case 1  : return AL_FORMAT_MONO16;
        case 2  : return AL_FORMAT_STEREO16;
        case 4  : return alGetEnumValue("AL_FORMAT_QUAD16");
        case 6  : return alGetEnumValue("AL_FORMAT_51CHN16");
        case 7  : return alGetEnumValue("AL_FORMAT_61CHN16");
        case 8  : return alGetEnumValue("AL_FORMAT_71CHN16");
        default : return 0;
    }
}

} // namespace priv

} // namespace sf
