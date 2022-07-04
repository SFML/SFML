////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef SFML_SHAREDDISPLAY_HPP
#define SFML_SHAREDDISPLAY_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/WindowStyle.hpp> // Prevent conflict with macro None from Xlib

#include <X11/Xlib.h>

#include <string>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Get the shared Display
///
/// This function increments the reference count of the display,
/// it must be matched with a call to CloseDisplay.
///
/// \return Pointer to the shared display
///
////////////////////////////////////////////////////////////
Display* OpenDisplay();

////////////////////////////////////////////////////////////
/// \brief Release a reference to the shared display
///
/// \param display Display to release
///
////////////////////////////////////////////////////////////
void CloseDisplay(Display* display);

////////////////////////////////////////////////////////////
/// \brief Get the shared XIM context for the Display
///
/// This function increments the reference count of the XIM context,
/// it must be matched with a call to CloseXIM.
///
/// It must be called with a display already opened.
///
/// \return XIM handle (a pointer) of the context
///
////////////////////////////////////////////////////////////
XIM OpenXIM();

////////////////////////////////////////////////////////////
/// \brief Release a reference to the shared XIM context
///
/// \param xim XIM context to release
///
////////////////////////////////////////////////////////////
void CloseXIM(XIM xim);

////////////////////////////////////////////////////////////
/// \brief Get the atom with the specified name
///
/// \param name         Name of the atom
/// \param onlyIfExists Don't try to create the atom if it doesn't already exist
///
/// \return Atom if it exists or None (0) if it doesn't
///
////////////////////////////////////////////////////////////
Atom getAtom(const std::string& name, bool onlyIfExists = false);

} // namespace priv

} // namespace sf


#endif // SFML_SHAREDDISPLAY_HPP
