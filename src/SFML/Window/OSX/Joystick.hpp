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

#ifndef SFML_JOYSTICKOSX_HPP
#define SFML_JOYSTICKOSX_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/hid/IOHIDDevice.h>
#include <map>
#include <vector>

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief OSX implementation of Joystick
///
////////////////////////////////////////////////////////////
class Joystick
{
public :
    
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructors initializes all members to 0.
    /// That is, it does nothing.
    ///
    ////////////////////////////////////////////////////////////
    Joystick();
    
    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// Close all connections to any devices if required.
    ///
    ////////////////////////////////////////////////////////////
    ~Joystick();
    
    ////////////////////////////////////////////////////////////
    /// \brief Initialize the instance and bind it to a physical joystick
    ///
    /// \param index Index of the physical joystick to bind to
    ///
    ////////////////////////////////////////////////////////////
    void Initialize(unsigned int index);
    
    ////////////////////////////////////////////////////////////
    /// \brief Update the current joystick and return its new state
    ///
    /// \return Current state of the joystick
    ///
    ////////////////////////////////////////////////////////////
    JoystickState UpdateState();
    
    ////////////////////////////////////////////////////////////
    /// \brief Check if the joystick supports the given axis
    ///
    /// \param axis Axis to check
    ///
    /// \return True of the axis is supported, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    bool HasAxis(Joy::Axis Axis) const;
    
    ////////////////////////////////////////////////////////////
    /// \brief Get the number of buttons supported by the joystick
    ///
    /// \return Number of buttons
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetButtonsCount() const;
    
private :
    
    ////////////////////////////////////////////////////////////
    /// \brief Create a mask (dictionary) for an IOHIDManager
    ///
    /// \param page  
    /// \param usage 
    ///
    ////////////////////////////////////////////////////////////
    static CFDictionaryRef DevicesMaskForManager(UInt32 page, UInt32 usage);
    
    ////////////////////////////////////////////////////////////
    /// \brief Create and open the manager
    ///
    /// \return Return false if someting went wrong
    ///
    ////////////////////////////////////////////////////////////
    bool CreateManager();
    
    ////////////////////////////////////////////////////////////
    /// \brief Copy all connected joysticks to the manager
    ///
    /// \return NULL or a valid (possibly empty) set of devices
    ///
    ////////////////////////////////////////////////////////////
    CFSetRef CopyJoysticksOnly();
    
    ////////////////////////////////////////////////////////////
    /// \brief Load all connected elements to the given device
    ///
    /// \param device The desired joystick
    /// \return False if something went wrong
    ///
    ////////////////////////////////////////////////////////////
    bool RetriveElements(IOHIDDeviceRef device);
    
    ////////////////////////////////////////////////////////////
    /// \brief Release all resources
    ///
    /// Close all connections to any devices, if required
    ///
    ////////////////////////////////////////////////////////////
    void FreeUp();
    
    ////////////////////////////////////////////////////////////
    /// \brief Close and release the manager
    ///
    ////////////////////////////////////////////////////////////
    void ReleaseManager();
    
    ////////////////////////////////////////////////////////////
    /// \brief Release all elements
    ///
    ////////////////////////////////////////////////////////////
    void ReleaseElements();
    
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    typedef std::map<sf::Joy::Axis, IOHIDElementRef> AxisMap;
    typedef std::vector<IOHIDElementRef> ButtonsVector;
    
    AxisMap myAxis;             ///< Axis (IOHIDElementRef) connected to the joystick.
    ButtonsVector myButtons;    ///< Buttons (IOHIDElementRef) connected to the joystick.
    // Note : Both myAxis and myButton contains only reference from myElements.
    //        Thus no special cleanup is required on these two.
    
    IOHIDManagerRef myManager;  ///< HID Manager.
    CFArrayRef myElements;      ///< IOHIDElementRef connected to the joytick.
};
    
} // namespace priv
    
} // namespace sf


#endif // SFML_JOYSTICKOSX_HPP
