////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2013 Jonathan De Wachter (dewachter.jonathan@gmail.com)
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
#include <SFML/Window/EglContext.hpp>
#include <SFML/Window/WindowImpl.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Sleep.hpp>

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <mutex>
#include <ostream>
#ifdef SFML_SYSTEM_ANDROID
#include <SFML/System/Android/Activity.hpp>
#endif
#if defined(SFML_SYSTEM_LINUX) && !defined(SFML_USE_DRM)
#include <SFML/Window/Unix/Utils.hpp>

#include <X11/Xlib.h>
#endif

// We check for this definition in order to avoid multiple definitions of GLAD
// entities during unity builds of SFML.
#ifndef SF_GLAD_EGL_IMPLEMENTATION_INCLUDED
#define SF_GLAD_EGL_IMPLEMENTATION_INCLUDED
#define SF_GLAD_EGL_IMPLEMENTATION
#include <glad/egl.h>
#undef SF_GLAD_EGL_IMPLEMENTATION
#endif

#include <SFML/Window/EglFunctionLoader.hpp>

namespace
{
// A nested named namespace is used here to allow unity builds of SFML.
namespace EglContextImpl
{
#ifdef SFML_SYSTEM_ANDROID
template <typename T>
T loadEglBootstrapSymbol(void* handle, const char* name)
{
    return reinterpret_cast<T>(reinterpret_cast<std::uintptr_t>(dlsym(handle, name)));
}

bool loadEglBootstrap()
{
    static void* const handle = dlopen("libEGL.so", RTLD_LAZY | RTLD_LOCAL);

    if (!handle)
        return false;

    eglGetDisplay = loadEglBootstrapSymbol<PFNEGLGETDISPLAYPROC>(handle, "eglGetDisplay");
    eglInitialize = loadEglBootstrapSymbol<PFNEGLINITIALIZEPROC>(handle, "eglInitialize");
    eglGetError   = loadEglBootstrapSymbol<PFNEGLGETERRORPROC>(handle, "eglGetError");
    return eglGetDisplay && eglInitialize && eglGetError;
}
#endif

EGLDisplay getInitializedDisplay()
{
    static EGLDisplay display = EGL_NO_DISPLAY;

    if (display == EGL_NO_DISPLAY)
    {
        display = eglCheck(eglGetDisplay(EGL_DEFAULT_DISPLAY));
#ifdef SFML_SYSTEM_ANDROID
        if (display == EGL_NO_DISPLAY || eglCheck(eglInitialize(display, nullptr, nullptr)) == EGL_FALSE)
            display = EGL_NO_DISPLAY;
#else
        eglCheck(eglInitialize(display, nullptr, nullptr));
#endif
    }

    return display;
}


////////////////////////////////////////////////////////////
void ensureInit()
{
    static std::once_flag flag;

    std::call_once(flag,
                   []
                   {
#ifdef SFML_SYSTEM_ANDROID
                       if (!loadEglBootstrap())
#else
                       if (!gladLoaderLoadEGL(EGL_NO_DISPLAY))
#endif
                       {
                           // At this point, the failure is unrecoverable
                           // Dump a message to the console and let the application terminate
                           sf::err() << "Failed to load EGL entry points" << std::endl;

                           assert(false);

                           return false;
                       }

                       // Continue loading with a display
#ifdef SFML_SYSTEM_ANDROID
                       const EGLDisplay display = getInitializedDisplay();
                       if (display == EGL_NO_DISPLAY || !gladLoaderLoadEGL(display))
                       {
                           sf::err() << "Failed to initialize EGL or load display entry points" << std::endl;

                           assert(false);

                           return false;
                       }
#else
                       gladLoaderLoadEGL(getInitializedDisplay());
#endif

                       return true;
                   });
}
} // namespace EglContextImpl
} // namespace


namespace sf::priv
{
////////////////////////////////////////////////////////////
EglContext::EglContext(EglContext* shared)
{
    EglContextImpl::ensureInit();

    // Get the initialized EGL display
    m_display = EglContextImpl::getInitializedDisplay();

    const ContextSettings settings = shared ? shared->m_settings : ContextSettings{};

    // Get the best EGL config matching the default video settings
    m_config = getBestConfig(m_display, VideoMode::getDesktopMode().bitsPerPixel, settings, EGL_PBUFFER_BIT);
    if (!m_config)
    {
        err() << "Failed to find an EGL pbuffer configuration supporting OpenGL ES " << settings.majorVersion << "."
              << settings.minorVersion << std::endl;
        return;
    }
    updateSettings();

    // Create EGL context
    createContext(shared, settings, VideoMode::getDesktopMode().bitsPerPixel, EGL_PBUFFER_BIT);

    // Note: The EGL specs say that attribList can be a null pointer when passed to eglCreatePbufferSurface,
    // but this is resulting in a segfault. Bug in Android?
    static constexpr std::array attribList = {EGL_WIDTH, 1, EGL_HEIGHT, 1, EGL_NONE};

    m_surface = eglCheck(eglCreatePbufferSurface(m_display, m_config, attribList.data()));
}


////////////////////////////////////////////////////////////
EglContext::EglContext(EglContext*                        shared,
                       const ContextSettings&             settings,
                       [[maybe_unused]] const WindowImpl& owner,
                       unsigned int                       bitsPerPixel)
{
    EglContextImpl::ensureInit();

#ifdef SFML_SYSTEM_ANDROID

    // On Android, we must save the created context
    ActivityStates&       states = getActivity();
    const std::lock_guard lock(states.mutex);

    states.context = this;

#endif

    // Get the initialized EGL display
    m_display = EglContextImpl::getInitializedDisplay();

    // Get the best EGL config matching the requested video settings
    ContextSettings effectiveSettings = settings;
    m_config = getBestConfig(m_display, bitsPerPixel, effectiveSettings, EGL_WINDOW_BIT);
    if (!m_config && !shared && (settings.majorVersion >= 3))
    {
        err() << "Warning: No EGL configuration supports requested OpenGL ES " << settings.majorVersion << "."
              << settings.minorVersion << "; falling back to OpenGL ES 2.0" << std::endl;
        effectiveSettings.majorVersion = 2;
        effectiveSettings.minorVersion = 0;
        m_config = getBestConfig(m_display, bitsPerPixel, effectiveSettings, EGL_WINDOW_BIT);
    }

    if (!m_config)
    {
        if (shared)
            err() << "Failed to find an EGL window configuration matching the locked OpenGL ES share-group version"
                  << std::endl;
        else
            err() << "Failed to find an EGL configuration supporting OpenGL ES 2" << std::endl;
        return;
    }

    updateSettings();

    // Create EGL context
    createContext(shared, effectiveSettings, bitsPerPixel, EGL_WINDOW_BIT);

    // Create EGL surface
    createSurface(static_cast<EGLNativeWindowType>(owner.getNativeHandle()));
}


////////////////////////////////////////////////////////////
EglContext::EglContext(EglContext* shared, const ContextSettings& settings, Vector2u size)
{
    EglContextImpl::ensureInit();

    m_display = EglContextImpl::getInitializedDisplay();
    m_config = getBestConfig(m_display, VideoMode::getDesktopMode().bitsPerPixel, settings, EGL_PBUFFER_BIT);

    ContextSettings effectiveSettings = settings;
    if (!m_config && !shared && (settings.majorVersion >= 3))
    {
        err() << "Warning: No EGL pbuffer configuration supports requested OpenGL ES " << settings.majorVersion << "."
              << settings.minorVersion << "; falling back to OpenGL ES 2.0" << std::endl;
        effectiveSettings.majorVersion = 2;
        effectiveSettings.minorVersion = 0;
        m_config =
            getBestConfig(m_display, VideoMode::getDesktopMode().bitsPerPixel, effectiveSettings, EGL_PBUFFER_BIT);
    }

    if (!m_config)
    {
        if (shared)
            err() << "Failed to find an EGL pbuffer configuration matching the locked OpenGL ES share-group version"
                  << std::endl;
        else
            err() << "Failed to find an EGL configuration supporting OpenGL ES 2" << std::endl;
        return;
    }

    updateSettings();

    createContext(shared, effectiveSettings, VideoMode::getDesktopMode().bitsPerPixel, EGL_PBUFFER_BIT);

    const std::array attribList = {EGL_WIDTH,
                                   static_cast<EGLint>(std::max(size.x, 1u)),
                                   EGL_HEIGHT,
                                   static_cast<EGLint>(std::max(size.y, 1u)),
                                   EGL_NONE};
    m_surface = eglCheck(eglCreatePbufferSurface(m_display, m_config, attribList.data()));
}


////////////////////////////////////////////////////////////
EglContext::~EglContext()
{
    // Notify unshared OpenGL resources of context destruction
    cleanupUnsharedResources();

    // Deactivate the current context
    const EGLContext currentContext = eglCheck(eglGetCurrentContext());

    if (currentContext == m_context)
    {
        eglCheck(eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));
    }

    // Destroy context
    if (m_context != EGL_NO_CONTEXT)
    {
        eglCheck(eglDestroyContext(m_display, m_context));
    }

    // Destroy surface
    if (m_surface != EGL_NO_SURFACE)
    {
        eglCheck(eglDestroySurface(m_display, m_surface));
    }
}


////////////////////////////////////////////////////////////
GlFunctionPointer EglContext::getFunction(const char* name)
{
    EglContextImpl::ensureInit();

    return getEglGlFunction(name);
}


////////////////////////////////////////////////////////////
bool EglContext::makeCurrent(bool current)
{
    if (m_surface == EGL_NO_SURFACE)
        return false;

    if (current)
        return EGL_FALSE != eglCheck(eglMakeCurrent(m_display, m_surface, m_surface, m_context));

    return EGL_FALSE != eglCheck(eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));
}


////////////////////////////////////////////////////////////
void EglContext::display()
{
    if (m_surface != EGL_NO_SURFACE)
        eglCheck(eglSwapBuffers(m_display, m_surface));
}


////////////////////////////////////////////////////////////
void EglContext::setVerticalSyncEnabled(bool enabled)
{
    eglCheck(eglSwapInterval(m_display, enabled));
}


////////////////////////////////////////////////////////////
void EglContext::createContext(EglContext*           shared,
                               const ContextSettings& settings,
                               unsigned int           bitsPerPixel,
                               EGLint                 surfaceType)
{
    const EGLContext toShared = shared ? shared->m_context : EGL_NO_CONTEXT;
    if (toShared != EGL_NO_CONTEXT)
        eglCheck(eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));

    const auto create = [&](const ContextSettings& requested)
    {
        if ((requested.majorVersion >= 3) && (SF_GLAD_EGL_VERSION_1_5 || SF_GLAD_EGL_KHR_create_context))
        {
            const std::array contextAttributes = {EGL_CONTEXT_MAJOR_VERSION,
                                                  static_cast<EGLint>(requested.majorVersion),
                                                  EGL_CONTEXT_MINOR_VERSION,
                                                  static_cast<EGLint>(requested.minorVersion),
                                                  EGL_NONE};
            return eglCreateContext(m_display, m_config, toShared, contextAttributes.data());
        }

        const std::array contextAttributes = {
            EGL_CONTEXT_CLIENT_VERSION, static_cast<EGLint>(requested.majorVersion >= 3 ? 3 : 2), EGL_NONE};
        return eglCreateContext(m_display, m_config, toShared, contextAttributes.data());
    };

    m_context = create(settings);

    if ((m_context == EGL_NO_CONTEXT) && (settings.majorVersion >= 3) && (toShared == EGL_NO_CONTEXT))
    {
        err() << "Warning: Failed to create requested OpenGL ES " << settings.majorVersion << "."
              << settings.minorVersion << " context; falling back to OpenGL ES 2.0" << std::endl;

        ContextSettings fallbackSettings = settings;
        fallbackSettings.majorVersion    = 2;
        fallbackSettings.minorVersion    = 0;

        m_config = getBestConfig(m_display, bitsPerPixel, fallbackSettings, surfaceType);
        if (m_config)
        {
            updateSettings();
            m_context = create(fallbackSettings);
        }
    }

    if (m_context == EGL_NO_CONTEXT)
        err() << "Failed to create an OpenGL ES context" << std::endl;
}


////////////////////////////////////////////////////////////
void EglContext::createSurface(EGLNativeWindowType window)
{
    m_surface = eglCheck(eglCreateWindowSurface(m_display, m_config, window, nullptr));
}


////////////////////////////////////////////////////////////
void EglContext::destroySurface()
{
    // Ensure that this context is no longer active since our surface is going to be destroyed
    setActive(false);

    eglCheck(eglDestroySurface(m_display, m_surface));
    m_surface = EGL_NO_SURFACE;
}


////////////////////////////////////////////////////////////
EGLConfig EglContext::getBestConfig(EGLDisplay            display,
                                    unsigned int          bitsPerPixel,
                                    const ContextSettings& settings,
                                    EGLint                requestedSurfaceType)
{
    EglContextImpl::ensureInit();

    // Determine the number of available configs
    EGLint configCount = 0;
    eglCheck(eglGetConfigs(display, nullptr, 0, &configCount));

    // Retrieve the list of available configs
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
    const auto configs = std::make_unique<EGLConfig[]>(static_cast<std::size_t>(configCount));

    eglCheck(eglGetConfigs(display, configs.get(), configCount, &configCount));

    // Evaluate all the returned configs, and pick the best one
    int       bestScore = 0x7FFFFFFF;
    EGLConfig bestConfig{};

    for (std::size_t i = 0; i < static_cast<std::size_t>(configCount); ++i)
    {
        // Check mandatory attributes
        int surfaceType    = 0;
        int renderableType = 0;
        eglCheck(eglGetConfigAttrib(display, configs[i], EGL_SURFACE_TYPE, &surfaceType));
        eglCheck(eglGetConfigAttrib(display, configs[i], EGL_RENDERABLE_TYPE, &renderableType));
        const int requiredRenderableType =
            settings.majorVersion >= 3 ? EGL_OPENGL_ES3_BIT_KHR : EGL_OPENGL_ES2_BIT;
        if (!(surfaceType & requestedSurfaceType) || !(renderableType & requiredRenderableType))
            continue;

        // Extract the components of the current config
        int red           = 0;
        int green         = 0;
        int blue          = 0;
        int alpha         = 0;
        int depth         = 0;
        int stencil       = 0;
        int multiSampling = 0;
        int samples       = 0;
        int caveat        = 0;
        eglCheck(eglGetConfigAttrib(display, configs[i], EGL_RED_SIZE, &red));
        eglCheck(eglGetConfigAttrib(display, configs[i], EGL_GREEN_SIZE, &green));
        eglCheck(eglGetConfigAttrib(display, configs[i], EGL_BLUE_SIZE, &blue));
        eglCheck(eglGetConfigAttrib(display, configs[i], EGL_ALPHA_SIZE, &alpha));
        eglCheck(eglGetConfigAttrib(display, configs[i], EGL_DEPTH_SIZE, &depth));
        eglCheck(eglGetConfigAttrib(display, configs[i], EGL_STENCIL_SIZE, &stencil));
        eglCheck(eglGetConfigAttrib(display, configs[i], EGL_SAMPLE_BUFFERS, &multiSampling));
        eglCheck(eglGetConfigAttrib(display, configs[i], EGL_SAMPLES, &samples));
        eglCheck(eglGetConfigAttrib(display, configs[i], EGL_CONFIG_CAVEAT, &caveat));

        // Evaluate the config
        const int color = red + green + blue + alpha;
        const int score = evaluateFormat(bitsPerPixel,
                                         settings,
                                         color,
                                         depth,
                                         stencil,
                                         multiSampling ? samples : 0,
                                         caveat == EGL_NONE,
                                         false);

        // If it's better than the current best, make it the new best
        if (score < bestScore)
        {
            bestScore  = score;
            bestConfig = configs[i];
        }
    }

    return bestConfig;
}


////////////////////////////////////////////////////////////
void EglContext::updateSettings()
{
    m_settings.majorVersion      = 2;
    m_settings.minorVersion      = 0;
    m_settings.attributeFlags    = ContextSettings::Default;
    m_settings.depthBits         = 0;
    m_settings.stencilBits       = 0;
    m_settings.antiAliasingLevel = 0;

    EGLint tmp = 0;

    // Update the internal context settings with the current config
    if (eglCheck(eglGetConfigAttrib(m_display, m_config, EGL_DEPTH_SIZE, &tmp)) != EGL_FALSE)
        m_settings.depthBits = static_cast<unsigned int>(tmp);

    if (eglCheck(eglGetConfigAttrib(m_display, m_config, EGL_STENCIL_SIZE, &tmp)) != EGL_FALSE)
        m_settings.stencilBits = static_cast<unsigned int>(tmp);

    if (eglCheck(eglGetConfigAttrib(m_display, m_config, EGL_SAMPLE_BUFFERS, &tmp)) != EGL_FALSE && tmp &&
        eglCheck(eglGetConfigAttrib(m_display, m_config, EGL_SAMPLES, &tmp)) != EGL_FALSE)
        m_settings.antiAliasingLevel = static_cast<unsigned int>(tmp);
}


#if defined(SFML_SYSTEM_LINUX) && !defined(SFML_USE_DRM)
////////////////////////////////////////////////////////////
XVisualInfo EglContext::selectBestVisual(::Display* xDisplay, unsigned int bitsPerPixel, const ContextSettings& settings)
{
    EglContextImpl::ensureInit();

    // Get the initialized EGL display
    EGLDisplay display = EglContextImpl::getInitializedDisplay();

    // Get the best EGL config matching the default video settings
    EGLConfig config = getBestConfig(display, bitsPerPixel, settings, EGL_WINDOW_BIT);
    if (!config && (settings.majorVersion >= 3))
    {
        ContextSettings fallbackSettings = settings;
        fallbackSettings.majorVersion    = 2;
        fallbackSettings.minorVersion    = 0;
        config = getBestConfig(display, bitsPerPixel, fallbackSettings, EGL_WINDOW_BIT);
    }

    if (!config)
    {
        err() << "No EGL window configuration supporting OpenGL ES 2 is available" << std::endl;
        return {};
    }

    // Retrieve the visual id associated with this EGL config
    EGLint nativeVisualId = 0;

    eglCheck(eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &nativeVisualId));

    if (nativeVisualId == 0)
    {
        // Should never happen...
        err() << "No EGL visual found. You should check your graphics driver" << std::endl;

        return {};
    }

    XVisualInfo vTemplate;
    vTemplate.visualid = static_cast<VisualID>(nativeVisualId);

    // Get X11 visuals compatible with this EGL config
    int visualCount = 0;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
    const auto availableVisuals = X11Ptr<XVisualInfo[]>(XGetVisualInfo(xDisplay, VisualIDMask, &vTemplate, &visualCount));

    if (visualCount == 0)
    {
        // Can't happen...
        err() << "No X11 visual found. Bug in your EGL implementation ?" << std::endl;

        return {};
    }

    // Pick up the best one
    return availableVisuals[0];
}
#endif

} // namespace sf::priv
