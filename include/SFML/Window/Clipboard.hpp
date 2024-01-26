////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
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


namespace sf
{
class String;

////////////////////////////////////////////////////////////
/// \brief Give access to the system clipboard
///
////////////////////////////////////////////////////////////
namespace Clipboard
{
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
SFML_WINDOW_API String getString();

////////////////////////////////////////////////////////////
/// \brief Set the content of the clipboard as string data
///
/// This function sets the content of the clipboard as a
/// string.
///
/// \warning Due to limitations on some operating systems,
///          setting the clipboard contents is only
///          guaranteed to work if there is currently an
///          open window for which events are being handled.
///
/// \param text sf::String containing the data to be sent
/// to the clipboard
///
////////////////////////////////////////////////////////////
SFML_WINDOW_API void setString(const String& text);
} // namespace Clipboard

} // namespace sf


////////////////////////////////////////////////////////////
/// \namespace sf::Clipboard
/// \ingroup window
///
/// sf::Clipboard provides an interface for getting and
/// setting the contents of the system clipboard.
///
/// It is important to note that due to limitations on some
/// operating systems, setting the clipboard contents is
/// only guaranteed to work if there is currently an open
/// window for which events are being handled.
///
/// Usage example:
/// \code
/// // get the clipboard content as a string
/// sf::String string = sf::Clipboard::getString();
///
/// // or use it in the event loop
/// for (sf::Event event; window.pollEvent(event);)
/// {
///     if(event.type == sf::Event::Closed)
///         window.close();
///     if(event.type == sf::Event::KeyPressed)
///     {
///         // Using Ctrl + V to paste a string into SFML
///         if(event.key.control && event.key.code == sf::Keyboard::Key::V)
///             string = sf::Clipboard::getString();
///
///         // Using Ctrl + C to copy a string out of SFML
///         if(event.key.control && event.key.code == sf::Keyboard::Key::C)
///             sf::Clipboard::setString("Hello World!");
///     }
/// }
/// \endcode
///
/// \see sf::String, sf::Event
///
////////////////////////////////////////////////////////////
