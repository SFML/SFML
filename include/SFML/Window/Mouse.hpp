////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_MOUSE_HPP
#define SFML_MOUSE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <SFML/System/Vector2.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Give access to the real-time state of the mouse
///
////////////////////////////////////////////////////////////
class SFML_API Mouse
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Mouse buttons
    ///
    ////////////////////////////////////////////////////////////
    enum Button
    {
        Left,       ///< The left mouse button
        Right,      ///< The right mouse button
        Middle,     ///< The middle (wheel) mouse button
        XButton1,   ///< The first extra mouse button
        XButton2,   ///< The second extra mouse button

        ButtonCount ///< Keep last -- the total number of mouse buttons
    };

    ////////////////////////////////////////////////////////////
    /// \brief Check if a mouse button is pressed
    ///
    /// \param button Button to check
    ///
    /// \return True if the button is pressed, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool IsButtonPressed(Button button);

    ////////////////////////////////////////////////////////////
    /// \brief Get the current position of the mouse
    ///
    /// This function returns the current position of the mouse
    /// cursor.
    /// If the cursor is over a SFML window, the returned position
    /// is relative to this window. Otherwise, the returned position
    /// is in desktop coordinates.
    ///
    /// \return Current position of the mouse
    ///
    ////////////////////////////////////////////////////////////
    static Vector2i GetPosition();
};

} // namespace sf


#endif // SFML_MOUSE_HPP


////////////////////////////////////////////////////////////
/// \class sf::Mouse
/// \ingroup window
///
/// sf::Mouse provides an interface to the state of the
/// mouse. It only contains static functions (a single
/// mouse is assumed), so it's not meant to be instanciated.
///
/// This class allows users to query the mouse state at any
/// time and directly, without having to deal with a window and
/// its events. Compared to the MouseMoved, MouseButtonPressed
/// and MouseButtonReleased events, sf::Mouse can retrieve the
/// state of the cursor and the buttons at any time
/// (you don't need to store and update a boolean on your side
/// in order to know if a button is pressed or released), and you
/// always get the real state of the mouse, even if it is
/// moved, pressed or released when your window is out of focus
/// and no event is triggered.
///
/// Note that the sf::Mouse::GetPosition function has a special
/// behaviour: it returns the cursor position relative to the
/// window which has the mouse focus (ie. the window on which
/// the cursor is).
///
/// Usage example:
/// \code
/// if (sf::Mouse::IsButtonPressed(sf::Mouse::Left))
/// {
///     // left click...
/// }
/// else if (sf::Mouse::IsButtonPressed(sf::Mouse::Right))
/// {
///     // right click...
/// }
///
/// sf::Vector2i position = sf::Mouse::GetPosition();
/// \endcode
///
/// \see sf::Joystick, sf::Keyboard
///
////////////////////////////////////////////////////////////
