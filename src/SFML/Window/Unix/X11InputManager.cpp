////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2018 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/Unix/X11InputManager.hpp>
#include <SFML/Window/Unix/Display.hpp>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <cstring>

namespace sf
{
namespace priv
{

namespace {
////////////////////////////////////////////////////////////
sf::Keyboard::Key keysymToSF(KeySym symbol)
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
KeySym SFtoKeysym(sf::Keyboard::Key key)
{
    // Get the corresponding X11 keysym
    KeySym keysym = 0;
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


////////////////////////////////////////////////////////////
sf::Keyboard::Scancode translateKeyCode(Display* display, KeyCode keycode)
{
    KeySym keySym;

    // Valid key code range is [8,255], according to the Xlib manual
    if (keycode < 8 || keycode > 255)
        return sf::Keyboard::ScanUnknown;

    // Try secondary keysym, for numeric keypad keys
    // Note: This way we always force "NumLock = ON", which is intentional
    // since the returned key code should correspond to a physical
    // location.
    keySym = XkbKeycodeToKeysym(display, keycode, 0, 1);
    switch (keySym)
    {
        case XK_KP_0:           return sf::Keyboard::ScanNumpad0;
        case XK_KP_1:           return sf::Keyboard::ScanNumpad1;
        case XK_KP_2:           return sf::Keyboard::ScanNumpad2;
        case XK_KP_3:           return sf::Keyboard::ScanNumpad3;
        case XK_KP_4:           return sf::Keyboard::ScanNumpad4;
        case XK_KP_5:           return sf::Keyboard::ScanNumpad5;
        case XK_KP_6:           return sf::Keyboard::ScanNumpad6;
        case XK_KP_7:           return sf::Keyboard::ScanNumpad7;
        case XK_KP_8:           return sf::Keyboard::ScanNumpad8;
        case XK_KP_9:           return sf::Keyboard::ScanNumpad9;
        case XK_KP_Separator:
        case XK_KP_Decimal:     return sf::Keyboard::ScanDecimal;
        case XK_KP_Equal:       return sf::Keyboard::ScanPadEquals;
        case XK_KP_Enter:       return sf::Keyboard::ScanReturn;
        default:                break;
    }

    // Now try primary keysym for function keys (non-printable keys)
    // These should not depend on the current keyboard layout
    keySym = XkbKeycodeToKeysym(display, keycode, 0, 0);

    switch (keySym)
    {
        case XK_Escape:         return sf::Keyboard::ScanEscape;
        case XK_Tab:            return sf::Keyboard::ScanTab;
        case XK_Shift_L:        return sf::Keyboard::ScanLShift;
        case XK_Shift_R:        return sf::Keyboard::ScanRShift;
        case XK_Control_L:      return sf::Keyboard::ScanLControl;
        case XK_Control_R:      return sf::Keyboard::ScanRControl;
        case XK_Meta_L:
        case XK_Alt_L:          return sf::Keyboard::ScanLAlt;
        case XK_Mode_switch: // Mapped to Alt_R on many keyboards
        case XK_ISO_Level3_Shift: // AltGr on at least some machines
        case XK_Meta_R:
        case XK_Alt_R:          return sf::Keyboard::ScanRAlt;
        case XK_Super_L:        return sf::Keyboard::ScanLSystem;
        case XK_Super_R:        return sf::Keyboard::ScanRSystem;
        case XK_Menu:           return sf::Keyboard::ScanMenu;
        case XK_Num_Lock:       return sf::Keyboard::ScanNumLock;
        case XK_Caps_Lock:      return sf::Keyboard::ScanCapsLock;
        case XK_Print:          return sf::Keyboard::ScanPrintScreen;
        case XK_Scroll_Lock:    return sf::Keyboard::ScanScrollLock;
        case XK_Pause:          return sf::Keyboard::ScanPause;
        case XK_Delete:         return sf::Keyboard::ScanDelete;
        case XK_BackSpace:      return sf::Keyboard::ScanBackspace;
        case XK_Return:         return sf::Keyboard::ScanEnter;
        case XK_Home:           return sf::Keyboard::ScanHome;
        case XK_End:            return sf::Keyboard::ScanEnd;
        case XK_Page_Up:        return sf::Keyboard::ScanPageUp;
        case XK_Page_Down:      return sf::Keyboard::ScanPageDown;
        case XK_Insert:         return sf::Keyboard::ScanInsert;
        case XK_Left:           return sf::Keyboard::ScanLeft;
        case XK_Right:          return sf::Keyboard::ScanRight;
        case XK_Down:           return sf::Keyboard::ScanDown;
        case XK_Up:             return sf::Keyboard::ScanUp;
        case XK_F1:             return sf::Keyboard::ScanF1;
        case XK_F2:             return sf::Keyboard::ScanF2;
        case XK_F3:             return sf::Keyboard::ScanF3;
        case XK_F4:             return sf::Keyboard::ScanF4;
        case XK_F5:             return sf::Keyboard::ScanF5;
        case XK_F6:             return sf::Keyboard::ScanF6;
        case XK_F7:             return sf::Keyboard::ScanF7;
        case XK_F8:             return sf::Keyboard::ScanF8;
        case XK_F9:             return sf::Keyboard::ScanF9;
        case XK_F10:            return sf::Keyboard::ScanF10;
        case XK_F11:            return sf::Keyboard::ScanF11;
        case XK_F12:            return sf::Keyboard::ScanF12;
        case XK_F13:            return sf::Keyboard::ScanF13;
        case XK_F14:            return sf::Keyboard::ScanF14;
        case XK_F15:            return sf::Keyboard::ScanF15;
        // SFML doesn't currently have these scancodes
        /* case XK_F16:            return sf::Keyboard::ScanF16;
        case XK_F17:            return sf::Keyboard::ScanF17;
        case XK_F18:            return sf::Keyboard::ScanF18;
        case XK_F19:            return sf::Keyboard::ScanF19;
        case XK_F20:            return sf::Keyboard::ScanF20;
        case XK_F21:            return sf::Keyboard::ScanF21;
        case XK_F22:            return sf::Keyboard::ScanF22;
        case XK_F23:            return sf::Keyboard::ScanF23;
        case XK_F24:            return sf::Keyboard::ScanF24;
        case XK_F25:            return sf::Keyboard::ScanF25; */

        // Numeric keypad
        case XK_KP_Divide:      return sf::Keyboard::ScanDivide;
        case XK_KP_Multiply:    return sf::Keyboard::ScanMultiply;
        case XK_KP_Subtract:    return sf::Keyboard::ScanMinus;
        case XK_KP_Add:         return sf::Keyboard::ScanPlus;

        // These should have been detected in secondary keysym test above!
        case XK_KP_Insert:      return sf::Keyboard::ScanNumpad0;
        case XK_KP_End:         return sf::Keyboard::ScanNumpad1;
        case XK_KP_Down:        return sf::Keyboard::ScanNumpad2;
        case XK_KP_Page_Down:   return sf::Keyboard::ScanNumpad3;
        case XK_KP_Left:        return sf::Keyboard::ScanNumpad4;
        case XK_KP_Right:       return sf::Keyboard::ScanNumpad6;
        case XK_KP_Home:        return sf::Keyboard::ScanNumpad7;
        case XK_KP_Up:          return sf::Keyboard::ScanNumpad8;
        case XK_KP_Page_Up:     return sf::Keyboard::ScanNumpad9;
        case XK_KP_Delete:      return sf::Keyboard::ScanDecimal;
        case XK_KP_Equal:       return sf::Keyboard::ScanPadEquals;
        case XK_KP_Enter:       return sf::Keyboard::ScanReturn;

        // Last resort: Check for printable keys (should not happen if the XKB
        // extension is available). This will give a layout dependent mapping
        // (which is wrong, and we may miss some keys, especially on non-US
        // keyboards), but it's better than nothing...
        case XK_a:              return sf::Keyboard::ScanA;
        case XK_b:              return sf::Keyboard::ScanB;
        case XK_c:              return sf::Keyboard::ScanC;
        case XK_d:              return sf::Keyboard::ScanD;
        case XK_e:              return sf::Keyboard::ScanE;
        case XK_f:              return sf::Keyboard::ScanF;
        case XK_g:              return sf::Keyboard::ScanG;
        case XK_h:              return sf::Keyboard::ScanH;
        case XK_i:              return sf::Keyboard::ScanI;
        case XK_j:              return sf::Keyboard::ScanJ;
        case XK_k:              return sf::Keyboard::ScanK;
        case XK_l:              return sf::Keyboard::ScanL;
        case XK_m:              return sf::Keyboard::ScanM;
        case XK_n:              return sf::Keyboard::ScanN;
        case XK_o:              return sf::Keyboard::ScanO;
        case XK_p:              return sf::Keyboard::ScanP;
        case XK_q:              return sf::Keyboard::ScanQ;
        case XK_r:              return sf::Keyboard::ScanR;
        case XK_s:              return sf::Keyboard::ScanS;
        case XK_t:              return sf::Keyboard::ScanT;
        case XK_u:              return sf::Keyboard::ScanU;
        case XK_v:              return sf::Keyboard::ScanV;
        case XK_w:              return sf::Keyboard::ScanW;
        case XK_x:              return sf::Keyboard::ScanX;
        case XK_y:              return sf::Keyboard::ScanY;
        case XK_z:              return sf::Keyboard::ScanZ;
        case XK_1:              return sf::Keyboard::ScanNum1;
        case XK_2:              return sf::Keyboard::ScanNum2;
        case XK_3:              return sf::Keyboard::ScanNum3;
        case XK_4:              return sf::Keyboard::ScanNum4;
        case XK_5:              return sf::Keyboard::ScanNum5;
        case XK_6:              return sf::Keyboard::ScanNum6;
        case XK_7:              return sf::Keyboard::ScanNum7;
        case XK_8:              return sf::Keyboard::ScanNum8;
        case XK_9:              return sf::Keyboard::ScanNum9;
        case XK_0:              return sf::Keyboard::ScanNum0;
        case XK_space:          return sf::Keyboard::ScanSpace;
        case XK_minus:          return sf::Keyboard::ScanHyphen;
        case XK_equal:          return sf::Keyboard::ScanEquals;
        case XK_bracketleft:    return sf::Keyboard::ScanLBracket;
        case XK_bracketright:   return sf::Keyboard::ScanRBracket;
        case XK_backslash:      return sf::Keyboard::ScanBackslash;
        case XK_semicolon:      return sf::Keyboard::ScanSemicolon;
        case XK_apostrophe:     return sf::Keyboard::ScanQuote;
        case XK_grave:          return sf::Keyboard::ScanGraveAccent;
        case XK_comma:          return sf::Keyboard::ScanComma;
        case XK_period:         return sf::Keyboard::ScanPeriod;
        case XK_slash:          return sf::Keyboard::ScanForwardSlash;
        // case XK_less:           return sf::Keyboard::ScanWorld1; // At least in some layouts...
        default:                break;
    }

    // No matching translation was found
    return sf::Keyboard::ScanUnknown;
}


} // anonymous namespace


////////////////////////////////////////////////////////////
X11InputManager::X11InputManager() :
    m_display(NULL)
{
    for (int i = 0; i < sf::Keyboard::ScanCodeCount; ++i)
    {
        m_scancodeToKeycode[i] = 0;
    }

    for (int i = 0; i < 256; ++i)
    {
        m_keycodeToScancode[i] = sf::Keyboard::ScanUnknown;
    }
}


////////////////////////////////////////////////////////////
X11InputManager& X11InputManager::getInstance()
{
    static X11InputManager instance;
    return instance;
}


////////////////////////////////////////////////////////////
void X11InputManager::initialize(Display* display)
{
    m_display = display;

    // Find the X11 key code -> SFML key code mapping
    // This code was inspired by GLFW implementation

    char name[XkbKeyNameLength + 1];
    XkbDescPtr desc = XkbGetMap(m_display, 0, XkbUseCoreKbd);
    XkbGetNames(m_display, XkbKeyNamesMask, desc);

    sf::Keyboard::Scancode sc;
    for (int keycode = desc->min_key_code; keycode <= desc->max_key_code; ++keycode)
    {
        std::memcpy(name, desc->names->keys[keycode].name, XkbKeyNameLength);
        name[XkbKeyNameLength] = '\0';

        if (strcmp(name, "TLDE") == 0)      sc = sf::Keyboard::ScanGraveAccent;
        else if (strcmp(name, "AE01") == 0) sc = sf::Keyboard::ScanNum1;
        else if (strcmp(name, "AE02") == 0) sc = sf::Keyboard::ScanNum2;
        else if (strcmp(name, "AE03") == 0) sc = sf::Keyboard::ScanNum3;
        else if (strcmp(name, "AE04") == 0) sc = sf::Keyboard::ScanNum4;
        else if (strcmp(name, "AE05") == 0) sc = sf::Keyboard::ScanNum5;
        else if (strcmp(name, "AE06") == 0) sc = sf::Keyboard::ScanNum6;
        else if (strcmp(name, "AE07") == 0) sc = sf::Keyboard::ScanNum7;
        else if (strcmp(name, "AE08") == 0) sc = sf::Keyboard::ScanNum8;
        else if (strcmp(name, "AE09") == 0) sc = sf::Keyboard::ScanNum9;
        else if (strcmp(name, "AE10") == 0) sc = sf::Keyboard::ScanNum0;
        else if (strcmp(name, "AE11") == 0) sc = sf::Keyboard::ScanDash;
        else if (strcmp(name, "AE12") == 0) sc = sf::Keyboard::ScanEquals;
        else if (strcmp(name, "TAB") == 0)  sc = sf::Keyboard::ScanTab;
        else if (strcmp(name, "AD01") == 0) sc = sf::Keyboard::ScanQ;
        else if (strcmp(name, "AD02") == 0) sc = sf::Keyboard::ScanW;
        else if (strcmp(name, "AD03") == 0) sc = sf::Keyboard::ScanE;
        else if (strcmp(name, "AD04") == 0) sc = sf::Keyboard::ScanR;
        else if (strcmp(name, "AD05") == 0) sc = sf::Keyboard::ScanT;
        else if (strcmp(name, "AD06") == 0) sc = sf::Keyboard::ScanY;
        else if (strcmp(name, "AD07") == 0) sc = sf::Keyboard::ScanU;
        else if (strcmp(name, "AD08") == 0) sc = sf::Keyboard::ScanI;
        else if (strcmp(name, "AD09") == 0) sc = sf::Keyboard::ScanO;
        else if (strcmp(name, "AD10") == 0) sc = sf::Keyboard::ScanP;
        else if (strcmp(name, "AD11") == 0) sc = sf::Keyboard::ScanLBracket;
        else if (strcmp(name, "AD12") == 0) sc = sf::Keyboard::ScanRBracket;
        else if (strcmp(name, "BKSL") == 0) sc = sf::Keyboard::ScanBackslash;
        else if (strcmp(name, "AC01") == 0) sc = sf::Keyboard::ScanA;
        else if (strcmp(name, "AC02") == 0) sc = sf::Keyboard::ScanS;
        else if (strcmp(name, "AC03") == 0) sc = sf::Keyboard::ScanD;
        else if (strcmp(name, "AC04") == 0) sc = sf::Keyboard::ScanF;
        else if (strcmp(name, "AC05") == 0) sc = sf::Keyboard::ScanG;
        else if (strcmp(name, "AC06") == 0) sc = sf::Keyboard::ScanH;
        else if (strcmp(name, "AC07") == 0) sc = sf::Keyboard::ScanJ;
        else if (strcmp(name, "AC08") == 0) sc = sf::Keyboard::ScanK;
        else if (strcmp(name, "AC09") == 0) sc = sf::Keyboard::ScanL;
        else if (strcmp(name, "AC10") == 0) sc = sf::Keyboard::ScanSemicolon;
        else if (strcmp(name, "AC11") == 0) sc = sf::Keyboard::ScanQuote;
        else if (strcmp(name, "AB01") == 0) sc = sf::Keyboard::ScanZ;
        else if (strcmp(name, "AB02") == 0) sc = sf::Keyboard::ScanX;
        else if (strcmp(name, "AB03") == 0) sc = sf::Keyboard::ScanC;
        else if (strcmp(name, "AB04") == 0) sc = sf::Keyboard::ScanV;
        else if (strcmp(name, "AB05") == 0) sc = sf::Keyboard::ScanB;
        else if (strcmp(name, "AB06") == 0) sc = sf::Keyboard::ScanN;
        else if (strcmp(name, "AB07") == 0) sc = sf::Keyboard::ScanM;
        else if (strcmp(name, "AB08") == 0) sc = sf::Keyboard::ScanComma;
        else if (strcmp(name, "AB09") == 0) sc = sf::Keyboard::ScanPeriod;
        else if (strcmp(name, "AB10") == 0) sc = sf::Keyboard::ScanForwardSlash;
        else sc = sf::Keyboard::ScanUnknown;

        if ((keycode >= 0) && (keycode < 256))
        {
            m_scancodeToKeycode[sc] = keycode;
            m_keycodeToScancode[keycode] = sc;
        }
    }

    XkbFreeNames(desc, XkbKeyNamesMask, True);
    XkbFreeKeyboard(desc, 0, True);

    // Translate un-translated keycodes using traditional X11 KeySym lookups
    for (int keycode = 0;  keycode < 256;  ++keycode)
    {
        if (m_keycodeToScancode[keycode] == sf::Keyboard::ScanUnknown)
        {
            sf::Keyboard::Scancode sc = translateKeyCode(m_display, keycode);
            m_scancodeToKeycode[sc] = keycode;
            m_keycodeToScancode[keycode] = sc;
        }
    }
}



namespace {
////////////////////////////////////////////////////////////
bool isKeyPressedImpl(Display* display, KeyCode keycode)
{
    if (keycode != 0)
    {
        // Get the whole keyboard state
        char keys[32];
        XQueryKeymap(display, keys);

        // Check our keycode
        return (keys[keycode / 8] & (1 << (keycode % 8))) != 0;
    }
    return false;
}

} // anonymous namespace

////////////////////////////////////////////////////////////
bool X11InputManager::isKeyPressed(sf::Keyboard::Key key) const
{
    KeyCode keycode = SFtoKeyCode(key);
    return isKeyPressedImpl(m_display, keycode);
}


////////////////////////////////////////////////////////////
bool X11InputManager::isKeyPressed(sf::Keyboard::Scancode code) const
{
    KeyCode keycode = SFtoKeyCode(code);
    return isKeyPressedImpl(m_display, keycode);
}


////////////////////////////////////////////////////////////
sf::Keyboard::Scancode X11InputManager::unlocalize(sf::Keyboard::Key key) const
{
    KeyCode keycode = SFtoKeyCode(key);
    return keyCodeToSF(keycode);
}


////////////////////////////////////////////////////////////
sf::Keyboard::Key X11InputManager::localize(sf::Keyboard::Scancode code) const
{
    KeyCode keycode = SFtoKeyCode(code);
    KeySym keysym = XkbKeycodeToKeysym(m_display, keycode, 0, 0);
    return keysymToSF(keysym);
}


////////////////////////////////////////////////////////////
sf::String X11InputManager::getDescription(Keyboard::Scancode code) const
{
    return ""; // TODO
}


////////////////////////////////////////////////////////////
sf::Keyboard::Key X11InputManager::getKeyFromEvent(XKeyEvent& event) const
{
    sf::Keyboard::Key key = Keyboard::Unknown;

    // Try each KeySym index (modifier group) until we get a match
    for (int i = 0; i < 4; ++i)
    {
        // Get the SFML keyboard code from the keysym of the key that has been pressed
        KeySym keysym = XLookupKeysym(&event, i);
        key = keysymToSF(keysym);

        if (key != Keyboard::Unknown)
            break;
    }
    return key;
}


////////////////////////////////////////////////////////////
sf::Keyboard::Scancode X11InputManager::getScancodeFromEvent(XKeyEvent& event) const
{
    return keyCodeToSF(event.keycode);
}


////////////////////////////////////////////////////////////
KeyCode X11InputManager::SFtoKeyCode(sf::Keyboard::Key key) const
{
    KeySym keysym = SFtoKeysym(key);
    return XKeysymToKeycode(m_display, keysym);
}


////////////////////////////////////////////////////////////
KeyCode X11InputManager::SFtoKeyCode(sf::Keyboard::Scancode code) const
{
    return m_scancodeToKeycode[code];
}


////////////////////////////////////////////////////////////
sf::Keyboard::Scancode X11InputManager::keyCodeToSF(KeyCode code) const
{
    return m_keycodeToScancode[code];
}

} // namespace priv
} // namespace sf
