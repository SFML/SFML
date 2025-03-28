////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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
// Module preamble
////////////////////////////////////////////////////////////

module;

#include <SFML/System.hpp>

export module sfml.system;

export import sfml.Config;

export namespace sf {
    using sf::Angle;
    using sf::Clock;
    using sf::Exception;
    using sf::FileInputStream;
    using sf::InputStream;
    using sf::MemoryInputStream;
    #ifdef SFML_SYSTEM_ANDROID
    using ::ANativeActivity;
    using sf::getNativeActivity;
    #endif
    using sf::U8StringCharTraits;
    using sf::String;
    using sf::U8String;
    using sf::SuspendAwareClock;
    using sf::Time;
    using sf::Utf;
    using sf::Utf8;
    using sf::Utf16;
    using sf::Utf32;
    using sf::Vector2;
    using sf::Vector2i;
    using sf::Vector2u;
    using sf::Vector2f;
    using sf::Vector3;
    using sf::Vector3i;
    using sf::Vector3f;

    using sf::degrees;
    using sf::radians;
    using sf::err;
    using sf::sleep;
}

////////////////////////////////////////////////////////////
/// \defgroup system System module
///
/// Base module of SFML, defining various utilities. It provides
/// vector classes, Unicode strings and conversion functions,
/// threads and mutexes, timing classes.
///
////////////////////////////////////////////////////////////
