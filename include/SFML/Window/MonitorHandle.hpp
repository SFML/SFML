////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2026 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter this and redistribute it freely,
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
// 3. This notice may not be removed or altered from this source distribution.
//
////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>

#include <cstdint>

// Windows' HMONITOR is a type alias for struct HMONITOR__*
#if defined(SFML_SYSTEM_WINDOWS)
struct HMONITOR__; // NOLINT(bugprone-reserved-identifier)
#endif

namespace sf
{
#if defined(SFML_SYSTEM_WINDOWS)

// Monitor handle is HMONITOR (HMONITOR__*) on Windows
using MonitorHandle = HMONITOR__*;

#elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD) || defined(SFML_SYSTEM_OPENBSD) || \
    defined(SFML_SYSTEM_NETBSD)

// Monitor handle is RROutput (an XRandR XID, i.e. unsigned long) on Unix - X11
using MonitorHandle = unsigned long;

#elif defined(SFML_SYSTEM_MACOS)

// Monitor handle is CGDirectDisplayID (std::uint32_t) on macOS
using MonitorHandle = std::uint32_t;

#elif defined(SFML_SYSTEM_IOS)

// Monitor handle is UIScreen* (void*) on iOS - UIKit
using MonitorHandle = void*;

#elif defined(SFML_SYSTEM_ANDROID)

// Android does not expose a native per-display handle; always null
using MonitorHandle = void*;

#elif defined(SFML_DOXYGEN)

// Define type alias symbol so that Doxygen can attach some documentation to it
using MonitorHandle = "platform-specific";

#endif

} // namespace sf


////////////////////////////////////////////////////////////
/// \typedef sf::MonitorHandle
/// \ingroup window
///
/// Low-level monitor handle type, specific to each platform.
///
/// Platform        | Type
/// ----------------|------------------------------------------------------------
/// Windows         | \p HMONITOR
/// Linux/FreeBSD   | \p RROutput
/// macOS           | \p CGDirectDisplayID
/// iOS             | \p UIScreen*
/// Android         | always null, no native per-display handle is exposed
///
/// This handle is only meaningful for the lifetime of the `sf::Monitor`
/// snapshot it was obtained from: like the rest of `sf::Monitor`, it is not
/// updated if the display configuration changes afterward, and on some
/// platforms (notably Windows' `HMONITOR`) the OS itself does not guarantee
/// the handle stays valid or even refers to the same monitor across
/// unrelated display reconfiguration events. Don't cache it, don't compare
/// it for equality between separately-obtained monitors (use
/// `Monitor::getIdentifier()`/`operator==` for that), and re-fetch it via a
/// fresh `Monitor` if you need it after any topology change.
///
////////////////////////////////////////////////////////////
