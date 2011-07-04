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

#ifndef SFML_INPUTIMPLWIN32_HPP
#define SFML_INPUTIMPLWIN32_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Windows implementation of inputs (keyboard + mouse)
///
////////////////////////////////////////////////////////////
class InputImpl
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Check if a key is pressed
    ///
    /// \param key Key to check
    ///
    /// \return True if the key is pressed, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool IsKeyPressed(Keyboard::Key key);

    ////////////////////////////////////////////////////////////
    /// \brief Check if a mouse button is pressed
    ///
    /// \param button Button to check
    ///
    /// \return True if the button is pressed, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool IsMouseButtonPressed(Mouse::Button button);

    ////////////////////////////////////////////////////////////
    /// \brief Get the current position of the mouse
    ///
    /// This function returns the mouse position in desktop coordinates.
    ///
    /// \return Current position of the mouse
    ///
    ////////////////////////////////////////////////////////////
    static Vector2i GetMousePosition();
};

} // namespace priv

} // namespace sf


#endif // SFML_INPUTIMPLWIN32_HPP
