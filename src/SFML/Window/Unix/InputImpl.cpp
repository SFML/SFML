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
#include <SFML/Window/Unix/Display.hpp>
#include <SFML/Window/Unix/InputImpl.hpp>
#include <SFML/Window/Unix/KeyboardImpl.hpp>
#include <SFML/Window/WindowBase.hpp>
#include <SFML/Window/WindowHandle.hpp>

#include <SFML/System/String.hpp>

#include <X11/Xlib.h>
#include <X11/keysym.h>


namespace sf::priv
{
////////////////////////////////////////////////////////////
bool InputImpl::isKeyPressed(Keyboard::Key key)
{
    return KeyboardImpl::isKeyPressed(key);
}


////////////////////////////////////////////////////////////
bool InputImpl::isKeyPressed(Keyboard::Scancode code)
{
    return KeyboardImpl::isKeyPressed(code);
}


////////////////////////////////////////////////////////////
Keyboard::Key InputImpl::localize(Keyboard::Scancode code)
{
    return KeyboardImpl::localize(code);
}


////////////////////////////////////////////////////////////
Keyboard::Scancode InputImpl::delocalize(Keyboard::Key key)
{
    return KeyboardImpl::delocalize(key);
}


////////////////////////////////////////////////////////////
String InputImpl::getDescription(Keyboard::Scancode code)
{
    return KeyboardImpl::getDescription(code);
}


////////////////////////////////////////////////////////////
void InputImpl::setVirtualKeyboardVisible(bool /*visible*/)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
bool InputImpl::isMouseButtonPressed(Mouse::Button button)
{
    // Open a connection with the X server
    Display* display = openDisplay();

    // we don't care about these but they are required
    ::Window root;
    ::Window child;
    int      wx;
    int      wy;
    int      gx;
    int      gy;

    unsigned int buttons = 0;
    XQueryPointer(display, DefaultRootWindow(display), &root, &child, &gx, &gy, &wx, &wy, &buttons);

    // Close the connection with the X server
    closeDisplay(display);

    // Buttons 4 and 5 are the vertical wheel and 6 and 7 the horizontal wheel.
    // There is no mask for buttons 8 and 9, so checking the state of buttons
    // Mouse::XButton1 and Mouse::XButton2 is not supported.
    // clang-format off
    switch (button)
    {
        case Mouse::Left:     return buttons & Button1Mask;
        case Mouse::Right:    return buttons & Button3Mask;
        case Mouse::Middle:   return buttons & Button2Mask;
        case Mouse::XButton1: return false; // not supported by X
        case Mouse::XButton2: return false; // not supported by X
        default:              return false;
    }
    // clang-format on
}


////////////////////////////////////////////////////////////
Vector2i InputImpl::getMousePosition()
{
    // Open a connection with the X server
    Display* display = openDisplay();

    // we don't care about these but they are required
    ::Window     root;
    ::Window     child;
    int          x;
    int          y;
    unsigned int buttons;

    int gx = 0;
    int gy = 0;
    XQueryPointer(display, DefaultRootWindow(display), &root, &child, &gx, &gy, &x, &y, &buttons);

    // Close the connection with the X server
    closeDisplay(display);

    return Vector2i(gx, gy);
}


////////////////////////////////////////////////////////////
Vector2i InputImpl::getMousePosition(const WindowBase& relativeTo)
{
    WindowHandle handle = relativeTo.getSystemHandle();
    if (handle)
    {
        // Open a connection with the X server
        Display* display = openDisplay();

        // we don't care about these but they are required
        ::Window     root;
        ::Window     child;
        int          gx;
        int          gy;
        unsigned int buttons;

        int x = 0;
        int y = 0;
        XQueryPointer(display, handle, &root, &child, &gx, &gy, &x, &y, &buttons);

        // Close the connection with the X server
        closeDisplay(display);

        return Vector2i(x, y);
    }
    else
    {
        return Vector2i();
    }
}


////////////////////////////////////////////////////////////
void InputImpl::setMousePosition(const Vector2i& position)
{
    // Open a connection with the X server
    Display* display = openDisplay();

    XWarpPointer(display, None, DefaultRootWindow(display), 0, 0, 0, 0, position.x, position.y);
    XFlush(display);

    // Close the connection with the X server
    closeDisplay(display);
}


////////////////////////////////////////////////////////////
void InputImpl::setMousePosition(const Vector2i& position, const WindowBase& relativeTo)
{
    // Open a connection with the X server
    Display* display = openDisplay();

    WindowHandle handle = relativeTo.getSystemHandle();
    if (handle)
    {
        XWarpPointer(display, None, handle, 0, 0, 0, 0, position.x, position.y);
        XFlush(display);
    }

    // Close the connection with the X server
    closeDisplay(display);
}


////////////////////////////////////////////////////////////
bool InputImpl::isTouchDown(unsigned int /*finger*/)
{
    // Not applicable
    return false;
}


////////////////////////////////////////////////////////////
Vector2i InputImpl::getTouchPosition(unsigned int /*finger*/)
{
    // Not applicable
    return Vector2i();
}


////////////////////////////////////////////////////////////
Vector2i InputImpl::getTouchPosition(unsigned int /*finger*/, const WindowBase& /*relativeTo*/)
{
    // Not applicable
    return Vector2i();
}

} // namespace sf::priv
