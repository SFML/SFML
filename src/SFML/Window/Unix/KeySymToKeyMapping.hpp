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
#include <SFML/Window/Keyboard.hpp> // sf::Keyboard::Key

#include <X11/X.h> // KeySym

namespace sf::priv
{
////////////////////////////////////////////////////////////
/// \brief Convert X11 KeySym to sf::Keyboard::Key
///
/// \param symbol X11 KeySym
///
/// \return The corresponding sf::Keyboard::Key
///
////////////////////////////////////////////////////////////
Keyboard::Key keySymToKey(KeySym symbol);


////////////////////////////////////////////////////////////
/// \brief Convert sf::Keyboard::Key to X11 KeySym
///
/// \param key X11 sf::Keyboard::Key
///
/// \return The corresponding X11 KeySym
///
////////////////////////////////////////////////////////////
KeySym keyToKeySym(Keyboard::Key key);

} // namespace sf::priv
