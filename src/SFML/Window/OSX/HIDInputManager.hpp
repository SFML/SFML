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

#ifndef SFML_HIDINPUTMANAGER_HPP
#define SFML_HIDINPUTMANAGER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/JoystickImpl.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <Carbon/Carbon.h>
#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/hid/IOHIDDevice.h>
#include <vector>

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief This class manage as a singleton instance the
/// keyboard and mouse states. It's only purpose is
/// to help sf::priv::InputImpl class.
///
////////////////////////////////////////////////////////////
class HIDInputManager : NonCopyable
{
public :
    
    ////////////////////////////////////////////////////////////
    /// \brief Get the unique instance of the class
    ///
    /// \note Private use only
    ///
    /// \return Reference to the HIDInputManager instance
    ///
    ////////////////////////////////////////////////////////////
    static HIDInputManager& GetInstance();
    
    ////////////////////////////////////////////////////////////
    /// \brief Check if a key is pressed
    ///
    /// \param key Key to check
    ///
    /// \return True if the key is pressed, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    bool IsKeyPressed(Keyboard::Key key);
    
    ////////////////////////////////////////////////////////////
    /// \brief Check if a mouse button is pressed
    ///
    /// \param button Button to check
    ///
    /// \return True if the button is pressed, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    bool IsMouseButtonPressed(Mouse::Button button);

public :
    
    ////////////////////////////////////////////////////////////
    /// \brief Get the usb location ID of a given device
    ///
    /// This location ID is unique and can be used a usb port identifier
    ///
    /// \param device HID device to query
    /// \return the device's location ID or 0 if something went wrong
    ///
    ////////////////////////////////////////////////////////////
    static long GetLocationID(IOHIDDeviceRef device);
    
    ////////////////////////////////////////////////////////////
    /// \brief Create a mask (dictionary) for an IOHIDManager
    ///
    /// \param page  HID page
    /// \param usage HID usage page
    /// \return a retained CFDictionaryRef
    ///
    ////////////////////////////////////////////////////////////
    static CFDictionaryRef CopyDevicesMask(UInt32 page, UInt32 usage);
    
    ////////////////////////////////////////////////////////////
    /// Try to convert a character into a SFML key code.
    ///
    /// Return sf::Keyboard::KeyCount if it doesn't match any 'localized' keys.
    ///
    /// By 'localized' I mean keys that depend on the keyboard layout
    /// and might not be the same as the US keycode in some country
    /// (e.g. the keys 'Y' and 'Z' are switched on QWERTZ keyboard and
    /// US keyboard layouts.)
    ///
    ////////////////////////////////////////////////////////////
    static Keyboard::Key LocalizedKeys(UniChar ch);
    
    ////////////////////////////////////////////////////////////
    /// Try to convert a virtual keycode into a SFML key code.
    ///
    /// Return sf::Keyboard::KeyCount if the keycode is unknown.
    ///
    ////////////////////////////////////////////////////////////
    static Keyboard::Key NonLocalizedKeys(UniChar virtualKeycode);

private :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    HIDInputManager();
    
    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~HIDInputManager();
    
    ////////////////////////////////////////////////////////////
    /// \brief Initialize the keyboard part of this class
    ///
    /// If something went wrong FreeUp is called
    ///
    ////////////////////////////////////////////////////////////
    void InitializeKeyboard();
    
    ////////////////////////////////////////////////////////////
    /// \brief Initialize the mouse part of this class
    ///
    /// If something went wrong FreeUp is called
    ///
    ////////////////////////////////////////////////////////////
    void InitializeMouse();
    
    ////////////////////////////////////////////////////////////
    /// \brief Load the given keyboard into myKeys
    ///
    /// If the given keyboard has no key this function simply
    /// returns. FreeUp is _not_ called because this is not fatal.
    ///
    /// \param keyboard Keyboard to load
    ///
    ////////////////////////////////////////////////////////////
    void LoadKeyboard(IOHIDDeviceRef keyboard);
    
    ////////////////////////////////////////////////////////////
    /// \brief Load the given mouse into myButtons
    ///
    /// If the given mouse has no button this function simply
    /// returns. FreeUp is _not_ called because this is not fatal.
    ///
    /// \param mouse Mouse to load
    ///
    ////////////////////////////////////////////////////////////
    void LoadMouse(IOHIDDeviceRef mouse);
    
    ////////////////////////////////////////////////////////////
    /// \brief Load the given key into myKeys
    ///
    /// FreeUp is _not_ called by this function.
    ///
    /// \param key Key to load
    ///
    ////////////////////////////////////////////////////////////
    void LoadKey(IOHIDElementRef key);
    
    ////////////////////////////////////////////////////////////
    /// \brief Load the given button into myButtons
    ///
    /// FreeUp is _not_ called by this function.
    ///
    /// \param button Button to load
    ///
    ////////////////////////////////////////////////////////////
    void LoadButton(IOHIDElementRef button);
    
    ////////////////////////////////////////////////////////////
    /// \brief Release all resources
    ///
    /// Close all connections to any devices, if required
    /// Set amIValid to false
    ///
    ////////////////////////////////////////////////////////////
    void FreeUp();
    
    ////////////////////////////////////////////////////////////
    /// \brief Filter the devices and return them.
    ///
    /// FreeUp is _not_ called by this function.
    ///
    /// \param page  HID page like kHIDPage_GenericDesktop
    /// \param usage HID usage page like kHIDUsage_GD_Keyboard or kHIDUsage_GD_Mouse
    /// \return a retained CFSetRef of IOHIDDeviceRef or NULL
    ///
    ////////////////////////////////////////////////////////////
    CFSetRef CopyDevices(UInt32 page, UInt32 usage);
    
    ////////////////////////////////////////////////////////////
    /// \brief Converte a HID key usage to its corresponding virtual code
    ///
    /// See IOHIDUsageTables.h
    ///
    /// \param usage Any kHIDUsage_Keyboard* usage
    /// \return the virtual code associate with the given HID key usage
    ///         or 0xff if it is associate with no virtual code
    ///
    ////////////////////////////////////////////////////////////
    static UInt8 UsageToVirtualCode(UInt32 usage);

private :
    
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    bool              amIValid;                     ///< If any error occurs this variable is false
    CFDataRef         myLayoutData;                 ///< CFData containing the layout
    UCKeyboardLayout* myLayout;                     ///< Current Keyboard Layout
    IOHIDManagerRef   myManager;                    ///< HID Manager
    
    typedef std::vector<IOHIDElementRef> IOHIDElements;
    IOHIDElements     myKeys[Keyboard::KeyCount];   ///< All the keys on any connected keyboard
    IOHIDElements     myButtons[Mouse::ButtonCount];///< All the buttons on any connected mouse
    
    ////////////////////////////////////////////////////////////
    /// myKeys' index corresponds to sf::Keyboard::Key enum.
    /// if no key is assigned with key XYZ then myKeys[XYZ].size() == 0.
    /// if there are several keyboards connected and several HID keys associate
    /// with the same sf::Keyboard::Key then myKeys[XYZ] contains all these
    /// HID keys.
    ///
    /// myButtons works the same way.
    ///
    ////////////////////////////////////////////////////////////
};

} // namespace priv
    
} // namespace sf

#endif