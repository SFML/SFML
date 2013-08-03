////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Audio/ALCheck.hpp>
#include <SFML/Audio/AudioDevice.hpp>
#include <SFML/System/Err.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
void alCheckError(const std::string& file, unsigned int line)
{
    // Get the last error
    ALenum errorCode = alGetError();

    if (errorCode != AL_NO_ERROR)
    {
        std::string error, description;

        // Decode the error code
        switch (errorCode)
        {
            case AL_INVALID_NAME :
            {
                error = "AL_INVALID_NAME";
                description = "an unacceptable name has been specified";
                break;
            }

            case AL_INVALID_ENUM :
            {
                error = "AL_INVALID_ENUM";
                description = "an unacceptable value has been specified for an enumerated argument";
                break;
            }

            case AL_INVALID_VALUE :
            {
                error = "AL_INVALID_VALUE";
                description = "a numeric argument is out of range";
                break;
            }

            case AL_INVALID_OPERATION :
            {
                error = "AL_INVALID_OPERATION";
                description = "the specified operation is not allowed in the current state";
                break;
            }

            case AL_OUT_OF_MEMORY :
            {
                error = "AL_OUT_OF_MEMORY";
                description = "there is not enough memory left to execute the command";
                break;
            }
        }

        // Log the error
        err() << "An internal OpenAL call failed in "
              << file.substr(file.find_last_of("\\/") + 1) << " (" << line << ") : "
              << error << ", " << description
              << std::endl;
    }
}


////////////////////////////////////////////////////////////
/// Make sure that OpenAL is initialized
////////////////////////////////////////////////////////////
void ensureALInit()
{
    // The audio device is instanciated on demand rather than at global startup,
    // which solves a lot of weird crashes and errors.
    // It is destroyed at global exit which is fine.

    static AudioDevice globalDevice;
}

} // namespace priv

} // namespace sf
