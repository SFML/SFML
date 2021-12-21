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
#include <SFML/Window/GlContext.hpp>
#include <SFML/Window/Context.hpp>
#include <SFML/Window/EglContext.hpp>
#include <SFML/System/Err.hpp>
#include <glad/gl.h>
#include <algorithm>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <cstring>


#if defined(SFML_SYSTEM_WINDOWS)

    #if defined(SFML_OPENGL_ES)

        using ContextType = sf::priv::EglContext;

    #else

        #include <SFML/Window/Win32/WglContext.hpp>
        using ContextType = sf::priv::WglContext;

    #endif

#elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD) || defined(SFML_SYSTEM_OPENBSD) || defined(SFML_SYSTEM_NETBSD)

    #if defined(SFML_OPENGL_ES)

        using ContextType = sf::priv::EglContext;

    #else

        #include <SFML/Window/Unix/GlxContext.hpp>
        using ContextType = sf::priv::GlxContext;

    #endif

#elif defined(SFML_SYSTEM_MACOS)

    #include <SFML/Window/OSX/SFContext.hpp>
    using ContextType = sf::priv::SFContext;

#elif defined(SFML_SYSTEM_IOS)

    #include <SFML/Window/iOS/EaglContext.hpp>
    using ContextType = sf::priv::EaglContext;

#elif defined(SFML_SYSTEM_ANDROID)

    using ContextType = sf::priv::EglContext;

#endif

#if defined(SFML_SYSTEM_WINDOWS)

    using glEnableFuncType = void (APIENTRY *)(GLenum);
    using glGetErrorFuncType = GLenum (APIENTRY *)();
    using glGetIntegervFuncType = void (APIENTRY *)(GLenum, GLint*);
    using glGetStringFuncType = const GLubyte* (APIENTRY *)(GLenum);
    using glGetStringiFuncType = const GLubyte* (APIENTRY *)(GLenum, GLuint);
    using glIsEnabledFuncType = GLboolean (APIENTRY *)(GLenum);

#else

    using glEnableFuncType = void (*)(GLenum);
    using glGetErrorFuncType = GLenum (*)();
    using glGetIntegervFuncType = void (*)(GLenum, GLint*);
    using glGetStringFuncType = const GLubyte* (*)(GLenum);
    using glGetStringiFuncType = const GLubyte* (*)(GLenum, GLuint);
    using glIsEnabledFuncType = GLboolean (*)(GLenum);

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
    // A nested named namespace is used here to allow unity builds of SFML.
    namespace GlContextImpl
    {
        // AMD drivers have issues with internal synchronization
        // We need to make sure that no operating system context
        // or pixel format operations are performed simultaneously
        // This mutex is also used to protect the shared context
        // from being locked on multiple threads and for managing
        // the resource count
        std::recursive_mutex mutex;

        // OpenGL resources counter
        unsigned int resourceCount = 0;

        // This per-thread variable holds the current context for each thread
        thread_local sf::priv::GlContext* currentContext(nullptr);

        // The hidden, inactive context that will be shared with all other contexts
        std::unique_ptr<ContextType> sharedContext;

        // Unique identifier, used for identifying contexts when managing unshareable OpenGL resources
        sf::Uint64 id = 1; // start at 1, zero is "no context"

        // Set containing callback functions to be called whenever a
        // context is going to be destroyed
        // Unshareable OpenGL resources rely on this to clean up properly
        // whenever a context containing them is destroyed
        using ContextDestroyCallbacks = std::unordered_map<sf::ContextDestroyCallback, void *>;
        ContextDestroyCallbacks contextDestroyCallbacks;

        // This structure contains all the state necessary to
        // track TransientContext usage
        struct TransientContext
        {
            ////////////////////////////////////////////////////////////
            /// \brief Constructor
            ///
            ////////////////////////////////////////////////////////////
            TransientContext() :
            referenceCount   (0),
            context          (nullptr),
            sharedContextLock(),
            useSharedContext (false)
            {
                if (resourceCount == 0)
                {
                    context = std::make_unique<sf::Context>();
                }
                else if (!currentContext)
                {
                    sharedContextLock.emplace(mutex);
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

                sharedContextLock.reset();
                context.reset();
            }

            ////////////////////////////////////////////////////////////
            /// \brief Deleted copy constructor
            ///
            ////////////////////////////////////////////////////////////
            TransientContext(const TransientContext&) = delete;

            ////////////////////////////////////////////////////////////
            /// \brief Deleted copy assignment
            ///
            ////////////////////////////////////////////////////////////
            TransientContext& operator=(const TransientContext&) = delete;

            ///////////////////////////////////////////////////////////
            // Member data
            ////////////////////////////////////////////////////////////
            unsigned int                                          referenceCount;
            std::unique_ptr<sf::Context>                          context;
            std::optional<std::scoped_lock<std::recursive_mutex>> sharedContextLock;
            bool                                                  useSharedContext;
        };

        // This per-thread variable tracks if and how a transient
        // context is currently being used on the current thread
        thread_local std::unique_ptr<TransientContext> transientContext;

        // Supported OpenGL extensions
        std::vector<std::string> extensions;

        // Load our extensions vector with the supported extensions
        void loadExtensions()
        {
            extensions.clear();

            auto glGetErrorFunc = reinterpret_cast<glGetErrorFuncType>(sf::priv::GlContext::getFunction("glGetError"));
            auto glGetIntegervFunc = reinterpret_cast<glGetIntegervFuncType>(sf::priv::GlContext::getFunction("glGetIntegerv"));
            auto glGetStringFunc = reinterpret_cast<glGetStringFuncType>(sf::priv::GlContext::getFunction("glGetString"));

            if (!glGetErrorFunc || !glGetIntegervFunc || !glGetStringFunc)
                return;

            // Check whether a >= 3.0 context is available
            int majorVersion = 0;
            glGetIntegervFunc(GL_MAJOR_VERSION, &majorVersion);

            auto glGetStringiFunc = reinterpret_cast<glGetStringiFuncType>(sf::priv::GlContext::getFunction("glGetStringi"));

            if (glGetErrorFunc() == GL_INVALID_ENUM || !glGetStringiFunc)
            {
                // Try to load the < 3.0 way
                const char* extensionString = reinterpret_cast<const char*>(glGetStringFunc(GL_EXTENSIONS));

                do
                {
                    const char* extension = extensionString;

                    while (*extensionString && (*extensionString != ' '))
                        extensionString++;

                    extensions.emplace_back(extension, extensionString);
                }
                while (*extensionString++);
            }
            else
            {
                // Try to load the >= 3.0 way
                int numExtensions = 0;
                glGetIntegervFunc(GL_NUM_EXTENSIONS, &numExtensions);

                if (numExtensions)
                {
                    for (unsigned int i = 0; i < static_cast<unsigned int>(numExtensions); ++i)
                    {
                        const char* extensionString = reinterpret_cast<const char*>(glGetStringiFunc(GL_EXTENSIONS, i));
                        extensions.emplace_back(extensionString);
                    }
                }
            }
        }

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
                major = static_cast<unsigned int>(version[prefixLength] - '0');
                minor = static_cast<unsigned int>(version[prefixLength + 2] - '0');

                return true;
            }

            return false;
        }
    }
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
void GlContext::initResource()
{
    using GlContextImpl::mutex;
    using GlContextImpl::resourceCount;
    using GlContextImpl::currentContext;
    using GlContextImpl::sharedContext;
    using GlContextImpl::loadExtensions;

    // Protect from concurrent access
    std::scoped_lock lock(mutex);

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
        sharedContext = std::make_unique<ContextType>(nullptr);
        sharedContext->initialize(ContextSettings());

        // Load our extensions vector
        loadExtensions();

        // Deactivate the shared context so that others can activate it when necessary
        sharedContext->setActive(false);
    }

    // Increment the resources counter
    resourceCount++;
}


////////////////////////////////////////////////////////////
void GlContext::cleanupResource()
{
    using GlContextImpl::mutex;
    using GlContextImpl::resourceCount;
    using GlContextImpl::sharedContext;

    // Protect from concurrent access
    std::scoped_lock lock(mutex);

    // Decrement the resources counter
    resourceCount--;

    // If there's no more resource alive, we can trigger the global context cleanup
    if (resourceCount == 0)
    {
        if (!sharedContext)
            return;

        // Destroy the shared context
        sharedContext.reset();
    }
}


////////////////////////////////////////////////////////////
void GlContext::registerContextDestroyCallback(ContextDestroyCallback callback, void* arg)
{
    GlContextImpl::contextDestroyCallbacks.emplace(callback, arg);
}


////////////////////////////////////////////////////////////
void GlContext::acquireTransientContext()
{
    using GlContextImpl::mutex;
    using GlContextImpl::TransientContext;
    using GlContextImpl::transientContext;

    // Protect from concurrent access
    std::scoped_lock lock(mutex);

    // If this is the first TransientContextLock on this thread
    // construct the state object
    if (!transientContext)
        transientContext = std::make_unique<TransientContext>();

    // Increase the reference count
    transientContext->referenceCount++;
}


////////////////////////////////////////////////////////////
void GlContext::releaseTransientContext()
{
    using GlContextImpl::mutex;
    using GlContextImpl::transientContext;

    // Protect from concurrent access
    std::scoped_lock lock(mutex);

    // Make sure a matching acquireTransientContext() was called
    assert(transientContext);

    // Decrease the reference count
    transientContext->referenceCount--;

    // If this is the last TransientContextLock that is released
    // destroy the state object
    if (transientContext->referenceCount == 0)
    {
        transientContext.reset();
    }
}


////////////////////////////////////////////////////////////
std::unique_ptr<GlContext> GlContext::create()
{
    using GlContextImpl::mutex;
    using GlContextImpl::sharedContext;

    // Make sure that there's an active context (context creation may need extensions, and thus a valid context)
    assert(sharedContext != nullptr);

    std::scoped_lock lock(mutex);

    std::unique_ptr<GlContext> context;

    // We don't use acquireTransientContext here since we have
    // to ensure we have exclusive access to the shared context
    // in order to make sure it is not active during context creation
    {
        sharedContext->setActive(true);

        // Create the context
        context = std::make_unique<ContextType>(sharedContext.get());

        sharedContext->setActive(false);
    }

    context->initialize(ContextSettings());

    return context;
}


////////////////////////////////////////////////////////////
std::unique_ptr<GlContext> GlContext::create(const ContextSettings& settings, const WindowImpl& owner, unsigned int bitsPerPixel)
{
    using GlContextImpl::mutex;
    using GlContextImpl::resourceCount;
    using GlContextImpl::sharedContext;
    using GlContextImpl::loadExtensions;

    // Make sure that there's an active context (context creation may need extensions, and thus a valid context)
    assert(sharedContext != nullptr);

    std::scoped_lock lock(mutex);

    // If resourceCount is 1 we know that we are inside sf::Context or sf::Window
    // Only in this situation we allow the user to indirectly re-create the shared context as a core context

    // Check if we need to convert our shared context into a core context
    if ((resourceCount == 1) &&
        (settings.attributeFlags & ContextSettings::Core) &&
        !(sharedContext->m_settings.attributeFlags & ContextSettings::Core))
    {
        // Re-create our shared context as a core context
        ContextSettings sharedSettings(0, 0, 0, settings.majorVersion, settings.minorVersion, settings.attributeFlags);

        sharedContext = std::make_unique<ContextType>(nullptr, sharedSettings, 1, 1);
        sharedContext->initialize(sharedSettings);

        // Reload our extensions vector
        loadExtensions();
    }

    std::unique_ptr<GlContext> context;

    // We don't use acquireTransientContext here since we have
    // to ensure we have exclusive access to the shared context
    // in order to make sure it is not active during context creation
    {
        sharedContext->setActive(true);

        // Create the context
        context = std::make_unique<ContextType>(sharedContext.get(), settings, owner, bitsPerPixel);

        sharedContext->setActive(false);
    }

    context->initialize(settings);
    context->checkSettings(settings);

    return context;
}


////////////////////////////////////////////////////////////
std::unique_ptr<GlContext> GlContext::create(const ContextSettings& settings, unsigned int width, unsigned int height)
{
    using GlContextImpl::mutex;
    using GlContextImpl::resourceCount;
    using GlContextImpl::sharedContext;
    using GlContextImpl::loadExtensions;

    // Make sure that there's an active context (context creation may need extensions, and thus a valid context)
    assert(sharedContext != nullptr);

    std::scoped_lock lock(mutex);

    // If resourceCount is 1 we know that we are inside sf::Context or sf::Window
    // Only in this situation we allow the user to indirectly re-create the shared context as a core context

    // Check if we need to convert our shared context into a core context
    if ((resourceCount == 1) &&
        (settings.attributeFlags & ContextSettings::Core) &&
        !(sharedContext->m_settings.attributeFlags & ContextSettings::Core))
    {
        // Re-create our shared context as a core context
        ContextSettings sharedSettings(0, 0, 0, settings.majorVersion, settings.minorVersion, settings.attributeFlags);

        sharedContext = std::make_unique<ContextType>(nullptr, sharedSettings, 1, 1);
        sharedContext->initialize(sharedSettings);

        // Reload our extensions vector
        loadExtensions();
    }

    std::unique_ptr<GlContext> context;

    // We don't use acquireTransientContext here since we have
    // to ensure we have exclusive access to the shared context
    // in order to make sure it is not active during context creation
    {
        sharedContext->setActive(true);

        // Create the context
        context = std::make_unique<ContextType>(sharedContext.get(), settings, width, height);

        sharedContext->setActive(false);
    }

    context->initialize(settings);
    context->checkSettings(settings);

    return context;
}


////////////////////////////////////////////////////////////
bool GlContext::isExtensionAvailable(const char* name)
{
    using GlContextImpl::extensions;
    return std::find(extensions.begin(), extensions.end(), name) != extensions.end();
}


////////////////////////////////////////////////////////////
GlFunctionPointer GlContext::getFunction(const char* name)
{
    std::scoped_lock lock(GlContextImpl::mutex);

    return ContextType::getFunction(name);
}


////////////////////////////////////////////////////////////
const GlContext* GlContext::getActiveContext()
{
    using GlContextImpl::currentContext;
    return currentContext;
}


////////////////////////////////////////////////////////////
Uint64 GlContext::getActiveContextId()
{
    using GlContextImpl::currentContext;
    return currentContext ? currentContext->m_id : 0;
}


////////////////////////////////////////////////////////////
GlContext::~GlContext()
{
    using GlContextImpl::currentContext;
    using GlContextImpl::sharedContext;

    // Deactivate the context before killing it, unless we're inside Cleanup()
    if (sharedContext)
    {
        if (this == currentContext)
            currentContext = nullptr;
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
    using GlContextImpl::mutex;
    using GlContextImpl::currentContext;
    using GlContextImpl::sharedContext;

    if (active)
    {
        if (this != currentContext)
        {
            std::scoped_lock lock(mutex);

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
            std::scoped_lock lock(mutex);

            // Deactivate the context
            if (makeCurrent(false))
            {
                currentContext = nullptr;
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
m_id(GlContextImpl::id++)
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
    using GlContextImpl::currentContext;
    using GlContextImpl::ContextDestroyCallbacks;
    using GlContextImpl::contextDestroyCallbacks;

    // Save the current context so we can restore it later
    GlContext* contextToRestore = currentContext;

    // If this context is already active there is no need to save it
    if (contextToRestore == this)
        contextToRestore = nullptr;

    // Make this context active so resources can be freed
    setActive(true);

    // Call the registered destruction callbacks
    for (auto& [callback, ptr] : contextDestroyCallbacks)
        callback(ptr);

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
    auto glGetIntegervFunc = reinterpret_cast<glGetIntegervFuncType>(getFunction("glGetIntegerv"));
    auto glGetErrorFunc = reinterpret_cast<glGetErrorFuncType>(getFunction("glGetError"));
    auto glGetStringFunc = reinterpret_cast<glGetStringFuncType>(getFunction("glGetString"));
    auto glEnableFunc = reinterpret_cast<glEnableFuncType>(getFunction("glEnable"));
    auto glIsEnabledFunc = reinterpret_cast<glIsEnabledFuncType>(getFunction("glIsEnabled"));

    if (!glGetIntegervFunc || !glGetErrorFunc || !glGetStringFunc || !glEnableFunc || !glIsEnabledFunc)
    {
        err() << "Could not load necessary function to initialize OpenGL context" << std::endl;
        return;
    }

    glGetIntegervFunc(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegervFunc(GL_MINOR_VERSION, &minorVersion);

    if (glGetErrorFunc() != GL_INVALID_ENUM)
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

        const char* version = reinterpret_cast<const char*>(glGetStringFunc(GL_VERSION));
        if (version)
        {
            // OpenGL ES Common Lite profile: The beginning of the returned string is "OpenGL ES-CL major.minor"
            // OpenGL ES Common profile:      The beginning of the returned string is "OpenGL ES-CM major.minor"
            // OpenGL ES Full profile:        The beginning of the returned string is "OpenGL ES major.minor"
            // Desktop OpenGL:                The beginning of the returned string is "major.minor"

            using GlContextImpl::parseVersionString;

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
        glGetIntegervFunc(GL_CONTEXT_FLAGS, &flags);

        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
            m_settings.attributeFlags |= ContextSettings::Debug;

        if ((m_settings.majorVersion == 3) && (m_settings.minorVersion == 1))
        {
            m_settings.attributeFlags |= ContextSettings::Core;

            auto glGetStringiFunc = reinterpret_cast<glGetStringiFuncType>(getFunction("glGetStringi"));

            if (glGetStringiFunc)
            {
                int numExtensions = 0;
                glGetIntegervFunc(GL_NUM_EXTENSIONS, &numExtensions);

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
            glGetIntegervFunc(GL_CONTEXT_PROFILE_MASK, &profile);

            if (profile & GL_CONTEXT_CORE_PROFILE_BIT)
                m_settings.attributeFlags |= ContextSettings::Core;
        }
    }

    // Enable anti-aliasing if requested by the user and supported
    if ((requestedSettings.antialiasingLevel > 0) && (m_settings.antialiasingLevel > 0))
    {
        glEnableFunc(GL_MULTISAMPLE);
    }
    else
    {
        m_settings.antialiasingLevel = 0;
    }

    // Enable sRGB if requested by the user and supported
    if (requestedSettings.sRgbCapable && m_settings.sRgbCapable)
    {
        glEnableFunc(GL_FRAMEBUFFER_SRGB);

        // Check to see if the enable was successful
        if (glIsEnabledFunc(GL_FRAMEBUFFER_SRGB) == GL_FALSE)
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

    auto glGetStringFunc = reinterpret_cast<glGetStringFuncType>(getFunction("glGetString"));

    if (!glGetStringFunc)
    {
        err() << "Could not load glGetString function" << std::endl;

        return;
    }

    // Detect any known non-accelerated implementations and warn
    const char* vendorName = reinterpret_cast<const char*>(glGetStringFunc(GL_VENDOR));
    const char* rendererName = reinterpret_cast<const char*>(glGetStringFunc(GL_RENDERER));

    if (vendorName && rendererName)
    {
        if ((std::strcmp(vendorName, "Microsoft Corporation") == 0) && (std::strcmp(rendererName, "GDI Generic") == 0))
        {
            err() << "Warning: Detected \"Microsoft Corporation GDI Generic\" OpenGL implementation" << std::endl
                  << "The current OpenGL implementation is not hardware-accelerated" << std::endl;
        }
    }

    int version = static_cast<int>(m_settings.majorVersion * 10u + m_settings.minorVersion);
    int requestedVersion = static_cast<int>(requestedSettings.majorVersion * 10u + requestedSettings.minorVersion);

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
