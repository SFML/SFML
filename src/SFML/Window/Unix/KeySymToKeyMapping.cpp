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
#include <SFML/Window/Unix/KeySymToKeyMapping.hpp>

#include <X11/keysym.h>

namespace sf::priv
{

////////////////////////////////////////////////////////////
Keyboard::Key keySymToKey(KeySym symbol)
{
    // clang-format off
    switch (symbol)
    {
        case XK_Shift_L:      return Keyboard::Key::LShift;
        case XK_Shift_R:      return Keyboard::Key::RShift;
        case XK_Control_L:    return Keyboard::Key::LControl;
        case XK_Control_R:    return Keyboard::Key::RControl;
        case XK_Alt_L:        return Keyboard::Key::LAlt;
        case XK_ISO_Level3_Shift:
        case XK_Alt_R:        return Keyboard::Key::RAlt;
        case XK_Super_L:      return Keyboard::Key::LSystem;
        case XK_Super_R:      return Keyboard::Key::RSystem;
        case XK_Menu:         return Keyboard::Key::Menu;
        case XK_Escape:       return Keyboard::Key::Escape;
        case XK_semicolon:    return Keyboard::Key::Semicolon;
        case XK_slash:        return Keyboard::Key::Slash;
        case XK_equal:        return Keyboard::Key::Equal;
        case XK_minus:        return Keyboard::Key::Hyphen;
        case XK_bracketleft:  return Keyboard::Key::LBracket;
        case XK_bracketright: return Keyboard::Key::RBracket;
        case XK_comma:        return Keyboard::Key::Comma;
        case XK_period:       return Keyboard::Key::Period;
        case XK_apostrophe:   return Keyboard::Key::Apostrophe;
        case XK_backslash:    return Keyboard::Key::Backslash;
        case XK_grave:        return Keyboard::Key::Grave;
        case XK_space:        return Keyboard::Key::Space;
        case XK_Return:       return Keyboard::Key::Enter;
        case XK_KP_Enter:     return Keyboard::Key::Enter;
        case XK_BackSpace:    return Keyboard::Key::Backspace;
        case XK_Tab:          return Keyboard::Key::Tab;
        case XK_Prior:        return Keyboard::Key::PageUp;
        case XK_Next:         return Keyboard::Key::PageDown;
        case XK_End:          return Keyboard::Key::End;
        case XK_Home:         return Keyboard::Key::Home;
        case XK_Insert:       return Keyboard::Key::Insert;
        case XK_Delete:       return Keyboard::Key::Delete;
        case XK_KP_Add:       return Keyboard::Key::Add;
        case XK_KP_Subtract:  return Keyboard::Key::Subtract;
        case XK_KP_Multiply:  return Keyboard::Key::Multiply;
        case XK_KP_Divide:    return Keyboard::Key::Divide;
        case XK_KP_Delete:    return Keyboard::Key::Period;
        case XK_Pause:        return Keyboard::Key::Pause;
        case XK_F1:           return Keyboard::Key::F1;
        case XK_F2:           return Keyboard::Key::F2;
        case XK_F3:           return Keyboard::Key::F3;
        case XK_F4:           return Keyboard::Key::F4;
        case XK_F5:           return Keyboard::Key::F5;
        case XK_F6:           return Keyboard::Key::F6;
        case XK_F7:           return Keyboard::Key::F7;
        case XK_F8:           return Keyboard::Key::F8;
        case XK_F9:           return Keyboard::Key::F9;
        case XK_F10:          return Keyboard::Key::F10;
        case XK_F11:          return Keyboard::Key::F11;
        case XK_F12:          return Keyboard::Key::F12;
        case XK_F13:          return Keyboard::Key::F13;
        case XK_F14:          return Keyboard::Key::F14;
        case XK_F15:          return Keyboard::Key::F15;
        case XK_Left:         return Keyboard::Key::Left;
        case XK_Right:        return Keyboard::Key::Right;
        case XK_Up:           return Keyboard::Key::Up;
        case XK_Down:         return Keyboard::Key::Down;
        case XK_KP_Insert:    return Keyboard::Key::Numpad0;
        case XK_KP_End:       return Keyboard::Key::Numpad1;
        case XK_KP_Down:      return Keyboard::Key::Numpad2;
        case XK_KP_Page_Down: return Keyboard::Key::Numpad3;
        case XK_KP_Left:      return Keyboard::Key::Numpad4;
        case XK_KP_Begin:     return Keyboard::Key::Numpad5;
        case XK_KP_Right:     return Keyboard::Key::Numpad6;
        case XK_KP_Home:      return Keyboard::Key::Numpad7;
        case XK_KP_Up:        return Keyboard::Key::Numpad8;
        case XK_KP_Page_Up:   return Keyboard::Key::Numpad9;
        case XK_a:            return Keyboard::Key::A;
        case XK_b:            return Keyboard::Key::B;
        case XK_c:            return Keyboard::Key::C;
        case XK_d:            return Keyboard::Key::D;
        case XK_e:            return Keyboard::Key::E;
        case XK_f:            return Keyboard::Key::F;
        case XK_g:            return Keyboard::Key::G;
        case XK_h:            return Keyboard::Key::H;
        case XK_i:            return Keyboard::Key::I;
        case XK_j:            return Keyboard::Key::J;
        case XK_k:            return Keyboard::Key::K;
        case XK_l:            return Keyboard::Key::L;
        case XK_m:            return Keyboard::Key::M;
        case XK_n:            return Keyboard::Key::N;
        case XK_o:            return Keyboard::Key::O;
        case XK_p:            return Keyboard::Key::P;
        case XK_q:            return Keyboard::Key::Q;
        case XK_r:            return Keyboard::Key::R;
        case XK_s:            return Keyboard::Key::S;
        case XK_t:            return Keyboard::Key::T;
        case XK_u:            return Keyboard::Key::U;
        case XK_v:            return Keyboard::Key::V;
        case XK_w:            return Keyboard::Key::W;
        case XK_x:            return Keyboard::Key::X;
        case XK_y:            return Keyboard::Key::Y;
        case XK_z:            return Keyboard::Key::Z;
        case XK_0:            return Keyboard::Key::Num0;
        case XK_1:            return Keyboard::Key::Num1;
        case XK_2:            return Keyboard::Key::Num2;
        case XK_3:            return Keyboard::Key::Num3;
        case XK_4:            return Keyboard::Key::Num4;
        case XK_5:            return Keyboard::Key::Num5;
        case XK_6:            return Keyboard::Key::Num6;
        case XK_7:            return Keyboard::Key::Num7;
        case XK_8:            return Keyboard::Key::Num8;
        case XK_9:            return Keyboard::Key::Num9;
        default:              return Keyboard::Key::Unknown;
    }
    // clang-format on
}


////////////////////////////////////////////////////////////
KeySym keyToKeySym(Keyboard::Key key)
{
    // clang-format off
    switch (key)
    {
        case Keyboard::Key::LShift:     return XK_Shift_L;
        case Keyboard::Key::RShift:     return XK_Shift_R;
        case Keyboard::Key::LControl:   return XK_Control_L;
        case Keyboard::Key::RControl:   return XK_Control_R;
        case Keyboard::Key::LAlt:       return XK_Alt_L;
        case Keyboard::Key::RAlt:       return XK_Alt_R;
        case Keyboard::Key::LSystem:    return XK_Super_L;
        case Keyboard::Key::RSystem:    return XK_Super_R;
        case Keyboard::Key::Menu:       return XK_Menu;
        case Keyboard::Key::Escape:     return XK_Escape;
        case Keyboard::Key::Semicolon:  return XK_semicolon;
        case Keyboard::Key::Slash:      return XK_slash;
        case Keyboard::Key::Equal:      return XK_equal;
        case Keyboard::Key::Hyphen:     return XK_minus;
        case Keyboard::Key::LBracket:   return XK_bracketleft;
        case Keyboard::Key::RBracket:   return XK_bracketright;
        case Keyboard::Key::Comma:      return XK_comma;
        case Keyboard::Key::Period:     return XK_period;
        case Keyboard::Key::Apostrophe: return XK_apostrophe;
        case Keyboard::Key::Backslash:  return XK_backslash;
        case Keyboard::Key::Grave:      return XK_grave;
        case Keyboard::Key::Space:      return XK_space;
        case Keyboard::Key::Enter:      return XK_Return;
        case Keyboard::Key::Backspace:  return XK_BackSpace;
        case Keyboard::Key::Tab:        return XK_Tab;
        case Keyboard::Key::PageUp:     return XK_Prior;
        case Keyboard::Key::PageDown:   return XK_Next;
        case Keyboard::Key::End:        return XK_End;
        case Keyboard::Key::Home:       return XK_Home;
        case Keyboard::Key::Insert:     return XK_Insert;
        case Keyboard::Key::Delete:     return XK_Delete;
        case Keyboard::Key::Add:        return XK_KP_Add;
        case Keyboard::Key::Subtract:   return XK_KP_Subtract;
        case Keyboard::Key::Multiply:   return XK_KP_Multiply;
        case Keyboard::Key::Divide:     return XK_KP_Divide;
        case Keyboard::Key::Pause:      return XK_Pause;
        case Keyboard::Key::F1:         return XK_F1;
        case Keyboard::Key::F2:         return XK_F2;
        case Keyboard::Key::F3:         return XK_F3;
        case Keyboard::Key::F4:         return XK_F4;
        case Keyboard::Key::F5:         return XK_F5;
        case Keyboard::Key::F6:         return XK_F6;
        case Keyboard::Key::F7:         return XK_F7;
        case Keyboard::Key::F8:         return XK_F8;
        case Keyboard::Key::F9:         return XK_F9;
        case Keyboard::Key::F10:        return XK_F10;
        case Keyboard::Key::F11:        return XK_F11;
        case Keyboard::Key::F12:        return XK_F12;
        case Keyboard::Key::F13:        return XK_F13;
        case Keyboard::Key::F14:        return XK_F14;
        case Keyboard::Key::F15:        return XK_F15;
        case Keyboard::Key::Left:       return XK_Left;
        case Keyboard::Key::Right:      return XK_Right;
        case Keyboard::Key::Up:         return XK_Up;
        case Keyboard::Key::Down:       return XK_Down;
        case Keyboard::Key::Numpad0:    return XK_KP_Insert;
        case Keyboard::Key::Numpad1:    return XK_KP_End;
        case Keyboard::Key::Numpad2:    return XK_KP_Down;
        case Keyboard::Key::Numpad3:    return XK_KP_Page_Down;
        case Keyboard::Key::Numpad4:    return XK_KP_Left;
        case Keyboard::Key::Numpad5:    return XK_KP_Begin;
        case Keyboard::Key::Numpad6:    return XK_KP_Right;
        case Keyboard::Key::Numpad7:    return XK_KP_Home;
        case Keyboard::Key::Numpad8:    return XK_KP_Up;
        case Keyboard::Key::Numpad9:    return XK_KP_Page_Up;
        case Keyboard::Key::A:          return XK_a;
        case Keyboard::Key::B:          return XK_b;
        case Keyboard::Key::C:          return XK_c;
        case Keyboard::Key::D:          return XK_d;
        case Keyboard::Key::E:          return XK_e;
        case Keyboard::Key::F:          return XK_f;
        case Keyboard::Key::G:          return XK_g;
        case Keyboard::Key::H:          return XK_h;
        case Keyboard::Key::I:          return XK_i;
        case Keyboard::Key::J:          return XK_j;
        case Keyboard::Key::K:          return XK_k;
        case Keyboard::Key::L:          return XK_l;
        case Keyboard::Key::M:          return XK_m;
        case Keyboard::Key::N:          return XK_n;
        case Keyboard::Key::O:          return XK_o;
        case Keyboard::Key::P:          return XK_p;
        case Keyboard::Key::Q:          return XK_q;
        case Keyboard::Key::R:          return XK_r;
        case Keyboard::Key::S:          return XK_s;
        case Keyboard::Key::T:          return XK_t;
        case Keyboard::Key::U:          return XK_u;
        case Keyboard::Key::V:          return XK_v;
        case Keyboard::Key::W:          return XK_w;
        case Keyboard::Key::X:          return XK_x;
        case Keyboard::Key::Y:          return XK_y;
        case Keyboard::Key::Z:          return XK_z;
        case Keyboard::Key::Num0:       return XK_0;
        case Keyboard::Key::Num1:       return XK_1;
        case Keyboard::Key::Num2:       return XK_2;
        case Keyboard::Key::Num3:       return XK_3;
        case Keyboard::Key::Num4:       return XK_4;
        case Keyboard::Key::Num5:       return XK_5;
        case Keyboard::Key::Num6:       return XK_6;
        case Keyboard::Key::Num7:       return XK_7;
        case Keyboard::Key::Num8:       return XK_8;
        case Keyboard::Key::Num9:       return XK_9;
        default:                        return NoSymbol;
    }
    // clang-format on
}
} // namespace sf::priv
