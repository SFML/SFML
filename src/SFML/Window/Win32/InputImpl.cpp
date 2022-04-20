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
Keyboard::Scancode InputImpl::m_keyToScancodeMapping[Keyboard::KeyCount];            ///< Mapping from Key to Scancode
Keyboard::Key      InputImpl::m_scancodeToKeyMapping[Keyboard::Scan::ScancodeCount]; ///< Mapping from Scancode to Key

////////////////////////////////////////////////////////////
Keyboard::Key virtualKeyToSfKey(UINT virtualKey)
{
    Keyboard::Key key;
    switch (virtualKey)
    {
        default:            key = Keyboard::Unknown;    break;
        case 'A':           key = Keyboard::A;          break;
        case 'B':           key = Keyboard::B;          break;
        case 'C':           key = Keyboard::C;          break;
        case 'D':           key = Keyboard::D;          break;
        case 'E':           key = Keyboard::E;          break;
        case 'F':           key = Keyboard::F;          break;
        case 'G':           key = Keyboard::G;          break;
        case 'H':           key = Keyboard::H;          break;
        case 'I':           key = Keyboard::I;          break;
        case 'J':           key = Keyboard::J;          break;
        case 'K':           key = Keyboard::K;          break;
        case 'L':           key = Keyboard::L;          break;
        case 'M':           key = Keyboard::M;          break;
        case 'N':           key = Keyboard::N;          break;
        case 'O':           key = Keyboard::O;          break;
        case 'P':           key = Keyboard::P;          break;
        case 'Q':           key = Keyboard::Q;          break;
        case 'R':           key = Keyboard::R;          break;
        case 'S':           key = Keyboard::S;          break;
        case 'T':           key = Keyboard::T;          break;
        case 'U':           key = Keyboard::U;          break;
        case 'V':           key = Keyboard::V;          break;
        case 'W':           key = Keyboard::W;          break;
        case 'X':           key = Keyboard::X;          break;
        case 'Y':           key = Keyboard::Y;          break;
        case 'Z':           key = Keyboard::Z;          break;
        case '0':           key = Keyboard::Num0;       break;
        case '1':           key = Keyboard::Num1;       break;
        case '2':           key = Keyboard::Num2;       break;
        case '3':           key = Keyboard::Num3;       break;
        case '4':           key = Keyboard::Num4;       break;
        case '5':           key = Keyboard::Num5;       break;
        case '6':           key = Keyboard::Num6;       break;
        case '7':           key = Keyboard::Num7;       break;
        case '8':           key = Keyboard::Num8;       break;
        case '9':           key = Keyboard::Num9;       break;
        case VK_ESCAPE:     key = Keyboard::Escape;     break;
        case VK_LCONTROL:   key = Keyboard::LControl;   break;
        case VK_LSHIFT:     key = Keyboard::LShift;     break;
        case VK_LMENU:      key = Keyboard::LAlt;       break;
        case VK_LWIN:       key = Keyboard::LSystem;    break;
        case VK_RCONTROL:   key = Keyboard::RControl;   break;
        case VK_RSHIFT:     key = Keyboard::RShift;     break;
        case VK_RMENU:      key = Keyboard::RAlt;       break;
        case VK_RWIN:       key = Keyboard::RSystem;    break;
        case VK_APPS:       key = Keyboard::Menu;       break;
        case VK_OEM_4:      key = Keyboard::LBracket;   break;
        case VK_OEM_6:      key = Keyboard::RBracket;   break;
        case VK_OEM_1:      key = Keyboard::Semicolon;  break;
        case VK_OEM_COMMA:  key = Keyboard::Comma;      break;
        case VK_OEM_PERIOD: key = Keyboard::Period;     break;
        case VK_OEM_7:      key = Keyboard::Apostrophe; break;
        case VK_OEM_2:      key = Keyboard::Slash;      break;
        case VK_OEM_5:      key = Keyboard::Backslash;  break;
        case VK_OEM_3:      key = Keyboard::Tilde;      break;
        case VK_OEM_PLUS:   key = Keyboard::Equal;      break;
        case VK_OEM_MINUS:  key = Keyboard::Hyphen;     break;
        case VK_SPACE:      key = Keyboard::Space;      break;
        case VK_RETURN:     key = Keyboard::Enter;      break;
        case VK_BACK:       key = Keyboard::Backspace;  break;
        case VK_TAB:        key = Keyboard::Tab;        break;
        case VK_PRIOR:      key = Keyboard::PageUp;     break;
        case VK_NEXT:       key = Keyboard::PageDown;   break;
        case VK_END:        key = Keyboard::End;        break;
        case VK_HOME:       key = Keyboard::Home;       break;
        case VK_INSERT:     key = Keyboard::Insert;     break;
        case VK_DELETE:     key = Keyboard::Delete;     break;
        case VK_ADD:        key = Keyboard::Add;        break;
        case VK_SUBTRACT:   key = Keyboard::Subtract;   break;
        case VK_MULTIPLY:   key = Keyboard::Multiply;   break;
        case VK_DIVIDE:     key = Keyboard::Divide;     break;
        case VK_LEFT:       key = Keyboard::Left;       break;
        case VK_RIGHT:      key = Keyboard::Right;      break;
        case VK_UP:         key = Keyboard::Up;         break;
        case VK_DOWN:       key = Keyboard::Down;       break;
        case VK_NUMPAD0:    key = Keyboard::Numpad0;    break;
        case VK_NUMPAD1:    key = Keyboard::Numpad1;    break;
        case VK_NUMPAD2:    key = Keyboard::Numpad2;    break;
        case VK_NUMPAD3:    key = Keyboard::Numpad3;    break;
        case VK_NUMPAD4:    key = Keyboard::Numpad4;    break;
        case VK_NUMPAD5:    key = Keyboard::Numpad5;    break;
        case VK_NUMPAD6:    key = Keyboard::Numpad6;    break;
        case VK_NUMPAD7:    key = Keyboard::Numpad7;    break;
        case VK_NUMPAD8:    key = Keyboard::Numpad8;    break;
        case VK_NUMPAD9:    key = Keyboard::Numpad9;    break;
        case VK_F1:         key = Keyboard::F1;         break;
        case VK_F2:         key = Keyboard::F2;         break;
        case VK_F3:         key = Keyboard::F3;         break;
        case VK_F4:         key = Keyboard::F4;         break;
        case VK_F5:         key = Keyboard::F5;         break;
        case VK_F6:         key = Keyboard::F6;         break;
        case VK_F7:         key = Keyboard::F7;         break;
        case VK_F8:         key = Keyboard::F8;         break;
        case VK_F9:         key = Keyboard::F9;         break;
        case VK_F10:        key = Keyboard::F10;        break;
        case VK_F11:        key = Keyboard::F11;        break;
        case VK_F12:        key = Keyboard::F12;        break;
        case VK_F13:        key = Keyboard::F13;        break;
        case VK_F14:        key = Keyboard::F14;        break;
        case VK_F15:        key = Keyboard::F15;        break;
        case VK_PAUSE:      key = Keyboard::Pause;      break;
    }
    return key;
}

////////////////////////////////////////////////////////////
int sfKeyToVirtualKey(Keyboard::Key key)
{
    int virtualKey = 0;
    switch (key)
    {
        default:                   virtualKey = 0;             break;
        case Keyboard::A:          virtualKey = 'A';           break;
        case Keyboard::B:          virtualKey = 'B';           break;
        case Keyboard::C:          virtualKey = 'C';           break;
        case Keyboard::D:          virtualKey = 'D';           break;
        case Keyboard::E:          virtualKey = 'E';           break;
        case Keyboard::F:          virtualKey = 'F';           break;
        case Keyboard::G:          virtualKey = 'G';           break;
        case Keyboard::H:          virtualKey = 'H';           break;
        case Keyboard::I:          virtualKey = 'I';           break;
        case Keyboard::J:          virtualKey = 'J';           break;
        case Keyboard::K:          virtualKey = 'K';           break;
        case Keyboard::L:          virtualKey = 'L';           break;
        case Keyboard::M:          virtualKey = 'M';           break;
        case Keyboard::N:          virtualKey = 'N';           break;
        case Keyboard::O:          virtualKey = 'O';           break;
        case Keyboard::P:          virtualKey = 'P';           break;
        case Keyboard::Q:          virtualKey = 'Q';           break;
        case Keyboard::R:          virtualKey = 'R';           break;
        case Keyboard::S:          virtualKey = 'S';           break;
        case Keyboard::T:          virtualKey = 'T';           break;
        case Keyboard::U:          virtualKey = 'U';           break;
        case Keyboard::V:          virtualKey = 'V';           break;
        case Keyboard::W:          virtualKey = 'W';           break;
        case Keyboard::X:          virtualKey = 'X';           break;
        case Keyboard::Y:          virtualKey = 'Y';           break;
        case Keyboard::Z:          virtualKey = 'Z';           break;
        case Keyboard::Num0:       virtualKey = '0';           break;
        case Keyboard::Num1:       virtualKey = '1';           break;
        case Keyboard::Num2:       virtualKey = '2';           break;
        case Keyboard::Num3:       virtualKey = '3';           break;
        case Keyboard::Num4:       virtualKey = '4';           break;
        case Keyboard::Num5:       virtualKey = '5';           break;
        case Keyboard::Num6:       virtualKey = '6';           break;
        case Keyboard::Num7:       virtualKey = '7';           break;
        case Keyboard::Num8:       virtualKey = '8';           break;
        case Keyboard::Num9:       virtualKey = '9';           break;
        case Keyboard::Escape:     virtualKey = VK_ESCAPE;     break;
        case Keyboard::LControl:   virtualKey = VK_LCONTROL;   break;
        case Keyboard::LShift:     virtualKey = VK_LSHIFT;     break;
        case Keyboard::LAlt:       virtualKey = VK_LMENU;      break;
        case Keyboard::LSystem:    virtualKey = VK_LWIN;       break;
        case Keyboard::RControl:   virtualKey = VK_RCONTROL;   break;
        case Keyboard::RShift:     virtualKey = VK_RSHIFT;     break;
        case Keyboard::RAlt:       virtualKey = VK_RMENU;      break;
        case Keyboard::RSystem:    virtualKey = VK_RWIN;       break;
        case Keyboard::Menu:       virtualKey = VK_APPS;       break;
        case Keyboard::LBracket:   virtualKey = VK_OEM_4;      break;
        case Keyboard::RBracket:   virtualKey = VK_OEM_6;      break;
        case Keyboard::Semicolon:  virtualKey = VK_OEM_1;      break;
        case Keyboard::Comma:      virtualKey = VK_OEM_COMMA;  break;
        case Keyboard::Period:     virtualKey = VK_OEM_PERIOD; break;
        case Keyboard::Apostrophe: virtualKey = VK_OEM_7;      break;
        case Keyboard::Slash:      virtualKey = VK_OEM_2;      break;
        case Keyboard::Backslash:  virtualKey = VK_OEM_5;      break;
        case Keyboard::Tilde:      virtualKey = VK_OEM_3;      break;
        case Keyboard::Equal:      virtualKey = VK_OEM_PLUS;   break;
        case Keyboard::Hyphen:     virtualKey = VK_OEM_MINUS;  break;
        case Keyboard::Space:      virtualKey = VK_SPACE;      break;
        case Keyboard::Enter:      virtualKey = VK_RETURN;     break;
        case Keyboard::Backspace:  virtualKey = VK_BACK;       break;
        case Keyboard::Tab:        virtualKey = VK_TAB;        break;
        case Keyboard::PageUp:     virtualKey = VK_PRIOR;      break;
        case Keyboard::PageDown:   virtualKey = VK_NEXT;       break;
        case Keyboard::End:        virtualKey = VK_END;        break;
        case Keyboard::Home:       virtualKey = VK_HOME;       break;
        case Keyboard::Insert:     virtualKey = VK_INSERT;     break;
        case Keyboard::Delete:     virtualKey = VK_DELETE;     break;
        case Keyboard::Add:        virtualKey = VK_ADD;        break;
        case Keyboard::Subtract:   virtualKey = VK_SUBTRACT;   break;
        case Keyboard::Multiply:   virtualKey = VK_MULTIPLY;   break;
        case Keyboard::Divide:     virtualKey = VK_DIVIDE;     break;
        case Keyboard::Left:       virtualKey = VK_LEFT;       break;
        case Keyboard::Right:      virtualKey = VK_RIGHT;      break;
        case Keyboard::Up:         virtualKey = VK_UP;         break;
        case Keyboard::Down:       virtualKey = VK_DOWN;       break;
        case Keyboard::Numpad0:    virtualKey = VK_NUMPAD0;    break;
        case Keyboard::Numpad1:    virtualKey = VK_NUMPAD1;    break;
        case Keyboard::Numpad2:    virtualKey = VK_NUMPAD2;    break;
        case Keyboard::Numpad3:    virtualKey = VK_NUMPAD3;    break;
        case Keyboard::Numpad4:    virtualKey = VK_NUMPAD4;    break;
        case Keyboard::Numpad5:    virtualKey = VK_NUMPAD5;    break;
        case Keyboard::Numpad6:    virtualKey = VK_NUMPAD6;    break;
        case Keyboard::Numpad7:    virtualKey = VK_NUMPAD7;    break;
        case Keyboard::Numpad8:    virtualKey = VK_NUMPAD8;    break;
        case Keyboard::Numpad9:    virtualKey = VK_NUMPAD9;    break;
        case Keyboard::F1:         virtualKey = VK_F1;         break;
        case Keyboard::F2:         virtualKey = VK_F2;         break;
        case Keyboard::F3:         virtualKey = VK_F3;         break;
        case Keyboard::F4:         virtualKey = VK_F4;         break;
        case Keyboard::F5:         virtualKey = VK_F5;         break;
        case Keyboard::F6:         virtualKey = VK_F6;         break;
        case Keyboard::F7:         virtualKey = VK_F7;         break;
        case Keyboard::F8:         virtualKey = VK_F8;         break;
        case Keyboard::F9:         virtualKey = VK_F9;         break;
        case Keyboard::F10:        virtualKey = VK_F10;        break;
        case Keyboard::F11:        virtualKey = VK_F11;        break;
        case Keyboard::F12:        virtualKey = VK_F12;        break;
        case Keyboard::F13:        virtualKey = VK_F13;        break;
        case Keyboard::F14:        virtualKey = VK_F14;        break;
        case Keyboard::F15:        virtualKey = VK_F15;        break;
        case Keyboard::Pause:      virtualKey = VK_PAUSE;      break;
    }
    return virtualKey;
}

////////////////////////////////////////////////////////////
WORD sfScanToWinScan(Keyboard::Scancode code)
{
    // Convert an SFML scancode to a Windows scancode
    // Reference: https://msdn.microsoft.com/en-us/library/aa299374(v=vs.60).aspx
    switch (code)
    {
        case Keyboard::Scan::A: return 0x1E;
        case Keyboard::Scan::B: return 0x30;
        case Keyboard::Scan::C: return 0x2E;
        case Keyboard::Scan::D: return 0x20;
        case Keyboard::Scan::E: return 0x12;
        case Keyboard::Scan::F: return 0x21;
        case Keyboard::Scan::G: return 0x22;
        case Keyboard::Scan::H: return 0x23;
        case Keyboard::Scan::I: return 0x17;
        case Keyboard::Scan::J: return 0x24;
        case Keyboard::Scan::K: return 0x25;
        case Keyboard::Scan::L: return 0x26;
        case Keyboard::Scan::M: return 0x32;
        case Keyboard::Scan::N: return 0x31;
        case Keyboard::Scan::O: return 0x18;
        case Keyboard::Scan::P: return 0x19;
        case Keyboard::Scan::Q: return 0x10;
        case Keyboard::Scan::R: return 0x13;
        case Keyboard::Scan::S: return 0x1F;
        case Keyboard::Scan::T: return 0x14;
        case Keyboard::Scan::U: return 0x16;
        case Keyboard::Scan::V: return 0x2F;
        case Keyboard::Scan::W: return 0x11;
        case Keyboard::Scan::X: return 0x2D;
        case Keyboard::Scan::Y: return 0x15;
        case Keyboard::Scan::Z: return 0x2C;
    
        case Keyboard::Scan::Num1: return 0x02;
        case Keyboard::Scan::Num2: return 0x03;
        case Keyboard::Scan::Num3: return 0x04;
        case Keyboard::Scan::Num4: return 0x05;
        case Keyboard::Scan::Num5: return 0x06;
        case Keyboard::Scan::Num6: return 0x07;
        case Keyboard::Scan::Num7: return 0x08;
        case Keyboard::Scan::Num8: return 0x09;
        case Keyboard::Scan::Num9: return 0x0A;
        case Keyboard::Scan::Num0: return 0x0B;
    
        case Keyboard::Scan::Enter:      return 0x1C;
        case Keyboard::Scan::Escape:     return 0x01;
        case Keyboard::Scan::Backspace:  return 0x0E;
        case Keyboard::Scan::Tab:        return 0x0F;
        case Keyboard::Scan::Space:      return 0x39;
        case Keyboard::Scan::Hyphen:     return 0x0C;
        case Keyboard::Scan::Equal:      return 0x0D;
        case Keyboard::Scan::LBracket:   return 0x1A;
        case Keyboard::Scan::RBracket:   return 0x1B;
        case Keyboard::Scan::Backslash:  return 0x2B;
        case Keyboard::Scan::Semicolon:  return 0x27;
        case Keyboard::Scan::Apostrophe: return 0x28;
        case Keyboard::Scan::Grave:      return 0x29;
        case Keyboard::Scan::Comma:      return 0x33;
        case Keyboard::Scan::Period:     return 0x34;
        case Keyboard::Scan::Slash:      return 0x35;
    
        case Keyboard::Scan::F1:  return 0x3B;
        case Keyboard::Scan::F2:  return 0x3C;
        case Keyboard::Scan::F3:  return 0x3D;
        case Keyboard::Scan::F4:  return 0x3E;
        case Keyboard::Scan::F5:  return 0x3F;
        case Keyboard::Scan::F6:  return 0x40;
        case Keyboard::Scan::F7:  return 0x41;
        case Keyboard::Scan::F8:  return 0x42;
        case Keyboard::Scan::F9:  return 0x43;
        case Keyboard::Scan::F10: return 0x44;
        case Keyboard::Scan::F11: return 0x57;
        case Keyboard::Scan::F12: return 0x58;
        case Keyboard::Scan::F13: return 0x64;
        case Keyboard::Scan::F14: return 0x65;
        case Keyboard::Scan::F15: return 0x66;
        case Keyboard::Scan::F16: return 0x67;
        case Keyboard::Scan::F17: return 0x68;
        case Keyboard::Scan::F18: return 0x69;
        case Keyboard::Scan::F19: return 0x6A;
        case Keyboard::Scan::F20: return 0x6B;
        case Keyboard::Scan::F21: return 0x6C;
        case Keyboard::Scan::F22: return 0x6D;
        case Keyboard::Scan::F23: return 0x6E;
        case Keyboard::Scan::F24: return 0x76;
    
        case Keyboard::Scan::CapsLock:    return 0x3A;
        case Keyboard::Scan::PrintScreen: return 0xE037;
        case Keyboard::Scan::ScrollLock:  return 0x46;
        case Keyboard::Scan::Pause:       return 0x45;
        case Keyboard::Scan::Insert:      return 0xE052;
        case Keyboard::Scan::Home:        return 0xE047;
        case Keyboard::Scan::PageUp:      return 0xE049;
        case Keyboard::Scan::Delete:      return 0xE053;
        case Keyboard::Scan::End:         return 0xE04F;
        case Keyboard::Scan::PageDown:    return 0xE051;
        case Keyboard::Scan::Right:       return 0xE04D;
        case Keyboard::Scan::Left:        return 0xE04B;
        case Keyboard::Scan::Down:        return 0xE050;
        case Keyboard::Scan::Up:          return 0xE048;
        case Keyboard::Scan::NumLock:     return 0xE045;

        case Keyboard::Scan::NumpadDivide:   return 0xE035;
        case Keyboard::Scan::NumpadMultiply: return 0x37;
        case Keyboard::Scan::NumpadMinus:    return 0x4A;
        case Keyboard::Scan::NumpadPlus:     return 0x4E;
        case Keyboard::Scan::NumpadEqual:    return 0x7E;
        case Keyboard::Scan::NumpadEnter:    return 0xE01C;
        case Keyboard::Scan::NumpadDecimal:  return 0x53;
        
        case Keyboard::Scan::Numpad1:        return 0x4F;
        case Keyboard::Scan::Numpad2:        return 0x50;
        case Keyboard::Scan::Numpad3:        return 0x51;
        case Keyboard::Scan::Numpad4:        return 0x4B;
        case Keyboard::Scan::Numpad5:        return 0x4C;
        case Keyboard::Scan::Numpad6:        return 0x4D;
        case Keyboard::Scan::Numpad7:        return 0x47;
        case Keyboard::Scan::Numpad8:        return 0x48;
        case Keyboard::Scan::Numpad9:        return 0x49;
        case Keyboard::Scan::Numpad0:        return 0x52;
    
        case Keyboard::Scan::NonUsBackslash: return 0x56;
        // No known scancode for Keyboard::Scan::Application
        // No known scancode for Keyboard::Scan::Execute
        // No known scancode for Keyboard::Scan::ModeChange
        case Keyboard::Scan::Help:           return 0xE061;
        case Keyboard::Scan::Menu:           return 0xE05D;
        case Keyboard::Scan::Select:         return 0xE01E;
        // No known scancode for Keyboard::Scan::Redo
        // No known scancode for Keyboard::Scan::Undo
        // No known scancode for Keyboard::Scan::Cut
        // No known scancode for Keyboard::Scan::Copy
        // No known scancode for Keyboard::Scan::Paste

        case Keyboard::Scan::VolumeMute:         return 0xE020;
        case Keyboard::Scan::VolumeUp:           return 0xE02E;
        case Keyboard::Scan::VolumeDown:         return 0xE02C;
        case Keyboard::Scan::MediaPlayPause:     return 0xE022;
        case Keyboard::Scan::MediaStop:          return 0xE024;
        case Keyboard::Scan::MediaNextTrack:     return 0xE019;
        case Keyboard::Scan::MediaPreviousTrack: return 0xE010;

        case Keyboard::Scan::LControl: return 0x1D;
        case Keyboard::Scan::LShift:   return 0x2A;
        case Keyboard::Scan::LAlt:     return 0x38;
        case Keyboard::Scan::LSystem:  return 0xE05B;
        case Keyboard::Scan::RControl: return 0xE01D;
        case Keyboard::Scan::RShift:   return 0x36;
        case Keyboard::Scan::RAlt:     return 0xE038;
        case Keyboard::Scan::RSystem:  return 0xE05C;

        case Keyboard::Scan::Back:      return 0xE06A;
        case Keyboard::Scan::Forward:   return 0xE069;
        case Keyboard::Scan::Refresh:   return 0xE067;
        case Keyboard::Scan::Stop:      return 0xE068;
        case Keyboard::Scan::Search:    return 0xE065;
        case Keyboard::Scan::Favorites: return 0xE066;
        case Keyboard::Scan::HomePage:  return 0xE030;

        case Keyboard::Scan::LaunchApplication1: return 0xE06B;
        case Keyboard::Scan::LaunchApplication2: return 0xE021;
        case Keyboard::Scan::LaunchMail:         return 0xE06C;
        case Keyboard::Scan::LaunchMediaSelect:  return 0xE06D;

        // Unable to map to a scancode
        default: return 0x0;
    }
}

////////////////////////////////////////////////////////////
WORD sfScanToWinScanExtended(Keyboard::Scancode code)
{
    // Convert an SFML scancode to a Windows scancode
    // Reference: https://msdn.microsoft.com/en-us/library/aa299374(v=vs.60).aspx
    switch (code)
    {
        case Keyboard::Scan::PrintScreen:        return 55  | 0xE100;
        case Keyboard::Scan::Insert:             return 82  | 0xE100;
        case Keyboard::Scan::Home:               return 71  | 0xE100;
        case Keyboard::Scan::PageUp:             return 73  | 0xE100;
        case Keyboard::Scan::Delete:             return 83  | 0xE100;
        case Keyboard::Scan::End:                return 79  | 0xE100;
        case Keyboard::Scan::PageDown:           return 81  | 0xE100;
        case Keyboard::Scan::Right:              return 77  | 0xE100;
        case Keyboard::Scan::Left:               return 75  | 0xE100;
        case Keyboard::Scan::Down:               return 80  | 0xE100;
        case Keyboard::Scan::Up:                 return 72  | 0xE100;
        case Keyboard::Scan::NumLock:            return 69  | 0xE100;
        case Keyboard::Scan::NumpadEnter:        return 28  | 0xE100;
        case Keyboard::Scan::NumpadDivide:       return 53  | 0xE100;
        case Keyboard::Scan::Help:               return 97  | 0xE100;
        case Keyboard::Scan::Menu:               return 93  | 0xE100;
        case Keyboard::Scan::Select:             return 30  | 0xE100;
        case Keyboard::Scan::VolumeMute:         return 32  | 0xE100;
        case Keyboard::Scan::VolumeUp:           return 46  | 0xE100;
        case Keyboard::Scan::VolumeDown:         return 44  | 0xE100;
        case Keyboard::Scan::MediaPlayPause:     return 34  | 0xE100;
        case Keyboard::Scan::MediaStop:          return 36  | 0xE100;
        case Keyboard::Scan::MediaNextTrack:     return 25  | 0xE100;
        case Keyboard::Scan::MediaPreviousTrack: return 16  | 0xE100;
        case Keyboard::Scan::LSystem:            return 91  | 0xE100;
        case Keyboard::Scan::RControl:           return 29  | 0xE100;
        case Keyboard::Scan::RAlt:               return 56  | 0xE100;
        case Keyboard::Scan::RSystem:            return 92  | 0xE100;
        case Keyboard::Scan::Back:               return 106 | 0xE100;
        case Keyboard::Scan::Forward:            return 105 | 0xE100;
        case Keyboard::Scan::Refresh:            return 103 | 0xE100;
        case Keyboard::Scan::Stop:               return 104 | 0xE100;
        case Keyboard::Scan::Search:             return 101 | 0xE100;
        case Keyboard::Scan::Favorites:          return 102 | 0xE100;
        case Keyboard::Scan::HomePage:           return 48  | 0xE100;
        case Keyboard::Scan::LaunchApplication1: return 107 | 0xE100;
        case Keyboard::Scan::LaunchApplication2: return 33  | 0xE100;
        case Keyboard::Scan::LaunchMail:         return 108 | 0xE100;
        case Keyboard::Scan::LaunchMediaSelect:  return 109 | 0xE100;

        // Use non-extended mapping
        default: return sfScanToWinScan(code);
    }
}

UINT sfScanToVirtualKey(Keyboard::Scancode code)
{
    WORD winScancode = sfScanToWinScan(code);

    // Manually map non-extended key codes
    switch (code)
    {
        case Keyboard::Scan::Numpad0:
            return VK_NUMPAD0;
        case Keyboard::Scan::Numpad1:
            return VK_NUMPAD1;
        case Keyboard::Scan::Numpad2:
            return VK_NUMPAD2;
        case Keyboard::Scan::Numpad3:
            return VK_NUMPAD3;
        case Keyboard::Scan::Numpad4:
            return VK_NUMPAD4;
        case Keyboard::Scan::Numpad5:
            return VK_NUMPAD5;
        case Keyboard::Scan::Numpad6:
            return VK_NUMPAD6;
        case Keyboard::Scan::Numpad7:
            return VK_NUMPAD7;
        case Keyboard::Scan::Numpad8:
            return VK_NUMPAD8;
        case Keyboard::Scan::Numpad9:
            return VK_NUMPAD9;
        case Keyboard::Scan::NumpadMinus:
            return VK_SUBTRACT;
        case Keyboard::Scan::NumpadDecimal:
            return VK_DECIMAL;
        case Keyboard::Scan::NumpadDivide:
            return VK_DIVIDE;
        case Keyboard::Scan::Pause:
            return VK_PAUSE;
        case Keyboard::Scan::RControl:
            return VK_RCONTROL;
        case Keyboard::Scan::RAlt:
            return VK_RMENU;
        default:
            return MapVirtualKey(winScancode, MAPVK_VSC_TO_VK_EX);
    }
}

bool isValidScancode(Keyboard::Scancode code)
{
    return code > Keyboard::Scan::Unknown && code < Keyboard::Scan::ScancodeCount;
}

bool isValidKey(Keyboard::Key key)
{
    return key > Keyboard::Unknown && key < Keyboard::KeyCount;
}

////////////////////////////////////////////////////////////
void InputImpl::ensureMappings()
{
    static bool isMappingInitialized = false;

    if (isMappingInitialized)
        return;

    // Phase 1: Initialize mappings with default values
    for (int i = 0; i < Keyboard::KeyCount; ++i)
        m_keyToScancodeMapping[i] = Keyboard::Scan::Unknown;

    for (int i = 0; i < Keyboard::Scan::ScancodeCount; ++i)
        m_scancodeToKeyMapping[i] = Keyboard::Unknown;

    // Phase 2: Translate scancode to virtual code to key names
    for (int i = 0; i < Keyboard::Scan::ScancodeCount; ++i)
    {
        Keyboard::Scancode scan = static_cast<Keyboard::Scancode>(i);
        UINT virtualKey = sfScanToVirtualKey(scan);
        Keyboard::Key key = virtualKeyToSfKey(virtualKey);
        if (key != Keyboard::Unknown && m_keyToScancodeMapping[key] == Keyboard::Scan::Unknown)
            m_keyToScancodeMapping[key] = scan;
        m_scancodeToKeyMapping[scan] = key;
    }

    isMappingInitialized = true;
}

////////////////////////////////////////////////////////////
bool InputImpl::isKeyPressed(Keyboard::Key key)
{
    int virtualKey = sfKeyToVirtualKey(key);
    return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
}

////////////////////////////////////////////////////////////
bool InputImpl::isKeyPressed(Keyboard::Scancode code)
{
    UINT virtualKey = sfScanToVirtualKey(code);
    return (GetAsyncKeyState(static_cast<int>(virtualKey)) & KF_UP) != 0;
}

////////////////////////////////////////////////////////////
Keyboard::Key InputImpl::localize(Keyboard::Scancode code)
{
    if (!isValidScancode(code))
        return Keyboard::Unknown;

    ensureMappings();

    return m_scancodeToKeyMapping[code];
}

////////////////////////////////////////////////////////////
Keyboard::Scancode InputImpl::delocalize(Keyboard::Key key)
{
    if (!isValidKey(key))
        return Keyboard::Scan::Unknown;

    ensureMappings();

    return m_keyToScancodeMapping[key];
}

////////////////////////////////////////////////////////////
String InputImpl::getDescription(Keyboard::Scancode code)
{
    WORD winCode = sfScanToWinScanExtended(code);
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
    int virtualKey = 0;
    switch (button)
    {
        case Mouse::Left:     virtualKey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON; break;
        case Mouse::Right:    virtualKey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_LBUTTON : VK_RBUTTON; break;
        case Mouse::Middle:   virtualKey = VK_MBUTTON;  break;
        case Mouse::XButton1: virtualKey = VK_XBUTTON1; break;
        case Mouse::XButton2: virtualKey = VK_XBUTTON2; break;
        default:              virtualKey = 0;           break;
    }

    return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
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
