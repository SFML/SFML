////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2021 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/Unix/KeyboardImpl.hpp>
#include <SFML/Window/Unix/Display.hpp>
#include <SFML/Window/Unix/KeySymToKeyMapping.hpp>
#include <SFML/Window/Unix/KeySymToUnicodeMapping.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Utf.hpp>
#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <cstring>
#include <map>
#include <string>
#include <utility>

namespace
{

const KeyCode NullKeyCode = 0;
const int MaxKeyCode = 256;
KeyCode scancodeToKeycode[sf::Keyboard::ScancodeCount]; ///< Mapping of SFML scancode to X11 KeyCode
sf::Keyboard::Scancode keycodeToScancode[MaxKeyCode]; ///< Mapping of X11 KeyCode to SFML scancode

////////////////////////////////////////////////////////////
bool isValidKeycode(KeyCode keycode)
{
    // Valid key code range is [8,255], according to the Xlib manual
    return (keycode >= 8);
}


////////////////////////////////////////////////////////////
sf::Keyboard::Scancode translateKeyCode(Display* display, KeyCode keycode)
{
    if (!isValidKeycode(keycode))
        return sf::Keyboard::ScanUnknown;

    // Try secondary keysym, for numeric keypad keys
    // Note: This way we always force "NumLock = ON", which is intentional
    // since the returned key code should correspond to a physical location.
    KeySym keySym = XkbKeycodeToKeysym(display, keycode, 0, 1);

    switch (keySym)
    {
        case XK_KP_0:             return sf::Keyboard::ScanNumpad0;
        case XK_KP_1:             return sf::Keyboard::ScanNumpad1;
        case XK_KP_2:             return sf::Keyboard::ScanNumpad2;
        case XK_KP_3:             return sf::Keyboard::ScanNumpad3;
        case XK_KP_4:             return sf::Keyboard::ScanNumpad4;
        case XK_KP_5:             return sf::Keyboard::ScanNumpad5;
        case XK_KP_6:             return sf::Keyboard::ScanNumpad6;
        case XK_KP_7:             return sf::Keyboard::ScanNumpad7;
        case XK_KP_8:             return sf::Keyboard::ScanNumpad8;
        case XK_KP_9:             return sf::Keyboard::ScanNumpad9;
        case XK_KP_Separator:     return sf::Keyboard::ScanNumpadDecimal;
        case XK_KP_Decimal:       return sf::Keyboard::ScanNumpadDecimal;
        case XK_KP_Equal:         return sf::Keyboard::ScanNumpadEqual;
        case XK_KP_Enter:         return sf::Keyboard::ScanNumpadEnter;
        default:                  break;
    }

    // Now try primary keysym for function keys (non-printable keys)
    // These should not depend on the current keyboard layout
    keySym = XkbKeycodeToKeysym(display, keycode, 0, 0);

    switch (keySym)
    {
        case XK_Return:           return sf::Keyboard::ScanEnter;
        case XK_Escape:           return sf::Keyboard::ScanEscape;
        case XK_BackSpace:        return sf::Keyboard::ScanBackspace;
        case XK_Tab:              return sf::Keyboard::ScanTab;
        case XK_Shift_L:          return sf::Keyboard::ScanLShift;
        case XK_Shift_R:          return sf::Keyboard::ScanRShift;
        case XK_Control_L:        return sf::Keyboard::ScanLControl;
        case XK_Control_R:        return sf::Keyboard::ScanRControl;
        case XK_Alt_L:            return sf::Keyboard::ScanLAlt;
        case XK_ISO_Level3_Shift: // AltGr on at least some machines
        case XK_Alt_R:            return sf::Keyboard::ScanRAlt;
        case XK_Meta_L:
        case XK_Super_L:          return sf::Keyboard::ScanLSystem;
        case XK_Meta_R:
        case XK_Super_R:          return sf::Keyboard::ScanRSystem;
        case XK_Menu:             return sf::Keyboard::ScanMenu;

        case XK_Num_Lock:         return sf::Keyboard::ScanNumLock;
        case XK_Caps_Lock:        return sf::Keyboard::ScanCapsLock;
        case XK_Execute:          return sf::Keyboard::ScanExecute;
        case XK_Hyper_R:          return sf::Keyboard::ScanApplication;
        case XK_Select:           return sf::Keyboard::ScanSelect;
        case XK_Cancel:           return sf::Keyboard::ScanStop;
        case XK_Redo:             return sf::Keyboard::ScanRedo;
        case XK_Undo:             return sf::Keyboard::ScanUndo;
        case XK_Find:             return sf::Keyboard::ScanSearch;
        case XK_Mode_switch:      return sf::Keyboard::ScanModeChange;

        case XK_Print:            return sf::Keyboard::ScanPrintScreen;
        case XK_Scroll_Lock:      return sf::Keyboard::ScanScrollLock;
        case XK_Pause:
        case XK_Break:            return sf::Keyboard::ScanPause;

        case XK_Delete:
        case XK_Clear:            return sf::Keyboard::ScanDelete;
        case XK_Home:             return sf::Keyboard::ScanHome;
        case XK_End:              return sf::Keyboard::ScanEnd;
        case XK_Page_Up:          return sf::Keyboard::ScanPageUp;
        case XK_Page_Down:        return sf::Keyboard::ScanPageDown;
        case XK_Insert:           return sf::Keyboard::ScanInsert;

        case XK_Left:             return sf::Keyboard::ScanLeft;
        case XK_Right:            return sf::Keyboard::ScanRight;
        case XK_Down:             return sf::Keyboard::ScanDown;
        case XK_Up:               return sf::Keyboard::ScanUp;

        case XK_F1:               return sf::Keyboard::ScanF1;
        case XK_F2:               return sf::Keyboard::ScanF2;
        case XK_F3:               return sf::Keyboard::ScanF3;
        case XK_F4:               return sf::Keyboard::ScanF4;
        case XK_F5:               return sf::Keyboard::ScanF5;
        case XK_F6:               return sf::Keyboard::ScanF6;
        case XK_F7:               return sf::Keyboard::ScanF7;
        case XK_F8:               return sf::Keyboard::ScanF8;
        case XK_F9:               return sf::Keyboard::ScanF9;
        case XK_F10:              return sf::Keyboard::ScanF10;
        case XK_F11:              return sf::Keyboard::ScanF11;
        case XK_F12:              return sf::Keyboard::ScanF12;
        case XK_F13:              return sf::Keyboard::ScanF13;
        case XK_F14:              return sf::Keyboard::ScanF14;
        case XK_F15:              return sf::Keyboard::ScanF15;
        case XK_F16:              return sf::Keyboard::ScanF16;
        case XK_F17:              return sf::Keyboard::ScanF17;
        case XK_F18:              return sf::Keyboard::ScanF18;
        case XK_F19:              return sf::Keyboard::ScanF19;
        case XK_F20:              return sf::Keyboard::ScanF20;
        case XK_F21:              return sf::Keyboard::ScanF21;
        case XK_F22:              return sf::Keyboard::ScanF22;
        case XK_F23:              return sf::Keyboard::ScanF23;
        case XK_F24:              return sf::Keyboard::ScanF24;

        // Numeric keypad
        case XK_KP_Divide:        return sf::Keyboard::ScanNumpadDivide;
        case XK_KP_Multiply:      return sf::Keyboard::ScanNumpadMultiply;
        case XK_KP_Subtract:      return sf::Keyboard::ScanNumpadMinus;
        case XK_KP_Add:           return sf::Keyboard::ScanNumpadPlus;

        // These should have been detected in secondary keysym test above!
        case XK_KP_Insert:        return sf::Keyboard::ScanNumpad0;
        case XK_KP_End:           return sf::Keyboard::ScanNumpad1;
        case XK_KP_Down:          return sf::Keyboard::ScanNumpad2;
        case XK_KP_Page_Down:     return sf::Keyboard::ScanNumpad3;
        case XK_KP_Left:          return sf::Keyboard::ScanNumpad4;
        case XK_KP_Right:         return sf::Keyboard::ScanNumpad6;
        case XK_KP_Home:          return sf::Keyboard::ScanNumpad7;
        case XK_KP_Up:            return sf::Keyboard::ScanNumpad8;
        case XK_KP_Page_Up:       return sf::Keyboard::ScanNumpad9;
        case XK_KP_Delete:        return sf::Keyboard::ScanNumpadDecimal;
        case XK_KP_Equal:         return sf::Keyboard::ScanNumpadEqual;
        case XK_KP_Enter:         return sf::Keyboard::ScanNumpadEnter;

        // Last resort: Check for printable keys (should not happen if the XKB
        // extension is available). This will give a layout dependent mapping
        // (which is wrong, and we may miss some keys, especially on non-US
        // keyboards), but it's better than nothing...
        case XK_a:
        case XK_A:                return sf::Keyboard::ScanA;
        case XK_b:
        case XK_B:                return sf::Keyboard::ScanB;
        case XK_c:
        case XK_C:                return sf::Keyboard::ScanC;
        case XK_d:
        case XK_D:                return sf::Keyboard::ScanD;
        case XK_e:
        case XK_E:                return sf::Keyboard::ScanE;
        case XK_f:
        case XK_F:                return sf::Keyboard::ScanF;
        case XK_g:
        case XK_G:                return sf::Keyboard::ScanG;
        case XK_h:
        case XK_H:                return sf::Keyboard::ScanH;
        case XK_i:
        case XK_I:                return sf::Keyboard::ScanI;
        case XK_j:
        case XK_J:                return sf::Keyboard::ScanJ;
        case XK_k:
        case XK_K:                return sf::Keyboard::ScanK;
        case XK_l:
        case XK_L:                return sf::Keyboard::ScanL;
        case XK_m:
        case XK_M:                return sf::Keyboard::ScanM;
        case XK_n:
        case XK_N:                return sf::Keyboard::ScanN;
        case XK_o:
        case XK_O:                return sf::Keyboard::ScanO;
        case XK_p:
        case XK_P:                return sf::Keyboard::ScanP;
        case XK_q:
        case XK_Q:                return sf::Keyboard::ScanQ;
        case XK_r:
        case XK_R:                return sf::Keyboard::ScanR;
        case XK_s:
        case XK_S:                return sf::Keyboard::ScanS;
        case XK_t:
        case XK_T:                return sf::Keyboard::ScanT;
        case XK_u:
        case XK_U:                return sf::Keyboard::ScanU;
        case XK_v:
        case XK_V:                return sf::Keyboard::ScanV;
        case XK_w:
        case XK_W:                return sf::Keyboard::ScanW;
        case XK_x:
        case XK_X:                return sf::Keyboard::ScanX;
        case XK_y:
        case XK_Y:                return sf::Keyboard::ScanY;
        case XK_z:
        case XK_Z:                return sf::Keyboard::ScanZ;

        case XK_1:                return sf::Keyboard::ScanNum1;
        case XK_2:                return sf::Keyboard::ScanNum2;
        case XK_3:                return sf::Keyboard::ScanNum3;
        case XK_4:                return sf::Keyboard::ScanNum4;
        case XK_5:                return sf::Keyboard::ScanNum5;
        case XK_6:                return sf::Keyboard::ScanNum6;
        case XK_7:                return sf::Keyboard::ScanNum7;
        case XK_8:                return sf::Keyboard::ScanNum8;
        case XK_9:                return sf::Keyboard::ScanNum9;
        case XK_0:                return sf::Keyboard::ScanNum0;

        case XK_space:            return sf::Keyboard::ScanSpace;
        case XK_minus:            return sf::Keyboard::ScanHyphen;
        case XK_equal:            return sf::Keyboard::ScanEqual;
        case XK_bracketleft:      return sf::Keyboard::ScanLBracket;
        case XK_bracketright:     return sf::Keyboard::ScanRBracket;
        case XK_backslash:        return sf::Keyboard::ScanBackslash;
        case XK_semicolon:        return sf::Keyboard::ScanSemicolon;
        case XK_apostrophe:       return sf::Keyboard::ScanApostrophe;
        case XK_grave:            return sf::Keyboard::ScanGrave;
        case XK_comma:            return sf::Keyboard::ScanComma;
        case XK_period:           return sf::Keyboard::ScanPeriod;
        case XK_slash:            return sf::Keyboard::ScanSlash;
        case XK_less:             return sf::Keyboard::ScanNonUsBackslash;

        default:                  return sf::Keyboard::ScanUnknown;
    }
}

////////////////////////////////////////////////////////////
std::map<std::string, sf::Keyboard::Scancode> GetNameScancodeMap()
{
    std::map<std::string, sf::Keyboard::Scancode> mapping;

    mapping.insert(std::make_pair("LSGT", sf::Keyboard::ScanNonUsBackslash));

    mapping.insert(std::make_pair("TLDE", sf::Keyboard::ScanGrave));
    mapping.insert(std::make_pair("AE01", sf::Keyboard::ScanNum1));
    mapping.insert(std::make_pair("AE02", sf::Keyboard::ScanNum2));
    mapping.insert(std::make_pair("AE03", sf::Keyboard::ScanNum3));
    mapping.insert(std::make_pair("AE04", sf::Keyboard::ScanNum4));
    mapping.insert(std::make_pair("AE05", sf::Keyboard::ScanNum5));
    mapping.insert(std::make_pair("AE06", sf::Keyboard::ScanNum6));
    mapping.insert(std::make_pair("AE07", sf::Keyboard::ScanNum7));
    mapping.insert(std::make_pair("AE08", sf::Keyboard::ScanNum8));
    mapping.insert(std::make_pair("AE09", sf::Keyboard::ScanNum9));
    mapping.insert(std::make_pair("AE10", sf::Keyboard::ScanNum0));
    mapping.insert(std::make_pair("AE11", sf::Keyboard::ScanHyphen));
    mapping.insert(std::make_pair("AE12", sf::Keyboard::ScanEqual));
    mapping.insert(std::make_pair("BKSP", sf::Keyboard::ScanBackspace));
    mapping.insert(std::make_pair("TAB",  sf::Keyboard::ScanTab));
    mapping.insert(std::make_pair("AD01", sf::Keyboard::ScanQ));
    mapping.insert(std::make_pair("AD02", sf::Keyboard::ScanW));
    mapping.insert(std::make_pair("AD03", sf::Keyboard::ScanE));
    mapping.insert(std::make_pair("AD04", sf::Keyboard::ScanR));
    mapping.insert(std::make_pair("AD05", sf::Keyboard::ScanT));
    mapping.insert(std::make_pair("AD06", sf::Keyboard::ScanY));
    mapping.insert(std::make_pair("AD07", sf::Keyboard::ScanU));
    mapping.insert(std::make_pair("AD08", sf::Keyboard::ScanI));
    mapping.insert(std::make_pair("AD09", sf::Keyboard::ScanO));
    mapping.insert(std::make_pair("AD10", sf::Keyboard::ScanP));
    mapping.insert(std::make_pair("AD11", sf::Keyboard::ScanLBracket));
    mapping.insert(std::make_pair("AD12", sf::Keyboard::ScanRBracket));
    mapping.insert(std::make_pair("BKSL", sf::Keyboard::ScanBackslash));
    mapping.insert(std::make_pair("RTRN", sf::Keyboard::ScanEnter));

    mapping.insert(std::make_pair("CAPS", sf::Keyboard::ScanCapsLock));
    mapping.insert(std::make_pair("AC01", sf::Keyboard::ScanA));
    mapping.insert(std::make_pair("AC02", sf::Keyboard::ScanS));
    mapping.insert(std::make_pair("AC03", sf::Keyboard::ScanD));
    mapping.insert(std::make_pair("AC04", sf::Keyboard::ScanF));
    mapping.insert(std::make_pair("AC05", sf::Keyboard::ScanG));
    mapping.insert(std::make_pair("AC06", sf::Keyboard::ScanH));
    mapping.insert(std::make_pair("AC07", sf::Keyboard::ScanJ));
    mapping.insert(std::make_pair("AC08", sf::Keyboard::ScanK));
    mapping.insert(std::make_pair("AC09", sf::Keyboard::ScanL));
    mapping.insert(std::make_pair("AC10", sf::Keyboard::ScanSemicolon));
    mapping.insert(std::make_pair("AC11", sf::Keyboard::ScanApostrophe));
    mapping.insert(std::make_pair("AC12", sf::Keyboard::ScanBackslash));

    mapping.insert(std::make_pair("LFSH", sf::Keyboard::ScanLShift));
    mapping.insert(std::make_pair("AB01", sf::Keyboard::ScanZ));
    mapping.insert(std::make_pair("AB02", sf::Keyboard::ScanX));
    mapping.insert(std::make_pair("AB03", sf::Keyboard::ScanC));
    mapping.insert(std::make_pair("AB04", sf::Keyboard::ScanV));
    mapping.insert(std::make_pair("AB05", sf::Keyboard::ScanB));
    mapping.insert(std::make_pair("AB06", sf::Keyboard::ScanN));
    mapping.insert(std::make_pair("AB07", sf::Keyboard::ScanM));
    mapping.insert(std::make_pair("AB08", sf::Keyboard::ScanComma));
    mapping.insert(std::make_pair("AB09", sf::Keyboard::ScanPeriod));
    mapping.insert(std::make_pair("AB10", sf::Keyboard::ScanSlash));
    mapping.insert(std::make_pair("RTSH", sf::Keyboard::ScanRShift));

    mapping.insert(std::make_pair("LCTL", sf::Keyboard::ScanLControl));
    mapping.insert(std::make_pair("LALT", sf::Keyboard::ScanLAlt));
    mapping.insert(std::make_pair("SPCE", sf::Keyboard::ScanSpace));
    mapping.insert(std::make_pair("RCTL", sf::Keyboard::ScanRControl));
    mapping.insert(std::make_pair("RALT", sf::Keyboard::ScanRAlt));
    mapping.insert(std::make_pair("LVL3", sf::Keyboard::ScanRAlt));
    mapping.insert(std::make_pair("ALGR", sf::Keyboard::ScanRAlt));
    mapping.insert(std::make_pair("LWIN", sf::Keyboard::ScanLSystem));
    mapping.insert(std::make_pair("RWIN", sf::Keyboard::ScanRSystem));

    mapping.insert(std::make_pair("HYPR", sf::Keyboard::ScanApplication));
    mapping.insert(std::make_pair("EXEC", sf::Keyboard::ScanExecute));
    mapping.insert(std::make_pair("MDSW", sf::Keyboard::ScanModeChange));
    mapping.insert(std::make_pair("MENU", sf::Keyboard::ScanMenu));
    mapping.insert(std::make_pair("COMP", sf::Keyboard::ScanMenu));
    mapping.insert(std::make_pair("SELE", sf::Keyboard::ScanSelect));

    mapping.insert(std::make_pair("ESC",  sf::Keyboard::ScanEscape));
    mapping.insert(std::make_pair("FK01", sf::Keyboard::ScanF1));
    mapping.insert(std::make_pair("FK02", sf::Keyboard::ScanF2));
    mapping.insert(std::make_pair("FK03", sf::Keyboard::ScanF3));
    mapping.insert(std::make_pair("FK04", sf::Keyboard::ScanF4));
    mapping.insert(std::make_pair("FK05", sf::Keyboard::ScanF5));
    mapping.insert(std::make_pair("FK06", sf::Keyboard::ScanF6));
    mapping.insert(std::make_pair("FK07", sf::Keyboard::ScanF7));
    mapping.insert(std::make_pair("FK08", sf::Keyboard::ScanF8));
    mapping.insert(std::make_pair("FK09", sf::Keyboard::ScanF9));
    mapping.insert(std::make_pair("FK10", sf::Keyboard::ScanF10));
    mapping.insert(std::make_pair("FK11", sf::Keyboard::ScanF11));
    mapping.insert(std::make_pair("FK12", sf::Keyboard::ScanF12));

    mapping.insert(std::make_pair("PRSC", sf::Keyboard::ScanPrintScreen));
    mapping.insert(std::make_pair("SCLK", sf::Keyboard::ScanScrollLock));
    mapping.insert(std::make_pair("PAUS", sf::Keyboard::ScanPause));

    mapping.insert(std::make_pair("INS",  sf::Keyboard::ScanInsert));
    mapping.insert(std::make_pair("HOME", sf::Keyboard::ScanHome));
    mapping.insert(std::make_pair("PGUP", sf::Keyboard::ScanPageUp));
    mapping.insert(std::make_pair("DELE", sf::Keyboard::ScanDelete));
    mapping.insert(std::make_pair("END",  sf::Keyboard::ScanEnd));
    mapping.insert(std::make_pair("PGDN", sf::Keyboard::ScanPageDown));

    mapping.insert(std::make_pair("UP",   sf::Keyboard::ScanUp));
    mapping.insert(std::make_pair("RGHT", sf::Keyboard::ScanRight));
    mapping.insert(std::make_pair("DOWN", sf::Keyboard::ScanDown));
    mapping.insert(std::make_pair("LEFT", sf::Keyboard::ScanLeft));

    mapping.insert(std::make_pair("NMLK", sf::Keyboard::ScanNumLock));
    mapping.insert(std::make_pair("KPDV", sf::Keyboard::ScanNumpadDivide));
    mapping.insert(std::make_pair("KPMU", sf::Keyboard::ScanNumpadMultiply));
    mapping.insert(std::make_pair("KPSU", sf::Keyboard::ScanNumpadMinus));

    mapping.insert(std::make_pair("KP7",  sf::Keyboard::ScanNumpad7));
    mapping.insert(std::make_pair("KP8",  sf::Keyboard::ScanNumpad8));
    mapping.insert(std::make_pair("KP9",  sf::Keyboard::ScanNumpad9));
    mapping.insert(std::make_pair("KPAD", sf::Keyboard::ScanNumpadPlus));
    mapping.insert(std::make_pair("KP4",  sf::Keyboard::ScanNumpad4));
    mapping.insert(std::make_pair("KP5",  sf::Keyboard::ScanNumpad5));
    mapping.insert(std::make_pair("KP6",  sf::Keyboard::ScanNumpad6));
    mapping.insert(std::make_pair("KP1",  sf::Keyboard::ScanNumpad1));
    mapping.insert(std::make_pair("KP2",  sf::Keyboard::ScanNumpad2));
    mapping.insert(std::make_pair("KP3",  sf::Keyboard::ScanNumpad3));
    mapping.insert(std::make_pair("KPEN", sf::Keyboard::ScanNumpadEnter));
    mapping.insert(std::make_pair("KP0",  sf::Keyboard::ScanNumpad0));
    mapping.insert(std::make_pair("KPDL", sf::Keyboard::ScanNumpadDecimal));
    mapping.insert(std::make_pair("KPEQ", sf::Keyboard::ScanNumpadEqual));

    mapping.insert(std::make_pair("FK13", sf::Keyboard::ScanF13));
    mapping.insert(std::make_pair("FK14", sf::Keyboard::ScanF14));
    mapping.insert(std::make_pair("FK15", sf::Keyboard::ScanF15));
    mapping.insert(std::make_pair("FK16", sf::Keyboard::ScanF16));
    mapping.insert(std::make_pair("FK17", sf::Keyboard::ScanF17));
    mapping.insert(std::make_pair("FK18", sf::Keyboard::ScanF18));
    mapping.insert(std::make_pair("FK19", sf::Keyboard::ScanF19));
    mapping.insert(std::make_pair("FK20", sf::Keyboard::ScanF20));
    mapping.insert(std::make_pair("FK21", sf::Keyboard::ScanF21));
    mapping.insert(std::make_pair("FK22", sf::Keyboard::ScanF22));
    mapping.insert(std::make_pair("FK23", sf::Keyboard::ScanF23));
    mapping.insert(std::make_pair("FK24", sf::Keyboard::ScanF24));
    mapping.insert(std::make_pair("LMTA", sf::Keyboard::ScanLSystem));
    mapping.insert(std::make_pair("RMTA", sf::Keyboard::ScanRSystem));
    mapping.insert(std::make_pair("MUTE", sf::Keyboard::ScanVolumeMute));
    mapping.insert(std::make_pair("VOL-", sf::Keyboard::ScanVolumeDown));
    mapping.insert(std::make_pair("VOL+", sf::Keyboard::ScanVolumeUp));
    mapping.insert(std::make_pair("STOP", sf::Keyboard::ScanStop));
    mapping.insert(std::make_pair("REDO", sf::Keyboard::ScanRedo));
    mapping.insert(std::make_pair("AGAI", sf::Keyboard::ScanRedo));
    mapping.insert(std::make_pair("UNDO", sf::Keyboard::ScanUndo));
    mapping.insert(std::make_pair("COPY", sf::Keyboard::ScanCopy));
    mapping.insert(std::make_pair("PAST", sf::Keyboard::ScanPaste));
    mapping.insert(std::make_pair("FIND", sf::Keyboard::ScanSearch));
    mapping.insert(std::make_pair("CUT",  sf::Keyboard::ScanCut));
    mapping.insert(std::make_pair("HELP", sf::Keyboard::ScanHelp));

    mapping.insert(std::make_pair("I156", sf::Keyboard::ScanLaunchApplication1));
    mapping.insert(std::make_pair("I157", sf::Keyboard::ScanLaunchApplication2));
    mapping.insert(std::make_pair("I164", sf::Keyboard::ScanFavorites));
    mapping.insert(std::make_pair("I166", sf::Keyboard::ScanBack));
    mapping.insert(std::make_pair("I167", sf::Keyboard::ScanForward));
    mapping.insert(std::make_pair("I171", sf::Keyboard::ScanMediaNextTrack));
    mapping.insert(std::make_pair("I172", sf::Keyboard::ScanMediaPlayPause));
    mapping.insert(std::make_pair("I173", sf::Keyboard::ScanMediaPreviousTrack));
    mapping.insert(std::make_pair("I174", sf::Keyboard::ScanMediaStop));
    mapping.insert(std::make_pair("I180", sf::Keyboard::ScanHomePage));
    mapping.insert(std::make_pair("I181", sf::Keyboard::ScanRefresh));
    mapping.insert(std::make_pair("I223", sf::Keyboard::ScanLaunchMail));
    mapping.insert(std::make_pair("I234", sf::Keyboard::ScanLaunchMediaSelect));

    return mapping;
}

////////////////////////////////////////////////////////////
void ensureMapping()
{
    static bool isMappingInitialized = false;

    if (isMappingInitialized)
        return;

    // Phase 1: Initialize mappings with default values
    for (int i = 0; i < sf::Keyboard::ScancodeCount; ++i)
        scancodeToKeycode[i] = NullKeyCode;

    for (int i = 0; i < MaxKeyCode; ++i)
        keycodeToScancode[i] = sf::Keyboard::ScanUnknown;

    // Phase 2: Get XKB names with key code
    Display* display = sf::priv::OpenDisplay();

    char name[XkbKeyNameLength + 1];
    XkbDescPtr descriptor = XkbGetMap(display, 0, XkbUseCoreKbd);
    XkbGetNames(display, XkbKeyNamesMask, descriptor);

    std::map<std::string, sf::Keyboard::Scancode> nameScancodeMap = GetNameScancodeMap();
    sf::Keyboard::Scancode scancode = sf::Keyboard::ScanUnknown;

    for (int keycode = descriptor->min_key_code; keycode <= descriptor->max_key_code; ++keycode)
    {
        if (!isValidKeycode(static_cast<KeyCode>(keycode)))
        {
            continue;
        }

        std::memcpy(name, descriptor->names->keys[keycode].name, XkbKeyNameLength);
        name[XkbKeyNameLength] = '\0';

        std::map<std::string, sf::Keyboard::Scancode>::iterator mappedScancode = nameScancodeMap.find(std::string(name));
        scancode = sf::Keyboard::ScanUnknown;

        if (mappedScancode != nameScancodeMap.end())
            scancode = mappedScancode->second;

        if (scancode != sf::Keyboard::ScanUnknown)
            scancodeToKeycode[scancode] = static_cast<KeyCode>(keycode);

        keycodeToScancode[keycode] = scancode;
    }

    XkbFreeNames(descriptor, XkbKeyNamesMask, True);
    XkbFreeKeyboard(descriptor, 0, True);

    // Phase 3: Translate un-translated keycodes using traditional X11 KeySym lookups
    for (int keycode = 8; keycode < MaxKeyCode; ++keycode)
    {
        if (keycodeToScancode[static_cast<KeyCode>(keycode)] == sf::Keyboard::ScanUnknown)
        {
            scancode = translateKeyCode(display, static_cast<KeyCode>(keycode));
            scancodeToKeycode[scancode] = static_cast<KeyCode>(keycode);
            keycodeToScancode[keycode] = scancode;
        }
    }

    sf::priv::CloseDisplay(display);

    isMappingInitialized = true;
}


////////////////////////////////////////////////////////////
KeyCode scancodeToKeyCode(sf::Keyboard::Scancode code)
{
    ensureMapping();

    if (code != sf::Keyboard::ScanUnknown)
        return scancodeToKeycode[code];

    return NullKeyCode;
}


////////////////////////////////////////////////////////////
sf::Keyboard::Scancode keyCodeToScancode(KeyCode code)
{
    ensureMapping();

    if (isValidKeycode(code))
        return keycodeToScancode[code];

    return sf::Keyboard::ScanUnknown;
}


////////////////////////////////////////////////////////////
KeyCode keyToKeyCode(sf::Keyboard::Key key)
{
    KeySym keysym = sf::priv::keyToKeySym(key);

    if (keysym != NoSymbol)
    {
        Display* display = sf::priv::OpenDisplay();
        KeyCode keycode = XKeysymToKeycode(display, keysym);
        sf::priv::CloseDisplay(display);

        if (keycode != NullKeyCode)
            return keycode;
    }

    // Fallback for when XKeysymToKeycode cannot tell the KeyCode for XK_Alt_R
    if (key == sf::Keyboard::RAlt)
        return scancodeToKeycode[sf::Keyboard::ScanRAlt];

    return NullKeyCode;
}


////////////////////////////////////////////////////////////
KeySym scancodeToKeySym(sf::Keyboard::Scancode code)
{
    Display* display = sf::priv::OpenDisplay();

    KeySym keysym = NoSymbol;
    KeyCode keycode = scancodeToKeyCode(code);

    if (keycode != NullKeyCode) // ensure that this Scancode is mapped to keycode
        keysym = XkbKeycodeToKeysym(display, keycode, 0, 0);

    sf::priv::CloseDisplay(display);

    return keysym;
}


////////////////////////////////////////////////////////////
bool isKeyPressedImpl(KeyCode keycode)
{
    if (keycode != NullKeyCode)
    {
        Display* display = sf::priv::OpenDisplay();

        // Get the whole keyboard state
        char keys[32];
        XQueryKeymap(display, keys);

        sf::priv::CloseDisplay(display);

        // Check our keycode
        return (keys[keycode / 8] & (1 << (keycode % 8))) != 0;
    }

    return false;
}

} // anonymous namespace

namespace sf
{
namespace priv
{

////////////////////////////////////////////////////////////
bool KeyboardImpl::isKeyPressed(Keyboard::Key key)
{
    KeyCode keycode = keyToKeyCode(key);
    return isKeyPressedImpl(keycode);
}


////////////////////////////////////////////////////////////
bool KeyboardImpl::isKeyPressed(Keyboard::Scancode code)
{
    KeyCode keycode = scancodeToKeyCode(code);
    return isKeyPressedImpl(keycode);
}


////////////////////////////////////////////////////////////
Keyboard::Scancode KeyboardImpl::delocalize(Keyboard::Key key)
{
    KeyCode keycode = keyToKeyCode(key);
    return keyCodeToScancode(keycode);
}


////////////////////////////////////////////////////////////
Keyboard::Key KeyboardImpl::localize(Keyboard::Scancode code)
{
    KeySym keysym = scancodeToKeySym(code);
    return keySymToKey(keysym);
}


////////////////////////////////////////////////////////////
String KeyboardImpl::getDescription(Keyboard::Scancode code)
{
    bool checkInput = true;

    // these scancodes actually correspond to keys with input
    // but we want to return their description, not their behaviour
    if (code == Keyboard::ScanEnter          ||
        code == Keyboard::ScanEscape         ||
        code == Keyboard::ScanBackspace      ||
        code == Keyboard::ScanTab            ||
        code == Keyboard::ScanSpace          ||
        code == Keyboard::ScanScrollLock     ||
        code == Keyboard::ScanPause          ||
        code == Keyboard::ScanDelete         ||
        code == Keyboard::ScanNumpadDivide   ||
        code == Keyboard::ScanNumpadMultiply ||
        code == Keyboard::ScanNumpadMinus    ||
        code == Keyboard::ScanNumpadPlus     ||
        code == Keyboard::ScanNumpadEqual    ||
        code == Keyboard::ScanNumpadEnter    ||
        code == Keyboard::ScanNumpadDecimal)
    {
        checkInput = false;
    }

    if (checkInput)
    {
        KeySym keysym = scancodeToKeySym(code);
        Uint32 unicode = keysymToUnicode(keysym);

        if (unicode != 0)
            return String(unicode);
    }

    // Fallback to our best guess for the keys that are known to be independent of the layout.
    switch (code)
    {
        case Keyboard::ScanEnter:              return "Enter";
        case Keyboard::ScanEscape:             return "Escape";
        case Keyboard::ScanBackspace:          return "Backspace";
        case Keyboard::ScanTab:                return "Tab";
        case Keyboard::ScanSpace:              return "Space";

        case Keyboard::ScanF1:                 return "F1";
        case Keyboard::ScanF2:                 return "F2";
        case Keyboard::ScanF3:                 return "F3";
        case Keyboard::ScanF4:                 return "F4";
        case Keyboard::ScanF5:                 return "F5";
        case Keyboard::ScanF6:                 return "F6";
        case Keyboard::ScanF7:                 return "F7";
        case Keyboard::ScanF8:                 return "F8";
        case Keyboard::ScanF9:                 return "F9";
        case Keyboard::ScanF10:                return "F10";
        case Keyboard::ScanF11:                return "F11";
        case Keyboard::ScanF12:                return "F12";
        case Keyboard::ScanF13:                return "F13";
        case Keyboard::ScanF14:                return "F14";
        case Keyboard::ScanF15:                return "F15";
        case Keyboard::ScanF16:                return "F16";
        case Keyboard::ScanF17:                return "F17";
        case Keyboard::ScanF18:                return "F18";
        case Keyboard::ScanF19:                return "F19";
        case Keyboard::ScanF20:                return "F20";
        case Keyboard::ScanF21:                return "F21";
        case Keyboard::ScanF22:                return "F22";
        case Keyboard::ScanF23:                return "F23";
        case Keyboard::ScanF24:                return "F24";

        case Keyboard::ScanCapsLock:           return "Caps Lock";
        case Keyboard::ScanPrintScreen:        return "Print Screen";
        case Keyboard::ScanScrollLock:         return "Scroll Lock";

        case Keyboard::ScanPause:              return "Pause";
        case Keyboard::ScanInsert:             return "Insert";
        case Keyboard::ScanHome:               return "Home";
        case Keyboard::ScanPageUp:             return "Page Up";
        case Keyboard::ScanDelete:             return "Delete";
        case Keyboard::ScanEnd:                return "End";
        case Keyboard::ScanPageDown:           return "Page Down";

        case Keyboard::ScanLeft:               return "Left Arrow";
        case Keyboard::ScanRight:              return "Right Arrow";
        case Keyboard::ScanDown:               return "Down Arrow";
        case Keyboard::ScanUp:                 return "Up Arrow";

        case Keyboard::ScanNumLock:            return "Num Lock";
        case Keyboard::ScanNumpadDivide:       return "Divide (Numpad)";
        case Keyboard::ScanNumpadMultiply:     return "Multiply (Numpad)";
        case Keyboard::ScanNumpadMinus:        return "Minus (Numpad)";
        case Keyboard::ScanNumpadPlus:         return "Plus (Numpad)";
        case Keyboard::ScanNumpadEqual:        return "Equal (Numpad)";
        case Keyboard::ScanNumpadEnter:        return "Enter (Numpad)";
        case Keyboard::ScanNumpadDecimal:      return "Decimal (Numpad)";

        case Keyboard::ScanNumpad0:            return "0 (Numpad)";
        case Keyboard::ScanNumpad1:            return "1 (Numpad)";
        case Keyboard::ScanNumpad2:            return "2 (Numpad)";
        case Keyboard::ScanNumpad3:            return "3 (Numpad)";
        case Keyboard::ScanNumpad4:            return "4 (Numpad)";
        case Keyboard::ScanNumpad5:            return "5 (Numpad)";
        case Keyboard::ScanNumpad6:            return "6 (Numpad)";
        case Keyboard::ScanNumpad7:            return "7 (Numpad)";
        case Keyboard::ScanNumpad8:            return "8 (Numpad)";
        case Keyboard::ScanNumpad9:            return "9 (Numpad)";

        case Keyboard::ScanApplication:        return "Application";
        case Keyboard::ScanExecute:            return "Execute";
        case Keyboard::ScanHelp:               return "Help";
        case Keyboard::ScanMenu:               return "Menu";
        case Keyboard::ScanSelect:             return "Select";
        case Keyboard::ScanStop:               return "Stop";
        case Keyboard::ScanRedo:               return "Redo";
        case Keyboard::ScanUndo:               return "Undo";
        case Keyboard::ScanCut:                return "Cut";
        case Keyboard::ScanCopy:               return "Copy";
        case Keyboard::ScanPaste:              return "Paste";
        case Keyboard::ScanSearch:             return "Search";

        case Keyboard::ScanVolumeMute:         return "Volume Mute";
        case Keyboard::ScanVolumeUp:           return "Volume Up";
        case Keyboard::ScanVolumeDown:         return "Volume Down";

        case Keyboard::ScanLControl:           return "Left Control";
        case Keyboard::ScanLShift:             return "Left Shift";
        case Keyboard::ScanLAlt:               return "Left Alt";
        case Keyboard::ScanLSystem:            return "Left System";
        case Keyboard::ScanRControl:           return "Right Control";
        case Keyboard::ScanRShift:             return "Right Shift";
        case Keyboard::ScanRAlt:               return "Right Alt";
        case Keyboard::ScanRSystem:            return "Right System";

        case Keyboard::ScanLaunchApplication1: return "Launch Application 1";
        case Keyboard::ScanLaunchApplication2: return "Launch Application 2";
        case Keyboard::ScanFavorites:          return "Favorites";
        case Keyboard::ScanBack:               return "Back";
        case Keyboard::ScanForward:            return "Forward";
        case Keyboard::ScanMediaNextTrack:     return "Media Next Track";
        case Keyboard::ScanMediaPlayPause:     return "Media Play Pause";
        case Keyboard::ScanMediaPreviousTrack: return "Media Previous Track";
        case Keyboard::ScanMediaStop:          return "Media Stop";
        case Keyboard::ScanHomePage:           return "Home Page";
        case Keyboard::ScanRefresh:            return "Refresh";
        case Keyboard::ScanLaunchMail:         return "Launch Mail";
        case Keyboard::ScanLaunchMediaSelect:  return "Launch Media Select";

        default:                               return "Unknown Scancode";
    }
}


////////////////////////////////////////////////////////////
Keyboard::Key KeyboardImpl::getKeyFromEvent(XKeyEvent& event)
{
    Keyboard::Key key = Keyboard::Unknown;

    // Try each KeySym index (modifier group) until we get a match
    for (int i = 0; i < 4; ++i)
    {
        // Get the SFML keyboard code from the keysym of the key that has been pressed
        KeySym keysym = XLookupKeysym(&event, i);
        key = keySymToKey(keysym);

        if (key != Keyboard::Unknown)
            break;
    }

    return key;
}


////////////////////////////////////////////////////////////
Keyboard::Scancode KeyboardImpl::getScancodeFromEvent(XKeyEvent& event)
{
    return keyCodeToScancode(static_cast<KeyCode>(event.keycode));
}

} // namespace priv

} // namespace sf
