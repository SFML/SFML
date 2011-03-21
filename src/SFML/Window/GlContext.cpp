////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Window/glext/glext.h>
#include <vector>
#include <cstdlib>
#include <cassert>


#if defined(SFML_SYSTEM_WINDOWS)

    #include <SFML/Window/Win32/WglContext.hpp>
    typedef sf::priv::WglContext ContextType;

#elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD)

    #include <SFML/Window/Linux/GlxContext.hpp>
    typedef sf::priv::GlxContext ContextType;

#elif defined(SFML_SYSTEM_MACOS)

	#include <SFML/Window/OSX/SFContext.hpp>
	typedef sf::priv::SFContext ContextType;

#endif


////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
    // This per-thread variable holds the current context for each thread
    sf::ThreadLocalPtr<sf::priv::GlContext> currentContext(NULL);

    // The hidden, inactive context that will be shared with all other contexts
    ContextType* sharedContext = NULL;

    // Internal contexts
    sf::ThreadLocalPtr<sf::priv::GlContext> internalContext(NULL);
    std::vector<sf::priv::GlContext*> internalContexts;
    sf::Mutex internalContextsMutex;

    // Retrieve the internal context for the current thread
    sf::priv::GlContext* GetInternalContext()
    {
        if (!internalContext)
        {
            internalContext = sf::priv::GlContext::New();
            sf::Lock lock(internalContextsMutex);
            internalContexts.push_back(internalContext);
        }

        return internalContext;
    }
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
void GlContext::Initialize()
{
    // Create the shared context
    sharedContext = new ContextType(NULL);

    // This call makes sure that:
    // - the shared context is inactive (it must never be)
    // - another valid context is activated in the current thread
    sharedContext->SetActive(false);
}


////////////////////////////////////////////////////////////
void GlContext::Cleanup()
{
    // Destroy the shared context
    delete sharedContext;
    sharedContext = NULL;

    // Destroy the internal contexts
    sf::Lock lock(internalContextsMutex);
    for (std::vector<GlContext*>::iterator it = internalContexts.begin(); it != internalContexts.end(); ++it)
        delete *it;
    internalContexts.clear();
}


////////////////////////////////////////////////////////////
void GlContext::EnsureContext()
{
    // If there's no active context on the current thread, activate an internal one
    if (!currentContext)
        GetInternalContext()->SetActive(true);
}


////////////////////////////////////////////////////////////
GlContext* GlContext::New()
{
    return new ContextType(sharedContext);
}


////////////////////////////////////////////////////////////
GlContext* GlContext::New(const WindowImpl* owner, unsigned int bitsPerPixel, const ContextSettings& settings)
{
    // Make sure that there's an active context (context creation may need extensions, and thus a valid context)
    EnsureContext();

    // Create the context
    GlContext* context = new ContextType(sharedContext, owner, bitsPerPixel, settings);

    // Enable antialiasing if needed
    if (context->GetSettings().AntialiasingLevel > 0)
        glEnable(GL_MULTISAMPLE_ARB);

    return context;
}


////////////////////////////////////////////////////////////
GlContext::~GlContext()
{
    // Deactivate the context before killing it
    SetActive(false);
}


////////////////////////////////////////////////////////////
const ContextSettings& GlContext::GetSettings() const
{
    return mySettings;
}


////////////////////////////////////////////////////////////
bool GlContext::SetActive(bool active)
{
    if (active)
    {
        if (this != currentContext)
        {
            // Activate the context
            if (MakeCurrent())
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
            return GetInternalContext()->SetActive(true);
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
int GlContext::EvaluateFormat(unsigned int bitsPerPixel, const ContextSettings& settings, int colorBits, int depthBits, int stencilBits, int antialiasing)
{
    return std::abs(static_cast<int>(bitsPerPixel               - colorBits))   +
           std::abs(static_cast<int>(settings.DepthBits         - depthBits))   +
           std::abs(static_cast<int>(settings.StencilBits       - stencilBits)) +
           std::abs(static_cast<int>(settings.AntialiasingLevel - antialiasing));
}

} // namespace priv

} // namespace sf
