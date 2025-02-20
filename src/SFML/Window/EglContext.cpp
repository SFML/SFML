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

#include <array>
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
#endif

namespace
{
// A nested named namespace is used here to allow unity builds of SFML.
namespace EglContextImpl
{
EGLDisplay getInitializedDisplay()
{
#if defined(SFML_SYSTEM_ANDROID)

    // On Android, its native activity handles this for us
    sf::priv::ActivityStates& states = sf::priv::getActivity();
    const std::lock_guard     lock(states.mutex);

    return states.display;

#endif

    static EGLDisplay display = EGL_NO_DISPLAY;

    if (display == EGL_NO_DISPLAY)
    {
        display = eglCheck(eglGetDisplay(EGL_DEFAULT_DISPLAY));
        eglCheck(eglInitialize(display, nullptr, nullptr));
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
                       if (!gladLoaderLoadEGL(EGL_NO_DISPLAY))
                       {
                           // At this point, the failure is unrecoverable
                           // Dump a message to the console and let the application terminate
                           sf::err() << "Failed to load EGL entry points" << std::endl;

                           assert(false);

                           return false;
                       }

                       // Continue loading with a display
                       gladLoaderLoadEGL(getInitializedDisplay());

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

    // Get the best EGL config matching the default video settings
    m_config = getBestConfig(m_display, VideoMode::getDesktopMode().bitsPerPixel, ContextSettings());
    updateSettings();

    // Note: The EGL specs say that attribList can be a null pointer when passed to eglCreatePbufferSurface,
    // but this is resulting in a segfault. Bug in Android?
    static constexpr std::array attribList = {EGL_WIDTH, 1, EGL_HEIGHT, 1, EGL_NONE};

    m_surface = eglCheck(eglCreatePbufferSurface(m_display, m_config, attribList.data()));

    // Create EGL context
    createContext(shared);
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
    m_config = getBestConfig(m_display, bitsPerPixel, settings);
    updateSettings();

    // Create EGL context
    createContext(shared);

#if !defined(SFML_SYSTEM_ANDROID)
    // Create EGL surface (except on Android because the window is created
    // asynchronously, its activity manager will call it for us)
    createSurface(owner.getNativeHandle());

#endif
}


////////////////////////////////////////////////////////////
EglContext::EglContext(EglContext* /*shared*/, const ContextSettings& /*settings*/, Vector2u /*size*/)
{
    EglContextImpl::ensureInit();

    err() << "Warning: context has not been initialized. The constructor EglContext(shared, settings, size) is "
             "currently not implemented."
          << std::endl;
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

    return eglGetProcAddress(name);
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
void EglContext::createContext(EglContext* shared)
{
    static constexpr std::array contextVersion = {EGL_CONTEXT_CLIENT_VERSION, 1, EGL_NONE};

    const EGLContext toShared = shared ? shared->m_context : EGL_NO_CONTEXT;
    if (toShared != EGL_NO_CONTEXT)
        eglCheck(eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));

    // Create EGL context
    m_context = eglCheck(eglCreateContext(m_display, m_config, toShared, contextVersion.data()));
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
EGLConfig EglContext::getBestConfig(EGLDisplay display, unsigned int bitsPerPixel, const ContextSettings& settings)
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
        if (!(surfaceType & (EGL_WINDOW_BIT | EGL_PBUFFER_BIT)) || !(renderableType & EGL_OPENGL_ES_BIT))
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

    assert(bestScore < 0x7FFFFFFF && "Failed to calculate best config");

    return bestConfig;
}


////////////////////////////////////////////////////////////
void EglContext::updateSettings()
{
    m_settings.majorVersion      = 1;
    m_settings.minorVersion      = 1;
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
    EGLConfig config = getBestConfig(display, bitsPerPixel, settings);

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
