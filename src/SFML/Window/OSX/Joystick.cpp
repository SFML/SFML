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
#include <SFML/Window/Joystick.hpp>
#include <SFML/System/Err.hpp>
#include <sstream>


namespace sf
{
namespace priv
{

////////////////////////////////////////////////////////////
Joystick::Joystick()
: myManager(0)
, myElements(0)
{
    /* Nothing else */
}


////////////////////////////////////////////////////////////
Joystick::~Joystick()
{
    FreeUp();
}


////////////////////////////////////////////////////////////
void Joystick::Initialize(unsigned int Index)
{
    // Try to create a joystick manager.
    if (!CreateManager()) return;
    
    // Get the joysticks.
    CFSetRef devices = CopyJoysticksOnly();
    
    // If none exit the function.
    if (devices == NULL) {
        FreeUp();
        return;
    }

    // Is there enough joystick ?
    CFIndex joysticksCount = CFSetGetCount(devices);
    if (joysticksCount <= Index) {
        FreeUp();
        return;
    }
    
    // Get a usable copy of the joysticks devices.
    CFTypeRef devicesArray[joysticksCount];
    CFSetGetValues(devices, devicesArray);
    
    // Release unused stuff.
    CFRelease(devices); // Maybe we should have a field for that and not release it here...
    
    // Get the Index-th joystick.
    IOHIDDeviceRef device = (IOHIDDeviceRef) devicesArray[Index];
    
    // Retrive all connected elements to this joystick.
    if (!RetriveElements(device)) {
        FreeUp();
        return;
    }
    
    // Happy end!
}


////////////////////////////////////////////////////////////
JoystickState Joystick::UpdateState()
{
    // If we don't have any joystick we exit.
    if (myElements == 0) return JoystickState();
    
    // Fill a JoystickState instance with the current joystick state.
    JoystickState s;
    
    // Update the buttons.
    for (ButtonsVector::size_type i = 0; i < myButtons.size(); ++i) {
        IOHIDValueRef value = 0;
        IOHIDDeviceGetValue(IOHIDElementGetDevice(myButtons[i]), myButtons[i], &value);
        
        // Check for plug out.
        if (!value) {
            // No value ? Hum... Seems like the joystick is gone.
            
            FreeUp();
            return JoystickState();
        }
        
        s.Buttons[i] = IOHIDValueGetIntegerValue(value) == 1; // 1 means pressed, others mean released.
    }
    
    for (AxisMap::iterator it = myAxis.begin(); it != myAxis.end(); ++it) {
        IOHIDValueRef value = 0;
        IOHIDDeviceGetValue(IOHIDElementGetDevice(it->second), it->second, &value);
        
        // Check for plug out.
        if (!value) {
            // No value ? Hum... Seems like the joystick is gone.
            
            FreeUp();
            return JoystickState();
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
        s.Axis[it->first]     = scaledValue;
    }
    
    return s;
}


////////////////////////////////////////////////////////////
bool Joystick::HasAxis(Joy::Axis Axis) const
{
    return myAxis.find(Axis) != myAxis.end();
}


////////////////////////////////////////////////////////////
unsigned int Joystick::GetButtonsCount() const
{
    // Return number of supported buttons.
    return myButtons.size();
}


////////////////////////////////////////////////////////////
CFDictionaryRef Joystick::DevicesMaskForManager(UInt32 page, UInt32 usage)
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
bool Joystick::CreateManager()
{
    // Create HID Manager reference.
    myManager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    
    // Open a HID Manager reference.
    IOReturn openStatus = IOHIDManagerOpen(myManager, kIOHIDOptionsTypeNone);
    
    if (openStatus != kIOReturnSuccess) {
        sf::Err() << "Error when opening the joystick manager : "
                  << std::hex << openStatus << std::endl;
        
        CFRelease(myManager);
        myManager = 0;
        
        return false;
    }
    
    // Everything went fine.
    return true;
}


////////////////////////////////////////////////////////////
CFSetRef Joystick::CopyJoysticksOnly()
{
    // Create a mask to get only joystick devices.
	CFDictionaryRef joysticksMask = DevicesMaskForManager(kHIDPage_GenericDesktop, kHIDUsage_GD_Joystick);
	
	// Sets single matching criteria (dictionary) for device enumeration.
	IOHIDManagerSetDeviceMatching(myManager, joysticksMask);
    
    // No more needed -> free up.
	CFRelease(joysticksMask);
    
    // Retrieve devices.
	CFSetRef devices = IOHIDManagerCopyDevices(myManager);
    return devices; // The caller is responsible for releasing it.
}


////////////////////////////////////////////////////////////
bool Joystick::RetriveElements(IOHIDDeviceRef device)
{
    // Get a list of all elements attached to the device.
    myElements = IOHIDDeviceCopyMatchingElements(device, NULL, kIOHIDOptionsTypeNone);
	
	if (myElements == NULL) {
        // What is a joystick with no element ? Let the user know that.
		sf::Err() << "No array of element for this device" << std::endl;
        
        return false;
	}
	
    // How many elements are there ?
	CFIndex elements_count = CFArrayGetCount(myElements);
	
	if (elements_count == 0) {
        // What is a joystick with no element ? Let the user know that.
		sf::Err() << "No element attached to this device" << std::endl;
    
        CFRelease(myElements);
        myElements = 0;
        
        return false;
	}
    
    // Go through all connected elements.
    for (int i = 0; i < elements_count; ++i) {
        IOHIDElementRef element = (IOHIDElementRef) CFArrayGetValueAtIndex(myElements, i);
        
        switch (IOHIDElementGetType(element)) {

            case kIOHIDElementTypeInput_Misc:
                switch (IOHIDElementGetUsage(element)) {
                        
                    case kHIDUsage_GD_X:
                        myAxis[Joy::AxisX] = element;
                        break;
                        
                    case kHIDUsage_GD_Y:
                        myAxis[Joy::AxisY] = element;
                        break;
                        
                    case kHIDUsage_GD_Z:
                        myAxis[Joy::AxisZ] = element;
                        break;
                        
                    case kHIDUsage_GD_Rx:
                        myAxis[Joy::AxisU] = element; // use same binding as on Linux.
                        break;
                        
                    case kHIDUsage_GD_Ry:
                        myAxis[Joy::AxisV] = element; // use same binding as on Linux.
                        break;
                        
                    case kHIDUsage_GD_Rz:
                        myAxis[Joy::AxisR] = element; // use same binding as on Linux.
                        break;
                        
                    // kHIDUsage_GD_Vx, kHIDUsage_GD_Vy, kHIDUsage_GD_Vz are ignored.
                }
                break;
                
            case kIOHIDElementTypeInput_Button:
                if (myButtons.size() < Joy::ButtonCount) { // If we can managed this button through events...
                    myButtons.push_back(element); // ...we add this element to the list.
                } else {
                    // Too many buttons. We ignore this one.
                }
                break;
        }
	}
    
    // Note : Joy::AxisPOV not yet supported.
    // Maybe kIOHIDElementTypeInput_Axis is the type but I can't test.
    
    return true;
}


////////////////////////////////////////////////////////////
void Joystick::FreeUp()
{
    ReleaseElements();
    
    ReleaseManager();
}


////////////////////////////////////////////////////////////
void Joystick::ReleaseManager()
{
    if (myManager != 0) {
        // Closes the IOHIDManager
        IOReturn closeStatus = IOHIDManagerClose(myManager, kIOHIDOptionsTypeNone);
        
        if (closeStatus != kIOReturnSuccess) {
            // Closing the manager failed. We don't care that much about this.
            // It often happens when the connection with the device is closed after
            // the device is deconected from the computer.
            
            /*
            sf::Err() << "Error when closing the manager : "
                      << std::hex << closeStatus << std::endl;
            //*/
        }
        
        // Release the manager.
        CFRelease(myManager);
        myManager = 0;
    }
}


////////////////////////////////////////////////////////////
void Joystick::ReleaseElements()
{
    if (myElements != 0) {
        // Release all elements.
        CFRelease(myElements);
        myElements = 0;
        
        // Both myAxis and myButton contains only reference from myElements.
        // Thus no special cleanup is required on these two.
        myButtons.clear();
        myAxis.clear();
    }
}


} // namespace priv

} // namespace sf
