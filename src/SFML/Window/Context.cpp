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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Context.hpp>
#include <SFML/Window/GlContext.hpp>
#include <SFML/System/ThreadLocalPtr.hpp>


namespace
{
    // A nested named namespace is used here to allow unity builds of SFML.
    namespace ContextImpl
    {
        // This per-thread variable holds the current context for each thread
        sf::ThreadLocalPtr<sf::Context> currentContext(NULL);
    }
}

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
    setActive(false);
    delete m_context;
}


////////////////////////////////////////////////////////////
bool Context::setActive(bool active)
{
    bool result = m_context->setActive(active);

    if (result)
        ContextImpl::currentContext = (active ? this : NULL);

    return result;
}


////////////////////////////////////////////////////////////
const ContextSettings& Context::getSettings() const
{
    return m_context->getSettings();
}


////////////////////////////////////////////////////////////
const Context* Context::getActiveContext()
{
    using ContextImpl::currentContext;

    // We have to check that the last activated sf::Context is still active (a RenderTarget activation may have deactivated it)
    if (currentContext && currentContext->m_context == priv::GlContext::getActiveContext())
        return currentContext;
    else
        return NULL;
}


////////////////////////////////////////////////////////////
Uint64 Context::getActiveContextId()
{
    return priv::GlContext::getActiveContextId();
}


////////////////////////////////////////////////////////////
bool Context::isExtensionAvailable(const char* name)
{
    return priv::GlContext::isExtensionAvailable(name);
}


////////////////////////////////////////////////////////////
GlFunctionPointer Context::getFunction(const char* name)
{
    return priv::GlContext::getFunction(name);
}


////////////////////////////////////////////////////////////
Context::Context(const ContextSettings& settings, unsigned int width, unsigned int height)
{
    m_context = priv::GlContext::create(settings, width, height);
    setActive(true);
}

} // namespace sf
