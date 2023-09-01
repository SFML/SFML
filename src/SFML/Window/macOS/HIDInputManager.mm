////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Marco Antognini (antognini.marco@gmail.com),
//                         Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/macOS/HIDInputManager.hpp>

#include <SFML/System/Err.hpp>

#include <AppKit/AppKit.h>
#include <ostream>

namespace
{
const std::uint8_t unknownVirtualCode = 0xff;
const bool         isIsoKeyboard      = (KBGetLayoutType(LMGetKbdType()) == kKeyboardISO);
}

namespace sf::priv
{
////////////////////////////////////////////////////////////
HIDInputManager& HIDInputManager::getInstance()
{
    static HIDInputManager instance;
    return instance;
}


////////////////////////////////////////////////////////////
long HIDInputManager::getLocationID(IOHIDDeviceRef device)
{
    long loc = 0;

    // Get a unique ID: its USB location ID
    CFTypeRef typeRef = IOHIDDeviceGetProperty(device, CFSTR(kIOHIDLocationIDKey));
    if (!typeRef || (CFGetTypeID(typeRef) != CFNumberGetTypeID()))
        return 0;

    const auto* locRef = static_cast<CFNumberRef>(typeRef);

    if (!CFNumberGetValue(locRef, kCFNumberLongType, &loc))
        return 0;

    return loc;
}


////////////////////////////////////////////////////////////
CFDictionaryRef HIDInputManager::copyDevicesMask(std::uint32_t page, std::uint32_t usage)
{
    // Create the dictionary.
    CFMutableDictionaryRef dict = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                            2,
                                                            &kCFTypeDictionaryKeyCallBacks,
                                                            &kCFTypeDictionaryValueCallBacks);

    // Add the page value.
    CFNumberRef value = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &page);
    CFDictionarySetValue(dict, CFSTR(kIOHIDDeviceUsagePageKey), value);
    CFRelease(value);

    // Add the usage value (which is only valid if page value exists).
    value = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usage);
    CFDictionarySetValue(dict, CFSTR(kIOHIDDeviceUsageKey), value);
    CFRelease(value);

    return dict;
}


////////////////////////////////////////////////////////
Keyboard::Key HIDInputManager::localizedKey(UniChar ch)
{
    // Apple is using the private range 0xE000 - 0xF8FF for special keys.
    // Otherwise one can refer to the usual Unicode table.
    // clang-format off
    switch (ch)
    {
        case 0x41: return Keyboard::A; // uppercase A
        case 0x42: return Keyboard::B;
        case 0x43: return Keyboard::C;
        case 0x44: return Keyboard::D;
        case 0x45: return Keyboard::E;
        case 0x46: return Keyboard::F;
        case 0x47: return Keyboard::G;
        case 0x48: return Keyboard::H;
        case 0x49: return Keyboard::I;
        case 0x4a: return Keyboard::J;
        case 0x4b: return Keyboard::K;
        case 0x4c: return Keyboard::L;
        case 0x4d: return Keyboard::M;
        case 0x4e: return Keyboard::N;
        case 0x4f: return Keyboard::O;
        case 0x50: return Keyboard::P;
        case 0x51: return Keyboard::Q;
        case 0x52: return Keyboard::R;
        case 0x53: return Keyboard::S;
        case 0x54: return Keyboard::T;
        case 0x55: return Keyboard::U;
        case 0x56: return Keyboard::V;
        case 0x57: return Keyboard::W;
        case 0x58: return Keyboard::X;
        case 0x59: return Keyboard::Y;
        case 0x5a: return Keyboard::Z;

        case 0x61: return Keyboard::A; // lowercase A
        case 0x62: return Keyboard::B;
        case 0x63: return Keyboard::C;
        case 0x64: return Keyboard::D;
        case 0x65: return Keyboard::E;
        case 0x66: return Keyboard::F;
        case 0x67: return Keyboard::G;
        case 0x68: return Keyboard::H;
        case 0x69: return Keyboard::I;
        case 0x6a: return Keyboard::J;
        case 0x6b: return Keyboard::K;
        case 0x6c: return Keyboard::L;
        case 0x6d: return Keyboard::M;
        case 0x6e: return Keyboard::N;
        case 0x6f: return Keyboard::O;
        case 0x70: return Keyboard::P;
        case 0x71: return Keyboard::Q;
        case 0x72: return Keyboard::R;
        case 0x73: return Keyboard::S;
        case 0x74: return Keyboard::T;
        case 0x75: return Keyboard::U;
        case 0x76: return Keyboard::V;
        case 0x77: return Keyboard::W;
        case 0x78: return Keyboard::X;
        case 0x79: return Keyboard::Y;
        case 0x7a: return Keyboard::Z;

        case 0x30: return Keyboard::Num0;
        case 0x31: return Keyboard::Num1;
        case 0x32: return Keyboard::Num2;
        case 0x33: return Keyboard::Num3;
        case 0x34: return Keyboard::Num4;
        case 0x35: return Keyboard::Num5;
        case 0x36: return Keyboard::Num6;
        case 0x37: return Keyboard::Num7;
        case 0x38: return Keyboard::Num8;
        case 0x39: return Keyboard::Num9;

        case 0x1b: return Keyboard::Escape;

        /* Those are not represented using Unicode.
         * case 0x: return Keyboard::LControl;
         * case 0x: return Keyboard::LShift;
         * case 0x: return Keyboard::LAlt;
         * case 0x: return Keyboard::LSystem;
         * case 0x: return Keyboard::RControl;
         * case 0x: return Keyboard::RShift;
         * case 0x: return Keyboard::RAlt;
         * case 0x: return Keyboard::RSystem;
         */

        case NSMenuFunctionKey: return Keyboard::Menu;

        case 0x5b: return Keyboard::LBracket;
        case 0x5d: return Keyboard::RBracket;
        case 0x3b: return Keyboard::Semicolon;
        case 0x2c: return Keyboard::Comma;
        case 0x2e: return Keyboard::Period;
        case 0x27: return Keyboard::Apostrophe;
        case 0x2f: return Keyboard::Slash;
        case 0x5c: return Keyboard::Backslash;
        case 0x60: return Keyboard::Grave;
        case 0x3d: return Keyboard::Equal;
        case 0x2d: return Keyboard::Hyphen;
        case 0x20: return Keyboard::Space;
        case 0x0d: return Keyboard::Enter;
        case 0x08: return Keyboard::Backspace;
        case 0x09: return Keyboard::Tab;

        case NSPageUpFunctionKey:   return Keyboard::PageUp;
        case NSPageDownFunctionKey: return Keyboard::PageDown;
        case NSEndFunctionKey:      return Keyboard::End;
        case NSHomeFunctionKey:     return Keyboard::Home;
        case NSInsertFunctionKey:   return Keyboard::Insert;
        case NSDeleteFunctionKey:   return Keyboard::Delete;
        case 0x7f:                  return Keyboard::Delete;

        case 0x2b: return Keyboard::Add;
        // case 0x: return Keyboard::Subtract; // collision with Keyboard::Hyphen
        case 0x2a: return Keyboard::Multiply;
        // case 0x: return Keyboard::Divide; // collision with Keyboard::Slash

        case NSLeftArrowFunctionKey:  return Keyboard::Left;
        case NSRightArrowFunctionKey: return Keyboard::Right;
        case NSUpArrowFunctionKey:    return Keyboard::Up;
        case NSDownArrowFunctionKey:  return Keyboard::Down;

        /* Those are not represented using Unicode.
         * case 0x: return Keyboard::Numpad0;
         * case 0x: return Keyboard::Numpad1;
         * case 0x: return Keyboard::Numpad2;
         * case 0x: return Keyboard::Numpad3;
         * case 0x: return Keyboard::Numpad4;
         * case 0x: return Keyboard::Numpad5;
         * case 0x: return Keyboard::Numpad6;
         * case 0x: return Keyboard::Numpad7;
         * case 0x: return Keyboard::Numpad8;
         * case 0x: return Keyboard::Numpad9;
         */

        case NSF1FunctionKey:  return Keyboard::F1;
        case NSF2FunctionKey:  return Keyboard::F2;
        case NSF3FunctionKey:  return Keyboard::F3;
        case NSF4FunctionKey:  return Keyboard::F4;
        case NSF5FunctionKey:  return Keyboard::F5;
        case NSF6FunctionKey:  return Keyboard::F6;
        case NSF7FunctionKey:  return Keyboard::F7;
        case NSF8FunctionKey:  return Keyboard::F8;
        case NSF9FunctionKey:  return Keyboard::F9;
        case NSF10FunctionKey: return Keyboard::F10;
        case NSF11FunctionKey: return Keyboard::F11;
        case NSF12FunctionKey: return Keyboard::F12;
        case NSF13FunctionKey: return Keyboard::F13;
        case NSF14FunctionKey: return Keyboard::F14;
        case NSF15FunctionKey: return Keyboard::F15;

        case NSPauseFunctionKey:  return Keyboard::Pause;

        default:  return Keyboard::Unknown;
    }
    // clang-format on
}


////////////////////////////////////////////////////////
UniChar HIDInputManager::toUnicode(Keyboard::Key key)
{
    // clang-format off
    switch (key)
    {
        case Keyboard::A: return 0x41;
        case Keyboard::B: return 0x42;
        case Keyboard::C: return 0x43;
        case Keyboard::D: return 0x44;
        case Keyboard::E: return 0x45;
        case Keyboard::F: return 0x46;
        case Keyboard::G: return 0x47;
        case Keyboard::H: return 0x48;
        case Keyboard::I: return 0x49;
        case Keyboard::J: return 0x4a;
        case Keyboard::K: return 0x4b;
        case Keyboard::L: return 0x4c;
        case Keyboard::M: return 0x4d;
        case Keyboard::N: return 0x4e;
        case Keyboard::O: return 0x4f;
        case Keyboard::P: return 0x50;
        case Keyboard::Q: return 0x51;
        case Keyboard::R: return 0x52;
        case Keyboard::S: return 0x53;
        case Keyboard::T: return 0x54;
        case Keyboard::U: return 0x55;
        case Keyboard::V: return 0x56;
        case Keyboard::W: return 0x57;
        case Keyboard::X: return 0x58;
        case Keyboard::Y: return 0x59;
        case Keyboard::Z: return 0x5a;

        case Keyboard::Num0: return 0x30;
        case Keyboard::Num1: return 0x31;
        case Keyboard::Num2: return 0x32;
        case Keyboard::Num3: return 0x33;
        case Keyboard::Num4: return 0x34;
        case Keyboard::Num5: return 0x35;
        case Keyboard::Num6: return 0x36;
        case Keyboard::Num7: return 0x37;
        case Keyboard::Num8: return 0x38;
        case Keyboard::Num9: return 0x39;

        case Keyboard::Escape:  return 0x1b;

        /* Not representable as Unicode:
         * case Keyboard::LControl: return 0x;
         * case Keyboard::LShift:   return 0x;
         * case Keyboard::LAlt:     return 0x;
         * case Keyboard::LSystem:  return 0x;
         * case Keyboard::RControl: return 0x;
         * case Keyboard::RShift:   return 0x;
         * case Keyboard::RAlt:     return 0x;
         * case Keyboard::RSystem:  return 0x;
         */

        case Keyboard::Menu:  return NSMenuFunctionKey;

        case Keyboard::LBracket:   return 0x5b;
        case Keyboard::RBracket:   return 0x5d;
        case Keyboard::Semicolon:  return 0x3b;
        case Keyboard::Comma:      return 0x2c;
        case Keyboard::Period:     return 0x2e;
        case Keyboard::Apostrophe: return 0x27;
        case Keyboard::Slash:      return 0x2f;
        case Keyboard::Backslash:  return 0x5c;
        case Keyboard::Grave:      return 0x60;
        case Keyboard::Equal:      return 0x3d;
        case Keyboard::Hyphen:     return 0x2d;
        case Keyboard::Space:      return 0x20;
        case Keyboard::Enter:      return 0x0d;
        case Keyboard::Backspace:  return 0x08;
        case Keyboard::Tab:        return 0x09;

        case Keyboard::PageUp:   return NSPageUpFunctionKey;
        case Keyboard::PageDown: return NSPageDownFunctionKey;
        case Keyboard::End:      return NSEndFunctionKey;
        case Keyboard::Home:     return NSHomeFunctionKey;
        case Keyboard::Insert:   return NSInsertFunctionKey;
        case Keyboard::Delete:   return NSDeleteFunctionKey;

        case Keyboard::Add:      return 0x2b;
        case Keyboard::Subtract: return 0x2d;
        case Keyboard::Multiply: return 0x2a;
        case Keyboard::Divide:   return 0x2f;

        case Keyboard::Left:  return NSLeftArrowFunctionKey;
        case Keyboard::Right: return NSRightArrowFunctionKey;
        case Keyboard::Up:    return NSUpArrowFunctionKey;
        case Keyboard::Down:  return NSDownArrowFunctionKey;

        /* Those are not represented using Unicode.
         * case Keyboard::Numpad0: return 0x;
         * case Keyboard::Numpad1: return 0x;
         * case Keyboard::Numpad2: return 0x;
         * case Keyboard::Numpad3: return 0x;
         * case Keyboard::Numpad4: return 0x;
         * case Keyboard::Numpad5: return 0x;
         * case Keyboard::Numpad6: return 0x;
         * case Keyboard::Numpad7: return 0x;
         * case Keyboard::Numpad8: return 0x;
         * case Keyboard::Numpad9: return 0x;
         */

        case Keyboard::F1:  return NSF1FunctionKey;
        case Keyboard::F2:  return NSF2FunctionKey;
        case Keyboard::F3:  return NSF3FunctionKey;
        case Keyboard::F4:  return NSF4FunctionKey;
        case Keyboard::F5:  return NSF5FunctionKey;
        case Keyboard::F6:  return NSF6FunctionKey;
        case Keyboard::F7:  return NSF7FunctionKey;
        case Keyboard::F8:  return NSF8FunctionKey;
        case Keyboard::F9:  return NSF9FunctionKey;
        case Keyboard::F10: return NSF10FunctionKey;
        case Keyboard::F11: return NSF11FunctionKey;
        case Keyboard::F12: return NSF12FunctionKey;
        case Keyboard::F13: return NSF13FunctionKey;
        case Keyboard::F14: return NSF14FunctionKey;
        case Keyboard::F15: return NSF15FunctionKey;

        case Keyboard::Pause: return NSPauseFunctionKey;

        default: return 0x00;
    }
    // clang-format on
}


////////////////////////////////////////////////////////
Keyboard::Scancode HIDInputManager::nonLocalizedKey(UniChar virtualKeycode)
{
    // See Chapter 2, esp. Figure 2-10 of
    // https://developer.apple.com/legacy/library/documentation/mac/pdf/MacintoshToolboxEssentials.pdf
    // Additional virtual codes come from
    // /System/Library/Frameworks/Carbon.framework/Versions/A/Frameworks/HIToolbox.framework/Versions/A/Headers/Events.h
    // clang-format off
    switch (virtualKeycode)
    {
        case 0x00: return Keyboard::Scan::A;
        case 0x0b: return Keyboard::Scan::B;
        case 0x08: return Keyboard::Scan::C;
        case 0x02: return Keyboard::Scan::D;
        case 0x0e: return Keyboard::Scan::E;
        case 0x03: return Keyboard::Scan::F;
        case 0x05: return Keyboard::Scan::G;
        case 0x04: return Keyboard::Scan::H;
        case 0x22: return Keyboard::Scan::I;
        case 0x26: return Keyboard::Scan::J;
        case 0x28: return Keyboard::Scan::K;
        case 0x25: return Keyboard::Scan::L;
        case 0x2e: return Keyboard::Scan::M;
        case 0x2d: return Keyboard::Scan::N;
        case 0x1f: return Keyboard::Scan::O;
        case 0x23: return Keyboard::Scan::P;
        case 0x0c: return Keyboard::Scan::Q;
        case 0x0f: return Keyboard::Scan::R;
        case 0x01: return Keyboard::Scan::S;
        case 0x11: return Keyboard::Scan::T;
        case 0x20: return Keyboard::Scan::U;
        case 0x09: return Keyboard::Scan::V;
        case 0x0d: return Keyboard::Scan::W;
        case 0x07: return Keyboard::Scan::X;
        case 0x10: return Keyboard::Scan::Y;
        case 0x06: return Keyboard::Scan::Z;

        case 0x12: return Keyboard::Scan::Num1;
        case 0x13: return Keyboard::Scan::Num2;
        case 0x14: return Keyboard::Scan::Num3;
        case 0x15: return Keyboard::Scan::Num4;
        case 0x17: return Keyboard::Scan::Num5;
        case 0x16: return Keyboard::Scan::Num6;
        case 0x1a: return Keyboard::Scan::Num7;
        case 0x1c: return Keyboard::Scan::Num8;
        case 0x19: return Keyboard::Scan::Num9;
        case 0x1d: return Keyboard::Scan::Num0;

        case 0x24: return Keyboard::Scan::Enter;
        case 0x35: return Keyboard::Scan::Escape;
        case 0x33: return Keyboard::Scan::Backspace;
        case 0x30: return Keyboard::Scan::Tab;
        case 0x31: return Keyboard::Scan::Space;

        case 0x18: return Keyboard::Scan::Equal;
        case 0x21: return Keyboard::Scan::LBracket;
        case 0x1e: return Keyboard::Scan::RBracket;
        case 0x2a: return Keyboard::Scan::Backslash;
        case 0x1b: return Keyboard::Scan::Hyphen;
        case 0x29: return Keyboard::Scan::Semicolon;
        case 0x27: return Keyboard::Scan::Apostrophe;
        case 0x32: return isIsoKeyboard ? Keyboard::Scan::NonUsBackslash : Keyboard::Scan::Grave;
        case 0x0a: return Keyboard::Scan::Grave;
        case 0x2b: return Keyboard::Scan::Comma;
        case 0x2f: return Keyboard::Scan::Period;
        case 0x2c: return Keyboard::Scan::Slash;

        case 0x7a: return Keyboard::Scan::F1;
        case 0x78: return Keyboard::Scan::F2;
        case 0x63: return Keyboard::Scan::F3;
        case 0x76: return Keyboard::Scan::F4;
        case 0x60: return Keyboard::Scan::F5;
        case 0x61: return Keyboard::Scan::F6;
        case 0x62: return Keyboard::Scan::F7;
        case 0x64: return Keyboard::Scan::F8;
        case 0x65: return Keyboard::Scan::F9;
        case 0x6d: return Keyboard::Scan::F10;
        case 0x67: return Keyboard::Scan::F11;
        case 0x6f: return Keyboard::Scan::F12;
        case 0x69: return Keyboard::Scan::F13;
        case 0x6b: return Keyboard::Scan::F14;
        case 0x71: return Keyboard::Scan::F15;

        case 0x6A: return Keyboard::Scan::F16;
        case 0x40: return Keyboard::Scan::F17;
        case 0x4F: return Keyboard::Scan::F18;
        case 0x50: return Keyboard::Scan::F19;
        case 0x5A: return Keyboard::Scan::F20;

        /* TODO Those are missing:
         * case 0x: return Keyboard::Scan::F21;
         * case 0x: return Keyboard::Scan::F22;
         * case 0x: return Keyboard::Scan::F23;
         * case 0x: return Keyboard::Scan::F24;
         */

        case 0x39: return Keyboard::Scan::CapsLock;

        /* TODO Those are missing:
         * case 0x: return Keyboard::Scan::PrintScreen;
         * case 0x: return Keyboard::Scan::ScrollLock;
         * case 0x: return Keyboard::Scan::Pause;
         */

        case 0x72: return Keyboard::Scan::Insert;
        case 0x73: return Keyboard::Scan::Home;
        case 0x74: return Keyboard::Scan::PageUp;
        case 0x75: return Keyboard::Scan::Delete;
        case 0x77: return Keyboard::Scan::End;
        case 0x79: return Keyboard::Scan::PageDown;

        case 0x7c: return Keyboard::Scan::Right;
        case 0x7b: return Keyboard::Scan::Left;
        case 0x7d: return Keyboard::Scan::Down;
        case 0x7e: return Keyboard::Scan::Up;

        case 0x47: return Keyboard::Scan::NumLock;
        case 0x4b: return Keyboard::Scan::NumpadDivide;
        case 0x43: return Keyboard::Scan::NumpadMultiply;
        case 0x4e: return Keyboard::Scan::NumpadMinus;
        case 0x45: return Keyboard::Scan::NumpadPlus;
        case 0x51: return Keyboard::Scan::NumpadEqual;
        case 0x4c: return Keyboard::Scan::NumpadEnter;
        case 0x41: return Keyboard::Scan::NumpadDecimal;

        case 0x53: return Keyboard::Scan::Numpad1;
        case 0x54: return Keyboard::Scan::Numpad2;
        case 0x55: return Keyboard::Scan::Numpad3;
        case 0x56: return Keyboard::Scan::Numpad4;
        case 0x57: return Keyboard::Scan::Numpad5;
        case 0x58: return Keyboard::Scan::Numpad6;
        case 0x59: return Keyboard::Scan::Numpad7;
        case 0x5b: return Keyboard::Scan::Numpad8;
        case 0x5c: return Keyboard::Scan::Numpad9;
        case 0x52: return Keyboard::Scan::Numpad0;

        /* TODO Those are missing:
         * case 0x: return Keyboard::Scan::Application;
         * case 0x: return Keyboard::Scan::Execute;
         * case 0x: return Keyboard::Scan::Help;
         * case 0x: return Keyboard::Scan::Menu;
         * case 0x: return Keyboard::Scan::Select;
         * case 0x: return Keyboard::Scan::Stop;
         * case 0x: return Keyboard::Scan::Redo;
         * case 0x: return Keyboard::Scan::Undo;
         * case 0x: return Keyboard::Scan::Cut;
         * case 0x: return Keyboard::Scan::Copy;
         * case 0x: return Keyboard::Scan::Paste;
         * case 0x: return Keyboard::Scan::Search;
         */

        case 0x4a: return Keyboard::Scan::VolumeMute;
        case 0x48: return Keyboard::Scan::VolumeUp;
        case 0x49: return Keyboard::Scan::VolumeDown;

        /* NOTE Those are symmetric so we leave them out.
         *      Thankfully handled through modifiers and not virtual codes.
         * case 0x3b: return Keyboard::Scan::LControl;
         * case 0x38: return Keyboard::Scan::LShift;
         * case 0x3a: return Keyboard::Scan::LAlt;
         * case 0x37: return Keyboard::Scan::LSystem;
         * case 0x3b: return Keyboard::Scan::RControl;
         * case 0x38: return Keyboard::Scan::RShift;
         * case 0x3a: return Keyboard::Scan::RAlt;
         * case 0x37: return Keyboard::Scan::RSystem;
         */

        default: return Keyboard::Scan::Unknown;
    }
    // clang-format on
}


////////////////////////////////////////////////////////////
bool HIDInputManager::isKeyPressed(Keyboard::Key key)
{
    return isKeyPressed(delocalize(key));
}


////////////////////////////////////////////////////////////
bool HIDInputManager::isKeyPressed(Keyboard::Scancode code)
{
    return (code != Keyboard::Scan::Unknown) && isPressed(m_keys[static_cast<std::size_t>(code)]);
}


////////////////////////////////////////////////////////////
Keyboard::Key HIDInputManager::localize(Keyboard::Scancode code)
{
    if (code == Keyboard::Scan::Unknown)
        return Keyboard::Unknown;

    return m_scancodeToKeyMapping[static_cast<std::size_t>(code)];
}


////////////////////////////////////////////////////////////
Keyboard::Scancode HIDInputManager::delocalize(Keyboard::Key key)
{
    if (key == Keyboard::Unknown)
        return Keyboard::Scan::Unknown;

    return m_keyToScancodeMapping[key];
}


////////////////////////////////////////////////////////////
String HIDInputManager::getDescription(Keyboard::Scancode code)
{
    // Phase 1: Get names for layout independent keys
    // clang-format off
    switch (code)
    {
        case Keyboard::Scan::Enter:     return "Enter";
        case Keyboard::Scan::Escape:    return "Escape";
        case Keyboard::Scan::Backspace: return "Backspace";
        case Keyboard::Scan::Tab:       return "Tab";
        case Keyboard::Scan::Space:     return "Space";

        case Keyboard::Scan::F1:  return "F1";
        case Keyboard::Scan::F2:  return "F2";
        case Keyboard::Scan::F3:  return "F3";
        case Keyboard::Scan::F4:  return "F4";
        case Keyboard::Scan::F5:  return "F5";
        case Keyboard::Scan::F6:  return "F6";
        case Keyboard::Scan::F7:  return "F7";
        case Keyboard::Scan::F8:  return "F8";
        case Keyboard::Scan::F9:  return "F9";
        case Keyboard::Scan::F10: return "F10";
        case Keyboard::Scan::F11: return "F11";
        case Keyboard::Scan::F12: return "F12";
        case Keyboard::Scan::F13: return "F13";
        case Keyboard::Scan::F14: return "F14";
        case Keyboard::Scan::F15: return "F15";
        case Keyboard::Scan::F16: return "F16";
        case Keyboard::Scan::F17: return "F17";
        case Keyboard::Scan::F18: return "F18";
        case Keyboard::Scan::F19: return "F19";
        case Keyboard::Scan::F20: return "F20";
        case Keyboard::Scan::F21: return "F21";
        case Keyboard::Scan::F22: return "F22";
        case Keyboard::Scan::F23: return "F23";
        case Keyboard::Scan::F24: return "F24";

        case Keyboard::Scan::CapsLock:    return "Caps Lock";
        case Keyboard::Scan::PrintScreen: return "Print Screen";
        case Keyboard::Scan::ScrollLock:  return "Scroll Lock";

        case Keyboard::Scan::Pause:    return "Pause";
        case Keyboard::Scan::Insert:   return "Insert";
        case Keyboard::Scan::Home:     return "Home";
        case Keyboard::Scan::PageUp:   return "Page Up";
        case Keyboard::Scan::Delete:   return "Delete";
        case Keyboard::Scan::End:      return "End";
        case Keyboard::Scan::PageDown: return "Page Down";

        case Keyboard::Scan::Left:  return "Left Arrow";
        case Keyboard::Scan::Right: return "Right Arrow";
        case Keyboard::Scan::Down:  return "Down Arrow";
        case Keyboard::Scan::Up:    return "Up Arrow";

        case Keyboard::Scan::NumLock:        return "Num Lock";
        case Keyboard::Scan::NumpadDivide:   return "Divide (Numpad)";
        case Keyboard::Scan::NumpadMultiply: return "Multiply (Numpad)";
        case Keyboard::Scan::NumpadMinus:    return "Minus (Numpad)";
        case Keyboard::Scan::NumpadPlus:     return "Plus (Numpad)";
        case Keyboard::Scan::NumpadEqual:    return "Equal (Numpad)";
        case Keyboard::Scan::NumpadEnter:    return "Enter (Numpad)";
        case Keyboard::Scan::NumpadDecimal:  return "Decimal (Numpad)";

        case Keyboard::Scan::Numpad0: return "0 (Numpad)";
        case Keyboard::Scan::Numpad1: return "1 (Numpad)";
        case Keyboard::Scan::Numpad2: return "2 (Numpad)";
        case Keyboard::Scan::Numpad3: return "3 (Numpad)";
        case Keyboard::Scan::Numpad4: return "4 (Numpad)";
        case Keyboard::Scan::Numpad5: return "5 (Numpad)";
        case Keyboard::Scan::Numpad6: return "6 (Numpad)";
        case Keyboard::Scan::Numpad7: return "7 (Numpad)";
        case Keyboard::Scan::Numpad8: return "8 (Numpad)";
        case Keyboard::Scan::Numpad9: return "9 (Numpad)";

        case Keyboard::Scan::Application: return "Application";
        case Keyboard::Scan::Execute:     return "Execute";
        case Keyboard::Scan::Help:        return "Help";
        case Keyboard::Scan::Menu:        return "Menu";
        case Keyboard::Scan::Select:      return "Select";
        case Keyboard::Scan::Stop:        return "Stop";
        case Keyboard::Scan::Redo:        return "Redo";
        case Keyboard::Scan::Undo:        return "Undo";
        case Keyboard::Scan::Cut:         return "Cut";
        case Keyboard::Scan::Copy:        return "Copy";
        case Keyboard::Scan::Paste:       return "Paste";
        case Keyboard::Scan::Search:      return "Search";
        case Keyboard::Scan::VolumeMute:  return "Volume Mute";
        case Keyboard::Scan::VolumeUp:    return "Volume Up";
        case Keyboard::Scan::VolumeDown:  return "Volume Down";

        case Keyboard::Scan::LControl: return "Left Control";
        case Keyboard::Scan::LShift:   return "Left Shift";
        case Keyboard::Scan::LAlt:     return "Left Option";
        case Keyboard::Scan::LSystem:  return "Left Command";
        case Keyboard::Scan::RControl: return "Right Control";
        case Keyboard::Scan::RShift:   return "Right Shift";
        case Keyboard::Scan::RAlt:     return "Right Option";
        case Keyboard::Scan::RSystem:  return "Right Command";

        default:
        {
            // Phase 2: Try to convert the key to unicode
            const UniChar unicode = toUnicode(localize(code));
            if (unicode != 0x00)
                return {static_cast<char32_t>(unicode)};
        }

        // Phase 3: Return final fallback
        return "Unknown";
    }
    // clang-format on
}


////////////////////////////////////////////////////////////
HIDInputManager::HIDInputManager()
{
    // Create an HID Manager reference
    m_manager                 = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    const IOReturn openStatus = IOHIDManagerOpen(m_manager, kIOHIDOptionsTypeNone);

    if (openStatus != kIOReturnSuccess)
    {
        err() << "Error when opening the HID manager" << std::endl;
        freeUp();
        return;
    }

    // Build up our knownledge of the hardware
    initializeKeyboard();
    buildMappings();

    // Register for notification on keyboard layout changes
    CFNotificationCenterAddObserver(CFNotificationCenterGetDistributedCenter(),
                                    this,
                                    keyboardChanged, // callback
                                    kTISNotifySelectedKeyboardInputSourceChanged,
                                    nullptr, // use callback
                                    CFNotificationSuspensionBehaviorDeliverImmediately);
}


////////////////////////////////////////////////////////////
HIDInputManager::~HIDInputManager()
{
    freeUp();

    CFNotificationCenterRemoveEveryObserver(CFNotificationCenterGetDistributedCenter(), this);
}


void HIDInputManager::initializeKeyboard()
{
    ////////////////////////////////////////////////////////////
    // The purpose of this function is to initialize m_keys so we can get
    // the associate IOHIDElementRef with a sf::Keyboard::Scancode
    // in approximately constant time.

    // Get only keyboards
    CFSetRef underlying = copyDevices(kHIDPage_GenericDesktop, kHIDUsage_GD_Keyboard);
    if (underlying == nullptr)
    {
        err() << "No keyboard detected by the HID manager!" << std::endl;
        freeUp();
        return;
    }

    auto* const keyboards = static_cast<NSSet*>(underlying); // Toll-Free Bridge
    for (id keyboard in keyboards)
        loadKeyboard(static_cast<IOHIDDeviceRef>(keyboard));

    CFRelease(underlying);

    ////////////////////////////////////////////////////////////
    // At this point m_keys is filled with as many IOHIDElementRef as possible
}


////////////////////////////////////////////////////////////
void HIDInputManager::loadKeyboard(IOHIDDeviceRef keyboard)
{
    CFArrayRef underlying = IOHIDDeviceCopyMatchingElements(keyboard, nullptr, kIOHIDOptionsTypeNone);
    if ((underlying == nullptr) || (CFArrayGetCount(underlying) == 0))
    {
        err() << "Detected a keyboard without any keys." << std::endl;
        return;
    }

    auto* const keys = static_cast<NSArray*>(underlying); // Toll-Free Bridge
    for (id key in keys)
    {
        auto* elem = static_cast<IOHIDElementRef>(key);
        if (IOHIDElementGetUsagePage(elem) == kHIDPage_KeyboardOrKeypad)
            loadKey(elem);
    }

    CFRelease(underlying);
}


////////////////////////////////////////////////////////////
void HIDInputManager::loadKey(IOHIDElementRef key)
{
    const std::uint32_t      usage = IOHIDElementGetUsage(key);
    const Keyboard::Scancode code  = usageToScancode(usage);
    if (code != Keyboard::Scan::Unknown)
    {
        CFRetain(key);
        m_keys[static_cast<std::size_t>(code)].push_back(key);
    }
}


////////////////////////////////////////////////////////////
void HIDInputManager::buildMappings()
{
    // Reset the mappings
    for (auto& scancode : m_keyToScancodeMapping)
        scancode = Keyboard::Scan::Unknown;
    for (auto& key : m_scancodeToKeyMapping)
        key = Keyboard::Unknown;

    // Get the current keyboard layout
    TISInputSourceRef tis  = TISCopyCurrentKeyboardLayoutInputSource();
    const auto* layoutData = static_cast<CFDataRef>(TISGetInputSourceProperty(tis, kTISPropertyUnicodeKeyLayoutData));

    if (layoutData == nullptr)
    {
        err() << "Cannot get the keyboard layout\n";
        CFRelease(tis);
        return;
    }

    auto* layout = reinterpret_cast<UCKeyboardLayout*>(const_cast<std::uint8_t*>(CFDataGetBytePtr(layoutData)));

    // For each scancode having a IOHIDElement, we translate the corresponding
    // virtual code to a localized Key.
    for (int i = 0; i < static_cast<int>(Keyboard::Scan::ScancodeCount); ++i)
    {
        const auto         scan        = static_cast<Keyboard::Scancode>(i);
        const std::uint8_t virtualCode = scanToVirtualCode(scan);

        if (virtualCode == unknownVirtualCode)
            continue;

        // Translating virtual code to string is not injective. For example, virtual codes corresponding to
        // Scan::Num0 and Scan::Numpad0 may be both translated to the same string "0". We want to map those to
        // different Keyboard::Key nonetheless. This is why we do not translate some layout-independent keys
        // to string and use fallback mapping instead.
        bool translateToString = true;
        switch (scan)
        {
            case Keyboard::Scan::NumpadDivide:
            case Keyboard::Scan::NumpadMultiply:
            case Keyboard::Scan::NumpadMinus:
            case Keyboard::Scan::NumpadPlus:
            case Keyboard::Scan::Numpad1:
            case Keyboard::Scan::Numpad2:
            case Keyboard::Scan::Numpad3:
            case Keyboard::Scan::Numpad4:
            case Keyboard::Scan::Numpad5:
            case Keyboard::Scan::Numpad6:
            case Keyboard::Scan::Numpad7:
            case Keyboard::Scan::Numpad8:
            case Keyboard::Scan::Numpad9:
            case Keyboard::Scan::Numpad0:
                translateToString = false;
                break;
            default:
                translateToString = true;
                break;
        }

        Keyboard::Key code = Keyboard::Unknown;
        if (translateToString)
        {
            // Unicode string length is usually less or equal to 4
            const UniCharCount  maxLength = 4;
            UniChar             string[maxLength];
            UniCharCount        length       = 0;
            std::uint32_t       deadKeyState = 0;     // unused value
            std::uint32_t const modifiers    = 0x100; // no modifiers

            // Use current layout for translation
            const OSStatus error = UCKeyTranslate(layout,
                                                  virtualCode,
                                                  kUCKeyActionDown,
                                                  modifiers,
                                                  LMGetKbdType(),
                                                  kUCKeyTranslateNoDeadKeysMask,
                                                  &deadKeyState,
                                                  maxLength,
                                                  &length,
                                                  string);

            if (error != noErr)
            {
                err() << "Cannot translate the virtual key code, error: " << error << "\n";
                continue;
            }

            if (length > 0)
                code = localizedKey(string[0]);
        }
        if (code == Keyboard::Unknown)
            code = localizedKeyFallback(scan);
        if (code == Keyboard::Unknown)
            continue;

        // Register the bi-mapping
        if (m_keyToScancodeMapping[code] == Keyboard::Scan::Unknown)
            m_keyToScancodeMapping[code] = scan;
        m_scancodeToKeyMapping[static_cast<std::size_t>(scan)] = code;
    }

    CFRelease(tis);
}


////////////////////////////////////////////////////////////
void HIDInputManager::keyboardChanged(CFNotificationCenterRef /* center */,
                                      void* observer,
                                      CFStringRef /* name */,
                                      const void* /* object */,
                                      CFDictionaryRef /* userInfo */)
{
    auto* manager = static_cast<HIDInputManager*>(observer);
    manager->buildMappings();
}


////////////////////////////////////////////////////////////
void HIDInputManager::freeUp()
{
    if (m_manager != nil)
        CFRelease(m_manager);

    m_manager = nil;

    for (auto& key : m_keys)
    {
        for (IOHIDElementRef iohidElementRef : key)
            CFRelease(iohidElementRef);

        key.clear();
    }
}


////////////////////////////////////////////////////////////
CFSetRef HIDInputManager::copyDevices(std::uint32_t page, std::uint32_t usage)
{
    // Filter and keep only the requested devices
    CFDictionaryRef mask = copyDevicesMask(page, usage);

    IOHIDManagerSetDeviceMatching(m_manager, mask);

    CFRelease(mask);
    mask = nil;

    CFSetRef devices = IOHIDManagerCopyDevices(m_manager);
    if (devices == nullptr)
        return nullptr;

    // Is there at least one device?
    if (CFSetGetCount(devices) < 1)
    {
        CFRelease(devices);
        return nullptr;
    }

    return devices;
}


////////////////////////////////////////////////////////////
bool HIDInputManager::isPressed(IOHIDElements& elements) const
{
    bool pressed = false;
    for (auto it = elements.begin(); it != elements.end() && !pressed; /* noop */)
    {
        IOHIDValueRef value = nil;

        IOHIDDeviceRef device = IOHIDElementGetDevice(*it);
        IOHIDDeviceGetValue(device, *it, &value);

        if (!value)
        {
            // This means some kind of error / disconnection so we remove this element from our database.
            CFRelease(*it);
            it = elements.erase(it);
        }
        else
        {
            pressed = (IOHIDValueGetIntegerValue(value) == 1);
            ++it;
        }
    }

    return pressed;
}


////////////////////////////////////////////////////////////
Keyboard::Scancode HIDInputManager::usageToScancode(std::uint32_t usage)
{
    // clang-format off
    switch (usage)
    {
        case kHIDUsage_KeyboardErrorRollOver:  return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardPOSTFail:       return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardErrorUndefined: return Keyboard::Scan::Unknown;

        case kHIDUsage_KeyboardA: return Keyboard::Scan::A;
        case kHIDUsage_KeyboardB: return Keyboard::Scan::B;
        case kHIDUsage_KeyboardC: return Keyboard::Scan::C;
        case kHIDUsage_KeyboardD: return Keyboard::Scan::D;
        case kHIDUsage_KeyboardE: return Keyboard::Scan::E;
        case kHIDUsage_KeyboardF: return Keyboard::Scan::F;
        case kHIDUsage_KeyboardG: return Keyboard::Scan::G;
        case kHIDUsage_KeyboardH: return Keyboard::Scan::H;
        case kHIDUsage_KeyboardI: return Keyboard::Scan::I;
        case kHIDUsage_KeyboardJ: return Keyboard::Scan::J;
        case kHIDUsage_KeyboardK: return Keyboard::Scan::K;
        case kHIDUsage_KeyboardL: return Keyboard::Scan::L;
        case kHIDUsage_KeyboardM: return Keyboard::Scan::M;
        case kHIDUsage_KeyboardN: return Keyboard::Scan::N;
        case kHIDUsage_KeyboardO: return Keyboard::Scan::O;
        case kHIDUsage_KeyboardP: return Keyboard::Scan::P;
        case kHIDUsage_KeyboardQ: return Keyboard::Scan::Q;
        case kHIDUsage_KeyboardR: return Keyboard::Scan::R;
        case kHIDUsage_KeyboardS: return Keyboard::Scan::S;
        case kHIDUsage_KeyboardT: return Keyboard::Scan::T;
        case kHIDUsage_KeyboardU: return Keyboard::Scan::U;
        case kHIDUsage_KeyboardV: return Keyboard::Scan::V;
        case kHIDUsage_KeyboardW: return Keyboard::Scan::W;
        case kHIDUsage_KeyboardX: return Keyboard::Scan::X;
        case kHIDUsage_KeyboardY: return Keyboard::Scan::Y;
        case kHIDUsage_KeyboardZ: return Keyboard::Scan::Z;

        case kHIDUsage_Keyboard1: return Keyboard::Scan::Num1;
        case kHIDUsage_Keyboard2: return Keyboard::Scan::Num2;
        case kHIDUsage_Keyboard3: return Keyboard::Scan::Num3;
        case kHIDUsage_Keyboard4: return Keyboard::Scan::Num4;
        case kHIDUsage_Keyboard5: return Keyboard::Scan::Num5;
        case kHIDUsage_Keyboard6: return Keyboard::Scan::Num6;
        case kHIDUsage_Keyboard7: return Keyboard::Scan::Num7;
        case kHIDUsage_Keyboard8: return Keyboard::Scan::Num8;
        case kHIDUsage_Keyboard9: return Keyboard::Scan::Num9;
        case kHIDUsage_Keyboard0: return Keyboard::Scan::Num0;

        case kHIDUsage_KeyboardReturnOrEnter:       return Keyboard::Scan::Enter;
        case kHIDUsage_KeyboardEscape:              return Keyboard::Scan::Escape;
        case kHIDUsage_KeyboardDeleteOrBackspace:   return Keyboard::Scan::Backspace;
        case kHIDUsage_KeyboardTab:                 return Keyboard::Scan::Tab;
        case kHIDUsage_KeyboardSpacebar:            return Keyboard::Scan::Space;
        case kHIDUsage_KeyboardHyphen:              return Keyboard::Scan::Hyphen;
        case kHIDUsage_KeyboardEqualSign:           return Keyboard::Scan::Equal;
        case kHIDUsage_KeyboardOpenBracket:         return Keyboard::Scan::LBracket;
        case kHIDUsage_KeyboardCloseBracket:        return Keyboard::Scan::RBracket;
        case kHIDUsage_KeyboardBackslash:           return Keyboard::Scan::Backslash;
        case kHIDUsage_KeyboardNonUSPound:          return Keyboard::Scan::Backslash;
        case kHIDUsage_KeyboardSemicolon:           return Keyboard::Scan::Semicolon;
        case kHIDUsage_KeyboardQuote:               return Keyboard::Scan::Apostrophe;
        case kHIDUsage_KeyboardGraveAccentAndTilde: return isIsoKeyboard ? Keyboard::Scan::NonUsBackslash : Keyboard::Scan::Grave;
        case kHIDUsage_KeyboardComma:               return Keyboard::Scan::Comma;
        case kHIDUsage_KeyboardPeriod:              return Keyboard::Scan::Period;
        case kHIDUsage_KeyboardSlash:               return Keyboard::Scan::Slash;
        case kHIDUsage_KeyboardCapsLock:            return Keyboard::Scan::CapsLock;

        case kHIDUsage_KeyboardF1:  return Keyboard::Scan::F1;
        case kHIDUsage_KeyboardF2:  return Keyboard::Scan::F2;
        case kHIDUsage_KeyboardF3:  return Keyboard::Scan::F3;
        case kHIDUsage_KeyboardF4:  return Keyboard::Scan::F4;
        case kHIDUsage_KeyboardF5:  return Keyboard::Scan::F5;
        case kHIDUsage_KeyboardF6:  return Keyboard::Scan::F6;
        case kHIDUsage_KeyboardF7:  return Keyboard::Scan::F7;
        case kHIDUsage_KeyboardF8:  return Keyboard::Scan::F8;
        case kHIDUsage_KeyboardF9:  return Keyboard::Scan::F9;
        case kHIDUsage_KeyboardF10: return Keyboard::Scan::F10;
        case kHIDUsage_KeyboardF11: return Keyboard::Scan::F11;
        case kHIDUsage_KeyboardF12: return Keyboard::Scan::F12;

        case kHIDUsage_KeyboardPrintScreen:   return Keyboard::Scan::PrintScreen;
        case kHIDUsage_KeyboardScrollLock:    return Keyboard::Scan::ScrollLock;
        case kHIDUsage_KeyboardPause:         return Keyboard::Scan::Pause;
        case kHIDUsage_KeyboardInsert:        return Keyboard::Scan::Insert;
        case kHIDUsage_KeyboardHome:          return Keyboard::Scan::Home;
        case kHIDUsage_KeyboardPageUp:        return Keyboard::Scan::PageUp;
        case kHIDUsage_KeyboardDeleteForward: return Keyboard::Scan::Delete;
        case kHIDUsage_KeyboardEnd:           return Keyboard::Scan::End;
        case kHIDUsage_KeyboardPageDown:      return Keyboard::Scan::PageDown;

        case kHIDUsage_KeyboardRightArrow: return Keyboard::Scan::Right;
        case kHIDUsage_KeyboardLeftArrow:  return Keyboard::Scan::Left;
        case kHIDUsage_KeyboardDownArrow:  return Keyboard::Scan::Down;
        case kHIDUsage_KeyboardUpArrow:    return Keyboard::Scan::Up;

        case kHIDUsage_KeypadNumLock:   return Keyboard::Scan::NumLock;
        case kHIDUsage_KeypadSlash:     return Keyboard::Scan::NumpadDivide;
        case kHIDUsage_KeypadAsterisk:  return Keyboard::Scan::NumpadMultiply;
        case kHIDUsage_KeypadHyphen:    return Keyboard::Scan::NumpadMinus;
        case kHIDUsage_KeypadPlus:      return Keyboard::Scan::NumpadPlus;
        case kHIDUsage_KeypadEqualSign: return Keyboard::Scan::NumpadEqual;
        case kHIDUsage_KeypadEnter:     return Keyboard::Scan::NumpadEnter;
        case kHIDUsage_KeypadPeriod:    return Keyboard::Scan::NumpadDecimal;

        case kHIDUsage_Keypad1: return Keyboard::Scan::Numpad1;
        case kHIDUsage_Keypad2: return Keyboard::Scan::Numpad2;
        case kHIDUsage_Keypad3: return Keyboard::Scan::Numpad3;
        case kHIDUsage_Keypad4: return Keyboard::Scan::Numpad4;
        case kHIDUsage_Keypad5: return Keyboard::Scan::Numpad5;
        case kHIDUsage_Keypad6: return Keyboard::Scan::Numpad6;
        case kHIDUsage_Keypad7: return Keyboard::Scan::Numpad7;
        case kHIDUsage_Keypad8: return Keyboard::Scan::Numpad8;
        case kHIDUsage_Keypad9: return Keyboard::Scan::Numpad9;
        case kHIDUsage_Keypad0: return Keyboard::Scan::Numpad0;

        case kHIDUsage_KeyboardNonUSBackslash: return isIsoKeyboard ? Keyboard::Scan::Grave : Keyboard::Scan::NonUsBackslash;
        case kHIDUsage_KeyboardApplication:    return Keyboard::Scan::Application;
        case kHIDUsage_KeyboardPower:          return Keyboard::Scan::Unknown;

        case kHIDUsage_KeyboardF13: return Keyboard::Scan::F13;
        case kHIDUsage_KeyboardF14: return Keyboard::Scan::F14;
        case kHIDUsage_KeyboardF15: return Keyboard::Scan::F15;
        case kHIDUsage_KeyboardF16: return Keyboard::Scan::F16;
        case kHIDUsage_KeyboardF17: return Keyboard::Scan::F17;
        case kHIDUsage_KeyboardF18: return Keyboard::Scan::F18;
        case kHIDUsage_KeyboardF19: return Keyboard::Scan::F19;
        case kHIDUsage_KeyboardF20: return Keyboard::Scan::F20;
        case kHIDUsage_KeyboardF21: return Keyboard::Scan::F21;
        case kHIDUsage_KeyboardF22: return Keyboard::Scan::F22;
        case kHIDUsage_KeyboardF23: return Keyboard::Scan::F23;
        case kHIDUsage_KeyboardF24: return Keyboard::Scan::F24;

        case kHIDUsage_KeyboardExecute: return Keyboard::Scan::Execute;
        case kHIDUsage_KeyboardHelp:    return Keyboard::Scan::Help;
        case kHIDUsage_KeyboardMenu:    return Keyboard::Scan::Menu;
        case kHIDUsage_KeyboardSelect:  return Keyboard::Scan::Select;
        case kHIDUsage_KeyboardStop:    return Keyboard::Scan::Stop;
        case kHIDUsage_KeyboardAgain:   return Keyboard::Scan::Redo;
        case kHIDUsage_KeyboardUndo:    return Keyboard::Scan::Undo;
        case kHIDUsage_KeyboardCut:     return Keyboard::Scan::Cut;
        case kHIDUsage_KeyboardCopy:    return Keyboard::Scan::Copy;
        case kHIDUsage_KeyboardPaste:   return Keyboard::Scan::Paste;
        case kHIDUsage_KeyboardFind:    return Keyboard::Scan::Search;

        case kHIDUsage_KeyboardMute:       return Keyboard::Scan::VolumeMute;
        case kHIDUsage_KeyboardVolumeUp:   return Keyboard::Scan::VolumeUp;
        case kHIDUsage_KeyboardVolumeDown: return Keyboard::Scan::VolumeDown;

        case kHIDUsage_KeyboardLockingCapsLock:   return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardLockingNumLock:    return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardLockingScrollLock: return Keyboard::Scan::Unknown;

        case kHIDUsage_KeypadComma:            return Keyboard::Scan::Unknown;
        case kHIDUsage_KeypadEqualSignAS400:   return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardInternational1: return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardInternational2: return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardInternational3: return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardInternational4: return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardInternational5: return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardInternational6: return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardInternational7: return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardInternational8: return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardInternational9: return Keyboard::Scan::Unknown;

        case kHIDUsage_KeyboardLANG1: return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardLANG2: return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardLANG3: return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardLANG4: return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardLANG5: return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardLANG6: return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardLANG7: return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardLANG8: return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardLANG9: return Keyboard::Scan::Unknown;

        case kHIDUsage_KeyboardAlternateErase:    return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardSysReqOrAttention: return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardCancel:            return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardClear:             return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardPrior:             return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardReturn:            return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardSeparator:         return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardOut:               return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardOper:              return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardClearOrAgain:      return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardCrSelOrProps:      return Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardExSel:             return Keyboard::Scan::Unknown;

        /* 0xa5-0xdf Reserved */

        case kHIDUsage_KeyboardLeftControl:  return Keyboard::Scan::LControl;
        case kHIDUsage_KeyboardLeftShift:    return Keyboard::Scan::LShift;
        case kHIDUsage_KeyboardLeftAlt:      return Keyboard::Scan::LAlt;
        case kHIDUsage_KeyboardLeftGUI:      return Keyboard::Scan::LSystem;
        case kHIDUsage_KeyboardRightControl: return Keyboard::Scan::RControl;
        case kHIDUsage_KeyboardRightShift:   return Keyboard::Scan::RShift;
        case kHIDUsage_KeyboardRightAlt:     return Keyboard::Scan::RAlt;
        case kHIDUsage_KeyboardRightGUI:     return Keyboard::Scan::RSystem;

        /* 0xe8-0xffff Reserved */

        case kHIDUsage_Keyboard_Reserved: return Keyboard::Scan::Unknown;
        default:                          return Keyboard::Scan::Unknown;
    }
    // clang-format on
}


////////////////////////////////////////////////////////
std::uint8_t HIDInputManager::scanToVirtualCode(Keyboard::Scancode code)
{
    // clang-format off
    switch (code)
    {
        case Keyboard::Scan::A: return 0x00;
        case Keyboard::Scan::B: return 0x0b;
        case Keyboard::Scan::C: return 0x08;
        case Keyboard::Scan::D: return 0x02;
        case Keyboard::Scan::E: return 0x0e;
        case Keyboard::Scan::F: return 0x03;
        case Keyboard::Scan::G: return 0x05;
        case Keyboard::Scan::H: return 0x04;
        case Keyboard::Scan::I: return 0x22;
        case Keyboard::Scan::J: return 0x26;
        case Keyboard::Scan::K: return 0x28;
        case Keyboard::Scan::L: return 0x25;
        case Keyboard::Scan::M: return 0x2e;
        case Keyboard::Scan::N: return 0x2d;
        case Keyboard::Scan::O: return 0x1f;
        case Keyboard::Scan::P: return 0x23;
        case Keyboard::Scan::Q: return 0x0c;
        case Keyboard::Scan::R: return 0x0f;
        case Keyboard::Scan::S: return 0x01;
        case Keyboard::Scan::T: return 0x11;
        case Keyboard::Scan::U: return 0x20;
        case Keyboard::Scan::V: return 0x09;
        case Keyboard::Scan::W: return 0x0d;
        case Keyboard::Scan::X: return 0x07;
        case Keyboard::Scan::Y: return 0x10;
        case Keyboard::Scan::Z: return 0x06;

        case Keyboard::Scan::Num1: return 0x12;
        case Keyboard::Scan::Num2: return 0x13;
        case Keyboard::Scan::Num3: return 0x14;
        case Keyboard::Scan::Num4: return 0x15;
        case Keyboard::Scan::Num5: return 0x17;
        case Keyboard::Scan::Num6: return 0x16;
        case Keyboard::Scan::Num7: return 0x1a;
        case Keyboard::Scan::Num8: return 0x1c;
        case Keyboard::Scan::Num9: return 0x19;
        case Keyboard::Scan::Num0: return 0x1d;

        case Keyboard::Scan::Enter:     return 0x24;
        case Keyboard::Scan::Escape:    return 0x35;
        case Keyboard::Scan::Backspace: return 0x33;
        case Keyboard::Scan::Tab:       return 0x30;
        case Keyboard::Scan::Space:     return 0x31;

        case Keyboard::Scan::Equal:       return 0x18;
        case Keyboard::Scan::LBracket:    return 0x21;
        case Keyboard::Scan::RBracket:    return 0x1e;
        case Keyboard::Scan::Backslash:   return 0x2a;
        case Keyboard::Scan::Hyphen:      return 0x1b;
        case Keyboard::Scan::Semicolon:   return 0x29;
        case Keyboard::Scan::Apostrophe:  return 0x27;
        case Keyboard::Scan::Grave:       return isIsoKeyboard ? 0x0a : 0x32;
        case Keyboard::Scan::Comma:       return 0x2b;
        case Keyboard::Scan::Period:      return 0x2f;
        case Keyboard::Scan::Slash:       return 0x2c;

        case Keyboard::Scan::F1:  return 0x7a;
        case Keyboard::Scan::F2:  return 0x78;
        case Keyboard::Scan::F3:  return 0x63;
        case Keyboard::Scan::F4:  return 0x76;
        case Keyboard::Scan::F5:  return 0x60;
        case Keyboard::Scan::F6:  return 0x61;
        case Keyboard::Scan::F7:  return 0x62;
        case Keyboard::Scan::F8:  return 0x64;
        case Keyboard::Scan::F9:  return 0x65;
        case Keyboard::Scan::F10: return 0x6d;
        case Keyboard::Scan::F11: return 0x67;
        case Keyboard::Scan::F12: return 0x6f;
        case Keyboard::Scan::F13: return 0x69;
        case Keyboard::Scan::F14: return 0x6b;
        case Keyboard::Scan::F15: return 0x71;

        case Keyboard::Scan::CapsLock: return 0x39;

        /* TODO Those are missing:
         * case Keyboard::Scan::PrintScreen: return 0;
         * case Keyboard::Scan::ScrollLock:  return 0;
         * case Keyboard::Scan::Pause:    return 0;
         */
        case Keyboard::Scan::Insert:   return 0x72;
        case Keyboard::Scan::Home:     return 0x73;
        case Keyboard::Scan::PageUp:   return 0x74;
        case Keyboard::Scan::Delete:   return 0x75;
        case Keyboard::Scan::End:      return 0x77;
        case Keyboard::Scan::PageDown: return 0x79;

        case Keyboard::Scan::Right: return 0x7c;
        case Keyboard::Scan::Left:  return 0x7b;
        case Keyboard::Scan::Down:  return 0x7d;
        case Keyboard::Scan::Up:    return 0x7e;

        case Keyboard::Scan::NumLock:        return 0x47;
        case Keyboard::Scan::NumpadDivide:   return 0x4b;
        case Keyboard::Scan::NumpadMultiply: return 0x43;
        case Keyboard::Scan::NumpadMinus:    return 0x4e;
        case Keyboard::Scan::NumpadPlus:     return 0x45;
        case Keyboard::Scan::NumpadEqual:    return 0x51;
        case Keyboard::Scan::NumpadEnter:    return 0x4c;
        case Keyboard::Scan::NumpadDecimal:  return 0x41;

        case Keyboard::Scan::Numpad1: return 0x53;
        case Keyboard::Scan::Numpad2: return 0x54;
        case Keyboard::Scan::Numpad3: return 0x55;
        case Keyboard::Scan::Numpad4: return 0x56;
        case Keyboard::Scan::Numpad5: return 0x57;
        case Keyboard::Scan::Numpad6: return 0x58;
        case Keyboard::Scan::Numpad7: return 0x59;
        case Keyboard::Scan::Numpad8: return 0x5b;
        case Keyboard::Scan::Numpad9: return 0x5c;
        case Keyboard::Scan::Numpad0: return 0x52;

        case Keyboard::Scan::NonUsBackslash: return 0x32;

        /* TODO Those are missing:
         * case Keyboard::Scan::Application: return 0;
         * case Keyboard::Scan::Execute:     return 0;
         * case Keyboard::Scan::Help:        return 0;
         * case Keyboard::Scan::Menu:        return 0;
         * case Keyboard::Scan::Select:      return 0;
         * case Keyboard::Scan::Stop:        return 0;
         * case Keyboard::Scan::Redo:        return 0;
         * case Keyboard::Scan::Undo:        return 0;
         * case Keyboard::Scan::Cut:         return 0;
         * case Keyboard::Scan::Copy:        return 0;
         * case Keyboard::Scan::Paste:       return 0;
         * case Keyboard::Scan::Search:      return 0;
         */

        case Keyboard::Scan::VolumeMute: return 0x4a;
        case Keyboard::Scan::VolumeUp:   return 0x48;
        case Keyboard::Scan::VolumeDown: return 0x49;

        case Keyboard::Scan::LControl: return 0x3b;
        case Keyboard::Scan::LShift:   return 0x38;
        case Keyboard::Scan::LAlt:     return 0x3a;
        case Keyboard::Scan::LSystem:  return 0x37;
        case Keyboard::Scan::RControl: return 0x3b;
        case Keyboard::Scan::RShift:   return 0x38;
        case Keyboard::Scan::RAlt:     return 0x3a;
        case Keyboard::Scan::RSystem:  return 0x37;

        default: return unknownVirtualCode;
    }
    // clang-format on
}


////////////////////////////////////////////////////////
Keyboard::Key HIDInputManager::localizedKeyFallback(Keyboard::Scancode code)
{
    // clang-format off
    switch (code)
    {
        case Keyboard::Scan::Enter:      return Keyboard::Enter;
        case Keyboard::Scan::Escape:     return Keyboard::Escape;
        case Keyboard::Scan::Backspace:  return Keyboard::Backspace;
        case Keyboard::Scan::Tab:        return Keyboard::Tab;
        case Keyboard::Scan::Space:      return Keyboard::Space;

        case Keyboard::Scan::F1:   return Keyboard::F1;
        case Keyboard::Scan::F2:   return Keyboard::F2;
        case Keyboard::Scan::F3:   return Keyboard::F3;
        case Keyboard::Scan::F4:   return Keyboard::F4;
        case Keyboard::Scan::F5:   return Keyboard::F5;
        case Keyboard::Scan::F6:   return Keyboard::F6;
        case Keyboard::Scan::F7:   return Keyboard::F7;
        case Keyboard::Scan::F8:   return Keyboard::F8;
        case Keyboard::Scan::F9:   return Keyboard::F9;
        case Keyboard::Scan::F10:  return Keyboard::F10;
        case Keyboard::Scan::F11:  return Keyboard::F11;
        case Keyboard::Scan::F12:  return Keyboard::F12;
        case Keyboard::Scan::F13:  return Keyboard::F13;
        case Keyboard::Scan::F14:  return Keyboard::F14;
        case Keyboard::Scan::F15:  return Keyboard::F15;

        case Keyboard::Scan::Pause:    return Keyboard::Pause;
        case Keyboard::Scan::Insert:   return Keyboard::Insert;
        case Keyboard::Scan::Home:     return Keyboard::Home;
        case Keyboard::Scan::PageUp:   return Keyboard::PageUp;
        case Keyboard::Scan::Delete:   return Keyboard::Delete;
        case Keyboard::Scan::End:      return Keyboard::End;
        case Keyboard::Scan::PageDown: return Keyboard::PageDown;

        case Keyboard::Scan::Right:  return Keyboard::Right;
        case Keyboard::Scan::Left:   return Keyboard::Left;
        case Keyboard::Scan::Down:   return Keyboard::Down;
        case Keyboard::Scan::Up:     return Keyboard::Up;

        case Keyboard::Scan::NumpadDivide:   return Keyboard::Divide;
        case Keyboard::Scan::NumpadMultiply: return Keyboard::Multiply;
        case Keyboard::Scan::NumpadMinus:    return Keyboard::Subtract;
        case Keyboard::Scan::NumpadPlus:     return Keyboard::Add;

        case Keyboard::Scan::NumpadEnter: return Keyboard::Enter;

        case Keyboard::Scan::Numpad1:  return Keyboard::Numpad1;
        case Keyboard::Scan::Numpad2:  return Keyboard::Numpad2;
        case Keyboard::Scan::Numpad3:  return Keyboard::Numpad3;
        case Keyboard::Scan::Numpad4:  return Keyboard::Numpad4;
        case Keyboard::Scan::Numpad5:  return Keyboard::Numpad5;
        case Keyboard::Scan::Numpad6:  return Keyboard::Numpad6;
        case Keyboard::Scan::Numpad7:  return Keyboard::Numpad7;
        case Keyboard::Scan::Numpad8:  return Keyboard::Numpad8;
        case Keyboard::Scan::Numpad9:  return Keyboard::Numpad9;
        case Keyboard::Scan::Numpad0:  return Keyboard::Numpad0;

        case Keyboard::Scan::LControl:  return Keyboard::LControl;
        case Keyboard::Scan::LShift:    return Keyboard::LShift;
        case Keyboard::Scan::LAlt:      return Keyboard::LAlt;
        case Keyboard::Scan::LSystem:   return Keyboard::LSystem;
        case Keyboard::Scan::RControl:  return Keyboard::RControl;
        case Keyboard::Scan::RShift:    return Keyboard::RShift;
        case Keyboard::Scan::RAlt:      return Keyboard::RAlt;
        case Keyboard::Scan::RSystem:   return Keyboard::RSystem;

        default: return Keyboard::Unknown;
    }
    // clang-format on
}


} // namespace sf::priv
