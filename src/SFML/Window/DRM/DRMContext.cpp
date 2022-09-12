////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2020 Andrew Mickelson
//               2013 Jonathan De Wachter (dewachter.jonathan@gmail.com)
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
#include <SFML/OpenGL.hpp>
#include <SFML/System/Err.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/Window/DRM/DRMContext.hpp>
#include <SFML/Window/DRM/WindowImplDRM.hpp>

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <poll.h>
#include <unistd.h>

namespace
{
bool            initialized = false;
drm             drmNode;
drmEventContext drmEventCtx;
pollfd          pollFD;
gbm_device*     gbmDevice      = nullptr;
int             contextCount   = 0;
EGLDisplay      display        = EGL_NO_DISPLAY;
int             waitingForFlip = 0;

static void pageFlipHandler(int /* fd */, unsigned int /* frame */, unsigned int /* sec */, unsigned int /* usec */, void* data)
{
    int* temp = static_cast<int*>(data);
    *temp     = 0;
}

static bool waitForFlip(int timeout)
{
    while (waitingForFlip)
    {
        pollFD.revents = 0;

        if (poll(&pollFD, 1, timeout) < 0)
            return false;

        if (pollFD.revents & (POLLHUP | POLLERR))
            return false;

        if (pollFD.revents & POLLIN)
        {
            drmHandleEvent(drmNode.fd, &drmEventCtx);
        }
        else
        {
            return false;
        }
    }
    return true;
}

void cleanup()
{
    if (!initialized)
        return;

    drmModeSetCrtc(drmNode.fd,
                   drmNode.original_crtc->crtc_id,
                   drmNode.original_crtc->buffer_id,
                   drmNode.original_crtc->x,
                   drmNode.original_crtc->y,
                   &drmNode.connector_id,
                   1,
                   &drmNode.original_crtc->mode);

    drmModeFreeConnector(drmNode.saved_connector);
    drmModeFreeEncoder(drmNode.saved_encoder);
    drmModeFreeCrtc(drmNode.original_crtc);

    eglTerminate(display);
    display = EGL_NO_DISPLAY;

    gbm_device_destroy(gbmDevice);
    gbmDevice = nullptr;

    close(drmNode.fd);

    drmNode.fd   = -1;
    drmNode.mode = 0;

    std::memset(&pollFD, 0, sizeof(pollfd));
    std::memset(&drmEventCtx, 0, sizeof(drmEventContext));

    waitingForFlip = 0;

    initialized = false;
}

void checkInit()
{
    if (initialized)
        return;

    // Use environment variable "SFML_DRM_DEVICE" (or nullptr if not set)
    char* deviceString = std::getenv("SFML_DRM_DEVICE");
    if (deviceString && !*deviceString)
        deviceString = nullptr;

    // Use environment variable "SFML_DRM_MODE" (or nullptr if not set)
    char* modeString = std::getenv("SFML_DRM_MODE");

    // Use environment variable "SFML_DRM_REFRESH" (or 0 if not set)
    // Use in combination with mode to request specific refresh rate for the mode
    // if multiple refresh rates for same mode might be supported
    unsigned int refreshRate   = 0;
    char*        refreshString = std::getenv("SFML_DRM_REFRESH");

    if (refreshString)
        refreshRate = static_cast<unsigned int>(atoi(refreshString));

    if (init_drm(&drmNode,
                 deviceString,     // device
                 modeString,       // requested mode
                 refreshRate) < 0) // screen refresh rate
    {
        sf::err() << "Error initializing DRM" << std::endl;
        return;
    }

    gbmDevice = gbm_create_device(drmNode.fd);

    std::atexit(cleanup);
    initialized = true;

    pollFD.fd                     = drmNode.fd;
    pollFD.events                 = POLLIN;
    drmEventCtx.version           = 2;
    drmEventCtx.page_flip_handler = pageFlipHandler;
}


EGLDisplay getInitializedDisplay()
{
    checkInit();

    if (display == EGL_NO_DISPLAY)
    {
        display = eglCheck(eglGetDisplay(reinterpret_cast<EGLNativeDisplayType>(gbmDevice)));

        EGLint major, minor;
        eglCheck(eglInitialize(display, &major, &minor));

#if defined(SFML_OPENGL_ES)
        if (!eglBindAPI(EGL_OPENGL_ES_API))
        {
            sf::err() << "failed to bind api EGL_OPENGL_ES_API" << std::endl;
        }
#else
        if (!eglBindAPI(EGL_OPENGL_API))
        {
            sf::err() << "failed to bind api EGL_OPENGL_API" << std::endl;
        }
#endif
    }

    return display;
}
} // namespace


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
DRMContext::DRMContext(DRMContext* shared) :
m_display(EGL_NO_DISPLAY),
m_context(EGL_NO_CONTEXT),
m_surface(EGL_NO_SURFACE),
m_config(nullptr),
m_currentBO(nullptr),
m_nextBO(nullptr),
m_gbmSurface(nullptr),
m_size(0, 0),
m_shown(false),
m_scanOut(false)
{
    contextCount++;

    // Get the initialized EGL display
    m_display = getInitializedDisplay();

    // Get the best EGL config matching the default video settings
    m_config = getBestConfig(m_display, VideoMode::getDesktopMode().bitsPerPixel, ContextSettings());
    updateSettings();

    // Create EGL context
    createContext(shared);

    if (shared)
        createSurface(shared->m_size, VideoMode::getDesktopMode().bitsPerPixel, false);
    else // create a surface to force the GL to initialize (seems to be required for glGetString() etc )
        createSurface({1, 1}, VideoMode::getDesktopMode().bitsPerPixel, false);
}


////////////////////////////////////////////////////////////
DRMContext::DRMContext(DRMContext* shared, const ContextSettings& settings, const WindowImpl& owner, unsigned int bitsPerPixel) :
m_display(EGL_NO_DISPLAY),
m_context(EGL_NO_CONTEXT),
m_surface(EGL_NO_SURFACE),
m_config(nullptr),
m_currentBO(nullptr),
m_nextBO(nullptr),
m_gbmSurface(nullptr),
m_size(0, 0),
m_shown(false),
m_scanOut(false)
{
    contextCount++;

    // Get the initialized EGL display
    m_display = getInitializedDisplay();

    // Get the best EGL config matching the requested video settings
    m_config = getBestConfig(m_display, bitsPerPixel, settings);
    updateSettings();

    // Create EGL context
    createContext(shared);

    Vector2u size = owner.getSize();
    createSurface(size, bitsPerPixel, true);
}


////////////////////////////////////////////////////////////
DRMContext::DRMContext(DRMContext* shared, const ContextSettings& settings, const Vector2u& size) :
m_display(EGL_NO_DISPLAY),
m_context(EGL_NO_CONTEXT),
m_surface(EGL_NO_SURFACE),
m_config(nullptr),
m_currentBO(nullptr),
m_nextBO(nullptr),
m_gbmSurface(nullptr),
m_size(0, 0),
m_shown(false),
m_scanOut(false)
{
    contextCount++;

    // Get the initialized EGL display
    m_display = getInitializedDisplay();

    // Get the best EGL config matching the requested video settings
    m_config = getBestConfig(m_display, VideoMode::getDesktopMode().bitsPerPixel, settings);
    updateSettings();

    // Create EGL context
    createContext(shared);
    createSurface(size, VideoMode::getDesktopMode().bitsPerPixel, false);
}


////////////////////////////////////////////////////////////
DRMContext::~DRMContext()
{
    // Deactivate the current context
    EGLContext currentContext = eglCheck(eglGetCurrentContext());

    if (currentContext == m_context)
    {
        eglCheck(eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));
    }

    // Destroy context
    if (m_context != EGL_NO_CONTEXT)
    {
        eglCheck(eglDestroyContext(m_display, m_context));
        m_context = EGL_NO_CONTEXT;
    }

    // Destroy surface
    if (m_surface != EGL_NO_SURFACE)
    {
        eglCheck(eglDestroySurface(m_display, m_surface));
        m_surface = EGL_NO_SURFACE;
    }

    if (m_currentBO)
        gbm_surface_release_buffer(m_gbmSurface, m_currentBO);

    if (m_nextBO)
        gbm_surface_release_buffer(m_gbmSurface, m_nextBO);

    if (m_gbmSurface)
        gbm_surface_destroy(m_gbmSurface);

    contextCount--;
    if (contextCount == 0)
        cleanup();
}


////////////////////////////////////////////////////////////
bool DRMContext::makeCurrent(bool current)
{
    const EGLSurface surface = current ? m_surface : EGL_NO_SURFACE;
    const EGLContext context = current ? m_context : EGL_NO_CONTEXT;
    return m_surface != EGL_NO_SURFACE && eglMakeCurrent(m_display, surface, surface, context);
}


////////////////////////////////////////////////////////////
void DRMContext::display()
{
    if (m_surface == EGL_NO_SURFACE)
        return;

    if (!m_scanOut)
    {
        eglCheck(eglSwapBuffers(m_display, m_surface));
        return;
    }

    // Handle display of buffer to the screen
    drm_fb* fb = nullptr;

    if (!waitForFlip(-1))
        return;

    if (m_currentBO)
    {
        gbm_surface_release_buffer(m_gbmSurface, m_currentBO);
        m_currentBO = nullptr;
    }

    eglCheck(eglSwapBuffers(m_display, m_surface));

    m_currentBO = m_nextBO;

    // This call must be preceeded by a single call to eglSwapBuffers()
    m_nextBO = gbm_surface_lock_front_buffer(m_gbmSurface);

    if (!m_nextBO)
        return;

    fb = drm_fb_get_from_bo(m_nextBO);
    if (!fb)
    {
        err() << "Failed to get FB from buffer object" << std::endl;
        return;
    }

    // If first time, need to first call drmModeSetCrtc()
    if (!m_shown)
    {
        if (drmModeSetCrtc(drmNode.fd, drmNode.crtc_id, fb->fb_id, 0, 0, &drmNode.connector_id, 1, drmNode.mode))
        {
            err() << "Failed to set mode: " << std::strerror(errno) << std::endl;
            std::abort();
        }
        m_shown = true;
    }

    // Do page flip
    if (!drmModePageFlip(drmNode.fd, drmNode.crtc_id, fb->fb_id, DRM_MODE_PAGE_FLIP_EVENT, &waitingForFlip))
        waitingForFlip = 1;
}


////////////////////////////////////////////////////////////
void DRMContext::setVerticalSyncEnabled(bool enabled)
{
    eglCheck(eglSwapInterval(m_display, enabled ? 1 : 0));
}


////////////////////////////////////////////////////////////
void DRMContext::createContext(DRMContext* shared)
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
    m_context = eglCheck(eglCreateContext(m_display, m_config, toShared, contextVersion));
    if (m_context == EGL_NO_CONTEXT)
        err() << "Failed to create EGL context" << std::endl;
}


////////////////////////////////////////////////////////////
void DRMContext::createSurface(const Vector2u& size, unsigned int /*bpp*/, bool scanout)
{
    std::uint32_t flags = GBM_BO_USE_RENDERING;

    m_scanOut = scanout;
    if (m_scanOut)
        flags |= GBM_BO_USE_SCANOUT;

    m_gbmSurface = gbm_surface_create(gbmDevice, size.x, size.y, GBM_FORMAT_ARGB8888, flags);

    if (!m_gbmSurface)
    {
        err() << "Failed to create gbm surface." << std::endl;
        return;
    }

    m_size = size;

    m_surface = eglCheck(
        eglCreateWindowSurface(m_display, m_config, reinterpret_cast<EGLNativeWindowType>(m_gbmSurface), nullptr));

    if (m_surface == EGL_NO_SURFACE)
    {
        err() << "Failed to create EGL Surface" << std::endl;
    }
}


////////////////////////////////////////////////////////////
void DRMContext::destroySurface()
{
    eglCheck(eglDestroySurface(m_display, m_surface));
    m_surface = EGL_NO_SURFACE;

    gbm_surface_destroy(m_gbmSurface);
    m_gbmSurface = nullptr;

    // Ensure that this context is no longer active since our surface is now destroyed
    setActive(false);
}


////////////////////////////////////////////////////////////
EGLConfig DRMContext::getBestConfig(EGLDisplay display, unsigned int bitsPerPixel, const ContextSettings& settings)
{
    // Set our video settings constraint
    const EGLint attributes[] =
    { EGL_BUFFER_SIZE,
      static_cast<EGLint>(bitsPerPixel),
      EGL_DEPTH_SIZE,
      static_cast<EGLint>(settings.depthBits),
      EGL_STENCIL_SIZE,
      static_cast<EGLint>(settings.stencilBits),
      EGL_SAMPLE_BUFFERS,
      static_cast<EGLint>(settings.antialiasingLevel),
      EGL_BLUE_SIZE,
      8,
      EGL_GREEN_SIZE,
      8,
      EGL_RED_SIZE,
      8,
      EGL_ALPHA_SIZE,
      8,

      EGL_SURFACE_TYPE,
      EGL_WINDOW_BIT,
#if defined(SFML_OPENGL_ES)
      EGL_RENDERABLE_TYPE,
      EGL_OPENGL_ES_BIT,
#else
      EGL_RENDERABLE_TYPE,
      EGL_OPENGL_BIT,
#endif
      EGL_NONE };

    EGLint    configCount;
    EGLConfig configs[1];

    // Ask EGL for the best config matching our video settings
    eglCheck(eglChooseConfig(display, attributes, configs, 1, &configCount));

    return configs[0];
}


////////////////////////////////////////////////////////////
void DRMContext::updateSettings()
{
    EGLint tmp;

    // Update the internal context settings with the current config
    eglCheck(eglGetConfigAttrib(m_display, m_config, EGL_DEPTH_SIZE, &tmp));
    m_settings.depthBits = static_cast<unsigned int>(tmp);

    eglCheck(eglGetConfigAttrib(m_display, m_config, EGL_STENCIL_SIZE, &tmp));
    m_settings.stencilBits = static_cast<unsigned int>(tmp);

    eglCheck(eglGetConfigAttrib(m_display, m_config, EGL_SAMPLES, &tmp));
    m_settings.antialiasingLevel = static_cast<unsigned int>(tmp);

    m_settings.majorVersion   = 1;
    m_settings.minorVersion   = 1;
    m_settings.attributeFlags = ContextSettings::Default;
}


////////////////////////////////////////////////////////////
GlFunctionPointer DRMContext::getFunction(const char* name)
{
    return reinterpret_cast<GlFunctionPointer>(eglGetProcAddress(name));
}


////////////////////////////////////////////////////////////
drm* DRMContext::getDRM()
{
    checkInit();
    return &drmNode;
}

} // namespace priv

} // namespace sf
