////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2017 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/Window.hpp> // important to be included first (conflict with None)
#include <SFML/Window/Unix/InputImpl.hpp>
#include <SFML/Window/Unix/Display.hpp>
#include <SFML/System/Err.hpp>
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
        case Keyboard::Key::LShift:     keysym = XK_Shift_L;      break;
        case Keyboard::Key::RShift:     keysym = XK_Shift_R;      break;
        case Keyboard::Key::LControl:   keysym = XK_Control_L;    break;
        case Keyboard::Key::RControl:   keysym = XK_Control_R;    break;
        case Keyboard::Key::LAlt:       keysym = XK_Alt_L;        break;
        case Keyboard::Key::RAlt:       keysym = XK_Alt_R;        break;
        case Keyboard::Key::LSystem:    keysym = XK_Super_L;      break;
        case Keyboard::Key::RSystem:    keysym = XK_Super_R;      break;
        case Keyboard::Key::Menu:       keysym = XK_Menu;         break;
        case Keyboard::Key::Escape:     keysym = XK_Escape;       break;
        case Keyboard::Key::SemiColon:  keysym = XK_semicolon;    break;
        case Keyboard::Key::Slash:      keysym = XK_slash;        break;
        case Keyboard::Key::Equal:      keysym = XK_equal;        break;
        case Keyboard::Key::Dash:       keysym = XK_minus;        break;
        case Keyboard::Key::LBracket:   keysym = XK_bracketleft;  break;
        case Keyboard::Key::RBracket:   keysym = XK_bracketright; break;
        case Keyboard::Key::Comma:      keysym = XK_comma;        break;
        case Keyboard::Key::Period:     keysym = XK_period;       break;
        case Keyboard::Key::Quote:      keysym = XK_apostrophe;   break;
        case Keyboard::Key::BackSlash:  keysym = XK_backslash;    break;
        case Keyboard::Key::Tilde:      keysym = XK_grave;        break;
        case Keyboard::Key::Space:      keysym = XK_space;        break;
        case Keyboard::Key::Return:     keysym = XK_Return;       break;
        case Keyboard::Key::BackSpace:  keysym = XK_BackSpace;    break;
        case Keyboard::Key::Tab:        keysym = XK_Tab;          break;
        case Keyboard::Key::PageUp:     keysym = XK_Prior;        break;
        case Keyboard::Key::PageDown:   keysym = XK_Next;         break;
        case Keyboard::Key::End:        keysym = XK_End;          break;
        case Keyboard::Key::Home:       keysym = XK_Home;         break;
        case Keyboard::Key::Insert:     keysym = XK_Insert;       break;
        case Keyboard::Key::Delete:     keysym = XK_Delete;       break;
        case Keyboard::Key::Add:        keysym = XK_KP_Add;       break;
        case Keyboard::Key::Subtract:   keysym = XK_KP_Subtract;  break;
        case Keyboard::Key::Multiply:   keysym = XK_KP_Multiply;  break;
        case Keyboard::Key::Divide:     keysym = XK_KP_Divide;    break;
        case Keyboard::Key::Pause:      keysym = XK_Pause;        break;
        case Keyboard::Key::F1:         keysym = XK_F1;           break;
        case Keyboard::Key::F2:         keysym = XK_F2;           break;
        case Keyboard::Key::F3:         keysym = XK_F3;           break;
        case Keyboard::Key::F4:         keysym = XK_F4;           break;
        case Keyboard::Key::F5:         keysym = XK_F5;           break;
        case Keyboard::Key::F6:         keysym = XK_F6;           break;
        case Keyboard::Key::F7:         keysym = XK_F7;           break;
        case Keyboard::Key::F8:         keysym = XK_F8;           break;
        case Keyboard::Key::F9:         keysym = XK_F9;           break;
        case Keyboard::Key::F10:        keysym = XK_F10;          break;
        case Keyboard::Key::F11:        keysym = XK_F11;          break;
        case Keyboard::Key::F12:        keysym = XK_F12;          break;
        case Keyboard::Key::F13:        keysym = XK_F13;          break;
        case Keyboard::Key::F14:        keysym = XK_F14;          break;
        case Keyboard::Key::F15:        keysym = XK_F15;          break;
        case Keyboard::Key::Left:       keysym = XK_Left;         break;
        case Keyboard::Key::Right:      keysym = XK_Right;        break;
        case Keyboard::Key::Up:         keysym = XK_Up;           break;
        case Keyboard::Key::Down:       keysym = XK_Down;         break;
        case Keyboard::Key::Numpad0:    keysym = XK_KP_Insert;    break;
        case Keyboard::Key::Numpad1:    keysym = XK_KP_End;       break;
        case Keyboard::Key::Numpad2:    keysym = XK_KP_Down;      break;
        case Keyboard::Key::Numpad3:    keysym = XK_KP_Page_Down; break;
        case Keyboard::Key::Numpad4:    keysym = XK_KP_Left;      break;
        case Keyboard::Key::Numpad5:    keysym = XK_KP_Begin;     break;
        case Keyboard::Key::Numpad6:    keysym = XK_KP_Right;     break;
        case Keyboard::Key::Numpad7:    keysym = XK_KP_Home;      break;
        case Keyboard::Key::Numpad8:    keysym = XK_KP_Up;        break;
        case Keyboard::Key::Numpad9:    keysym = XK_KP_Page_Up;   break;
        case Keyboard::Key::A:          keysym = XK_a;            break;
        case Keyboard::Key::B:          keysym = XK_b;            break;
        case Keyboard::Key::C:          keysym = XK_c;            break;
        case Keyboard::Key::D:          keysym = XK_d;            break;
        case Keyboard::Key::E:          keysym = XK_e;            break;
        case Keyboard::Key::F:          keysym = XK_f;            break;
        case Keyboard::Key::G:          keysym = XK_g;            break;
        case Keyboard::Key::H:          keysym = XK_h;            break;
        case Keyboard::Key::I:          keysym = XK_i;            break;
        case Keyboard::Key::J:          keysym = XK_j;            break;
        case Keyboard::Key::K:          keysym = XK_k;            break;
        case Keyboard::Key::L:          keysym = XK_l;            break;
        case Keyboard::Key::M:          keysym = XK_m;            break;
        case Keyboard::Key::N:          keysym = XK_n;            break;
        case Keyboard::Key::O:          keysym = XK_o;            break;
        case Keyboard::Key::P:          keysym = XK_p;            break;
        case Keyboard::Key::Q:          keysym = XK_q;            break;
        case Keyboard::Key::R:          keysym = XK_r;            break;
        case Keyboard::Key::S:          keysym = XK_s;            break;
        case Keyboard::Key::T:          keysym = XK_t;            break;
        case Keyboard::Key::U:          keysym = XK_u;            break;
        case Keyboard::Key::V:          keysym = XK_v;            break;
        case Keyboard::Key::W:          keysym = XK_w;            break;
        case Keyboard::Key::X:          keysym = XK_x;            break;
        case Keyboard::Key::Y:          keysym = XK_y;            break;
        case Keyboard::Key::Z:          keysym = XK_z;            break;
        case Keyboard::Key::Num0:       keysym = XK_0;            break;
        case Keyboard::Key::Num1:       keysym = XK_1;            break;
        case Keyboard::Key::Num2:       keysym = XK_2;            break;
        case Keyboard::Key::Num3:       keysym = XK_3;            break;
        case Keyboard::Key::Num4:       keysym = XK_4;            break;
        case Keyboard::Key::Num5:       keysym = XK_5;            break;
        case Keyboard::Key::Num6:       keysym = XK_6;            break;
        case Keyboard::Key::Num7:       keysym = XK_7;            break;
        case Keyboard::Key::Num8:       keysym = XK_8;            break;
        case Keyboard::Key::Num9:       keysym = XK_9;            break;
        default:                        keysym = 0;               break;
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
void InputImpl::setVirtualKeyboardVisible(bool /*visible*/)
{
    // Not applicable
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
        case Mouse::Button::Left:     return buttons & Button1Mask;
        case Mouse::Button::Right:    return buttons & Button3Mask;
        case Mouse::Button::Middle:   return buttons & Button2Mask;
        case Mouse::Button::XButton1: return false; // not supported by X
        case Mouse::Button::XButton2: return false; // not supported by X
        default:                      return false;
    }
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
Vector2i InputImpl::getTouchPosition(unsigned int /*finger*/, const Window& /*relativeTo*/)
{
    // Not applicable
    return Vector2i();
}

} // namespace priv

} // namespace sf
