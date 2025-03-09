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
#include <SFML/Window/Context.hpp>
#include <SFML/Window/GlContext.hpp>

#include <SFML/System/Err.hpp>

#include <ostream>
#include <utility>

#include <cassert>


namespace
{
// A nested named namespace is used here to allow unity builds of SFML.
namespace ContextImpl
{
// This per-thread variable holds the current context for each thread
thread_local sf::Context* currentContext(nullptr);
} // namespace ContextImpl
} // namespace

namespace sf
{
////////////////////////////////////////////////////////////
Context::Context() : m_context(priv::GlContext::create())
{
    if (!setActive(true))
        err() << "Failed to set context as active during construction" << std::endl;
}


////////////////////////////////////////////////////////////
Context::~Context()
{
    if (m_context && !setActive(false))
        err() << "Failed to set context as inactive during destruction" << std::endl;
}


////////////////////////////////////////////////////////////
Context::Context(Context&& context) noexcept : m_context(std::move(context.m_context))
{
    if (&context == ContextImpl::currentContext)
        ContextImpl::currentContext = this;
}


////////////////////////////////////////////////////////////
Context& Context::operator=(Context&& context) noexcept
{
    if (this == &context)
        return *this;

    m_context = std::move(context.m_context);
    if (&context == ContextImpl::currentContext)
        ContextImpl::currentContext = this;

    return *this;
}


////////////////////////////////////////////////////////////
bool Context::setActive(bool active)
{
    if (!m_context->setActive(active))
        return false;

    if (active)
        ContextImpl::currentContext = this;
    else if (this == ContextImpl::currentContext)
        ContextImpl::currentContext = nullptr;
    return true;
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
    if (currentContext && currentContext->m_context.get() == priv::GlContext::getActiveContext())
        return currentContext;

    return nullptr;
}


////////////////////////////////////////////////////////////
std::uint64_t Context::getActiveContextId()
{
    return priv::GlContext::getActiveContextId();
}


////////////////////////////////////////////////////////////
bool Context::isExtensionAvailable(std::string_view name)
{
    return priv::GlContext::isExtensionAvailable(name);
}


////////////////////////////////////////////////////////////
GlFunctionPointer Context::getFunction(const char* name)
{
    assert(name && "Context::getFunction Name must not be null");
    return priv::GlContext::getFunction(name);
}


////////////////////////////////////////////////////////////
Context::Context(const ContextSettings& settings, Vector2u size) : m_context(priv::GlContext::create(settings, size))
{
    if (!setActive(true))
        err() << "Failed to set context as active during construction" << std::endl;
}

} // namespace sf
