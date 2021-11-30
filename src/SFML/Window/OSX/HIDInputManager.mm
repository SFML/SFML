////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Marco Antognini (antognini.marco@gmail.com),
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
#include <SFML/Window/OSX/HIDInputManager.hpp>
#include <SFML/System/Err.hpp>
#include <AppKit/AppKit.h>

namespace
{
    void keyboardChanged(CFNotificationCenterRef, void* observer, CFStringRef, const void*, CFDictionaryRef)
    {
        sf::priv::HIDInputManager* manager = static_cast<sf::priv::HIDInputManager*>(observer);
        manager->buildMappings();
    }
}

namespace sf
{
namespace priv
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
    CFTypeRef typeRef = IOHIDDeviceGetProperty(device,
                                               CFSTR(kIOHIDLocationIDKey));
    if (!typeRef || (CFGetTypeID(typeRef) != CFNumberGetTypeID()))
        return 0;

    CFNumberRef locRef = static_cast<CFNumberRef>(typeRef);

    if (!CFNumberGetValue(locRef, kCFNumberLongType, &loc))
        return 0;

    return loc;
}


////////////////////////////////////////////////////////////
CFDictionaryRef HIDInputManager::copyDevicesMask(UInt32 page, UInt32 usage)
{
    // Create the dictionary.
    CFMutableDictionaryRef dict = CFDictionaryCreateMutable(kCFAllocatorDefault, 2,
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


////////////////////////////////////////////////////////////
bool HIDInputManager::isKeyPressed(Keyboard::Key key)
{
    return isKeyPressed(delocalize(key));
}


////////////////////////////////////////////////////////////
bool HIDInputManager::isKeyPressed(Keyboard::Scancode code)
{
    return (code != Keyboard::Scan::Unknown) && isPressed(m_keys[code]);
}


////////////////////////////////////////////////////////////
Keyboard::Key HIDInputManager::localize(Keyboard::Scancode code)
{
    // TODO ensure mapping is still valid
    return m_scancodeToKeyMapping[code];
}


////////////////////////////////////////////////////////////
Keyboard::Scancode HIDInputManager::delocalize(Keyboard::Key key)
{
    // TODO ensure mapping is still valid
    return m_keyToScancodeMapping[key];
}


////////////////////////////////////////////////////////////
String HIDInputManager::getDescription(Keyboard::Scancode code)
{
    // Phase 1: Get names for layout independent keys
    switch (code)
    {
        case sf::Keyboard::Scan::Enter:     return "Enter";
        case sf::Keyboard::Scan::Escape:    return "Escape";
        case sf::Keyboard::Scan::Backspace: return "Backspace";
        case sf::Keyboard::Scan::Tab:       return "Tab";
        case sf::Keyboard::Scan::Space:     return "Space";

        case sf::Keyboard::Scan::F1:  return "F1";
        case sf::Keyboard::Scan::F2:  return "F2";
        case sf::Keyboard::Scan::F3:  return "F3";
        case sf::Keyboard::Scan::F4:  return "F4";
        case sf::Keyboard::Scan::F5:  return "F5";
        case sf::Keyboard::Scan::F6:  return "F6";
        case sf::Keyboard::Scan::F7:  return "F7";
        case sf::Keyboard::Scan::F8:  return "F8";
        case sf::Keyboard::Scan::F9:  return "F9";
        case sf::Keyboard::Scan::F10: return "F10";
        case sf::Keyboard::Scan::F11: return "F11";
        case sf::Keyboard::Scan::F12: return "F12";
        case sf::Keyboard::Scan::F13: return "F13";
        case sf::Keyboard::Scan::F14: return "F14";
        case sf::Keyboard::Scan::F15: return "F15";
        case sf::Keyboard::Scan::F16: return "F16";
        case sf::Keyboard::Scan::F17: return "F17";
        case sf::Keyboard::Scan::F18: return "F18";
        case sf::Keyboard::Scan::F19: return "F19";
        case sf::Keyboard::Scan::F20: return "F20";
        case sf::Keyboard::Scan::F21: return "F21";
        case sf::Keyboard::Scan::F22: return "F22";
        case sf::Keyboard::Scan::F23: return "F23";
        case sf::Keyboard::Scan::F24: return "F24";

        case sf::Keyboard::Scan::CapsLock:    return "CapsLock";
        case sf::Keyboard::Scan::PrintScreen: return "PrintScreen";
        case sf::Keyboard::Scan::ScrollLock:  return "ScrollLock";

        case sf::Keyboard::Scan::Pause:    return "Pause";
        case sf::Keyboard::Scan::Insert:   return "Insert";
        case sf::Keyboard::Scan::Home:     return "Home";
        case sf::Keyboard::Scan::PageUp:   return "PageUp";
        case sf::Keyboard::Scan::Delete:   return "Delete";
        case sf::Keyboard::Scan::End:      return "End";
        case sf::Keyboard::Scan::PageDown: return "PageDown";

        case sf::Keyboard::Scan::Left:  return "Left Arrow";
        case sf::Keyboard::Scan::Right: return "Right Arrow";
        case sf::Keyboard::Scan::Down:  return "Down Arrow";
        case sf::Keyboard::Scan::Up:    return "Up Arrow";

        case sf::Keyboard::Scan::NumLock:        return "NumLock";
        case sf::Keyboard::Scan::NumpadDivide:   return "Divide (Numpad)";
        case sf::Keyboard::Scan::NumpadMultiply: return "Multiply (Numpad)";
        case sf::Keyboard::Scan::NumpadMinus:    return "Minus (Numpad)";
        case sf::Keyboard::Scan::NumpadPlus:     return "Plus (Numpad)";
        case sf::Keyboard::Scan::NumpadEqual:    return "Equal (Numpad)";
        case sf::Keyboard::Scan::NumpadEnter:    return "Enter (Numpad)";
        case sf::Keyboard::Scan::NumpadDecimal:  return "Decimal (Numpad)";

        case sf::Keyboard::Scan::Numpad0: return "0 (Numpad)";
        case sf::Keyboard::Scan::Numpad1: return "1 (Numpad)";
        case sf::Keyboard::Scan::Numpad2: return "2 (Numpad)";
        case sf::Keyboard::Scan::Numpad3: return "3 (Numpad)";
        case sf::Keyboard::Scan::Numpad4: return "4 (Numpad)";
        case sf::Keyboard::Scan::Numpad5: return "5 (Numpad)";
        case sf::Keyboard::Scan::Numpad6: return "6 (Numpad)";
        case sf::Keyboard::Scan::Numpad7: return "7 (Numpad)";
        case sf::Keyboard::Scan::Numpad8: return "8 (Numpad)";
        case sf::Keyboard::Scan::Numpad9: return "9 (Numpad)";

        case sf::Keyboard::Scan::Application: return "Application";
        case sf::Keyboard::Scan::Execute:     return "Execute";
        case sf::Keyboard::Scan::Help:        return "Help";
        case sf::Keyboard::Scan::Menu:        return "Menu";
        case sf::Keyboard::Scan::Select:      return "Select";
        case sf::Keyboard::Scan::Stop:        return "Stop";
        case sf::Keyboard::Scan::Redo:        return "Redo";
        case sf::Keyboard::Scan::Undo:        return "Undo";
        case sf::Keyboard::Scan::Cut:         return "Cut";
        case sf::Keyboard::Scan::Copy:        return "Copy";
        case sf::Keyboard::Scan::Paste:       return "Paste";
        case sf::Keyboard::Scan::Search:      return "Search";
        case sf::Keyboard::Scan::VolumeMute:  return "Volume Mute";
        case sf::Keyboard::Scan::VolumeUp:    return "Volume Up";
        case sf::Keyboard::Scan::VolumeDown:  return "Volume Down";

        case sf::Keyboard::Scan::LControl: return "Left Control";
        case sf::Keyboard::Scan::LShift:   return "Left Shift";
        case sf::Keyboard::Scan::LAlt:     return "Left Alt";
        case sf::Keyboard::Scan::LSystem:  return "Left Command";
        case sf::Keyboard::Scan::RControl: return "Right Control";
        case sf::Keyboard::Scan::RShift:   return "Right Shift";
        case sf::Keyboard::Scan::RAlt:     return "Right Alt";
        case sf::Keyboard::Scan::RSystem:  return "Right Command";

        default:
        {
            // Phase 2: Try to convert the key to unicode
            UniChar unicode = toUnicode(localize(code));
            if (unicode != 0x00)
                return sf::String(static_cast<Uint32>(unicode));
        }

        // Phase 3: Return final fallback
        return "Unknown";
    }
}


////////////////////////////////////////////////////////////
HIDInputManager::HIDInputManager() :
m_manager(0)
{
    // Create an HID Manager reference
    m_manager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    IOReturn openStatus = IOHIDManagerOpen(m_manager, kIOHIDOptionsTypeNone);

    if (openStatus != kIOReturnSuccess)
    {
        sf::err() << "Error when opening the HID manager" << std::endl;
        freeUp();
        return;
    }

    // Build up our knownledge of the hardware
    initializeKeyboard();
    buildMappings();

    // Register for notification on keyboard layout changes
    CFNotificationCenterAddObserver(
      CFNotificationCenterGetDistributedCenter(),
      this,
      keyboardChanged, // callback
      kTISNotifySelectedKeyboardInputSourceChanged,
      NULL, // use callback
      CFNotificationSuspensionBehaviorDeliverImmediately
    );
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
    if (underlying == NULL)
    {
        sf::err() << "No keyboard detected by the HID manager!" << std::endl;
        freeUp();
        return;
    }

    NSSet* keyboards = static_cast<NSSet*>(underlying); // Toll-Free Bridge
    for (id keyboard in keyboards)
      loadKeyboard(static_cast<IOHIDDeviceRef>(keyboard));

    CFRelease(underlying);

    ////////////////////////////////////////////////////////////
    // At this point m_keys is filled with as many IOHIDElementRef as possible
}


////////////////////////////////////////////////////////////
void HIDInputManager::loadKeyboard(IOHIDDeviceRef keyboard)
{
    CFArrayRef underlying = IOHIDDeviceCopyMatchingElements(keyboard, NULL, kIOHIDOptionsTypeNone);
    if ((underlying == NULL) || (CFArrayGetCount(underlying) == 0))
    {
        sf::err() << "We got a keyboard without any keys." << std::endl;
        return;
    }

    NSArray* keys = static_cast<NSArray*>(underlying); // Toll-Free Bridge
    for (id key in keys)
    {
        IOHIDElementRef elem = static_cast<IOHIDElementRef>(key);
        if (IOHIDElementGetUsagePage(elem) == kHIDPage_KeyboardOrKeypad)
          loadKey(elem);
    }

    CFRelease(underlying);
}


////////////////////////////////////////////////////////////
void HIDInputManager::loadKey(IOHIDElementRef key)
{
    UInt32 usage = IOHIDElementGetUsage(key);
    sf::Keyboard::Scancode code = usageToScancode(usage);
    if (code != Keyboard::Scan::Unknown)
    {
        CFRetain(key);
        m_keys[code].push_back(key);
    }
}


////////////////////////////////////////////////////////////
void HIDInputManager::buildMappings()
{
    // Reset the mappings
    for (int i = 0; i < Keyboard::KeyCount; ++i)
        m_keyToScancodeMapping[i] = Keyboard::Scan::Unknown;
    for (int i = 0; i < Keyboard::Scan::ScancodeCount; ++i)
        m_scancodeToKeyMapping[i] = Keyboard::Unknown;

    // Get the current keyboard layout
    TISInputSourceRef tis = TISCopyCurrentKeyboardLayoutInputSource();
    CFDataRef layoutData = static_cast<CFDataRef>(TISGetInputSourceProperty(tis, kTISPropertyUnicodeKeyLayoutData));

    if (layoutData == 0)
    {
        sf::err() << "Cannot get the keyboard layout\n";
        CFRelease(tis);
        return;
    }

    UCKeyboardLayout* layout = reinterpret_cast<UCKeyboardLayout*>(const_cast<UInt8*>(CFDataGetBytePtr(layoutData)));

    // For each scancode having a IOHIDElement, we translate the corresponding
    // virtual code to a localized Key.
    for (int i = 0; i < Keyboard::Scan::ScancodeCount; ++i)
    {
        Keyboard::Scancode scan = static_cast<Keyboard::Scancode>(i);
        UInt8 virtualCode = scanToVirtualCode(scan);

        if (virtualCode == 0xff) continue;

        // Unicode string length is usually less or equal to 4
        UniCharCount const MAX_LENGTH = 4;
        UniChar string[MAX_LENGTH];
        UniCharCount length = 0;
        UInt32 deadKeyState = 0; // unused value
        UInt32 const modifiers = 0x100; // no modifiers

        // Use current layout for translation
        OSStatus error = UCKeyTranslate(
            layout, virtualCode, kUCKeyActionDown, modifiers, LMGetKbdType(),
            kUCKeyTranslateNoDeadKeysBit, &deadKeyState, MAX_LENGTH, &length, string
        );

        if (error != noErr)
        {
            sf::err() << "Cannot translate the virtual key code, error: " << error << "\n";
            continue;
        }

        Keyboard::Key code = (length > 0) ? localizedKey(string[0]) : Keyboard::Unknown;
        
        if (code == Keyboard::Unknown)
            code = localizedKeyFallback(scan);
        if (code == Keyboard::Unknown)
            continue;

        // Register the bi-mapping
        m_keyToScancodeMapping[code] = scan;
        m_scancodeToKeyMapping[scan] = code;
    }

    CFRelease(tis);
}


////////////////////////////////////////////////////////////
void HIDInputManager::freeUp()
{
    if (m_manager != 0)
        CFRelease(m_manager);
    m_manager = 0;

    for (unsigned int i = 0; i < Keyboard::KeyCount; ++i)
    {
        for (IOHIDElements::iterator it = m_keys[i].begin(); it != m_keys[i].end(); ++it)
            CFRelease(*it);

        m_keys[i].clear();
    }
}


////////////////////////////////////////////////////////////
CFSetRef HIDInputManager::copyDevices(UInt32 page, UInt32 usage)
{
    // Filter and keep only the requested devices
    CFDictionaryRef mask = copyDevicesMask(page, usage);

    IOHIDManagerSetDeviceMatching(m_manager, mask);

    CFRelease(mask);
    mask = NULL;

    CFSetRef devices = IOHIDManagerCopyDevices(m_manager);
    if (devices == NULL)
        return NULL;

    // Is there at least one device?
    if (CFSetGetCount(devices) < 1)
    {
        CFRelease(devices);
        return NULL;
    }
    
    return devices;
}


////////////////////////////////////////////////////////////
bool HIDInputManager::isPressed(IOHIDElements& elements)
{
    bool pressed = false;
    for (IOHIDElements::iterator it = elements.begin(); it != elements.end() && !pressed; /* noop */)
    {
        IOHIDValueRef value = 0;

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
sf::Keyboard::Scancode HIDInputManager::usageToScancode(UInt32 usage)
{
    switch (usage)
    {
        case kHIDUsage_KeyboardErrorRollOver:  return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardPOSTFail:       return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardErrorUndefined: return sf::Keyboard::Scan::Unknown;

        case kHIDUsage_KeyboardA: return sf::Keyboard::Scan::A;
        case kHIDUsage_KeyboardB: return sf::Keyboard::Scan::B;
        case kHIDUsage_KeyboardC: return sf::Keyboard::Scan::C;
        case kHIDUsage_KeyboardD: return sf::Keyboard::Scan::D;
        case kHIDUsage_KeyboardE: return sf::Keyboard::Scan::E;
        case kHIDUsage_KeyboardF: return sf::Keyboard::Scan::F;
        case kHIDUsage_KeyboardG: return sf::Keyboard::Scan::G;
        case kHIDUsage_KeyboardH: return sf::Keyboard::Scan::H;
        case kHIDUsage_KeyboardI: return sf::Keyboard::Scan::I;
        case kHIDUsage_KeyboardJ: return sf::Keyboard::Scan::J;
        case kHIDUsage_KeyboardK: return sf::Keyboard::Scan::K;
        case kHIDUsage_KeyboardL: return sf::Keyboard::Scan::L;
        case kHIDUsage_KeyboardM: return sf::Keyboard::Scan::M;
        case kHIDUsage_KeyboardN: return sf::Keyboard::Scan::N;
        case kHIDUsage_KeyboardO: return sf::Keyboard::Scan::O;
        case kHIDUsage_KeyboardP: return sf::Keyboard::Scan::P;
        case kHIDUsage_KeyboardQ: return sf::Keyboard::Scan::Q;
        case kHIDUsage_KeyboardR: return sf::Keyboard::Scan::R;
        case kHIDUsage_KeyboardS: return sf::Keyboard::Scan::S;
        case kHIDUsage_KeyboardT: return sf::Keyboard::Scan::T;
        case kHIDUsage_KeyboardU: return sf::Keyboard::Scan::U;
        case kHIDUsage_KeyboardV: return sf::Keyboard::Scan::V;
        case kHIDUsage_KeyboardW: return sf::Keyboard::Scan::W;
        case kHIDUsage_KeyboardX: return sf::Keyboard::Scan::X;
        case kHIDUsage_KeyboardY: return sf::Keyboard::Scan::Y;
        case kHIDUsage_KeyboardZ: return sf::Keyboard::Scan::Z;

        case kHIDUsage_Keyboard1: return sf::Keyboard::Scan::Num1;
        case kHIDUsage_Keyboard2: return sf::Keyboard::Scan::Num2;
        case kHIDUsage_Keyboard3: return sf::Keyboard::Scan::Num3;
        case kHIDUsage_Keyboard4: return sf::Keyboard::Scan::Num4;
        case kHIDUsage_Keyboard5: return sf::Keyboard::Scan::Num5;
        case kHIDUsage_Keyboard6: return sf::Keyboard::Scan::Num6;
        case kHIDUsage_Keyboard7: return sf::Keyboard::Scan::Num7;
        case kHIDUsage_Keyboard8: return sf::Keyboard::Scan::Num8;
        case kHIDUsage_Keyboard9: return sf::Keyboard::Scan::Num9;
        case kHIDUsage_Keyboard0: return sf::Keyboard::Scan::Num0;

        case kHIDUsage_KeyboardReturnOrEnter:       return sf::Keyboard::Scan::Enter;
        case kHIDUsage_KeyboardEscape:              return sf::Keyboard::Scan::Escape;
        case kHIDUsage_KeyboardDeleteOrBackspace:   return sf::Keyboard::Scan::Backspace;
        case kHIDUsage_KeyboardTab:                 return sf::Keyboard::Scan::Tab;
        case kHIDUsage_KeyboardSpacebar:            return sf::Keyboard::Scan::Space;
        case kHIDUsage_KeyboardHyphen:              return sf::Keyboard::Scan::Hyphen;
        case kHIDUsage_KeyboardEqualSign:           return sf::Keyboard::Scan::Equal;
        case kHIDUsage_KeyboardOpenBracket:         return sf::Keyboard::Scan::LBracket;
        case kHIDUsage_KeyboardCloseBracket:        return sf::Keyboard::Scan::RBracket;
        case kHIDUsage_KeyboardBackslash:           return sf::Keyboard::Scan::Backslash;
        case kHIDUsage_KeyboardNonUSPound:          return sf::Keyboard::Scan::Backslash;
        case kHIDUsage_KeyboardSemicolon:           return sf::Keyboard::Scan::Semicolon;
        case kHIDUsage_KeyboardQuote:               return sf::Keyboard::Scan::Apostrophe;
        case kHIDUsage_KeyboardGraveAccentAndTilde: return sf::Keyboard::Scan::Grave;
        case kHIDUsage_KeyboardComma:               return sf::Keyboard::Scan::Comma;
        case kHIDUsage_KeyboardPeriod:              return sf::Keyboard::Scan::Period;
        case kHIDUsage_KeyboardSlash:               return sf::Keyboard::Scan::Slash;
        case kHIDUsage_KeyboardCapsLock:            return sf::Keyboard::Scan::CapsLock;

        case kHIDUsage_KeyboardF1:  return sf::Keyboard::Scan::F1;
        case kHIDUsage_KeyboardF2:  return sf::Keyboard::Scan::F2;
        case kHIDUsage_KeyboardF3:  return sf::Keyboard::Scan::F3;
        case kHIDUsage_KeyboardF4:  return sf::Keyboard::Scan::F4;
        case kHIDUsage_KeyboardF5:  return sf::Keyboard::Scan::F5;
        case kHIDUsage_KeyboardF6:  return sf::Keyboard::Scan::F6;
        case kHIDUsage_KeyboardF7:  return sf::Keyboard::Scan::F7;
        case kHIDUsage_KeyboardF8:  return sf::Keyboard::Scan::F8;
        case kHIDUsage_KeyboardF9:  return sf::Keyboard::Scan::F9;
        case kHIDUsage_KeyboardF10: return sf::Keyboard::Scan::F10;
        case kHIDUsage_KeyboardF11: return sf::Keyboard::Scan::F11;
        case kHIDUsage_KeyboardF12: return sf::Keyboard::Scan::F12;

        case kHIDUsage_KeyboardPrintScreen:   return sf::Keyboard::Scan::PrintScreen;
        case kHIDUsage_KeyboardScrollLock:    return sf::Keyboard::Scan::ScrollLock;
        case kHIDUsage_KeyboardPause:         return sf::Keyboard::Scan::Pause;
        case kHIDUsage_KeyboardInsert:        return sf::Keyboard::Scan::Insert;
        case kHIDUsage_KeyboardHome:          return sf::Keyboard::Scan::Home;
        case kHIDUsage_KeyboardPageUp:        return sf::Keyboard::Scan::PageUp;
        case kHIDUsage_KeyboardDeleteForward: return sf::Keyboard::Scan::Delete;
        case kHIDUsage_KeyboardEnd:           return sf::Keyboard::Scan::End;
        case kHIDUsage_KeyboardPageDown:      return sf::Keyboard::Scan::PageDown;

        case kHIDUsage_KeyboardRightArrow: return sf::Keyboard::Scan::Right;
        case kHIDUsage_KeyboardLeftArrow:  return sf::Keyboard::Scan::Left;
        case kHIDUsage_KeyboardDownArrow:  return sf::Keyboard::Scan::Down;
        case kHIDUsage_KeyboardUpArrow:    return sf::Keyboard::Scan::Up;

        case kHIDUsage_KeypadNumLock:   return sf::Keyboard::Scan::NumLock;
        case kHIDUsage_KeypadSlash:     return sf::Keyboard::Scan::NumpadDivide;
        case kHIDUsage_KeypadAsterisk:  return sf::Keyboard::Scan::NumpadMultiply;
        case kHIDUsage_KeypadHyphen:    return sf::Keyboard::Scan::NumpadMinus;
        case kHIDUsage_KeypadPlus:      return sf::Keyboard::Scan::NumpadPlus;
        case kHIDUsage_KeypadEqualSign: return sf::Keyboard::Scan::NumpadEqual;
        case kHIDUsage_KeypadEnter:     return sf::Keyboard::Scan::NumpadEnter;
        case kHIDUsage_KeypadPeriod:    return sf::Keyboard::Scan::NumpadDecimal;

        case kHIDUsage_Keypad1: return sf::Keyboard::Scan::Numpad1;
        case kHIDUsage_Keypad2: return sf::Keyboard::Scan::Numpad2;
        case kHIDUsage_Keypad3: return sf::Keyboard::Scan::Numpad3;
        case kHIDUsage_Keypad4: return sf::Keyboard::Scan::Numpad4;
        case kHIDUsage_Keypad5: return sf::Keyboard::Scan::Numpad5;
        case kHIDUsage_Keypad6: return sf::Keyboard::Scan::Numpad6;
        case kHIDUsage_Keypad7: return sf::Keyboard::Scan::Numpad7;
        case kHIDUsage_Keypad8: return sf::Keyboard::Scan::Numpad8;
        case kHIDUsage_Keypad9: return sf::Keyboard::Scan::Numpad9;
        case kHIDUsage_Keypad0: return sf::Keyboard::Scan::Numpad0;

        case kHIDUsage_KeyboardNonUSBackslash: return sf::Keyboard::Scan::NonUsBackslash;
        case kHIDUsage_KeyboardApplication:    return sf::Keyboard::Scan::Application;
        case kHIDUsage_KeyboardPower:          return sf::Keyboard::Scan::Unknown;

        case kHIDUsage_KeyboardF13: return sf::Keyboard::Scan::F13;
        case kHIDUsage_KeyboardF14: return sf::Keyboard::Scan::F14;
        case kHIDUsage_KeyboardF15: return sf::Keyboard::Scan::F15;
        case kHIDUsage_KeyboardF16: return sf::Keyboard::Scan::F16;
        case kHIDUsage_KeyboardF17: return sf::Keyboard::Scan::F17;
        case kHIDUsage_KeyboardF18: return sf::Keyboard::Scan::F18;
        case kHIDUsage_KeyboardF19: return sf::Keyboard::Scan::F19;
        case kHIDUsage_KeyboardF20: return sf::Keyboard::Scan::F20;
        case kHIDUsage_KeyboardF21: return sf::Keyboard::Scan::F21;
        case kHIDUsage_KeyboardF22: return sf::Keyboard::Scan::F22;
        case kHIDUsage_KeyboardF23: return sf::Keyboard::Scan::F23;
        case kHIDUsage_KeyboardF24: return sf::Keyboard::Scan::F24;

        case kHIDUsage_KeyboardExecute: return sf::Keyboard::Scan::Execute;
        case kHIDUsage_KeyboardHelp:    return sf::Keyboard::Scan::Help;
        case kHIDUsage_KeyboardMenu:    return sf::Keyboard::Scan::Menu;
        case kHIDUsage_KeyboardSelect:  return sf::Keyboard::Scan::Select;
        case kHIDUsage_KeyboardStop:    return sf::Keyboard::Scan::Stop;
        case kHIDUsage_KeyboardAgain:   return sf::Keyboard::Scan::Redo;
        case kHIDUsage_KeyboardUndo:    return sf::Keyboard::Scan::Undo;
        case kHIDUsage_KeyboardCut:     return sf::Keyboard::Scan::Cut;
        case kHIDUsage_KeyboardCopy:    return sf::Keyboard::Scan::Copy;
        case kHIDUsage_KeyboardPaste:   return sf::Keyboard::Scan::Paste;
        case kHIDUsage_KeyboardFind:    return sf::Keyboard::Scan::Search;

        case kHIDUsage_KeyboardMute:       return sf::Keyboard::Scan::VolumeMute;
        case kHIDUsage_KeyboardVolumeUp:   return sf::Keyboard::Scan::VolumeUp;
        case kHIDUsage_KeyboardVolumeDown: return sf::Keyboard::Scan::VolumeDown;

        case kHIDUsage_KeyboardLockingCapsLock:   return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardLockingNumLock:    return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardLockingScrollLock: return sf::Keyboard::Scan::Unknown;

        case kHIDUsage_KeypadComma:            return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeypadEqualSignAS400:   return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardInternational1: return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardInternational2: return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardInternational3: return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardInternational4: return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardInternational5: return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardInternational6: return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardInternational7: return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardInternational8: return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardInternational9: return sf::Keyboard::Scan::Unknown;

        case kHIDUsage_KeyboardLANG1: return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardLANG2: return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardLANG3: return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardLANG4: return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardLANG5: return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardLANG6: return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardLANG7: return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardLANG8: return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardLANG9: return sf::Keyboard::Scan::Unknown;

        case kHIDUsage_KeyboardAlternateErase:    return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardSysReqOrAttention: return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardCancel:            return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardClear:             return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardPrior:             return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardReturn:            return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardSeparator:         return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardOut:               return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardOper:              return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardClearOrAgain:      return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardCrSelOrProps:      return sf::Keyboard::Scan::Unknown;
        case kHIDUsage_KeyboardExSel:             return sf::Keyboard::Scan::Unknown;

            /* 0xa5-0xdf Reserved */

        case kHIDUsage_KeyboardLeftControl:  return sf::Keyboard::Scan::LControl;
        case kHIDUsage_KeyboardLeftShift:    return sf::Keyboard::Scan::LShift;
        case kHIDUsage_KeyboardLeftAlt:      return sf::Keyboard::Scan::LAlt;
        case kHIDUsage_KeyboardLeftGUI:      return sf::Keyboard::Scan::LSystem;
        case kHIDUsage_KeyboardRightControl: return sf::Keyboard::Scan::RControl;
        case kHIDUsage_KeyboardRightShift:   return sf::Keyboard::Scan::RShift;
        case kHIDUsage_KeyboardRightAlt:     return sf::Keyboard::Scan::RAlt;
        case kHIDUsage_KeyboardRightGUI:     return sf::Keyboard::Scan::RSystem;

            /* 0xe8-0xffff Reserved */

        case kHIDUsage_Keyboard_Reserved: return sf::Keyboard::Scan::Unknown;
        default:                          return sf::Keyboard::Scan::Unknown;
    }
}


////////////////////////////////////////////////////////
Keyboard::Key HIDInputManager::localizedKey(UniChar ch)
{
    // Apple is using the private range 0xE000 - 0xF8FF for special keys.
    // Otherwise one can refer to the usual Unicode table.
    switch (ch)
    {
        case 0x41: return sf::Keyboard::A; // uppercase A
        case 0x42: return sf::Keyboard::B;
        case 0x43: return sf::Keyboard::C;
        case 0x44: return sf::Keyboard::D;
        case 0x45: return sf::Keyboard::E;
        case 0x46: return sf::Keyboard::F;
        case 0x47: return sf::Keyboard::G;
        case 0x48: return sf::Keyboard::H;
        case 0x49: return sf::Keyboard::I;
        case 0x4a: return sf::Keyboard::J;
        case 0x4b: return sf::Keyboard::K;
        case 0x4c: return sf::Keyboard::L;
        case 0x4d: return sf::Keyboard::M;
        case 0x4e: return sf::Keyboard::N;
        case 0x4f: return sf::Keyboard::O;
        case 0x50: return sf::Keyboard::P;
        case 0x51: return sf::Keyboard::Q;
        case 0x52: return sf::Keyboard::R;
        case 0x53: return sf::Keyboard::S;
        case 0x54: return sf::Keyboard::T;
        case 0x55: return sf::Keyboard::U;
        case 0x56: return sf::Keyboard::V;
        case 0x57: return sf::Keyboard::W;
        case 0x58: return sf::Keyboard::X;
        case 0x59: return sf::Keyboard::Y;

        case 0x5a: return sf::Keyboard::Z; // lowercase A
        case 0x61: return sf::Keyboard::A;
        case 0x62: return sf::Keyboard::B;
        case 0x63: return sf::Keyboard::C;
        case 0x64: return sf::Keyboard::D;
        case 0x65: return sf::Keyboard::E;
        case 0x66: return sf::Keyboard::F;
        case 0x67: return sf::Keyboard::G;
        case 0x68: return sf::Keyboard::H;
        case 0x69: return sf::Keyboard::I;
        case 0x6a: return sf::Keyboard::J;
        case 0x6b: return sf::Keyboard::K;
        case 0x6c: return sf::Keyboard::L;
        case 0x6d: return sf::Keyboard::M;
        case 0x6e: return sf::Keyboard::N;
        case 0x6f: return sf::Keyboard::O;
        case 0x70: return sf::Keyboard::P;
        case 0x71: return sf::Keyboard::Q;
        case 0x72: return sf::Keyboard::R;
        case 0x73: return sf::Keyboard::S;
        case 0x74: return sf::Keyboard::T;
        case 0x75: return sf::Keyboard::U;
        case 0x76: return sf::Keyboard::V;
        case 0x77: return sf::Keyboard::W;
        case 0x78: return sf::Keyboard::X;
        case 0x79: return sf::Keyboard::Y;
        case 0x7a: return sf::Keyboard::Z;

        case 0x30: return sf::Keyboard::Num0;
        case 0x31: return sf::Keyboard::Num1;
        case 0x32: return sf::Keyboard::Num2;
        case 0x33: return sf::Keyboard::Num3;
        case 0x34: return sf::Keyboard::Num4;
        case 0x35: return sf::Keyboard::Num5;
        case 0x36: return sf::Keyboard::Num6;
        case 0x37: return sf::Keyboard::Num7;
        case 0x38: return sf::Keyboard::Num8;
        case 0x39: return sf::Keyboard::Num9;

        case 0x1b: return sf::Keyboard::Escape;

        /* Those are not represented using Unicode.
         * case 0x: return sf::Keyboard::LControl;
         * case 0x: return sf::Keyboard::LShift;
         * case 0x: return sf::Keyboard::LAlt;
         * case 0x: return sf::Keyboard::LSystem;
         * case 0x: return sf::Keyboard::RControl;
         * case 0x: return sf::Keyboard::RShift;
         * case 0x: return sf::Keyboard::RAlt;
         * case 0x: return sf::Keyboard::RSystem;
         */

        case NSMenuFunctionKey: return sf::Keyboard::Menu;

        case 0x5b:   return sf::Keyboard::LBracket;
        case 0x5d:   return sf::Keyboard::RBracket;
        case 0x3b:   return sf::Keyboard::SemiColon;
        case 0x2c:   return sf::Keyboard::Comma;
        case 0x2e:   return sf::Keyboard::Period;
        case 0x27:   return sf::Keyboard::Quote;
        case 0x2044: return sf::Keyboard::Slash; // 0x2f is Divide
        case 0x5c:   return sf::Keyboard::BackSlash;
        case 0x7e:   return sf::Keyboard::Tilde;
        case 0x3d:   return sf::Keyboard::Equal;
        case 0x2013: return sf::Keyboard::Dash; // 0x2d is Subtract
        case 0x20:   return sf::Keyboard::Space;
        case 0x0d:   return sf::Keyboard::Return;
        case 0x7f:   return sf::Keyboard::BackSpace; // NOTE backspace & delete are swapped on Mac.
        case 0x09:   return sf::Keyboard::Tab;

        case NSPageUpFunctionKey:   return sf::Keyboard::PageUp;
        case NSPageDownFunctionKey: return sf::Keyboard::PageDown;
        case NSEndFunctionKey:      return sf::Keyboard::End;
        case NSHomeFunctionKey:     return sf::Keyboard::Home;
        case NSInsertFunctionKey:   return sf::Keyboard::Insert;
        case NSDeleteFunctionKey:   return sf::Keyboard::Delete;

        case 0x2b: return sf::Keyboard::Add;
        case 0x2d: return sf::Keyboard::Subtract;
        case 0x2a: return sf::Keyboard::Multiply;
        case 0x2f: return sf::Keyboard::Divide;

        case NSLeftArrowFunctionKey:  return sf::Keyboard::Left;
        case NSRightArrowFunctionKey: return sf::Keyboard::Right;
        case NSUpArrowFunctionKey:    return sf::Keyboard::Up;
        case NSDownArrowFunctionKey:  return sf::Keyboard::Down;

        /* Those are not represented using Unicode.
         * case 0x: return sf::Keyboard::Numpad0;
         * case 0x: return sf::Keyboard::Numpad1;
         * case 0x: return sf::Keyboard::Numpad2;
         * case 0x: return sf::Keyboard::Numpad3;
         * case 0x: return sf::Keyboard::Numpad4;
         * case 0x: return sf::Keyboard::Numpad5;
         * case 0x: return sf::Keyboard::Numpad6;
         * case 0x: return sf::Keyboard::Numpad7;
         * case 0x: return sf::Keyboard::Numpad8;
         * case 0x: return sf::Keyboard::Numpad9;
         */

        case NSF1FunctionKey:  return sf::Keyboard::F1;
        case NSF2FunctionKey:  return sf::Keyboard::F2;
        case NSF3FunctionKey:  return sf::Keyboard::F3;
        case NSF4FunctionKey:  return sf::Keyboard::F4;
        case NSF5FunctionKey:  return sf::Keyboard::F5;
        case NSF6FunctionKey:  return sf::Keyboard::F6;
        case NSF7FunctionKey:  return sf::Keyboard::F7;
        case NSF8FunctionKey:  return sf::Keyboard::F8;
        case NSF9FunctionKey:  return sf::Keyboard::F9;
        case NSF10FunctionKey: return sf::Keyboard::F10;
        case NSF11FunctionKey: return sf::Keyboard::F11;
        case NSF12FunctionKey: return sf::Keyboard::F12;
        case NSF13FunctionKey: return sf::Keyboard::F13;
        case NSF14FunctionKey: return sf::Keyboard::F14;
        case NSF15FunctionKey: return sf::Keyboard::F15;

        case NSPauseFunctionKey:  return sf::Keyboard::Pause;

        default:  return sf::Keyboard::Unknown;
    }
}


////////////////////////////////////////////////////////
UniChar HIDInputManager::toUnicode(Keyboard::Key key)
{
    switch (key)
    {
        case sf::Keyboard::A: return 0x41;
        case sf::Keyboard::B: return 0x42;
        case sf::Keyboard::C: return 0x43;
        case sf::Keyboard::D: return 0x44;
        case sf::Keyboard::E: return 0x45;
        case sf::Keyboard::F: return 0x46;
        case sf::Keyboard::G: return 0x47;
        case sf::Keyboard::H: return 0x48;
        case sf::Keyboard::I: return 0x49;
        case sf::Keyboard::J: return 0x4a;
        case sf::Keyboard::K: return 0x4b;
        case sf::Keyboard::L: return 0x4c;
        case sf::Keyboard::M: return 0x4d;
        case sf::Keyboard::N: return 0x4e;
        case sf::Keyboard::O: return 0x4f;
        case sf::Keyboard::P: return 0x50;
        case sf::Keyboard::Q: return 0x51;
        case sf::Keyboard::R: return 0x52;
        case sf::Keyboard::S: return 0x53;
        case sf::Keyboard::T: return 0x54;
        case sf::Keyboard::U: return 0x55;
        case sf::Keyboard::V: return 0x56;
        case sf::Keyboard::W: return 0x57;
        case sf::Keyboard::X: return 0x58;
        case sf::Keyboard::Y: return 0x59;
        case sf::Keyboard::Z: return 0x5a;

        case sf::Keyboard::Num0: return 0x30;
        case sf::Keyboard::Num1: return 0x31;
        case sf::Keyboard::Num2: return 0x32;
        case sf::Keyboard::Num3: return 0x33;
        case sf::Keyboard::Num4: return 0x34;
        case sf::Keyboard::Num5: return 0x35;
        case sf::Keyboard::Num6: return 0x36;
        case sf::Keyboard::Num7: return 0x37;
        case sf::Keyboard::Num8: return 0x38;
        case sf::Keyboard::Num9: return 0x39;

        case sf::Keyboard::Escape:  return 0x1b;

        /* Not representable as Unicode:
         * case sf::Keyboard::LControl: return 0x;
         * case sf::Keyboard::LShift:   return 0x;
         * case sf::Keyboard::LAlt:     return 0x;
         * case sf::Keyboard::LSystem:  return 0x;
         * case sf::Keyboard::RControl: return 0x;
         * case sf::Keyboard::RShift:   return 0x;
         * case sf::Keyboard::RAlt:     return 0x;
         * case sf::Keyboard::RSystem:  return 0x;
         */

        case sf::Keyboard::Menu:  return NSMenuFunctionKey;

        case sf::Keyboard::LBracket:  return 0x5b;
        case sf::Keyboard::RBracket:  return 0x5d;
        case sf::Keyboard::SemiColon: return 0x3b;
        case sf::Keyboard::Comma:     return 0x2c;
        case sf::Keyboard::Period:    return 0x2e;
        case sf::Keyboard::Quote:     return 0x27;
        case sf::Keyboard::Slash:     return 0x2044; // 0x2f is Divide
        case sf::Keyboard::BackSlash: return 0x5c;
        case sf::Keyboard::Tilde:     return 0x7e;
        case sf::Keyboard::Equal:     return 0x3d;
        case sf::Keyboard::Dash:      return 0x2013; // 0x2d is Subtract
        case sf::Keyboard::Space:     return 0x20;
        case sf::Keyboard::Return:    return 0x0d;
        case sf::Keyboard::BackSpace: return 0x7f; // NOTE backspace & delete are swapped on Mac.
        case sf::Keyboard::Tab:       return 0x09;

        case sf::Keyboard::PageUp:   return NSPageUpFunctionKey;
        case sf::Keyboard::PageDown: return NSPageDownFunctionKey;
        case sf::Keyboard::End:      return NSEndFunctionKey;
        case sf::Keyboard::Home:     return NSHomeFunctionKey;
        case sf::Keyboard::Insert:   return NSInsertFunctionKey;
        case sf::Keyboard::Delete:   return NSDeleteFunctionKey;

        case sf::Keyboard::Add:      return 0x2b;
        case sf::Keyboard::Subtract: return 0x2d;
        case sf::Keyboard::Multiply: return 0x2a;
        case sf::Keyboard::Divide:   return 0x2f;

        case sf::Keyboard::Left:  return NSLeftArrowFunctionKey;
        case sf::Keyboard::Right: return NSRightArrowFunctionKey;
        case sf::Keyboard::Up:    return NSUpArrowFunctionKey;
        case sf::Keyboard::Down:  return NSDownArrowFunctionKey;

        /* Those are not represented using Unicode.
         * case sf::Keyboard::Numpad0: return 0x;
         * case sf::Keyboard::Numpad1: return 0x;
         * case sf::Keyboard::Numpad2: return 0x;
         * case sf::Keyboard::Numpad3: return 0x;
         * case sf::Keyboard::Numpad4: return 0x;
         * case sf::Keyboard::Numpad5: return 0x;
         * case sf::Keyboard::Numpad6: return 0x;
         * case sf::Keyboard::Numpad7: return 0x;
         * case sf::Keyboard::Numpad8: return 0x;
         * case sf::Keyboard::Numpad9: return 0x;
         */

        case sf::Keyboard::F1:  return NSF1FunctionKey;
        case sf::Keyboard::F2:  return NSF2FunctionKey;
        case sf::Keyboard::F3:  return NSF3FunctionKey;
        case sf::Keyboard::F4:  return NSF4FunctionKey;
        case sf::Keyboard::F5:  return NSF5FunctionKey;
        case sf::Keyboard::F6:  return NSF6FunctionKey;
        case sf::Keyboard::F7:  return NSF7FunctionKey;
        case sf::Keyboard::F8:  return NSF8FunctionKey;
        case sf::Keyboard::F9:  return NSF9FunctionKey;
        case sf::Keyboard::F10: return NSF10FunctionKey;
        case sf::Keyboard::F11: return NSF11FunctionKey;
        case sf::Keyboard::F12: return NSF12FunctionKey;
        case sf::Keyboard::F13: return NSF13FunctionKey;
        case sf::Keyboard::F14: return NSF14FunctionKey;
        case sf::Keyboard::F15: return NSF15FunctionKey;

        case sf::Keyboard::Pause: return NSPauseFunctionKey;

        default: return 0x00;
    }
}


////////////////////////////////////////////////////////
Keyboard::Scancode HIDInputManager::nonLocalizedKey(UniChar virtualKeycode)
{
    // See Chapter 2, esp. Figure 2-10 of
    // https://developer.apple.com/legacy/library/documentation/mac/pdf/MacintoshToolboxEssentials.pdf
    // Additional virtual codes come from
    // /System/Library/Frameworks/Carbon.framework/Versions/A/Frameworks/HIToolbox.framework/Versions/A/Headers/Events.h
    switch (virtualKeycode)
    {
        case 0x00: return sf::Keyboard::Scan::A;
        case 0x0b: return sf::Keyboard::Scan::B;
        case 0x08: return sf::Keyboard::Scan::C;
        case 0x02: return sf::Keyboard::Scan::D;
        case 0x0e: return sf::Keyboard::Scan::E;
        case 0x03: return sf::Keyboard::Scan::F;
        case 0x05: return sf::Keyboard::Scan::G;
        case 0x04: return sf::Keyboard::Scan::H;
        case 0x22: return sf::Keyboard::Scan::I;
        case 0x26: return sf::Keyboard::Scan::J;
        case 0x28: return sf::Keyboard::Scan::K;
        case 0x25: return sf::Keyboard::Scan::L;
        case 0x2e: return sf::Keyboard::Scan::M;
        case 0x2d: return sf::Keyboard::Scan::N;
        case 0x1f: return sf::Keyboard::Scan::O;
        case 0x23: return sf::Keyboard::Scan::P;
        case 0x0c: return sf::Keyboard::Scan::Q;
        case 0x0f: return sf::Keyboard::Scan::R;
        case 0x01: return sf::Keyboard::Scan::S;
        case 0x11: return sf::Keyboard::Scan::T;
        case 0x20: return sf::Keyboard::Scan::U;
        case 0x09: return sf::Keyboard::Scan::V;
        case 0x0d: return sf::Keyboard::Scan::W;
        case 0x07: return sf::Keyboard::Scan::X;
        case 0x10: return sf::Keyboard::Scan::Y;
        case 0x06: return sf::Keyboard::Scan::Z;

        case 0x12: return sf::Keyboard::Scan::Num1;
        case 0x13: return sf::Keyboard::Scan::Num2;
        case 0x14: return sf::Keyboard::Scan::Num3;
        case 0x15: return sf::Keyboard::Scan::Num4;
        case 0x17: return sf::Keyboard::Scan::Num5;
        case 0x16: return sf::Keyboard::Scan::Num6;
        case 0x1a: return sf::Keyboard::Scan::Num7;
        case 0x1c: return sf::Keyboard::Scan::Num8;
        case 0x19: return sf::Keyboard::Scan::Num9;
        case 0x1d: return sf::Keyboard::Scan::Num0;

        case 0x24: return sf::Keyboard::Scan::Enter;
        case 0x35: return sf::Keyboard::Scan::Escape;
        case 0x33: return sf::Keyboard::Scan::Backspace;
        case 0x30: return sf::Keyboard::Scan::Tab;
        case 0x31: return sf::Keyboard::Scan::Space;

        case 0x18: return sf::Keyboard::Scan::Equal;
        case 0x21: return sf::Keyboard::Scan::LBracket;
        case 0x1e: return sf::Keyboard::Scan::RBracket;
        case 0x2a: return sf::Keyboard::Scan::Backslash;
        case 0x1b: return sf::Keyboard::Scan::Hyphen;
        case 0x29: return sf::Keyboard::Scan::Semicolon;
        case 0x27: return sf::Keyboard::Scan::Apostrophe;
        case 0x32: return sf::Keyboard::Scan::Grave;
        case 0x2b: return sf::Keyboard::Scan::Comma;
        case 0x2f: return sf::Keyboard::Scan::Period;
        case 0x2c: return sf::Keyboard::Scan::Slash;

        case 0x7a: return sf::Keyboard::Scan::F1;
        case 0x78: return sf::Keyboard::Scan::F2;
        case 0x63: return sf::Keyboard::Scan::F3;
        case 0x76: return sf::Keyboard::Scan::F4;
        case 0x60: return sf::Keyboard::Scan::F5;
        case 0x61: return sf::Keyboard::Scan::F6;
        case 0x62: return sf::Keyboard::Scan::F7;
        case 0x64: return sf::Keyboard::Scan::F8;
        case 0x65: return sf::Keyboard::Scan::F9;
        case 0x6d: return sf::Keyboard::Scan::F10;
        case 0x67: return sf::Keyboard::Scan::F11;
        case 0x6f: return sf::Keyboard::Scan::F12;
        case 0x69: return sf::Keyboard::Scan::F13;
        case 0x6b: return sf::Keyboard::Scan::F14;
        case 0x71: return sf::Keyboard::Scan::F15;

        case 0x6A: return sf::Keyboard::Scan::F16;
        case 0x40: return sf::Keyboard::Scan::F17;
        case 0x4F: return sf::Keyboard::Scan::F18;
        case 0x50: return sf::Keyboard::Scan::F19;
        case 0x5A: return sf::Keyboard::Scan::F20;
        
        /* TODO Those are missing:
         * case 0x: return sf::Keyboard::Scan::F21;
         * case 0x: return sf::Keyboard::Scan::F22;
         * case 0x: return sf::Keyboard::Scan::F23;
         * case 0x: return sf::Keyboard::Scan::F24;
         */

        case 0x39: return sf::Keyboard::Scan::CapsLock;

        /* TODO Those are missing:
         * case 0x: return sf::Keyboard::Scan::PrintScreen;
         * case 0x: return sf::Keyboard::Scan::ScrollLock;
         * case 0x: return sf::Keyboard::Scan::Pause;
         */

        case 0x72: return sf::Keyboard::Scan::Insert;
        case 0x73: return sf::Keyboard::Scan::Home;
        case 0x74: return sf::Keyboard::Scan::PageUp;
        case 0x75: return sf::Keyboard::Scan::Delete;
        case 0x77: return sf::Keyboard::Scan::End;
        case 0x79: return sf::Keyboard::Scan::PageDown;

        case 0x7c: return sf::Keyboard::Scan::Right;
        case 0x7b: return sf::Keyboard::Scan::Left;
        case 0x7d: return sf::Keyboard::Scan::Down;
        case 0x7e: return sf::Keyboard::Scan::Up;

        case 0x47: return sf::Keyboard::Scan::NumLock;
        case 0x4b: return sf::Keyboard::Scan::NumpadDivide;
        case 0x43: return sf::Keyboard::Scan::NumpadMultiply;
        case 0x4e: return sf::Keyboard::Scan::NumpadMinus;
        case 0x45: return sf::Keyboard::Scan::NumpadPlus;
        case 0x51: return sf::Keyboard::Scan::NumpadEqual;
        case 0x4c: return sf::Keyboard::Scan::NumpadEnter;
        case 0x41: return sf::Keyboard::Scan::NumpadDecimal;

        case 0x53: return sf::Keyboard::Scan::Numpad1;
        case 0x54: return sf::Keyboard::Scan::Numpad2;
        case 0x55: return sf::Keyboard::Scan::Numpad3;
        case 0x56: return sf::Keyboard::Scan::Numpad4;
        case 0x57: return sf::Keyboard::Scan::Numpad5;
        case 0x58: return sf::Keyboard::Scan::Numpad6;
        case 0x59: return sf::Keyboard::Scan::Numpad7;
        case 0x5b: return sf::Keyboard::Scan::Numpad8;
        case 0x5c: return sf::Keyboard::Scan::Numpad9;
        case 0x52: return sf::Keyboard::Scan::Numpad0;

        /* TODO Those are missing:
         * case 0x: return sf::Keyboard::Scan::NonUsBackslash;
         * case 0x: return sf::Keyboard::Scan::Application;
         * case 0x: return sf::Keyboard::Scan::Execute;
         * case 0x: return sf::Keyboard::Scan::Help;
         * case 0x: return sf::Keyboard::Scan::Menu;
         * case 0x: return sf::Keyboard::Scan::Select;
         * case 0x: return sf::Keyboard::Scan::Stop;
         * case 0x: return sf::Keyboard::Scan::Redo;
         * case 0x: return sf::Keyboard::Scan::Undo;
         * case 0x: return sf::Keyboard::Scan::Cut;
         * case 0x: return sf::Keyboard::Scan::Copy;
         * case 0x: return sf::Keyboard::Scan::Paste;
         * case 0x: return sf::Keyboard::Scan::Search;
         */

        case 0x4a: return sf::Keyboard::Scan::VolumeMute;
        case 0x48: return sf::Keyboard::Scan::VolumeUp;
        case 0x49: return sf::Keyboard::Scan::VolumeDown;

        /* NOTE Those are symmetric so we leave them out.
         *      Thankfully handled through modifiers and not virtual codes.
         * case 0x3b: return sf::Keyboard::Scan::LControl;
         * case 0x38: return sf::Keyboard::Scan::LShift;
         * case 0x3a: return sf::Keyboard::Scan::LAlt;
         * case 0x37: return sf::Keyboard::Scan::LSystem;
         * case 0x3b: return sf::Keyboard::Scan::RControl;
         * case 0x38: return sf::Keyboard::Scan::RShift;
         * case 0x3a: return sf::Keyboard::Scan::RAlt;
         * case 0x37: return sf::Keyboard::Scan::RSystem;
         */

        default: return sf::Keyboard::Scan::Unknown;
    }
}


////////////////////////////////////////////////////////
UInt8 HIDInputManager::scanToVirtualCode(Keyboard::Scancode code)
{
    switch (code)
    {
        case sf::Keyboard::Scan::A: return 0x00;
        case sf::Keyboard::Scan::B: return 0x0b;
        case sf::Keyboard::Scan::C: return 0x08;
        case sf::Keyboard::Scan::D: return 0x02;
        case sf::Keyboard::Scan::E: return 0x0e;
        case sf::Keyboard::Scan::F: return 0x03;
        case sf::Keyboard::Scan::G: return 0x05;
        case sf::Keyboard::Scan::H: return 0x04;
        case sf::Keyboard::Scan::I: return 0x22;
        case sf::Keyboard::Scan::J: return 0x26;
        case sf::Keyboard::Scan::K: return 0x28;
        case sf::Keyboard::Scan::L: return 0x25;
        case sf::Keyboard::Scan::M: return 0x2e;
        case sf::Keyboard::Scan::N: return 0x2d;
        case sf::Keyboard::Scan::O: return 0x1f;
        case sf::Keyboard::Scan::P: return 0x23;
        case sf::Keyboard::Scan::Q: return 0x0c;
        case sf::Keyboard::Scan::R: return 0x0f;
        case sf::Keyboard::Scan::S: return 0x01;
        case sf::Keyboard::Scan::T: return 0x11;
        case sf::Keyboard::Scan::U: return 0x20;
        case sf::Keyboard::Scan::V: return 0x09;
        case sf::Keyboard::Scan::W: return 0x0d;
        case sf::Keyboard::Scan::X: return 0x07;
        case sf::Keyboard::Scan::Y: return 0x10;
        case sf::Keyboard::Scan::Z: return 0x06;

        case sf::Keyboard::Scan::Num1: return 0x12;
        case sf::Keyboard::Scan::Num2: return 0x13;
        case sf::Keyboard::Scan::Num3: return 0x14;
        case sf::Keyboard::Scan::Num4: return 0x15;
        case sf::Keyboard::Scan::Num5: return 0x17;
        case sf::Keyboard::Scan::Num6: return 0x16;
        case sf::Keyboard::Scan::Num7: return 0x1a;
        case sf::Keyboard::Scan::Num8: return 0x1c;
        case sf::Keyboard::Scan::Num9: return 0x19;
        case sf::Keyboard::Scan::Num0: return 0x1d;

        case sf::Keyboard::Scan::Enter:     return 0x24;
        case sf::Keyboard::Scan::Escape:    return 0x35;
        case sf::Keyboard::Scan::Backspace: return 0x33;
        case sf::Keyboard::Scan::Tab:       return 0x30;
        case sf::Keyboard::Scan::Space:     return 0x31;

        case sf::Keyboard::Scan::Equal:       return 0x18;
        case sf::Keyboard::Scan::LBracket:    return 0x21;
        case sf::Keyboard::Scan::RBracket:    return 0x1e;
        case sf::Keyboard::Scan::Backslash:   return 0x2a;
        case sf::Keyboard::Scan::Hyphen:      return 0x1b;
        case sf::Keyboard::Scan::Semicolon:   return 0x29;
        case sf::Keyboard::Scan::Apostrophe:  return 0x27;
        case sf::Keyboard::Scan::Grave:       return 0x32;
        case sf::Keyboard::Scan::Comma:       return 0x2b;
        case sf::Keyboard::Scan::Period:      return 0x2f;
        case sf::Keyboard::Scan::Slash:       return 0x2c;

        case sf::Keyboard::Scan::F1:  return 0x7a;
        case sf::Keyboard::Scan::F2:  return 0x78;
        case sf::Keyboard::Scan::F3:  return 0x63;
        case sf::Keyboard::Scan::F4:  return 0x76;
        case sf::Keyboard::Scan::F5:  return 0x60;
        case sf::Keyboard::Scan::F6:  return 0x61;
        case sf::Keyboard::Scan::F7:  return 0x62;
        case sf::Keyboard::Scan::F8:  return 0x64;
        case sf::Keyboard::Scan::F9:  return 0x65;
        case sf::Keyboard::Scan::F10: return 0x6d;
        case sf::Keyboard::Scan::F11: return 0x67;
        case sf::Keyboard::Scan::F12: return 0x6f;
        case sf::Keyboard::Scan::F13: return 0x69;
        case sf::Keyboard::Scan::F14: return 0x6b;
        case sf::Keyboard::Scan::F15: return 0x71;

        case sf::Keyboard::Scan::CapsLock: return 0x39;

        /* TODO Those are missing:
         * case sf::Keyboard::Scan::PrintScreen: return 0;
         * case sf::Keyboard::Scan::ScrollLock:  return 0;
         * case sf::Keyboard::Scan::Pause:    return 0;
         */
        case sf::Keyboard::Scan::Insert:   return 0x72;
        case sf::Keyboard::Scan::Home:     return 0x73;
        case sf::Keyboard::Scan::PageUp:   return 0x74;
        case sf::Keyboard::Scan::Delete:   return 0x75;
        case sf::Keyboard::Scan::End:      return 0x77;
        case sf::Keyboard::Scan::PageDown: return 0x79;

        case sf::Keyboard::Scan::Right: return 0x7c;
        case sf::Keyboard::Scan::Left:  return 0x7b;
        case sf::Keyboard::Scan::Down:  return 0x7d;
        case sf::Keyboard::Scan::Up:    return 0x7e;

        case sf::Keyboard::Scan::NumLock:        return 0x47;
        case sf::Keyboard::Scan::NumpadDivide:   return 0x4b;
        case sf::Keyboard::Scan::NumpadMultiply: return 0x43;
        case sf::Keyboard::Scan::NumpadMinus:    return 0x4e;
        case sf::Keyboard::Scan::NumpadPlus:     return 0x45;
        case sf::Keyboard::Scan::NumpadEqual:    return 0x51;
        case sf::Keyboard::Scan::NumpadEnter:    return 0x4c;
        case sf::Keyboard::Scan::NumpadDecimal:  return 0x41;

        case sf::Keyboard::Scan::Numpad1: return 0x53;
        case sf::Keyboard::Scan::Numpad2: return 0x54;
        case sf::Keyboard::Scan::Numpad3: return 0x55;
        case sf::Keyboard::Scan::Numpad4: return 0x56;
        case sf::Keyboard::Scan::Numpad5: return 0x57;
        case sf::Keyboard::Scan::Numpad6: return 0x58;
        case sf::Keyboard::Scan::Numpad7: return 0x59;
        case sf::Keyboard::Scan::Numpad8: return 0x5b;
        case sf::Keyboard::Scan::Numpad9: return 0x5c;
        case sf::Keyboard::Scan::Numpad0: return 0x52;

        /* TODO Those are missing:
         * case sf::Keyboard::Scan::NonUsBackslash: return 0;
         * case sf::Keyboard::Scan::Application: return 0;
         * case sf::Keyboard::Scan::Execute:     return 0;
         * case sf::Keyboard::Scan::Help:        return 0;
         * case sf::Keyboard::Scan::Menu:        return 0;
         * case sf::Keyboard::Scan::Select:      return 0;
         * case sf::Keyboard::Scan::Stop:        return 0;
         * case sf::Keyboard::Scan::Redo:        return 0;
         * case sf::Keyboard::Scan::Undo:        return 0;
         * case sf::Keyboard::Scan::Cut:         return 0;
         * case sf::Keyboard::Scan::Copy:        return 0;
         * case sf::Keyboard::Scan::Paste:       return 0;
         * case sf::Keyboard::Scan::Search:      return 0;
         */

        case sf::Keyboard::Scan::VolumeMute: return 0x4a;
        case sf::Keyboard::Scan::VolumeUp:   return 0x48;
        case sf::Keyboard::Scan::VolumeDown: return 0x49;

        case sf::Keyboard::Scan::LControl: return 0x3b;
        case sf::Keyboard::Scan::LShift:   return 0x38;
        case sf::Keyboard::Scan::LAlt:     return 0x3a;
        case sf::Keyboard::Scan::LSystem:  return 0x37;
        case sf::Keyboard::Scan::RControl: return 0x3b;
        case sf::Keyboard::Scan::RShift:   return 0x38;
        case sf::Keyboard::Scan::RAlt:     return 0x3a;
        case sf::Keyboard::Scan::RSystem:  return 0x37;

        default: return 0x00;
    }
}


////////////////////////////////////////////////////////
Keyboard::Key HIDInputManager::localizedKeyFallback(Keyboard::Scancode code)
{
    switch (code)
    {
        case sf::Keyboard::Scan::Enter:      return sf::Keyboard::Return;
        case sf::Keyboard::Scan::Escape:     return sf::Keyboard::Escape;
        case sf::Keyboard::Scan::Backspace:  return sf::Keyboard::BackSpace;
        case sf::Keyboard::Scan::Tab:        return sf::Keyboard::Tab;
        case sf::Keyboard::Scan::Space:      return sf::Keyboard::Space;

        case sf::Keyboard::Scan::F1:   return sf::Keyboard::F1;
        case sf::Keyboard::Scan::F2:   return sf::Keyboard::F2;
        case sf::Keyboard::Scan::F3:   return sf::Keyboard::F3;
        case sf::Keyboard::Scan::F4:   return sf::Keyboard::F4;
        case sf::Keyboard::Scan::F5:   return sf::Keyboard::F5;
        case sf::Keyboard::Scan::F6:   return sf::Keyboard::F6;
        case sf::Keyboard::Scan::F7:   return sf::Keyboard::F7;
        case sf::Keyboard::Scan::F8:   return sf::Keyboard::F8;
        case sf::Keyboard::Scan::F9:   return sf::Keyboard::F9;
        case sf::Keyboard::Scan::F10:  return sf::Keyboard::F10;
        case sf::Keyboard::Scan::F11:  return sf::Keyboard::F11;
        case sf::Keyboard::Scan::F12:  return sf::Keyboard::F12;
        case sf::Keyboard::Scan::F13:  return sf::Keyboard::F13;
        case sf::Keyboard::Scan::F14:  return sf::Keyboard::F14;
        case sf::Keyboard::Scan::F15:  return sf::Keyboard::F15;

        case sf::Keyboard::Scan::Pause:    return sf::Keyboard::Pause;
        case sf::Keyboard::Scan::Insert:   return sf::Keyboard::Insert;
        case sf::Keyboard::Scan::Home:     return sf::Keyboard::Home;
        case sf::Keyboard::Scan::PageUp:   return sf::Keyboard::PageUp;
        case sf::Keyboard::Scan::Delete:   return sf::Keyboard::Delete;
        case sf::Keyboard::Scan::End:      return sf::Keyboard::End;
        case sf::Keyboard::Scan::PageDown: return sf::Keyboard::PageDown;

        case sf::Keyboard::Scan::Right:  return sf::Keyboard::Right;
        case sf::Keyboard::Scan::Left:   return sf::Keyboard::Left;
        case sf::Keyboard::Scan::Down:   return sf::Keyboard::Down;
        case sf::Keyboard::Scan::Up:     return sf::Keyboard::Up;

        case sf::Keyboard::Scan::NumpadDivide:   return sf::Keyboard::Divide;
        case sf::Keyboard::Scan::NumpadMultiply: return sf::Keyboard::Multiply;
        case sf::Keyboard::Scan::NumpadMinus:    return sf::Keyboard::Subtract;
        case sf::Keyboard::Scan::NumpadPlus:     return sf::Keyboard::Add;

        case sf::Keyboard::Scan::Numpad0:  return sf::Keyboard::Numpad1;
        case sf::Keyboard::Scan::Numpad1:  return sf::Keyboard::Numpad2;
        case sf::Keyboard::Scan::Numpad2:  return sf::Keyboard::Numpad3;
        case sf::Keyboard::Scan::Numpad3:  return sf::Keyboard::Numpad4;
        case sf::Keyboard::Scan::Numpad4:  return sf::Keyboard::Numpad5;
        case sf::Keyboard::Scan::Numpad5:  return sf::Keyboard::Numpad6;
        case sf::Keyboard::Scan::Numpad6:  return sf::Keyboard::Numpad7;
        case sf::Keyboard::Scan::Numpad7:  return sf::Keyboard::Numpad8;
        case sf::Keyboard::Scan::Numpad8:  return sf::Keyboard::Numpad9;
        case sf::Keyboard::Scan::Numpad9:  return sf::Keyboard::Numpad0;

        case sf::Keyboard::Scan::LControl:  return sf::Keyboard::LControl;
        case sf::Keyboard::Scan::LShift:    return sf::Keyboard::LShift;
        case sf::Keyboard::Scan::LAlt:      return sf::Keyboard::LAlt;
        case sf::Keyboard::Scan::LSystem:   return sf::Keyboard::LSystem;
        case sf::Keyboard::Scan::RControl:  return sf::Keyboard::RControl;
        case sf::Keyboard::Scan::RShift:    return sf::Keyboard::RShift;
        case sf::Keyboard::Scan::RAlt:      return sf::Keyboard::RAlt;
        case sf::Keyboard::Scan::RSystem:   return sf::Keyboard::RSystem;

        default: return sf::Keyboard::Unknown;
    }
}


} // namespace priv

} // namespace sf

