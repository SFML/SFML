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
#include <SFML/Window/Context.hpp>
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


namespace sf
{
////////////////////////////////////////////////////////////
/// Create a new context, not associated to a window
////////////////////////////////////////////////////////////
Context* Context::New()
{
    ContextType* Shared = static_cast<ContextType*>(&GetDefault());
    return new ContextType(Shared);
}


////////////////////////////////////////////////////////////
/// Create a new context attached to a window
////////////////////////////////////////////////////////////
Context* Context::New(const priv::WindowImpl* Owner, unsigned int BitsPerPixel, const ContextSettings& Settings)
{
    ContextType* Shared = static_cast<ContextType*>(&GetDefault());
    ContextType* NewContext = new ContextType(Shared, Owner, BitsPerPixel, Settings);

    // Enable antialiasing if needed
    if (NewContext->GetSettings().AntialiasingLevel > 0)
        glEnable(GL_MULTISAMPLE_ARB);

    return NewContext;
}


////////////////////////////////////////////////////////////
/// Check if a context is active on the current thread
////////////////////////////////////////////////////////////
bool Context::IsContextActive()
{
    return ContextType::IsContextActive();
}


////////////////////////////////////////////////////////////
/// Return the default context
////////////////////////////////////////////////////////////
Context& Context::GetDefault()
{
    static ContextType DefaultContext(NULL);

    return DefaultContext;
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
Context::~Context()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
/// Get the settings of the context
////////////////////////////////////////////////////////////
const ContextSettings& Context::GetSettings() const
{
    return mySettings;
}


////////////////////////////////////////////////////////////
/// Activate or deactivate the context as the current target
/// for rendering
////////////////////////////////////////////////////////////
bool Context::SetActive(bool Active)
{
    return MakeCurrent(Active);
}


////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
Context::Context()
{

}


////////////////////////////////////////////////////////////
/// Evaluate a pixel format configuration.
/// This functions can be used by implementations that have
/// several valid formats and want to get the best one
////////////////////////////////////////////////////////////
int Context::EvaluateFormat(unsigned int BitsPerPixel, const ContextSettings& Settings, int ColorBits, int DepthBits, int StencilBits, int Antialiasing)
{
    return abs(static_cast<int>(BitsPerPixel               - ColorBits))   +
           abs(static_cast<int>(Settings.DepthBits         - DepthBits))   +
           abs(static_cast<int>(Settings.StencilBits       - StencilBits)) +
           abs(static_cast<int>(Settings.AntialiasingLevel - Antialiasing));
}

} // namespace sf
