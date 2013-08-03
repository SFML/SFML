////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Window/Linux/InputImpl.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/Linux/Display.hpp>
#include <X11/Xlib.h>
#include <X11/keysym.h>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
bool InputImpl::isKeyPressed(Keyboard::Key key)
{
    // Get the corresponding X11 keysym
    KeySym keysym = 0;
    switch (key)
    {
        case Keyboard::A:          keysym = XK_A;            break;
        case Keyboard::B:          keysym = XK_B;            break;
        case Keyboard::C:          keysym = XK_C;            break;
        case Keyboard::D:          keysym = XK_D;            break;
        case Keyboard::E:          keysym = XK_E;            break;
        case Keyboard::F:          keysym = XK_F;            break;
        case Keyboard::G:          keysym = XK_G;            break;
        case Keyboard::H:          keysym = XK_H;            break;
        case Keyboard::I:          keysym = XK_I;            break;
        case Keyboard::J:          keysym = XK_J;            break;
        case Keyboard::K:          keysym = XK_K;            break;
        case Keyboard::L:          keysym = XK_L;            break;
        case Keyboard::M:          keysym = XK_M;            break;
        case Keyboard::N:          keysym = XK_N;            break;
        case Keyboard::O:          keysym = XK_O;            break;
        case Keyboard::P:          keysym = XK_P;            break;
        case Keyboard::Q:          keysym = XK_Q;            break;
        case Keyboard::R:          keysym = XK_R;            break;
        case Keyboard::S:          keysym = XK_S;            break;
        case Keyboard::T:          keysym = XK_T;            break;
        case Keyboard::U:          keysym = XK_U;            break;
        case Keyboard::V:          keysym = XK_V;            break;
        case Keyboard::W:          keysym = XK_W;            break;
        case Keyboard::X:          keysym = XK_X;            break;
        case Keyboard::Y:          keysym = XK_Y;            break;
        case Keyboard::Z:          keysym = XK_Z;            break;
        case Keyboard::Num0:       keysym = XK_0;            break;
        case Keyboard::Num1:       keysym = XK_1;            break;
        case Keyboard::Num2:       keysym = XK_2;            break;
        case Keyboard::Num3:       keysym = XK_3;            break;
        case Keyboard::Num4:       keysym = XK_4;            break;
        case Keyboard::Num5:       keysym = XK_5;            break;
        case Keyboard::Num6:       keysym = XK_6;            break;
        case Keyboard::Num7:       keysym = XK_7;            break;
        case Keyboard::Num8:       keysym = XK_8;            break;
        case Keyboard::Num9:       keysym = XK_9;            break;
        case Keyboard::Escape:     keysym = XK_Escape;       break;
        case Keyboard::LControl:   keysym = XK_Control_L;    break;
        case Keyboard::LShift:     keysym = XK_Shift_L;      break;
        case Keyboard::LAlt:       keysym = XK_Alt_L;        break;
        case Keyboard::LSystem:    keysym = XK_Super_L;      break;
        case Keyboard::RControl:   keysym = XK_Control_R;    break;
        case Keyboard::RShift:     keysym = XK_Shift_R;      break;
        case Keyboard::RAlt:       keysym = XK_Alt_R;        break;
        case Keyboard::RSystem:    keysym = XK_Super_R;      break;
        case Keyboard::Menu:       keysym = XK_Menu;         break;
        case Keyboard::LBracket:   keysym = XK_bracketleft;  break;
        case Keyboard::RBracket:   keysym = XK_bracketright; break;
        case Keyboard::SemiColon:  keysym = XK_semicolon;    break;
        case Keyboard::Comma:      keysym = XK_comma;        break;
        case Keyboard::Period:     keysym = XK_period;       break;
        case Keyboard::Quote:      keysym = XK_dead_acute;   break;
        case Keyboard::Slash:      keysym = XK_slash;        break;
        case Keyboard::BackSlash:  keysym = XK_backslash;    break;
        case Keyboard::Tilde:      keysym = XK_dead_grave;   break;
        case Keyboard::Equal:      keysym = XK_equal;        break;
        case Keyboard::Dash:       keysym = XK_minus;        break;
        case Keyboard::Space:      keysym = XK_space;        break;
        case Keyboard::Return:     keysym = XK_Return;       break;
        case Keyboard::BackSpace:  keysym = XK_BackSpace;    break;
        case Keyboard::Tab:        keysym = XK_Tab;          break;
        case Keyboard::PageUp:     keysym = XK_Prior;        break;
        case Keyboard::PageDown:   keysym = XK_Next;         break;
        case Keyboard::End:        keysym = XK_End;          break;
        case Keyboard::Home:       keysym = XK_Home;         break;
        case Keyboard::Insert:     keysym = XK_Insert;       break;
        case Keyboard::Delete:     keysym = XK_Delete;       break;
        case Keyboard::Add:        keysym = XK_KP_Add;       break;
        case Keyboard::Subtract:   keysym = XK_KP_Subtract;  break;
        case Keyboard::Multiply:   keysym = XK_KP_Multiply;  break;
        case Keyboard::Divide:     keysym = XK_KP_Divide;    break;
        case Keyboard::Left:       keysym = XK_Left;         break;
        case Keyboard::Right:      keysym = XK_Right;        break;
        case Keyboard::Up:         keysym = XK_Up;           break;
        case Keyboard::Down:       keysym = XK_Down;         break;
        case Keyboard::Numpad0:    keysym = XK_KP_0;         break;
        case Keyboard::Numpad1:    keysym = XK_KP_1;         break;
        case Keyboard::Numpad2:    keysym = XK_KP_2;         break;
        case Keyboard::Numpad3:    keysym = XK_KP_3;         break;
        case Keyboard::Numpad4:    keysym = XK_KP_4;         break;
        case Keyboard::Numpad5:    keysym = XK_KP_5;         break;
        case Keyboard::Numpad6:    keysym = XK_KP_6;         break;
        case Keyboard::Numpad7:    keysym = XK_KP_7;         break;
        case Keyboard::Numpad8:    keysym = XK_KP_8;         break;
        case Keyboard::Numpad9:    keysym = XK_KP_9;         break;
        case Keyboard::F1:         keysym = XK_F1;           break;
        case Keyboard::F2:         keysym = XK_F2;           break;
        case Keyboard::F3:         keysym = XK_F3;           break;
        case Keyboard::F4:         keysym = XK_F4;           break;
        case Keyboard::F5:         keysym = XK_F5;           break;
        case Keyboard::F6:         keysym = XK_F6;           break;
        case Keyboard::F7:         keysym = XK_F7;           break;
        case Keyboard::F8:         keysym = XK_F8;           break;
        case Keyboard::F9:         keysym = XK_F9;           break;
        case Keyboard::F10:        keysym = XK_F10;          break;
        case Keyboard::F11:        keysym = XK_F11;          break;
        case Keyboard::F12:        keysym = XK_F12;          break;
        case Keyboard::F13:        keysym = XK_F13;          break;
        case Keyboard::F14:        keysym = XK_F14;          break;
        case Keyboard::F15:        keysym = XK_F15;          break;
        case Keyboard::Pause:      keysym = XK_Pause;        break;
        default:                   keysym = 0;               break;
    }

    // Open a connection with the X server
    Display* display = OpenDisplay();

    // Convert to keycode
    KeyCode keycode = XKeysymToKeycode(display, keysym);
    if (keycode != 0)
    {
        // Get the whole keyboard state
        char keys[32];
        XQueryKeymap(display, keys);

        // Close the connection with the X server
        CloseDisplay(display);

        // Check our keycode
        return (keys[keycode / 8] & (1 << (keycode % 8))) != 0;
    }
    else
    {
        // Close the connection with the X server
        CloseDisplay(display);

        return false;
    }
}


////////////////////////////////////////////////////////////
bool InputImpl::isMouseButtonPressed(Mouse::Button button)
{
    // Open a connection with the X server
    Display* display = OpenDisplay();

    // we don't care about these but they are required
    ::Window root, child;
    int wx, wy;
    int gx, gy;

    unsigned int buttons = 0;
    XQueryPointer(display, DefaultRootWindow(display), &root, &child, &gx, &gy, &wx, &wy, &buttons);

    // Close the connection with the X server
    CloseDisplay(display);

    switch (button)
    {
        case Mouse::Left:     return buttons & Button1Mask;
        case Mouse::Right:    return buttons & Button3Mask;
        case Mouse::Middle:   return buttons & Button2Mask;
        case Mouse::XButton1: return false; // not supported by X
        case Mouse::XButton2: return false; // not supported by X
        default:              return false;
    }

    return false;
}


////////////////////////////////////////////////////////////
Vector2i InputImpl::getMousePosition()
{
    // Open a connection with the X server
    Display* display = OpenDisplay();

    // we don't care about these but they are required
    ::Window root, child;
    int x, y;
    unsigned int buttons;

    int gx = 0;
    int gy = 0;
    XQueryPointer(display, DefaultRootWindow(display), &root, &child, &gx, &gy, &x, &y, &buttons);

    // Close the connection with the X server
    CloseDisplay(display);

    return Vector2i(gx, gy);
}


////////////////////////////////////////////////////////////
Vector2i InputImpl::getMousePosition(const Window& relativeTo)
{
    WindowHandle handle = relativeTo.getSystemHandle();
    if (handle)
    {
        // Open a connection with the X server
        Display* display = OpenDisplay();

        // we don't care about these but they are required
        ::Window root, child;
        int gx, gy;
        unsigned int buttons;

        int x = 0;
        int y = 0;
        XQueryPointer(display, handle, &root, &child, &gx, &gy, &x, &y, &buttons);

        // Close the connection with the X server
        CloseDisplay(display);

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
    Display* display = OpenDisplay();

    XWarpPointer(display, None, DefaultRootWindow(display), 0, 0, 0, 0, position.x, position.y);
    XFlush(display);

    // Close the connection with the X server
    CloseDisplay(display);
}


////////////////////////////////////////////////////////////
void InputImpl::setMousePosition(const Vector2i& position, const Window& relativeTo)
{
    // Open a connection with the X server
    Display* display = OpenDisplay();

    WindowHandle handle = relativeTo.getSystemHandle();
    if (handle)
    {
        XWarpPointer(display, None, handle, 0, 0, 0, 0, position.x, position.y);
        XFlush(display);
    }

    // Close the connection with the X server
    CloseDisplay(display);
}

} // namespace priv

} // namespace sf
