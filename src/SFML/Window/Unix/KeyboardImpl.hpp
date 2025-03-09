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
#include <SFML/Window/Keyboard.hpp>

#include <X11/Xlib.h> // XKeyEvent


////////////////////////////////////////////////////////////
/// \brief sf::priv::KeyboardImpl helper
///
/// This class implements keyboard handling functions
/// to help sf::priv::InputImpl class.
////////////////////////////////////////////////////////////
namespace sf::priv::KeyboardImpl
{
////////////////////////////////////////////////////////////
/// \copydoc sf::Keyboard::isKeyPressed(Key)
///
////////////////////////////////////////////////////////////
bool isKeyPressed(Keyboard::Key key);

////////////////////////////////////////////////////////////
/// \copydoc sf::Keyboard::isKeyPressed(Scancode)
///
////////////////////////////////////////////////////////////
bool isKeyPressed(Keyboard::Scancode code);

////////////////////////////////////////////////////////////
/// \copydoc sf::Keyboard::localize
///
////////////////////////////////////////////////////////////
Keyboard::Scancode delocalize(Keyboard::Key key);

////////////////////////////////////////////////////////////
/// \copydoc sf::Keyboard::delocalize
///
////////////////////////////////////////////////////////////
Keyboard::Key localize(Keyboard::Scancode code);

////////////////////////////////////////////////////////////
/// \copydoc sf::Keyboard::getDescription
///
////////////////////////////////////////////////////////////
String getDescription(Keyboard::Scancode code);

////////////////////////////////////////////////////////////
/// \brief Get the sf::Keyboard::Key from XKeyEvent
///
/// \param event Event from which key is gotten
///
/// \return A key being pressed or released
///
////////////////////////////////////////////////////////////
Keyboard::Key getKeyFromEvent(XKeyEvent& event);

////////////////////////////////////////////////////////////
/// \brief Get the sf::Keyboard::Scancode from XKeyEvent
///
/// \param event Event from which scancode is gotten
///
/// \return A scancode of a key being pressed or released
///
////////////////////////////////////////////////////////////
Keyboard::Scancode getScancodeFromEvent(XKeyEvent& event);

} // namespace sf::priv::KeyboardImpl
