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
    /// as a string. If the clipboard does not contain string
    /// it returns an empty sf::String object.
    ///
    /// \return Clipboard contents as sf::String object
    ///
    ////////////////////////////////////////////////////////////
    static String getString();

    ////////////////////////////////////////////////////////////
    /// \brief Set the content of the clipboard as string data
    ///
    /// This function sets the content of the clipboard as a
    /// string.
    ///
    /// \param text sf::String containing the data to be sent 
    /// to the clipboard
    ///
    ////////////////////////////////////////////////////////////
    static void setString(const String& text);
};

} // namespace sf


#endif // SFML_CLIPBOARD_HPP


////////////////////////////////////////////////////////////
/// \class sf::Clipboard
/// \ingroup window
///
/// sf::Clipboard provides an interface for getting and
/// setting the contents of the system clipboard.
///
/// Usage example:
/// \code
/// // get the clipboard content as a string
/// sf::String string = sf::Clipboard::getString();
///
/// // or use it in the event loop
/// sf::Event event;
/// while(window.pollEvent(event))
/// {
///     if(event.type == sf::Event::Closed)
///         window.close();
///     if(event.type == sf::Event::KeyPressed)
///     {
///         // Using Ctrl + V to paste a string into SFML
///         if(event.key.control && event.key.code == sf::Keyboard::V)
///             string = sf::Clipboard::getString();
///     }
/// }
///
/// // set the clipboard to a string
/// sf::Clipboard::setString("Hello World!");
/// \endcode
///
/// \see sf::String, sf::Event
///
////////////////////////////////////////////////////////////
