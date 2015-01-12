////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Window/GlContext.hpp>
#include <SFML/System/ThreadLocalPtr.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/OpenGL.hpp>
#include <set>
#include <cstdlib>

#if !defined(SFML_OPENGL_ES)

    #if defined(SFML_SYSTEM_WINDOWS)

        #include <SFML/Window/Win32/WglContext.hpp>
        typedef sf::priv::WglContext ContextType;

    #elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD)

        #include <SFML/Window/Unix/GlxContext.hpp>
        typedef sf::priv::GlxContext ContextType;

    #elif defined(SFML_SYSTEM_MACOS)

        #include <SFML/Window/OSX/SFContext.hpp>
        typedef sf::priv::SFContext ContextType;

    #endif

#else

    #if defined(SFML_SYSTEM_IOS)

        #include <SFML/Window/iOS/EaglContext.hpp>
        typedef sf::priv::EaglContext ContextType;

    #else

        #include <SFML/Window/EglContext.hpp>
        typedef sf::priv::EglContext ContextType;

    #endif

#endif

#if !defined(GL_MULTISAMPLE)
    #define GL_MULTISAMPLE 0x809D
#endif

#if !defined(GL_CONTEXT_FLAGS)
    #define GL_CONTEXT_FLAGS 0x821E
#endif

#if !defined(GL_CONTEXT_FLAG_DEBUG_BIT)
    #define GL_CONTEXT_FLAG_DEBUG_BIT 0x00000002
#endif

#if !defined(GL_CONTEXT_PROFILE_MASK)
    #define GL_CONTEXT_PROFILE_MASK 0x9126
#endif

#if !defined(GL_CONTEXT_CORE_PROFILE_BIT)
    #define GL_CONTEXT_CORE_PROFILE_BIT 0x00000001
#endif

#if !defined(GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
    #define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT 0x00000002
#endif


namespace
{
    // This per-thread variable holds the current context for each thread
    sf::ThreadLocalPtr<sf::priv::GlContext> currentContext(NULL);

    // The hidden, inactive context that will be shared with all other contexts
    ContextType* sharedContext = NULL;

    // Internal contexts
    sf::ThreadLocalPtr<sf::priv::GlContext> internalContext(NULL);
    std::set<sf::priv::GlContext*> internalContexts;
    sf::Mutex internalContextsMutex;

    // Check if the internal context of the current thread is valid
    bool hasInternalContext()
    {
        // The internal context can be null...
        if (!internalContext)
            return false;

        // ... or non-null but deleted from the list of internal contexts
        sf::Lock lock(internalContextsMutex);
        return internalContexts.find(internalContext) != internalContexts.end();
    }

    // Retrieve the internal context for the current thread
    sf::priv::GlContext* getInternalContext()
    {
        if (!hasInternalContext())
        {
            internalContext = sf::priv::GlContext::create(sf::ContextSettings(0, 0, 0, 2, 1, sharedContext->getSettings().attributeFlags & ~sf::ContextSettings::Core), 1, 1);
            sf::Lock lock(internalContextsMutex);
            internalContexts.insert(internalContext);
        }

        // Check if we need to re-create the internal context with debugging enabled
        if ((sharedContext->getSettings().attributeFlags & sf::ContextSettings::Debug) && !(internalContext->getSettings().attributeFlags & sf::ContextSettings::Debug))
        {
            sf::priv::GlContext* newInternalContext = sf::priv::GlContext::create(sf::ContextSettings(0, 0, 0, 2, 1, sf::ContextSettings::Debug), 1, 1);

            sf::Lock lock(internalContextsMutex);
            internalContexts.erase(internalContext);
            internalContexts.insert(newInternalContext);

            delete internalContext;
            internalContext = newInternalContext;
        }

        return internalContext;
    }
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
void GlContext::globalInit()
{
    // Create the shared context
    sharedContext = new ContextType(NULL);
    sharedContext->initialize();

    // This call makes sure that:
    // - the shared context is inactive (it must never be)
    // - another valid context is activated in the current thread
    sharedContext->setActive(false);
}


////////////////////////////////////////////////////////////
void GlContext::globalCleanup()
{
    // Destroy the shared context
    delete sharedContext;
    sharedContext = NULL;

    // Destroy the internal contexts
    sf::Lock lock(internalContextsMutex);
    for (std::set<GlContext*>::iterator it = internalContexts.begin(); it != internalContexts.end(); ++it)
        delete *it;
    internalContexts.clear();
}


////////////////////////////////////////////////////////////
void GlContext::ensureContext()
{
    // If there's no active context on the current thread, activate an internal one
    if (!currentContext)
        getInternalContext()->setActive(true);
}


////////////////////////////////////////////////////////////
GlContext* GlContext::create()
{
    GlContext* context = new ContextType(sharedContext, ContextSettings(0, 0, 0, 2, 1, sharedContext->getSettings().attributeFlags & ~sf::ContextSettings::Core), 1, 1);
    context->initialize();

    return context;
}


////////////////////////////////////////////////////////////
GlContext* GlContext::create(const ContextSettings& settings, const WindowImpl* owner, unsigned int bitsPerPixel)
{
    // Make sure that there's an active context (context creation may need extensions, and thus a valid context)
    ensureContext();

    // Check if we need to re-create the shared context with debugging enabled
    if ((settings.attributeFlags & ContextSettings::Debug) && !(sharedContext->getSettings().attributeFlags & ContextSettings::Debug))
    {
        ContextType* newSharedContext = new ContextType(sharedContext, ContextSettings(0, 0, 0, 2, 1, ContextSettings::Debug), 1, 1);
        newSharedContext->initialize();

        delete sharedContext;
        sharedContext = newSharedContext;
    }
    else if (sharedContext->getSettings().attributeFlags & ContextSettings::Debug)
    {
        sf::ContextSettings new_settings = settings;
        new_settings.attributeFlags |= ContextSettings::Debug;

        // Create the context
        GlContext* context = new ContextType(sharedContext, new_settings, owner, bitsPerPixel);
        context->initialize();

        return context;
    }

    // Create the context
    GlContext* context = new ContextType(sharedContext, settings, owner, bitsPerPixel);
    context->initialize();

    return context;
}


////////////////////////////////////////////////////////////
GlContext* GlContext::create(const ContextSettings& settings, unsigned int width, unsigned int height)
{
    // Make sure that there's an active context (context creation may need extensions, and thus a valid context)
    ensureContext();

    // Check if we need to re-create the shared context with debugging enabled
    if ((settings.attributeFlags & ContextSettings::Debug) && !(sharedContext->getSettings().attributeFlags && ContextSettings::Debug))
    {
        ContextType* newSharedContext = new ContextType(sharedContext, ContextSettings(0, 0, 0, 2, 1, ContextSettings::Debug), 1, 1);
        newSharedContext->initialize();

        delete sharedContext;
        sharedContext = newSharedContext;
    }
    else if (sharedContext->getSettings().attributeFlags & ContextSettings::Debug)
    {
        sf::ContextSettings new_settings = settings;
        new_settings.attributeFlags |= ContextSettings::Debug;

        // Create the context
        GlContext* context = new ContextType(sharedContext, new_settings, width, height);
        context->initialize();

        return context;
    }

    // Create the context
    GlContext* context = new ContextType(sharedContext, settings, width, height);
    context->initialize();

    return context;
}


////////////////////////////////////////////////////////////
GlContext::~GlContext()
{
    // Deactivate the context before killing it, unless we're inside Cleanup()
    if (sharedContext)
        setActive(false);
}


////////////////////////////////////////////////////////////
const ContextSettings& GlContext::getSettings() const
{
    return m_settings;
}


////////////////////////////////////////////////////////////
bool GlContext::setActive(bool active)
{
    if (active)
    {
        if (this != currentContext)
        {
            // Activate the context
            if (makeCurrent())
            {
                // Set it as the new current context for this thread
                currentContext = this;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            // This context is already the active one on this thread, don't do anything
            return true;
        }
    }
    else
    {
        if (this == currentContext)
        {
            // To deactivate the context, we actually activate another one so that we make
            // sure that there is always an active context for subsequent graphics operations
            return getInternalContext()->setActive(true);
        }
        else
        {
            // This context is not the active one on this thread, don't do anything
            return true;
        }
    }
}


////////////////////////////////////////////////////////////
GlContext::GlContext()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
int GlContext::evaluateFormat(unsigned int bitsPerPixel, const ContextSettings& settings, int colorBits, int depthBits, int stencilBits, int antialiasing)
{
    return std::abs(static_cast<int>(bitsPerPixel               - colorBits))   +
           std::abs(static_cast<int>(settings.depthBits         - depthBits))   +
           std::abs(static_cast<int>(settings.stencilBits       - stencilBits)) +
           std::abs(static_cast<int>(settings.antialiasingLevel - antialiasing));
}


////////////////////////////////////////////////////////////
void GlContext::initialize()
{
    // Activate the context
    setActive(true);

    // Retrieve the context version number
    const GLubyte* version = glGetString(GL_VERSION);
    if (version)
    {
        // The beginning of the returned string is "major.minor" (this is standard)
        m_settings.majorVersion = version[0] - '0';
        m_settings.minorVersion = version[2] - '0';
    }
    else
    {
        // Can't get the version number, assume 2.1
        m_settings.majorVersion = 2;
        m_settings.minorVersion = 1;
    }

    // Verify non-default settings, just to be extra sure
    if (m_settings.attributeFlags & ContextSettings::Debug)
    {
        // Retrieve the context flags
        int flags = 0;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

        if (!(flags & GL_CONTEXT_FLAG_DEBUG_BIT))
        {
            m_settings.attributeFlags ^= ContextSettings::Debug;
        }
    }

    if (m_settings.attributeFlags & ContextSettings::Core)
    {
        // Retrieve the context profile
        int profile = 0;
        glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

        if (!(profile & GL_CONTEXT_CORE_PROFILE_BIT))
        {
            m_settings.attributeFlags ^= ContextSettings::Core;
        }
    }

    // Enable antialiasing if needed
    if (m_settings.antialiasingLevel > 0)
        glEnable(GL_MULTISAMPLE);
}

} // namespace priv

} // namespace sf
