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
    return isKeyPressed(unlocalize(key));
}


////////////////////////////////////////////////////////////
bool HIDInputManager::isKeyPressed(Keyboard::Scancode code)
{
    return (code != Keyboard::sUnknown) && isPressed(m_keys[code]);
}


////////////////////////////////////////////////////////////
Keyboard::Key HIDInputManager::localize(Keyboard::Scancode code)
{
    // TODO ensure mapping is still valid
    return m_gnippam[code];
}


////////////////////////////////////////////////////////////
Keyboard::Scancode HIDInputManager::unlocalize(Keyboard::Key key)
{
    // TODO ensure mapping is still valid
    return m_mapping[key];
}


////////////////////////////////////////////////////////////
String HIDInputManager::localizedRepresentation(Keyboard::Scancode code)
{
    UniChar unicode = toUnicode(localize(code));
    if (unicode != 0x00)
        return sf::String(static_cast<Uint32>(unicode));

    // Fallback to our best guess for the keys that are known to be independent of the layout.
    switch (code)
    {
        case sf::Keyboard::sEnter:      return "Enter";
        case sf::Keyboard::sEscape:     return "Escape";
        case sf::Keyboard::sBackspace:  return "Backspace";
        case sf::Keyboard::sTab:        return "Tab";
        case sf::Keyboard::sSpace:      return "Space";

        case sf::Keyboard::sF1:   return "F1";
        case sf::Keyboard::sF2:   return "F2";
        case sf::Keyboard::sF3:   return "F3";
        case sf::Keyboard::sF4:   return "F4";
        case sf::Keyboard::sF5:   return "F5";
        case sf::Keyboard::sF6:   return "F6";
        case sf::Keyboard::sF7:   return "F7";
        case sf::Keyboard::sF8:   return "F8";
        case sf::Keyboard::sF9:   return "F9";
        case sf::Keyboard::sF10:  return "F10";
        case sf::Keyboard::sF11:  return "F11";
        case sf::Keyboard::sF12:  return "F12";
        case sf::Keyboard::sF13:  return "F13";
        case sf::Keyboard::sF14:  return "F14";
        case sf::Keyboard::sF15:  return "F15";

        case sf::Keyboard::sCapsLock:     return "CapsLock";
        case sf::Keyboard::sPrintScreen:  return "PrintScreen";
        case sf::Keyboard::sScrollLock:   return "ScrollLock";

        case sf::Keyboard::sPause:    return "Pause";
        case sf::Keyboard::sInsert:   return "Insert";
        case sf::Keyboard::sHome:     return "Home";
        case sf::Keyboard::sPageUp:   return "PageUp";
        case sf::Keyboard::sDelete:   return "Delete";
        case sf::Keyboard::sEnd:      return "End";
        case sf::Keyboard::sPageDown: return "PageDown";

        case sf::Keyboard::sLeft:   return "Left Arrow";
        case sf::Keyboard::sRight:  return "Right Arrow";
        case sf::Keyboard::sDown:   return "Down Arrow";
        case sf::Keyboard::sUp:     return "Up Arrow";

        case sf::Keyboard::sNumLock:    return "NumLock";
        case sf::Keyboard::sDivide:     return "Divide (Numpad)";
        case sf::Keyboard::sMultiply:   return "Multiply (Numpad)";
        case sf::Keyboard::sMinus:      return "Minux (Numpad)";
        case sf::Keyboard::sPlus:       return "Plus (Numpad)";
        case sf::Keyboard::sPadEquals:  return "Equals (Numpad)";
        case sf::Keyboard::sReturn:     return "Return (Numpad)";
        case sf::Keyboard::sDecimal:    return "Decimal (Numpad)";

        case sf::Keyboard::sNumpad0:  return "0 (Numpad)";
        case sf::Keyboard::sNumpad1:  return "1 (Numpad)";
        case sf::Keyboard::sNumpad2:  return "2 (Numpad)";
        case sf::Keyboard::sNumpad3:  return "3 (Numpad)";
        case sf::Keyboard::sNumpad4:  return "4 (Numpad)";
        case sf::Keyboard::sNumpad5:  return "5 (Numpad)";
        case sf::Keyboard::sNumpad6:  return "6 (Numpad)";
        case sf::Keyboard::sNumpad7:  return "7 (Numpad)";
        case sf::Keyboard::sNumpad8:  return "8 (Numpad)";
        case sf::Keyboard::sNumpad9:  return "9 (Numpad)";

        case sf::Keyboard::sApplication:  return "Application";
        case sf::Keyboard::sExecute:      return "Execute";
        case sf::Keyboard::sHelp:         return "Help";
        case sf::Keyboard::sMenu:         return "Menu";
        case sf::Keyboard::sSelect:       return "Select";
        case sf::Keyboard::sStop:         return "Stop";
        case sf::Keyboard::sAgain:        return "Again";
        case sf::Keyboard::sUndo:         return "Undo";
        case sf::Keyboard::sCut:          return "Cut";
        case sf::Keyboard::sCopy:         return "Copy";
        case sf::Keyboard::sPaste:        return "Paste";
        case sf::Keyboard::sFind:         return "Find";
        case sf::Keyboard::sMute:         return "Mute";
        case sf::Keyboard::sVolumeUp:     return "Volume Up";
        case sf::Keyboard::sVolumeDown:   return "Volume Down";

        case sf::Keyboard::sLControl:  return "Control (Left)";
        case sf::Keyboard::sLShift:    return "Shift (Left)";
        case sf::Keyboard::sLAlt:      return "Alt (Left)";
        case sf::Keyboard::sLSystem:   return "Command (Left)";
        case sf::Keyboard::sRControl:  return "Control (Right)";
        case sf::Keyboard::sRShift:    return "Shift (Right)";
        case sf::Keyboard::sRAlt:      return "Alt (Right)";
        case sf::Keyboard::sRSystem:   return "Command (Right)";

        default: return "Unknown Scancode"; // no guess good enough possible.
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

    initializeKeyboard();
    buildMappings();
}


////////////////////////////////////////////////////////////
HIDInputManager::~HIDInputManager()
{
    freeUp();
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
    if (code != Keyboard::sUnknown)
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
      m_mapping[i] = Keyboard::sUnknown;
    for (int i = 0; i < Keyboard::sCodeCount; ++i)
      m_gnippam[i] = Keyboard::Unknown;

    // Get the current keyboard layout
    TISInputSourceRef tis = TISCopyCurrentKeyboardLayoutInputSource();
    CFDataRef layoutData = static_cast<CFDataRef>(TISGetInputSourceProperty(tis, kTISPropertyUnicodeKeyLayoutData));

    if (layoutData == 0)
    {
        sf::err() << "Cannot get the keyboard layout\n";
        CFRelease(tis);
        return;
    }

    UCKeyboardLayout* layout = reinterpret_cast<UCKeyboardLayout*>(CFDataGetBytePtr(layoutData));

    // For each scancode having a IOHIDElement, we translate the corresponding
    // virtual code to a localized Key.
    for (int i = 0; i < Keyboard::sCodeCount; ++i)
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
        if (code == Keyboard::Unknown) code = localizedKeyFallback(scan);

        if (code == Keyboard::Unknown) continue;

        // Register the bi-mapping
        m_mapping[code] = scan;
        m_gnippam[scan] = code;
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
        case kHIDUsage_KeyboardErrorRollOver:   return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardPOSTFail:        return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardErrorUndefined:  return sf::Keyboard::sUnknown;

        case kHIDUsage_KeyboardA:  return sf::Keyboard::sA;
        case kHIDUsage_KeyboardB:  return sf::Keyboard::sB;
        case kHIDUsage_KeyboardC:  return sf::Keyboard::sC;
        case kHIDUsage_KeyboardD:  return sf::Keyboard::sD;
        case kHIDUsage_KeyboardE:  return sf::Keyboard::sE;
        case kHIDUsage_KeyboardF:  return sf::Keyboard::sF;
        case kHIDUsage_KeyboardG:  return sf::Keyboard::sG;
        case kHIDUsage_KeyboardH:  return sf::Keyboard::sH;
        case kHIDUsage_KeyboardI:  return sf::Keyboard::sI;
        case kHIDUsage_KeyboardJ:  return sf::Keyboard::sJ;
        case kHIDUsage_KeyboardK:  return sf::Keyboard::sK;
        case kHIDUsage_KeyboardL:  return sf::Keyboard::sL;
        case kHIDUsage_KeyboardM:  return sf::Keyboard::sM;
        case kHIDUsage_KeyboardN:  return sf::Keyboard::sN;
        case kHIDUsage_KeyboardO:  return sf::Keyboard::sO;
        case kHIDUsage_KeyboardP:  return sf::Keyboard::sP;
        case kHIDUsage_KeyboardQ:  return sf::Keyboard::sQ;
        case kHIDUsage_KeyboardR:  return sf::Keyboard::sR;
        case kHIDUsage_KeyboardS:  return sf::Keyboard::sS;
        case kHIDUsage_KeyboardT:  return sf::Keyboard::sT;
        case kHIDUsage_KeyboardU:  return sf::Keyboard::sU;
        case kHIDUsage_KeyboardV:  return sf::Keyboard::sV;
        case kHIDUsage_KeyboardW:  return sf::Keyboard::sW;
        case kHIDUsage_KeyboardX:  return sf::Keyboard::sX;
        case kHIDUsage_KeyboardY:  return sf::Keyboard::sY;
        case kHIDUsage_KeyboardZ:  return sf::Keyboard::sZ;

        case kHIDUsage_Keyboard1:  return sf::Keyboard::sNum1;
        case kHIDUsage_Keyboard2:  return sf::Keyboard::sNum2;
        case kHIDUsage_Keyboard3:  return sf::Keyboard::sNum3;
        case kHIDUsage_Keyboard4:  return sf::Keyboard::sNum4;
        case kHIDUsage_Keyboard5:  return sf::Keyboard::sNum5;
        case kHIDUsage_Keyboard6:  return sf::Keyboard::sNum6;
        case kHIDUsage_Keyboard7:  return sf::Keyboard::sNum7;
        case kHIDUsage_Keyboard8:  return sf::Keyboard::sNum8;
        case kHIDUsage_Keyboard9:  return sf::Keyboard::sNum9;
        case kHIDUsage_Keyboard0:  return sf::Keyboard::sNum0;

        case kHIDUsage_KeyboardReturnOrEnter:       return sf::Keyboard::sEnter;
        case kHIDUsage_KeyboardEscape:              return sf::Keyboard::sEscape;
        case kHIDUsage_KeyboardDeleteOrBackspace:   return sf::Keyboard::sBackspace;
        case kHIDUsage_KeyboardTab:                 return sf::Keyboard::sTab;
        case kHIDUsage_KeyboardSpacebar:            return sf::Keyboard::sSpace;
        case kHIDUsage_KeyboardHyphen:              return sf::Keyboard::sHyphen;
        case kHIDUsage_KeyboardEqualSign:           return sf::Keyboard::sEquals;
        case kHIDUsage_KeyboardOpenBracket:         return sf::Keyboard::sLBracket;
        case kHIDUsage_KeyboardCloseBracket:        return sf::Keyboard::sRBracket;
        case kHIDUsage_KeyboardBackslash:           return sf::Keyboard::sBackslash;
        case kHIDUsage_KeyboardNonUSPound:          return sf::Keyboard::sDash;
        case kHIDUsage_KeyboardSemicolon:           return sf::Keyboard::sSemicolon;
        case kHIDUsage_KeyboardQuote:               return sf::Keyboard::sQuote;
        case kHIDUsage_KeyboardGraveAccentAndTilde: return sf::Keyboard::sGraveAccent;
        case kHIDUsage_KeyboardComma:               return sf::Keyboard::sComma;
        case kHIDUsage_KeyboardPeriod:              return sf::Keyboard::sPeriod;
        case kHIDUsage_KeyboardSlash:               return sf::Keyboard::sForwardSlash;
        case kHIDUsage_KeyboardCapsLock:            return sf::Keyboard::sCapsLock;

        case kHIDUsage_KeyboardF1:   return sf::Keyboard::sF1;
        case kHIDUsage_KeyboardF2:   return sf::Keyboard::sF2;
        case kHIDUsage_KeyboardF3:   return sf::Keyboard::sF3;
        case kHIDUsage_KeyboardF4:   return sf::Keyboard::sF4;
        case kHIDUsage_KeyboardF5:   return sf::Keyboard::sF5;
        case kHIDUsage_KeyboardF6:   return sf::Keyboard::sF6;
        case kHIDUsage_KeyboardF7:   return sf::Keyboard::sF7;
        case kHIDUsage_KeyboardF8:   return sf::Keyboard::sF8;
        case kHIDUsage_KeyboardF9:   return sf::Keyboard::sF9;
        case kHIDUsage_KeyboardF10:  return sf::Keyboard::sF10;
        case kHIDUsage_KeyboardF11:  return sf::Keyboard::sF11;
        case kHIDUsage_KeyboardF12:  return sf::Keyboard::sF12;

        case kHIDUsage_KeyboardPrintScreen:    return sf::Keyboard::sPrintScreen;
        case kHIDUsage_KeyboardScrollLock:     return sf::Keyboard::sScrollLock;
        case kHIDUsage_KeyboardPause:          return sf::Keyboard::sPause;
        case kHIDUsage_KeyboardInsert:         return sf::Keyboard::sInsert;
        case kHIDUsage_KeyboardHome:           return sf::Keyboard::sHome;
        case kHIDUsage_KeyboardPageUp:         return sf::Keyboard::sPageUp;
        case kHIDUsage_KeyboardDeleteForward:  return sf::Keyboard::sDelete;
        case kHIDUsage_KeyboardEnd:            return sf::Keyboard::sEnd;
        case kHIDUsage_KeyboardPageDown:       return sf::Keyboard::sPageDown;

        case kHIDUsage_KeyboardRightArrow:  return sf::Keyboard::sRight;
        case kHIDUsage_KeyboardLeftArrow:   return sf::Keyboard::sLeft;
        case kHIDUsage_KeyboardDownArrow:   return sf::Keyboard::sDown;
        case kHIDUsage_KeyboardUpArrow:     return sf::Keyboard::sUp;

        case kHIDUsage_KeypadNumLock:   return sf::Keyboard::sNumLock;
        case kHIDUsage_KeypadSlash:     return sf::Keyboard::sDivide;
        case kHIDUsage_KeypadAsterisk:  return sf::Keyboard::sMultiply;
        case kHIDUsage_KeypadHyphen:    return sf::Keyboard::sMinus;
        case kHIDUsage_KeypadPlus:      return sf::Keyboard::sPlus;
        case kHIDUsage_KeypadEnter:     return sf::Keyboard::sReturn;

        case kHIDUsage_Keypad1:  return sf::Keyboard::sNumpad1;
        case kHIDUsage_Keypad2:  return sf::Keyboard::sNumpad2;
        case kHIDUsage_Keypad3:  return sf::Keyboard::sNumpad3;
        case kHIDUsage_Keypad4:  return sf::Keyboard::sNumpad4;
        case kHIDUsage_Keypad5:  return sf::Keyboard::sNumpad5;
        case kHIDUsage_Keypad6:  return sf::Keyboard::sNumpad6;
        case kHIDUsage_Keypad7:  return sf::Keyboard::sNumpad7;
        case kHIDUsage_Keypad8:  return sf::Keyboard::sNumpad8;
        case kHIDUsage_Keypad9:  return sf::Keyboard::sNumpad9;
        case kHIDUsage_Keypad0:  return sf::Keyboard::sNumpad0;

        case kHIDUsage_KeypadPeriod:            return sf::Keyboard::sDecimal;
        case kHIDUsage_KeyboardNonUSBackslash:  return sf::Keyboard::sReverseSolidus;
        case kHIDUsage_KeyboardApplication:     return sf::Keyboard::sApplication;
        case kHIDUsage_KeyboardPower:           return sf::Keyboard::sUnknown;
        case kHIDUsage_KeypadEqualSign:         return sf::Keyboard::sPadEquals;

        case kHIDUsage_KeyboardF13:   return sf::Keyboard::sF13;
        case kHIDUsage_KeyboardF14:   return sf::Keyboard::sF14;
        case kHIDUsage_KeyboardF15:   return sf::Keyboard::sF15;
        case kHIDUsage_KeyboardF16:   return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardF17:   return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardF18:   return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardF19:   return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardF20:   return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardF21:   return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardF22:   return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardF23:   return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardF24:   return sf::Keyboard::sUnknown;

        case kHIDUsage_KeyboardExecute:  return sf::Keyboard::sExecute;
        case kHIDUsage_KeyboardHelp:     return sf::Keyboard::sHelp;
        case kHIDUsage_KeyboardMenu:     return sf::Keyboard::sMenu;
        case kHIDUsage_KeyboardSelect:   return sf::Keyboard::sSelect;
        case kHIDUsage_KeyboardStop:     return sf::Keyboard::sStop;
        case kHIDUsage_KeyboardAgain:    return sf::Keyboard::sAgain;
        case kHIDUsage_KeyboardUndo:     return sf::Keyboard::sUndo;
        case kHIDUsage_KeyboardCut:      return sf::Keyboard::sCut;
        case kHIDUsage_KeyboardCopy:     return sf::Keyboard::sCopy;
        case kHIDUsage_KeyboardPaste:    return sf::Keyboard::sPaste;
        case kHIDUsage_KeyboardFind:     return sf::Keyboard::sFind;

        case kHIDUsage_KeyboardMute:       return sf::Keyboard::sMute;
        case kHIDUsage_KeyboardVolumeUp:   return sf::Keyboard::sVolumeUp;
        case kHIDUsage_KeyboardVolumeDown: return sf::Keyboard::sVolumeDown;

        case kHIDUsage_KeyboardLockingCapsLock:     return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardLockingNumLock:      return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardLockingScrollLock:   return sf::Keyboard::sUnknown;

        case kHIDUsage_KeypadComma:             return sf::Keyboard::sUnknown;
        case kHIDUsage_KeypadEqualSignAS400:    return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardInternational1:  return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardInternational2:  return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardInternational3:  return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardInternational4:  return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardInternational5:  return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardInternational6:  return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardInternational7:  return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardInternational8:  return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardInternational9:  return sf::Keyboard::sUnknown;

        case kHIDUsage_KeyboardLANG1:  return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardLANG2:  return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardLANG3:  return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardLANG4:  return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardLANG5:  return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardLANG6:  return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardLANG7:  return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardLANG8:  return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardLANG9:  return sf::Keyboard::sUnknown;

        case kHIDUsage_KeyboardAlternateErase:      return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardSysReqOrAttention:   return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardCancel:              return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardClear:               return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardPrior:               return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardReturn:              return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardSeparator:           return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardOut:                 return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardOper:                return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardClearOrAgain:        return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardCrSelOrProps:        return sf::Keyboard::sUnknown;
        case kHIDUsage_KeyboardExSel:               return sf::Keyboard::sUnknown;

            /* 0xa5-0xdf Reserved */

        case kHIDUsage_KeyboardLeftControl:  return sf::Keyboard::sLControl;
        case kHIDUsage_KeyboardLeftShift:    return sf::Keyboard::sLShift;
        case kHIDUsage_KeyboardLeftAlt:      return sf::Keyboard::sLAlt;
        case kHIDUsage_KeyboardLeftGUI:      return sf::Keyboard::sLSystem;
        case kHIDUsage_KeyboardRightControl: return sf::Keyboard::sRControl;
        case kHIDUsage_KeyboardRightShift:   return sf::Keyboard::sRShift;
        case kHIDUsage_KeyboardRightAlt:     return sf::Keyboard::sRAlt;
        case kHIDUsage_KeyboardRightGUI:     return sf::Keyboard::sRSystem;

            /* 0xe8-0xffff Reserved */

        case kHIDUsage_Keyboard_Reserved:  return sf::Keyboard::sUnknown;
        default:                           return sf::Keyboard::sUnknown;
    }
}


////////////////////////////////////////////////////////
Keyboard::Key HIDInputManager::localizedKey(UniChar ch)
{
    // Apple is using the private range 0xE000 - 0xF8FF for special keys.
    // Otherwise one can refer to the usual Unicode table.
    switch (ch)
    {
        case 0x41:  return sf::Keyboard::A; // uppercase A
        case 0x42:  return sf::Keyboard::B;
        case 0x43:  return sf::Keyboard::C;
        case 0x44:  return sf::Keyboard::D;
        case 0x45:  return sf::Keyboard::E;
        case 0x46:  return sf::Keyboard::F;
        case 0x47:  return sf::Keyboard::G;
        case 0x48:  return sf::Keyboard::H;
        case 0x49:  return sf::Keyboard::I;
        case 0x4a:  return sf::Keyboard::J;
        case 0x4b:  return sf::Keyboard::K;
        case 0x4c:  return sf::Keyboard::L;
        case 0x4d:  return sf::Keyboard::M;
        case 0x4e:  return sf::Keyboard::N;
        case 0x4f:  return sf::Keyboard::O;
        case 0x50:  return sf::Keyboard::P;
        case 0x51:  return sf::Keyboard::Q;
        case 0x52:  return sf::Keyboard::R;
        case 0x53:  return sf::Keyboard::S;
        case 0x54:  return sf::Keyboard::T;
        case 0x55:  return sf::Keyboard::U;
        case 0x56:  return sf::Keyboard::V;
        case 0x57:  return sf::Keyboard::W;
        case 0x58:  return sf::Keyboard::X;
        case 0x59:  return sf::Keyboard::Y;

        case 0x5a:  return sf::Keyboard::Z; // lowercase A
        case 0x61:  return sf::Keyboard::A;
        case 0x62:  return sf::Keyboard::B;
        case 0x63:  return sf::Keyboard::C;
        case 0x64:  return sf::Keyboard::D;
        case 0x65:  return sf::Keyboard::E;
        case 0x66:  return sf::Keyboard::F;
        case 0x67:  return sf::Keyboard::G;
        case 0x68:  return sf::Keyboard::H;
        case 0x69:  return sf::Keyboard::I;
        case 0x6a:  return sf::Keyboard::J;
        case 0x6b:  return sf::Keyboard::K;
        case 0x6c:  return sf::Keyboard::L;
        case 0x6d:  return sf::Keyboard::M;
        case 0x6e:  return sf::Keyboard::N;
        case 0x6f:  return sf::Keyboard::O;
        case 0x70:  return sf::Keyboard::P;
        case 0x71:  return sf::Keyboard::Q;
        case 0x72:  return sf::Keyboard::R;
        case 0x73:  return sf::Keyboard::S;
        case 0x74:  return sf::Keyboard::T;
        case 0x75:  return sf::Keyboard::U;
        case 0x76:  return sf::Keyboard::V;
        case 0x77:  return sf::Keyboard::W;
        case 0x78:  return sf::Keyboard::X;
        case 0x79:  return sf::Keyboard::Y;
        case 0x7a:  return sf::Keyboard::Z;

        case 0x30:  return sf::Keyboard::Num0;
        case 0x31:  return sf::Keyboard::Num1;
        case 0x32:  return sf::Keyboard::Num2;
        case 0x33:  return sf::Keyboard::Num3;
        case 0x34:  return sf::Keyboard::Num4;
        case 0x35:  return sf::Keyboard::Num5;
        case 0x36:  return sf::Keyboard::Num6;
        case 0x37:  return sf::Keyboard::Num7;
        case 0x38:  return sf::Keyboard::Num8;
        case 0x39:  return sf::Keyboard::Num9;

        case 0x1b:  return sf::Keyboard::Escape;

        /* Those are not represented using Unicode.
         * case 0x:  return sf::Keyboard::LControl;
         * case 0x:  return sf::Keyboard::LShift;
         * case 0x:  return sf::Keyboard::LAlt;
         * case 0x:  return sf::Keyboard::LSystem;
         * case 0x:  return sf::Keyboard::RControl;
         * case 0x:  return sf::Keyboard::RShift;
         * case 0x:  return sf::Keyboard::RAlt;
         * case 0x:  return sf::Keyboard::RSystem;
         */

        case NSMenuFunctionKey:  return sf::Keyboard::Menu;

        case 0x5b:  return sf::Keyboard::LBracket;
        case 0x5d:  return sf::Keyboard::RBracket;
        case 0x3b:  return sf::Keyboard::SemiColon;
        case 0x2c:  return sf::Keyboard::Comma;
        case 0x2e:  return sf::Keyboard::Period;
        case 0x27:  return sf::Keyboard::Quote;
        case 0x2044:  return sf::Keyboard::Slash; // 0x2f is Divide
        case 0x5c:  return sf::Keyboard::BackSlash;
        case 0x7e:  return sf::Keyboard::Tilde;
        case 0x3d:  return sf::Keyboard::Equal;
        case 0x2013:  return sf::Keyboard::Dash; // 0x2d is Subtract
        case 0x20:  return sf::Keyboard::Space;
        case 0x0d:  return sf::Keyboard::Return;
        case 0x7f:  return sf::Keyboard::BackSpace; // NOTE backspace & delete are swapped on Mac.
        case 0x09:  return sf::Keyboard::Tab;

        case NSPageUpFunctionKey:    return sf::Keyboard::PageUp;
        case NSPageDownFunctionKey:  return sf::Keyboard::PageDown;
        case NSEndFunctionKey:       return sf::Keyboard::End;
        case NSHomeFunctionKey:      return sf::Keyboard::Home;
        case NSInsertFunctionKey:    return sf::Keyboard::Insert;
        case NSDeleteFunctionKey:    return sf::Keyboard::Delete;

        case 0x2b:  return sf::Keyboard::Add;
        case 0x2d:  return sf::Keyboard::Subtract;
        case 0x2a:  return sf::Keyboard::Multiply;
        case 0x2f:  return sf::Keyboard::Divide;

        case NSLeftArrowFunctionKey:   return sf::Keyboard::Left;
        case NSRightArrowFunctionKey:  return sf::Keyboard::Right;
        case NSUpArrowFunctionKey:     return sf::Keyboard::Up;
        case NSDownArrowFunctionKey:   return sf::Keyboard::Down;

        /* Those are not represented using Unicode.
         * case 0x:  return sf::Keyboard::Numpad0;
         * case 0x:  return sf::Keyboard::Numpad1;
         * case 0x:  return sf::Keyboard::Numpad2;
         * case 0x:  return sf::Keyboard::Numpad3;
         * case 0x:  return sf::Keyboard::Numpad4;
         * case 0x:  return sf::Keyboard::Numpad5;
         * case 0x:  return sf::Keyboard::Numpad6;
         * case 0x:  return sf::Keyboard::Numpad7;
         * case 0x:  return sf::Keyboard::Numpad8;
         * case 0x:  return sf::Keyboard::Numpad9;
         */

        case NSF1FunctionKey:   return sf::Keyboard::F1;
        case NSF2FunctionKey:   return sf::Keyboard::F2;
        case NSF3FunctionKey:   return sf::Keyboard::F3;
        case NSF4FunctionKey:   return sf::Keyboard::F4;
        case NSF5FunctionKey:   return sf::Keyboard::F5;
        case NSF6FunctionKey:   return sf::Keyboard::F6;
        case NSF7FunctionKey:   return sf::Keyboard::F7;
        case NSF8FunctionKey:   return sf::Keyboard::F8;
        case NSF9FunctionKey:   return sf::Keyboard::F9;
        case NSF10FunctionKey:  return sf::Keyboard::F10;
        case NSF11FunctionKey:  return sf::Keyboard::F11;
        case NSF12FunctionKey:  return sf::Keyboard::F12;
        case NSF13FunctionKey:  return sf::Keyboard::F13;
        case NSF14FunctionKey:  return sf::Keyboard::F14;
        case NSF15FunctionKey:  return sf::Keyboard::F15;

        case NSPauseFunctionKey:  return sf::Keyboard::Pause;

        default:  return sf::Keyboard::Unknown;
    }
}


////////////////////////////////////////////////////////
UniChar HIDInputManager::toUnicode(Keyboard::Key key)
{
    switch (key)
    {
        case sf::Keyboard::A:  return 0x41;
        case sf::Keyboard::B:  return 0x42;
        case sf::Keyboard::C:  return 0x43;
        case sf::Keyboard::D:  return 0x44;
        case sf::Keyboard::E:  return 0x45;
        case sf::Keyboard::F:  return 0x46;
        case sf::Keyboard::G:  return 0x47;
        case sf::Keyboard::H:  return 0x48;
        case sf::Keyboard::I:  return 0x49;
        case sf::Keyboard::J:  return 0x4a;
        case sf::Keyboard::K:  return 0x4b;
        case sf::Keyboard::L:  return 0x4c;
        case sf::Keyboard::M:  return 0x4d;
        case sf::Keyboard::N:  return 0x4e;
        case sf::Keyboard::O:  return 0x4f;
        case sf::Keyboard::P:  return 0x50;
        case sf::Keyboard::Q:  return 0x51;
        case sf::Keyboard::R:  return 0x52;
        case sf::Keyboard::S:  return 0x53;
        case sf::Keyboard::T:  return 0x54;
        case sf::Keyboard::U:  return 0x55;
        case sf::Keyboard::V:  return 0x56;
        case sf::Keyboard::W:  return 0x57;
        case sf::Keyboard::X:  return 0x58;
        case sf::Keyboard::Y:  return 0x59;
        case sf::Keyboard::Z:  return 0x5a;

        case sf::Keyboard::Num0:  return 0x30;
        case sf::Keyboard::Num1:  return 0x31;
        case sf::Keyboard::Num2:  return 0x32;
        case sf::Keyboard::Num3:  return 0x33;
        case sf::Keyboard::Num4:  return 0x34;
        case sf::Keyboard::Num5:  return 0x35;
        case sf::Keyboard::Num6:  return 0x36;
        case sf::Keyboard::Num7:  return 0x37;
        case sf::Keyboard::Num8:  return 0x38;
        case sf::Keyboard::Num9:  return 0x39;

        case sf::Keyboard::Escape:  return 0x1b;

        /* Not representable as Unicode:
         * case sf::Keyboard::LControl:  return 0x;
         * case sf::Keyboard::LShift:    return 0x;
         * case sf::Keyboard::LAlt:      return 0x;
         * case sf::Keyboard::LSystem:   return 0x;
         * case sf::Keyboard::RControl:  return 0x;
         * case sf::Keyboard::RShift:    return 0x;
         * case sf::Keyboard::RAlt:      return 0x;
         * case sf::Keyboard::RSystem:   return 0x;
         */

        case sf::Keyboard::Menu:  return NSMenuFunctionKey;

        case sf::Keyboard::LBracket:    return 0x5b;
        case sf::Keyboard::RBracket:    return 0x5d;
        case sf::Keyboard::SemiColon:   return 0x3b;
        case sf::Keyboard::Comma:       return 0x2c;
        case sf::Keyboard::Period:      return 0x2e;
        case sf::Keyboard::Quote:       return 0x27;
        case sf::Keyboard::Slash:       return 0x2044; // 0x2f is Divide
        case sf::Keyboard::BackSlash:   return 0x5c;
        case sf::Keyboard::Tilde:       return 0x7e;
        case sf::Keyboard::Equal:       return 0x3d;
        case sf::Keyboard::Dash:        return 0x2013; // 0x2d is Subtract
        case sf::Keyboard::Space:       return 0x20;
        case sf::Keyboard::Return:      return 0x0d;
        case sf::Keyboard::BackSpace:   return 0x7f; // NOTE backspace & delete are swapped on Mac.
        case sf::Keyboard::Tab:         return 0x09;

        case sf::Keyboard::PageUp:    return NSPageUpFunctionKey;
        case sf::Keyboard::PageDown:  return NSPageDownFunctionKey;
        case sf::Keyboard::End:       return NSEndFunctionKey;
        case sf::Keyboard::Home:      return NSHomeFunctionKey;
        case sf::Keyboard::Insert:    return NSInsertFunctionKey;
        case sf::Keyboard::Delete:    return NSDeleteFunctionKey;

        case sf::Keyboard::Add:       return 0x2b;
        case sf::Keyboard::Subtract:  return 0x2d;
        case sf::Keyboard::Multiply:  return 0x2a;
        case sf::Keyboard::Divide:    return 0x2f;

        case sf::Keyboard::Left:    return NSLeftArrowFunctionKey;
        case sf::Keyboard::Right:   return NSRightArrowFunctionKey;
        case sf::Keyboard::Up:      return NSUpArrowFunctionKey;
        case sf::Keyboard::Down:    return NSDownArrowFunctionKey;

        /* Those are not represented using Unicode.
         * case sf::Keyboard::Numpad0:  return 0x;
         * case sf::Keyboard::Numpad1:  return 0x;
         * case sf::Keyboard::Numpad2:  return 0x;
         * case sf::Keyboard::Numpad3:  return 0x;
         * case sf::Keyboard::Numpad4:  return 0x;
         * case sf::Keyboard::Numpad5:  return 0x;
         * case sf::Keyboard::Numpad6:  return 0x;
         * case sf::Keyboard::Numpad7:  return 0x;
         * case sf::Keyboard::Numpad8:  return 0x;
         * case sf::Keyboard::Numpad9:  return 0x;
         */

        case sf::Keyboard::F1:   return NSF1FunctionKey;
        case sf::Keyboard::F2:   return NSF2FunctionKey;
        case sf::Keyboard::F3:   return NSF3FunctionKey;
        case sf::Keyboard::F4:   return NSF4FunctionKey;
        case sf::Keyboard::F5:   return NSF5FunctionKey;
        case sf::Keyboard::F6:   return NSF6FunctionKey;
        case sf::Keyboard::F7:   return NSF7FunctionKey;
        case sf::Keyboard::F8:   return NSF8FunctionKey;
        case sf::Keyboard::F9:   return NSF9FunctionKey;
        case sf::Keyboard::F10:  return NSF10FunctionKey;
        case sf::Keyboard::F11:  return NSF11FunctionKey;
        case sf::Keyboard::F12:  return NSF12FunctionKey;
        case sf::Keyboard::F13:  return NSF13FunctionKey;
        case sf::Keyboard::F14:  return NSF14FunctionKey;
        case sf::Keyboard::F15:  return NSF15FunctionKey;

        case sf::Keyboard::Pause:  return NSPauseFunctionKey;

        default:  return 0x00;
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
        case 0x00:  return sf::Keyboard::sA;
        case 0x0b:  return sf::Keyboard::sB;
        case 0x08:  return sf::Keyboard::sC;
        case 0x02:  return sf::Keyboard::sD;
        case 0x0e:  return sf::Keyboard::sE;
        case 0x03:  return sf::Keyboard::sF;
        case 0x05:  return sf::Keyboard::sG;
        case 0x04:  return sf::Keyboard::sH;
        case 0x22:  return sf::Keyboard::sI;
        case 0x26:  return sf::Keyboard::sJ;
        case 0x28:  return sf::Keyboard::sK;
        case 0x25:  return sf::Keyboard::sL;
        case 0x2e:  return sf::Keyboard::sM;
        case 0x2d:  return sf::Keyboard::sN;
        case 0x1f:  return sf::Keyboard::sO;
        case 0x23:  return sf::Keyboard::sP;
        case 0x0c:  return sf::Keyboard::sQ;
        case 0x0f:  return sf::Keyboard::sR;
        case 0x01:  return sf::Keyboard::sS;
        case 0x11:  return sf::Keyboard::sT;
        case 0x20:  return sf::Keyboard::sU;
        case 0x09:  return sf::Keyboard::sV;
        case 0x0d:  return sf::Keyboard::sW;
        case 0x07:  return sf::Keyboard::sX;
        case 0x10:  return sf::Keyboard::sY;
        case 0x06:  return sf::Keyboard::sZ;

        case 0x12:  return sf::Keyboard::sNum1;
        case 0x13:  return sf::Keyboard::sNum2;
        case 0x14:  return sf::Keyboard::sNum3;
        case 0x15:  return sf::Keyboard::sNum4;
        case 0x17:  return sf::Keyboard::sNum5;
        case 0x16:  return sf::Keyboard::sNum6;
        case 0x1a:  return sf::Keyboard::sNum7;
        case 0x1c:  return sf::Keyboard::sNum8;
        case 0x19:  return sf::Keyboard::sNum9;
        case 0x1d:  return sf::Keyboard::sNum0;

        case 0x24:  return sf::Keyboard::sEnter;
        case 0x35:  return sf::Keyboard::sEscape;
        case 0x33:  return sf::Keyboard::sBackspace;
        case 0x30:  return sf::Keyboard::sTab;
        case 0x31:  return sf::Keyboard::sSpace;
        // case 0x27:  return sf::Keyboard::sHyphen; // TODO 0x27 is for Quote
        case 0x18:  return sf::Keyboard::sEquals;
        case 0x21:  return sf::Keyboard::sLBracket;
        case 0x1e:  return sf::Keyboard::sRBracket;
        case 0x2a:  return sf::Keyboard::sBackslash;
        case 0x1b:  return sf::Keyboard::sDash;
        case 0x29:  return sf::Keyboard::sSemicolon;
        case 0x27:  return sf::Keyboard::sQuote;
        case 0x32:  return sf::Keyboard::sGraveAccent;
        case 0x2b:  return sf::Keyboard::sComma;
        case 0x2f:  return sf::Keyboard::sPeriod;
        case 0x2c:  return sf::Keyboard::sForwardSlash;

        case 0x7a:  return sf::Keyboard::sF1;
        case 0x78:  return sf::Keyboard::sF2;
        case 0x63:  return sf::Keyboard::sF3;
        case 0x76:  return sf::Keyboard::sF4;
        case 0x60:  return sf::Keyboard::sF5;
        case 0x61:  return sf::Keyboard::sF6;
        case 0x62:  return sf::Keyboard::sF7;
        case 0x64:  return sf::Keyboard::sF8;
        case 0x65:  return sf::Keyboard::sF9;
        case 0x6d:  return sf::Keyboard::sF10;
        case 0x67:  return sf::Keyboard::sF11;
        case 0x6f:  return sf::Keyboard::sF12;
        case 0x69:  return sf::Keyboard::sF13;
        case 0x6b:  return sf::Keyboard::sF14;
        case 0x71:  return sf::Keyboard::sF15;

        case 0x39:  return sf::Keyboard::sCapsLock;

        /* TODO Those are missing:
         * case 0x:  return sf::Keyboard::sPrintScreen;
         * case 0x:  return sf::Keyboard::sScrollLock;
         * case 0x:  return sf::Keyboard::sPause;
         */

        case 0x72:  return sf::Keyboard::sInsert;
        case 0x73:  return sf::Keyboard::sHome;
        case 0x74:  return sf::Keyboard::sPageUp;
        case 0x75:  return sf::Keyboard::sDelete;
        case 0x77:  return sf::Keyboard::sEnd;
        case 0x79:  return sf::Keyboard::sPageDown;

        case 0x7c:  return sf::Keyboard::sRight;
        case 0x7b:  return sf::Keyboard::sLeft;
        case 0x7d:  return sf::Keyboard::sDown;
        case 0x7e:  return sf::Keyboard::sUp;

        case 0x47:  return sf::Keyboard::sNumLock;
        case 0x4b:  return sf::Keyboard::sDivide;
        case 0x43:  return sf::Keyboard::sMultiply;
        case 0x4e:  return sf::Keyboard::sMinus;
        case 0x45:  return sf::Keyboard::sPlus;
        case 0x51:  return sf::Keyboard::sPadEquals;
        case 0x4c:  return sf::Keyboard::sReturn;
        case 0x41:  return sf::Keyboard::sDecimal;

        case 0x53:  return sf::Keyboard::sNumpad1;
        case 0x54:  return sf::Keyboard::sNumpad2;
        case 0x55:  return sf::Keyboard::sNumpad3;
        case 0x56:  return sf::Keyboard::sNumpad4;
        case 0x57:  return sf::Keyboard::sNumpad5;
        case 0x58:  return sf::Keyboard::sNumpad6;
        case 0x59:  return sf::Keyboard::sNumpad7;
        case 0x5b:  return sf::Keyboard::sNumpad8;
        case 0x5c:  return sf::Keyboard::sNumpad9;
        case 0x52:  return sf::Keyboard::sNumpad0;

        /* TODO Those are missing:
         * case 0x:  return sf::Keyboard::sReverseSolidus;
         * case 0x:  return sf::Keyboard::sApplication;
         * case 0x:  return sf::Keyboard::sExecute;
         * case 0x72:  return sf::Keyboard::sHelp; // 0x72 is for Insert
         * case 0x:  return sf::Keyboard::sMenu;
         * case 0x:  return sf::Keyboard::sSelect;
         * case 0x:  return sf::Keyboard::sStop;
         * case 0x:  return sf::Keyboard::sAgain;
         * case 0x:  return sf::Keyboard::sUndo;
         * case 0x:  return sf::Keyboard::sCut;
         * case 0x:  return sf::Keyboard::sCopy;
         * case 0x:  return sf::Keyboard::sPaste;
         * case 0x:  return sf::Keyboard::sFind;
         */

        case 0x4a:  return sf::Keyboard::sMute;
        case 0x48:  return sf::Keyboard::sVolumeUp;
        case 0x49:  return sf::Keyboard::sVolumeDown;

        /* NOTE Those are symmetric so we leave them out.
         *      Thankfully handled through modifiers and not virtual codes.
         * case 0x3b:  return sf::Keyboard::sLControl;
         * case 0x38:  return sf::Keyboard::sLShift;
         * case 0x3a:  return sf::Keyboard::sLAlt;
         * case 0x37:  return sf::Keyboard::sLSystem;
         * case 0x3b:  return sf::Keyboard::sRControl;
         * case 0x38:  return sf::Keyboard::sRShift;
         * case 0x3a:  return sf::Keyboard::sRAlt;
         * case 0x37:  return sf::Keyboard::sRSystem;
         */

        default:  return sf::Keyboard::sUnknown;
    }
}


////////////////////////////////////////////////////////
UInt8 HIDInputManager::scanToVirtualCode(Keyboard::Scancode code)
{
    switch (code)
    {
        case sf::Keyboard::sA:  return 0x00;
        case sf::Keyboard::sB:  return 0x0b;
        case sf::Keyboard::sC:  return 0x08;
        case sf::Keyboard::sD:  return 0x02;
        case sf::Keyboard::sE:  return 0x0e;
        case sf::Keyboard::sF:  return 0x03;
        case sf::Keyboard::sG:  return 0x05;
        case sf::Keyboard::sH:  return 0x04;
        case sf::Keyboard::sI:  return 0x22;
        case sf::Keyboard::sJ:  return 0x26;
        case sf::Keyboard::sK:  return 0x28;
        case sf::Keyboard::sL:  return 0x25;
        case sf::Keyboard::sM:  return 0x2e;
        case sf::Keyboard::sN:  return 0x2d;
        case sf::Keyboard::sO:  return 0x1f;
        case sf::Keyboard::sP:  return 0x23;
        case sf::Keyboard::sQ:  return 0x0c;
        case sf::Keyboard::sR:  return 0x0f;
        case sf::Keyboard::sS:  return 0x01;
        case sf::Keyboard::sT:  return 0x11;
        case sf::Keyboard::sU:  return 0x20;
        case sf::Keyboard::sV:  return 0x09;
        case sf::Keyboard::sW:  return 0x0d;
        case sf::Keyboard::sX:  return 0x07;
        case sf::Keyboard::sY:  return 0x10;
        case sf::Keyboard::sZ:  return 0x06;

        case sf::Keyboard::sNum1:  return 0x12;
        case sf::Keyboard::sNum2:  return 0x13;
        case sf::Keyboard::sNum3:  return 0x14;
        case sf::Keyboard::sNum4:  return 0x15;
        case sf::Keyboard::sNum5:  return 0x17;
        case sf::Keyboard::sNum6:  return 0x16;
        case sf::Keyboard::sNum7:  return 0x1a;
        case sf::Keyboard::sNum8:  return 0x1c;
        case sf::Keyboard::sNum9:  return 0x19;
        case sf::Keyboard::sNum0:  return 0x1d;

        case sf::Keyboard::sEnter:      return 0x24;
        case sf::Keyboard::sEscape:     return 0x35;
        case sf::Keyboard::sBackspace:  return 0x33;
        case sf::Keyboard::sTab:        return 0x30;
        case sf::Keyboard::sSpace:      return 0x31;

        // case sf::Keyboard::sHyphen:       return 0; // 0x27 is for Quote

        case sf::Keyboard::sEquals:       return 0x18;
        case sf::Keyboard::sLBracket:     return 0x21;
        case sf::Keyboard::sRBracket:     return 0x1e;
        case sf::Keyboard::sBackslash:    return 0x2a;
        case sf::Keyboard::sDash:         return 0x1b;
        case sf::Keyboard::sSemicolon:    return 0x29;
        case sf::Keyboard::sQuote:        return 0x27;
        case sf::Keyboard::sGraveAccent:  return 0x32;
        case sf::Keyboard::sComma:        return 0x2b;
        case sf::Keyboard::sPeriod:       return 0x2f;
        case sf::Keyboard::sForwardSlash: return 0x2c;

        case sf::Keyboard::sF1:   return 0x7a;
        case sf::Keyboard::sF2:   return 0x78;
        case sf::Keyboard::sF3:   return 0x63;
        case sf::Keyboard::sF4:   return 0x76;
        case sf::Keyboard::sF5:   return 0x60;
        case sf::Keyboard::sF6:   return 0x61;
        case sf::Keyboard::sF7:   return 0x62;
        case sf::Keyboard::sF8:   return 0x64;
        case sf::Keyboard::sF9:   return 0x65;
        case sf::Keyboard::sF10:  return 0x6d;
        case sf::Keyboard::sF11:  return 0x67;
        case sf::Keyboard::sF12:  return 0x6f;
        case sf::Keyboard::sF13:  return 0x69;
        case sf::Keyboard::sF14:  return 0x6b;
        case sf::Keyboard::sF15:  return 0x71;

        case sf::Keyboard::sCapsLock:    return 0x39;

        /* TODO Those are missing:
         * case sf::Keyboard::sPrintScreen: return 0;
         * case sf::Keyboard::sScrollLock:  return 0;
         * case sf::Keyboard::sPause:    return 0;
         */
        case sf::Keyboard::sInsert:   return 0x72;
        case sf::Keyboard::sHome:     return 0x73;
        case sf::Keyboard::sPageUp:   return 0x74;
        case sf::Keyboard::sDelete:   return 0x75;
        case sf::Keyboard::sEnd:      return 0x77;
        case sf::Keyboard::sPageDown: return 0x79;

        case sf::Keyboard::sRight:  return 0x7c;
        case sf::Keyboard::sLeft:   return 0x7b;
        case sf::Keyboard::sDown:   return 0x7d;
        case sf::Keyboard::sUp:     return 0x7e;

        case sf::Keyboard::sNumLock:   return 0x47;
        case sf::Keyboard::sDivide:    return 0x4b;
        case sf::Keyboard::sMultiply:  return 0x43;
        case sf::Keyboard::sMinus:     return 0x4e;
        case sf::Keyboard::sPlus:      return 0x45;
        case sf::Keyboard::sPadEquals: return 0x51;
        case sf::Keyboard::sReturn:    return 0x4c;
        case sf::Keyboard::sDecimal:   return 0x41;

        case sf::Keyboard::sNumpad1:  return 0x53;
        case sf::Keyboard::sNumpad2:  return 0x54;
        case sf::Keyboard::sNumpad3:  return 0x55;
        case sf::Keyboard::sNumpad4:  return 0x56;
        case sf::Keyboard::sNumpad5:  return 0x57;
        case sf::Keyboard::sNumpad6:  return 0x58;
        case sf::Keyboard::sNumpad7:  return 0x59;
        case sf::Keyboard::sNumpad8:  return 0x5b;
        case sf::Keyboard::sNumpad9:  return 0x5c;
        case sf::Keyboard::sNumpad0:  return 0x52;

        /* TODO Those are missing:
         * case sf::Keyboard::sReverseSolidus: return 0;
         * case sf::Keyboard::sApplication: return 0;
         * case sf::Keyboard::sExecute:     return 0;
         * case sf::Keyboard::sHelp:        return 0;
         * case sf::Keyboard::sMenu:        return 0;
         * case sf::Keyboard::sSelect:      return 0;
         * case sf::Keyboard::sStop:        return 0;
         * case sf::Keyboard::sAgain:       return 0;
         * case sf::Keyboard::sUndo:        return 0;
         * case sf::Keyboard::sCut:         return 0;
         * case sf::Keyboard::sCopy:        return 0;
         * case sf::Keyboard::sPaste:       return 0;
         * case sf::Keyboard::sFind:        return 0;
         */

        case sf::Keyboard::sMute:        return 0x4a;
        case sf::Keyboard::sVolumeUp:    return 0x48;
        case sf::Keyboard::sVolumeDown:  return 0x49;

        case sf::Keyboard::sLControl:  return 0x3b;
        case sf::Keyboard::sLShift:    return 0x38;
        case sf::Keyboard::sLAlt:      return 0x3a;
        case sf::Keyboard::sLSystem:   return 0x37;
        case sf::Keyboard::sRControl:  return 0x3b;
        case sf::Keyboard::sRShift:    return 0x38;
        case sf::Keyboard::sRAlt:      return 0x3a;
        case sf::Keyboard::sRSystem:   return 0x37;

        default: return 0x00;
    }
}


////////////////////////////////////////////////////////
Keyboard::Key HIDInputManager::localizedKeyFallback(Keyboard::Scancode code)
{
    switch (code)
    {
        case sf::Keyboard::sEnter:      return sf::Keyboard::Return;
        case sf::Keyboard::sEscape:     return sf::Keyboard::Escape;
        case sf::Keyboard::sBackspace:  return sf::Keyboard::BackSpace;
        case sf::Keyboard::sTab:        return sf::Keyboard::Tab;
        case sf::Keyboard::sSpace:      return sf::Keyboard::Space;

        case sf::Keyboard::sF1:   return sf::Keyboard::F1;
        case sf::Keyboard::sF2:   return sf::Keyboard::F2;
        case sf::Keyboard::sF3:   return sf::Keyboard::F3;
        case sf::Keyboard::sF4:   return sf::Keyboard::F4;
        case sf::Keyboard::sF5:   return sf::Keyboard::F5;
        case sf::Keyboard::sF6:   return sf::Keyboard::F6;
        case sf::Keyboard::sF7:   return sf::Keyboard::F7;
        case sf::Keyboard::sF8:   return sf::Keyboard::F8;
        case sf::Keyboard::sF9:   return sf::Keyboard::F9;
        case sf::Keyboard::sF10:  return sf::Keyboard::F10;
        case sf::Keyboard::sF11:  return sf::Keyboard::F11;
        case sf::Keyboard::sF12:  return sf::Keyboard::F12;
        case sf::Keyboard::sF13:  return sf::Keyboard::F13;
        case sf::Keyboard::sF14:  return sf::Keyboard::F14;
        case sf::Keyboard::sF15:  return sf::Keyboard::F15;

        case sf::Keyboard::sPause:    return sf::Keyboard::Pause;
        case sf::Keyboard::sInsert:   return sf::Keyboard::Insert;
        case sf::Keyboard::sHome:     return sf::Keyboard::Home;
        case sf::Keyboard::sPageUp:   return sf::Keyboard::PageUp;
        case sf::Keyboard::sDelete:   return sf::Keyboard::Delete;
        case sf::Keyboard::sEnd:      return sf::Keyboard::End;
        case sf::Keyboard::sPageDown: return sf::Keyboard::PageDown;

        case sf::Keyboard::sRight:  return sf::Keyboard::Right;
        case sf::Keyboard::sLeft:   return sf::Keyboard::Left;
        case sf::Keyboard::sDown:   return sf::Keyboard::Down;
        case sf::Keyboard::sUp:     return sf::Keyboard::Up;

        case sf::Keyboard::sDivide:   return sf::Keyboard::Divide;
        case sf::Keyboard::sMultiply: return sf::Keyboard::Multiply;
        case sf::Keyboard::sMinus:    return sf::Keyboard::Subtract;
        case sf::Keyboard::sPlus:     return sf::Keyboard::Add;

        case sf::Keyboard::sNumpad0:  return sf::Keyboard::Numpad1;
        case sf::Keyboard::sNumpad1:  return sf::Keyboard::Numpad2;
        case sf::Keyboard::sNumpad2:  return sf::Keyboard::Numpad3;
        case sf::Keyboard::sNumpad3:  return sf::Keyboard::Numpad4;
        case sf::Keyboard::sNumpad4:  return sf::Keyboard::Numpad5;
        case sf::Keyboard::sNumpad5:  return sf::Keyboard::Numpad6;
        case sf::Keyboard::sNumpad6:  return sf::Keyboard::Numpad7;
        case sf::Keyboard::sNumpad7:  return sf::Keyboard::Numpad8;
        case sf::Keyboard::sNumpad8:  return sf::Keyboard::Numpad9;
        case sf::Keyboard::sNumpad9:  return sf::Keyboard::Numpad0;

        case sf::Keyboard::sLControl:  return sf::Keyboard::LControl;
        case sf::Keyboard::sLShift:    return sf::Keyboard::LShift;
        case sf::Keyboard::sLAlt:      return sf::Keyboard::LAlt;
        case sf::Keyboard::sLSystem:   return sf::Keyboard::LSystem;
        case sf::Keyboard::sRControl:  return sf::Keyboard::RControl;
        case sf::Keyboard::sRShift:    return sf::Keyboard::RShift;
        case sf::Keyboard::sRAlt:      return sf::Keyboard::RAlt;
        case sf::Keyboard::sRSystem:   return sf::Keyboard::RSystem;

        default: return sf::Keyboard::Unknown;
    }
}


} // namespace priv

} // namespace sf

