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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>

#if defined(SFML_SYSTEM_WINDOWS)
#include <SFML/Window/Win32/ClipboardImpl.hpp>
#elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD) || defined(SFML_SYSTEM_OPENBSD) || \
    defined(SFML_SYSTEM_NETBSD)
#if defined(SFML_USE_DRM)
#include <SFML/Window/DRM/ClipboardImpl.hpp>
#else
#include <SFML/Window/Unix/ClipboardImpl.hpp>
#endif
#elif defined(SFML_SYSTEM_MACOS)
#include <SFML/Window/macOS/ClipboardImpl.hpp>
#elif defined(SFML_SYSTEM_IOS)
#include <SFML/Window/iOS/ClipboardImpl.hpp>
#elif defined(SFML_SYSTEM_ANDROID)
#include <SFML/Window/Android/ClipboardImpl.hpp>
#endif
