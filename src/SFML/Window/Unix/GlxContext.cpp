////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/Unix/GlxContext.hpp>
#include <SFML/Window/Unix/WindowImplX11.hpp>
#include <SFML/Window/Unix/Display.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Err.hpp>

#if !defined(GLX_DEBUGGING) && defined(SFML_DEBUG)
    // Enable this to print messages to err() everytime GLX produces errors
    //#define GLX_DEBUGGING
#endif

namespace
{
    sf::Mutex glxErrorMutex;
    bool glxErrorOccurred = false;

    int HandleXError(::Display*, XErrorEvent*)
    {
        glxErrorOccurred = true;
        return 0;
    }

    class GlxErrorHandler
    {
    public:

        GlxErrorHandler(::Display* display) :
        m_display(display),
        m_lock   (glxErrorMutex)
        {
            glxErrorOccurred = false;
            m_previousHandler = XSetErrorHandler(HandleXError);
        }

        ~GlxErrorHandler()
        {
            XSync(m_display, False);
            XSetErrorHandler(m_previousHandler);
        }

    private:
        sf::Lock   m_lock;
        ::Display* m_display;
        int      (*m_previousHandler)(::Display*, XErrorEvent*);
    };
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
void ensureExtensionsInit(::Display* display, int screen)
{
    static bool initialized = false;
    if (!initialized)
    {
        initialized = true;

        // We don't check the return value since the extension
        // flags are cleared even if loading fails
        sfglx_LoadFunctions(display, screen);
    }
}


////////////////////////////////////////////////////////////
GlxContext::GlxContext(GlxContext* shared) :
m_window    (0),
m_context   (NULL),
m_ownsWindow(true)
{
    // Open a connection with the X server
    m_display = OpenDisplay();
    m_connection = XGetXCBConnection(m_display);
    xcb_screen_t* screen = XCBScreenOfDisplay(m_connection, DefaultScreen(m_display));

    // Choose the visual according to the context settings
    XVisualInfo visualInfo = selectBestVisual(m_display, VideoMode::getDesktopMode().bitsPerPixel, ContextSettings());

    // Define the window attributes
    xcb_colormap_t colormap = xcb_generate_id(m_connection);
    xcb_create_colormap(m_connection, XCB_COLORMAP_ALLOC_NONE, colormap, screen->root, visualInfo.visualid);
    const uint32_t value_list[] = {colormap};

    // Create a dummy window (disabled and hidden)
    m_window = xcb_generate_id(m_connection);
    xcb_create_window(
        m_connection,
        static_cast<uint8_t>(visualInfo.depth),
        m_window,
        screen->root,
        0, 0,
        1, 1,
        0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        visualInfo.visualid,
        XCB_CW_COLORMAP,
        value_list
    );

    // Create the context
    createContext(shared, VideoMode::getDesktopMode().bitsPerPixel, ContextSettings());
}


////////////////////////////////////////////////////////////
GlxContext::GlxContext(GlxContext* shared, const ContextSettings& settings, const WindowImpl* owner, unsigned int bitsPerPixel) :
m_window    (0),
m_context   (NULL),
m_ownsWindow(false)
{
    // Open a connection with the X server
    // (important: must be the same display as the owner window)
    m_display = OpenDisplay();
    m_connection = XGetXCBConnection(m_display);

    // Get the owner window and its device context
    m_window = static_cast< ::Window>(owner->getSystemHandle());

    // Create the context
    if (m_window)
        createContext(shared, bitsPerPixel, settings);
}


////////////////////////////////////////////////////////////
GlxContext::GlxContext(GlxContext* shared, const ContextSettings& settings, unsigned int width, unsigned int height) :
m_window    (0),
m_context   (NULL),
m_ownsWindow(true)
{
    // Open a connection with the X server
    m_display = OpenDisplay();
    m_connection = XGetXCBConnection(m_display);
    xcb_screen_t* screen = XCBScreenOfDisplay(m_connection, DefaultScreen(m_display));

    // Choose the visual according to the context settings
    XVisualInfo visualInfo = selectBestVisual(m_display, VideoMode::getDesktopMode().bitsPerPixel, settings);

    // Define the window attributes
    xcb_colormap_t colormap = xcb_generate_id(m_connection);
    xcb_create_colormap(m_connection, XCB_COLORMAP_ALLOC_NONE, colormap, screen->root, visualInfo.visualid);
    const uint32_t value_list[] = {colormap};

    // Create the hidden window
    m_window = xcb_generate_id(m_connection);
    xcb_create_window(
        m_connection,
        static_cast<uint8_t>(visualInfo.depth),
        m_window,
        screen->root,
        0, 0,
        width, height,
        0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        visualInfo.visualid,
        XCB_CW_COLORMAP,
        value_list
    );

    // Create the context
    createContext(shared, VideoMode::getDesktopMode().bitsPerPixel, settings);
}


////////////////////////////////////////////////////////////
GlxContext::~GlxContext()
{
    // Destroy the context
    if (m_context)
    {
#if defined(GLX_DEBUGGING)
        GlxErrorHandler handler(m_display);
#endif

        if (glXGetCurrentContext() == m_context)
            glXMakeCurrent(m_display, None, NULL);
        glXDestroyContext(m_display, m_context);

#if defined(GLX_DEBUGGING)
        if (glxErrorOccurred)
            err() << "GLX error in GlxContext::~GlxContext()" << std::endl;
#endif
    }

    // Destroy the window if we own it
    if (m_window && m_ownsWindow)
    {
        xcb_destroy_window(m_connection, m_window);
        xcb_flush(m_connection);
    }

    // Close the connection with the X server
    CloseDisplay(m_display);
}


////////////////////////////////////////////////////////////
GlFunctionPointer GlxContext::getFunction(const char* name)
{
    return reinterpret_cast<GlFunctionPointer>(glXGetProcAddressARB(reinterpret_cast<const GLubyte*>(name)));
}


////////////////////////////////////////////////////////////
bool GlxContext::makeCurrent()
{
    if (!m_context)
        return false;

#if defined(GLX_DEBUGGING)
    GlxErrorHandler handler(m_display);
#endif

    bool result = glXMakeCurrent(m_display, m_window, m_context);

#if defined(GLX_DEBUGGING)
    if (glxErrorOccurred)
        err() << "GLX error in GlxContext::makeCurrent()" << std::endl;
#endif

    return result;
}


////////////////////////////////////////////////////////////
void GlxContext::display()
{
#if defined(GLX_DEBUGGING)
    GlxErrorHandler handler(m_display);
#endif

    if (m_window)
        glXSwapBuffers(m_display, m_window);

#if defined(GLX_DEBUGGING)
    if (glxErrorOccurred)
        err() << "GLX error in GlxContext::display()" << std::endl;
#endif
}


////////////////////////////////////////////////////////////
void GlxContext::setVerticalSyncEnabled(bool enabled)
{
    // Make sure that extensions are initialized
    ensureExtensionsInit(m_display, DefaultScreen(m_display));

    int result = 0;

    // Prioritize the EXT variant and fall back to MESA or SGI if needed
    // We use the direct pointer to the MESA entry point instead of the alias
    // because glx.h declares the entry point as an external function
    // which would require us to link in an additional library
    if (sfglx_ext_EXT_swap_control == sfglx_LOAD_SUCCEEDED)
    {
        glXSwapIntervalEXT(m_display, glXGetCurrentDrawable(), enabled ? 1 : 0);
    }
    else if (sfglx_ext_MESA_swap_control == sfglx_LOAD_SUCCEEDED)
    {
        result = sf_ptrc_glXSwapIntervalMESA(enabled ? 1 : 0);
    }
    else if (sfglx_ext_SGI_swap_control == sfglx_LOAD_SUCCEEDED)
    {
        result = glXSwapIntervalSGI(enabled ? 1 : 0);
    }
    else
    {
        static bool warned = false;

        if (!warned)
        {
            err() << "Setting vertical sync not supported" << std::endl;

            warned = true;
        }
    }

    if (result != 0)
        err() << "Setting vertical sync failed" << std::endl;
}


////////////////////////////////////////////////////////////
XVisualInfo GlxContext::selectBestVisual(::Display* display, unsigned int bitsPerPixel, const ContextSettings& settings)
{
    // Retrieve all the visuals
    int count;
    XVisualInfo* visuals = XGetVisualInfo(display, 0, NULL, &count);
    if (visuals)
    {
        // Evaluate all the returned visuals, and pick the best one
        int bestScore = 0x7FFFFFFF;
        XVisualInfo bestVisual;
        for (int i = 0; i < count; ++i)
        {
            // Check mandatory attributes
            int doubleBuffer;
            glXGetConfig(display, &visuals[i], GLX_DOUBLEBUFFER, &doubleBuffer);
            if (!doubleBuffer)
                continue;

            // Extract the components of the current visual
            int red, green, blue, alpha, depth, stencil, multiSampling, samples;
            glXGetConfig(display, &visuals[i], GLX_RED_SIZE,     &red);
            glXGetConfig(display, &visuals[i], GLX_GREEN_SIZE,   &green);
            glXGetConfig(display, &visuals[i], GLX_BLUE_SIZE,    &blue);
            glXGetConfig(display, &visuals[i], GLX_ALPHA_SIZE,   &alpha);
            glXGetConfig(display, &visuals[i], GLX_DEPTH_SIZE,   &depth);
            glXGetConfig(display, &visuals[i], GLX_STENCIL_SIZE, &stencil);

            if (sfglx_ext_ARB_multisample == sfglx_LOAD_SUCCEEDED)
            {
                glXGetConfig(display, &visuals[i], GLX_SAMPLE_BUFFERS_ARB, &multiSampling);
                glXGetConfig(display, &visuals[i], GLX_SAMPLES_ARB,        &samples);
            }
            else
            {
                multiSampling = 0;
                samples = 0;
            }

            // TODO: Replace this with proper acceleration detection
            bool accelerated = true;

            // Evaluate the visual
            int color = red + green + blue + alpha;
            int score = evaluateFormat(bitsPerPixel, settings, color, depth, stencil, multiSampling ? samples : 0, accelerated);

            // If it's better than the current best, make it the new best
            if (score < bestScore)
            {
                bestScore = score;
                bestVisual = visuals[i];
            }
        }

        // Free the array of visuals
        XFree(visuals);

        return bestVisual;
    }
    else
    {
        // Should never happen...
        err() << "No GLX visual found. You should check your graphics driver" << std::endl;

        return XVisualInfo();
    }
}

////////////////////////////////////////////////////////////
void GlxContext::createContext(GlxContext* shared, unsigned int bitsPerPixel, const ContextSettings& settings)
{
    // Save the creation settings
    m_settings = settings;

    // Retrieve the attributes of the target window
    XWindowAttributes windowAttributes;
    if (XGetWindowAttributes(m_display, m_window, &windowAttributes) == 0)
    {
        err() << "Failed to get the window attributes" << std::endl;
        return;
    }

    // Get its visuals
    XVisualInfo tpl;
    tpl.screen   = DefaultScreen(m_display);
    tpl.visualid = XVisualIDFromVisual(windowAttributes.visual);
    int nbVisuals = 0;
    XVisualInfo* visualInfo = XGetVisualInfo(m_display, VisualIDMask | VisualScreenMask, &tpl, &nbVisuals);

    // Get the context to share display lists with
    GLXContext toShare = shared ? shared->m_context : NULL;

    // There are no GLX versions prior to 1.0
    int major = 0;
    int minor = 0;

    if (!glXQueryVersion(m_display, &major, &minor))
        err() << "Failed to query GLX version, limited to legacy context creation" << std::endl;

    // Make sure that extensions are initialized if this is not the shared context
    // The shared context is the context used to initialize the extensions
    if (shared)
        ensureExtensionsInit(m_display, DefaultScreen(m_display));

    // Check if glXCreateContextAttribsARB is available (requires GLX 1.3 or greater)
    bool hasCreateContextArb = (sfglx_ext_ARB_create_context == sfglx_LOAD_SUCCEEDED) && ((major > 1) || (minor >= 3));

    // Check if we need to use glXCreateContextAttribsARB
    bool needCreateContextArb = false;

    if (m_settings.attributeFlags)
        needCreateContextArb = true;
    else if (m_settings.majorVersion >= 3)
        needCreateContextArb = true;

    // Create the OpenGL context -- first try using glXCreateContextAttribsARB if we need to
    if (hasCreateContextArb && needCreateContextArb)
    {
        // Get a GLXFBConfig that matches the the window's visual, for glXCreateContextAttribsARB
        GLXFBConfig* config = NULL;

        // We don't supply attributes to match against, since
        // the visual we are matching against was already
        // deemed suitable in selectBestVisual()
        int nbConfigs = 0;
        GLXFBConfig* configs = glXChooseFBConfig(m_display, DefaultScreen(m_display), NULL, &nbConfigs);

        for (int i = 0; configs && (i < nbConfigs); ++i)
        {
            XVisualInfo* visual = glXGetVisualFromFBConfig(m_display, configs[i]);

            if (!visual)
                continue;

            if (visual->visualid == visualInfo->visualid)
            {
                config = &configs[i];
                break;
            }
        }

        if (!config)
            err() << "Failed to get GLXFBConfig which corresponds to the window's visual" << std::endl;

        while (config && !m_context && m_settings.majorVersion)
        {
            // Check if setting the profile is supported
            if (sfglx_ext_ARB_create_context_profile == sfglx_LOAD_SUCCEEDED)
            {
                int profile = (m_settings.attributeFlags & ContextSettings::Core) ? GLX_CONTEXT_CORE_PROFILE_BIT_ARB : GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
                int debug = (m_settings.attributeFlags & ContextSettings::Debug) ? GLX_CONTEXT_DEBUG_BIT_ARB : 0;

                // Create the context
                int attributes[] =
                {
                    GLX_CONTEXT_MAJOR_VERSION_ARB, static_cast<int>(m_settings.majorVersion),
                    GLX_CONTEXT_MINOR_VERSION_ARB, static_cast<int>(m_settings.minorVersion),
                    GLX_CONTEXT_PROFILE_MASK_ARB,  profile,
                    GLX_CONTEXT_FLAGS_ARB,         debug,
                    0,                             0
                };

                // RAII GLX error handler (we simply ignore errors here)
                // On an error, glXCreateContextAttribsARB will return 0 anyway
                GlxErrorHandler handler(m_display);

                m_context = glXCreateContextAttribsARB(m_display, *config, toShare, true, attributes);
            }
            else
            {
                if ((m_settings.attributeFlags & ContextSettings::Core) || (m_settings.attributeFlags & ContextSettings::Debug))
                    err() << "Selecting a profile during context creation is not supported,"
                          << "disabling comptibility and debug" << std::endl;

                m_settings.attributeFlags = ContextSettings::Default;

                // Create the context
                int attributes[] =
                {
                    GLX_CONTEXT_MAJOR_VERSION_ARB, static_cast<int>(m_settings.majorVersion),
                    GLX_CONTEXT_MINOR_VERSION_ARB, static_cast<int>(m_settings.minorVersion),
                    0,                             0
                };

                // RAII GLX error handler (we simply ignore errors here)
                // On an error, glXCreateContextAttribsARB will return 0 anyway
                GlxErrorHandler handler(m_display);

                m_context = glXCreateContextAttribsARB(m_display, *config, toShare, true, attributes);
            }

            if (!m_context)
            {
                // If we couldn't create the context, first try disabling flags,
                // then lower the version number and try again -- stop at 0.0
                // Invalid version numbers will be generated by this algorithm (like 3.9), but we really don't care
                if (m_settings.attributeFlags != ContextSettings::Default)
                {
                    m_settings.attributeFlags = ContextSettings::Default;
                }
                else if (m_settings.minorVersion > 0)
                {
                    // If the minor version is not 0, we decrease it and try again
                    m_settings.minorVersion--;

                    m_settings.attributeFlags = settings.attributeFlags;
                }
                else
                {
                    // If the minor version is 0, we decrease the major version
                    m_settings.majorVersion--;
                    m_settings.minorVersion = 9;

                    m_settings.attributeFlags = settings.attributeFlags;
                }
            }
        }

        if (configs)
            XFree(configs);
    }

    // If glXCreateContextAttribsARB failed, use glXCreateContext
    if (!m_context)
    {
        // set the context version to 2.1 (arbitrary) and disable flags
        m_settings.majorVersion = 2;
        m_settings.minorVersion = 1;
        m_settings.attributeFlags = ContextSettings::Default;

#if defined(GLX_DEBUGGING)
    GlxErrorHandler handler(m_display);
#endif

        // Create the context, using the target window's visual
        m_context = glXCreateContext(m_display, visualInfo, toShare, true);

#if defined(GLX_DEBUGGING)
    if (glxErrorOccurred)
        err() << "GLX error in GlxContext::createContext()" << std::endl;
#endif
    }

    if (!m_context)
    {
        err() << "Failed to create an OpenGL context for this window" << std::endl;
    }
    else
    {
        // Update the creation settings from the chosen format
        int depth, stencil, multiSampling, samples;
        glXGetConfig(m_display, visualInfo, GLX_DEPTH_SIZE,         &depth);
        glXGetConfig(m_display, visualInfo, GLX_STENCIL_SIZE,       &stencil);

        if (sfglx_ext_ARB_multisample == sfglx_LOAD_SUCCEEDED)
        {
            glXGetConfig(m_display, visualInfo, GLX_SAMPLE_BUFFERS_ARB, &multiSampling);
            glXGetConfig(m_display, visualInfo, GLX_SAMPLES_ARB,        &samples);
        }
        else
        {
            multiSampling = 0;
            samples = 0;
        }

        m_settings.depthBits         = static_cast<unsigned int>(depth);
        m_settings.stencilBits       = static_cast<unsigned int>(stencil);
        m_settings.antialiasingLevel = multiSampling ? samples : 0;
    }

    // Free the visual info
    XFree(visualInfo);
}

} // namespace priv

} // namespace sf
