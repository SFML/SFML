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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/InputImpl.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/iOS/SFAppDelegate.hpp>

#include <SFML/System/Err.hpp>


namespace sf::priv::InputImpl
{
////////////////////////////////////////////////////////////
bool isKeyPressed(Keyboard::Key /* key */)
{
    // Not applicable
    return false;
}


////////////////////////////////////////////////////////////
bool isKeyPressed(Keyboard::Scancode /* codes */)
{
    // Not applicable
    return false;
}


////////////////////////////////////////////////////////////
Keyboard::Key localize(Keyboard::Scancode /* code */)
{
    // Not applicable
    return Keyboard::Unknown;
}


////////////////////////////////////////////////////////////
Keyboard::Scancode delocalize(Keyboard::Key /* key */)
{
    // Not applicable
    return Keyboard::Scan::Unknown;
}


////////////////////////////////////////////////////////////
String getDescription(Keyboard::Scancode /* code */)
{
    // Not applicable
    return "";
}


////////////////////////////////////////////////////////////
void setVirtualKeyboardVisible(bool visible)
{
    [[SFAppDelegate getInstance] setVirtualKeyboardVisible:visible];
}


////////////////////////////////////////////////////////////
bool isMouseButtonPressed(Mouse::Button /* button */)
{
    // Not applicable
    return false;
}


////////////////////////////////////////////////////////////
Vector2i getMousePosition()
{
    return {};
}


////////////////////////////////////////////////////////////
Vector2i getMousePosition(const WindowBase& /* relativeTo */)
{
    return getMousePosition();
}


////////////////////////////////////////////////////////////
void setMousePosition(const Vector2i& /* position */)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void setMousePosition(const Vector2i& /* position */, const WindowBase& /* relativeTo */)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
bool isTouchDown(unsigned int finger)
{
    return [[SFAppDelegate getInstance] getTouchPosition:finger] != Vector2i(-1, -1);
}


////////////////////////////////////////////////////////////
Vector2i getTouchPosition(unsigned int finger)
{
    return [[SFAppDelegate getInstance] getTouchPosition:finger];
}


////////////////////////////////////////////////////////////
Vector2i getTouchPosition(unsigned int finger, const WindowBase& /* relativeTo */)
{
    return getTouchPosition(finger);
}

} // namespace sf::priv
