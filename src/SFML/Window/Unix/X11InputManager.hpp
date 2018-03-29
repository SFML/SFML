////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2018 Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef SFML_X11INPUTMANAGER_HPP
#define SFML_X11INPUTMANAGER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/String.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <X11/X.h> // Keycode
#include <X11/XKBlib.h>

namespace sf {
namespace priv {

////////////////////////////////////////////////////////////
/// \brief sf::priv::InputImpl helper
///
/// This class manage as a singleton instance the keyboard state.
/// Its purpose is to help sf::priv::InputImpl class.
///
////////////////////////////////////////////////////////////
class X11InputManager
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Get the unique instance of the class
    ///
    /// \note Private use only
    ///
    /// \return Reference to the X11InputManager instance
    ///
    ////////////////////////////////////////////////////////////
    static X11InputManager& getInstance();

private:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    X11InputManager();
public:

    ////////////////////////////////////////////////////////////
    /// \brief Initialize the keyboard
    ///
    /// Builds a mapping between sf::Keyboard::Scancode and
    /// X11 keycodes
    ///
    ////////////////////////////////////////////////////////////
    void initialize(Display* display);

    ////////////////////////////////////////////////////////////
    /// \copydoc sf::Keyboard::isKeyPressed(Key)
    ///
    ////////////////////////////////////////////////////////////
    bool isKeyPressed(sf::Keyboard::Key key) const;

    ////////////////////////////////////////////////////////////
    /// \copydoc sf::Keyboard::isKeyPressed(Scancode)
    ///
    ////////////////////////////////////////////////////////////
    bool isKeyPressed(sf::Keyboard::Scancode code) const;

    ////////////////////////////////////////////////////////////
    /// \copydoc sf::Keyboard::localize
    ///
    ////////////////////////////////////////////////////////////
    sf::Keyboard::Scancode unlocalize(sf::Keyboard::Key key) const;

    ////////////////////////////////////////////////////////////
    /// \copydoc sf::Keyboard::unlocalize
    ///
    ////////////////////////////////////////////////////////////
    sf::Keyboard::Key localize(sf::Keyboard::Scancode code) const;

    ////////////////////////////////////////////////////////////
    /// \copydoc sf::Keyboard::getDescription
    ///
    ////////////////////////////////////////////////////////////
    sf::String getDescription(Keyboard::Scancode code) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the sf::Keyboard::Key from XKeyEvent
    ///
    /// \param event Event from which key is gotten
    ///
    /// \return A key being pressed or released
    ///
    ////////////////////////////////////////////////////////////
    sf::Keyboard::Key getKeyFromEvent(XKeyEvent& event) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the sf::Keyboard::Scancode from XKeyEvent
    ///
    /// \param event Event from which scancode is gotten
    ///
    /// \return A scancode of a key being pressed or released
    ///
    ////////////////////////////////////////////////////////////
    sf::Keyboard::Scancode getScancodeFromEvent(XKeyEvent& event) const;

private:

    ////////////////////////////////////////////////////////////
    /// \brief Converts sf::Keyboard::Key to X11 keycode
    ///
    /// \param key A key to be converted
    ///
    /// \return A corresponding X11 Keycode
    ///
    ////////////////////////////////////////////////////////////
    KeyCode SFtoKeyCode(sf::Keyboard::Key key) const;

    ////////////////////////////////////////////////////////////
    /// \brief Converts sf::Keyboard::Scancode to X11 keycode
    ///
    /// \param code A scancode to be converted
    ///
    /// \return A corresponding X11 Keycode
    ///
    ////////////////////////////////////////////////////////////
    KeyCode SFtoKeyCode(sf::Keyboard::Scancode code) const;

    ////////////////////////////////////////////////////////////
    /// \brief Convert X11 Keycode to sf::Keyboard::Scancode
    ///
    /// \param code X11 keycode
    ///
    /// \return The corresponding sf::Keyboard::Scancode
    ///
    ////////////////////////////////////////////////////////////
    sf::Keyboard::Scancode keyCodeToSF(KeyCode code) const;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    KeyCode                m_scancodeToKeycode[sf::Keyboard::ScanCodeCount]; ///< Mapping of SFML scancode to X11 KeyCode
    sf::Keyboard::Scancode m_keycodeToScancode[256];                         ///< Mapping of X11 KeyCode to SFML scancode
    Display*               m_display;                                        ///< Pointer to the display
};

} // namespace priv
} // namespace sf

#endif // SFML_X11INPUTMANAGER_HPP
