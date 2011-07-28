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
#include <SFML/Window/JoystickImpl.hpp>
#include <SFML/Window/OSX/HIDInputManager.hpp>
#include <SFML/Window/OSX/HIDJoystickManager.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
JoystickImpl::Location JoystickImpl::myLocationIDs[sf::Joystick::Count] = { 0 };


////////////////////////////////////////////////////////////
bool JoystickImpl::IsConnected(unsigned int index)
{
    bool state = false; // Is the index-th joystick connected ?
    
    // First, let's check if the device was previously detected :
    
    if (myLocationIDs[index] != 0) {
        state = true;
    }
    
    // Otherwise, let's check if it is now connected :
    else { // i.e., myLocationIDs[index] == 0
        
        // if there is more connected joystick to the HID manager than
        // opened joystick devices then we find the new one.
        
        unsigned int openedCount = 0;
        for (unsigned int i(0); i < sf::Joystick::Count; ++i) {
            if (myLocationIDs[i] != 0) openedCount++;
        }
        
        unsigned int connectedCount = HIDJoystickManager::GetInstance().GetJoystickCount();
        
        if (connectedCount > openedCount) {
        
            // Get all devices
            CFSetRef devices = HIDJoystickManager::GetInstance().CopyJoysticks();
            
            if (devices != NULL) {
                
                CFIndex size = CFSetGetCount(devices);
                
                if (size > 0) {
                    
                    CFTypeRef array[size]; // array of IOHIDDeviceRef
                    CFSetGetValues(devices, array);
                    
                    // If there exists a device d s.t. there is no j s.t.
                    // myLocationIDs[j] == d's location then we have a new device.
                    
                    for (CFIndex didx(0); didx < size; ++didx) {
                        IOHIDDeviceRef d = (IOHIDDeviceRef)array[didx];
                        Location dloc = HIDInputManager::GetLocationID(d);
                        
                        bool foundJ = false;
                        for (unsigned int j(0); j < Joystick::Count; ++j) {
                            if (myLocationIDs[j] == dloc) {
                                foundJ = true;
                                break; // no need to loop again
                            }
                        }
                        
                        if (foundJ) {
                            // This is a known device
                            // Nothing else to do
                        } else {
                            // This is a new device
                            // We set it up for Open(..)
                            myLocationIDs[index] = dloc;
                            state = true;
                            break; // We stop looking for a new device
                        }
                    }
                    
                }
                
                CFRelease(devices);
            }
        }
    }
    
    return state;
}


////////////////////////////////////////////////////////////
bool JoystickImpl::Open(unsigned int index)
{
    myIndex = index;
    Location deviceLoc = myLocationIDs[index]; // The device we need to load
    
    // Get all devices
    CFSetRef devices = HIDJoystickManager::GetInstance().CopyJoysticks();
    if (devices == NULL) {
        return false;
    }
    
    // Get a usable copy of the joysticks devices.
    CFIndex joysticksCount = CFSetGetCount(devices);
    CFTypeRef devicesArray[joysticksCount];
    CFSetGetValues(devices, devicesArray);
    
    // Get the desired joystick.
    IOHIDDeviceRef self = 0;
    for (CFIndex i(0); i < joysticksCount; ++i) {
        IOHIDDeviceRef d = (IOHIDDeviceRef)devicesArray[i];
        if (deviceLoc == HIDInputManager::GetLocationID(d)) {
            self = d;
            break; // We found it so we stop looping.
        }
    }
    
    if (self == 0) {
        // This shouldn't happen!
        CFRelease(devices);
        return false;
    }
    
    // Get a list of all elements attached to the device.
    CFArrayRef elements = IOHIDDeviceCopyMatchingElements(self, 
                                                          NULL, 
                                                          kIOHIDOptionsTypeNone);
    
    if (elements == NULL) {
        CFRelease(devices);
        return false;
    }
    
    // How many elements are there ?
    CFIndex elementsCount = CFArrayGetCount(elements);
    
    if (elementsCount == 0) {
        // What is a joystick with no element ?
        CFRelease(elements);
        CFRelease(devices);
        return false;
    }
        
    // Go through all connected elements.
    for (int i = 0; i < elementsCount; ++i) {
        IOHIDElementRef element = (IOHIDElementRef) CFArrayGetValueAtIndex(elements, i);
        
        switch (IOHIDElementGetType(element)) {
                
            case kIOHIDElementTypeInput_Misc:
                switch (IOHIDElementGetUsage(element)) {
                        
                    case kHIDUsage_GD_X:
                        myAxis[Joystick::X] = element;
                        break;
                        
                    case kHIDUsage_GD_Y:
                        myAxis[Joystick::Y] = element;
                        break;
                        
                    case kHIDUsage_GD_Z:
                        myAxis[Joystick::Z] = element;
                        break;
                        
                    case kHIDUsage_GD_Rx:
                        myAxis[Joystick::U] = element; 
                        break;
                        
                    case kHIDUsage_GD_Ry:
                        myAxis[Joystick::V] = element; 
                        break;
                        
                    case kHIDUsage_GD_Rz:
                        myAxis[Joystick::R] = element; 
                        break;
                        
                        // kHIDUsage_GD_Vx, kHIDUsage_GD_Vy, kHIDUsage_GD_Vz are ignored.
                }
                break;
                
            case kIOHIDElementTypeInput_Button:
                if (myButtons.size() < Joystick::ButtonCount) { // If we have free slot...
                    myButtons.push_back(element); // ...we add this element to the list
                } else {
                    // Too many buttons. We ignore this one.
                }
                break;
                
            default: // Make compiler happy
                break;
        }
    }
    
    // Note : Joy::AxisPovX/Y are not supported (yet).
    // Maybe kIOHIDElementTypeInput_Axis is the corresponding type but I can't test.
    
    // Retain all these objets for personal use
    for (ButtonsVector::iterator it(myButtons.begin()); it != myButtons.end(); ++it) {
        CFRetain(*it);
    }
    for (AxisMap::iterator it(myAxis.begin()); it != myAxis.end(); ++it) {
        CFRetain(it->second);
    }
    
    // Note : we didn't retain element in the switch because we might have multiple
    // Axis X (for example) and we want to keep only the last one. So to prevent
    // leaking we retain objects 'only' now.
    
    CFRelease(devices);
    CFRelease(elements);
    
    return true;
}


////////////////////////////////////////////////////////////
void JoystickImpl::Close()
{
    for (ButtonsVector::iterator it(myButtons.begin()); it != myButtons.end(); ++it) {
        CFRelease(*it);
    }
    myButtons.clear();
    
    for (AxisMap::iterator it(myAxis.begin()); it != myAxis.end(); ++it) {
        CFRelease(it->second);
    }
    myAxis.clear();
    
    // And we unregister this joystick
    myLocationIDs[myIndex] = 0;
}


////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::GetCapabilities() const
{
    JoystickCaps caps;
    
    // Buttons :
    caps.ButtonCount = myButtons.size();
    
    // Axis :
    for (AxisMap::const_iterator it(myAxis.begin()); it != myAxis.end(); ++it) {
        caps.Axes[it->first] = true;
    }
    
    return caps;
}


////////////////////////////////////////////////////////////
JoystickState JoystickImpl::Update()
{
    static const JoystickState disconnectedState; // return this if joystick was disconnected
    JoystickState       state; // otherwise return that
    state.Connected = true;
    
    // Note : free up is done in Close() which is called, if required,
    //        by the joystick manager. So we don't release buttons nor axes here.
    
    // First, let's determine if the joystick is still connected
    Location selfLoc = myLocationIDs[myIndex];
    
    // Get all devices
    CFSetRef devices = HIDJoystickManager::GetInstance().CopyJoysticks();
    if (devices == NULL) {
        return disconnectedState;
    }
    
    // Get a usable copy of the joysticks devices.
    CFIndex joysticksCount = CFSetGetCount(devices);
    CFTypeRef devicesArray[joysticksCount];
    CFSetGetValues(devices, devicesArray);
    
    // Search for it
    bool found = false;
    for (CFIndex i(0); i < joysticksCount; ++i) {
        IOHIDDeviceRef d = (IOHIDDeviceRef)devicesArray[i];
        if (selfLoc == HIDInputManager::GetLocationID(d)) {
            found = true;
            break; // Stop looping
        }
    }
    
    // Release unused stuff
    CFRelease(devices);
    
    // Was it found ?
    if (found) {
        // Yes, so we can continue.
    } else {
        // No, so we stop here
        return disconnectedState;
    }
    
    // Update buttons' state
    unsigned int i = 0;
    for (ButtonsVector::iterator it(myButtons.begin()); it != myButtons.end(); ++it, ++i) {
        IOHIDValueRef value = 0;
        IOHIDDeviceGetValue(IOHIDElementGetDevice(*it), *it, &value);
        
        // Check for plug out.
        if (!value) {
            // No value ? Hum... Seems like the joystick is gone
            return disconnectedState;
        }
        
        // 1 means pressed, others mean released
        state.Buttons[i] = IOHIDValueGetIntegerValue(value) == 1;
    }
    
    // Update axes' state
    for (AxisMap::iterator it = myAxis.begin(); it != myAxis.end(); ++it) {
        IOHIDValueRef value = 0;
        IOHIDDeviceGetValue(IOHIDElementGetDevice(it->second), it->second, &value);
        
        // Check for plug out.
        if (!value) {
            // No value ? Hum... Seems like the joystick is gone
            return disconnectedState;
        }
        
        // We want to bind [physicalMin,physicalMax] to [-100=min,100=max].
        //
        // General formula to bind [a,b] to [c,d] with a linear progression :
        // 
        // f : [a, b] -> [c, d]
        //        x  |->  (x-a)(d-c)/(b-a)+c
        //
        // This method might not be very accurate (the "0 position" can be
        // slightly shift with some device) but we don't care because most
        // of devices are so sensitive that this is not relevant.
        double  physicalMax   = IOHIDElementGetPhysicalMax(it->second);
        double  physicalMin   = IOHIDElementGetPhysicalMin(it->second);
        double  scaledMin     = -100;
        double  scaledMax     =  100;
        double  physicalValue = IOHIDValueGetScaledValue(value, kIOHIDValueScaleTypePhysical);
        float   scaledValue   = ((physicalValue - physicalMin) * (scaledMax - scaledMin) / (physicalMax - physicalMin)) + scaledMin;
        state.Axes[it->first] = scaledValue;
    }
    
    
    return state;
}

} // namespace priv

} // namespace sf
