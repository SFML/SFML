////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2011 Marco Antognini (antognini.marco@gmail.com), 
//                         Laurent Gomila (laurent.gom@gmail.com), 
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
#include <SFML/Window/OSX/HIDJoystickManager.hpp>
#include <SFML/Window/OSX/HIDInputManager.hpp>

////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
    // Using a custom run loop mode solve some issues that appears when SFML
    // is used with Cocoa.
    CFStringRef const runLoopMode = CFSTR("SFML_RUN_LOOP_MODE");
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
HIDJoystickManager& HIDJoystickManager::GetInstance()
{
    static HIDJoystickManager manager;
    return manager;
}


////////////////////////////////////////////////////////////
unsigned int HIDJoystickManager::GetJoystickCount()
{
    Update();
    return myJoystickCount;
}


////////////////////////////////////////////////////////////
CFSetRef HIDJoystickManager::CopyJoysticks()
{
    CFSetRef devices = IOHIDManagerCopyDevices(myHIDManager);
    return devices;
}


////////////////////////////////////////////////////////////
HIDJoystickManager::HIDJoystickManager()
: myHIDManager(0)
, myJoystickCount(0)
{
    myHIDManager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    
    CFDictionaryRef mask0 = HIDInputManager::CopyDevicesMask(kHIDPage_GenericDesktop,
                                                             kHIDUsage_GD_Joystick);
    
    CFDictionaryRef mask1 = HIDInputManager::CopyDevicesMask(kHIDPage_GenericDesktop,
                                                             kHIDUsage_GD_GamePad);
    
    CFDictionaryRef maskArray[2];
    maskArray[0] = mask0;
    maskArray[1] = mask1;
    
    CFArrayRef mask = CFArrayCreate(NULL, (const void**)maskArray, 2, NULL);
    
    IOHIDManagerSetDeviceMatchingMultiple(myHIDManager, mask);
    CFRelease(mask);
    CFRelease(mask1);
    CFRelease(mask0);
    
    
    IOHIDManagerRegisterDeviceMatchingCallback(myHIDManager, pluggedIn, this);
    IOHIDManagerRegisterDeviceRemovalCallback(myHIDManager, pluggedOut, this);
    
    IOHIDManagerScheduleWithRunLoop(myHIDManager, 
                                    CFRunLoopGetCurrent(), 
                                    runLoopMode);
    
    IOHIDManagerOpen(myHIDManager, kIOHIDOptionsTypeNone);
}


////////////////////////////////////////////////////////////
HIDJoystickManager::~HIDJoystickManager()
{
    IOHIDManagerUnscheduleFromRunLoop(myHIDManager, 
                                      CFRunLoopGetCurrent(), 
                                      runLoopMode);
    
    IOHIDManagerRegisterDeviceMatchingCallback(myHIDManager, NULL, 0);
    IOHIDManagerRegisterDeviceRemovalCallback(myHIDManager, NULL, 0);
    
    IOHIDManagerClose(myHIDManager, kIOHIDOptionsTypeNone);
}


////////////////////////////////////////////////////////////
void HIDJoystickManager::Update()
{
    SInt32 status = kCFRunLoopRunHandledSource;
    
    while (status == kCFRunLoopRunHandledSource) {
        status = CFRunLoopRunInMode(runLoopMode, 0, true);
    }
}


////////////////////////////////////////////////////////////
void HIDJoystickManager::pluggedIn(void * context, IOReturn, void *, IOHIDDeviceRef)
{
    HIDJoystickManager* manager = (HIDJoystickManager*)context;
    manager->myJoystickCount++;
}


////////////////////////////////////////////////////////////
void HIDJoystickManager::pluggedOut(void * context, IOReturn, void *, IOHIDDeviceRef)
{
    HIDJoystickManager* manager = (HIDJoystickManager*)context;
    manager->myJoystickCount--;
}


} // namespace priv

} // namespace sf

