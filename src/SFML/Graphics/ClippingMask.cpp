////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/ClippingMask.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
ClippingMask::ClippingMask() :
m_drawables(),
m_mode     (Mode::Inclusive)
{
}


////////////////////////////////////////////////////////////
ClippingMask::ClippingMask(Mode theMode) :
m_drawables(),
m_mode     (theMode)
{
}


////////////////////////////////////////////////////////////
std::size_t ClippingMask::getDrawableCount() const
{
    return m_drawables.size();
}


////////////////////////////////////////////////////////////
const Drawable*& ClippingMask::operator [](std::size_t index)
{
    return m_drawables[index];
}


////////////////////////////////////////////////////////////
const Drawable* const& ClippingMask::operator [](std::size_t index) const
{
    return m_drawables[index];
}


////////////////////////////////////////////////////////////
void ClippingMask::clear()
{
    m_drawables.clear();
}


////////////////////////////////////////////////////////////
void ClippingMask::append(const Drawable& drawable)
{
    m_drawables.push_back(&drawable);
}


////////////////////////////////////////////////////////////
void ClippingMask::remove(const Drawable& drawable)
{
    m_drawables.erase(std::remove(m_drawables.begin(), m_drawables.end(), &drawable), m_drawables.end());
}


////////////////////////////////////////////////////////////
ClippingMask::Mode ClippingMask::getMode() const
{
    return m_mode;
}


////////////////////////////////////////////////////////////
void ClippingMask::setMode(Mode theMode)
{
    m_mode = theMode;
}


} // namespace sf
