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
#include <SFML/Window/ContextGL.hpp>
#include <SFML/System/ThreadLocalPtr.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window/glext/glext.h>
#include <stdlib.h>


#if defined(SFML_SYSTEM_WINDOWS)

    #include <SFML/Window/Win32/ContextWGL.hpp>
    typedef sf::priv::ContextWGL ContextType;

#elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD)

    #include <SFML/Window/Linux/ContextGLX.hpp>
    typedef sf::priv::ContextGLX ContextType;

#elif defined(SFML_SYSTEM_MACOS)

	#include <SFML/Window/Cocoa/ContextAGL.hpp>
	typedef sf::priv::ContextAGL ContextType;

#endif


////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
    // This thread-local variable will hold the "gloabl" context for each thread
    sf::ThreadLocalPtr<sf::priv::ContextGL> ThreadContext(NULL);

    // Now we create two global contexts.
    // The first one is the reference context: it will be shared with every other
    // context, and he can't be activated if we want the sharing operation to always succeed.
    // That why we need the second context: this one will be activated and used
    // in the main thread whenever there's no other context (window) active.
    ContextType ReferenceContext(NULL);
    ContextType DefaultContext(&ReferenceContext);
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Create a new context, not associated to a window
////////////////////////////////////////////////////////////
ContextGL* ContextGL::New()
{
    return new ContextType(&ReferenceContext);
}


////////////////////////////////////////////////////////////
/// Create a new context attached to a window
////////////////////////////////////////////////////////////
ContextGL* ContextGL::New(const WindowImpl* Owner, unsigned int BitsPerPixel, const ContextSettings& Settings)
{
    ContextType* Context = new ContextType(&ReferenceContext, Owner, BitsPerPixel, Settings);

    // Enable antialiasing if needed
    if (Context->GetSettings().AntialiasingLevel > 0)
        glEnable(GL_MULTISAMPLE_ARB);

    return Context;
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
ContextGL::~ContextGL()
{
    if (ThreadContext == this)
    {
        ThreadContext = NULL;
    }
    else if (ThreadContext != NULL)
    {
        ThreadContext->SetActive(true);
    }
}


////////////////////////////////////////////////////////////
/// Get the settings of the context
////////////////////////////////////////////////////////////
const ContextSettings& ContextGL::GetSettings() const
{
    return mySettings;
}


////////////////////////////////////////////////////////////
/// Activate or deactivate the context as the current target
/// for rendering
////////////////////////////////////////////////////////////
bool ContextGL::SetActive(bool Active)
{
    if (MakeCurrent(Active))
    {
        if (Active && (ThreadContext == NULL))
        {
            // If this is the first context to be activated on this thread, make
            // it the reference context for the whole thread
            ThreadContext = this;
        }
        else if (!Active && (ThreadContext != NULL) && (ThreadContext != this))
        {
            // Activate the reference context for this thread to ensure
            // that there is always an active context for subsequent graphics operations
            ThreadContext->SetActive(true);
        }

        return true;
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
ContextGL::ContextGL()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
/// Evaluate a pixel format configuration.
/// This functions can be used by implementations that have
/// several valid formats and want to get the best one
////////////////////////////////////////////////////////////
int ContextGL::EvaluateFormat(unsigned int BitsPerPixel, const ContextSettings& Settings, int ColorBits, int DepthBits, int StencilBits, int Antialiasing)
{
    return abs(static_cast<int>(BitsPerPixel               - ColorBits))   +
           abs(static_cast<int>(Settings.DepthBits         - DepthBits))   +
           abs(static_cast<int>(Settings.StencilBits       - StencilBits)) +
           abs(static_cast<int>(Settings.AntialiasingLevel - Antialiasing));
}

} // namespace priv

} // namespace sf
