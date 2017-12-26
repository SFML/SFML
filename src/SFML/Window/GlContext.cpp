////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2018 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/Context.hpp>
#include <SFML/System/ThreadLocalPtr.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Err.hpp>
#include <SFML/OpenGL.hpp>
#include <algorithm>
#include <vector>
#include <string>
#include <set>
#include <utility>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cassert>

#if !defined(SFML_OPENGL_ES)

    #if defined(SFML_SYSTEM_WINDOWS)

        #include <SFML/Window/Win32/WglContext.hpp>
        typedef sf::priv::WglContext ContextType;

    #elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD) || defined(SFML_SYSTEM_OPENBSD)

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

#if defined(SFML_SYSTEM_WINDOWS)

    typedef const GLubyte* (APIENTRY *glGetStringiFuncType)(GLenum, GLuint);

#else

    typedef const GLubyte* (*glGetStringiFuncType)(GLenum, GLuint);

#endif

#if !defined(GL_MULTISAMPLE)
    #define GL_MULTISAMPLE 0x809D
#endif

#if !defined(GL_MAJOR_VERSION)
    #define GL_MAJOR_VERSION 0x821B
#endif

#if !defined(GL_MINOR_VERSION)
    #define GL_MINOR_VERSION 0x821C
#endif

#if !defined(GL_NUM_EXTENSIONS)
    #define GL_NUM_EXTENSIONS 0x821D
#endif

#if !defined(GL_CONTEXT_FLAGS)
    #define GL_CONTEXT_FLAGS 0x821E
#endif

#if !defined(GL_FRAMEBUFFER_SRGB)
    #define GL_FRAMEBUFFER_SRGB 0x8DB9
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
    // AMD drivers have issues with internal synchronization
    // We need to make sure that no operating system context
    // or pixel format operations are performed simultaneously
    // This mutex is also used to protect the shared context
    // from being locked on multiple threads and for managing
    // the resource count
    sf::Mutex mutex;

    // OpenGL resources counter
    unsigned int resourceCount = 0;

    // This per-thread variable holds the current context for each thread
    sf::ThreadLocalPtr<sf::priv::GlContext> currentContext(NULL);

    // The hidden, inactive context that will be shared with all other contexts
    ContextType* sharedContext = NULL;

    // Unique identifier, used for identifying contexts when managing unshareable OpenGL resources
    sf::Uint64 id = 1; // start at 1, zero is "no context"

    // Set containing callback functions to be called whenever a
    // context is going to be destroyed
    // Unshareable OpenGL resources rely on this to clean up properly
    // whenever a context containing them is destroyed
    typedef std::set<std::pair<sf::ContextDestroyCallback, void*> > ContextDestroyCallbacks;
    ContextDestroyCallbacks contextDestroyCallbacks;

    // This structure contains all the state necessary to
    // track TransientContext usage
    struct TransientContext : private sf::NonCopyable
    {
        ////////////////////////////////////////////////////////////
        /// \brief Constructor
        ///
        ////////////////////////////////////////////////////////////
        TransientContext() :
        referenceCount   (0),
        context          (0),
        sharedContextLock(0),
        useSharedContext (false)
        {
            if (resourceCount == 0)
            {
                context = new sf::Context;
            }
            else if (!currentContext)
            {
                sharedContextLock = new sf::Lock(mutex);
                useSharedContext = true;
                sharedContext->setActive(true);
            }
        }

        ////////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        ////////////////////////////////////////////////////////////
        ~TransientContext()
        {
            if (useSharedContext)
                sharedContext->setActive(false);

            delete sharedContextLock;
            delete context;
        }

        ///////////////////////////////////////////////////////////
        // Member data
        ////////////////////////////////////////////////////////////
        unsigned int referenceCount;
        sf::Context* context;
        sf::Lock*    sharedContextLock;
        bool         useSharedContext;
    };

    // This per-thread variable tracks if and how a transient
    // context is currently being used on the current thread
    sf::ThreadLocalPtr<TransientContext> transientContext(NULL);

    // Supported OpenGL extensions
    std::vector<std::string> extensions;

    // Helper to parse OpenGL version strings
    bool parseVersionString(const char* version, const char* prefix, unsigned int &major, unsigned int &minor)
    {
        std::size_t prefixLength = std::strlen(prefix);

        if ((std::strlen(version) >= (prefixLength + 3)) &&
            (std::strncmp(version, prefix, prefixLength) == 0) &&
            std::isdigit(version[prefixLength]) &&
            (version[prefixLength + 1] == '.') &&
            std::isdigit(version[prefixLength + 2]))
        {
            major = version[prefixLength] - '0';
            minor = version[prefixLength + 2] - '0';

            return true;
        }

        return false;
    }
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
void GlContext::initResource()
{
    // Protect from concurrent access
    Lock lock(mutex);

    // If this is the very first resource, trigger the global context initialization
    if (resourceCount == 0)
    {
        if (sharedContext)
        {
            // Increment the resources counter
            resourceCount++;

            return;
        }

        // Create the shared context
        sharedContext = new ContextType(NULL);
        sharedContext->initialize(ContextSettings());

        // Load our extensions vector
        extensions.clear();

        // Check whether a >= 3.0 context is available
        int majorVersion = 0;
        glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);

        if (glGetError() == GL_INVALID_ENUM)
        {
            // Try to load the < 3.0 way
            const char* extensionString = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));

            do
            {
                const char* extension = extensionString;

                while(*extensionString && (*extensionString != ' '))
                    extensionString++;

                extensions.push_back(std::string(extension, extensionString));
            }
            while (*extensionString++);
        }
        else
        {
            // Try to load the >= 3.0 way
            glGetStringiFuncType glGetStringiFunc = NULL;
            glGetStringiFunc = reinterpret_cast<glGetStringiFuncType>(getFunction("glGetStringi"));

            if (glGetStringiFunc)
            {
                int numExtensions = 0;
                glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

                if (numExtensions)
                {
                    for (unsigned int i = 0; i < static_cast<unsigned int>(numExtensions); ++i)
                    {
                        const char* extensionString = reinterpret_cast<const char*>(glGetStringiFunc(GL_EXTENSIONS, i));

                        extensions.push_back(extensionString);
                    }
                }
            }
        }

        // Deactivate the shared context so that others can activate it when necessary
        sharedContext->setActive(false);
    }

    // Increment the resources counter
    resourceCount++;
}


////////////////////////////////////////////////////////////
void GlContext::cleanupResource()
{
    // Protect from concurrent access
    Lock lock(mutex);

    // Decrement the resources counter
    resourceCount--;

    // If there's no more resource alive, we can trigger the global context cleanup
    if (resourceCount == 0)
    {
        if (!sharedContext)
            return;

        // Destroy the shared context
        delete sharedContext;
        sharedContext = NULL;
    }
}


////////////////////////////////////////////////////////////
void GlContext::registerContextDestroyCallback(ContextDestroyCallback callback, void* arg)
{
    contextDestroyCallbacks.insert(std::make_pair(callback, arg));
}


////////////////////////////////////////////////////////////
void GlContext::acquireTransientContext()
{
    // Protect from concurrent access
    Lock lock(mutex);

    // If this is the first TransientContextLock on this thread
    // construct the state object
    if (!transientContext)
        transientContext = new TransientContext;

    // Increase the reference count
    transientContext->referenceCount++;
}


////////////////////////////////////////////////////////////
void GlContext::releaseTransientContext()
{
    // Protect from concurrent access
    Lock lock(mutex);

    // Make sure a matching acquireTransientContext() was called
    assert(transientContext);

    // Decrease the reference count
    transientContext->referenceCount--;

    // If this is the last TransientContextLock that is released
    // destroy the state object
    if (transientContext->referenceCount == 0)
    {
        delete transientContext;
        transientContext = NULL;
    }
}


////////////////////////////////////////////////////////////
GlContext* GlContext::create()
{
    // Make sure that there's an active context (context creation may need extensions, and thus a valid context)
    assert(sharedContext != NULL);

    Lock lock(mutex);

    GlContext* context = NULL;

    // We don't use acquireTransientContext here since we have
    // to ensure we have exclusive access to the shared context
    // in order to make sure it is not active during context creation
    {
        sharedContext->setActive(true);

        // Create the context
        context = new ContextType(sharedContext);

        sharedContext->setActive(false);
    }

    context->initialize(ContextSettings());

    return context;
}


////////////////////////////////////////////////////////////
GlContext* GlContext::create(const ContextSettings& settings, const WindowImpl* owner, unsigned int bitsPerPixel)
{
    // Make sure that there's an active context (context creation may need extensions, and thus a valid context)
    assert(sharedContext != NULL);

    Lock lock(mutex);

    GlContext* context = NULL;

    // We don't use acquireTransientContext here since we have
    // to ensure we have exclusive access to the shared context
    // in order to make sure it is not active during context creation
    {
        sharedContext->setActive(true);

        // Create the context
        context = new ContextType(sharedContext, settings, owner, bitsPerPixel);

        sharedContext->setActive(false);
    }

    context->initialize(settings);
    context->checkSettings(settings);

    return context;
}


////////////////////////////////////////////////////////////
GlContext* GlContext::create(const ContextSettings& settings, unsigned int width, unsigned int height)
{
    // Make sure that there's an active context (context creation may need extensions, and thus a valid context)
    assert(sharedContext != NULL);

    Lock lock(mutex);

    GlContext* context = NULL;

    // We don't use acquireTransientContext here since we have
    // to ensure we have exclusive access to the shared context
    // in order to make sure it is not active during context creation
    {
        sharedContext->setActive(true);

        // Create the context
        context = new ContextType(sharedContext, settings, width, height);

        sharedContext->setActive(false);
    }

    context->initialize(settings);
    context->checkSettings(settings);

    return context;
}


////////////////////////////////////////////////////////////
bool GlContext::isExtensionAvailable(const char* name)
{
    return std::find(extensions.begin(), extensions.end(), name) != extensions.end();
}


////////////////////////////////////////////////////////////
GlFunctionPointer GlContext::getFunction(const char* name)
{
#if !defined(SFML_OPENGL_ES)

    Lock lock(mutex);

    return ContextType::getFunction(name);

#else

    return 0;

#endif
}


////////////////////////////////////////////////////////////
Uint64 GlContext::getActiveContextId()
{
    return currentContext ? currentContext->m_id : 0;
}


////////////////////////////////////////////////////////////
GlContext::~GlContext()
{
    // Deactivate the context before killing it, unless we're inside Cleanup()
    if (sharedContext)
    {
        if (this == currentContext)
            currentContext = NULL;
    }
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
            Lock lock(mutex);

            // Activate the context
            if (makeCurrent(true))
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
            Lock lock(mutex);

            // Deactivate the context
            if (makeCurrent(false))
            {
                currentContext = NULL;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            // This context is not the active one on this thread, don't do anything
            return true;
        }
    }
}


////////////////////////////////////////////////////////////
GlContext::GlContext() :
m_id(id++)
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
int GlContext::evaluateFormat(unsigned int bitsPerPixel, const ContextSettings& settings, int colorBits, int depthBits, int stencilBits, int antialiasing, bool accelerated, bool sRgb)
{
    int colorDiff        = static_cast<int>(bitsPerPixel)               - colorBits;
    int depthDiff        = static_cast<int>(settings.depthBits)         - depthBits;
    int stencilDiff      = static_cast<int>(settings.stencilBits)       - stencilBits;
    int antialiasingDiff = static_cast<int>(settings.antialiasingLevel) - antialiasing;

    // Weight sub-scores so that better settings don't score equally as bad as worse settings
    colorDiff        *= ((colorDiff        > 0) ? 100000 : 1);
    depthDiff        *= ((depthDiff        > 0) ? 100000 : 1);
    stencilDiff      *= ((stencilDiff      > 0) ? 100000 : 1);
    antialiasingDiff *= ((antialiasingDiff > 0) ? 100000 : 1);

    // Aggregate the scores
    int score = std::abs(colorDiff) + std::abs(depthDiff) + std::abs(stencilDiff) + std::abs(antialiasingDiff);

    // If the user wants an sRGB capable format, try really hard to get one
    if (settings.sRgbCapable && !sRgb)
        score += 10000000;

    // Make sure we prefer hardware acceleration over features
    if (!accelerated)
        score += 100000000;

    return score;
}


////////////////////////////////////////////////////////////
void GlContext::cleanupUnsharedResources()
{
    // Save the current context so we can restore it later
    GlContext* contextToRestore = currentContext;

    // If this context is already active there is no need to save it
    if (contextToRestore == this)
        contextToRestore = NULL;

    // Make this context active so resources can be freed
    setActive(true);

    // Call the registered destruction callbacks
    for (ContextDestroyCallbacks::iterator iter = contextDestroyCallbacks.begin(); iter != contextDestroyCallbacks.end(); ++iter)
        iter->first(iter->second);

    // Make the originally active context active again
    if (contextToRestore)
        contextToRestore->setActive(true);
}


////////////////////////////////////////////////////////////
void GlContext::initialize(const ContextSettings& requestedSettings)
{
    // Activate the context
    setActive(true);

    // Retrieve the context version number
    int majorVersion = 0;
    int minorVersion = 0;

    // Try the new way first
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

    if (glGetError() != GL_INVALID_ENUM)
    {
        m_settings.majorVersion = static_cast<unsigned int>(majorVersion);
        m_settings.minorVersion = static_cast<unsigned int>(minorVersion);
    }
    else
    {
        // Try the old way

        // If we can't get the version number, assume 1.1
        m_settings.majorVersion = 1;
        m_settings.minorVersion = 1;

        const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        if (version)
        {
            // OpenGL ES Common Lite profile: The beginning of the returned string is "OpenGL ES-CL major.minor"
            // OpenGL ES Common profile:      The beginning of the returned string is "OpenGL ES-CM major.minor"
            // OpenGL ES Full profile:        The beginning of the returned string is "OpenGL ES major.minor"
            // Desktop OpenGL:                The beginning of the returned string is "major.minor"

            if (!parseVersionString(version, "OpenGL ES-CL ", m_settings.majorVersion, m_settings.minorVersion) &&
                !parseVersionString(version, "OpenGL ES-CM ", m_settings.majorVersion, m_settings.minorVersion) &&
                !parseVersionString(version, "OpenGL ES ",    m_settings.majorVersion, m_settings.minorVersion) &&
                !parseVersionString(version, "",              m_settings.majorVersion, m_settings.minorVersion))
            {
                err() << "Unable to parse OpenGL version string: \"" << version << "\", defaulting to 1.1" << std::endl;
            }
        }
        else
        {
            err() << "Unable to retrieve OpenGL version string, defaulting to 1.1" << std::endl;
        }
    }

    // 3.0 contexts only deprecate features, but do not remove them yet
    // 3.1 contexts remove features if ARB_compatibility is not present
    // 3.2+ contexts remove features only if a core profile is requested

    // If the context was created with wglCreateContext, it is guaranteed to be compatibility.
    // If a 3.0 context was created with wglCreateContextAttribsARB, it is guaranteed to be compatibility.
    // If a 3.1 context was created with wglCreateContextAttribsARB, the compatibility flag
    // is set only if ARB_compatibility is present
    // If a 3.2+ context was created with wglCreateContextAttribsARB, the compatibility flag
    // would have been set correctly already depending on whether ARB_create_context_profile is supported.

    // If the user requests a 3.0 context, it will be a compatibility context regardless of the requested profile.
    // If the user requests a 3.1 context and its creation was successful, the specification
    // states that it will not be a compatibility profile context regardless of the requested
    // profile unless ARB_compatibility is present.

    m_settings.attributeFlags = ContextSettings::Default;

    if (m_settings.majorVersion >= 3)
    {
        // Retrieve the context flags
        int flags = 0;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
            m_settings.attributeFlags |= ContextSettings::Debug;

        if ((m_settings.majorVersion == 3) && (m_settings.minorVersion == 1))
        {
            m_settings.attributeFlags |= ContextSettings::Core;

            glGetStringiFuncType glGetStringiFunc = reinterpret_cast<glGetStringiFuncType>(getFunction("glGetStringi"));

            if (glGetStringiFunc)
            {
                int numExtensions = 0;
                glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

                for (unsigned int i = 0; i < static_cast<unsigned int>(numExtensions); ++i)
                {
                    const char* extensionString = reinterpret_cast<const char*>(glGetStringiFunc(GL_EXTENSIONS, i));

                    if (std::strstr(extensionString, "GL_ARB_compatibility"))
                    {
                        m_settings.attributeFlags &= ~static_cast<Uint32>(ContextSettings::Core);
                        break;
                    }
                }
            }
        }
        else if ((m_settings.majorVersion > 3) || (m_settings.minorVersion >= 2))
        {
            // Retrieve the context profile
            int profile = 0;
            glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

            if (profile & GL_CONTEXT_CORE_PROFILE_BIT)
                m_settings.attributeFlags |= ContextSettings::Core;
        }
    }

    // Enable anti-aliasing if requested by the user and supported
    if ((requestedSettings.antialiasingLevel > 0) && (m_settings.antialiasingLevel > 0))
    {
        glEnable(GL_MULTISAMPLE);
    }
    else
    {
        m_settings.antialiasingLevel = 0;
    }

    // Enable sRGB if requested by the user and supported
    if (requestedSettings.sRgbCapable && m_settings.sRgbCapable)
    {
        glEnable(GL_FRAMEBUFFER_SRGB);

        // Check to see if the enable was successful
        if (glIsEnabled(GL_FRAMEBUFFER_SRGB) == GL_FALSE)
        {
            err() << "Warning: Failed to enable GL_FRAMEBUFFER_SRGB" << std::endl;
            m_settings.sRgbCapable = false;
        }
    }
    else
    {
        m_settings.sRgbCapable = false;
    }
}


////////////////////////////////////////////////////////////
void GlContext::checkSettings(const ContextSettings& requestedSettings)
{
    // Perform checks to inform the user if they are getting a context they might not have expected

    // Detect any known non-accelerated implementations and warn
    const char* vendorName = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    const char* rendererName = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

    if (vendorName && rendererName)
    {
        if ((std::strcmp(vendorName, "Microsoft Corporation") == 0) && (std::strcmp(rendererName, "GDI Generic") == 0))
        {
            err() << "Warning: Detected \"Microsoft Corporation GDI Generic\" OpenGL implementation" << std::endl
                  << "The current OpenGL implementation is not hardware-accelerated" << std::endl;
        }
    }

    int version = m_settings.majorVersion * 10 + m_settings.minorVersion;
    int requestedVersion = requestedSettings.majorVersion * 10 + requestedSettings.minorVersion;

    if ((m_settings.attributeFlags    != requestedSettings.attributeFlags)    ||
        (version                      <  requestedVersion)                    ||
        (m_settings.stencilBits       <  requestedSettings.stencilBits)       ||
        (m_settings.antialiasingLevel <  requestedSettings.antialiasingLevel) ||
        (m_settings.depthBits         <  requestedSettings.depthBits)         ||
        (!m_settings.sRgbCapable      && requestedSettings.sRgbCapable))
    {
        err() << "Warning: The created OpenGL context does not fully meet the settings that were requested" << std::endl;
        err() << "Requested: version = " << requestedSettings.majorVersion << "." << requestedSettings.minorVersion
              << " ; depth bits = " << requestedSettings.depthBits
              << " ; stencil bits = " << requestedSettings.stencilBits
              << " ; AA level = " << requestedSettings.antialiasingLevel
              << std::boolalpha
              << " ; core = " << ((requestedSettings.attributeFlags & ContextSettings::Core) != 0)
              << " ; debug = " << ((requestedSettings.attributeFlags & ContextSettings::Debug) != 0)
              << " ; sRGB = " << requestedSettings.sRgbCapable
              << std::noboolalpha << std::endl;
        err() << "Created: version = " << m_settings.majorVersion << "." << m_settings.minorVersion
              << " ; depth bits = " << m_settings.depthBits
              << " ; stencil bits = " << m_settings.stencilBits
              << " ; AA level = " << m_settings.antialiasingLevel
              << std::boolalpha
              << " ; core = " << ((m_settings.attributeFlags & ContextSettings::Core) != 0)
              << " ; debug = " << ((m_settings.attributeFlags & ContextSettings::Debug) != 0)
              << " ; sRGB = " << m_settings.sRgbCapable
              << std::noboolalpha << std::endl;
    }
}

} // namespace priv

} // namespace sf
