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

#ifndef SFML_CONFIG_H
#define SFML_CONFIG_H


////////////////////////////////////////////////////////////
// Define the CSFML version
////////////////////////////////////////////////////////////
#define CSFML_VERSION_MAJOR 2
#define CSFML_VERSION_MINOR 0


////////////////////////////////////////////////////////////
// Identify the operating system
////////////////////////////////////////////////////////////
#if defined(_WIN32) || defined(__WIN32__)

    // Windows
    #define CSFML_SYSTEM_WINDOWS

#elif defined(linux) || defined(__linux)

    // Linux
    #define CSFML_SYSTEM_LINUX

#elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)

    // MacOS
    #define CSFML_SYSTEM_MACOS

#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)

    // FreeBSD
    #define CSFML_SYSTEM_FREEBSD

#else

    // Unsupported system
    #error This operating system is not supported by SFML library

#endif


////////////////////////////////////////////////////////////
// Define portable import / export macros
////////////////////////////////////////////////////////////
#if defined(CSFML_SYSTEM_WINDOWS)

    #ifdef CSFML_EXPORTS

        // From DLL side, we must export
        #define CSFML_API extern "C" __declspec(dllexport)

    #else

        // From client application side, we must import
        #define CSFML_API extern __declspec(dllimport)

    #endif

#else

    #ifdef CSFML_EXPORTS

        #define CSFML_API extern "C"

    #else

        #define CSFML_API extern

    #endif

#endif


////////////////////////////////////////////////////////////
// Define a portable boolean type
////////////////////////////////////////////////////////////
typedef int sfBool;
#define sfFalse 0
#define sfTrue  1


////////////////////////////////////////////////////////////
// Define portable types
////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <limits.h>

// 8 bits integer types
#if UCHAR_MAX == 0xFF
    typedef signed   char sfInt8;
    typedef unsigned char sfUint8;
#else
    #error No 8 bits integer type for this platform
#endif

// 16 bits integer types
#if USHRT_MAX == 0xFFFF
    typedef signed   short sfInt16;
    typedef unsigned short sfUint16;
#elif UINT_MAX == 0xFFFF
    typedef signed   int sfInt16;
    typedef unsigned int sfUint16;
#elif ULONG_MAX == 0xFFFF
    typedef signed   long sfInt16;
    typedef unsigned long sfUint16;
#else
    #error No 16 bits integer type for this platform
#endif

// 32 bits integer types
#if USHRT_MAX == 0xFFFFFFFF
    typedef signed   short sfInt32;
    typedef unsigned short sfUint32;
#elif UINT_MAX == 0xFFFFFFFF
    typedef signed   int sfInt32;
    typedef unsigned int sfUint32;
#elif ULONG_MAX == 0xFFFFFFFF
    typedef signed   long sfInt32;
    typedef unsigned long sfUint32;
#else
    #error No 32 bits integer type for this platform
#endif


#endif // SFML_CONFIG_H
