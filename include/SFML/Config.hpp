////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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

#pragma once


////////////////////////////////////////////////////////////
// Define the SFML version
////////////////////////////////////////////////////////////
#define SFML_VERSION_MAJOR      3
#define SFML_VERSION_MINOR      0
#define SFML_VERSION_PATCH      0
#define SFML_VERSION_IS_RELEASE false


////////////////////////////////////////////////////////////
// Identify the operating system
// see https://sourceforge.net/p/predef/wiki/Home/
////////////////////////////////////////////////////////////
#if defined(_WIN32)

// Windows
#define SFML_SYSTEM_WINDOWS
#ifndef NOMINMAX
#define NOMINMAX
#endif

#elif defined(__APPLE__) && defined(__MACH__)

// Apple platform, see which one it is
#include "TargetConditionals.h"

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR

// iOS
#define SFML_SYSTEM_IOS

#elif TARGET_OS_MAC

// MacOS
#define SFML_SYSTEM_MACOS

#else

// Unsupported Apple system
#error This Apple operating system is not supported by SFML library

#endif

#elif defined(__unix__)

// UNIX system, see which one it is
#if defined(__ANDROID__)

// Android
#define SFML_SYSTEM_ANDROID

#elif defined(__linux__)

// Linux
#define SFML_SYSTEM_LINUX

#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)

// FreeBSD
#define SFML_SYSTEM_FREEBSD

#elif defined(__OpenBSD__)

// OpenBSD
#define SFML_SYSTEM_OPENBSD

#elif defined(__NetBSD__)

// NetBSD
#define SFML_SYSTEM_NETBSD

#else

// Unsupported UNIX system
#error This UNIX operating system is not supported by SFML library

#endif

#else

// Unsupported system
#error This operating system is not supported by SFML library

#endif


////////////////////////////////////////////////////////////
// Define a portable debug macro
////////////////////////////////////////////////////////////
#if !defined(NDEBUG)

#define SFML_DEBUG

#endif


////////////////////////////////////////////////////////////
// Define helpers to create portable import / export macros for each module
////////////////////////////////////////////////////////////
#if !defined(SFML_STATIC)

#if defined(SFML_SYSTEM_WINDOWS)

// Windows compilers need specific (and different) keywords for export and import
#define SFML_API_EXPORT __declspec(dllexport)
#define SFML_API_IMPORT __declspec(dllimport)

// For Visual C++ compilers, we also need to turn off this annoying C4251 warning
#ifdef _MSC_VER

#pragma warning(disable : 4251)

#endif

#else // Linux, FreeBSD, Mac OS X

#define SFML_API_EXPORT __attribute__((__visibility__("default")))
#define SFML_API_IMPORT __attribute__((__visibility__("default")))

#endif

#else

// Static build doesn't need import/export macros
#define SFML_API_EXPORT
#define SFML_API_IMPORT

#endif
