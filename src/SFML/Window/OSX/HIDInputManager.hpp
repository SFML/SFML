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

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief This class manage as a singleton instance the
/// keyboard and mouse states. It's only purpose is
/// to help sf::priv::InputImpl class.
///
/// sf::priv::JoystickImpl is not concerned by this class.
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
    /// \brief Release all resources
    ///
    /// Close all connections to any devices, if required
    /// Set amIValid to false
    ///
    ////////////////////////////////////////////////////////////
    void FreeUp();
    
    ////////////////////////////////////////////////////////////
    /// \brief Create a mask (dictionary) for an IOHIDManager
    ///
    /// \param page  HID page
    /// \param usage HID usage page
    /// \return a retained CFDictionaryRef
    ///
    ////////////////////////////////////////////////////////////
    static CFDictionaryRef CopyDevicesMaskForManager(UInt32 page, UInt32 usage);
    
    ////////////////////////////////////////////////////////////
    /// \brief Converte a HID key usage to it's conrresponding virtual code
    ///
    /// See IOHIDUsageTables.h
    ///
    /// \param usage Any kHIDUsage_Keyboard* usage
    /// \return the virtual code associate to the given HID key usage
    ///         or 0xff if it is associate to no virtual code
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
    IOHIDElementRef   myKeys[Keyboard::KeyCount];   ///< All the keys on the current keyboard
    /* myKeys index correspond to sf::Keyboard::Key enum       */
    /* if no key is assigned to a key XYZ then myKeys[XYZ] = 0 */
};

} // namespace priv
    
} // namespace sf

#endif