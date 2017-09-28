////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
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
#ifdef _WIN32_WINDOWS
    #undef _WIN32_WINDOWS
#endif
#ifdef _WIN32_WINNT
    #undef _WIN32_WINNT
#endif
#define _WIN32_WINDOWS 0x0501
#define _WIN32_WINNT   0x0501
#include <SFML/Window/Window.hpp>
#include <SFML/Window/Win32/InputImpl.hpp>
#include <windows.h>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
Keyboard::Scancode InputImpl::m_keyToScancodeMapping[Keyboard::KeyCount];      ///< Mapping from Key to Scancode
Keyboard::Key      InputImpl::m_scancodeToKeyMapping[Keyboard::ScanCodeCount]; ///< Mapping from Scancode to Key

////////////////////////////////////////////////////////////
Keyboard::Key winKeyToSfKey(int vkey)
{
    Keyboard::Key key;
    switch (vkey)
    {
        default:                key = Keyboard::Unknown; break;
        case 'A':               key = Keyboard::A; break;
        case 'B':               key = Keyboard::B; break;
        case 'C':               key = Keyboard::C; break;
        case 'D':               key = Keyboard::D; break;
        case 'E':               key = Keyboard::E; break;
        case 'F':               key = Keyboard::F; break;
        case 'G':               key = Keyboard::G; break;
        case 'H':               key = Keyboard::H; break;
        case 'I':               key = Keyboard::I; break;
        case 'J':               key = Keyboard::J; break;
        case 'K':               key = Keyboard::K; break;
        case 'L':               key = Keyboard::L; break;
        case 'M':               key = Keyboard::M; break;
        case 'N':               key = Keyboard::N; break;
        case 'O':               key = Keyboard::O; break;
        case 'P':               key = Keyboard::P; break;
        case 'Q':               key = Keyboard::Q; break;
        case 'R':               key = Keyboard::R; break;
        case 'S':               key = Keyboard::S; break;
        case 'T':               key = Keyboard::T; break;
        case 'U':               key = Keyboard::U; break;
        case 'V':               key = Keyboard::V; break;
        case 'W':               key = Keyboard::W; break;
        case 'X':               key = Keyboard::X; break;
        case 'Y':               key = Keyboard::Y; break;
        case 'Z':               key = Keyboard::Z; break;
        case '0':               key = Keyboard::Num0; break;
        case '1':               key = Keyboard::Num1; break;
        case '2':               key = Keyboard::Num2; break;
        case '3':               key = Keyboard::Num3; break;
        case '4':               key = Keyboard::Num4; break;
        case '5':               key = Keyboard::Num5; break;
        case '6':               key = Keyboard::Num6; break;
        case '7':               key = Keyboard::Num7; break;
        case '8':               key = Keyboard::Num8; break;
        case '9':               key = Keyboard::Num9; break;
        case VK_ESCAPE:         key = Keyboard::Escape; break;
        case VK_LCONTROL:       key = Keyboard::LControl; break;
        case VK_LSHIFT:         key = Keyboard::LShift; break;
        case VK_LMENU:          key = Keyboard::LAlt; break;
        case VK_LWIN:           key = Keyboard::LSystem; break;
        case VK_RCONTROL:       key = Keyboard::RControl; break;
        case VK_RSHIFT:         key = Keyboard::RShift; break;
        case VK_RMENU:          key = Keyboard::RAlt; break;
        case VK_RWIN:           key = Keyboard::RSystem; break;
        case VK_APPS:           key = Keyboard::Menu; break;
        case VK_OEM_4:          key = Keyboard::LBracket; break;
        case VK_OEM_6:          key = Keyboard::RBracket; break;
        case VK_OEM_1:          key = Keyboard::Semicolon; break;
        case VK_OEM_COMMA:      key = Keyboard::Comma; break;
        case VK_OEM_PERIOD:     key = Keyboard::Period; ;
        case VK_OEM_7:          key = Keyboard::Quote; break;
        case VK_OEM_2:          key = Keyboard::Slash; break;
        case VK_OEM_5:          key = Keyboard::Backslash; break;
        case VK_OEM_3:          key = Keyboard::Tilde; break;
        case VK_OEM_PLUS:       key = Keyboard::Equal; break;
        case VK_OEM_MINUS:      key = Keyboard::Hyphen; break;
        case VK_SPACE:          key = Keyboard::Space; break;
        case VK_RETURN:         key = Keyboard::Enter; break;
        case VK_BACK:           key = Keyboard::Backspace; break;
        case VK_TAB:            key = Keyboard::Tab; break;
        case VK_PRIOR:          key = Keyboard::PageUp; break;
        case VK_NEXT:           key = Keyboard::PageDown; break;
        case VK_END:            key = Keyboard::End; break;
        case VK_HOME:           key = Keyboard::Home; break;
        case VK_INSERT:         key = Keyboard::Insert; break;
        case VK_DELETE:         key = Keyboard::Delete; break;
        case VK_ADD:            key = Keyboard::Add; break;
        case VK_SUBTRACT:       key = Keyboard::Subtract; break;
        case VK_MULTIPLY:       key = Keyboard::Multiply; break;
        case VK_DIVIDE:         key = Keyboard::Divide; break;
        case VK_LEFT:           key = Keyboard::Left; break;
        case VK_RIGHT:          key = Keyboard::Right; break;
        case VK_UP:             key = Keyboard::Up; break;
        case VK_DOWN:           key = Keyboard::Down; break;
        case VK_NUMPAD0:        key = Keyboard::Numpad0; break;
        case VK_NUMPAD1:        key = Keyboard::Numpad1; break;
        case VK_NUMPAD2:        key = Keyboard::Numpad2; break;
        case VK_NUMPAD3:        key = Keyboard::Numpad3; break;
        case VK_NUMPAD4:        key = Keyboard::Numpad4; break;
        case VK_NUMPAD5:        key = Keyboard::Numpad5; break;
        case VK_NUMPAD6:        key = Keyboard::Numpad6; break;
        case VK_NUMPAD7:        key = Keyboard::Numpad7; break;
        case VK_NUMPAD8:        key = Keyboard::Numpad8; break;
        case VK_NUMPAD9:        key = Keyboard::Numpad9; break;
        case VK_F1:             key = Keyboard::F1; break;
        case VK_F2:             key = Keyboard::F2; break;
        case VK_F3:             key = Keyboard::F3; break;
        case VK_F4:             key = Keyboard::F4; break;
        case VK_F5:             key = Keyboard::F5; break;
        case VK_F6:             key = Keyboard::F6; break;
        case VK_F7:             key = Keyboard::F7; break;
        case VK_F8:             key = Keyboard::F8; break;
        case VK_F9:             key = Keyboard::F9; break;
        case VK_F10:            key = Keyboard::F10; break;
        case VK_F11:            key = Keyboard::F11; break;
        case VK_F12:            key = Keyboard::F12; break;
        case VK_F13:            key = Keyboard::F13; break;
        case VK_F14:            key = Keyboard::F14; break;
        case VK_F15:            key = Keyboard::F15; break;
        case VK_PAUSE:          key = Keyboard::Pause; break;
    }
    return key;
}
////////////////////////////////////////////////////////////
int sfKeyToWin(Keyboard::Key key)
{
    int vkey = 0;
    switch (key)
    {
        default:                   vkey = 0;             break;
        case Keyboard::A:          vkey = 'A';           break;
        case Keyboard::B:          vkey = 'B';           break;
        case Keyboard::C:          vkey = 'C';           break;
        case Keyboard::D:          vkey = 'D';           break;
        case Keyboard::E:          vkey = 'E';           break;
        case Keyboard::F:          vkey = 'F';           break;
        case Keyboard::G:          vkey = 'G';           break;
        case Keyboard::H:          vkey = 'H';           break;
        case Keyboard::I:          vkey = 'I';           break;
        case Keyboard::J:          vkey = 'J';           break;
        case Keyboard::K:          vkey = 'K';           break;
        case Keyboard::L:          vkey = 'L';           break;
        case Keyboard::M:          vkey = 'M';           break;
        case Keyboard::N:          vkey = 'N';           break;
        case Keyboard::O:          vkey = 'O';           break;
        case Keyboard::P:          vkey = 'P';           break;
        case Keyboard::Q:          vkey = 'Q';           break;
        case Keyboard::R:          vkey = 'R';           break;
        case Keyboard::S:          vkey = 'S';           break;
        case Keyboard::T:          vkey = 'T';           break;
        case Keyboard::U:          vkey = 'U';           break;
        case Keyboard::V:          vkey = 'V';           break;
        case Keyboard::W:          vkey = 'W';           break;
        case Keyboard::X:          vkey = 'X';           break;
        case Keyboard::Y:          vkey = 'Y';           break;
        case Keyboard::Z:          vkey = 'Z';           break;
        case Keyboard::Num0:       vkey = '0';           break;
        case Keyboard::Num1:       vkey = '1';           break;
        case Keyboard::Num2:       vkey = '2';           break;
        case Keyboard::Num3:       vkey = '3';           break;
        case Keyboard::Num4:       vkey = '4';           break;
        case Keyboard::Num5:       vkey = '5';           break;
        case Keyboard::Num6:       vkey = '6';           break;
        case Keyboard::Num7:       vkey = '7';           break;
        case Keyboard::Num8:       vkey = '8';           break;
        case Keyboard::Num9:       vkey = '9';           break;
        case Keyboard::Escape:     vkey = VK_ESCAPE;     break;
        case Keyboard::LControl:   vkey = VK_LCONTROL;   break;
        case Keyboard::LShift:     vkey = VK_LSHIFT;     break;
        case Keyboard::LAlt:       vkey = VK_LMENU;      break;
        case Keyboard::LSystem:    vkey = VK_LWIN;       break;
        case Keyboard::RControl:   vkey = VK_RCONTROL;   break;
        case Keyboard::RShift:     vkey = VK_RSHIFT;     break;
        case Keyboard::RAlt:       vkey = VK_RMENU;      break;
        case Keyboard::RSystem:    vkey = VK_RWIN;       break;
        case Keyboard::Menu:       vkey = VK_APPS;       break;
        case Keyboard::LBracket:   vkey = VK_OEM_4;      break;
        case Keyboard::RBracket:   vkey = VK_OEM_6;      break;
        case Keyboard::Semicolon:  vkey = VK_OEM_1;      break;
        case Keyboard::Comma:      vkey = VK_OEM_COMMA;  break;
        case Keyboard::Period:     vkey = VK_OEM_PERIOD; break;
        case Keyboard::Quote:      vkey = VK_OEM_7;      break;
        case Keyboard::Slash:      vkey = VK_OEM_2;      break;
        case Keyboard::Backslash:  vkey = VK_OEM_5;      break;
        case Keyboard::Tilde:      vkey = VK_OEM_3;      break;
        case Keyboard::Equal:      vkey = VK_OEM_PLUS;   break;
        case Keyboard::Hyphen:     vkey = VK_OEM_MINUS;  break;
        case Keyboard::Space:      vkey = VK_SPACE;      break;
        case Keyboard::Enter:      vkey = VK_RETURN;     break;
        case Keyboard::Backspace:  vkey = VK_BACK;       break;
        case Keyboard::Tab:        vkey = VK_TAB;        break;
        case Keyboard::PageUp:     vkey = VK_PRIOR;      break;
        case Keyboard::PageDown:   vkey = VK_NEXT;       break;
        case Keyboard::End:        vkey = VK_END;        break;
        case Keyboard::Home:       vkey = VK_HOME;       break;
        case Keyboard::Insert:     vkey = VK_INSERT;     break;
        case Keyboard::Delete:     vkey = VK_DELETE;     break;
        case Keyboard::Add:        vkey = VK_ADD;        break;
        case Keyboard::Subtract:   vkey = VK_SUBTRACT;   break;
        case Keyboard::Multiply:   vkey = VK_MULTIPLY;   break;
        case Keyboard::Divide:     vkey = VK_DIVIDE;     break;
        case Keyboard::Left:       vkey = VK_LEFT;       break;
        case Keyboard::Right:      vkey = VK_RIGHT;      break;
        case Keyboard::Up:         vkey = VK_UP;         break;
        case Keyboard::Down:       vkey = VK_DOWN;       break;
        case Keyboard::Numpad0:    vkey = VK_NUMPAD0;    break;
        case Keyboard::Numpad1:    vkey = VK_NUMPAD1;    break;
        case Keyboard::Numpad2:    vkey = VK_NUMPAD2;    break;
        case Keyboard::Numpad3:    vkey = VK_NUMPAD3;    break;
        case Keyboard::Numpad4:    vkey = VK_NUMPAD4;    break;
        case Keyboard::Numpad5:    vkey = VK_NUMPAD5;    break;
        case Keyboard::Numpad6:    vkey = VK_NUMPAD6;    break;
        case Keyboard::Numpad7:    vkey = VK_NUMPAD7;    break;
        case Keyboard::Numpad8:    vkey = VK_NUMPAD8;    break;
        case Keyboard::Numpad9:    vkey = VK_NUMPAD9;    break;
        case Keyboard::F1:         vkey = VK_F1;         break;
        case Keyboard::F2:         vkey = VK_F2;         break;
        case Keyboard::F3:         vkey = VK_F3;         break;
        case Keyboard::F4:         vkey = VK_F4;         break;
        case Keyboard::F5:         vkey = VK_F5;         break;
        case Keyboard::F6:         vkey = VK_F6;         break;
        case Keyboard::F7:         vkey = VK_F7;         break;
        case Keyboard::F8:         vkey = VK_F8;         break;
        case Keyboard::F9:         vkey = VK_F9;         break;
        case Keyboard::F10:        vkey = VK_F10;        break;
        case Keyboard::F11:        vkey = VK_F11;        break;
        case Keyboard::F12:        vkey = VK_F12;        break;
        case Keyboard::F13:        vkey = VK_F13;        break;
        case Keyboard::F14:        vkey = VK_F14;        break;
        case Keyboard::F15:        vkey = VK_F15;        break;
        case Keyboard::Pause:      vkey = VK_PAUSE;      break;
    }
    return vkey;
}

////////////////////////////////////////////////////////////
WORD sfScanToWin(Keyboard::Scancode code)
{
    // Convert an SFML scancode to a windows scancode
    // Reference: https://msdn.microsoft.com/en-us/library/aa299374(v=vs.60).aspx
    switch (code)
    {
        case Keyboard::ScanA: return 30;
        case Keyboard::ScanB: return 48;
        case Keyboard::ScanC: return 46;
        case Keyboard::ScanD: return 32;
        case Keyboard::ScanE: return 18;
        case Keyboard::ScanF: return 33;
        case Keyboard::ScanG: return 34;
        case Keyboard::ScanH: return 35;
        case Keyboard::ScanI: return 23;
        case Keyboard::ScanJ: return 36;
        case Keyboard::ScanK: return 37;
        case Keyboard::ScanL: return 38;
        case Keyboard::ScanM: return 50;
        case Keyboard::ScanN: return 49;
        case Keyboard::ScanO: return 24;
        case Keyboard::ScanP: return 25;
        case Keyboard::ScanQ: return 16;
        case Keyboard::ScanR: return 19;
        case Keyboard::ScanS: return 31;
        case Keyboard::ScanT: return 20;
        case Keyboard::ScanU: return 22;
        case Keyboard::ScanV: return 47;
        case Keyboard::ScanW: return 17;
        case Keyboard::ScanX: return 45;
        case Keyboard::ScanY: return 21;
        case Keyboard::ScanZ: return 44;
    
        case Keyboard::ScanNum1: return 2;
        case Keyboard::ScanNum2: return 3;
        case Keyboard::ScanNum3: return 4;
        case Keyboard::ScanNum4: return 5;
        case Keyboard::ScanNum5: return 6;
        case Keyboard::ScanNum6: return 7;
        case Keyboard::ScanNum7: return 8;
        case Keyboard::ScanNum8: return 9;
        case Keyboard::ScanNum9: return 10;
        case Keyboard::ScanNum0: return 11;
    
        case Keyboard::ScanEnter:           return 28;
        case Keyboard::ScanEscape:          return 1;
        case Keyboard::ScanBackspace:       return 14;
        case Keyboard::ScanTab:             return 15;
        case Keyboard::ScanSpace:           return 57;
        case Keyboard::ScanHyphen:          return 12;
        case Keyboard::ScanEquals:          return 13;
        case Keyboard::ScanLBracket:        return 26;
        case Keyboard::ScanRBracket:        return 27;
        case Keyboard::ScanBackslash:       return 43;
        case Keyboard::ScanDash:            return 41;
        case Keyboard::ScanSemicolon:       return 39;
        case Keyboard::ScanQuote:           return 40;
        //case Keyboard::ScanGraveAccent:   return ? ? ? 
        case Keyboard::ScanComma:           return 51;
        case Keyboard::ScanPeriod:          return 52;
        case Keyboard::ScanSlash:           return 53;
    
        case Keyboard::ScanF1:      return 59;
        case Keyboard::ScanF2:      return 60;
        case Keyboard::ScanF3:      return 61;
        case Keyboard::ScanF4:      return 62;
        case Keyboard::ScanF5:      return 63;
        case Keyboard::ScanF6:      return 64;
        case Keyboard::ScanF7:      return 65;
        case Keyboard::ScanF8:      return 66;
        case Keyboard::ScanF9:      return 67;
        case Keyboard::ScanF10:     return 68;
        case Keyboard::ScanF11:     return KF_EXTENDED | 87;
        case Keyboard::ScanF12:     return KF_EXTENDED | 88;
        //case Keyboard::ScanF13:   return ???
        //case Keyboard::ScanF14:   return ???
        //case Keyboard::ScanF15:   return ???
    
        case Keyboard::ScanCapsLock:    return 58;
        case Keyboard::ScanPrintScreen: return 55 | KF_EXTENDED;
        case Keyboard::ScanScrollLock:  return 70;
        case Keyboard::ScanPause:       return 69;
        case Keyboard::ScanInsert:      return 82 | KF_EXTENDED;
        case Keyboard::ScanHome:        return 71 | KF_EXTENDED;
        case Keyboard::ScanPageUp:      return 73 | KF_EXTENDED;
        case Keyboard::ScanDelete:      return 83 | KF_EXTENDED;
        case Keyboard::ScanEnd:         return 79 | KF_EXTENDED;
        case Keyboard::ScanPageDown:    return 81 | KF_EXTENDED;
        case Keyboard::ScanRight:       return 77 | KF_EXTENDED;
        case Keyboard::ScanLeft:        return 75 | KF_EXTENDED;
        case Keyboard::ScanDown:        return 80 | KF_EXTENDED;
        case Keyboard::ScanUp:          return 72 | KF_EXTENDED;
        case Keyboard::ScanNumLock:     return 69 | KF_EXTENDED;
        case Keyboard::ScanDivide:      return 53;
        case Keyboard::ScanMultiply:    return 55;
        case Keyboard::ScanMinus:       return 74;
        case Keyboard::ScanPlus:        return 78;
        //case Keyboard::ScanPadEquals: return ???;
        case Keyboard::ScanNumpadEnter: return  KF_EXTENDED | 28;
        case Keyboard::ScanDecimal:     return 83;
    
        case Keyboard::ScanNumpad1: return 79;
        case Keyboard::ScanNumpad2: return 80;
        case Keyboard::ScanNumpad3: return 81 ;
        case Keyboard::ScanNumpad4: return 75 ;
        case Keyboard::ScanNumpad5: return 76;
        case Keyboard::ScanNumpad6: return 77 ;
        case Keyboard::ScanNumpad7: return 71 ;
        case Keyboard::ScanNumpad8: return 72 ;
        case Keyboard::ScanNumpad9: return 73 ;
        case Keyboard::ScanNumpad0: return 82 ;
    
        //case Keyboard::ScanReverseSolidus:    return ? ? ? ;
        //case Keyboard::ScanApplication:       return ? ? ? ;
        //case Keyboard::ScanExecute:           return ? ? ? ;
        //case Keyboard::ScanHelp:              return ? ? ? ;
        case Keyboard::ScanMenu:                return 93 | KF_EXTENDED;
        //case Keyboard::ScanSelect:            return ? ? ? ;
        //case Keyboard::ScanStop:              return ? ? ? ;
        //case Keyboard::ScanAgain:             return ? ? ? ;
        //case Keyboard::ScanUndo:              return ? ? ? ;
        //case Keyboard::ScanCut:               return ? ? ? ;
        //case Keyboard::ScanCopy:              return ? ? ? ;
        //case Keyboard::ScanPaste:             return ? ? ? ;
        //case Keyboard::ScanFind:              return ? ? ? ;
        //case Keyboard::ScanMute:              return ? ? ? ;
        //case Keyboard::ScanVolumeUp:          return ? ? ? ;
        //case Keyboard::ScanVolumeDown:        return ? ? ? ;
        case Keyboard::ScanLControl:            return 29;
        case Keyboard::ScanLShift:              return 42;
        case Keyboard::ScanLAlt:                return 56;
        case Keyboard::ScanLSystem:             return 91 | KF_EXTENDED ;
        case Keyboard::ScanRControl:            return KF_EXTENDED | 29;
        case Keyboard::ScanRShift:              return 54;
        case Keyboard::ScanRAlt:                return 56;
        //case Keyboard::ScanRSystem:           return ? ? ? ;
    
        default: return 0; // Not sure what to return here?
    }
}

////////////////////////////////////////////////////////////
InputImpl::InputImpl()
{
    buildMappings();
}

////////////////////////////////////////////////////////////
void InputImpl::buildMappings()
{
    // Reset the mappings
    for (int i = 0; i < Keyboard::KeyCount; ++i)
        m_keyToScancodeMapping[i] = Keyboard::ScanUnknown;
    for (int i = 0; i < Keyboard::ScanCodeCount; ++i)
        m_scancodeToKeyMapping[i] = Keyboard::Unknown;

    for (int i = 0; i < Keyboard::ScanCodeCount; ++i)
    {
        Keyboard::Scancode scan = static_cast<Keyboard::Scancode>(i);
        WORD winScanCode = sfScanToWin(scan);
        UINT vkey = MapVirtualKey(winScanCode, MAPVK_VSC_TO_VK_EX);
        Keyboard::Key key = winKeyToSfKey(vkey);
        m_keyToScancodeMapping[key] = scan;
        m_scancodeToKeyMapping[scan] = key;
    }
}

////////////////////////////////////////////////////////////
bool InputImpl::isKeyPressed(Keyboard::Key key)
{
    int vkey = sfKeyToWin(key);
    return (GetAsyncKeyState(vkey) & 0x8000) != 0;
}

////////////////////////////////////////////////////////////
bool InputImpl::isKeyPressed(Keyboard::Scancode code)
{
    WORD winScanCode = sfScanToWin(code);
    UINT vkey = MapVirtualKey(winScanCode, MAPVK_VSC_TO_VK_EX);
    return (GetAsyncKeyState(vkey) & 0x8000) != 0;
}

////////////////////////////////////////////////////////////
Keyboard::Key InputImpl::localize(Keyboard::Scancode code)
{
    return m_scancodeToKeyMapping[code];
}

////////////////////////////////////////////////////////////
Keyboard::Scancode InputImpl::unlocalize(Keyboard::Key key)
{
    return m_keyToScancodeMapping[key];
}

////////////////////////////////////////////////////////////
String InputImpl::getDescription(Keyboard::Scancode code)
{
    WORD winCode = sfScanToWin(code);
    const int bufSize(1024);
    WCHAR name[bufSize];
    int result = GetKeyNameText(winCode << 16, name, bufSize);
    if (result > 0)
    {
        return name;
    }
    return "Unknown";
}

////////////////////////////////////////////////////////////
void InputImpl::setVirtualKeyboardVisible(bool /*visible*/)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
bool InputImpl::isMouseButtonPressed(Mouse::Button button)
{
    int vkey = 0;
    switch (button)
    {
        case Mouse::Left:     vkey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON; break;
        case Mouse::Right:    vkey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_LBUTTON : VK_RBUTTON; break;
        case Mouse::Middle:   vkey = VK_MBUTTON;  break;
        case Mouse::XButton1: vkey = VK_XBUTTON1; break;
        case Mouse::XButton2: vkey = VK_XBUTTON2; break;
        default:              vkey = 0;           break;
    }

    return (GetAsyncKeyState(vkey) & 0x8000) != 0;
}


////////////////////////////////////////////////////////////
Vector2i InputImpl::getMousePosition()
{
    POINT point;
    GetCursorPos(&point);
    return Vector2i(point.x, point.y);
}


////////////////////////////////////////////////////////////
Vector2i InputImpl::getMousePosition(const WindowBase& relativeTo)
{
    WindowHandle handle = relativeTo.getSystemHandle();
    if (handle)
    {
        POINT point;
        GetCursorPos(&point);
        ScreenToClient(handle, &point);
        return Vector2i(point.x, point.y);
    }
    else
    {
        return Vector2i();
    }
}


////////////////////////////////////////////////////////////
void InputImpl::setMousePosition(const Vector2i& position)
{
    SetCursorPos(position.x, position.y);
}


////////////////////////////////////////////////////////////
void InputImpl::setMousePosition(const Vector2i& position, const WindowBase& relativeTo)
{
    WindowHandle handle = relativeTo.getSystemHandle();
    if (handle)
    {
        POINT point = {position.x, position.y};
        ClientToScreen(handle, &point);
        SetCursorPos(point.x, point.y);
    }
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
Vector2i InputImpl::getTouchPosition(unsigned int /*finger*/, const WindowBase& /*relativeTo*/)
{
    // Not applicable
    return Vector2i();
}

} // namespace priv

} // namespace sf
