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
#include <SFML/Window/Unix/KeySymToKeyMapping.hpp>
#include <X11/keysym.h>

namespace sf
{
namespace priv
{

////////////////////////////////////////////////////////////
Keyboard::Key keySymToKey(KeySym symbol)
{
    switch (symbol)
    {
        case XK_Shift_L:      return Keyboard::LShift;
        case XK_Shift_R:      return Keyboard::RShift;
        case XK_Control_L:    return Keyboard::LControl;
        case XK_Control_R:    return Keyboard::RControl;
        case XK_Alt_L:        return Keyboard::LAlt;
        case XK_ISO_Level3_Shift:
        case XK_Alt_R:        return Keyboard::RAlt;
        case XK_Super_L:      return Keyboard::LSystem;
        case XK_Super_R:      return Keyboard::RSystem;
        case XK_Menu:         return Keyboard::Menu;
        case XK_Escape:       return Keyboard::Escape;
        case XK_semicolon:    return Keyboard::SemiColon;
        case XK_slash:        return Keyboard::Slash;
        case XK_equal:        return Keyboard::Equal;
        case XK_minus:        return Keyboard::Dash;
        case XK_bracketleft:  return Keyboard::LBracket;
        case XK_bracketright: return Keyboard::RBracket;
        case XK_comma:        return Keyboard::Comma;
        case XK_period:       return Keyboard::Period;
        case XK_apostrophe:   return Keyboard::Quote;
        case XK_backslash:    return Keyboard::BackSlash;
        case XK_grave:        return Keyboard::Grave;
        case XK_space:        return Keyboard::Space;
        case XK_Return:       return Keyboard::Return;
        case XK_KP_Enter:     return Keyboard::Return;
        case XK_BackSpace:    return Keyboard::BackSpace;
        case XK_Tab:          return Keyboard::Tab;
        case XK_Prior:        return Keyboard::PageUp;
        case XK_Next:         return Keyboard::PageDown;
        case XK_End:          return Keyboard::End;
        case XK_Home:         return Keyboard::Home;
        case XK_Insert:       return Keyboard::Insert;
        case XK_Delete:       return Keyboard::Delete;
        case XK_KP_Add:       return Keyboard::Add;
        case XK_KP_Subtract:  return Keyboard::Subtract;
        case XK_KP_Multiply:  return Keyboard::Multiply;
        case XK_KP_Divide:    return Keyboard::Divide;
        case XK_KP_Delete:    return Keyboard::Period;
        case XK_Pause:        return Keyboard::Pause;
        case XK_F1:           return Keyboard::F1;
        case XK_F2:           return Keyboard::F2;
        case XK_F3:           return Keyboard::F3;
        case XK_F4:           return Keyboard::F4;
        case XK_F5:           return Keyboard::F5;
        case XK_F6:           return Keyboard::F6;
        case XK_F7:           return Keyboard::F7;
        case XK_F8:           return Keyboard::F8;
        case XK_F9:           return Keyboard::F9;
        case XK_F10:          return Keyboard::F10;
        case XK_F11:          return Keyboard::F11;
        case XK_F12:          return Keyboard::F12;
        case XK_F13:          return Keyboard::F13;
        case XK_F14:          return Keyboard::F14;
        case XK_F15:          return Keyboard::F15;
        case XK_Left:         return Keyboard::Left;
        case XK_Right:        return Keyboard::Right;
        case XK_Up:           return Keyboard::Up;
        case XK_Down:         return Keyboard::Down;
        case XK_KP_Insert:    return Keyboard::Numpad0;
        case XK_KP_End:       return Keyboard::Numpad1;
        case XK_KP_Down:      return Keyboard::Numpad2;
        case XK_KP_Page_Down: return Keyboard::Numpad3;
        case XK_KP_Left:      return Keyboard::Numpad4;
        case XK_KP_Begin:     return Keyboard::Numpad5;
        case XK_KP_Right:     return Keyboard::Numpad6;
        case XK_KP_Home:      return Keyboard::Numpad7;
        case XK_KP_Up:        return Keyboard::Numpad8;
        case XK_KP_Page_Up:   return Keyboard::Numpad9;
        case XK_a:            return Keyboard::A;
        case XK_b:            return Keyboard::B;
        case XK_c:            return Keyboard::C;
        case XK_d:            return Keyboard::D;
        case XK_e:            return Keyboard::E;
        case XK_f:            return Keyboard::F;
        case XK_g:            return Keyboard::G;
        case XK_h:            return Keyboard::H;
        case XK_i:            return Keyboard::I;
        case XK_j:            return Keyboard::J;
        case XK_k:            return Keyboard::K;
        case XK_l:            return Keyboard::L;
        case XK_m:            return Keyboard::M;
        case XK_n:            return Keyboard::N;
        case XK_o:            return Keyboard::O;
        case XK_p:            return Keyboard::P;
        case XK_q:            return Keyboard::Q;
        case XK_r:            return Keyboard::R;
        case XK_s:            return Keyboard::S;
        case XK_t:            return Keyboard::T;
        case XK_u:            return Keyboard::U;
        case XK_v:            return Keyboard::V;
        case XK_w:            return Keyboard::W;
        case XK_x:            return Keyboard::X;
        case XK_y:            return Keyboard::Y;
        case XK_z:            return Keyboard::Z;
        case XK_0:            return Keyboard::Num0;
        case XK_1:            return Keyboard::Num1;
        case XK_2:            return Keyboard::Num2;
        case XK_3:            return Keyboard::Num3;
        case XK_4:            return Keyboard::Num4;
        case XK_5:            return Keyboard::Num5;
        case XK_6:            return Keyboard::Num6;
        case XK_7:            return Keyboard::Num7;
        case XK_8:            return Keyboard::Num8;
        case XK_9:            return Keyboard::Num9;
        default:              return Keyboard::Unknown;
    }
}


////////////////////////////////////////////////////////////
KeySym keyToKeySym(Keyboard::Key key)
{
    switch (key)
    {
        case Keyboard::LShift:     return XK_Shift_L;
        case Keyboard::RShift:     return XK_Shift_R;
        case Keyboard::LControl:   return XK_Control_L;
        case Keyboard::RControl:   return XK_Control_R;
        case Keyboard::LAlt:       return XK_Alt_L;
        case Keyboard::RAlt:       return XK_Alt_R;
        case Keyboard::LSystem:    return XK_Super_L;
        case Keyboard::RSystem:    return XK_Super_R;
        case Keyboard::Menu:       return XK_Menu;
        case Keyboard::Escape:     return XK_Escape;
        case Keyboard::SemiColon:  return XK_semicolon;
        case Keyboard::Slash:      return XK_slash;
        case Keyboard::Equal:      return XK_equal;
        case Keyboard::Dash:       return XK_minus;
        case Keyboard::LBracket:   return XK_bracketleft;
        case Keyboard::RBracket:   return XK_bracketright;
        case Keyboard::Comma:      return XK_comma;
        case Keyboard::Period:     return XK_period;
        case Keyboard::Quote:      return XK_apostrophe;
        case Keyboard::BackSlash:  return XK_backslash;
        case Keyboard::Grave:      return XK_grave;
        case Keyboard::Space:      return XK_space;
        case Keyboard::Return:     return XK_Return;
        case Keyboard::BackSpace:  return XK_BackSpace;
        case Keyboard::Tab:        return XK_Tab;
        case Keyboard::PageUp:     return XK_Prior;
        case Keyboard::PageDown:   return XK_Next;
        case Keyboard::End:        return XK_End;
        case Keyboard::Home:       return XK_Home;
        case Keyboard::Insert:     return XK_Insert;
        case Keyboard::Delete:     return XK_Delete;
        case Keyboard::Add:        return XK_KP_Add;
        case Keyboard::Subtract:   return XK_KP_Subtract;
        case Keyboard::Multiply:   return XK_KP_Multiply;
        case Keyboard::Divide:     return XK_KP_Divide;
        case Keyboard::Pause:      return XK_Pause;
        case Keyboard::F1:         return XK_F1;
        case Keyboard::F2:         return XK_F2;
        case Keyboard::F3:         return XK_F3;
        case Keyboard::F4:         return XK_F4;
        case Keyboard::F5:         return XK_F5;
        case Keyboard::F6:         return XK_F6;
        case Keyboard::F7:         return XK_F7;
        case Keyboard::F8:         return XK_F8;
        case Keyboard::F9:         return XK_F9;
        case Keyboard::F10:        return XK_F10;
        case Keyboard::F11:        return XK_F11;
        case Keyboard::F12:        return XK_F12;
        case Keyboard::F13:        return XK_F13;
        case Keyboard::F14:        return XK_F14;
        case Keyboard::F15:        return XK_F15;
        case Keyboard::Left:       return XK_Left;
        case Keyboard::Right:      return XK_Right;
        case Keyboard::Up:         return XK_Up;
        case Keyboard::Down:       return XK_Down;
        case Keyboard::Numpad0:    return XK_KP_Insert;
        case Keyboard::Numpad1:    return XK_KP_End;
        case Keyboard::Numpad2:    return XK_KP_Down;
        case Keyboard::Numpad3:    return XK_KP_Page_Down;
        case Keyboard::Numpad4:    return XK_KP_Left;
        case Keyboard::Numpad5:    return XK_KP_Begin;
        case Keyboard::Numpad6:    return XK_KP_Right;
        case Keyboard::Numpad7:    return XK_KP_Home;
        case Keyboard::Numpad8:    return XK_KP_Up;
        case Keyboard::Numpad9:    return XK_KP_Page_Up;
        case Keyboard::A:          return XK_a;
        case Keyboard::B:          return XK_b;
        case Keyboard::C:          return XK_c;
        case Keyboard::D:          return XK_d;
        case Keyboard::E:          return XK_e;
        case Keyboard::F:          return XK_f;
        case Keyboard::G:          return XK_g;
        case Keyboard::H:          return XK_h;
        case Keyboard::I:          return XK_i;
        case Keyboard::J:          return XK_j;
        case Keyboard::K:          return XK_k;
        case Keyboard::L:          return XK_l;
        case Keyboard::M:          return XK_m;
        case Keyboard::N:          return XK_n;
        case Keyboard::O:          return XK_o;
        case Keyboard::P:          return XK_p;
        case Keyboard::Q:          return XK_q;
        case Keyboard::R:          return XK_r;
        case Keyboard::S:          return XK_s;
        case Keyboard::T:          return XK_t;
        case Keyboard::U:          return XK_u;
        case Keyboard::V:          return XK_v;
        case Keyboard::W:          return XK_w;
        case Keyboard::X:          return XK_x;
        case Keyboard::Y:          return XK_y;
        case Keyboard::Z:          return XK_z;
        case Keyboard::Num0:       return XK_0;
        case Keyboard::Num1:       return XK_1;
        case Keyboard::Num2:       return XK_2;
        case Keyboard::Num3:       return XK_3;
        case Keyboard::Num4:       return XK_4;
        case Keyboard::Num5:       return XK_5;
        case Keyboard::Num6:       return XK_6;
        case Keyboard::Num7:       return XK_7;
        case Keyboard::Num8:       return XK_8;
        case Keyboard::Num9:       return XK_9;
        default:                   return NoSymbol;
    }
}

} // namespace priv

} // namespace sf

