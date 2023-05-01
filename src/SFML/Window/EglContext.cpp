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

#include <memory>
#include <mutex>
#include <ostream>
#ifdef SFML_SYSTEM_ANDROID
#include <SFML/System/Android/Activity.hpp>
#endif
#if defined(SFML_SYSTEM_LINUX) && !defined(SFML_USE_DRM)
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
    std::lock_guard           lock(states.mutex);

    return states.display;

#endif

    static EGLDisplay display = EGL_NO_DISPLAY;

    if (display == EGL_NO_DISPLAY)
    {
        eglCheck(display = eglGetDisplay(EGL_DEFAULT_DISPLAY));
        eglCheck(eglInitialize(display, nullptr, nullptr));
    }

    return display;
}


////////////////////////////////////////////////////////////
void ensureInit()
{
    static std::once_flag flag;

    std::call_once(flag,
                   []()
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

    // Note: The EGL specs say that attrib_list can be a null pointer when passed to eglCreatePbufferSurface,
    // but this is resulting in a segfault. Bug in Android?
    EGLint attrib_list[] = {EGL_WIDTH, 1, EGL_HEIGHT, 1, EGL_NONE};

    eglCheck(m_surface = eglCreatePbufferSurface(m_display, m_config, attrib_list));

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
    ActivityStates& states = getActivity();
    std::lock_guard lock(states.mutex);

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
    createSurface(owner.getSystemHandle());

#endif
}


////////////////////////////////////////////////////////////
EglContext::EglContext(EglContext* /*shared*/, const ContextSettings& /*settings*/, const Vector2u& /*size*/)
{
    EglContextImpl::ensureInit();

    sf::err() << "Warning: context has not been initialized. The constructor EglContext(shared, settings, size) is "
                 "currently not implemented."
              << std::endl;
}


////////////////////////////////////////////////////////////
EglContext::~EglContext()
{
    // Notify unshared OpenGL resources of context destruction
    cleanupUnsharedResources();

    // Deactivate the current context
    EGLContext currentContext = EGL_NO_CONTEXT;
    eglCheck(currentContext = eglGetCurrentContext());

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

    EGLBoolean result = EGL_FALSE;

    if (current)
    {
        eglCheck(result = eglMakeCurrent(m_display, m_surface, m_surface, m_context));
    }
    else
    {
        eglCheck(result = eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));
    }

    return (result != EGL_FALSE);
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
    eglCheck(eglSwapInterval(m_display, enabled ? 1 : 0));
}


////////////////////////////////////////////////////////////
void EglContext::createContext(EglContext* shared)
{
    const EGLint contextVersion[] = {EGL_CONTEXT_CLIENT_VERSION, 1, EGL_NONE};

    EGLContext toShared;

    if (shared)
        toShared = shared->m_context;
    else
        toShared = EGL_NO_CONTEXT;

    if (toShared != EGL_NO_CONTEXT)
        eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    // Create EGL context
    eglCheck(m_context = eglCreateContext(m_display, m_config, toShared, contextVersion));
}


////////////////////////////////////////////////////////////
void EglContext::createSurface(EGLNativeWindowType window)
{
    eglCheck(m_surface = eglCreateWindowSurface(m_display, m_config, window, nullptr));
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
    EGLint configCount;
    eglCheck(eglGetConfigs(display, nullptr, 0, &configCount));

    // Retrieve the list of available configs
    auto configs = std::make_unique<EGLConfig[]>(static_cast<std::size_t>(configCount));

    eglCheck(eglGetConfigs(display, configs.get(), configCount, &configCount));

    // Evaluate all the returned configs, and pick the best one
    int       bestScore = 0x7FFFFFFF;
    EGLConfig bestConfig{};

    for (std::size_t i = 0; i < static_cast<std::size_t>(configCount); ++i)
    {
        // Check mandatory attributes
        int surfaceType;
        int renderableType;
        eglCheck(eglGetConfigAttrib(display, configs[i], EGL_SURFACE_TYPE, &surfaceType));
        eglCheck(eglGetConfigAttrib(display, configs[i], EGL_RENDERABLE_TYPE, &renderableType));
        if (!(surfaceType & (EGL_WINDOW_BIT | EGL_PBUFFER_BIT)) || !(renderableType & EGL_OPENGL_ES_BIT))
            continue;

        // Extract the components of the current config
        int red;
        int green;
        int blue;
        int alpha;
        int depth;
        int stencil;
        int multiSampling;
        int samples;
        int caveat;
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
        int color = red + green + blue + alpha;
        int score = evaluateFormat(bitsPerPixel, settings, color, depth, stencil, multiSampling ? samples : 0, caveat == EGL_NONE, false);

        // If it's better than the current best, make it the new best
        if (score < bestScore)
        {
            bestScore  = score;
            bestConfig = configs[i];
        }
    }

    assert(bestScore < 0x7FFFFFFF);

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
    m_settings.antialiasingLevel = 0;

    EGLBoolean result = EGL_FALSE;
    EGLint     tmp    = 0;

    // Update the internal context settings with the current config
    eglCheck(result = eglGetConfigAttrib(m_display, m_config, EGL_DEPTH_SIZE, &tmp));

    if (result != EGL_FALSE)
        m_settings.depthBits = static_cast<unsigned int>(tmp);

    eglCheck(result = eglGetConfigAttrib(m_display, m_config, EGL_STENCIL_SIZE, &tmp));

    if (result != EGL_FALSE)
        m_settings.stencilBits = static_cast<unsigned int>(tmp);

    eglCheck(result = eglGetConfigAttrib(m_display, m_config, EGL_SAMPLE_BUFFERS, &tmp));

    if ((result != EGL_FALSE) && tmp)
    {
        eglCheck(result = eglGetConfigAttrib(m_display, m_config, EGL_SAMPLES, &tmp));

        if (result != EGL_FALSE)
            m_settings.antialiasingLevel = static_cast<unsigned int>(tmp);
    }
}


#if defined(SFML_SYSTEM_LINUX) && !defined(SFML_USE_DRM)
////////////////////////////////////////////////////////////
XVisualInfo EglContext::selectBestVisual(::Display* XDisplay, unsigned int bitsPerPixel, const ContextSettings& settings)
{
    EglContextImpl::ensureInit();

    // Get the initialized EGL display
    EGLDisplay display = EglContextImpl::getInitializedDisplay();

    // Get the best EGL config matching the default video settings
    EGLConfig config = getBestConfig(display, bitsPerPixel, settings);

    // Retrieve the visual id associated with this EGL config
    EGLint nativeVisualId;

    eglCheck(eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &nativeVisualId));

    if (nativeVisualId == 0)
    {
        // Should never happen...
        err() << "No EGL visual found. You should check your graphics driver" << std::endl;

        return XVisualInfo();
    }

    XVisualInfo vTemplate;
    vTemplate.visualid = static_cast<VisualID>(nativeVisualId);

    // Get X11 visuals compatible with this EGL config
    XVisualInfo *availableVisuals, bestVisual;
    int          visualCount = 0;

    availableVisuals = XGetVisualInfo(XDisplay, VisualIDMask, &vTemplate, &visualCount);

    if (visualCount == 0)
    {
        // Can't happen...
        err() << "No X11 visual found. Bug in your EGL implementation ?" << std::endl;

        return XVisualInfo();
    }

    // Pick up the best one
    bestVisual = availableVisuals[0];
    XFree(availableVisuals);

    return bestVisual;
}
#endif

} // namespace sf::priv
