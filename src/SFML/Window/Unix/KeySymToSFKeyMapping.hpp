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

#ifndef SFML_KEYSYMTOSFKEYMAPPING_HPP
#define SFML_KEYSYMTOSFKEYMAPPING_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Keyboard.hpp>
#include <X11/Xlib.h>
#include <X11/keysym.h>

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Convert X11 KeySym to sf::Keyboard::Key
///
/// \param symbol X11 KeySym
///
/// \return The corresponding sf::Keyboard::Key
///
////////////////////////////////////////////////////////////
inline Keyboard::Key keySymToSFKey(KeySym symbol)
{
    switch (symbol)
    {
        case XK_Shift_L:      return Keyboard::LShift;
        case XK_Shift_R:      return Keyboard::RShift;
        case XK_Control_L:    return Keyboard::LControl;
        case XK_Control_R:    return Keyboard::RControl;
        case XK_Alt_L:        return Keyboard::LAlt;
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
        case XK_grave:        return Keyboard::Tilde;
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
    }

    return Keyboard::Unknown;
}


////////////////////////////////////////////////////////////
/// \brief Convert sf::Keyboard::Key to X11 KeySym
///
/// \param key X11 sf::Keyboard::Key
///
/// \return The corresponding X11 KeySym
///
////////////////////////////////////////////////////////////
inline KeySym SFKeyToKeySym(Keyboard::Key key)
{
    // Get the corresponding X11 keysym
    KeySym keysym = NoSymbol;
    switch (key)
    {
        case Keyboard::LShift:     keysym = XK_Shift_L;      break;
        case Keyboard::RShift:     keysym = XK_Shift_R;      break;
        case Keyboard::LControl:   keysym = XK_Control_L;    break;
        case Keyboard::RControl:   keysym = XK_Control_R;    break;
        case Keyboard::LAlt:       keysym = XK_Alt_L;        break;
        case Keyboard::RAlt:       keysym = XK_Alt_R;        break;
        case Keyboard::LSystem:    keysym = XK_Super_L;      break;
        case Keyboard::RSystem:    keysym = XK_Super_R;      break;
        case Keyboard::Menu:       keysym = XK_Menu;         break;
        case Keyboard::Escape:     keysym = XK_Escape;       break;
        case Keyboard::SemiColon:  keysym = XK_semicolon;    break;
        case Keyboard::Slash:      keysym = XK_slash;        break;
        case Keyboard::Equal:      keysym = XK_equal;        break;
        case Keyboard::Dash:       keysym = XK_minus;        break;
        case Keyboard::LBracket:   keysym = XK_bracketleft;  break;
        case Keyboard::RBracket:   keysym = XK_bracketright; break;
        case Keyboard::Comma:      keysym = XK_comma;        break;
        case Keyboard::Period:     keysym = XK_period;       break;
        case Keyboard::Quote:      keysym = XK_apostrophe;   break;
        case Keyboard::BackSlash:  keysym = XK_backslash;    break;
        case Keyboard::Tilde:      keysym = XK_grave;        break;
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
        case Keyboard::Pause:      keysym = XK_Pause;        break;
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
        case Keyboard::Left:       keysym = XK_Left;         break;
        case Keyboard::Right:      keysym = XK_Right;        break;
        case Keyboard::Up:         keysym = XK_Up;           break;
        case Keyboard::Down:       keysym = XK_Down;         break;
        case Keyboard::Numpad0:    keysym = XK_KP_Insert;    break;
        case Keyboard::Numpad1:    keysym = XK_KP_End;       break;
        case Keyboard::Numpad2:    keysym = XK_KP_Down;      break;
        case Keyboard::Numpad3:    keysym = XK_KP_Page_Down; break;
        case Keyboard::Numpad4:    keysym = XK_KP_Left;      break;
        case Keyboard::Numpad5:    keysym = XK_KP_Begin;     break;
        case Keyboard::Numpad6:    keysym = XK_KP_Right;     break;
        case Keyboard::Numpad7:    keysym = XK_KP_Home;      break;
        case Keyboard::Numpad8:    keysym = XK_KP_Up;        break;
        case Keyboard::Numpad9:    keysym = XK_KP_Page_Up;   break;
        case Keyboard::A:          keysym = XK_a;            break;
        case Keyboard::B:          keysym = XK_b;            break;
        case Keyboard::C:          keysym = XK_c;            break;
        case Keyboard::D:          keysym = XK_d;            break;
        case Keyboard::E:          keysym = XK_e;            break;
        case Keyboard::F:          keysym = XK_f;            break;
        case Keyboard::G:          keysym = XK_g;            break;
        case Keyboard::H:          keysym = XK_h;            break;
        case Keyboard::I:          keysym = XK_i;            break;
        case Keyboard::J:          keysym = XK_j;            break;
        case Keyboard::K:          keysym = XK_k;            break;
        case Keyboard::L:          keysym = XK_l;            break;
        case Keyboard::M:          keysym = XK_m;            break;
        case Keyboard::N:          keysym = XK_n;            break;
        case Keyboard::O:          keysym = XK_o;            break;
        case Keyboard::P:          keysym = XK_p;            break;
        case Keyboard::Q:          keysym = XK_q;            break;
        case Keyboard::R:          keysym = XK_r;            break;
        case Keyboard::S:          keysym = XK_s;            break;
        case Keyboard::T:          keysym = XK_t;            break;
        case Keyboard::U:          keysym = XK_u;            break;
        case Keyboard::V:          keysym = XK_v;            break;
        case Keyboard::W:          keysym = XK_w;            break;
        case Keyboard::X:          keysym = XK_x;            break;
        case Keyboard::Y:          keysym = XK_y;            break;
        case Keyboard::Z:          keysym = XK_z;            break;
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
    }
    return keysym;
}

} // namespace priv

} // namespace sf

#endif // SFML_KEYSYMTOSFKEYMAPPING_HPP
