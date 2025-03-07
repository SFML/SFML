////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Marco Antognini (antognini.marco@gmail.com),
//                         Laurent Gomila (laurent@sfml-dev.org)
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
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Event.hpp>

#import <AppKit/AppKit.h>

namespace sf::priv
{
class WindowImplCocoa;
}

////////////////////////////////////////////////////////////
/// Keyboard Modifiers Helper
///
/// Handle left & right modifiers (cmd, ctrl, alt, shift)
/// events and send them back to the requester.
///
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
/// \brief Initialize the global state (only if needed)
///
/// It needs to be called before any event, e.g. in the window constructor.
///
////////////////////////////////////////////////////////////
void initialiseKeyboardHelper();


////////////////////////////////////////////////////////////
/// \brief Set up a SFML key pressed event based on the given modifiers flags and key code
///
////////////////////////////////////////////////////////////
sf::Event::KeyPressed keyPressedEventWithModifiers(NSUInteger modifiers, sf::Keyboard::Key key, sf::Keyboard::Scancode code);


////////////////////////////////////////////////////////////
/// \brief Set up a SFML key released event based on the given modifiers flags and key code
///
////////////////////////////////////////////////////////////
sf::Event::KeyReleased keyReleasedEventWithModifiers(NSUInteger modifiers, sf::Keyboard::Key key, sf::Keyboard::Scancode code);


////////////////////////////////////////////////////////////
/// \brief Handle the state of modifiers keys
///
/// Send key released & pressed events to the requester.
///
////////////////////////////////////////////////////////////
void handleModifiersChanged(NSUInteger modifiers, sf::priv::WindowImplCocoa& requester);
