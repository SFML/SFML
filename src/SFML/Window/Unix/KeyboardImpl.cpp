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
#include <SFML/Window/Unix/KeyboardImpl.hpp>
#include <SFML/System/Utf.hpp>
#include <SFML/Window/Unix/Display.hpp>
#include <SFML/Window/Unix/KeySymToSFKeyMapping.hpp>
#include <SFML/Window/Unix/KeySymToUnicodeMapping.hpp>
#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstring>

namespace {

const KeyCode NullKeyCode = 0;
KeyCode scancodeToKeycode[sf::Keyboard::ScanCodeCount] = { NullKeyCode };     ///< Mapping of SFML scancode to X11 KeyCode
sf::Keyboard::Scancode keycodeToScancode[256] = { sf::Keyboard::ScanUnknown}; ///< Mapping of X11 KeyCode to SFML scancode
bool isMappingInitialized = false;

////////////////////////////////////////////////////////////
bool isValidKeycode(KeyCode keycode)
{
    // Valid key code range is [8,255], according to the Xlib manual
    return keycode >= 8 || keycode <= 255;
}

////////////////////////////////////////////////////////////
sf::Keyboard::Scancode translateKeyCode(Display* display, KeyCode keycode)
{
    if (!isValidKeycode(keycode))
        return sf::Keyboard::ScanUnknown;

    // Try secondary keysym, for numeric keypad keys
    // Note: This way we always force "NumLock = ON", which is intentional
    // since the returned key code should correspond to a physical
    // location.
    KeySym keySym = XkbKeycodeToKeysym(display, keycode, 0, 1);
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

void initMapping()
{
    Display* display = sf::priv::OpenDisplay();

    // Find the X11 key code -> SFML key code mapping
    // This code was inspired by GLFW implementation

    char name[XkbKeyNameLength + 1];
    XkbDescPtr desc = XkbGetMap(display, 0, XkbUseCoreKbd);
    XkbGetNames(display, XkbKeyNamesMask, desc);

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

        if (isValidKeycode(keycode))
        {
            scancodeToKeycode[sc] = keycode;
            keycodeToScancode[keycode] = sc;
        }
    }

    XkbFreeNames(desc, XkbKeyNamesMask, True);
    XkbFreeKeyboard(desc, 0, True);

    // Translate un-translated keycodes using traditional X11 KeySym lookups
    // Valid keycodes are [8;255], so we only initialize them
    for (int keycode = 8; keycode < 256; ++keycode)
    {
        if (keycodeToScancode[keycode] == sf::Keyboard::ScanUnknown)
        {
            sf::Keyboard::Scancode sc = translateKeyCode(display, keycode);
            scancodeToKeycode[sc] = keycode;
            keycodeToScancode[keycode] = sc;
        }
    }

    sf::priv::CloseDisplay(display);

    isMappingInitialized = true;
}

////////////////////////////////////////////////////////////
KeyCode SFScancodeToKeyCode(sf::Keyboard::Scancode code)
{
    if (!isMappingInitialized)
        initMapping();

    return scancodeToKeycode[code];
}

////////////////////////////////////////////////////////////
sf::Keyboard::Scancode keyCodeToSFScancode(KeyCode code)
{
    if (!isMappingInitialized)
        initMapping();

    if (isValidKeycode(code))
        return keycodeToScancode[code];
    return sf::Keyboard::ScanUnknown;
}

////////////////////////////////////////////////////////////
KeyCode SFKeyToKeyCode(sf::Keyboard::Key key)
{
    KeySym keysym = sf::priv::SFKeyToKeySym(key);
    if (keysym != NoSymbol)
    {
        Display* display = sf::priv::OpenDisplay();
        KeyCode keycode = XKeysymToKeycode(display, keysym);
        sf::priv::CloseDisplay(display);
        return keycode;
    }
    return NullKeyCode;
}

////////////////////////////////////////////////////////////
KeySym SFScancodeToKeySym(sf::Keyboard::Scancode code)
{
    Display* display = sf::priv::OpenDisplay();
    KeySym keysym = NoSymbol;
    KeyCode keycode = SFScancodeToKeyCode(code);
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
    KeyCode keycode = SFKeyToKeyCode(key);
    return isKeyPressedImpl(keycode);
}


////////////////////////////////////////////////////////////
bool KeyboardImpl::isKeyPressed(Keyboard::Scancode code)
{
    KeyCode keycode = SFScancodeToKeyCode(code);
    return isKeyPressedImpl(keycode);
}


////////////////////////////////////////////////////////////
Keyboard::Scancode KeyboardImpl::unlocalize(Keyboard::Key key)
{
    KeyCode keycode = SFKeyToKeyCode(key);
    return keyCodeToSFScancode(keycode);
}


////////////////////////////////////////////////////////////
Keyboard::Key KeyboardImpl::localize(Keyboard::Scancode code)
{
    KeySym keysym = SFScancodeToKeySym(code);
    return keySymToSFKey(keysym);
}

////////////////////////////////////////////////////////////
String KeyboardImpl::getDescription(Keyboard::Scancode code)
{
    bool checkInput = true;

    // these scancodes actually correspond to keys with input
    // but we want to return their description, not their behaviour
    if (code == Keyboard::ScanEnter     ||
        code == Keyboard::ScanReturn    ||
        code == Keyboard::ScanTab       ||
        code == Keyboard::ScanDelete    ||
        code == Keyboard::ScanBackspace ||
        code == Keyboard::ScanSpace)
    {
        checkInput = false;
    }

    if (checkInput)
    {
        KeySym keysym = SFScancodeToKeySym(code);
        Uint32 unicode = keysymToUnicode(keysym);

        if (unicode != 0)
            return String(unicode);
    }

    // Fallback to our best guess for the keys that are known to be independent of the layout.
    switch (code)
    {
        case Keyboard::ScanEnter:       return "Enter";
        case Keyboard::ScanEscape:      return "Escape";
        case Keyboard::ScanBackspace:   return "Backspace";
        case Keyboard::ScanTab:         return "Tab";
        case Keyboard::ScanSpace:       return "Space";

        case Keyboard::ScanF1:          return "F1";
        case Keyboard::ScanF2:          return "F2";
        case Keyboard::ScanF3:          return "F3";
        case Keyboard::ScanF4:          return "F4";
        case Keyboard::ScanF5:          return "F5";
        case Keyboard::ScanF6:          return "F6";
        case Keyboard::ScanF7:          return "F7";
        case Keyboard::ScanF8:          return "F8";
        case Keyboard::ScanF9:          return "F9";
        case Keyboard::ScanF10:         return "F10";
        case Keyboard::ScanF11:         return "F11";
        case Keyboard::ScanF12:         return "F12";
        case Keyboard::ScanF13:         return "F13";
        case Keyboard::ScanF14:         return "F14";
        case Keyboard::ScanF15:         return "F15";

        case Keyboard::ScanCapsLock:    return "CapsLock";
        case Keyboard::ScanPrintScreen: return "PrintScreen";
        case Keyboard::ScanScrollLock:  return "ScrollLock";

        case Keyboard::ScanPause:       return "Pause";
        case Keyboard::ScanInsert:      return "Insert";
        case Keyboard::ScanHome:        return "Home";
        case Keyboard::ScanPageUp:      return "PageUp";
        case Keyboard::ScanDelete:      return "Delete";
        case Keyboard::ScanEnd:         return "End";
        case Keyboard::ScanPageDown:    return "PageDown";

        case Keyboard::ScanLeft:        return "Left Arrow";
        case Keyboard::ScanRight:       return "Right Arrow";
        case Keyboard::ScanDown:        return "Down Arrow";
        case Keyboard::ScanUp:          return "Up Arrow";

        case Keyboard::ScanNumLock:     return "NumLock";
        case Keyboard::ScanDivide:      return "Divide (Numpad)";
        case Keyboard::ScanMultiply:    return "Multiply (Numpad)";
        case Keyboard::ScanMinus:       return "Minux (Numpad)";
        case Keyboard::ScanPlus:        return "Plus (Numpad)";
        case Keyboard::ScanPadEquals:   return "Equals (Numpad)";
        case Keyboard::ScanReturn:      return "Return (Numpad)";
        case Keyboard::ScanDecimal:     return "Decimal (Numpad)";

        case Keyboard::ScanNumpad0:     return "0 (Numpad)";
        case Keyboard::ScanNumpad1:     return "1 (Numpad)";
        case Keyboard::ScanNumpad2:     return "2 (Numpad)";
        case Keyboard::ScanNumpad3:     return "3 (Numpad)";
        case Keyboard::ScanNumpad4:     return "4 (Numpad)";
        case Keyboard::ScanNumpad5:     return "5 (Numpad)";
        case Keyboard::ScanNumpad6:     return "6 (Numpad)";
        case Keyboard::ScanNumpad7:     return "7 (Numpad)";
        case Keyboard::ScanNumpad8:     return "8 (Numpad)";
        case Keyboard::ScanNumpad9:     return "9 (Numpad)";

        case Keyboard::ScanApplication: return "Application";
        case Keyboard::ScanExecute:     return "Execute";
        case Keyboard::ScanHelp:        return "Help";
        case Keyboard::ScanMenu:        return "Menu";
        case Keyboard::ScanSelect:      return "Select";
        case Keyboard::ScanStop:        return "Stop";
        case Keyboard::ScanAgain:       return "Again";
        case Keyboard::ScanUndo:        return "Undo";
        case Keyboard::ScanCut:         return "Cut";
        case Keyboard::ScanCopy:        return "Copy";
        case Keyboard::ScanPaste:       return "Paste";
        case Keyboard::ScanFind:        return "Find";
        case Keyboard::ScanMute:        return "Mute";
        case Keyboard::ScanVolumeUp:    return "Volume Up";
        case Keyboard::ScanVolumeDown:  return "Volume Down";

        case Keyboard::ScanLControl:    return "Control (Left)";
        case Keyboard::ScanLShift:      return "Shift (Left)";
        case Keyboard::ScanLAlt:        return "Alt (Left)";
        case Keyboard::ScanLSystem:     return "Meta (Left)";
        case Keyboard::ScanRControl:    return "Control (Right)";
        case Keyboard::ScanRShift:      return "Shift (Right)";
        case Keyboard::ScanRAlt:        return "Alt (Right)";
        case Keyboard::ScanRSystem:     return "Meta (Right)";
    }

    return "Unknown Scancode"; // no guess good enough possible.
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
        key = keySymToSFKey(keysym);

        if (key != Keyboard::Unknown)
            break;
    }
    return key;
}


////////////////////////////////////////////////////////////
Keyboard::Scancode KeyboardImpl::getScancodeFromEvent(XKeyEvent& event)
{
    return keyCodeToSFScancode(event.keycode);
}

} // namespace priv
} // namespace sf
