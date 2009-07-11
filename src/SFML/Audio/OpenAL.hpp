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

#ifndef SFML_OPENAL_HPP
#define SFML_OPENAL_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <iostream>
#include <string>

#if defined(SFML_SYSTEM_MACOS)
    #include <OpenAL/al.h>
    #include <OpenAL/alc.h>
#else
    #include <AL/al.h>
    #include <AL/alc.h>
#endif


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Let's define a macro to quickly check every OpenAL
/// API calls
///
////////////////////////////////////////////////////////////
#ifdef SFML_DEBUG

    // If in debug mode, perform a test on every call
    #define ALCheck(Func) ((Func), priv::ALCheckError(__FILE__, __LINE__))

#else

    // Else, we don't add any overhead
    #define ALCheck(Func) (Func)

#endif


////////////////////////////////////////////////////////////
/// Check last OpenAL error
///
////////////////////////////////////////////////////////////
inline void ALCheckError(const std::string& file, unsigned int line)
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
        std::cerr << "An internal OpenAL call failed in "
                  << file.substr(file.find_last_of("\\/") + 1) << " (" << line << ") : "
                  << error << ", " << description
                  << std::endl;
    }
}

} // namespace priv

} // namespace sf


#endif // SFML_OPENAL_HPP
