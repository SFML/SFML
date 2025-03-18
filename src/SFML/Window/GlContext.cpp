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
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/GlContext.hpp>

#include <SFML/System/Err.hpp>

#include <glad/gl.h>

#include <algorithm>
#include <atomic>
#include <iomanip>
#include <memory>
#include <mutex>
#include <optional>
#include <ostream>
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

#include <SFML/Window/EglContext.hpp>
using ContextType = sf::priv::EglContext;

#else

#include <SFML/Window/Win32/WglContext.hpp>
using ContextType = sf::priv::WglContext;

#endif

#elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD) || defined(SFML_SYSTEM_OPENBSD) || \
    defined(SFML_SYSTEM_NETBSD)

#if defined(SFML_USE_DRM)

#include <SFML/Window/DRM/DRMContext.hpp>
using ContextType = sf::priv::DRMContext;

#elif defined(SFML_OPENGL_ES)

#include <SFML/Window/EglContext.hpp>
using ContextType = sf::priv::EglContext;

#else

#include <SFML/Window/Unix/GlxContext.hpp>
using ContextType = sf::priv::GlxContext;

#endif

#elif defined(SFML_SYSTEM_MACOS)

#include <SFML/Window/macOS/SFContext.hpp>
using ContextType = sf::priv::SFContext;

#elif defined(SFML_SYSTEM_IOS)

#include <SFML/Window/iOS/EaglContext.hpp>
using ContextType = sf::priv::EaglContext;

#elif defined(SFML_SYSTEM_ANDROID)

#include <SFML/Window/EglContext.hpp>
using ContextType = sf::priv::EglContext;

#endif

#if defined(SFML_SYSTEM_WINDOWS)

using glEnableFuncType      = void(APIENTRY*)(GLenum);
using glGetErrorFuncType    = GLenum(APIENTRY*)();
using glGetIntegervFuncType = void(APIENTRY*)(GLenum, GLint*);
using glGetStringFuncType   = const GLubyte*(APIENTRY*)(GLenum);
using glGetStringiFuncType  = const GLubyte*(APIENTRY*)(GLenum, GLuint);
using glIsEnabledFuncType   = GLboolean(APIENTRY*)(GLenum);

#else

using glEnableFuncType      = void (*)(GLenum);
using glGetErrorFuncType    = GLenum (*)();
using glGetIntegervFuncType = void (*)(GLenum, GLint*);
using glGetStringFuncType   = const GLubyte* (*)(GLenum);
using glGetStringiFuncType  = const GLubyte* (*)(GLenum, GLuint);
using glIsEnabledFuncType   = GLboolean (*)(GLenum);

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
// This structure contains all the state necessary to
// track current context information for each thread
struct CurrentContext
{
    std::uint64_t        id{};
    sf::priv::GlContext* ptr{};
    unsigned int         transientCount{};

    // This per-thread variable holds the current context information for each thread
    static CurrentContext& get()
    {
        thread_local CurrentContext currentContext;
        return currentContext;
    }

private:
    // Private constructor to prevent CurrentContext from being constructed outside of get()
    CurrentContext() = default;
};
} // namespace GlContextImpl
} // namespace


namespace sf::priv
{
// This structure contains all the state necessary to
// track SharedContext usage
struct GlContext::SharedContext
{
    ////////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    ////////////////////////////////////////////////////////////
    SharedContext()
    {
        const std::lock_guard lock(mutex);

        context.emplace(nullptr);
        context->initialize(ContextSettings{});

        loadExtensions();

        context->setActive(false);
    }

    ////////////////////////////////////////////////////////////
    /// \brief Get weak_ptr
    ///
    /// \return weak_ptr to the SharedContext
    ///
    ////////////////////////////////////////////////////////////
    static std::weak_ptr<SharedContext>& getWeakPtr()
    {
        static std::weak_ptr<SharedContext> weakSharedContext;
        return weakSharedContext;
    }

    ////////////////////////////////////////////////////////////
    /// \brief Get shared_ptr to the shared context
    ///
    /// Create new object if one doesn't already exist.
    ///
    /// \return shared_ptr to the shared context
    ///
    ////////////////////////////////////////////////////////////
    static std::shared_ptr<SharedContext> get()
    {
        auto& weakSharedContext = getWeakPtr();
        auto  sharedContext     = weakSharedContext.lock();

        if (!sharedContext)
        {
            sharedContext     = std::make_shared<GlContext::SharedContext>();
            weakSharedContext = sharedContext;
        }

        return sharedContext;
    }

    ////////////////////////////////////////////////////////////
    /// \brief Load our extensions vector with the supported extensions
    ///
    ////////////////////////////////////////////////////////////
    void loadExtensions()
    {
        auto glGetErrorFunc    = reinterpret_cast<glGetErrorFuncType>(getFunction("glGetError"));
        auto glGetIntegervFunc = reinterpret_cast<glGetIntegervFuncType>(getFunction("glGetIntegerv"));
        auto glGetStringFunc   = reinterpret_cast<glGetStringFuncType>(getFunction("glGetString"));

        if (!glGetErrorFunc || !glGetIntegervFunc || !glGetStringFunc)
            return;

        // Check whether a >= 3.0 context is available
        int majorVersion = 0;
        glGetIntegervFunc(GL_MAJOR_VERSION, &majorVersion);

        auto glGetStringiFunc = reinterpret_cast<glGetStringiFuncType>(getFunction("glGetStringi"));

        if (glGetErrorFunc() == GL_INVALID_ENUM || !majorVersion || !glGetStringiFunc)
        {
            // Try to load the < 3.0 way
            if (const char* extensionString = reinterpret_cast<const char*>(glGetStringFunc(GL_EXTENSIONS)))
            {
                extensions.clear();

                do
                {
                    const char* extension = extensionString;

                    while (*extensionString && (*extensionString != ' '))
                        ++extensionString;

                    extensions.emplace_back(extension, extensionString);
                } while (*extensionString++);
            }
        }
        else
        {
            // Try to load the >= 3.0 way
            int numExtensions = 0;
            glGetIntegervFunc(GL_NUM_EXTENSIONS, &numExtensions);

            if (numExtensions)
            {
                extensions.clear();

                for (unsigned int i = 0; i < static_cast<unsigned int>(numExtensions); ++i)
                    if (const char* extensionString = reinterpret_cast<const char*>(glGetStringiFunc(GL_EXTENSIONS, i)))
                        extensions.emplace_back(extensionString);
            }
        }
    }

    // AMD drivers have issues with internal synchronization
    // We need to make sure that no operating system context
    // or pixel format operations are performed simultaneously
    // This mutex is also used to protect the shared context
    // from being locked on multiple threads
    std::recursive_mutex mutex;

    // Supported OpenGL extensions
    std::vector<std::string> extensions;

    // The hidden, inactive context that will be shared with all other contexts
    std::optional<ContextType> context;
};


// This structure contains all the state necessary to
// track TransientContext usage
struct GlContext::TransientContext
{
    ////////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    ////////////////////////////////////////////////////////////
    TransientContext()
    {
        // TransientContext should never be created if there is
        // already a context active on the current thread
        assert(!GlContextImpl::CurrentContext::get().id && "Another context is active on the current thread");

        // Lock ourselves so we don't create a new object if one doesn't already exist
        sharedContext = SharedContext::getWeakPtr().lock();

        if (!sharedContext)
        {
            // Create a Context object for temporary use
            context.emplace();
        }
        else
        {
            // GlResources exist, currentContextId not yet set
            assert(sharedContext && "Shared context does not exist");

            // Lock the shared context for temporary use
            sharedContextLock = std::unique_lock(sharedContext->mutex);
            sharedContext->context->setActive(true);
        }
    }

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~TransientContext()
    {
        if (sharedContextLock)
            sharedContext->context->setActive(false);
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

    ////////////////////////////////////////////////////////////
    /// \brief Get the thread local TransientContext
    ///
    /// This per-thread variable tracks if and how a transient
    /// context is currently being used on the current thread
    ///
    /// \return The thread local TransientContext
    ///
    ////////////////////////////////////////////////////////////
    static std::optional<TransientContext>& get()
    {
        static thread_local std::optional<TransientContext> transientContext;
        return transientContext;
    }

    ///////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::optional<sf::Context>             context;
    std::unique_lock<std::recursive_mutex> sharedContextLock;
    std::shared_ptr<SharedContext>         sharedContext;
};


// This structure contains all the implementation data we
// don't want to expose through the visible interface
struct GlContext::Impl
{
    ////////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    ////////////////////////////////////////////////////////////
    Impl()
    {
        auto& weakUnsharedGlObjects = getWeakUnsharedGlObjects();
        unsharedGlObjects           = weakUnsharedGlObjects.lock();

        if (!unsharedGlObjects)
        {
            unsharedGlObjects = std::make_shared<UnsharedGlObjects>();

            weakUnsharedGlObjects = unsharedGlObjects;
        }
    }

    // Structure to track which unshared object belongs to which context
    struct UnsharedGlObject
    {
        std::uint64_t         contextId{};
        std::shared_ptr<void> object;
    };

    using UnsharedGlObjects = std::vector<UnsharedGlObject>;

    ////////////////////////////////////////////////////////////
    /// \brief Get weak_ptr to unshared objects
    ///
    /// \return weak_ptr to unshared objects
    ///
    ////////////////////////////////////////////////////////////
    static std::weak_ptr<UnsharedGlObjects>& getWeakUnsharedGlObjects()
    {
        static std::weak_ptr<UnsharedGlObjects> weakUnsharedGlObjects;

        return weakUnsharedGlObjects;
    }

    ////////////////////////////////////////////////////////////
    /// \brief Get mutex protecting unshared objects
    ///
    /// \return Mutex protecting unshared objects
    ///
    ////////////////////////////////////////////////////////////
    static std::mutex& getUnsharedGlObjectsMutex()
    {
        static std::mutex mutex;
        return mutex;
    }

    ///////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::shared_ptr<UnsharedGlObjects> unsharedGlObjects; //!< The current object's handle to unshared objects
    const std::uint64_t                id{
        []
        {
            static std::atomic<std::uint64_t> atomicId(1); // start at 1, zero is "no context"
            return atomicId.fetch_add(1);
        }()}; //!< Unique identifier, used for identifying contexts when managing unshareable OpenGL resources
};


////////////////////////////////////////////////////////////
std::shared_ptr<void> GlContext::getSharedContext()
{
    return GlContext::SharedContext::get();
}


////////////////////////////////////////////////////////////
void GlContext::registerUnsharedGlObject(std::shared_ptr<void> object)
{
    if (const std::lock_guard lock(Impl::getUnsharedGlObjectsMutex());
        const auto            unsharedGlObjects = Impl::getWeakUnsharedGlObjects().lock())
        unsharedGlObjects->emplace_back(Impl::UnsharedGlObject{GlContextImpl::CurrentContext::get().id, std::move(object)});
}


////////////////////////////////////////////////////////////
void GlContext::unregisterUnsharedGlObject(std::shared_ptr<void> object)
{
    if (const std::lock_guard lock(Impl::getUnsharedGlObjectsMutex());
        const auto            unsharedGlObjects = Impl::getWeakUnsharedGlObjects().lock())
    {
        // Find the object in unshared objects and remove it if its associated context is currently active
        // This will trigger the destructor of the object since shared_ptr
        // in unshared objects should be the only one existing
        const auto iter = std::find_if(unsharedGlObjects->begin(),
                                       unsharedGlObjects->end(),
                                       [&object](const Impl::UnsharedGlObject& obj) {
                                           return (obj.object == object) &&
                                                  (obj.contextId == GlContextImpl::CurrentContext::get().id);
                                       });

        if (iter != unsharedGlObjects->end())
            unsharedGlObjects->erase(iter);
    }
}


////////////////////////////////////////////////////////////
void GlContext::acquireTransientContext()
{
    auto& currentContext = GlContextImpl::CurrentContext::get();

    // Fast path if we already have a context active on this thread
    if (currentContext.id)
    {
        ++currentContext.transientCount;
        return;
    }

    // If we don't already have a context active on this thread the count should be 0
    assert(!currentContext.transientCount && "Transient count cannot be non-zero");

    // If currentContextId is not set, this must be the first
    // TransientContextLock on this thread, construct the state object
    TransientContext::get().emplace();

    // Make sure a context is active at this point
    assert(currentContext.id && "Current context ID cannot be zero");
}


////////////////////////////////////////////////////////////
void GlContext::releaseTransientContext()
{
    auto& currentContext = GlContextImpl::CurrentContext::get();

    // Make sure a context was left active after acquireTransientContext() was called
    assert(currentContext.id && "Current context ID cannot be zero");

    // Fast path if we already had a context active on this thread before acquireTransientContext() was called
    if (currentContext.transientCount)
    {
        --currentContext.transientCount;
        return;
    }

    // If currentContextId is set and currentContextTransientCount is 0,
    // this is the last TransientContextLock that is released, destroy the state object
    TransientContext::get().reset();
}


////////////////////////////////////////////////////////////
std::unique_ptr<GlContext> GlContext::create()
{
    // Make sure that there's an active context (context creation may need extensions, and thus a valid context)
    const auto sharedContext = SharedContext::get();

    const std::lock_guard lock(sharedContext->mutex);

    std::unique_ptr<GlContext> context;

    // We don't use acquireTransientContext here since we have
    // to ensure we have exclusive access to the shared context
    // in order to make sure it is not active during context creation
    sharedContext->context->setActive(true);

    // Create the context
    context = std::make_unique<ContextType>(&sharedContext->context.value());

    sharedContext->context->setActive(false);

    context->initialize(ContextSettings{});

    return context;
}


////////////////////////////////////////////////////////////
std::unique_ptr<GlContext> GlContext::create(const ContextSettings& settings, const WindowImpl& owner, unsigned int bitsPerPixel)
{
    // Make sure that there's an active context (context creation may need extensions, and thus a valid context)
    const auto sharedContext = SharedContext::get();

    const std::lock_guard lock(sharedContext->mutex);

    // If use_count is 2 (GlResource + sharedContext) we know that we are inside sf::Context or sf::Window
    // Only in this situation we allow the user to indirectly re-create the shared context as a core context

    // Check if we need to convert our shared context into a core context
    if ((sharedContext.use_count() == 2) && (settings.attributeFlags & ContextSettings::Core) &&
        !(sharedContext->context->m_settings.attributeFlags & ContextSettings::Core))
    {
        // Re-create our shared context as a core context
        const ContextSettings sharedSettings{/* depthBits */ 0,
                                             /* stencilBits */ 0,
                                             /* antiAliasingLevel */ 0,
                                             settings.majorVersion,
                                             settings.minorVersion,
                                             settings.attributeFlags};

        sharedContext->context.emplace(nullptr, sharedSettings, Vector2u(1, 1));
        sharedContext->context->initialize(sharedSettings);

        // Reload our extensions vector
        sharedContext->loadExtensions();
    }

    std::unique_ptr<GlContext> context;

    // We don't use acquireTransientContext here since we have
    // to ensure we have exclusive access to the shared context
    // in order to make sure it is not active during context creation
    sharedContext->context->setActive(true);

    // Create the context
    context = std::make_unique<ContextType>(&sharedContext->context.value(), settings, owner, bitsPerPixel);

    sharedContext->context->setActive(false);

    context->initialize(settings);
    context->checkSettings(settings);

    return context;
}


////////////////////////////////////////////////////////////
std::unique_ptr<GlContext> GlContext::create(const ContextSettings& settings, Vector2u size)
{
    // Make sure that there's an active context (context creation may need extensions, and thus a valid context)
    const auto sharedContext = SharedContext::get();

    const std::lock_guard lock(sharedContext->mutex);

    // If use_count is 2 (GlResource + sharedContext) we know that we are inside sf::Context or sf::Window
    // Only in this situation we allow the user to indirectly re-create the shared context as a core context

    // Check if we need to convert our shared context into a core context
    if ((sharedContext.use_count() == 2) && (settings.attributeFlags & ContextSettings::Core) &&
        !(sharedContext->context->m_settings.attributeFlags & ContextSettings::Core))
    {
        // Re-create our shared context as a core context
        const ContextSettings sharedSettings{/* depthBits */ 0,
                                             /* stencilBits */ 0,
                                             /* antiAliasingLevel */ 0,
                                             settings.majorVersion,
                                             settings.minorVersion,
                                             settings.attributeFlags};

        sharedContext->context.emplace(nullptr, sharedSettings, Vector2u(1, 1));
        sharedContext->context->initialize(sharedSettings);

        // Reload our extensions vector
        sharedContext->loadExtensions();
    }

    // We don't use acquireTransientContext here since we have
    // to ensure we have exclusive access to the shared context
    // in order to make sure it is not active during context creation
    sharedContext->context->setActive(true);

    // Create the context
    auto context = std::make_unique<ContextType>(&sharedContext->context.value(), settings, size);

    sharedContext->context->setActive(false);

    context->initialize(settings);
    context->checkSettings(settings);

    return context;
}


////////////////////////////////////////////////////////////
bool GlContext::isExtensionAvailable(std::string_view name)
{
    // If this function is called before any context is available,
    // the shared context will be created for the duration of this call
    const auto sharedContext = SharedContext::get();

    return std::find(sharedContext->extensions.begin(), sharedContext->extensions.end(), name) !=
           sharedContext->extensions.end();
}


////////////////////////////////////////////////////////////
GlFunctionPointer GlContext::getFunction(const char* name)
{
    // Make sure we don't try to create the shared context here since
    // setActive can be called during construction and lead to infinite recursion
    auto* sharedContext = SharedContext::getWeakPtr().lock().get();

    // We can't and don't need to lock when we are currently creating the shared context
    std::unique_lock<std::recursive_mutex> lock;

    if (sharedContext)
        lock = std::unique_lock(sharedContext->mutex);

    return ContextType::getFunction(name);
}


////////////////////////////////////////////////////////////
const GlContext* GlContext::getActiveContext()
{
    return GlContextImpl::CurrentContext::get().ptr;
}


////////////////////////////////////////////////////////////
std::uint64_t GlContext::getActiveContextId()
{
    return GlContextImpl::CurrentContext::get().id;
}


////////////////////////////////////////////////////////////
GlContext::~GlContext()
{
    auto& currentContext = GlContextImpl::CurrentContext::get();

    if (m_impl->id == currentContext.id)
    {
        currentContext.id  = 0;
        currentContext.ptr = nullptr;
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
    auto& currentContext = GlContextImpl::CurrentContext::get();

    // Make sure we don't try to create the shared context here since
    // setActive can be called during construction and lead to infinite recursion
    auto* sharedContext = SharedContext::getWeakPtr().lock().get();

    if (active)
    {
        if (m_impl->id != currentContext.id)
        {
            // We can't and don't need to lock when we are currently creating the shared context
            std::unique_lock<std::recursive_mutex> lock;

            if (sharedContext)
                lock = std::unique_lock(sharedContext->mutex);

            // Activate the context
            if (makeCurrent(true))
            {
                // Set it as the new current context for this thread
                currentContext.id  = m_impl->id;
                currentContext.ptr = this;
                return true;
            }

            return false;
        }

        // This context is already the active one on this thread, don't do anything
        return true;
    }

    if (m_impl->id == currentContext.id)
    {
        // We can't and don't need to lock when we are currently creating the shared context
        std::unique_lock<std::recursive_mutex> lock;

        if (sharedContext)
            lock = std::unique_lock(sharedContext->mutex);

        // Deactivate the context
        if (makeCurrent(false))
        {
            currentContext.id  = 0;
            currentContext.ptr = nullptr;
            return true;
        }

        return false;
    }

    // This context is not the active one on this thread, don't do anything
    return true;
}


////////////////////////////////////////////////////////////
GlContext::GlContext() : m_impl(std::make_unique<Impl>())
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
int GlContext::evaluateFormat(
    unsigned int           bitsPerPixel,
    const ContextSettings& settings,
    int                    colorBits,
    int                    depthBits,
    int                    stencilBits,
    int                    antiAliasing,
    bool                   accelerated,
    bool                   sRgb)
{
    int colorDiff        = static_cast<int>(bitsPerPixel) - colorBits;
    int depthDiff        = static_cast<int>(settings.depthBits) - depthBits;
    int stencilDiff      = static_cast<int>(settings.stencilBits) - stencilBits;
    int antiAliasingDiff = static_cast<int>(settings.antiAliasingLevel) - antiAliasing;

    // Weight sub-scores so that better settings don't score equally as bad as worse settings
    colorDiff *= ((colorDiff > 0) ? 100000 : 1);
    depthDiff *= ((depthDiff > 0) ? 100000 : 1);
    stencilDiff *= ((stencilDiff > 0) ? 100000 : 1);
    antiAliasingDiff *= ((antiAliasingDiff > 0) ? 100000 : 1);

    // Aggregate the scores
    int score = std::abs(colorDiff) + std::abs(depthDiff) + std::abs(stencilDiff) + std::abs(antiAliasingDiff);

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
    const auto& currentContext = GlContextImpl::CurrentContext::get();

    // Save the current context so we can restore it later
    GlContext* contextToRestore = currentContext.ptr;

    // If this context is already active there is no need to save it
    if (m_impl->id == currentContext.id)
        contextToRestore = nullptr;

    // Make this context active so resources can be freed
    setActive(true);

    {
        const std::lock_guard lock(Impl::getUnsharedGlObjectsMutex());

        // Destroy the unshared objects contained in this context
        for (auto iter = m_impl->unsharedGlObjects->begin(); iter != m_impl->unsharedGlObjects->end();)
        {
            if (iter->contextId == m_impl->id)
            {
                iter = m_impl->unsharedGlObjects->erase(iter);
            }
            else
            {
                ++iter;
            }
        }
    }

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
    auto glGetErrorFunc    = reinterpret_cast<glGetErrorFuncType>(getFunction("glGetError"));
    auto glGetStringFunc   = reinterpret_cast<glGetStringFuncType>(getFunction("glGetString"));
    auto glEnableFunc      = reinterpret_cast<glEnableFuncType>(getFunction("glEnable"));
    auto glIsEnabledFunc   = reinterpret_cast<glIsEnabledFuncType>(getFunction("glIsEnabled"));

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

        if (const char* version = reinterpret_cast<const char*>(glGetStringFunc(GL_VERSION)))
        {
            // OpenGL ES Common Lite profile: The beginning of the returned string is "OpenGL ES-CL major.minor"
            // OpenGL ES Common profile:      The beginning of the returned string is "OpenGL ES-CM major.minor"
            // OpenGL ES Full profile:        The beginning of the returned string is "OpenGL ES major.minor"
            // Desktop OpenGL:                The beginning of the returned string is "major.minor"

            // Helper to parse OpenGL version strings
            static const auto parseVersionString =
                [](const char* versionString, const char* prefix, unsigned int& major, unsigned int& minor)
            {
                const std::size_t prefixLength = std::strlen(prefix);

                if ((std::strlen(versionString) >= (prefixLength + 3)) &&
                    (std::strncmp(versionString, prefix, prefixLength) == 0) && std::isdigit(versionString[prefixLength]) &&
                    (versionString[prefixLength + 1] == '.') && std::isdigit(versionString[prefixLength + 2]))
                {
                    major = static_cast<unsigned int>(versionString[prefixLength] - '0');
                    minor = static_cast<unsigned int>(versionString[prefixLength + 2] - '0');

                    return true;
                }

                return false;
            };

            if (!parseVersionString(version, "OpenGL ES-CL ", m_settings.majorVersion, m_settings.minorVersion) &&
                !parseVersionString(version, "OpenGL ES-CM ", m_settings.majorVersion, m_settings.minorVersion) &&
                !parseVersionString(version, "OpenGL ES ", m_settings.majorVersion, m_settings.minorVersion) &&
                !parseVersionString(version, "", m_settings.majorVersion, m_settings.minorVersion))
            {
                err() << "Unable to parse OpenGL version string: " << std::quoted(version) << ", defaulting to 1.1"
                      << std::endl;
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
                        m_settings.attributeFlags &= ~static_cast<std::uint32_t>(ContextSettings::Core);
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
    if ((requestedSettings.antiAliasingLevel > 0) && (m_settings.antiAliasingLevel > 0))
    {
        glEnableFunc(GL_MULTISAMPLE);
    }
    else
    {
        m_settings.antiAliasingLevel = 0;
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
void GlContext::checkSettings(const ContextSettings& requestedSettings) const
{
    // Perform checks to inform the user if they are getting a context they might not have expected
    const int version = static_cast<int>(m_settings.majorVersion * 10u + m_settings.minorVersion);
    const int requestedVersion = static_cast<int>(requestedSettings.majorVersion * 10u + requestedSettings.minorVersion);

    if ((m_settings.attributeFlags != requestedSettings.attributeFlags) || (version < requestedVersion) ||
        (m_settings.stencilBits < requestedSettings.stencilBits) ||
        (m_settings.antiAliasingLevel < requestedSettings.antiAliasingLevel) ||
        (m_settings.depthBits < requestedSettings.depthBits) || (!m_settings.sRgbCapable && requestedSettings.sRgbCapable))
    {
        err() << "Warning: The created OpenGL context does not fully meet the settings that were requested" << '\n'
              << "Requested: version = " << requestedSettings.majorVersion << "." << requestedSettings.minorVersion
              << " ; depth bits = " << requestedSettings.depthBits << " ; stencil bits = " << requestedSettings.stencilBits
              << " ; AA level = " << requestedSettings.antiAliasingLevel << std::boolalpha
              << " ; core = " << ((requestedSettings.attributeFlags & ContextSettings::Core) != 0)
              << " ; debug = " << ((requestedSettings.attributeFlags & ContextSettings::Debug) != 0)
              << " ; sRGB = " << requestedSettings.sRgbCapable << std::noboolalpha << '\n'
              << "Created: version = " << m_settings.majorVersion << "." << m_settings.minorVersion
              << " ; depth bits = " << m_settings.depthBits << " ; stencil bits = " << m_settings.stencilBits
              << " ; AA level = " << m_settings.antiAliasingLevel << std::boolalpha
              << " ; core = " << ((m_settings.attributeFlags & ContextSettings::Core) != 0)
              << " ; debug = " << ((m_settings.attributeFlags & ContextSettings::Debug) != 0)
              << " ; sRGB = " << m_settings.sRgbCapable << std::noboolalpha << std::endl;
    }
}

} // namespace sf::priv
