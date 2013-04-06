////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Window/Context.hpp>
#include <SFML/Window/GlContext.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
Context::Context()
{
    m_context = priv::GlContext::create();
    setActive(true);
}


////////////////////////////////////////////////////////////
Context::~Context()
{
    delete m_context;
}


////////////////////////////////////////////////////////////
bool Context::setActive(bool active)
{
    return m_context->setActive(active);
}


////////////////////////////////////////////////////////////
Context::Context(const ContextSettings& settings, unsigned int width, unsigned int height)
{
    m_context = priv::GlContext::create(settings, width, height);
    setActive(true);
}

} // namespace sf
