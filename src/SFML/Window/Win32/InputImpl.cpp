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
#include <SFML/Window/InputImpl.hpp>
#include <SFML/Window/Window.hpp>

#include <SFML/System/String.hpp>
#include <SFML/System/Win32/WindowsHeader.hpp>

namespace
{
sf::Keyboard::Scancode keyToScancodeMapping[sf::Keyboard::KeyCount]; ///< Mapping from Key to Scancode
sf::Keyboard::Key scancodeToKeyMapping[static_cast<std::size_t>(sf::Keyboard::Scan::ScancodeCount)]; ///< Mapping from Scancode to Key

sf::Keyboard::Key virtualKeyToSfKey(UINT virtualKey)
{
    // clang-format off
    switch (virtualKey)
    {
        case 'A':           return sf::Keyboard::A;
        case 'B':           return sf::Keyboard::B;
        case 'C':           return sf::Keyboard::C;
        case 'D':           return sf::Keyboard::D;
        case 'E':           return sf::Keyboard::E;
        case 'F':           return sf::Keyboard::F;
        case 'G':           return sf::Keyboard::G;
        case 'H':           return sf::Keyboard::H;
        case 'I':           return sf::Keyboard::I;
        case 'J':           return sf::Keyboard::J;
        case 'K':           return sf::Keyboard::K;
        case 'L':           return sf::Keyboard::L;
        case 'M':           return sf::Keyboard::M;
        case 'N':           return sf::Keyboard::N;
        case 'O':           return sf::Keyboard::O;
        case 'P':           return sf::Keyboard::P;
        case 'Q':           return sf::Keyboard::Q;
        case 'R':           return sf::Keyboard::R;
        case 'S':           return sf::Keyboard::S;
        case 'T':           return sf::Keyboard::T;
        case 'U':           return sf::Keyboard::U;
        case 'V':           return sf::Keyboard::V;
        case 'W':           return sf::Keyboard::W;
        case 'X':           return sf::Keyboard::X;
        case 'Y':           return sf::Keyboard::Y;
        case 'Z':           return sf::Keyboard::Z;
        case '0':           return sf::Keyboard::Num0;
        case '1':           return sf::Keyboard::Num1;
        case '2':           return sf::Keyboard::Num2;
        case '3':           return sf::Keyboard::Num3;
        case '4':           return sf::Keyboard::Num4;
        case '5':           return sf::Keyboard::Num5;
        case '6':           return sf::Keyboard::Num6;
        case '7':           return sf::Keyboard::Num7;
        case '8':           return sf::Keyboard::Num8;
        case '9':           return sf::Keyboard::Num9;
        case VK_ESCAPE:     return sf::Keyboard::Escape;
        case VK_LCONTROL:   return sf::Keyboard::LControl;
        case VK_LSHIFT:     return sf::Keyboard::LShift;
        case VK_LMENU:      return sf::Keyboard::LAlt;
        case VK_LWIN:       return sf::Keyboard::LSystem;
        case VK_RCONTROL:   return sf::Keyboard::RControl;
        case VK_RSHIFT:     return sf::Keyboard::RShift;
        case VK_RMENU:      return sf::Keyboard::RAlt;
        case VK_RWIN:       return sf::Keyboard::RSystem;
        case VK_APPS:       return sf::Keyboard::Menu;
        case VK_OEM_4:      return sf::Keyboard::LBracket;
        case VK_OEM_6:      return sf::Keyboard::RBracket;
        case VK_OEM_1:      return sf::Keyboard::Semicolon;
        case VK_OEM_COMMA:  return sf::Keyboard::Comma;
        case VK_OEM_PERIOD: return sf::Keyboard::Period;
        case VK_OEM_7:      return sf::Keyboard::Apostrophe;
        case VK_OEM_2:      return sf::Keyboard::Slash;
        case VK_OEM_5:      return sf::Keyboard::Backslash;
        case VK_OEM_3:      return sf::Keyboard::Grave;
        case VK_OEM_PLUS:   return sf::Keyboard::Equal;
        case VK_OEM_MINUS:  return sf::Keyboard::Hyphen;
        case VK_SPACE:      return sf::Keyboard::Space;
        case VK_RETURN:     return sf::Keyboard::Enter;
        case VK_BACK:       return sf::Keyboard::Backspace;
        case VK_TAB:        return sf::Keyboard::Tab;
        case VK_PRIOR:      return sf::Keyboard::PageUp;
        case VK_NEXT:       return sf::Keyboard::PageDown;
        case VK_END:        return sf::Keyboard::End;
        case VK_HOME:       return sf::Keyboard::Home;
        case VK_INSERT:     return sf::Keyboard::Insert;
        case VK_DELETE:     return sf::Keyboard::Delete;
        case VK_ADD:        return sf::Keyboard::Add;
        case VK_SUBTRACT:   return sf::Keyboard::Subtract;
        case VK_MULTIPLY:   return sf::Keyboard::Multiply;
        case VK_DIVIDE:     return sf::Keyboard::Divide;
        case VK_LEFT:       return sf::Keyboard::Left;
        case VK_RIGHT:      return sf::Keyboard::Right;
        case VK_UP:         return sf::Keyboard::Up;
        case VK_DOWN:       return sf::Keyboard::Down;
        case VK_NUMPAD0:    return sf::Keyboard::Numpad0;
        case VK_NUMPAD1:    return sf::Keyboard::Numpad1;
        case VK_NUMPAD2:    return sf::Keyboard::Numpad2;
        case VK_NUMPAD3:    return sf::Keyboard::Numpad3;
        case VK_NUMPAD4:    return sf::Keyboard::Numpad4;
        case VK_NUMPAD5:    return sf::Keyboard::Numpad5;
        case VK_NUMPAD6:    return sf::Keyboard::Numpad6;
        case VK_NUMPAD7:    return sf::Keyboard::Numpad7;
        case VK_NUMPAD8:    return sf::Keyboard::Numpad8;
        case VK_NUMPAD9:    return sf::Keyboard::Numpad9;
        case VK_F1:         return sf::Keyboard::F1;
        case VK_F2:         return sf::Keyboard::F2;
        case VK_F3:         return sf::Keyboard::F3;
        case VK_F4:         return sf::Keyboard::F4;
        case VK_F5:         return sf::Keyboard::F5;
        case VK_F6:         return sf::Keyboard::F6;
        case VK_F7:         return sf::Keyboard::F7;
        case VK_F8:         return sf::Keyboard::F8;
        case VK_F9:         return sf::Keyboard::F9;
        case VK_F10:        return sf::Keyboard::F10;
        case VK_F11:        return sf::Keyboard::F11;
        case VK_F12:        return sf::Keyboard::F12;
        case VK_F13:        return sf::Keyboard::F13;
        case VK_F14:        return sf::Keyboard::F14;
        case VK_F15:        return sf::Keyboard::F15;
        case VK_PAUSE:      return sf::Keyboard::Pause;
        default:            return sf::Keyboard::Unknown;
    }
    // clang-format on
}

int sfKeyToVirtualKey(sf::Keyboard::Key key)
{
    // clang-format off
    switch (key)
    {
        case sf::Keyboard::A:          return 'A';
        case sf::Keyboard::B:          return 'B';
        case sf::Keyboard::C:          return 'C';
        case sf::Keyboard::D:          return 'D';
        case sf::Keyboard::E:          return 'E';
        case sf::Keyboard::F:          return 'F';
        case sf::Keyboard::G:          return 'G';
        case sf::Keyboard::H:          return 'H';
        case sf::Keyboard::I:          return 'I';
        case sf::Keyboard::J:          return 'J';
        case sf::Keyboard::K:          return 'K';
        case sf::Keyboard::L:          return 'L';
        case sf::Keyboard::M:          return 'M';
        case sf::Keyboard::N:          return 'N';
        case sf::Keyboard::O:          return 'O';
        case sf::Keyboard::P:          return 'P';
        case sf::Keyboard::Q:          return 'Q';
        case sf::Keyboard::R:          return 'R';
        case sf::Keyboard::S:          return 'S';
        case sf::Keyboard::T:          return 'T';
        case sf::Keyboard::U:          return 'U';
        case sf::Keyboard::V:          return 'V';
        case sf::Keyboard::W:          return 'W';
        case sf::Keyboard::X:          return 'X';
        case sf::Keyboard::Y:          return 'Y';
        case sf::Keyboard::Z:          return 'Z';
        case sf::Keyboard::Num0:       return '0';
        case sf::Keyboard::Num1:       return '1';
        case sf::Keyboard::Num2:       return '2';
        case sf::Keyboard::Num3:       return '3';
        case sf::Keyboard::Num4:       return '4';
        case sf::Keyboard::Num5:       return '5';
        case sf::Keyboard::Num6:       return '6';
        case sf::Keyboard::Num7:       return '7';
        case sf::Keyboard::Num8:       return '8';
        case sf::Keyboard::Num9:       return '9';
        case sf::Keyboard::Escape:     return VK_ESCAPE;
        case sf::Keyboard::LControl:   return VK_LCONTROL;
        case sf::Keyboard::LShift:     return VK_LSHIFT;
        case sf::Keyboard::LAlt:       return VK_LMENU;
        case sf::Keyboard::LSystem:    return VK_LWIN;
        case sf::Keyboard::RControl:   return VK_RCONTROL;
        case sf::Keyboard::RShift:     return VK_RSHIFT;
        case sf::Keyboard::RAlt:       return VK_RMENU;
        case sf::Keyboard::RSystem:    return VK_RWIN;
        case sf::Keyboard::Menu:       return VK_APPS;
        case sf::Keyboard::LBracket:   return VK_OEM_4;
        case sf::Keyboard::RBracket:   return VK_OEM_6;
        case sf::Keyboard::Semicolon:  return VK_OEM_1;
        case sf::Keyboard::Comma:      return VK_OEM_COMMA;
        case sf::Keyboard::Period:     return VK_OEM_PERIOD;
        case sf::Keyboard::Apostrophe: return VK_OEM_7;
        case sf::Keyboard::Slash:      return VK_OEM_2;
        case sf::Keyboard::Backslash:  return VK_OEM_5;
        case sf::Keyboard::Grave:      return VK_OEM_3;
        case sf::Keyboard::Equal:      return VK_OEM_PLUS;
        case sf::Keyboard::Hyphen:     return VK_OEM_MINUS;
        case sf::Keyboard::Space:      return VK_SPACE;
        case sf::Keyboard::Enter:      return VK_RETURN;
        case sf::Keyboard::Backspace:  return VK_BACK;
        case sf::Keyboard::Tab:        return VK_TAB;
        case sf::Keyboard::PageUp:     return VK_PRIOR;
        case sf::Keyboard::PageDown:   return VK_NEXT;
        case sf::Keyboard::End:        return VK_END;
        case sf::Keyboard::Home:       return VK_HOME;
        case sf::Keyboard::Insert:     return VK_INSERT;
        case sf::Keyboard::Delete:     return VK_DELETE;
        case sf::Keyboard::Add:        return VK_ADD;
        case sf::Keyboard::Subtract:   return VK_SUBTRACT;
        case sf::Keyboard::Multiply:   return VK_MULTIPLY;
        case sf::Keyboard::Divide:     return VK_DIVIDE;
        case sf::Keyboard::Left:       return VK_LEFT;
        case sf::Keyboard::Right:      return VK_RIGHT;
        case sf::Keyboard::Up:         return VK_UP;
        case sf::Keyboard::Down:       return VK_DOWN;
        case sf::Keyboard::Numpad0:    return VK_NUMPAD0;
        case sf::Keyboard::Numpad1:    return VK_NUMPAD1;
        case sf::Keyboard::Numpad2:    return VK_NUMPAD2;
        case sf::Keyboard::Numpad3:    return VK_NUMPAD3;
        case sf::Keyboard::Numpad4:    return VK_NUMPAD4;
        case sf::Keyboard::Numpad5:    return VK_NUMPAD5;
        case sf::Keyboard::Numpad6:    return VK_NUMPAD6;
        case sf::Keyboard::Numpad7:    return VK_NUMPAD7;
        case sf::Keyboard::Numpad8:    return VK_NUMPAD8;
        case sf::Keyboard::Numpad9:    return VK_NUMPAD9;
        case sf::Keyboard::F1:         return VK_F1;
        case sf::Keyboard::F2:         return VK_F2;
        case sf::Keyboard::F3:         return VK_F3;
        case sf::Keyboard::F4:         return VK_F4;
        case sf::Keyboard::F5:         return VK_F5;
        case sf::Keyboard::F6:         return VK_F6;
        case sf::Keyboard::F7:         return VK_F7;
        case sf::Keyboard::F8:         return VK_F8;
        case sf::Keyboard::F9:         return VK_F9;
        case sf::Keyboard::F10:        return VK_F10;
        case sf::Keyboard::F11:        return VK_F11;
        case sf::Keyboard::F12:        return VK_F12;
        case sf::Keyboard::F13:        return VK_F13;
        case sf::Keyboard::F14:        return VK_F14;
        case sf::Keyboard::F15:        return VK_F15;
        case sf::Keyboard::Pause:      return VK_PAUSE;
        default:                   return 0;
    }
    // clang-format on
}

WORD sfScanToWinScan(sf::Keyboard::Scancode code)
{
    // Convert an SFML scancode to a Windows scancode
    // Reference: https://msdn.microsoft.com/en-us/library/aa299374(v=vs.60).aspx
    // clang-format off
    switch (code)
    {
        case sf::Keyboard::Scan::A: return 0x1E;
        case sf::Keyboard::Scan::B: return 0x30;
        case sf::Keyboard::Scan::C: return 0x2E;
        case sf::Keyboard::Scan::D: return 0x20;
        case sf::Keyboard::Scan::E: return 0x12;
        case sf::Keyboard::Scan::F: return 0x21;
        case sf::Keyboard::Scan::G: return 0x22;
        case sf::Keyboard::Scan::H: return 0x23;
        case sf::Keyboard::Scan::I: return 0x17;
        case sf::Keyboard::Scan::J: return 0x24;
        case sf::Keyboard::Scan::K: return 0x25;
        case sf::Keyboard::Scan::L: return 0x26;
        case sf::Keyboard::Scan::M: return 0x32;
        case sf::Keyboard::Scan::N: return 0x31;
        case sf::Keyboard::Scan::O: return 0x18;
        case sf::Keyboard::Scan::P: return 0x19;
        case sf::Keyboard::Scan::Q: return 0x10;
        case sf::Keyboard::Scan::R: return 0x13;
        case sf::Keyboard::Scan::S: return 0x1F;
        case sf::Keyboard::Scan::T: return 0x14;
        case sf::Keyboard::Scan::U: return 0x16;
        case sf::Keyboard::Scan::V: return 0x2F;
        case sf::Keyboard::Scan::W: return 0x11;
        case sf::Keyboard::Scan::X: return 0x2D;
        case sf::Keyboard::Scan::Y: return 0x15;
        case sf::Keyboard::Scan::Z: return 0x2C;

        case sf::Keyboard::Scan::Num1: return 0x02;
        case sf::Keyboard::Scan::Num2: return 0x03;
        case sf::Keyboard::Scan::Num3: return 0x04;
        case sf::Keyboard::Scan::Num4: return 0x05;
        case sf::Keyboard::Scan::Num5: return 0x06;
        case sf::Keyboard::Scan::Num6: return 0x07;
        case sf::Keyboard::Scan::Num7: return 0x08;
        case sf::Keyboard::Scan::Num8: return 0x09;
        case sf::Keyboard::Scan::Num9: return 0x0A;
        case sf::Keyboard::Scan::Num0: return 0x0B;

        case sf::Keyboard::Scan::Enter:      return 0x1C;
        case sf::Keyboard::Scan::Escape:     return 0x01;
        case sf::Keyboard::Scan::Backspace:  return 0x0E;
        case sf::Keyboard::Scan::Tab:        return 0x0F;
        case sf::Keyboard::Scan::Space:      return 0x39;
        case sf::Keyboard::Scan::Hyphen:     return 0x0C;
        case sf::Keyboard::Scan::Equal:      return 0x0D;
        case sf::Keyboard::Scan::LBracket:   return 0x1A;
        case sf::Keyboard::Scan::RBracket:   return 0x1B;
        case sf::Keyboard::Scan::Backslash:  return 0x2B;
        case sf::Keyboard::Scan::Semicolon:  return 0x27;
        case sf::Keyboard::Scan::Apostrophe: return 0x28;
        case sf::Keyboard::Scan::Grave:      return 0x29;
        case sf::Keyboard::Scan::Comma:      return 0x33;
        case sf::Keyboard::Scan::Period:     return 0x34;
        case sf::Keyboard::Scan::Slash:      return 0x35;

        case sf::Keyboard::Scan::F1:  return 0x3B;
        case sf::Keyboard::Scan::F2:  return 0x3C;
        case sf::Keyboard::Scan::F3:  return 0x3D;
        case sf::Keyboard::Scan::F4:  return 0x3E;
        case sf::Keyboard::Scan::F5:  return 0x3F;
        case sf::Keyboard::Scan::F6:  return 0x40;
        case sf::Keyboard::Scan::F7:  return 0x41;
        case sf::Keyboard::Scan::F8:  return 0x42;
        case sf::Keyboard::Scan::F9:  return 0x43;
        case sf::Keyboard::Scan::F10: return 0x44;
        case sf::Keyboard::Scan::F11: return 0x57;
        case sf::Keyboard::Scan::F12: return 0x58;
        case sf::Keyboard::Scan::F13: return 0x64;
        case sf::Keyboard::Scan::F14: return 0x65;
        case sf::Keyboard::Scan::F15: return 0x66;
        case sf::Keyboard::Scan::F16: return 0x67;
        case sf::Keyboard::Scan::F17: return 0x68;
        case sf::Keyboard::Scan::F18: return 0x69;
        case sf::Keyboard::Scan::F19: return 0x6A;
        case sf::Keyboard::Scan::F20: return 0x6B;
        case sf::Keyboard::Scan::F21: return 0x6C;
        case sf::Keyboard::Scan::F22: return 0x6D;
        case sf::Keyboard::Scan::F23: return 0x6E;
        case sf::Keyboard::Scan::F24: return 0x76;

        case sf::Keyboard::Scan::CapsLock:    return 0x3A;
        case sf::Keyboard::Scan::PrintScreen: return 0xE037;
        case sf::Keyboard::Scan::ScrollLock:  return 0x46;
        case sf::Keyboard::Scan::Pause:       return 0x45;
        case sf::Keyboard::Scan::Insert:      return 0xE052;
        case sf::Keyboard::Scan::Home:        return 0xE047;
        case sf::Keyboard::Scan::PageUp:      return 0xE049;
        case sf::Keyboard::Scan::Delete:      return 0xE053;
        case sf::Keyboard::Scan::End:         return 0xE04F;
        case sf::Keyboard::Scan::PageDown:    return 0xE051;
        case sf::Keyboard::Scan::Right:       return 0xE04D;
        case sf::Keyboard::Scan::Left:        return 0xE04B;
        case sf::Keyboard::Scan::Down:        return 0xE050;
        case sf::Keyboard::Scan::Up:          return 0xE048;
        case sf::Keyboard::Scan::NumLock:     return 0xE045;

        case sf::Keyboard::Scan::NumpadDivide:   return 0xE035;
        case sf::Keyboard::Scan::NumpadMultiply: return 0x37;
        case sf::Keyboard::Scan::NumpadMinus:    return 0x4A;
        case sf::Keyboard::Scan::NumpadPlus:     return 0x4E;
        case sf::Keyboard::Scan::NumpadEqual:    return 0x7E;
        case sf::Keyboard::Scan::NumpadEnter:    return 0xE01C;
        case sf::Keyboard::Scan::NumpadDecimal:  return 0x53;

        case sf::Keyboard::Scan::Numpad1:        return 0x4F;
        case sf::Keyboard::Scan::Numpad2:        return 0x50;
        case sf::Keyboard::Scan::Numpad3:        return 0x51;
        case sf::Keyboard::Scan::Numpad4:        return 0x4B;
        case sf::Keyboard::Scan::Numpad5:        return 0x4C;
        case sf::Keyboard::Scan::Numpad6:        return 0x4D;
        case sf::Keyboard::Scan::Numpad7:        return 0x47;
        case sf::Keyboard::Scan::Numpad8:        return 0x48;
        case sf::Keyboard::Scan::Numpad9:        return 0x49;
        case sf::Keyboard::Scan::Numpad0:        return 0x52;

        case sf::Keyboard::Scan::NonUsBackslash: return 0x56;
        // No known scancode for Keyboard::Scan::Application
        // No known scancode for Keyboard::Scan::Execute
        // No known scancode for Keyboard::Scan::ModeChange
        case sf::Keyboard::Scan::Help:           return 0xE061;
        case sf::Keyboard::Scan::Menu:           return 0xE05D;
        case sf::Keyboard::Scan::Select:         return 0xE01E;
        // No known scancode for Keyboard::Scan::Redo
        // No known scancode for Keyboard::Scan::Undo
        // No known scancode for Keyboard::Scan::Cut
        // No known scancode for Keyboard::Scan::Copy
        // No known scancode for Keyboard::Scan::Paste

        case sf::Keyboard::Scan::VolumeMute:         return 0xE020;
        case sf::Keyboard::Scan::VolumeUp:           return 0xE02E;
        case sf::Keyboard::Scan::VolumeDown:         return 0xE02C;
        case sf::Keyboard::Scan::MediaPlayPause:     return 0xE022;
        case sf::Keyboard::Scan::MediaStop:          return 0xE024;
        case sf::Keyboard::Scan::MediaNextTrack:     return 0xE019;
        case sf::Keyboard::Scan::MediaPreviousTrack: return 0xE010;

        case sf::Keyboard::Scan::LControl: return 0x1D;
        case sf::Keyboard::Scan::LShift:   return 0x2A;
        case sf::Keyboard::Scan::LAlt:     return 0x38;
        case sf::Keyboard::Scan::LSystem:  return 0xE05B;
        case sf::Keyboard::Scan::RControl: return 0xE01D;
        case sf::Keyboard::Scan::RShift:   return 0x36;
        case sf::Keyboard::Scan::RAlt:     return 0xE038;
        case sf::Keyboard::Scan::RSystem:  return 0xE05C;

        case sf::Keyboard::Scan::Back:      return 0xE06A;
        case sf::Keyboard::Scan::Forward:   return 0xE069;
        case sf::Keyboard::Scan::Refresh:   return 0xE067;
        case sf::Keyboard::Scan::Stop:      return 0xE068;
        case sf::Keyboard::Scan::Search:    return 0xE065;
        case sf::Keyboard::Scan::Favorites: return 0xE066;
        case sf::Keyboard::Scan::HomePage:  return 0xE030;

        case sf::Keyboard::Scan::LaunchApplication1: return 0xE06B;
        case sf::Keyboard::Scan::LaunchApplication2: return 0xE021;
        case sf::Keyboard::Scan::LaunchMail:         return 0xE06C;
        case sf::Keyboard::Scan::LaunchMediaSelect:  return 0xE06D;

        // Unable to map to a scancode
        default: return 0x0;
    }
    // clang-format on
}

WORD sfScanToWinScanExtended(sf::Keyboard::Scancode code)
{
    // Convert an SFML scancode to a Windows scancode
    // Reference: https://msdn.microsoft.com/en-us/library/aa299374(v=vs.60).aspx
    // clang-format off
    switch (code)
    {
        case sf::Keyboard::Scan::PrintScreen:        return 55  | 0xE100;
        case sf::Keyboard::Scan::Insert:             return 82  | 0xE100;
        case sf::Keyboard::Scan::Home:               return 71  | 0xE100;
        case sf::Keyboard::Scan::PageUp:             return 73  | 0xE100;
        case sf::Keyboard::Scan::Delete:             return 83  | 0xE100;
        case sf::Keyboard::Scan::End:                return 79  | 0xE100;
        case sf::Keyboard::Scan::PageDown:           return 81  | 0xE100;
        case sf::Keyboard::Scan::Right:              return 77  | 0xE100;
        case sf::Keyboard::Scan::Left:               return 75  | 0xE100;
        case sf::Keyboard::Scan::Down:               return 80  | 0xE100;
        case sf::Keyboard::Scan::Up:                 return 72  | 0xE100;
        case sf::Keyboard::Scan::NumLock:            return 69  | 0xE100;
        case sf::Keyboard::Scan::NumpadEnter:        return 28  | 0xE100;
        case sf::Keyboard::Scan::NumpadDivide:       return 53  | 0xE100;
        case sf::Keyboard::Scan::Help:               return 97  | 0xE100;
        case sf::Keyboard::Scan::Menu:               return 93  | 0xE100;
        case sf::Keyboard::Scan::Select:             return 30  | 0xE100;
        case sf::Keyboard::Scan::VolumeMute:         return 32  | 0xE100;
        case sf::Keyboard::Scan::VolumeUp:           return 46  | 0xE100;
        case sf::Keyboard::Scan::VolumeDown:         return 44  | 0xE100;
        case sf::Keyboard::Scan::MediaPlayPause:     return 34  | 0xE100;
        case sf::Keyboard::Scan::MediaStop:          return 36  | 0xE100;
        case sf::Keyboard::Scan::MediaNextTrack:     return 25  | 0xE100;
        case sf::Keyboard::Scan::MediaPreviousTrack: return 16  | 0xE100;
        case sf::Keyboard::Scan::LSystem:            return 91  | 0xE100;
        case sf::Keyboard::Scan::RControl:           return 29  | 0xE100;
        case sf::Keyboard::Scan::RAlt:               return 56  | 0xE100;
        case sf::Keyboard::Scan::RSystem:            return 92  | 0xE100;
        case sf::Keyboard::Scan::Back:               return 106 | 0xE100;
        case sf::Keyboard::Scan::Forward:            return 105 | 0xE100;
        case sf::Keyboard::Scan::Refresh:            return 103 | 0xE100;
        case sf::Keyboard::Scan::Stop:               return 104 | 0xE100;
        case sf::Keyboard::Scan::Search:             return 101 | 0xE100;
        case sf::Keyboard::Scan::Favorites:          return 102 | 0xE100;
        case sf::Keyboard::Scan::HomePage:           return 48  | 0xE100;
        case sf::Keyboard::Scan::LaunchApplication1: return 107 | 0xE100;
        case sf::Keyboard::Scan::LaunchApplication2: return 33  | 0xE100;
        case sf::Keyboard::Scan::LaunchMail:         return 108 | 0xE100;
        case sf::Keyboard::Scan::LaunchMediaSelect:  return 109 | 0xE100;

        // Use non-extended mapping
        default: return sfScanToWinScan(code);
    }
    // clang-format on
}

UINT sfScanToVirtualKey(sf::Keyboard::Scancode code)
{
    const WORD winScancode = sfScanToWinScan(code);

    // Manually map non-extended key codes
    // clang-format off
    switch (code)
    {
        case sf::Keyboard::Scan::Numpad0:       return VK_NUMPAD0;
        case sf::Keyboard::Scan::Numpad1:       return VK_NUMPAD1;
        case sf::Keyboard::Scan::Numpad2:       return VK_NUMPAD2;
        case sf::Keyboard::Scan::Numpad3:       return VK_NUMPAD3;
        case sf::Keyboard::Scan::Numpad4:       return VK_NUMPAD4;
        case sf::Keyboard::Scan::Numpad5:       return VK_NUMPAD5;
        case sf::Keyboard::Scan::Numpad6:       return VK_NUMPAD6;
        case sf::Keyboard::Scan::Numpad7:       return VK_NUMPAD7;
        case sf::Keyboard::Scan::Numpad8:       return VK_NUMPAD8;
        case sf::Keyboard::Scan::Numpad9:       return VK_NUMPAD9;
        case sf::Keyboard::Scan::NumpadMinus:   return VK_SUBTRACT;
        case sf::Keyboard::Scan::NumpadDecimal: return VK_DECIMAL;
        case sf::Keyboard::Scan::NumpadDivide:  return VK_DIVIDE;
        case sf::Keyboard::Scan::Pause:         return VK_PAUSE;
        case sf::Keyboard::Scan::RControl:      return VK_RCONTROL;
        case sf::Keyboard::Scan::RAlt:          return VK_RMENU;
        default:                            return MapVirtualKey(winScancode, MAPVK_VSC_TO_VK_EX);
    }
    // clang-format on
}

/// Ensure the mappings are generated from/to Key and Scancode.
void ensureMappings()
{
    static bool isMappingInitialized = false;

    if (isMappingInitialized)
        return;

    // Phase 1: Initialize mappings with default values
    for (auto& scancode : keyToScancodeMapping)
        scancode = sf::Keyboard::Scan::Unknown;

    for (auto& key : scancodeToKeyMapping)
        key = sf::Keyboard::Unknown;

    // Phase 2: Translate scancode to virtual code to key names
    for (int i = 0; i < static_cast<int>(sf::Keyboard::Scan::ScancodeCount); ++i)
    {
        const auto              scan       = static_cast<sf::Keyboard::Scancode>(i);
        const UINT              virtualKey = sfScanToVirtualKey(scan);
        const sf::Keyboard::Key key        = virtualKeyToSfKey(virtualKey);
        if (key != sf::Keyboard::Unknown && keyToScancodeMapping[key] == sf::Keyboard::Scan::Unknown)
            keyToScancodeMapping[key] = scan;
        scancodeToKeyMapping[static_cast<std::size_t>(scan)] = key;
    }

    isMappingInitialized = true;
}

bool isValidScancode(sf::Keyboard::Scancode code)
{
    return code > sf::Keyboard::Scan::Unknown && code < sf::Keyboard::Scan::ScancodeCount;
}

bool isValidKey(sf::Keyboard::Key key)
{
    return key > sf::Keyboard::Unknown && key < sf::Keyboard::KeyCount;
}

} // namespace

namespace sf::priv::InputImpl
{
////////////////////////////////////////////////////////////
bool isKeyPressed(Keyboard::Key key)
{
    const int virtualKey = sfKeyToVirtualKey(key);
    return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
}


////////////////////////////////////////////////////////////
bool isKeyPressed(Keyboard::Scancode code)
{
    const UINT virtualKey = sfScanToVirtualKey(code);
    return (GetAsyncKeyState(static_cast<int>(virtualKey)) & KF_UP) != 0;
}


////////////////////////////////////////////////////////////
Keyboard::Key localize(Keyboard::Scancode code)
{
    if (!isValidScancode(code))
        return Keyboard::Unknown;

    ensureMappings();

    return scancodeToKeyMapping[static_cast<std::size_t>(code)];
}


////////////////////////////////////////////////////////////
Keyboard::Scancode delocalize(Keyboard::Key key)
{
    if (!isValidKey(key))
        return Keyboard::Scan::Unknown;

    ensureMappings();

    return keyToScancodeMapping[key];
}


////////////////////////////////////////////////////////////
String getDescription(Keyboard::Scancode code)
{
    const WORD winCode = sfScanToWinScanExtended(code);
    const int  bufSize = 1024;
    WCHAR      name[bufSize];
    const int  result = GetKeyNameText(winCode << 16, name, bufSize);
    if (result > 0)
    {
        return name;
    }
    return "Unknown";
}


////////////////////////////////////////////////////////////
void setVirtualKeyboardVisible(bool /*visible*/)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
bool isMouseButtonPressed(Mouse::Button button)
{
    int virtualKey = 0;
    switch (button)
    {
        case Mouse::Left:
            virtualKey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON;
            break;
        case Mouse::Right:
            virtualKey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_LBUTTON : VK_RBUTTON;
            break;
        case Mouse::Middle:
            virtualKey = VK_MBUTTON;
            break;
        case Mouse::XButton1:
            virtualKey = VK_XBUTTON1;
            break;
        case Mouse::XButton2:
            virtualKey = VK_XBUTTON2;
            break;
        default:
            virtualKey = 0;
            break;
    }

    return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
}


////////////////////////////////////////////////////////////
Vector2i getMousePosition()
{
    POINT point;
    GetCursorPos(&point);
    return {point.x, point.y};
}


////////////////////////////////////////////////////////////
Vector2i getMousePosition(const WindowBase& relativeTo)
{
    WindowHandle handle = relativeTo.getNativeHandle();
    if (handle)
    {
        POINT point;
        GetCursorPos(&point);
        ScreenToClient(handle, &point);
        return {point.x, point.y};
    }
    else
    {
        return {};
    }
}


////////////////////////////////////////////////////////////
void setMousePosition(const Vector2i& position)
{
    SetCursorPos(position.x, position.y);
}


////////////////////////////////////////////////////////////
void setMousePosition(const Vector2i& position, const WindowBase& relativeTo)
{
    WindowHandle handle = relativeTo.getNativeHandle();
    if (handle)
    {
        POINT point = {position.x, position.y};
        ClientToScreen(handle, &point);
        SetCursorPos(point.x, point.y);
    }
}


////////////////////////////////////////////////////////////
bool isTouchDown(unsigned int /*finger*/)
{
    // Not applicable
    return false;
}


////////////////////////////////////////////////////////////
Vector2i getTouchPosition(unsigned int /*finger*/)
{
    // Not applicable
    return {};
}


////////////////////////////////////////////////////////////
Vector2i getTouchPosition(unsigned int /*finger*/, const WindowBase& /*relativeTo*/)
{
    // Not applicable
    return {};
}

} // namespace sf::priv::InputImpl
