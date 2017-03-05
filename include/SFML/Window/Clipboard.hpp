////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2017 Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef SFML_CLIPBOARD_HPP
#define SFML_CLIPBOARD_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Export.hpp>
#include <SFML/System/String.hpp>


namespace sf
{

////////////////////////////////////////////////////////////
/// \brief Give access to the system clipboard
///
////////////////////////////////////////////////////////////
class SFML_WINDOW_API Clipboard
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Get the content of the clipboard as string data
    ///
    /// This function returns the content of the clipboard
    /// as a string.
    ///
    /// \return Current content of the clipboard
    ///
    ////////////////////////////////////////////////////////////
    static String getText();

    ////////////////////////////////////////////////////////////
    /// \brief Set the content of the clipboard as string data
    ///
    /// This function sets the content of the clipboard to a
    /// string
    ///
    /// \return Current position of the mouse
    ///
    ////////////////////////////////////////////////////////////
    static void setText(const String& text);
};

} // namespace sf


#endif // SFML_CLIPBOARD_HPP


////////////////////////////////////////////////////////////
/// \class sf::Clipboard
/// \ingroup window
///
/// sf::Clipboard provides an interface to access the clipboard
/// on the host system. It contains functions for getting data
/// from and setting data to the clipboard.
///
/// Usage example:
/// \code
/// sf::String str = sf::Clipboard::getText();
///
/// // set clipboard text
/// sf::Clipboard::setText("SFML is awesome!");
/// \endcode
///
/// \see sf::Mouse, sf::Window
///
////////////////////////////////////////////////////////////
