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
    return (code != Keyboard::ScanUnknown) && isPressed(m_keys[code]);
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
        case sf::Keyboard::ScanEnter:     return "Enter";
        case sf::Keyboard::ScanEscape:    return "Escape";
        case sf::Keyboard::ScanBackspace: return "Backspace";
        case sf::Keyboard::ScanTab:       return "Tab";
        case sf::Keyboard::ScanSpace:     return "Space";

        case sf::Keyboard::ScanF1:  return "F1";
        case sf::Keyboard::ScanF2:  return "F2";
        case sf::Keyboard::ScanF3:  return "F3";
        case sf::Keyboard::ScanF4:  return "F4";
        case sf::Keyboard::ScanF5:  return "F5";
        case sf::Keyboard::ScanF6:  return "F6";
        case sf::Keyboard::ScanF7:  return "F7";
        case sf::Keyboard::ScanF8:  return "F8";
        case sf::Keyboard::ScanF9:  return "F9";
        case sf::Keyboard::ScanF10: return "F10";
        case sf::Keyboard::ScanF11: return "F11";
        case sf::Keyboard::ScanF12: return "F12";
        case sf::Keyboard::ScanF13: return "F13";
        case sf::Keyboard::ScanF14: return "F14";
        case sf::Keyboard::ScanF15: return "F15";
        case sf::Keyboard::ScanF16: return "F16";
        case sf::Keyboard::ScanF17: return "F17";
        case sf::Keyboard::ScanF18: return "F18";
        case sf::Keyboard::ScanF19: return "F19";
        case sf::Keyboard::ScanF20: return "F20";
        case sf::Keyboard::ScanF21: return "F21";
        case sf::Keyboard::ScanF22: return "F22";
        case sf::Keyboard::ScanF23: return "F23";
        case sf::Keyboard::ScanF24: return "F24";

        case sf::Keyboard::ScanCapsLock:    return "CapsLock";
        case sf::Keyboard::ScanPrintScreen: return "PrintScreen";
        case sf::Keyboard::ScanScrollLock:  return "ScrollLock";

        case sf::Keyboard::ScanPause:    return "Pause";
        case sf::Keyboard::ScanInsert:   return "Insert";
        case sf::Keyboard::ScanHome:     return "Home";
        case sf::Keyboard::ScanPageUp:   return "PageUp";
        case sf::Keyboard::ScanDelete:   return "Delete";
        case sf::Keyboard::ScanEnd:      return "End";
        case sf::Keyboard::ScanPageDown: return "PageDown";

        case sf::Keyboard::ScanLeft:  return "Left Arrow";
        case sf::Keyboard::ScanRight: return "Right Arrow";
        case sf::Keyboard::ScanDown:  return "Down Arrow";
        case sf::Keyboard::ScanUp:    return "Up Arrow";

        case sf::Keyboard::ScanNumLock:        return "NumLock";
        case sf::Keyboard::ScanNumpadDivide:   return "Divide (Numpad)";
        case sf::Keyboard::ScanNumpadMultiply: return "Multiply (Numpad)";
        case sf::Keyboard::ScanNumpadMinus:    return "Minus (Numpad)";
        case sf::Keyboard::ScanNumpadPlus:     return "Plus (Numpad)";
        case sf::Keyboard::ScanNumpadEqual:    return "Equal (Numpad)";
        case sf::Keyboard::ScanNumpadEnter:    return "Enter (Numpad)";
        case sf::Keyboard::ScanNumpadDecimal:  return "Decimal (Numpad)";

        case sf::Keyboard::ScanNumpad0: return "0 (Numpad)";
        case sf::Keyboard::ScanNumpad1: return "1 (Numpad)";
        case sf::Keyboard::ScanNumpad2: return "2 (Numpad)";
        case sf::Keyboard::ScanNumpad3: return "3 (Numpad)";
        case sf::Keyboard::ScanNumpad4: return "4 (Numpad)";
        case sf::Keyboard::ScanNumpad5: return "5 (Numpad)";
        case sf::Keyboard::ScanNumpad6: return "6 (Numpad)";
        case sf::Keyboard::ScanNumpad7: return "7 (Numpad)";
        case sf::Keyboard::ScanNumpad8: return "8 (Numpad)";
        case sf::Keyboard::ScanNumpad9: return "9 (Numpad)";

        case sf::Keyboard::ScanApplication: return "Application";
        case sf::Keyboard::ScanExecute:     return "Execute";
        case sf::Keyboard::ScanHelp:        return "Help";
        case sf::Keyboard::ScanMenu:        return "Menu";
        case sf::Keyboard::ScanSelect:      return "Select";
        case sf::Keyboard::ScanStop:        return "Stop";
        case sf::Keyboard::ScanRedo:        return "Redo";
        case sf::Keyboard::ScanUndo:        return "Undo";
        case sf::Keyboard::ScanCut:         return "Cut";
        case sf::Keyboard::ScanCopy:        return "Copy";
        case sf::Keyboard::ScanPaste:       return "Paste";
        case sf::Keyboard::ScanSearch:      return "Search";
        case sf::Keyboard::ScanVolumeMute:  return "Volume Mute";
        case sf::Keyboard::ScanVolumeUp:    return "Volume Up";
        case sf::Keyboard::ScanVolumeDown:  return "Volume Down";

        case sf::Keyboard::ScanLControl: return "Left Control";
        case sf::Keyboard::ScanLShift:   return "Left Shift";
        case sf::Keyboard::ScanLAlt:     return "Left Alt";
        case sf::Keyboard::ScanLSystem:  return "Left Command";
        case sf::Keyboard::ScanRControl: return "Right Control";
        case sf::Keyboard::ScanRShift:   return "Right Shift";
        case sf::Keyboard::ScanRAlt:     return "Right Alt";
        case sf::Keyboard::ScanRSystem:  return "Right Command";

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
    if (code != Keyboard::ScanUnknown)
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
        m_keyToScancodeMapping[i] = Keyboard::ScanUnknown;
    for (int i = 0; i < Keyboard::ScancodeCount; ++i)
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
    for (int i = 0; i < Keyboard::ScancodeCount; ++i)
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
        case kHIDUsage_KeyboardErrorRollOver:  return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardPOSTFail:       return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardErrorUndefined: return sf::Keyboard::ScanUnknown;

        case kHIDUsage_KeyboardA: return sf::Keyboard::ScanA;
        case kHIDUsage_KeyboardB: return sf::Keyboard::ScanB;
        case kHIDUsage_KeyboardC: return sf::Keyboard::ScanC;
        case kHIDUsage_KeyboardD: return sf::Keyboard::ScanD;
        case kHIDUsage_KeyboardE: return sf::Keyboard::ScanE;
        case kHIDUsage_KeyboardF: return sf::Keyboard::ScanF;
        case kHIDUsage_KeyboardG: return sf::Keyboard::ScanG;
        case kHIDUsage_KeyboardH: return sf::Keyboard::ScanH;
        case kHIDUsage_KeyboardI: return sf::Keyboard::ScanI;
        case kHIDUsage_KeyboardJ: return sf::Keyboard::ScanJ;
        case kHIDUsage_KeyboardK: return sf::Keyboard::ScanK;
        case kHIDUsage_KeyboardL: return sf::Keyboard::ScanL;
        case kHIDUsage_KeyboardM: return sf::Keyboard::ScanM;
        case kHIDUsage_KeyboardN: return sf::Keyboard::ScanN;
        case kHIDUsage_KeyboardO: return sf::Keyboard::ScanO;
        case kHIDUsage_KeyboardP: return sf::Keyboard::ScanP;
        case kHIDUsage_KeyboardQ: return sf::Keyboard::ScanQ;
        case kHIDUsage_KeyboardR: return sf::Keyboard::ScanR;
        case kHIDUsage_KeyboardS: return sf::Keyboard::ScanS;
        case kHIDUsage_KeyboardT: return sf::Keyboard::ScanT;
        case kHIDUsage_KeyboardU: return sf::Keyboard::ScanU;
        case kHIDUsage_KeyboardV: return sf::Keyboard::ScanV;
        case kHIDUsage_KeyboardW: return sf::Keyboard::ScanW;
        case kHIDUsage_KeyboardX: return sf::Keyboard::ScanX;
        case kHIDUsage_KeyboardY: return sf::Keyboard::ScanY;
        case kHIDUsage_KeyboardZ: return sf::Keyboard::ScanZ;

        case kHIDUsage_Keyboard1: return sf::Keyboard::ScanNum1;
        case kHIDUsage_Keyboard2: return sf::Keyboard::ScanNum2;
        case kHIDUsage_Keyboard3: return sf::Keyboard::ScanNum3;
        case kHIDUsage_Keyboard4: return sf::Keyboard::ScanNum4;
        case kHIDUsage_Keyboard5: return sf::Keyboard::ScanNum5;
        case kHIDUsage_Keyboard6: return sf::Keyboard::ScanNum6;
        case kHIDUsage_Keyboard7: return sf::Keyboard::ScanNum7;
        case kHIDUsage_Keyboard8: return sf::Keyboard::ScanNum8;
        case kHIDUsage_Keyboard9: return sf::Keyboard::ScanNum9;
        case kHIDUsage_Keyboard0: return sf::Keyboard::ScanNum0;

        case kHIDUsage_KeyboardReturnOrEnter:       return sf::Keyboard::ScanEnter;
        case kHIDUsage_KeyboardEscape:              return sf::Keyboard::ScanEscape;
        case kHIDUsage_KeyboardDeleteOrBackspace:   return sf::Keyboard::ScanBackspace;
        case kHIDUsage_KeyboardTab:                 return sf::Keyboard::ScanTab;
        case kHIDUsage_KeyboardSpacebar:            return sf::Keyboard::ScanSpace;
        case kHIDUsage_KeyboardHyphen:              return sf::Keyboard::ScanHyphen;
        case kHIDUsage_KeyboardEqualSign:           return sf::Keyboard::ScanEqual;
        case kHIDUsage_KeyboardOpenBracket:         return sf::Keyboard::ScanLBracket;
        case kHIDUsage_KeyboardCloseBracket:        return sf::Keyboard::ScanRBracket;
        case kHIDUsage_KeyboardBackslash:           return sf::Keyboard::ScanBackslash;
        case kHIDUsage_KeyboardNonUSPound:          return sf::Keyboard::ScanBackslash;
        case kHIDUsage_KeyboardSemicolon:           return sf::Keyboard::ScanSemicolon;
        case kHIDUsage_KeyboardQuote:               return sf::Keyboard::ScanApostrophe;
        case kHIDUsage_KeyboardGraveAccentAndTilde: return sf::Keyboard::ScanGrave;
        case kHIDUsage_KeyboardComma:               return sf::Keyboard::ScanComma;
        case kHIDUsage_KeyboardPeriod:              return sf::Keyboard::ScanPeriod;
        case kHIDUsage_KeyboardSlash:               return sf::Keyboard::ScanSlash;
        case kHIDUsage_KeyboardCapsLock:            return sf::Keyboard::ScanCapsLock;

        case kHIDUsage_KeyboardF1:  return sf::Keyboard::ScanF1;
        case kHIDUsage_KeyboardF2:  return sf::Keyboard::ScanF2;
        case kHIDUsage_KeyboardF3:  return sf::Keyboard::ScanF3;
        case kHIDUsage_KeyboardF4:  return sf::Keyboard::ScanF4;
        case kHIDUsage_KeyboardF5:  return sf::Keyboard::ScanF5;
        case kHIDUsage_KeyboardF6:  return sf::Keyboard::ScanF6;
        case kHIDUsage_KeyboardF7:  return sf::Keyboard::ScanF7;
        case kHIDUsage_KeyboardF8:  return sf::Keyboard::ScanF8;
        case kHIDUsage_KeyboardF9:  return sf::Keyboard::ScanF9;
        case kHIDUsage_KeyboardF10: return sf::Keyboard::ScanF10;
        case kHIDUsage_KeyboardF11: return sf::Keyboard::ScanF11;
        case kHIDUsage_KeyboardF12: return sf::Keyboard::ScanF12;

        case kHIDUsage_KeyboardPrintScreen:   return sf::Keyboard::ScanPrintScreen;
        case kHIDUsage_KeyboardScrollLock:    return sf::Keyboard::ScanScrollLock;
        case kHIDUsage_KeyboardPause:         return sf::Keyboard::ScanPause;
        case kHIDUsage_KeyboardInsert:        return sf::Keyboard::ScanInsert;
        case kHIDUsage_KeyboardHome:          return sf::Keyboard::ScanHome;
        case kHIDUsage_KeyboardPageUp:        return sf::Keyboard::ScanPageUp;
        case kHIDUsage_KeyboardDeleteForward: return sf::Keyboard::ScanDelete;
        case kHIDUsage_KeyboardEnd:           return sf::Keyboard::ScanEnd;
        case kHIDUsage_KeyboardPageDown:      return sf::Keyboard::ScanPageDown;

        case kHIDUsage_KeyboardRightArrow: return sf::Keyboard::ScanRight;
        case kHIDUsage_KeyboardLeftArrow:  return sf::Keyboard::ScanLeft;
        case kHIDUsage_KeyboardDownArrow:  return sf::Keyboard::ScanDown;
        case kHIDUsage_KeyboardUpArrow:    return sf::Keyboard::ScanUp;

        case kHIDUsage_KeypadNumLock:   return sf::Keyboard::ScanNumLock;
        case kHIDUsage_KeypadSlash:     return sf::Keyboard::ScanNumpadDivide;
        case kHIDUsage_KeypadAsterisk:  return sf::Keyboard::ScanNumpadMultiply;
        case kHIDUsage_KeypadHyphen:    return sf::Keyboard::ScanNumpadMinus;
        case kHIDUsage_KeypadPlus:      return sf::Keyboard::ScanNumpadPlus;
        case kHIDUsage_KeypadEqualSign: return sf::Keyboard::ScanNumpadEqual;
        case kHIDUsage_KeypadEnter:     return sf::Keyboard::ScanNumpadEnter;
        case kHIDUsage_KeypadPeriod:    return sf::Keyboard::ScanNumpadDecimal;

        case kHIDUsage_Keypad1: return sf::Keyboard::ScanNumpad1;
        case kHIDUsage_Keypad2: return sf::Keyboard::ScanNumpad2;
        case kHIDUsage_Keypad3: return sf::Keyboard::ScanNumpad3;
        case kHIDUsage_Keypad4: return sf::Keyboard::ScanNumpad4;
        case kHIDUsage_Keypad5: return sf::Keyboard::ScanNumpad5;
        case kHIDUsage_Keypad6: return sf::Keyboard::ScanNumpad6;
        case kHIDUsage_Keypad7: return sf::Keyboard::ScanNumpad7;
        case kHIDUsage_Keypad8: return sf::Keyboard::ScanNumpad8;
        case kHIDUsage_Keypad9: return sf::Keyboard::ScanNumpad9;
        case kHIDUsage_Keypad0: return sf::Keyboard::ScanNumpad0;

        case kHIDUsage_KeyboardNonUSBackslash: return sf::Keyboard::ScanNonUsBackslash;
        case kHIDUsage_KeyboardApplication:    return sf::Keyboard::ScanApplication;
        case kHIDUsage_KeyboardPower:          return sf::Keyboard::ScanUnknown;

        case kHIDUsage_KeyboardF13: return sf::Keyboard::ScanF13;
        case kHIDUsage_KeyboardF14: return sf::Keyboard::ScanF14;
        case kHIDUsage_KeyboardF15: return sf::Keyboard::ScanF15;
        case kHIDUsage_KeyboardF16: return sf::Keyboard::ScanF16;
        case kHIDUsage_KeyboardF17: return sf::Keyboard::ScanF17;
        case kHIDUsage_KeyboardF18: return sf::Keyboard::ScanF18;
        case kHIDUsage_KeyboardF19: return sf::Keyboard::ScanF19;
        case kHIDUsage_KeyboardF20: return sf::Keyboard::ScanF20;
        case kHIDUsage_KeyboardF21: return sf::Keyboard::ScanF21;
        case kHIDUsage_KeyboardF22: return sf::Keyboard::ScanF22;
        case kHIDUsage_KeyboardF23: return sf::Keyboard::ScanF23;
        case kHIDUsage_KeyboardF24: return sf::Keyboard::ScanF24;

        case kHIDUsage_KeyboardExecute: return sf::Keyboard::ScanExecute;
        case kHIDUsage_KeyboardHelp:    return sf::Keyboard::ScanHelp;
        case kHIDUsage_KeyboardMenu:    return sf::Keyboard::ScanMenu;
        case kHIDUsage_KeyboardSelect:  return sf::Keyboard::ScanSelect;
        case kHIDUsage_KeyboardStop:    return sf::Keyboard::ScanStop;
        case kHIDUsage_KeyboardAgain:   return sf::Keyboard::ScanRedo;
        case kHIDUsage_KeyboardUndo:    return sf::Keyboard::ScanUndo;
        case kHIDUsage_KeyboardCut:     return sf::Keyboard::ScanCut;
        case kHIDUsage_KeyboardCopy:    return sf::Keyboard::ScanCopy;
        case kHIDUsage_KeyboardPaste:   return sf::Keyboard::ScanPaste;
        case kHIDUsage_KeyboardFind:    return sf::Keyboard::ScanSearch;

        case kHIDUsage_KeyboardMute:       return sf::Keyboard::ScanVolumeMute;
        case kHIDUsage_KeyboardVolumeUp:   return sf::Keyboard::ScanVolumeUp;
        case kHIDUsage_KeyboardVolumeDown: return sf::Keyboard::ScanVolumeDown;

        case kHIDUsage_KeyboardLockingCapsLock:   return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardLockingNumLock:    return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardLockingScrollLock: return sf::Keyboard::ScanUnknown;

        case kHIDUsage_KeypadComma:            return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeypadEqualSignAS400:   return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardInternational1: return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardInternational2: return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardInternational3: return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardInternational4: return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardInternational5: return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardInternational6: return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardInternational7: return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardInternational8: return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardInternational9: return sf::Keyboard::ScanUnknown;

        case kHIDUsage_KeyboardLANG1: return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardLANG2: return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardLANG3: return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardLANG4: return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardLANG5: return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardLANG6: return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardLANG7: return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardLANG8: return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardLANG9: return sf::Keyboard::ScanUnknown;

        case kHIDUsage_KeyboardAlternateErase:    return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardSysReqOrAttention: return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardCancel:            return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardClear:             return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardPrior:             return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardReturn:            return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardSeparator:         return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardOut:               return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardOper:              return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardClearOrAgain:      return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardCrSelOrProps:      return sf::Keyboard::ScanUnknown;
        case kHIDUsage_KeyboardExSel:             return sf::Keyboard::ScanUnknown;

            /* 0xa5-0xdf Reserved */

        case kHIDUsage_KeyboardLeftControl:  return sf::Keyboard::ScanLControl;
        case kHIDUsage_KeyboardLeftShift:    return sf::Keyboard::ScanLShift;
        case kHIDUsage_KeyboardLeftAlt:      return sf::Keyboard::ScanLAlt;
        case kHIDUsage_KeyboardLeftGUI:      return sf::Keyboard::ScanLSystem;
        case kHIDUsage_KeyboardRightControl: return sf::Keyboard::ScanRControl;
        case kHIDUsage_KeyboardRightShift:   return sf::Keyboard::ScanRShift;
        case kHIDUsage_KeyboardRightAlt:     return sf::Keyboard::ScanRAlt;
        case kHIDUsage_KeyboardRightGUI:     return sf::Keyboard::ScanRSystem;

            /* 0xe8-0xffff Reserved */

        case kHIDUsage_Keyboard_Reserved: return sf::Keyboard::ScanUnknown;
        default:                          return sf::Keyboard::ScanUnknown;
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
        case 0x00: return sf::Keyboard::ScanA;
        case 0x0b: return sf::Keyboard::ScanB;
        case 0x08: return sf::Keyboard::ScanC;
        case 0x02: return sf::Keyboard::ScanD;
        case 0x0e: return sf::Keyboard::ScanE;
        case 0x03: return sf::Keyboard::ScanF;
        case 0x05: return sf::Keyboard::ScanG;
        case 0x04: return sf::Keyboard::ScanH;
        case 0x22: return sf::Keyboard::ScanI;
        case 0x26: return sf::Keyboard::ScanJ;
        case 0x28: return sf::Keyboard::ScanK;
        case 0x25: return sf::Keyboard::ScanL;
        case 0x2e: return sf::Keyboard::ScanM;
        case 0x2d: return sf::Keyboard::ScanN;
        case 0x1f: return sf::Keyboard::ScanO;
        case 0x23: return sf::Keyboard::ScanP;
        case 0x0c: return sf::Keyboard::ScanQ;
        case 0x0f: return sf::Keyboard::ScanR;
        case 0x01: return sf::Keyboard::ScanS;
        case 0x11: return sf::Keyboard::ScanT;
        case 0x20: return sf::Keyboard::ScanU;
        case 0x09: return sf::Keyboard::ScanV;
        case 0x0d: return sf::Keyboard::ScanW;
        case 0x07: return sf::Keyboard::ScanX;
        case 0x10: return sf::Keyboard::ScanY;
        case 0x06: return sf::Keyboard::ScanZ;

        case 0x12: return sf::Keyboard::ScanNum1;
        case 0x13: return sf::Keyboard::ScanNum2;
        case 0x14: return sf::Keyboard::ScanNum3;
        case 0x15: return sf::Keyboard::ScanNum4;
        case 0x17: return sf::Keyboard::ScanNum5;
        case 0x16: return sf::Keyboard::ScanNum6;
        case 0x1a: return sf::Keyboard::ScanNum7;
        case 0x1c: return sf::Keyboard::ScanNum8;
        case 0x19: return sf::Keyboard::ScanNum9;
        case 0x1d: return sf::Keyboard::ScanNum0;

        case 0x24: return sf::Keyboard::ScanEnter;
        case 0x35: return sf::Keyboard::ScanEscape;
        case 0x33: return sf::Keyboard::ScanBackspace;
        case 0x30: return sf::Keyboard::ScanTab;
        case 0x31: return sf::Keyboard::ScanSpace;

        case 0x18: return sf::Keyboard::ScanEqual;
        case 0x21: return sf::Keyboard::ScanLBracket;
        case 0x1e: return sf::Keyboard::ScanRBracket;
        case 0x2a: return sf::Keyboard::ScanBackslash;
        case 0x1b: return sf::Keyboard::ScanHyphen;
        case 0x29: return sf::Keyboard::ScanSemicolon;
        case 0x27: return sf::Keyboard::ScanApostrophe;
        case 0x32: return sf::Keyboard::ScanGrave;
        case 0x2b: return sf::Keyboard::ScanComma;
        case 0x2f: return sf::Keyboard::ScanPeriod;
        case 0x2c: return sf::Keyboard::ScanSlash;

        case 0x7a: return sf::Keyboard::ScanF1;
        case 0x78: return sf::Keyboard::ScanF2;
        case 0x63: return sf::Keyboard::ScanF3;
        case 0x76: return sf::Keyboard::ScanF4;
        case 0x60: return sf::Keyboard::ScanF5;
        case 0x61: return sf::Keyboard::ScanF6;
        case 0x62: return sf::Keyboard::ScanF7;
        case 0x64: return sf::Keyboard::ScanF8;
        case 0x65: return sf::Keyboard::ScanF9;
        case 0x6d: return sf::Keyboard::ScanF10;
        case 0x67: return sf::Keyboard::ScanF11;
        case 0x6f: return sf::Keyboard::ScanF12;
        case 0x69: return sf::Keyboard::ScanF13;
        case 0x6b: return sf::Keyboard::ScanF14;
        case 0x71: return sf::Keyboard::ScanF15;

        case 0x6A: return sf::Keyboard::ScanF16;
        case 0x40: return sf::Keyboard::ScanF17;
        case 0x4F: return sf::Keyboard::ScanF18;
        case 0x50: return sf::Keyboard::ScanF19;
        case 0x5A: return sf::Keyboard::ScanF20;
        
        /* TODO Those are missing:
         * case 0x: return sf::Keyboard::ScanF21;
         * case 0x: return sf::Keyboard::ScanF22;
         * case 0x: return sf::Keyboard::ScanF23;
         * case 0x: return sf::Keyboard::ScanF24;
         */

        case 0x39: return sf::Keyboard::ScanCapsLock;

        /* TODO Those are missing:
         * case 0x: return sf::Keyboard::ScanPrintScreen;
         * case 0x: return sf::Keyboard::ScanScrollLock;
         * case 0x: return sf::Keyboard::ScanPause;
         */

        case 0x72: return sf::Keyboard::ScanInsert;
        case 0x73: return sf::Keyboard::ScanHome;
        case 0x74: return sf::Keyboard::ScanPageUp;
        case 0x75: return sf::Keyboard::ScanDelete;
        case 0x77: return sf::Keyboard::ScanEnd;
        case 0x79: return sf::Keyboard::ScanPageDown;

        case 0x7c: return sf::Keyboard::ScanRight;
        case 0x7b: return sf::Keyboard::ScanLeft;
        case 0x7d: return sf::Keyboard::ScanDown;
        case 0x7e: return sf::Keyboard::ScanUp;

        case 0x47: return sf::Keyboard::ScanNumLock;
        case 0x4b: return sf::Keyboard::ScanNumpadDivide;
        case 0x43: return sf::Keyboard::ScanNumpadMultiply;
        case 0x4e: return sf::Keyboard::ScanNumpadMinus;
        case 0x45: return sf::Keyboard::ScanNumpadPlus;
        case 0x51: return sf::Keyboard::ScanNumpadEqual;
        case 0x4c: return sf::Keyboard::ScanNumpadEnter;
        case 0x41: return sf::Keyboard::ScanNumpadDecimal;

        case 0x53: return sf::Keyboard::ScanNumpad1;
        case 0x54: return sf::Keyboard::ScanNumpad2;
        case 0x55: return sf::Keyboard::ScanNumpad3;
        case 0x56: return sf::Keyboard::ScanNumpad4;
        case 0x57: return sf::Keyboard::ScanNumpad5;
        case 0x58: return sf::Keyboard::ScanNumpad6;
        case 0x59: return sf::Keyboard::ScanNumpad7;
        case 0x5b: return sf::Keyboard::ScanNumpad8;
        case 0x5c: return sf::Keyboard::ScanNumpad9;
        case 0x52: return sf::Keyboard::ScanNumpad0;

        /* TODO Those are missing:
         * case 0x: return sf::Keyboard::ScanNonUsBackslash;
         * case 0x: return sf::Keyboard::ScanApplication;
         * case 0x: return sf::Keyboard::ScanExecute;
         * case 0x: return sf::Keyboard::ScanHelp;
         * case 0x: return sf::Keyboard::ScanMenu;
         * case 0x: return sf::Keyboard::ScanSelect;
         * case 0x: return sf::Keyboard::ScanStop;
         * case 0x: return sf::Keyboard::ScanRedo;
         * case 0x: return sf::Keyboard::ScanUndo;
         * case 0x: return sf::Keyboard::ScanCut;
         * case 0x: return sf::Keyboard::ScanCopy;
         * case 0x: return sf::Keyboard::ScanPaste;
         * case 0x: return sf::Keyboard::ScanSearch;
         */

        case 0x4a: return sf::Keyboard::ScanVolumeMute;
        case 0x48: return sf::Keyboard::ScanVolumeUp;
        case 0x49: return sf::Keyboard::ScanVolumeDown;

        /* NOTE Those are symmetric so we leave them out.
         *      Thankfully handled through modifiers and not virtual codes.
         * case 0x3b: return sf::Keyboard::ScanLControl;
         * case 0x38: return sf::Keyboard::ScanLShift;
         * case 0x3a: return sf::Keyboard::ScanLAlt;
         * case 0x37: return sf::Keyboard::ScanLSystem;
         * case 0x3b: return sf::Keyboard::ScanRControl;
         * case 0x38: return sf::Keyboard::ScanRShift;
         * case 0x3a: return sf::Keyboard::ScanRAlt;
         * case 0x37: return sf::Keyboard::ScanRSystem;
         */

        default: return sf::Keyboard::ScanUnknown;
    }
}


////////////////////////////////////////////////////////
UInt8 HIDInputManager::scanToVirtualCode(Keyboard::Scancode code)
{
    switch (code)
    {
        case sf::Keyboard::ScanA: return 0x00;
        case sf::Keyboard::ScanB: return 0x0b;
        case sf::Keyboard::ScanC: return 0x08;
        case sf::Keyboard::ScanD: return 0x02;
        case sf::Keyboard::ScanE: return 0x0e;
        case sf::Keyboard::ScanF: return 0x03;
        case sf::Keyboard::ScanG: return 0x05;
        case sf::Keyboard::ScanH: return 0x04;
        case sf::Keyboard::ScanI: return 0x22;
        case sf::Keyboard::ScanJ: return 0x26;
        case sf::Keyboard::ScanK: return 0x28;
        case sf::Keyboard::ScanL: return 0x25;
        case sf::Keyboard::ScanM: return 0x2e;
        case sf::Keyboard::ScanN: return 0x2d;
        case sf::Keyboard::ScanO: return 0x1f;
        case sf::Keyboard::ScanP: return 0x23;
        case sf::Keyboard::ScanQ: return 0x0c;
        case sf::Keyboard::ScanR: return 0x0f;
        case sf::Keyboard::ScanS: return 0x01;
        case sf::Keyboard::ScanT: return 0x11;
        case sf::Keyboard::ScanU: return 0x20;
        case sf::Keyboard::ScanV: return 0x09;
        case sf::Keyboard::ScanW: return 0x0d;
        case sf::Keyboard::ScanX: return 0x07;
        case sf::Keyboard::ScanY: return 0x10;
        case sf::Keyboard::ScanZ: return 0x06;

        case sf::Keyboard::ScanNum1: return 0x12;
        case sf::Keyboard::ScanNum2: return 0x13;
        case sf::Keyboard::ScanNum3: return 0x14;
        case sf::Keyboard::ScanNum4: return 0x15;
        case sf::Keyboard::ScanNum5: return 0x17;
        case sf::Keyboard::ScanNum6: return 0x16;
        case sf::Keyboard::ScanNum7: return 0x1a;
        case sf::Keyboard::ScanNum8: return 0x1c;
        case sf::Keyboard::ScanNum9: return 0x19;
        case sf::Keyboard::ScanNum0: return 0x1d;

        case sf::Keyboard::ScanEnter:     return 0x24;
        case sf::Keyboard::ScanEscape:    return 0x35;
        case sf::Keyboard::ScanBackspace: return 0x33;
        case sf::Keyboard::ScanTab:       return 0x30;
        case sf::Keyboard::ScanSpace:     return 0x31;

        case sf::Keyboard::ScanEqual:       return 0x18;
        case sf::Keyboard::ScanLBracket:    return 0x21;
        case sf::Keyboard::ScanRBracket:    return 0x1e;
        case sf::Keyboard::ScanBackslash:   return 0x2a;
        case sf::Keyboard::ScanHyphen:      return 0x1b;
        case sf::Keyboard::ScanSemicolon:   return 0x29;
        case sf::Keyboard::ScanApostrophe:  return 0x27;
        case sf::Keyboard::ScanGrave:       return 0x32;
        case sf::Keyboard::ScanComma:       return 0x2b;
        case sf::Keyboard::ScanPeriod:      return 0x2f;
        case sf::Keyboard::ScanSlash:       return 0x2c;

        case sf::Keyboard::ScanF1:  return 0x7a;
        case sf::Keyboard::ScanF2:  return 0x78;
        case sf::Keyboard::ScanF3:  return 0x63;
        case sf::Keyboard::ScanF4:  return 0x76;
        case sf::Keyboard::ScanF5:  return 0x60;
        case sf::Keyboard::ScanF6:  return 0x61;
        case sf::Keyboard::ScanF7:  return 0x62;
        case sf::Keyboard::ScanF8:  return 0x64;
        case sf::Keyboard::ScanF9:  return 0x65;
        case sf::Keyboard::ScanF10: return 0x6d;
        case sf::Keyboard::ScanF11: return 0x67;
        case sf::Keyboard::ScanF12: return 0x6f;
        case sf::Keyboard::ScanF13: return 0x69;
        case sf::Keyboard::ScanF14: return 0x6b;
        case sf::Keyboard::ScanF15: return 0x71;

        case sf::Keyboard::ScanCapsLock: return 0x39;

        /* TODO Those are missing:
         * case sf::Keyboard::ScanPrintScreen: return 0;
         * case sf::Keyboard::ScanScrollLock:  return 0;
         * case sf::Keyboard::ScanPause:    return 0;
         */
        case sf::Keyboard::ScanInsert:   return 0x72;
        case sf::Keyboard::ScanHome:     return 0x73;
        case sf::Keyboard::ScanPageUp:   return 0x74;
        case sf::Keyboard::ScanDelete:   return 0x75;
        case sf::Keyboard::ScanEnd:      return 0x77;
        case sf::Keyboard::ScanPageDown: return 0x79;

        case sf::Keyboard::ScanRight: return 0x7c;
        case sf::Keyboard::ScanLeft:  return 0x7b;
        case sf::Keyboard::ScanDown:  return 0x7d;
        case sf::Keyboard::ScanUp:    return 0x7e;

        case sf::Keyboard::ScanNumLock:        return 0x47;
        case sf::Keyboard::ScanNumpadDivide:   return 0x4b;
        case sf::Keyboard::ScanNumpadMultiply: return 0x43;
        case sf::Keyboard::ScanNumpadMinus:    return 0x4e;
        case sf::Keyboard::ScanNumpadPlus:     return 0x45;
        case sf::Keyboard::ScanNumpadEqual:    return 0x51;
        case sf::Keyboard::ScanNumpadEnter:    return 0x4c;
        case sf::Keyboard::ScanNumpadDecimal:  return 0x41;

        case sf::Keyboard::ScanNumpad1: return 0x53;
        case sf::Keyboard::ScanNumpad2: return 0x54;
        case sf::Keyboard::ScanNumpad3: return 0x55;
        case sf::Keyboard::ScanNumpad4: return 0x56;
        case sf::Keyboard::ScanNumpad5: return 0x57;
        case sf::Keyboard::ScanNumpad6: return 0x58;
        case sf::Keyboard::ScanNumpad7: return 0x59;
        case sf::Keyboard::ScanNumpad8: return 0x5b;
        case sf::Keyboard::ScanNumpad9: return 0x5c;
        case sf::Keyboard::ScanNumpad0: return 0x52;

        /* TODO Those are missing:
         * case sf::Keyboard::ScanNonUsBackslash: return 0;
         * case sf::Keyboard::ScanApplication: return 0;
         * case sf::Keyboard::ScanExecute:     return 0;
         * case sf::Keyboard::ScanHelp:        return 0;
         * case sf::Keyboard::ScanMenu:        return 0;
         * case sf::Keyboard::ScanSelect:      return 0;
         * case sf::Keyboard::ScanStop:        return 0;
         * case sf::Keyboard::ScanRedo:        return 0;
         * case sf::Keyboard::ScanUndo:        return 0;
         * case sf::Keyboard::ScanCut:         return 0;
         * case sf::Keyboard::ScanCopy:        return 0;
         * case sf::Keyboard::ScanPaste:       return 0;
         * case sf::Keyboard::ScanSearch:      return 0;
         */

        case sf::Keyboard::ScanVolumeMute: return 0x4a;
        case sf::Keyboard::ScanVolumeUp:   return 0x48;
        case sf::Keyboard::ScanVolumeDown: return 0x49;

        case sf::Keyboard::ScanLControl: return 0x3b;
        case sf::Keyboard::ScanLShift:   return 0x38;
        case sf::Keyboard::ScanLAlt:     return 0x3a;
        case sf::Keyboard::ScanLSystem:  return 0x37;
        case sf::Keyboard::ScanRControl: return 0x3b;
        case sf::Keyboard::ScanRShift:   return 0x38;
        case sf::Keyboard::ScanRAlt:     return 0x3a;
        case sf::Keyboard::ScanRSystem:  return 0x37;

        default: return 0x00;
    }
}


////////////////////////////////////////////////////////
Keyboard::Key HIDInputManager::localizedKeyFallback(Keyboard::Scancode code)
{
    switch (code)
    {
        case sf::Keyboard::ScanEnter:      return sf::Keyboard::Return;
        case sf::Keyboard::ScanEscape:     return sf::Keyboard::Escape;
        case sf::Keyboard::ScanBackspace:  return sf::Keyboard::BackSpace;
        case sf::Keyboard::ScanTab:        return sf::Keyboard::Tab;
        case sf::Keyboard::ScanSpace:      return sf::Keyboard::Space;

        case sf::Keyboard::ScanF1:   return sf::Keyboard::F1;
        case sf::Keyboard::ScanF2:   return sf::Keyboard::F2;
        case sf::Keyboard::ScanF3:   return sf::Keyboard::F3;
        case sf::Keyboard::ScanF4:   return sf::Keyboard::F4;
        case sf::Keyboard::ScanF5:   return sf::Keyboard::F5;
        case sf::Keyboard::ScanF6:   return sf::Keyboard::F6;
        case sf::Keyboard::ScanF7:   return sf::Keyboard::F7;
        case sf::Keyboard::ScanF8:   return sf::Keyboard::F8;
        case sf::Keyboard::ScanF9:   return sf::Keyboard::F9;
        case sf::Keyboard::ScanF10:  return sf::Keyboard::F10;
        case sf::Keyboard::ScanF11:  return sf::Keyboard::F11;
        case sf::Keyboard::ScanF12:  return sf::Keyboard::F12;
        case sf::Keyboard::ScanF13:  return sf::Keyboard::F13;
        case sf::Keyboard::ScanF14:  return sf::Keyboard::F14;
        case sf::Keyboard::ScanF15:  return sf::Keyboard::F15;

        case sf::Keyboard::ScanPause:    return sf::Keyboard::Pause;
        case sf::Keyboard::ScanInsert:   return sf::Keyboard::Insert;
        case sf::Keyboard::ScanHome:     return sf::Keyboard::Home;
        case sf::Keyboard::ScanPageUp:   return sf::Keyboard::PageUp;
        case sf::Keyboard::ScanDelete:   return sf::Keyboard::Delete;
        case sf::Keyboard::ScanEnd:      return sf::Keyboard::End;
        case sf::Keyboard::ScanPageDown: return sf::Keyboard::PageDown;

        case sf::Keyboard::ScanRight:  return sf::Keyboard::Right;
        case sf::Keyboard::ScanLeft:   return sf::Keyboard::Left;
        case sf::Keyboard::ScanDown:   return sf::Keyboard::Down;
        case sf::Keyboard::ScanUp:     return sf::Keyboard::Up;

        case sf::Keyboard::ScanNumpadDivide:   return sf::Keyboard::Divide;
        case sf::Keyboard::ScanNumpadMultiply: return sf::Keyboard::Multiply;
        case sf::Keyboard::ScanNumpadMinus:    return sf::Keyboard::Subtract;
        case sf::Keyboard::ScanNumpadPlus:     return sf::Keyboard::Add;

        case sf::Keyboard::ScanNumpad0:  return sf::Keyboard::Numpad1;
        case sf::Keyboard::ScanNumpad1:  return sf::Keyboard::Numpad2;
        case sf::Keyboard::ScanNumpad2:  return sf::Keyboard::Numpad3;
        case sf::Keyboard::ScanNumpad3:  return sf::Keyboard::Numpad4;
        case sf::Keyboard::ScanNumpad4:  return sf::Keyboard::Numpad5;
        case sf::Keyboard::ScanNumpad5:  return sf::Keyboard::Numpad6;
        case sf::Keyboard::ScanNumpad6:  return sf::Keyboard::Numpad7;
        case sf::Keyboard::ScanNumpad7:  return sf::Keyboard::Numpad8;
        case sf::Keyboard::ScanNumpad8:  return sf::Keyboard::Numpad9;
        case sf::Keyboard::ScanNumpad9:  return sf::Keyboard::Numpad0;

        case sf::Keyboard::ScanLControl:  return sf::Keyboard::LControl;
        case sf::Keyboard::ScanLShift:    return sf::Keyboard::LShift;
        case sf::Keyboard::ScanLAlt:      return sf::Keyboard::LAlt;
        case sf::Keyboard::ScanLSystem:   return sf::Keyboard::LSystem;
        case sf::Keyboard::ScanRControl:  return sf::Keyboard::RControl;
        case sf::Keyboard::ScanRShift:    return sf::Keyboard::RShift;
        case sf::Keyboard::ScanRAlt:      return sf::Keyboard::RAlt;
        case sf::Keyboard::ScanRSystem:   return sf::Keyboard::RSystem;

        default: return sf::Keyboard::Unknown;
    }
}


} // namespace priv

} // namespace sf

