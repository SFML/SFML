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
#include <SFML/Window/Keyboard.hpp> // sf::Keyboard::Key
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
inline sf::Keyboard::Key keySymToSFKey(KeySym symbol)
{
    switch (symbol)
    {
        case XK_Shift_L:      return sf::Keyboard::LShift;
        case XK_Shift_R:      return sf::Keyboard::RShift;
        case XK_Control_L:    return sf::Keyboard::LControl;
        case XK_Control_R:    return sf::Keyboard::RControl;
        case XK_Alt_L:        return sf::Keyboard::LAlt;
        case XK_Alt_R:        return sf::Keyboard::RAlt;
        case XK_Super_L:      return sf::Keyboard::LSystem;
        case XK_Super_R:      return sf::Keyboard::RSystem;
        case XK_Menu:         return sf::Keyboard::Menu;
        case XK_Escape:       return sf::Keyboard::Escape;
        case XK_semicolon:    return sf::Keyboard::SemiColon;
        case XK_slash:        return sf::Keyboard::Slash;
        case XK_equal:        return sf::Keyboard::Equal;
        case XK_minus:        return sf::Keyboard::Dash;
        case XK_bracketleft:  return sf::Keyboard::LBracket;
        case XK_bracketright: return sf::Keyboard::RBracket;
        case XK_comma:        return sf::Keyboard::Comma;
        case XK_period:       return sf::Keyboard::Period;
        case XK_apostrophe:   return sf::Keyboard::Quote;
        case XK_backslash:    return sf::Keyboard::BackSlash;
        case XK_grave:        return sf::Keyboard::Tilde;
        case XK_space:        return sf::Keyboard::Space;
        case XK_Return:       return sf::Keyboard::Return;
        case XK_KP_Enter:     return sf::Keyboard::Return;
        case XK_BackSpace:    return sf::Keyboard::BackSpace;
        case XK_Tab:          return sf::Keyboard::Tab;
        case XK_Prior:        return sf::Keyboard::PageUp;
        case XK_Next:         return sf::Keyboard::PageDown;
        case XK_End:          return sf::Keyboard::End;
        case XK_Home:         return sf::Keyboard::Home;
        case XK_Insert:       return sf::Keyboard::Insert;
        case XK_Delete:       return sf::Keyboard::Delete;
        case XK_KP_Add:       return sf::Keyboard::Add;
        case XK_KP_Subtract:  return sf::Keyboard::Subtract;
        case XK_KP_Multiply:  return sf::Keyboard::Multiply;
        case XK_KP_Divide:    return sf::Keyboard::Divide;
        case XK_Pause:        return sf::Keyboard::Pause;
        case XK_F1:           return sf::Keyboard::F1;
        case XK_F2:           return sf::Keyboard::F2;
        case XK_F3:           return sf::Keyboard::F3;
        case XK_F4:           return sf::Keyboard::F4;
        case XK_F5:           return sf::Keyboard::F5;
        case XK_F6:           return sf::Keyboard::F6;
        case XK_F7:           return sf::Keyboard::F7;
        case XK_F8:           return sf::Keyboard::F8;
        case XK_F9:           return sf::Keyboard::F9;
        case XK_F10:          return sf::Keyboard::F10;
        case XK_F11:          return sf::Keyboard::F11;
        case XK_F12:          return sf::Keyboard::F12;
        case XK_F13:          return sf::Keyboard::F13;
        case XK_F14:          return sf::Keyboard::F14;
        case XK_F15:          return sf::Keyboard::F15;
        case XK_Left:         return sf::Keyboard::Left;
        case XK_Right:        return sf::Keyboard::Right;
        case XK_Up:           return sf::Keyboard::Up;
        case XK_Down:         return sf::Keyboard::Down;
        case XK_KP_Insert:    return sf::Keyboard::Numpad0;
        case XK_KP_End:       return sf::Keyboard::Numpad1;
        case XK_KP_Down:      return sf::Keyboard::Numpad2;
        case XK_KP_Page_Down: return sf::Keyboard::Numpad3;
        case XK_KP_Left:      return sf::Keyboard::Numpad4;
        case XK_KP_Begin:     return sf::Keyboard::Numpad5;
        case XK_KP_Right:     return sf::Keyboard::Numpad6;
        case XK_KP_Home:      return sf::Keyboard::Numpad7;
        case XK_KP_Up:        return sf::Keyboard::Numpad8;
        case XK_KP_Page_Up:   return sf::Keyboard::Numpad9;
        case XK_a:            return sf::Keyboard::A;
        case XK_b:            return sf::Keyboard::B;
        case XK_c:            return sf::Keyboard::C;
        case XK_d:            return sf::Keyboard::D;
        case XK_e:            return sf::Keyboard::E;
        case XK_f:            return sf::Keyboard::F;
        case XK_g:            return sf::Keyboard::G;
        case XK_h:            return sf::Keyboard::H;
        case XK_i:            return sf::Keyboard::I;
        case XK_j:            return sf::Keyboard::J;
        case XK_k:            return sf::Keyboard::K;
        case XK_l:            return sf::Keyboard::L;
        case XK_m:            return sf::Keyboard::M;
        case XK_n:            return sf::Keyboard::N;
        case XK_o:            return sf::Keyboard::O;
        case XK_p:            return sf::Keyboard::P;
        case XK_q:            return sf::Keyboard::Q;
        case XK_r:            return sf::Keyboard::R;
        case XK_s:            return sf::Keyboard::S;
        case XK_t:            return sf::Keyboard::T;
        case XK_u:            return sf::Keyboard::U;
        case XK_v:            return sf::Keyboard::V;
        case XK_w:            return sf::Keyboard::W;
        case XK_x:            return sf::Keyboard::X;
        case XK_y:            return sf::Keyboard::Y;
        case XK_z:            return sf::Keyboard::Z;
        case XK_0:            return sf::Keyboard::Num0;
        case XK_1:            return sf::Keyboard::Num1;
        case XK_2:            return sf::Keyboard::Num2;
        case XK_3:            return sf::Keyboard::Num3;
        case XK_4:            return sf::Keyboard::Num4;
        case XK_5:            return sf::Keyboard::Num5;
        case XK_6:            return sf::Keyboard::Num6;
        case XK_7:            return sf::Keyboard::Num7;
        case XK_8:            return sf::Keyboard::Num8;
        case XK_9:            return sf::Keyboard::Num9;
    }

    return sf::Keyboard::Unknown;
}


////////////////////////////////////////////////////////////
/// \brief Convert sf::Keyboard::Key to X11 KeySym
///
/// \param key X11 sf::Keyboard::Key
///
/// \return The corresponding X11 KeySym
///
////////////////////////////////////////////////////////////
inline KeySym SFKeyToKeySym(sf::Keyboard::Key key)
{
    // Get the corresponding X11 keysym
    KeySym keysym = NoSymbol;
    switch (key)
    {
        case sf::Keyboard::LShift:     keysym = XK_Shift_L;      break;
        case sf::Keyboard::RShift:     keysym = XK_Shift_R;      break;
        case sf::Keyboard::LControl:   keysym = XK_Control_L;    break;
        case sf::Keyboard::RControl:   keysym = XK_Control_R;    break;
        case sf::Keyboard::LAlt:       keysym = XK_Alt_L;        break;
        case sf::Keyboard::RAlt:       keysym = XK_Alt_R;        break;
        case sf::Keyboard::LSystem:    keysym = XK_Super_L;      break;
        case sf::Keyboard::RSystem:    keysym = XK_Super_R;      break;
        case sf::Keyboard::Menu:       keysym = XK_Menu;         break;
        case sf::Keyboard::Escape:     keysym = XK_Escape;       break;
        case sf::Keyboard::SemiColon:  keysym = XK_semicolon;    break;
        case sf::Keyboard::Slash:      keysym = XK_slash;        break;
        case sf::Keyboard::Equal:      keysym = XK_equal;        break;
        case sf::Keyboard::Dash:       keysym = XK_minus;        break;
        case sf::Keyboard::LBracket:   keysym = XK_bracketleft;  break;
        case sf::Keyboard::RBracket:   keysym = XK_bracketright; break;
        case sf::Keyboard::Comma:      keysym = XK_comma;        break;
        case sf::Keyboard::Period:     keysym = XK_period;       break;
        case sf::Keyboard::Quote:      keysym = XK_apostrophe;   break;
        case sf::Keyboard::BackSlash:  keysym = XK_backslash;    break;
        case sf::Keyboard::Tilde:      keysym = XK_grave;        break;
        case sf::Keyboard::Space:      keysym = XK_space;        break;
        case sf::Keyboard::Return:     keysym = XK_Return;       break;
        case sf::Keyboard::BackSpace:  keysym = XK_BackSpace;    break;
        case sf::Keyboard::Tab:        keysym = XK_Tab;          break;
        case sf::Keyboard::PageUp:     keysym = XK_Prior;        break;
        case sf::Keyboard::PageDown:   keysym = XK_Next;         break;
        case sf::Keyboard::End:        keysym = XK_End;          break;
        case sf::Keyboard::Home:       keysym = XK_Home;         break;
        case sf::Keyboard::Insert:     keysym = XK_Insert;       break;
        case sf::Keyboard::Delete:     keysym = XK_Delete;       break;
        case sf::Keyboard::Add:        keysym = XK_KP_Add;       break;
        case sf::Keyboard::Subtract:   keysym = XK_KP_Subtract;  break;
        case sf::Keyboard::Multiply:   keysym = XK_KP_Multiply;  break;
        case sf::Keyboard::Divide:     keysym = XK_KP_Divide;    break;
        case sf::Keyboard::Pause:      keysym = XK_Pause;        break;
        case sf::Keyboard::F1:         keysym = XK_F1;           break;
        case sf::Keyboard::F2:         keysym = XK_F2;           break;
        case sf::Keyboard::F3:         keysym = XK_F3;           break;
        case sf::Keyboard::F4:         keysym = XK_F4;           break;
        case sf::Keyboard::F5:         keysym = XK_F5;           break;
        case sf::Keyboard::F6:         keysym = XK_F6;           break;
        case sf::Keyboard::F7:         keysym = XK_F7;           break;
        case sf::Keyboard::F8:         keysym = XK_F8;           break;
        case sf::Keyboard::F9:         keysym = XK_F9;           break;
        case sf::Keyboard::F10:        keysym = XK_F10;          break;
        case sf::Keyboard::F11:        keysym = XK_F11;          break;
        case sf::Keyboard::F12:        keysym = XK_F12;          break;
        case sf::Keyboard::F13:        keysym = XK_F13;          break;
        case sf::Keyboard::F14:        keysym = XK_F14;          break;
        case sf::Keyboard::F15:        keysym = XK_F15;          break;
        case sf::Keyboard::Left:       keysym = XK_Left;         break;
        case sf::Keyboard::Right:      keysym = XK_Right;        break;
        case sf::Keyboard::Up:         keysym = XK_Up;           break;
        case sf::Keyboard::Down:       keysym = XK_Down;         break;
        case sf::Keyboard::Numpad0:    keysym = XK_KP_Insert;    break;
        case sf::Keyboard::Numpad1:    keysym = XK_KP_End;       break;
        case sf::Keyboard::Numpad2:    keysym = XK_KP_Down;      break;
        case sf::Keyboard::Numpad3:    keysym = XK_KP_Page_Down; break;
        case sf::Keyboard::Numpad4:    keysym = XK_KP_Left;      break;
        case sf::Keyboard::Numpad5:    keysym = XK_KP_Begin;     break;
        case sf::Keyboard::Numpad6:    keysym = XK_KP_Right;     break;
        case sf::Keyboard::Numpad7:    keysym = XK_KP_Home;      break;
        case sf::Keyboard::Numpad8:    keysym = XK_KP_Up;        break;
        case sf::Keyboard::Numpad9:    keysym = XK_KP_Page_Up;   break;
        case sf::Keyboard::A:          keysym = XK_a;            break;
        case sf::Keyboard::B:          keysym = XK_b;            break;
        case sf::Keyboard::C:          keysym = XK_c;            break;
        case sf::Keyboard::D:          keysym = XK_d;            break;
        case sf::Keyboard::E:          keysym = XK_e;            break;
        case sf::Keyboard::F:          keysym = XK_f;            break;
        case sf::Keyboard::G:          keysym = XK_g;            break;
        case sf::Keyboard::H:          keysym = XK_h;            break;
        case sf::Keyboard::I:          keysym = XK_i;            break;
        case sf::Keyboard::J:          keysym = XK_j;            break;
        case sf::Keyboard::K:          keysym = XK_k;            break;
        case sf::Keyboard::L:          keysym = XK_l;            break;
        case sf::Keyboard::M:          keysym = XK_m;            break;
        case sf::Keyboard::N:          keysym = XK_n;            break;
        case sf::Keyboard::O:          keysym = XK_o;            break;
        case sf::Keyboard::P:          keysym = XK_p;            break;
        case sf::Keyboard::Q:          keysym = XK_q;            break;
        case sf::Keyboard::R:          keysym = XK_r;            break;
        case sf::Keyboard::S:          keysym = XK_s;            break;
        case sf::Keyboard::T:          keysym = XK_t;            break;
        case sf::Keyboard::U:          keysym = XK_u;            break;
        case sf::Keyboard::V:          keysym = XK_v;            break;
        case sf::Keyboard::W:          keysym = XK_w;            break;
        case sf::Keyboard::X:          keysym = XK_x;            break;
        case sf::Keyboard::Y:          keysym = XK_y;            break;
        case sf::Keyboard::Z:          keysym = XK_z;            break;
        case sf::Keyboard::Num0:       keysym = XK_0;            break;
        case sf::Keyboard::Num1:       keysym = XK_1;            break;
        case sf::Keyboard::Num2:       keysym = XK_2;            break;
        case sf::Keyboard::Num3:       keysym = XK_3;            break;
        case sf::Keyboard::Num4:       keysym = XK_4;            break;
        case sf::Keyboard::Num5:       keysym = XK_5;            break;
        case sf::Keyboard::Num6:       keysym = XK_6;            break;
        case sf::Keyboard::Num7:       keysym = XK_7;            break;
        case sf::Keyboard::Num8:       keysym = XK_8;            break;
        case sf::Keyboard::Num9:       keysym = XK_9;            break;
        default:                       keysym = 0;               break;
    }
    return keysym;
}

} // namespace priv

} // namespace sf

#endif // SFML_KEYSYMTOSFKEYMAPPING_HPP
