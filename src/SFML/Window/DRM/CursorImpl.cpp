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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/DRM/CursorImpl.hpp>

#include <vector>


namespace sf::priv
{
////////////////////////////////////////////////////////////
bool CursorImpl::loadFromPixels(const std::uint8_t* /*pixels*/, Vector2u /*size*/, Vector2u /*hotspot*/)
{
    return false;
}


////////////////////////////////////////////////////////////
bool CursorImpl::loadFromPixelsARGB(const std::uint8_t* /*pixels*/, Vector2u /*size*/, Vector2u /*hotspot*/)
{
    return false;
}


////////////////////////////////////////////////////////////
bool CursorImpl::loadFromPixelsMonochrome(const std::uint8_t* /*pixels*/, Vector2u /*size*/, Vector2u /*hotspot*/)
{
    return false;
}


////////////////////////////////////////////////////////////
bool CursorImpl::loadFromSystem(Cursor::Type /*type*/)
{
    return false;
}


////////////////////////////////////////////////////////////
bool CursorImpl::isColorCursorSupported()
{
    return false;
}


////////////////////////////////////////////////////////////
void CursorImpl::release()
{
}

} // namespace sf::priv
