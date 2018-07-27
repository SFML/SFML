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

#ifndef SFML_CURSORIMPLANDROID_HPP
#define SFML_CURSORIMPLANDROID_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Cursor.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector2.hpp>


namespace sf
{

namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Android implementation of Cursor
///
/// This is a typical "not supported" implementation.
///
////////////////////////////////////////////////////////////
class CursorImpl : NonCopyable
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Refer to sf::Cursor::Cursor().
    ///
    ////////////////////////////////////////////////////////////
    CursorImpl();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// Refer to sf::Cursor::~Cursor().
    ///
    ////////////////////////////////////////////////////////////
    ~CursorImpl();

    ////////////////////////////////////////////////////////////
    /// \brief Create a cursor with the provided image
    ///
    /// Returns false.
    ///
    ////////////////////////////////////////////////////////////
    bool loadFromPixels(const Uint8* pixels, Vector2u size, Vector2u hotspot);

    ////////////////////////////////////////////////////////////
    /// \brief Create a native system cursor
    ///
    /// Returns false.
    ///
    ////////////////////////////////////////////////////////////
    bool loadFromSystem(Cursor::Type type);
};

} // namespace priv

} // namespace sf

#endif // SFML_CURSORIMPLANDROID_HPP
