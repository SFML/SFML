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
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>

// Windows' HWND is a type alias for struct HWND__*
#if defined(SFML_SYSTEM_WINDOWS)
struct HWND__; // NOLINT(bugprone-reserved-identifier)
#endif

namespace sf
{
#if defined(SFML_SYSTEM_WINDOWS)

// Window handle is HWND (HWND__*) on Windows
using WindowHandle = HWND__*;

#elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD) || defined(SFML_SYSTEM_OPENBSD) || \
    defined(SFML_SYSTEM_NETBSD)

// Window handle is Window (unsigned long) on Unix - X11
using WindowHandle = unsigned long;

#elif defined(SFML_SYSTEM_MACOS)

// Window handle is NSWindow or NSView (void*) on Mac OS X - Cocoa
using WindowHandle = void*;

#elif defined(SFML_SYSTEM_IOS)

// Window handle is UIWindow (void*) on iOS - UIKit
using WindowHandle = void*;

#elif defined(SFML_SYSTEM_ANDROID)

// Window handle is ANativeWindow* (void*) on Android
using WindowHandle = void*;

#elif defined(SFML_DOXYGEN)

// Define type alias symbol so that Doxygen can attach some documentation to it
using WindowHandle = "platform-specific";

#endif

} // namespace sf


////////////////////////////////////////////////////////////
/// \typedef sf::WindowHandle
/// \ingroup window
///
/// Define a low-level window handle type, specific to
/// each platform.
///
/// Platform        | Type
/// ----------------|------------------------------------------------------------
/// Windows         | \p HWND
/// Linux/FreeBSD   | \p %Window
/// Mac OS X        | either \p NSWindow* or \p NSView*, disguised as \p void*
/// iOS             | \p UIWindow*
/// Android         | \p ANativeWindow*
///
/// \par Mac OS X Specification
///
/// On Mac OS X, a sf::Window can be created either from an
/// existing \p NSWindow* or an \p NSView*. When the window
/// is created from a window, SFML will use its content view
/// as the OpenGL area. sf::Window::getSystemHandle() will
/// return the handle that was used to create the window,
/// which is a \p NSWindow* by default.
///
////////////////////////////////////////////////////////////
