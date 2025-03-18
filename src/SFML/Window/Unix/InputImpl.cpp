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
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/InputImpl.hpp>
#include <SFML/Window/Unix/Display.hpp>
#include <SFML/Window/Unix/KeyboardImpl.hpp>
#include <SFML/Window/WindowBase.hpp>
#include <SFML/Window/WindowHandle.hpp>

#include <SFML/System/String.hpp>

#include <X11/Xlib.h>
#include <X11/keysym.h>


namespace sf::priv::InputImpl
{
////////////////////////////////////////////////////////////
bool isKeyPressed(Keyboard::Key key)
{
    return KeyboardImpl::isKeyPressed(key);
}


////////////////////////////////////////////////////////////
bool isKeyPressed(Keyboard::Scancode code)
{
    return KeyboardImpl::isKeyPressed(code);
}


////////////////////////////////////////////////////////////
Keyboard::Key localize(Keyboard::Scancode code)
{
    return KeyboardImpl::localize(code);
}


////////////////////////////////////////////////////////////
Keyboard::Scancode delocalize(Keyboard::Key key)
{
    return KeyboardImpl::delocalize(key);
}


////////////////////////////////////////////////////////////
String getDescription(Keyboard::Scancode code)
{
    return KeyboardImpl::getDescription(code);
}


////////////////////////////////////////////////////////////
void setVirtualKeyboardVisible(bool /*visible*/)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
bool isMouseButtonPressed(Mouse::Button button)
{
    // Open a connection with the X server
    const auto display = openDisplay();

    // we don't care about these but they are required
    ::Window root  = 0;
    ::Window child = 0;
    int      wx    = 0;
    int      wy    = 0;
    int      gx    = 0;
    int      gy    = 0;

    unsigned int buttons = 0;
    XQueryPointer(display.get(), DefaultRootWindow(display.get()), &root, &child, &gx, &gy, &wx, &wy, &buttons);

    // Buttons 4 and 5 are the vertical wheel and 6 and 7 the horizontal wheel.
    // There is no mask for buttons 8 and 9, so checking the state of buttons
    // Mouse::Button::Extra1 and Mouse::Button::Extra2 is not supported.
    // clang-format off
    switch (button)
    {
        case Mouse::Button::Left:   return buttons & Button1Mask;
        case Mouse::Button::Right:  return buttons & Button3Mask;
        case Mouse::Button::Middle: return buttons & Button2Mask;
        case Mouse::Button::Extra1: return false; // not supported by X
        case Mouse::Button::Extra2: return false; // not supported by X
        default:                    return false;
    }
    // clang-format on
}


////////////////////////////////////////////////////////////
Vector2i getMousePosition()
{
    // Open a connection with the X server
    const auto display = openDisplay();

    // we don't care about these but they are required
    ::Window     root    = 0;
    ::Window     child   = 0;
    int          x       = 0;
    int          y       = 0;
    unsigned int buttons = 0;

    int gx = 0;
    int gy = 0;
    XQueryPointer(display.get(), DefaultRootWindow(display.get()), &root, &child, &gx, &gy, &x, &y, &buttons);

    return {gx, gy};
}


////////////////////////////////////////////////////////////
Vector2i getMousePosition(const WindowBase& relativeTo)
{
    if (const WindowHandle handle = relativeTo.getNativeHandle())
    {
        // Open a connection with the X server
        const auto display = openDisplay();

        // we don't care about these but they are required
        ::Window     root    = 0;
        ::Window     child   = 0;
        int          gx      = 0;
        int          gy      = 0;
        unsigned int buttons = 0;

        int x = 0;
        int y = 0;
        XQueryPointer(display.get(), handle, &root, &child, &gx, &gy, &x, &y, &buttons);

        return {x, y};
    }

    return {};
}


////////////////////////////////////////////////////////////
void setMousePosition(Vector2i position)
{
    // Open a connection with the X server
    const auto display = openDisplay();

    XWarpPointer(display.get(), None, DefaultRootWindow(display.get()), 0, 0, 0, 0, position.x, position.y);
    XFlush(display.get());
}


////////////////////////////////////////////////////////////
void setMousePosition(Vector2i position, const WindowBase& relativeTo)
{
    // Open a connection with the X server
    const auto display = openDisplay();

    if (const WindowHandle handle = relativeTo.getNativeHandle())
    {
        XWarpPointer(display.get(), None, handle, 0, 0, 0, 0, position.x, position.y);
        XFlush(display.get());
    }
}


////////////////////////////////////////////////////////////
bool isTouchDown(unsigned int /*finger*/)
{
    // Not applicable
    return false;
}


////////////////////////////////////////////////////////////
Vector2i getTouchPosition(unsigned int /*finger*/)
{
    // Not applicable
    return {};
}


////////////////////////////////////////////////////////////
Vector2i getTouchPosition(unsigned int /*finger*/, const WindowBase& /*relativeTo*/)
{
    // Not applicable
    return {};
}

} // namespace sf::priv::InputImpl
