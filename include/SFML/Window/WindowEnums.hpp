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


namespace sf
{
namespace Style
{
////////////////////////////////////////////////////////////
/// \ingroup window
/// \brief Enumeration of the window styles
///
////////////////////////////////////////////////////////////
enum
{
    None     = 0,      //!< No border / title bar (this flag and all others are mutually exclusive)
    Titlebar = 1 << 0, //!< Title bar + fixed border
    Resize   = 1 << 1, //!< Title bar + resizable border + maximize button
    Close    = 1 << 2, //!< Title bar + close button

    Default = Titlebar | Resize | Close //!< Default window style
};

} // namespace Style

////////////////////////////////////////////////////////////
/// \ingroup window
/// \brief Enumeration of the window states
///
////////////////////////////////////////////////////////////
enum class State
{
    Windowed,  //!< Floating window
    Fullscreen //!< Fullscreen window
};

} // namespace sf
