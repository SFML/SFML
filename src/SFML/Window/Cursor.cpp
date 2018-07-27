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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Cursor.hpp>
#include <SFML/Window/CursorImpl.hpp>

namespace sf
{

////////////////////////////////////////////////////////////
Cursor::Cursor() :
m_impl(new priv::CursorImpl())
{
    // That's it
}


////////////////////////////////////////////////////////////
Cursor::~Cursor()
{
    delete m_impl;
}


////////////////////////////////////////////////////////////
bool Cursor::loadFromPixels(const Uint8* pixels, Vector2u size, Vector2u hotspot)
{
    if ((pixels == 0) || (size.x == 0) || (size.y == 0))
        return false;
    else
        return m_impl->loadFromPixels(pixels, size, hotspot);
}


////////////////////////////////////////////////////////////
bool Cursor::loadFromSystem(Type type)
{
    return m_impl->loadFromSystem(type);
}


////////////////////////////////////////////////////////////
const priv::CursorImpl& Cursor::getImpl() const
{
    return *m_impl;
}

} // namespace sf

