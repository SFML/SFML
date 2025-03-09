////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Export.hpp>

#include <SFML/System/Vector2.hpp>


namespace sf
{
class WindowBase;

////////////////////////////////////////////////////////////
/// \brief Give access to the real-time state of the mouse
///
////////////////////////////////////////////////////////////
namespace Mouse
{
////////////////////////////////////////////////////////////
/// \brief Mouse buttons
///
////////////////////////////////////////////////////////////
enum class Button
{
    Left,   //!< The left mouse button
    Right,  //!< The right mouse button
    Middle, //!< The middle (wheel) mouse button
    Extra1, //!< The first extra mouse button
    Extra2  //!< The second extra mouse button
};

// NOLINTNEXTLINE(readability-identifier-naming)
static constexpr unsigned int ButtonCount{5}; //!< The total number of mouse buttons

////////////////////////////////////////////////////////////
/// \brief Mouse wheels
///
////////////////////////////////////////////////////////////
enum class Wheel
{
    Vertical,  //!< The vertical mouse wheel
    Horizontal //!< The horizontal mouse wheel
};

////////////////////////////////////////////////////////////
/// \brief Check if a mouse button is pressed
///
/// \warning Checking the state of buttons `Mouse::Button::Extra1` and
/// `Mouse::Button::Extra2` is not supported on Linux with X11.
///
/// \param button Button to check
///
/// \return `true` if the button is pressed, `false` otherwise
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_WINDOW_API bool isButtonPressed(Button button);

////////////////////////////////////////////////////////////
/// \brief Get the current position of the mouse in desktop coordinates
///
/// This function returns the global position of the mouse
/// cursor on the desktop.
///
/// \return Current position of the mouse
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_WINDOW_API Vector2i getPosition();

////////////////////////////////////////////////////////////
/// \brief Get the current position of the mouse in window coordinates
///
/// This function returns the current position of the mouse
/// cursor, relative to the given window.
///
/// \param relativeTo Reference window
///
/// \return Current position of the mouse
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_WINDOW_API Vector2i getPosition(const WindowBase& relativeTo);

////////////////////////////////////////////////////////////
/// \brief Set the current position of the mouse in desktop coordinates
///
/// This function sets the global position of the mouse
/// cursor on the desktop.
///
/// \param position New position of the mouse
///
////////////////////////////////////////////////////////////
SFML_WINDOW_API void setPosition(Vector2i position);

////////////////////////////////////////////////////////////
/// \brief Set the current position of the mouse in window coordinates
///
/// This function sets the current position of the mouse
/// cursor, relative to the given window.
///
/// \param position New position of the mouse
/// \param relativeTo Reference window
///
////////////////////////////////////////////////////////////
SFML_WINDOW_API void setPosition(Vector2i position, const WindowBase& relativeTo);
} // namespace Mouse

} // namespace sf


////////////////////////////////////////////////////////////
/// \namespace sf::Mouse
/// \ingroup window
///
/// `sf::Mouse` provides an interface to the state of the
/// mouse. A single mouse is assumed.
///
/// This namespace allows users to query the mouse state at any
/// time and directly, without having to deal with a window and
/// its events. Compared to the `MouseMoved`, `MouseButtonPressed`
/// and `MouseButtonReleased` events, `sf::Mouse` can retrieve the
/// state of the cursor and the buttons at any time
/// (you don't need to store and update a boolean on your side
/// in order to know if a button is pressed or released), and you
/// always get the real state of the mouse, even if it is
/// moved, pressed or released when your window is out of focus
/// and no event is triggered.
///
/// The `setPosition` and `getPosition` functions can be used to change
/// or retrieve the current position of the mouse pointer. There are
/// two versions: one that operates in global coordinates (relative
/// to the desktop) and one that operates in window coordinates
/// (relative to a specific window).
///
/// Usage example:
/// \code
/// if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
/// {
///     // left click...
/// }
///
/// // get global mouse position
/// sf::Vector2i position = sf::Mouse::getPosition();
///
/// // set mouse position relative to a window
/// sf::Mouse::setPosition(sf::Vector2i(100, 200), window);
/// \endcode
///
/// \see `sf::Joystick`, `sf::Keyboard`, `sf::Touch`
///
////////////////////////////////////////////////////////////
