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
#include <SFML/OpenGL.hpp>
#include <SFML/Window/glext/glext.h>
#include <cstdlib>


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
    // This thread-local variable will hold the "global" context for each thread
    sf::ThreadLocalPtr<sf::priv::GlContext> threadContext(NULL);

    // Now we create two global contexts.
    // The first one is the reference context: it will be shared with every other
    // context, and it can't be activated if we want the sharing operation to always succeed.
    // That's why we need the second context: this one will be activated and used
    // in the main thread whenever there's no other context (window) active.
    ContextType referenceContext(NULL);
    ContextType defaultContext(&referenceContext);
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
GlContext* GlContext::New()
{
    return new ContextType(&referenceContext);
}


////////////////////////////////////////////////////////////
GlContext* GlContext::New(const WindowImpl* owner, unsigned int bitsPerPixel, const ContextSettings& settings)
{
    ContextType* context = new ContextType(&referenceContext, owner, bitsPerPixel, settings);

    // Enable antialiasing if needed
    if (context->GetSettings().AntialiasingLevel > 0)
        glEnable(GL_MULTISAMPLE_ARB);

    return context;
}


////////////////////////////////////////////////////////////
GlContext::~GlContext()
{
    if (threadContext == this)
    {
        threadContext = NULL;
    }
    else if (threadContext)
    {
        // Don't call this->SetActive(false), it would lead to a pure virtual function call
        threadContext->SetActive(true);
    }
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
        // Activate the context
        if (MakeCurrent())
        {
            // If this is the first context to be activated on this thread, make
            // it the reference context for the whole thread.
            // referenceContext must *not* be the threadContext of the main thread
            if (!threadContext && (this != &referenceContext))
                threadContext = this;

            return true;
        }
    }
    else
    {
        // Deactivate the context
        if (threadContext && (threadContext != this))
        {
            // To deactivate the context, we actually activate another one
            // so that we make sure that there is always an active context
            // for subsequent graphics operations
            return threadContext->SetActive(true);
        }
    }

    // If we got there then something failed
    return false;
}


////////////////////////////////////////////////////////////
bool GlContext::SetReferenceActive()
{
    if (threadContext)
        return threadContext->SetActive(true);
    else
        return false;
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
