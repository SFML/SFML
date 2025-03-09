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
#include <SFML/Window/InputImpl.hpp>
#include <SFML/Window/Window.hpp>

#include <SFML/System/EnumArray.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Win32/WindowsHeader.hpp>

#include <optional>


namespace
{
sf::priv::EnumArray<sf::Keyboard::Key, sf::Keyboard::Scancode, sf::Keyboard::KeyCount> keyToScancodeMapping; ///< Mapping from Key to Scancode
sf::priv::EnumArray<sf::Keyboard::Scancode, sf::Keyboard::Key, sf::Keyboard::ScancodeCount> scancodeToKeyMapping; ///< Mapping from Scancode to Key

sf::Keyboard::Key virtualKeyToSfKey(UINT virtualKey)
{
    // clang-format off
    switch (virtualKey)
    {
        case 'A':           return sf::Keyboard::Key::A;
        case 'B':           return sf::Keyboard::Key::B;
        case 'C':           return sf::Keyboard::Key::C;
        case 'D':           return sf::Keyboard::Key::D;
        case 'E':           return sf::Keyboard::Key::E;
        case 'F':           return sf::Keyboard::Key::F;
        case 'G':           return sf::Keyboard::Key::G;
        case 'H':           return sf::Keyboard::Key::H;
        case 'I':           return sf::Keyboard::Key::I;
        case 'J':           return sf::Keyboard::Key::J;
        case 'K':           return sf::Keyboard::Key::K;
        case 'L':           return sf::Keyboard::Key::L;
        case 'M':           return sf::Keyboard::Key::M;
        case 'N':           return sf::Keyboard::Key::N;
        case 'O':           return sf::Keyboard::Key::O;
        case 'P':           return sf::Keyboard::Key::P;
        case 'Q':           return sf::Keyboard::Key::Q;
        case 'R':           return sf::Keyboard::Key::R;
        case 'S':           return sf::Keyboard::Key::S;
        case 'T':           return sf::Keyboard::Key::T;
        case 'U':           return sf::Keyboard::Key::U;
        case 'V':           return sf::Keyboard::Key::V;
        case 'W':           return sf::Keyboard::Key::W;
        case 'X':           return sf::Keyboard::Key::X;
        case 'Y':           return sf::Keyboard::Key::Y;
        case 'Z':           return sf::Keyboard::Key::Z;
        case '0':           return sf::Keyboard::Key::Num0;
        case '1':           return sf::Keyboard::Key::Num1;
        case '2':           return sf::Keyboard::Key::Num2;
        case '3':           return sf::Keyboard::Key::Num3;
        case '4':           return sf::Keyboard::Key::Num4;
        case '5':           return sf::Keyboard::Key::Num5;
        case '6':           return sf::Keyboard::Key::Num6;
        case '7':           return sf::Keyboard::Key::Num7;
        case '8':           return sf::Keyboard::Key::Num8;
        case '9':           return sf::Keyboard::Key::Num9;
        case VK_ESCAPE:     return sf::Keyboard::Key::Escape;
        case VK_LCONTROL:   return sf::Keyboard::Key::LControl;
        case VK_LSHIFT:     return sf::Keyboard::Key::LShift;
        case VK_LMENU:      return sf::Keyboard::Key::LAlt;
        case VK_LWIN:       return sf::Keyboard::Key::LSystem;
        case VK_RCONTROL:   return sf::Keyboard::Key::RControl;
        case VK_RSHIFT:     return sf::Keyboard::Key::RShift;
        case VK_RMENU:      return sf::Keyboard::Key::RAlt;
        case VK_RWIN:       return sf::Keyboard::Key::RSystem;
        case VK_APPS:       return sf::Keyboard::Key::Menu;
        case VK_OEM_4:      return sf::Keyboard::Key::LBracket;
        case VK_OEM_6:      return sf::Keyboard::Key::RBracket;
        case VK_OEM_1:      return sf::Keyboard::Key::Semicolon;
        case VK_OEM_COMMA:  return sf::Keyboard::Key::Comma;
        case VK_OEM_PERIOD: return sf::Keyboard::Key::Period;
        case VK_OEM_7:      return sf::Keyboard::Key::Apostrophe;
        case VK_OEM_2:      return sf::Keyboard::Key::Slash;
        case VK_OEM_5:      return sf::Keyboard::Key::Backslash;
        case VK_OEM_3:      return sf::Keyboard::Key::Grave;
        case VK_OEM_PLUS:   return sf::Keyboard::Key::Equal;
        case VK_OEM_MINUS:  return sf::Keyboard::Key::Hyphen;
        case VK_SPACE:      return sf::Keyboard::Key::Space;
        case VK_RETURN:     return sf::Keyboard::Key::Enter;
        case VK_BACK:       return sf::Keyboard::Key::Backspace;
        case VK_TAB:        return sf::Keyboard::Key::Tab;
        case VK_PRIOR:      return sf::Keyboard::Key::PageUp;
        case VK_NEXT:       return sf::Keyboard::Key::PageDown;
        case VK_END:        return sf::Keyboard::Key::End;
        case VK_HOME:       return sf::Keyboard::Key::Home;
        case VK_INSERT:     return sf::Keyboard::Key::Insert;
        case VK_DELETE:     return sf::Keyboard::Key::Delete;
        case VK_ADD:        return sf::Keyboard::Key::Add;
        case VK_SUBTRACT:   return sf::Keyboard::Key::Subtract;
        case VK_MULTIPLY:   return sf::Keyboard::Key::Multiply;
        case VK_DIVIDE:     return sf::Keyboard::Key::Divide;
        case VK_LEFT:       return sf::Keyboard::Key::Left;
        case VK_RIGHT:      return sf::Keyboard::Key::Right;
        case VK_UP:         return sf::Keyboard::Key::Up;
        case VK_DOWN:       return sf::Keyboard::Key::Down;
        case VK_NUMPAD0:    return sf::Keyboard::Key::Numpad0;
        case VK_NUMPAD1:    return sf::Keyboard::Key::Numpad1;
        case VK_NUMPAD2:    return sf::Keyboard::Key::Numpad2;
        case VK_NUMPAD3:    return sf::Keyboard::Key::Numpad3;
        case VK_NUMPAD4:    return sf::Keyboard::Key::Numpad4;
        case VK_NUMPAD5:    return sf::Keyboard::Key::Numpad5;
        case VK_NUMPAD6:    return sf::Keyboard::Key::Numpad6;
        case VK_NUMPAD7:    return sf::Keyboard::Key::Numpad7;
        case VK_NUMPAD8:    return sf::Keyboard::Key::Numpad8;
        case VK_NUMPAD9:    return sf::Keyboard::Key::Numpad9;
        case VK_F1:         return sf::Keyboard::Key::F1;
        case VK_F2:         return sf::Keyboard::Key::F2;
        case VK_F3:         return sf::Keyboard::Key::F3;
        case VK_F4:         return sf::Keyboard::Key::F4;
        case VK_F5:         return sf::Keyboard::Key::F5;
        case VK_F6:         return sf::Keyboard::Key::F6;
        case VK_F7:         return sf::Keyboard::Key::F7;
        case VK_F8:         return sf::Keyboard::Key::F8;
        case VK_F9:         return sf::Keyboard::Key::F9;
        case VK_F10:        return sf::Keyboard::Key::F10;
        case VK_F11:        return sf::Keyboard::Key::F11;
        case VK_F12:        return sf::Keyboard::Key::F12;
        case VK_F13:        return sf::Keyboard::Key::F13;
        case VK_F14:        return sf::Keyboard::Key::F14;
        case VK_F15:        return sf::Keyboard::Key::F15;
        case VK_PAUSE:      return sf::Keyboard::Key::Pause;
        default:            return sf::Keyboard::Key::Unknown;
    }
    // clang-format on
}

int sfKeyToVirtualKey(sf::Keyboard::Key key)
{
    // clang-format off
    switch (key)
    {
        case sf::Keyboard::Key::A:          return 'A';
        case sf::Keyboard::Key::B:          return 'B';
        case sf::Keyboard::Key::C:          return 'C';
        case sf::Keyboard::Key::D:          return 'D';
        case sf::Keyboard::Key::E:          return 'E';
        case sf::Keyboard::Key::F:          return 'F';
        case sf::Keyboard::Key::G:          return 'G';
        case sf::Keyboard::Key::H:          return 'H';
        case sf::Keyboard::Key::I:          return 'I';
        case sf::Keyboard::Key::J:          return 'J';
        case sf::Keyboard::Key::K:          return 'K';
        case sf::Keyboard::Key::L:          return 'L';
        case sf::Keyboard::Key::M:          return 'M';
        case sf::Keyboard::Key::N:          return 'N';
        case sf::Keyboard::Key::O:          return 'O';
        case sf::Keyboard::Key::P:          return 'P';
        case sf::Keyboard::Key::Q:          return 'Q';
        case sf::Keyboard::Key::R:          return 'R';
        case sf::Keyboard::Key::S:          return 'S';
        case sf::Keyboard::Key::T:          return 'T';
        case sf::Keyboard::Key::U:          return 'U';
        case sf::Keyboard::Key::V:          return 'V';
        case sf::Keyboard::Key::W:          return 'W';
        case sf::Keyboard::Key::X:          return 'X';
        case sf::Keyboard::Key::Y:          return 'Y';
        case sf::Keyboard::Key::Z:          return 'Z';
        case sf::Keyboard::Key::Num0:       return '0';
        case sf::Keyboard::Key::Num1:       return '1';
        case sf::Keyboard::Key::Num2:       return '2';
        case sf::Keyboard::Key::Num3:       return '3';
        case sf::Keyboard::Key::Num4:       return '4';
        case sf::Keyboard::Key::Num5:       return '5';
        case sf::Keyboard::Key::Num6:       return '6';
        case sf::Keyboard::Key::Num7:       return '7';
        case sf::Keyboard::Key::Num8:       return '8';
        case sf::Keyboard::Key::Num9:       return '9';
        case sf::Keyboard::Key::Escape:     return VK_ESCAPE;
        case sf::Keyboard::Key::LControl:   return VK_LCONTROL;
        case sf::Keyboard::Key::LShift:     return VK_LSHIFT;
        case sf::Keyboard::Key::LAlt:       return VK_LMENU;
        case sf::Keyboard::Key::LSystem:    return VK_LWIN;
        case sf::Keyboard::Key::RControl:   return VK_RCONTROL;
        case sf::Keyboard::Key::RShift:     return VK_RSHIFT;
        case sf::Keyboard::Key::RAlt:       return VK_RMENU;
        case sf::Keyboard::Key::RSystem:    return VK_RWIN;
        case sf::Keyboard::Key::Menu:       return VK_APPS;
        case sf::Keyboard::Key::LBracket:   return VK_OEM_4;
        case sf::Keyboard::Key::RBracket:   return VK_OEM_6;
        case sf::Keyboard::Key::Semicolon:  return VK_OEM_1;
        case sf::Keyboard::Key::Comma:      return VK_OEM_COMMA;
        case sf::Keyboard::Key::Period:     return VK_OEM_PERIOD;
        case sf::Keyboard::Key::Apostrophe: return VK_OEM_7;
        case sf::Keyboard::Key::Slash:      return VK_OEM_2;
        case sf::Keyboard::Key::Backslash:  return VK_OEM_5;
        case sf::Keyboard::Key::Grave:      return VK_OEM_3;
        case sf::Keyboard::Key::Equal:      return VK_OEM_PLUS;
        case sf::Keyboard::Key::Hyphen:     return VK_OEM_MINUS;
        case sf::Keyboard::Key::Space:      return VK_SPACE;
        case sf::Keyboard::Key::Enter:      return VK_RETURN;
        case sf::Keyboard::Key::Backspace:  return VK_BACK;
        case sf::Keyboard::Key::Tab:        return VK_TAB;
        case sf::Keyboard::Key::PageUp:     return VK_PRIOR;
        case sf::Keyboard::Key::PageDown:   return VK_NEXT;
        case sf::Keyboard::Key::End:        return VK_END;
        case sf::Keyboard::Key::Home:       return VK_HOME;
        case sf::Keyboard::Key::Insert:     return VK_INSERT;
        case sf::Keyboard::Key::Delete:     return VK_DELETE;
        case sf::Keyboard::Key::Add:        return VK_ADD;
        case sf::Keyboard::Key::Subtract:   return VK_SUBTRACT;
        case sf::Keyboard::Key::Multiply:   return VK_MULTIPLY;
        case sf::Keyboard::Key::Divide:     return VK_DIVIDE;
        case sf::Keyboard::Key::Left:       return VK_LEFT;
        case sf::Keyboard::Key::Right:      return VK_RIGHT;
        case sf::Keyboard::Key::Up:         return VK_UP;
        case sf::Keyboard::Key::Down:       return VK_DOWN;
        case sf::Keyboard::Key::Numpad0:    return VK_NUMPAD0;
        case sf::Keyboard::Key::Numpad1:    return VK_NUMPAD1;
        case sf::Keyboard::Key::Numpad2:    return VK_NUMPAD2;
        case sf::Keyboard::Key::Numpad3:    return VK_NUMPAD3;
        case sf::Keyboard::Key::Numpad4:    return VK_NUMPAD4;
        case sf::Keyboard::Key::Numpad5:    return VK_NUMPAD5;
        case sf::Keyboard::Key::Numpad6:    return VK_NUMPAD6;
        case sf::Keyboard::Key::Numpad7:    return VK_NUMPAD7;
        case sf::Keyboard::Key::Numpad8:    return VK_NUMPAD8;
        case sf::Keyboard::Key::Numpad9:    return VK_NUMPAD9;
        case sf::Keyboard::Key::F1:         return VK_F1;
        case sf::Keyboard::Key::F2:         return VK_F2;
        case sf::Keyboard::Key::F3:         return VK_F3;
        case sf::Keyboard::Key::F4:         return VK_F4;
        case sf::Keyboard::Key::F5:         return VK_F5;
        case sf::Keyboard::Key::F6:         return VK_F6;
        case sf::Keyboard::Key::F7:         return VK_F7;
        case sf::Keyboard::Key::F8:         return VK_F8;
        case sf::Keyboard::Key::F9:         return VK_F9;
        case sf::Keyboard::Key::F10:        return VK_F10;
        case sf::Keyboard::Key::F11:        return VK_F11;
        case sf::Keyboard::Key::F12:        return VK_F12;
        case sf::Keyboard::Key::F13:        return VK_F13;
        case sf::Keyboard::Key::F14:        return VK_F14;
        case sf::Keyboard::Key::F15:        return VK_F15;
        case sf::Keyboard::Key::Pause:      return VK_PAUSE;
        default:                            return 0;
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

std::optional<sf::String> sfScanToConsumerKeyName(sf::Keyboard::Scancode code)
{
    // Convert an SFML scancode to a Windows consumer keyboard key name
    // Reference: https://learn.microsoft.com/en-us/windows/win32/inputdev/about-keyboard-input#keystroke-messages
    // clang-format off
    switch (code)
    {
        case sf::Keyboard::Scan::MediaNextTrack:     return "Next Track";
        case sf::Keyboard::Scan::MediaPreviousTrack: return "Previous Track";
        case sf::Keyboard::Scan::MediaStop:          return "Stop";
        case sf::Keyboard::Scan::MediaPlayPause:     return "Play/Pause";
        case sf::Keyboard::Scan::VolumeMute:         return "Mute";
        case sf::Keyboard::Scan::VolumeUp:           return "Volume Increment";
        case sf::Keyboard::Scan::VolumeDown:         return "Volume Decrement";
        case sf::Keyboard::Scan::LaunchMediaSelect:  return "Consumer Control Configuration";
        case sf::Keyboard::Scan::LaunchMail:         return "Email Reader";
        case sf::Keyboard::Scan::LaunchApplication2: return "Calculator";
        case sf::Keyboard::Scan::LaunchApplication1: return "Local Machine Browser";
        case sf::Keyboard::Scan::Search:             return "Search";
        case sf::Keyboard::Scan::HomePage:           return "Home";
        case sf::Keyboard::Scan::Back:               return "Back";
        case sf::Keyboard::Scan::Forward:            return "Forward";
        case sf::Keyboard::Scan::Stop:               return "Stop";
        case sf::Keyboard::Scan::Refresh:            return "Refresh";
        case sf::Keyboard::Scan::Favorites:          return "Bookmarks";

        // Not a consumer key
        default: return std::nullopt;
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
    keyToScancodeMapping.fill(sf::Keyboard::Scan::Unknown);
    scancodeToKeyMapping.fill(sf::Keyboard::Key::Unknown);

    // Phase 2: Translate scancode to virtual code to key names
    for (unsigned int i = 0; i < sf::Keyboard::ScancodeCount; ++i)
    {
        const auto              scan       = static_cast<sf::Keyboard::Scancode>(i);
        const UINT              virtualKey = sfScanToVirtualKey(scan);
        const sf::Keyboard::Key key        = virtualKeyToSfKey(virtualKey);
        if (key != sf::Keyboard::Key::Unknown && keyToScancodeMapping[key] == sf::Keyboard::Scan::Unknown)
            keyToScancodeMapping[key] = scan;
        scancodeToKeyMapping[scan] = key;
    }

    isMappingInitialized = true;
}

bool isValidScancode(sf::Keyboard::Scancode code)
{
    return code > sf::Keyboard::Scan::Unknown && static_cast<unsigned int>(code) < sf::Keyboard::ScancodeCount;
}

bool isValidKey(sf::Keyboard::Key key)
{
    return key > sf::Keyboard::Key::Unknown && static_cast<unsigned int>(key) < sf::Keyboard::KeyCount;
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
        return Keyboard::Key::Unknown;

    ensureMappings();

    return scancodeToKeyMapping[code];
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
    // Try to translate the scan code to a consumer key
    if (const auto consumerKeyName = sfScanToConsumerKeyName(code))
        return *consumerKeyName;

    WORD                    winCode = sfScanToWinScanExtended(code);
    std::array<WCHAR, 1024> name{};

    // Remap F13-F23 to values supported by GetKeyNameText
    if ((winCode >= 0x64) && (winCode <= 0x6E))
        winCode += 0x18;
    // Remap F24 to value supported by GetKeyNameText
    if (winCode == 0x76)
        winCode = 0x87;

    if (GetKeyNameText(winCode << 16, name.data(), static_cast<int>(name.size())) > 0)
        return name.data();

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
        case Mouse::Button::Left:
            virtualKey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON;
            break;
        case Mouse::Button::Right:
            virtualKey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_LBUTTON : VK_RBUTTON;
            break;
        case Mouse::Button::Middle:
            virtualKey = VK_MBUTTON;
            break;
        case Mouse::Button::Extra1:
            virtualKey = VK_XBUTTON1;
            break;
        case Mouse::Button::Extra2:
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
    if (const WindowHandle handle = relativeTo.getNativeHandle())
    {
        POINT point;
        GetCursorPos(&point);
        ScreenToClient(handle, &point);
        return {point.x, point.y};
    }

    return {};
}


////////////////////////////////////////////////////////////
void setMousePosition(Vector2i position)
{
    SetCursorPos(position.x, position.y);
}


////////////////////////////////////////////////////////////
void setMousePosition(Vector2i position, const WindowBase& relativeTo)
{
    if (const WindowHandle handle = relativeTo.getNativeHandle())
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
