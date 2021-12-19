////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2021 Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef SFML_CLIPBOARDIMPLCOCOA_HPP
#define SFML_CLIPBOARDIMPLCOCOA_HPP


namespace sf
{
class String;

namespace priv
{

////////////////////////////////////////////////////////////
/// \brief Give access to the system clipboard
///
////////////////////////////////////////////////////////////
class ClipboardImpl
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Get the content of the clipboard as string data
    ///
    /// This function returns the content of the clipboard
    /// as a string. If the clipboard does not contain string
    /// it returns an empty sf::String object.
    ///
    /// \return Current content of the clipboard
    ///
    ////////////////////////////////////////////////////////////
    static String getString();

    ////////////////////////////////////////////////////////////
    /// \brief Set the content of the clipboard as string data
    ///
    /// This function sets the content of the clipboard as a
    /// string.
    ///
    /// \param text sf::String object containing the data to be sent
    /// to the clipboard
    ///
    ////////////////////////////////////////////////////////////
    static void setString(const String& text);
};

} // namespace priv

} // namespace sf


#endif // SFML_CLIPBOARDIMPLCOCOA_HPP

