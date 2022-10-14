////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef SFML_CURSORIMPLWIN32_HPP
#define SFML_CURSORIMPLWIN32_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Cursor.hpp>

namespace sf
{

namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Win32 implementation of Cursor
///
////////////////////////////////////////////////////////////
class CursorImpl
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
    /// \brief Deleted copy constructor
    ///
    ////////////////////////////////////////////////////////////
    CursorImpl(const CursorImpl&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy assignment
    ///
    ////////////////////////////////////////////////////////////
    CursorImpl& operator=(const CursorImpl&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Create a cursor with the provided image
    ///
    /// Refer to sf::Cursor::loadFromPixels().
    ///
    ////////////////////////////////////////////////////////////
    bool loadFromPixels(const std::uint8_t* pixels, Vector2u size, Vector2u hotspot);

    ////////////////////////////////////////////////////////////
    /// \brief Create a native system cursor
    ///
    /// Refer to sf::Cursor::loadFromSystem().
    ///
    ////////////////////////////////////////////////////////////
    bool loadFromSystem(Cursor::Type type);

private:
    friend class WindowImplWin32;

    ////////////////////////////////////////////////////////////
    /// \brief Release the cursor, if we have loaded one.
    ///
    ////////////////////////////////////////////////////////////
    void release();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    void* m_cursor; // Type erasure via `void*` is used here to avoid depending on `windows.h`
    bool  m_systemCursor;
};

} // namespace priv

} // namespace sf

#endif // SFML_CUSROSIMPLWIN32_HPP
