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
    switch (virtualKey)
    {
        case 'A':           return Keyboard::A;
        case 'B':           return Keyboard::B;
        case 'C':           return Keyboard::C;
        case 'D':           return Keyboard::D;
        case 'E':           return Keyboard::E;
        case 'F':           return Keyboard::F;
        case 'G':           return Keyboard::G;
        case 'H':           return Keyboard::H;
        case 'I':           return Keyboard::I;
        case 'J':           return Keyboard::J;
        case 'K':           return Keyboard::K;
        case 'L':           return Keyboard::L;
        case 'M':           return Keyboard::M;
        case 'N':           return Keyboard::N;
        case 'O':           return Keyboard::O;
        case 'P':           return Keyboard::P;
        case 'Q':           return Keyboard::Q;
        case 'R':           return Keyboard::R;
        case 'S':           return Keyboard::S;
        case 'T':           return Keyboard::T;
        case 'U':           return Keyboard::U;
        case 'V':           return Keyboard::V;
        case 'W':           return Keyboard::W;
        case 'X':           return Keyboard::X;
        case 'Y':           return Keyboard::Y;
        case 'Z':           return Keyboard::Z;
        case '0':           return Keyboard::Num0;
        case '1':           return Keyboard::Num1;
        case '2':           return Keyboard::Num2;
        case '3':           return Keyboard::Num3;
        case '4':           return Keyboard::Num4;
        case '5':           return Keyboard::Num5;
        case '6':           return Keyboard::Num6;
        case '7':           return Keyboard::Num7;
        case '8':           return Keyboard::Num8;
        case '9':           return Keyboard::Num9;
        case VK_ESCAPE:     return Keyboard::Escape;
        case VK_LCONTROL:   return Keyboard::LControl;
        case VK_LSHIFT:     return Keyboard::LShift;
        case VK_LMENU:      return Keyboard::LAlt;
        case VK_LWIN:       return Keyboard::LSystem;
        case VK_RCONTROL:   return Keyboard::RControl;
        case VK_RSHIFT:     return Keyboard::RShift;
        case VK_RMENU:      return Keyboard::RAlt;
        case VK_RWIN:       return Keyboard::RSystem;
        case VK_APPS:       return Keyboard::Menu;
        case VK_OEM_4:      return Keyboard::LBracket;
        case VK_OEM_6:      return Keyboard::RBracket;
        case VK_OEM_1:      return Keyboard::Semicolon;
        case VK_OEM_COMMA:  return Keyboard::Comma;
        case VK_OEM_PERIOD: return Keyboard::Period;
        case VK_OEM_7:      return Keyboard::Apostrophe;
        case VK_OEM_2:      return Keyboard::Slash;
        case VK_OEM_5:      return Keyboard::Backslash;
        case VK_OEM_3:      return Keyboard::Grave;
        case VK_OEM_PLUS:   return Keyboard::Equal;
        case VK_OEM_MINUS:  return Keyboard::Hyphen;
        case VK_SPACE:      return Keyboard::Space;
        case VK_RETURN:     return Keyboard::Enter;
        case VK_BACK:       return Keyboard::Backspace;
        case VK_TAB:        return Keyboard::Tab;
        case VK_PRIOR:      return Keyboard::PageUp;
        case VK_NEXT:       return Keyboard::PageDown;
        case VK_END:        return Keyboard::End;
        case VK_HOME:       return Keyboard::Home;
        case VK_INSERT:     return Keyboard::Insert;
        case VK_DELETE:     return Keyboard::Delete;
        case VK_ADD:        return Keyboard::Add;
        case VK_SUBTRACT:   return Keyboard::Subtract;
        case VK_MULTIPLY:   return Keyboard::Multiply;
        case VK_DIVIDE:     return Keyboard::Divide;
        case VK_LEFT:       return Keyboard::Left;
        case VK_RIGHT:      return Keyboard::Right;
        case VK_UP:         return Keyboard::Up;
        case VK_DOWN:       return Keyboard::Down;
        case VK_NUMPAD0:    return Keyboard::Numpad0;
        case VK_NUMPAD1:    return Keyboard::Numpad1;
        case VK_NUMPAD2:    return Keyboard::Numpad2;
        case VK_NUMPAD3:    return Keyboard::Numpad3;
        case VK_NUMPAD4:    return Keyboard::Numpad4;
        case VK_NUMPAD5:    return Keyboard::Numpad5;
        case VK_NUMPAD6:    return Keyboard::Numpad6;
        case VK_NUMPAD7:    return Keyboard::Numpad7;
        case VK_NUMPAD8:    return Keyboard::Numpad8;
        case VK_NUMPAD9:    return Keyboard::Numpad9;
        case VK_F1:         return Keyboard::F1;
        case VK_F2:         return Keyboard::F2;
        case VK_F3:         return Keyboard::F3;
        case VK_F4:         return Keyboard::F4;
        case VK_F5:         return Keyboard::F5;
        case VK_F6:         return Keyboard::F6;
        case VK_F7:         return Keyboard::F7;
        case VK_F8:         return Keyboard::F8;
        case VK_F9:         return Keyboard::F9;
        case VK_F10:        return Keyboard::F10;
        case VK_F11:        return Keyboard::F11;
        case VK_F12:        return Keyboard::F12;
        case VK_F13:        return Keyboard::F13;
        case VK_F14:        return Keyboard::F14;
        case VK_F15:        return Keyboard::F15;
        case VK_PAUSE:      return Keyboard::Pause;
        default:            return Keyboard::Unknown;
    }
}

////////////////////////////////////////////////////////////
int sfKeyToVirtualKey(Keyboard::Key key)
{
    switch (key)
    {      
        case Keyboard::A:          return 'A';
        case Keyboard::B:          return 'B';
        case Keyboard::C:          return 'C';
        case Keyboard::D:          return 'D';
        case Keyboard::E:          return 'E';
        case Keyboard::F:          return 'F';
        case Keyboard::G:          return 'G';
        case Keyboard::H:          return 'H';
        case Keyboard::I:          return 'I';
        case Keyboard::J:          return 'J';
        case Keyboard::K:          return 'K';
        case Keyboard::L:          return 'L';
        case Keyboard::M:          return 'M';
        case Keyboard::N:          return 'N';
        case Keyboard::O:          return 'O';
        case Keyboard::P:          return 'P';
        case Keyboard::Q:          return 'Q';
        case Keyboard::R:          return 'R';
        case Keyboard::S:          return 'S';
        case Keyboard::T:          return 'T';
        case Keyboard::U:          return 'U';
        case Keyboard::V:          return 'V';
        case Keyboard::W:          return 'W';
        case Keyboard::X:          return 'X';
        case Keyboard::Y:          return 'Y';
        case Keyboard::Z:          return 'Z';
        case Keyboard::Num0:       return '0';
        case Keyboard::Num1:       return '1';
        case Keyboard::Num2:       return '2';
        case Keyboard::Num3:       return '3';
        case Keyboard::Num4:       return '4';
        case Keyboard::Num5:       return '5';
        case Keyboard::Num6:       return '6';
        case Keyboard::Num7:       return '7';
        case Keyboard::Num8:       return '8';
        case Keyboard::Num9:       return '9';
        case Keyboard::Escape:     return VK_ESCAPE;
        case Keyboard::LControl:   return VK_LCONTROL;
        case Keyboard::LShift:     return VK_LSHIFT;
        case Keyboard::LAlt:       return VK_LMENU;
        case Keyboard::LSystem:    return VK_LWIN;
        case Keyboard::RControl:   return VK_RCONTROL;
        case Keyboard::RShift:     return VK_RSHIFT;
        case Keyboard::RAlt:       return VK_RMENU;
        case Keyboard::RSystem:    return VK_RWIN;
        case Keyboard::Menu:       return VK_APPS;
        case Keyboard::LBracket:   return VK_OEM_4;
        case Keyboard::RBracket:   return VK_OEM_6;
        case Keyboard::Semicolon:  return VK_OEM_1;
        case Keyboard::Comma:      return VK_OEM_COMMA;
        case Keyboard::Period:     return VK_OEM_PERIOD;
        case Keyboard::Apostrophe: return VK_OEM_7;
        case Keyboard::Slash:      return VK_OEM_2;
        case Keyboard::Backslash:  return VK_OEM_5;
        case Keyboard::Grave:      return VK_OEM_3;
        case Keyboard::Equal:      return VK_OEM_PLUS;
        case Keyboard::Hyphen:     return VK_OEM_MINUS;
        case Keyboard::Space:      return VK_SPACE;
        case Keyboard::Enter:      return VK_RETURN;
        case Keyboard::Backspace:  return VK_BACK;
        case Keyboard::Tab:        return VK_TAB;
        case Keyboard::PageUp:     return VK_PRIOR; 
        case Keyboard::PageDown:   return VK_NEXT;
        case Keyboard::End:        return VK_END;
        case Keyboard::Home:       return VK_HOME;
        case Keyboard::Insert:     return VK_INSERT;
        case Keyboard::Delete:     return VK_DELETE;
        case Keyboard::Add:        return VK_ADD;
        case Keyboard::Subtract:   return VK_SUBTRACT;
        case Keyboard::Multiply:   return VK_MULTIPLY;
        case Keyboard::Divide:     return VK_DIVIDE;
        case Keyboard::Left:       return VK_LEFT;
        case Keyboard::Right:      return VK_RIGHT;
        case Keyboard::Up:         return VK_UP;
        case Keyboard::Down:       return VK_DOWN;
        case Keyboard::Numpad0:    return VK_NUMPAD0;
        case Keyboard::Numpad1:    return VK_NUMPAD1;
        case Keyboard::Numpad2:    return VK_NUMPAD2;
        case Keyboard::Numpad3:    return VK_NUMPAD3;
        case Keyboard::Numpad4:    return VK_NUMPAD4;
        case Keyboard::Numpad5:    return VK_NUMPAD5;
        case Keyboard::Numpad6:    return VK_NUMPAD6;
        case Keyboard::Numpad7:    return VK_NUMPAD7;
        case Keyboard::Numpad8:    return VK_NUMPAD8;
        case Keyboard::Numpad9:    return VK_NUMPAD9;
        case Keyboard::F1:         return VK_F1;
        case Keyboard::F2:         return VK_F2;
        case Keyboard::F3:         return VK_F3;
        case Keyboard::F4:         return VK_F4;
        case Keyboard::F5:         return VK_F5;
        case Keyboard::F6:         return VK_F6;
        case Keyboard::F7:         return VK_F7;
        case Keyboard::F8:         return VK_F8;
        case Keyboard::F9:         return VK_F9;
        case Keyboard::F10:        return VK_F10;
        case Keyboard::F11:        return VK_F11;
        case Keyboard::F12:        return VK_F12;
        case Keyboard::F13:        return VK_F13;
        case Keyboard::F14:        return VK_F14;
        case Keyboard::F15:        return VK_F15;
        case Keyboard::Pause:      return VK_PAUSE;
        default:                   return 0;
    }
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
    const int bufSize = 1024;
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
