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
#include <SFML/Window/Unix/Display.hpp>
#include <SFML/Window/Unix/KeySymToKeyMapping.hpp>
#include <SFML/Window/Unix/KeySymToUnicodeMapping.hpp>
#include <SFML/Window/Unix/KeyboardImpl.hpp>

#include <SFML/System/EnumArray.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Utf.hpp>

#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#include <array>
#include <string>
#include <unordered_map>
#include <utility>

#include <cstring>

namespace
{

constexpr KeyCode nullKeyCode = 0;
constexpr int     maxKeyCode  = 256;
sf::priv::EnumArray<sf::Keyboard::Scancode, KeyCode, sf::Keyboard::ScancodeCount> scancodeToKeycode; ///< Mapping of SFML scancode to X11 KeyCode
std::array<sf::Keyboard::Scancode, maxKeyCode> keycodeToScancode; ///< Mapping of X11 KeyCode to SFML scancode

////////////////////////////////////////////////////////////
bool isValidKeycode(KeyCode keycode)
{
    // Valid key code range is [8,255], according to the Xlib manual
    return keycode >= 8;
}


////////////////////////////////////////////////////////////
sf::Keyboard::Scancode translateKeyCode(Display* display, KeyCode keycode)
{
    if (!isValidKeycode(keycode))
        return sf::Keyboard::Scan::Unknown;

    // Try secondary keysym, for numeric keypad keys
    // Note: This way we always force "NumLock = ON", which is intentional
    // since the returned key code should correspond to a physical location.
    KeySym keySym = XkbKeycodeToKeysym(display, keycode, 0, 1);

    // clang-format off
    switch (keySym)
    {
        case XK_KP_0:         return sf::Keyboard::Scan::Numpad0;
        case XK_KP_1:         return sf::Keyboard::Scan::Numpad1;
        case XK_KP_2:         return sf::Keyboard::Scan::Numpad2;
        case XK_KP_3:         return sf::Keyboard::Scan::Numpad3;
        case XK_KP_4:         return sf::Keyboard::Scan::Numpad4;
        case XK_KP_5:         return sf::Keyboard::Scan::Numpad5;
        case XK_KP_6:         return sf::Keyboard::Scan::Numpad6;
        case XK_KP_7:         return sf::Keyboard::Scan::Numpad7;
        case XK_KP_8:         return sf::Keyboard::Scan::Numpad8;
        case XK_KP_9:         return sf::Keyboard::Scan::Numpad9;
        case XK_KP_Separator: return sf::Keyboard::Scan::NumpadDecimal;
        case XK_KP_Decimal:   return sf::Keyboard::Scan::NumpadDecimal;
        case XK_KP_Equal:     return sf::Keyboard::Scan::NumpadEqual;
        case XK_KP_Enter:     return sf::Keyboard::Scan::NumpadEnter;
        default:              break;
    }
    // clang-format on

    // Now try primary keysym for function keys (non-printable keys)
    // These should not depend on the current keyboard layout
    keySym = XkbKeycodeToKeysym(display, keycode, 0, 0);

    // clang-format off
    switch (keySym)
    {
        case XK_Return:           return sf::Keyboard::Scan::Enter;
        case XK_Escape:           return sf::Keyboard::Scan::Escape;
        case XK_BackSpace:        return sf::Keyboard::Scan::Backspace;
        case XK_Tab:              return sf::Keyboard::Scan::Tab;
        case XK_Shift_L:          return sf::Keyboard::Scan::LShift;
        case XK_Shift_R:          return sf::Keyboard::Scan::RShift;
        case XK_Control_L:        return sf::Keyboard::Scan::LControl;
        case XK_Control_R:        return sf::Keyboard::Scan::RControl;
        case XK_Alt_L:            return sf::Keyboard::Scan::LAlt;
        case XK_ISO_Level3_Shift: // AltGr on at least some machines
        case XK_Alt_R:            return sf::Keyboard::Scan::RAlt;
        case XK_Meta_L:
        case XK_Super_L:          return sf::Keyboard::Scan::LSystem;
        case XK_Meta_R:
        case XK_Super_R:          return sf::Keyboard::Scan::RSystem;
        case XK_Menu:             return sf::Keyboard::Scan::Menu;

        case XK_Num_Lock:         return sf::Keyboard::Scan::NumLock;
        case XK_Caps_Lock:        return sf::Keyboard::Scan::CapsLock;
        case XK_Execute:          return sf::Keyboard::Scan::Execute;
        case XK_Hyper_R:          return sf::Keyboard::Scan::Application;
        case XK_Select:           return sf::Keyboard::Scan::Select;
        case XK_Cancel:           return sf::Keyboard::Scan::Stop;
        case XK_Redo:             return sf::Keyboard::Scan::Redo;
        case XK_Undo:             return sf::Keyboard::Scan::Undo;
        case XK_Find:             return sf::Keyboard::Scan::Search;
        case XK_Mode_switch:      return sf::Keyboard::Scan::ModeChange;

        case XK_Print:            return sf::Keyboard::Scan::PrintScreen;
        case XK_Scroll_Lock:      return sf::Keyboard::Scan::ScrollLock;
        case XK_Pause:
        case XK_Break:            return sf::Keyboard::Scan::Pause;

        case XK_Delete:
        case XK_Clear:            return sf::Keyboard::Scan::Delete;
        case XK_Home:             return sf::Keyboard::Scan::Home;
        case XK_End:              return sf::Keyboard::Scan::End;
        case XK_Page_Up:          return sf::Keyboard::Scan::PageUp;
        case XK_Page_Down:        return sf::Keyboard::Scan::PageDown;
        case XK_Insert:           return sf::Keyboard::Scan::Insert;

        case XK_Left:             return sf::Keyboard::Scan::Left;
        case XK_Right:            return sf::Keyboard::Scan::Right;
        case XK_Down:             return sf::Keyboard::Scan::Down;
        case XK_Up:               return sf::Keyboard::Scan::Up;

        case XK_F1:               return sf::Keyboard::Scan::F1;
        case XK_F2:               return sf::Keyboard::Scan::F2;
        case XK_F3:               return sf::Keyboard::Scan::F3;
        case XK_F4:               return sf::Keyboard::Scan::F4;
        case XK_F5:               return sf::Keyboard::Scan::F5;
        case XK_F6:               return sf::Keyboard::Scan::F6;
        case XK_F7:               return sf::Keyboard::Scan::F7;
        case XK_F8:               return sf::Keyboard::Scan::F8;
        case XK_F9:               return sf::Keyboard::Scan::F9;
        case XK_F10:              return sf::Keyboard::Scan::F10;
        case XK_F11:              return sf::Keyboard::Scan::F11;
        case XK_F12:              return sf::Keyboard::Scan::F12;
        case XK_F13:              return sf::Keyboard::Scan::F13;
        case XK_F14:              return sf::Keyboard::Scan::F14;
        case XK_F15:              return sf::Keyboard::Scan::F15;
        case XK_F16:              return sf::Keyboard::Scan::F16;
        case XK_F17:              return sf::Keyboard::Scan::F17;
        case XK_F18:              return sf::Keyboard::Scan::F18;
        case XK_F19:              return sf::Keyboard::Scan::F19;
        case XK_F20:              return sf::Keyboard::Scan::F20;
        case XK_F21:              return sf::Keyboard::Scan::F21;
        case XK_F22:              return sf::Keyboard::Scan::F22;
        case XK_F23:              return sf::Keyboard::Scan::F23;
        case XK_F24:              return sf::Keyboard::Scan::F24;

        // Numeric keypad
        case XK_KP_Divide:        return sf::Keyboard::Scan::NumpadDivide;
        case XK_KP_Multiply:      return sf::Keyboard::Scan::NumpadMultiply;
        case XK_KP_Subtract:      return sf::Keyboard::Scan::NumpadMinus;
        case XK_KP_Add:           return sf::Keyboard::Scan::NumpadPlus;

        // These should have been detected in secondary keysym test above!
        case XK_KP_Insert:        return sf::Keyboard::Scan::Numpad0;
        case XK_KP_End:           return sf::Keyboard::Scan::Numpad1;
        case XK_KP_Down:          return sf::Keyboard::Scan::Numpad2;
        case XK_KP_Page_Down:     return sf::Keyboard::Scan::Numpad3;
        case XK_KP_Left:          return sf::Keyboard::Scan::Numpad4;
        case XK_KP_Right:         return sf::Keyboard::Scan::Numpad6;
        case XK_KP_Home:          return sf::Keyboard::Scan::Numpad7;
        case XK_KP_Up:            return sf::Keyboard::Scan::Numpad8;
        case XK_KP_Page_Up:       return sf::Keyboard::Scan::Numpad9;
        case XK_KP_Delete:        return sf::Keyboard::Scan::NumpadDecimal;
        case XK_KP_Equal:         return sf::Keyboard::Scan::NumpadEqual;
        case XK_KP_Enter:         return sf::Keyboard::Scan::NumpadEnter;

        // Last resort: Check for printable keys (should not happen if the XKB
        // extension is available). This will give a layout dependent mapping
        // (which is wrong, and we may miss some keys, especially on non-US
        // keyboards), but it's better than nothing...
        case XK_a:
        case XK_A:                return sf::Keyboard::Scan::A;
        case XK_b:
        case XK_B:                return sf::Keyboard::Scan::B;
        case XK_c:
        case XK_C:                return sf::Keyboard::Scan::C;
        case XK_d:
        case XK_D:                return sf::Keyboard::Scan::D;
        case XK_e:
        case XK_E:                return sf::Keyboard::Scan::E;
        case XK_f:
        case XK_F:                return sf::Keyboard::Scan::F;
        case XK_g:
        case XK_G:                return sf::Keyboard::Scan::G;
        case XK_h:
        case XK_H:                return sf::Keyboard::Scan::H;
        case XK_i:
        case XK_I:                return sf::Keyboard::Scan::I;
        case XK_j:
        case XK_J:                return sf::Keyboard::Scan::J;
        case XK_k:
        case XK_K:                return sf::Keyboard::Scan::K;
        case XK_l:
        case XK_L:                return sf::Keyboard::Scan::L;
        case XK_m:
        case XK_M:                return sf::Keyboard::Scan::M;
        case XK_n:
        case XK_N:                return sf::Keyboard::Scan::N;
        case XK_o:
        case XK_O:                return sf::Keyboard::Scan::O;
        case XK_p:
        case XK_P:                return sf::Keyboard::Scan::P;
        case XK_q:
        case XK_Q:                return sf::Keyboard::Scan::Q;
        case XK_r:
        case XK_R:                return sf::Keyboard::Scan::R;
        case XK_s:
        case XK_S:                return sf::Keyboard::Scan::S;
        case XK_t:
        case XK_T:                return sf::Keyboard::Scan::T;
        case XK_u:
        case XK_U:                return sf::Keyboard::Scan::U;
        case XK_v:
        case XK_V:                return sf::Keyboard::Scan::V;
        case XK_w:
        case XK_W:                return sf::Keyboard::Scan::W;
        case XK_x:
        case XK_X:                return sf::Keyboard::Scan::X;
        case XK_y:
        case XK_Y:                return sf::Keyboard::Scan::Y;
        case XK_z:
        case XK_Z:                return sf::Keyboard::Scan::Z;

        case XK_1:                return sf::Keyboard::Scan::Num1;
        case XK_2:                return sf::Keyboard::Scan::Num2;
        case XK_3:                return sf::Keyboard::Scan::Num3;
        case XK_4:                return sf::Keyboard::Scan::Num4;
        case XK_5:                return sf::Keyboard::Scan::Num5;
        case XK_6:                return sf::Keyboard::Scan::Num6;
        case XK_7:                return sf::Keyboard::Scan::Num7;
        case XK_8:                return sf::Keyboard::Scan::Num8;
        case XK_9:                return sf::Keyboard::Scan::Num9;
        case XK_0:                return sf::Keyboard::Scan::Num0;

        case XK_space:            return sf::Keyboard::Scan::Space;
        case XK_minus:            return sf::Keyboard::Scan::Hyphen;
        case XK_equal:            return sf::Keyboard::Scan::Equal;
        case XK_bracketleft:      return sf::Keyboard::Scan::LBracket;
        case XK_bracketright:     return sf::Keyboard::Scan::RBracket;
        case XK_backslash:        return sf::Keyboard::Scan::Backslash;
        case XK_semicolon:        return sf::Keyboard::Scan::Semicolon;
        case XK_apostrophe:       return sf::Keyboard::Scan::Apostrophe;
        case XK_grave:            return sf::Keyboard::Scan::Grave;
        case XK_comma:            return sf::Keyboard::Scan::Comma;
        case XK_period:           return sf::Keyboard::Scan::Period;
        case XK_slash:            return sf::Keyboard::Scan::Slash;
        case XK_less:             return sf::Keyboard::Scan::NonUsBackslash;

        default:                  return sf::Keyboard::Scan::Unknown;
    }
    // clang-format on
}

////////////////////////////////////////////////////////////
std::unordered_map<std::string, sf::Keyboard::Scancode> getNameScancodeMap()
{
    std::unordered_map<std::string, sf::Keyboard::Scancode> mapping;

    mapping.try_emplace("LSGT", sf::Keyboard::Scan::NonUsBackslash);

    mapping.try_emplace("TLDE", sf::Keyboard::Scan::Grave);
    mapping.try_emplace("AE01", sf::Keyboard::Scan::Num1);
    mapping.try_emplace("AE02", sf::Keyboard::Scan::Num2);
    mapping.try_emplace("AE03", sf::Keyboard::Scan::Num3);
    mapping.try_emplace("AE04", sf::Keyboard::Scan::Num4);
    mapping.try_emplace("AE05", sf::Keyboard::Scan::Num5);
    mapping.try_emplace("AE06", sf::Keyboard::Scan::Num6);
    mapping.try_emplace("AE07", sf::Keyboard::Scan::Num7);
    mapping.try_emplace("AE08", sf::Keyboard::Scan::Num8);
    mapping.try_emplace("AE09", sf::Keyboard::Scan::Num9);
    mapping.try_emplace("AE10", sf::Keyboard::Scan::Num0);
    mapping.try_emplace("AE11", sf::Keyboard::Scan::Hyphen);
    mapping.try_emplace("AE12", sf::Keyboard::Scan::Equal);
    mapping.try_emplace("BKSP", sf::Keyboard::Scan::Backspace);
    mapping.try_emplace("TAB", sf::Keyboard::Scan::Tab);
    mapping.try_emplace("AD01", sf::Keyboard::Scan::Q);
    mapping.try_emplace("AD02", sf::Keyboard::Scan::W);
    mapping.try_emplace("AD03", sf::Keyboard::Scan::E);
    mapping.try_emplace("AD04", sf::Keyboard::Scan::R);
    mapping.try_emplace("AD05", sf::Keyboard::Scan::T);
    mapping.try_emplace("AD06", sf::Keyboard::Scan::Y);
    mapping.try_emplace("AD07", sf::Keyboard::Scan::U);
    mapping.try_emplace("AD08", sf::Keyboard::Scan::I);
    mapping.try_emplace("AD09", sf::Keyboard::Scan::O);
    mapping.try_emplace("AD10", sf::Keyboard::Scan::P);
    mapping.try_emplace("AD11", sf::Keyboard::Scan::LBracket);
    mapping.try_emplace("AD12", sf::Keyboard::Scan::RBracket);
    mapping.try_emplace("BKSL", sf::Keyboard::Scan::Backslash);
    mapping.try_emplace("RTRN", sf::Keyboard::Scan::Enter);

    mapping.try_emplace("CAPS", sf::Keyboard::Scan::CapsLock);
    mapping.try_emplace("AC01", sf::Keyboard::Scan::A);
    mapping.try_emplace("AC02", sf::Keyboard::Scan::S);
    mapping.try_emplace("AC03", sf::Keyboard::Scan::D);
    mapping.try_emplace("AC04", sf::Keyboard::Scan::F);
    mapping.try_emplace("AC05", sf::Keyboard::Scan::G);
    mapping.try_emplace("AC06", sf::Keyboard::Scan::H);
    mapping.try_emplace("AC07", sf::Keyboard::Scan::J);
    mapping.try_emplace("AC08", sf::Keyboard::Scan::K);
    mapping.try_emplace("AC09", sf::Keyboard::Scan::L);
    mapping.try_emplace("AC10", sf::Keyboard::Scan::Semicolon);
    mapping.try_emplace("AC11", sf::Keyboard::Scan::Apostrophe);
    mapping.try_emplace("AC12", sf::Keyboard::Scan::Backslash);

    mapping.try_emplace("LFSH", sf::Keyboard::Scan::LShift);
    mapping.try_emplace("AB01", sf::Keyboard::Scan::Z);
    mapping.try_emplace("AB02", sf::Keyboard::Scan::X);
    mapping.try_emplace("AB03", sf::Keyboard::Scan::C);
    mapping.try_emplace("AB04", sf::Keyboard::Scan::V);
    mapping.try_emplace("AB05", sf::Keyboard::Scan::B);
    mapping.try_emplace("AB06", sf::Keyboard::Scan::N);
    mapping.try_emplace("AB07", sf::Keyboard::Scan::M);
    mapping.try_emplace("AB08", sf::Keyboard::Scan::Comma);
    mapping.try_emplace("AB09", sf::Keyboard::Scan::Period);
    mapping.try_emplace("AB10", sf::Keyboard::Scan::Slash);
    mapping.try_emplace("RTSH", sf::Keyboard::Scan::RShift);

    mapping.try_emplace("LCTL", sf::Keyboard::Scan::LControl);
    mapping.try_emplace("LALT", sf::Keyboard::Scan::LAlt);
    mapping.try_emplace("SPCE", sf::Keyboard::Scan::Space);
    mapping.try_emplace("RCTL", sf::Keyboard::Scan::RControl);
    mapping.try_emplace("RALT", sf::Keyboard::Scan::RAlt);
    mapping.try_emplace("LVL3", sf::Keyboard::Scan::RAlt);
    mapping.try_emplace("ALGR", sf::Keyboard::Scan::RAlt);
    mapping.try_emplace("LWIN", sf::Keyboard::Scan::LSystem);
    mapping.try_emplace("RWIN", sf::Keyboard::Scan::RSystem);

    mapping.try_emplace("HYPR", sf::Keyboard::Scan::Application);
    mapping.try_emplace("EXEC", sf::Keyboard::Scan::Execute);
    mapping.try_emplace("MDSW", sf::Keyboard::Scan::ModeChange);
    mapping.try_emplace("MENU", sf::Keyboard::Scan::Menu);
    mapping.try_emplace("COMP", sf::Keyboard::Scan::Menu);
    mapping.try_emplace("SELE", sf::Keyboard::Scan::Select);

    mapping.try_emplace("ESC", sf::Keyboard::Scan::Escape);
    mapping.try_emplace("FK01", sf::Keyboard::Scan::F1);
    mapping.try_emplace("FK02", sf::Keyboard::Scan::F2);
    mapping.try_emplace("FK03", sf::Keyboard::Scan::F3);
    mapping.try_emplace("FK04", sf::Keyboard::Scan::F4);
    mapping.try_emplace("FK05", sf::Keyboard::Scan::F5);
    mapping.try_emplace("FK06", sf::Keyboard::Scan::F6);
    mapping.try_emplace("FK07", sf::Keyboard::Scan::F7);
    mapping.try_emplace("FK08", sf::Keyboard::Scan::F8);
    mapping.try_emplace("FK09", sf::Keyboard::Scan::F9);
    mapping.try_emplace("FK10", sf::Keyboard::Scan::F10);
    mapping.try_emplace("FK11", sf::Keyboard::Scan::F11);
    mapping.try_emplace("FK12", sf::Keyboard::Scan::F12);

    mapping.try_emplace("PRSC", sf::Keyboard::Scan::PrintScreen);
    mapping.try_emplace("SCLK", sf::Keyboard::Scan::ScrollLock);
    mapping.try_emplace("PAUS", sf::Keyboard::Scan::Pause);

    mapping.try_emplace("INS", sf::Keyboard::Scan::Insert);
    mapping.try_emplace("HOME", sf::Keyboard::Scan::Home);
    mapping.try_emplace("PGUP", sf::Keyboard::Scan::PageUp);
    mapping.try_emplace("DELE", sf::Keyboard::Scan::Delete);
    mapping.try_emplace("END", sf::Keyboard::Scan::End);
    mapping.try_emplace("PGDN", sf::Keyboard::Scan::PageDown);

    mapping.try_emplace("UP", sf::Keyboard::Scan::Up);
    mapping.try_emplace("RGHT", sf::Keyboard::Scan::Right);
    mapping.try_emplace("DOWN", sf::Keyboard::Scan::Down);
    mapping.try_emplace("LEFT", sf::Keyboard::Scan::Left);

    mapping.try_emplace("NMLK", sf::Keyboard::Scan::NumLock);
    mapping.try_emplace("KPDV", sf::Keyboard::Scan::NumpadDivide);
    mapping.try_emplace("KPMU", sf::Keyboard::Scan::NumpadMultiply);
    mapping.try_emplace("KPSU", sf::Keyboard::Scan::NumpadMinus);

    mapping.try_emplace("KP7", sf::Keyboard::Scan::Numpad7);
    mapping.try_emplace("KP8", sf::Keyboard::Scan::Numpad8);
    mapping.try_emplace("KP9", sf::Keyboard::Scan::Numpad9);
    mapping.try_emplace("KPAD", sf::Keyboard::Scan::NumpadPlus);
    mapping.try_emplace("KP4", sf::Keyboard::Scan::Numpad4);
    mapping.try_emplace("KP5", sf::Keyboard::Scan::Numpad5);
    mapping.try_emplace("KP6", sf::Keyboard::Scan::Numpad6);
    mapping.try_emplace("KP1", sf::Keyboard::Scan::Numpad1);
    mapping.try_emplace("KP2", sf::Keyboard::Scan::Numpad2);
    mapping.try_emplace("KP3", sf::Keyboard::Scan::Numpad3);
    mapping.try_emplace("KPEN", sf::Keyboard::Scan::NumpadEnter);
    mapping.try_emplace("KP0", sf::Keyboard::Scan::Numpad0);
    mapping.try_emplace("KPDL", sf::Keyboard::Scan::NumpadDecimal);
    mapping.try_emplace("KPEQ", sf::Keyboard::Scan::NumpadEqual);

    mapping.try_emplace("FK13", sf::Keyboard::Scan::F13);
    mapping.try_emplace("FK14", sf::Keyboard::Scan::F14);
    mapping.try_emplace("FK15", sf::Keyboard::Scan::F15);
    mapping.try_emplace("FK16", sf::Keyboard::Scan::F16);
    mapping.try_emplace("FK17", sf::Keyboard::Scan::F17);
    mapping.try_emplace("FK18", sf::Keyboard::Scan::F18);
    mapping.try_emplace("FK19", sf::Keyboard::Scan::F19);
    mapping.try_emplace("FK20", sf::Keyboard::Scan::F20);
    mapping.try_emplace("FK21", sf::Keyboard::Scan::F21);
    mapping.try_emplace("FK22", sf::Keyboard::Scan::F22);
    mapping.try_emplace("FK23", sf::Keyboard::Scan::F23);
    mapping.try_emplace("FK24", sf::Keyboard::Scan::F24);
    mapping.try_emplace("LMTA", sf::Keyboard::Scan::LSystem);
    mapping.try_emplace("RMTA", sf::Keyboard::Scan::RSystem);
    mapping.try_emplace("MUTE", sf::Keyboard::Scan::VolumeMute);
    mapping.try_emplace("VOL-", sf::Keyboard::Scan::VolumeDown);
    mapping.try_emplace("VOL+", sf::Keyboard::Scan::VolumeUp);
    mapping.try_emplace("STOP", sf::Keyboard::Scan::Stop);
    mapping.try_emplace("REDO", sf::Keyboard::Scan::Redo);
    mapping.try_emplace("AGAI", sf::Keyboard::Scan::Redo);
    mapping.try_emplace("UNDO", sf::Keyboard::Scan::Undo);
    mapping.try_emplace("COPY", sf::Keyboard::Scan::Copy);
    mapping.try_emplace("PAST", sf::Keyboard::Scan::Paste);
    mapping.try_emplace("FIND", sf::Keyboard::Scan::Search);
    mapping.try_emplace("CUT", sf::Keyboard::Scan::Cut);
    mapping.try_emplace("HELP", sf::Keyboard::Scan::Help);

    mapping.try_emplace("I156", sf::Keyboard::Scan::LaunchApplication1);
    mapping.try_emplace("I157", sf::Keyboard::Scan::LaunchApplication2);
    mapping.try_emplace("I164", sf::Keyboard::Scan::Favorites);
    mapping.try_emplace("I166", sf::Keyboard::Scan::Back);
    mapping.try_emplace("I167", sf::Keyboard::Scan::Forward);
    mapping.try_emplace("I171", sf::Keyboard::Scan::MediaNextTrack);
    mapping.try_emplace("I172", sf::Keyboard::Scan::MediaPlayPause);
    mapping.try_emplace("I173", sf::Keyboard::Scan::MediaPreviousTrack);
    mapping.try_emplace("I174", sf::Keyboard::Scan::MediaStop);
    mapping.try_emplace("I180", sf::Keyboard::Scan::HomePage);
    mapping.try_emplace("I181", sf::Keyboard::Scan::Refresh);
    mapping.try_emplace("I223", sf::Keyboard::Scan::LaunchMail);
    mapping.try_emplace("I234", sf::Keyboard::Scan::LaunchMediaSelect);

    return mapping;
}

////////////////////////////////////////////////////////////
void ensureMapping()
{
    static bool isMappingInitialized = false;

    if (isMappingInitialized)
        return;

    // Phase 1: Initialize mappings with default values
    scancodeToKeycode.fill(nullKeyCode);
    keycodeToScancode.fill(sf::Keyboard::Scan::Unknown);

    // Phase 2: Get XKB names with key code
    const auto display = sf::priv::openDisplay();

    std::array<char, XkbKeyNameLength + 1> name{};
    XkbDescPtr                             descriptor = XkbGetMap(display.get(), 0, XkbUseCoreKbd);
    XkbGetNames(display.get(), XkbKeyNamesMask, descriptor);

    std::unordered_map<std::string, sf::Keyboard::Scancode> nameScancodeMap = getNameScancodeMap();

    for (int keycode = descriptor->min_key_code; keycode <= descriptor->max_key_code; ++keycode)
    {
        if (!isValidKeycode(static_cast<KeyCode>(keycode)))
        {
            continue;
        }

        std::memcpy(name.data(), descriptor->names->keys[keycode].name, XkbKeyNameLength);
        name[XkbKeyNameLength] = '\0';

        const auto mappedScancode = nameScancodeMap.find(std::string(name.data()));
        auto       scancode       = sf::Keyboard::Scan::Unknown;

        if (mappedScancode != nameScancodeMap.end())
            scancode = mappedScancode->second;

        if (scancode != sf::Keyboard::Scan::Unknown)
            scancodeToKeycode[scancode] = static_cast<KeyCode>(keycode);

        keycodeToScancode[static_cast<KeyCode>(keycode)] = scancode;
    }

    XkbFreeNames(descriptor, XkbKeyNamesMask, True);
    XkbFreeKeyboard(descriptor, 0, True);

    // Phase 3: Translate un-translated keycodes using traditional X11 KeySym lookups
    for (int keycode = 8; keycode < maxKeyCode; ++keycode)
    {
        if (keycodeToScancode[static_cast<KeyCode>(keycode)] == sf::Keyboard::Scan::Unknown)
        {
            const auto scancode = translateKeyCode(display.get(), static_cast<KeyCode>(keycode));

            if (scancode != sf::Keyboard::Scan::Unknown && scancodeToKeycode[scancode] == nullKeyCode)
                scancodeToKeycode[scancode] = static_cast<KeyCode>(keycode);

            keycodeToScancode[static_cast<KeyCode>(keycode)] = scancode;
        }
    }

    isMappingInitialized = true;
}


////////////////////////////////////////////////////////////
KeyCode scancodeToKeyCode(sf::Keyboard::Scancode code)
{
    ensureMapping();

    if (code != sf::Keyboard::Scan::Unknown)
        return scancodeToKeycode[code];

    return nullKeyCode;
}


////////////////////////////////////////////////////////////
sf::Keyboard::Scancode keyCodeToScancode(KeyCode code)
{
    ensureMapping();

    if (isValidKeycode(code))
        return keycodeToScancode[code];

    return sf::Keyboard::Scan::Unknown;
}


////////////////////////////////////////////////////////////
KeyCode keyToKeyCode(sf::Keyboard::Key key)
{
    const KeySym keysym = sf::priv::keyToKeySym(key);

    if (keysym != NoSymbol)
    {
        const auto    display = sf::priv::openDisplay();
        const KeyCode keycode = XKeysymToKeycode(display.get(), keysym);

        if (keycode != nullKeyCode)
            return keycode;
    }

    // Fallback for when XKeysymToKeycode cannot tell the KeyCode for XK_Alt_R
    if (key == sf::Keyboard::Key::RAlt)
        return scancodeToKeycode[sf::Keyboard::Scan::RAlt];

    return nullKeyCode;
}


////////////////////////////////////////////////////////////
KeySym scancodeToKeySym(sf::Keyboard::Scancode code)
{
    const auto display = sf::priv::openDisplay();

    KeySym        keysym  = NoSymbol;
    const KeyCode keycode = scancodeToKeyCode(code);

    if (keycode != nullKeyCode) // ensure that this Scancode is mapped to keycode
        keysym = XkbKeycodeToKeysym(display.get(), keycode, 0, 0);

    return keysym;
}


////////////////////////////////////////////////////////////
bool isKeyPressedImpl(KeyCode keycode)
{
    if (keycode != nullKeyCode)
    {
        const auto display = sf::priv::openDisplay();

        // Get the whole keyboard state
        std::array<char, 32> keys{};
        XQueryKeymap(display.get(), keys.data());

        // Check our keycode
        return (keys[keycode / 8] & (1 << (keycode % 8))) != 0;
    }

    return false;
}

} // anonymous namespace

namespace sf::priv
{

////////////////////////////////////////////////////////////
bool KeyboardImpl::isKeyPressed(Keyboard::Key key)
{
    const KeyCode keycode = keyToKeyCode(key);
    return isKeyPressedImpl(keycode);
}


////////////////////////////////////////////////////////////
bool KeyboardImpl::isKeyPressed(Keyboard::Scancode code)
{
    const KeyCode keycode = scancodeToKeyCode(code);
    return isKeyPressedImpl(keycode);
}


////////////////////////////////////////////////////////////
Keyboard::Scancode KeyboardImpl::delocalize(Keyboard::Key key)
{
    const KeyCode keycode = keyToKeyCode(key);
    return keyCodeToScancode(keycode);
}


////////////////////////////////////////////////////////////
Keyboard::Key KeyboardImpl::localize(Keyboard::Scancode code)
{
    const KeySym keysym = scancodeToKeySym(code);
    return keySymToKey(keysym);
}


////////////////////////////////////////////////////////////
String KeyboardImpl::getDescription(Keyboard::Scancode code)
{
    bool checkInput = true;

    // these scancodes actually correspond to keys with input
    // but we want to return their description, not their behaviour
    // clang-format off
    if (code == Keyboard::Scan::Enter ||
        code == Keyboard::Scan::Escape ||
        code == Keyboard::Scan::Backspace ||
        code == Keyboard::Scan::Tab ||
        code == Keyboard::Scan::Space ||
        code == Keyboard::Scan::ScrollLock ||
        code == Keyboard::Scan::Pause ||
        code == Keyboard::Scan::Delete ||
        code == Keyboard::Scan::NumpadDivide ||
        code == Keyboard::Scan::NumpadMultiply ||
        code == Keyboard::Scan::NumpadMinus ||
        code == Keyboard::Scan::NumpadPlus ||
        code == Keyboard::Scan::NumpadEqual ||
        code == Keyboard::Scan::NumpadEnter ||
        code == Keyboard::Scan::NumpadDecimal)
    // clang-format on
    {
        checkInput = false;
    }

    if (checkInput)
    {
        const KeySym   keysym  = scancodeToKeySym(code);
        const char32_t unicode = keysymToUnicode(keysym);

        if (unicode != 0)
            return {unicode};
    }

    // Fallback to our best guess for the keys that are known to be independent of the layout.
    // clang-format off
    switch (code)
    {
        case Keyboard::Scan::Enter:              return "Enter";
        case Keyboard::Scan::Escape:             return "Escape";
        case Keyboard::Scan::Backspace:          return "Backspace";
        case Keyboard::Scan::Tab:                return "Tab";
        case Keyboard::Scan::Space:              return "Space";

        case Keyboard::Scan::F1:                 return "F1";
        case Keyboard::Scan::F2:                 return "F2";
        case Keyboard::Scan::F3:                 return "F3";
        case Keyboard::Scan::F4:                 return "F4";
        case Keyboard::Scan::F5:                 return "F5";
        case Keyboard::Scan::F6:                 return "F6";
        case Keyboard::Scan::F7:                 return "F7";
        case Keyboard::Scan::F8:                 return "F8";
        case Keyboard::Scan::F9:                 return "F9";
        case Keyboard::Scan::F10:                return "F10";
        case Keyboard::Scan::F11:                return "F11";
        case Keyboard::Scan::F12:                return "F12";
        case Keyboard::Scan::F13:                return "F13";
        case Keyboard::Scan::F14:                return "F14";
        case Keyboard::Scan::F15:                return "F15";
        case Keyboard::Scan::F16:                return "F16";
        case Keyboard::Scan::F17:                return "F17";
        case Keyboard::Scan::F18:                return "F18";
        case Keyboard::Scan::F19:                return "F19";
        case Keyboard::Scan::F20:                return "F20";
        case Keyboard::Scan::F21:                return "F21";
        case Keyboard::Scan::F22:                return "F22";
        case Keyboard::Scan::F23:                return "F23";
        case Keyboard::Scan::F24:                return "F24";

        case Keyboard::Scan::CapsLock:           return "Caps Lock";
        case Keyboard::Scan::PrintScreen:        return "Print Screen";
        case Keyboard::Scan::ScrollLock:         return "Scroll Lock";

        case Keyboard::Scan::Pause:              return "Pause";
        case Keyboard::Scan::Insert:             return "Insert";
        case Keyboard::Scan::Home:               return "Home";
        case Keyboard::Scan::PageUp:             return "Page Up";
        case Keyboard::Scan::Delete:             return "Delete";
        case Keyboard::Scan::End:                return "End";
        case Keyboard::Scan::PageDown:           return "Page Down";

        case Keyboard::Scan::Left:               return "Left Arrow";
        case Keyboard::Scan::Right:              return "Right Arrow";
        case Keyboard::Scan::Down:               return "Down Arrow";
        case Keyboard::Scan::Up:                 return "Up Arrow";

        case Keyboard::Scan::NumLock:            return "Num Lock";
        case Keyboard::Scan::NumpadDivide:       return "Divide (Numpad)";
        case Keyboard::Scan::NumpadMultiply:     return "Multiply (Numpad)";
        case Keyboard::Scan::NumpadMinus:        return "Minus (Numpad)";
        case Keyboard::Scan::NumpadPlus:         return "Plus (Numpad)";
        case Keyboard::Scan::NumpadEqual:        return "Equal (Numpad)";
        case Keyboard::Scan::NumpadEnter:        return "Enter (Numpad)";
        case Keyboard::Scan::NumpadDecimal:      return "Decimal (Numpad)";

        case Keyboard::Scan::Numpad0:            return "0 (Numpad)";
        case Keyboard::Scan::Numpad1:            return "1 (Numpad)";
        case Keyboard::Scan::Numpad2:            return "2 (Numpad)";
        case Keyboard::Scan::Numpad3:            return "3 (Numpad)";
        case Keyboard::Scan::Numpad4:            return "4 (Numpad)";
        case Keyboard::Scan::Numpad5:            return "5 (Numpad)";
        case Keyboard::Scan::Numpad6:            return "6 (Numpad)";
        case Keyboard::Scan::Numpad7:            return "7 (Numpad)";
        case Keyboard::Scan::Numpad8:            return "8 (Numpad)";
        case Keyboard::Scan::Numpad9:            return "9 (Numpad)";

        case Keyboard::Scan::Application:        return "Application";
        case Keyboard::Scan::Execute:            return "Execute";
        case Keyboard::Scan::Help:               return "Help";
        case Keyboard::Scan::Menu:               return "Menu";
        case Keyboard::Scan::Select:             return "Select";
        case Keyboard::Scan::Stop:               return "Stop";
        case Keyboard::Scan::Redo:               return "Redo";
        case Keyboard::Scan::Undo:               return "Undo";
        case Keyboard::Scan::Cut:                return "Cut";
        case Keyboard::Scan::Copy:               return "Copy";
        case Keyboard::Scan::Paste:              return "Paste";
        case Keyboard::Scan::Search:             return "Search";

        case Keyboard::Scan::VolumeMute:         return "Volume Mute";
        case Keyboard::Scan::VolumeUp:           return "Volume Up";
        case Keyboard::Scan::VolumeDown:         return "Volume Down";

        case Keyboard::Scan::LControl:           return "Left Control";
        case Keyboard::Scan::LShift:             return "Left Shift";
        case Keyboard::Scan::LAlt:               return "Left Alt";
        case Keyboard::Scan::LSystem:            return "Left System";
        case Keyboard::Scan::RControl:           return "Right Control";
        case Keyboard::Scan::RShift:             return "Right Shift";
        case Keyboard::Scan::RAlt:               return "Right Alt";
        case Keyboard::Scan::RSystem:            return "Right System";

        case Keyboard::Scan::LaunchApplication1: return "Launch Application 1";
        case Keyboard::Scan::LaunchApplication2: return "Launch Application 2";
        case Keyboard::Scan::Favorites:          return "Favorites";
        case Keyboard::Scan::Back:               return "Back";
        case Keyboard::Scan::Forward:            return "Forward";
        case Keyboard::Scan::MediaNextTrack:     return "Media Next Track";
        case Keyboard::Scan::MediaPlayPause:     return "Media Play Pause";
        case Keyboard::Scan::MediaPreviousTrack: return "Media Previous Track";
        case Keyboard::Scan::MediaStop:          return "Media Stop";
        case Keyboard::Scan::HomePage:           return "Home Page";
        case Keyboard::Scan::Refresh:            return "Refresh";
        case Keyboard::Scan::LaunchMail:         return "Launch Mail";
        case Keyboard::Scan::LaunchMediaSelect:  return "Launch Media Select";

        default:                                 return "Unknown Scancode";
    }
    // clang-format on
}


////////////////////////////////////////////////////////////
Keyboard::Key KeyboardImpl::getKeyFromEvent(XKeyEvent& event)
{
    // Try each KeySym index (modifier group) until we get a match
    for (int i = 0; i < 4; ++i)
    {
        // Get the SFML keyboard code from the keysym of the key that has been pressed
        const KeySym        keysym = XLookupKeysym(&event, i);
        const Keyboard::Key key    = keySymToKey(keysym);
        if (key != Keyboard::Key::Unknown)
            return key;
    }

    return Keyboard::Key::Unknown;
}


////////////////////////////////////////////////////////////
Keyboard::Scancode KeyboardImpl::getScancodeFromEvent(XKeyEvent& event)
{
    return keyCodeToScancode(static_cast<KeyCode>(event.keycode));
}

} // namespace sf::priv
