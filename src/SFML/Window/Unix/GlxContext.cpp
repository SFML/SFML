////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/System/Err.hpp>


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
        int loaded = sfglx_LoadFunctions(display, screen);
        if (loaded == sfglx_LOAD_FAILED)
        {
            err() << "Failed to initialize GlxExtensions" << std::endl;
        }
        else
        {
            initialized = true;
        }
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

    // Create a dummy window (disabled and hidden)
    xcb_screen_t* screen = XCBScreenOfDisplay(m_connection, DefaultScreen(m_display));
    m_window = xcb_generate_id(m_connection);
    xcb_create_window(
        m_connection,
        screen->root_depth,
        m_window, screen->root,
        0, 0,
        1, 1,
        0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        screen->root_visual,
        0, NULL
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

    // Create the hidden window
    xcb_screen_t* screen = XCBScreenOfDisplay(m_connection, DefaultScreen(m_display));
    m_window = xcb_generate_id(m_connection);
    xcb_create_window(
        m_connection,
        screen->root_depth,
        m_window, screen->root,
        0, 0,
        width, height,
        0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        screen->root_visual,
        0, NULL
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
        if (glXGetCurrentContext() == m_context)
            glXMakeCurrent(m_display, None, NULL);
        glXDestroyContext(m_display, m_context);
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
void* GlxContext::getFunction(const char* name)
{
    return reinterpret_cast<void*>(glXGetProcAddressARB(reinterpret_cast<const GLubyte*>(name)));
}


////////////////////////////////////////////////////////////
bool GlxContext::makeCurrent()
{
    return m_context && glXMakeCurrent(m_display, m_window, m_context);
}


////////////////////////////////////////////////////////////
void GlxContext::display()
{
    if (m_window)
        glXSwapBuffers(m_display, m_window);
}


////////////////////////////////////////////////////////////
void GlxContext::setVerticalSyncEnabled(bool enabled)
{
    // Make sure that extensions are initialized
    ensureExtensionsInit(m_display, DefaultScreen(m_display));

    if (sfglx_ext_EXT_swap_control == sfglx_LOAD_SUCCEEDED)
        glXSwapIntervalEXT(m_display, glXGetCurrentDrawable(), enabled ? 1 : 0);
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
        int bestScore = 0xFFFF;
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
            glXGetConfig(display, &visuals[i], GLX_RED_SIZE,           &red);
            glXGetConfig(display, &visuals[i], GLX_GREEN_SIZE,         &green);
            glXGetConfig(display, &visuals[i], GLX_BLUE_SIZE,          &blue);
            glXGetConfig(display, &visuals[i], GLX_ALPHA_SIZE,         &alpha);
            glXGetConfig(display, &visuals[i], GLX_DEPTH_SIZE,         &depth);
            glXGetConfig(display, &visuals[i], GLX_STENCIL_SIZE,       &stencil);
            glXGetConfig(display, &visuals[i], GLX_SAMPLE_BUFFERS_ARB, &multiSampling);
            glXGetConfig(display, &visuals[i], GLX_SAMPLES_ARB,        &samples);

            // Evaluate the visual
            int color = red + green + blue + alpha;
            int score = evaluateFormat(bitsPerPixel, settings, color, depth, stencil, multiSampling ? samples : 0);

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

    // Get the context to share display lists with
    GLXContext toShare = shared ? shared->m_context : NULL;

    // Make sure that extensions are initialized if this is not the shared context
    // The shared context is the context used to initialize the extensions
    if (shared)
        ensureExtensionsInit(m_display, DefaultScreen(m_display));

    // Create the OpenGL context -- first try using glXCreateContextAttribsARB
    if (sfglx_ext_ARB_create_context == sfglx_LOAD_SUCCEEDED)
    {
        // Select a GLXFB config that matches the requested context settings
        int nbConfigs = 0;
        GLXFBConfig* configs = NULL;

        // Check if multisampling is supported
        if (sfglx_ext_ARB_multisample == sfglx_LOAD_SUCCEEDED)
        {
            int fbAttributes[] =
            {
                GLX_DEPTH_SIZE,         static_cast<int>(settings.depthBits),
                GLX_STENCIL_SIZE,       static_cast<int>(settings.stencilBits),
                GLX_SAMPLE_BUFFERS_ARB, (settings.antialiasingLevel > 0) ? 1 : 0,
                GLX_SAMPLES_ARB,        static_cast<int>(settings.antialiasingLevel),
                GLX_RED_SIZE,           8,
                GLX_GREEN_SIZE,         8,
                GLX_BLUE_SIZE,          8,
                GLX_ALPHA_SIZE,         bitsPerPixel == 32 ? 8 : 0,
                GLX_DOUBLEBUFFER,       True,
                GLX_X_RENDERABLE,       True,
                GLX_DRAWABLE_TYPE,      GLX_WINDOW_BIT,
                GLX_RENDER_TYPE,        GLX_RGBA_BIT,
                GLX_CONFIG_CAVEAT,      GLX_NONE,
                None
            };
            configs = glXChooseFBConfig(m_display, DefaultScreen(m_display), fbAttributes, &nbConfigs);
        }
        else
        {
            m_settings.antialiasingLevel = 0;

            int fbAttributes[] =
            {
                GLX_DEPTH_SIZE,    static_cast<int>(settings.depthBits),
                GLX_STENCIL_SIZE,  static_cast<int>(settings.stencilBits),
                GLX_RED_SIZE,      8,
                GLX_GREEN_SIZE,    8,
                GLX_BLUE_SIZE,     8,
                GLX_ALPHA_SIZE,    bitsPerPixel == 32 ? 8 : 0,
                GLX_DOUBLEBUFFER,  True,
                GLX_X_RENDERABLE,  True,
                GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                GLX_RENDER_TYPE,   GLX_RGBA_BIT,
                GLX_CONFIG_CAVEAT, GLX_NONE,
                None
            };
            configs = glXChooseFBConfig(m_display, DefaultScreen(m_display), fbAttributes, &nbConfigs);
        }
        if (configs && nbConfigs)
        {
            while (!m_context && m_settings.majorVersion)
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
                    m_context = glXCreateContextAttribsARB(m_display, configs[0], toShare, true, attributes);
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
                    m_context = glXCreateContextAttribsARB(m_display, configs[0], toShare, true, attributes);
                }

                if (m_context)
                {
                    // Ok: retrieve the config's visual to change the colormap
                    XVisualInfo* visualInfo = glXGetVisualFromFBConfig(m_display, configs[0]);

                    // Change the target window's colormap so that it matches the context's one
                    ::Window root = RootWindow(m_display, DefaultScreen(m_display));
                    Colormap colorMap = XCreateColormap(m_display, root, visualInfo->visual, AllocNone);
                    XSetWindowColormap(m_display, m_window, colorMap);

                    // Free the visual info
                    XFree(visualInfo);

                    // Update the creation settings from the chosen format
                    int depth, stencil, multiSampling, samples;
                    glXGetFBConfigAttrib(m_display, configs[0], GLX_DEPTH_SIZE,         &depth);
                    glXGetFBConfigAttrib(m_display, configs[0], GLX_STENCIL_SIZE,       &stencil);
                    glXGetFBConfigAttrib(m_display, configs[0], GLX_SAMPLE_BUFFERS_ARB, &multiSampling);
                    glXGetFBConfigAttrib(m_display, configs[0], GLX_SAMPLES_ARB,        &samples);
                    m_settings.depthBits         = static_cast<unsigned int>(depth);
                    m_settings.stencilBits       = static_cast<unsigned int>(stencil);
                    m_settings.antialiasingLevel = multiSampling ? samples : 0;
                }
                else
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
            XFree(configs);
        }
    }

    // If glXCreateContextAttribsARB failed, use glXCreateContext
    if (!m_context)
    {
        // set the context version to 2.1 (arbitrary) and disable flags
        m_settings.majorVersion = 2;
        m_settings.minorVersion = 1;
        m_settings.attributeFlags = ContextSettings::Default;

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
        XVisualInfo* visuals = XGetVisualInfo(m_display, VisualIDMask | VisualScreenMask, &tpl, &nbVisuals);

        // Find the best visual
        int          bestScore  = 0xFFFF;
        XVisualInfo* bestVisual = NULL;
        for (int i = 0; i < nbVisuals; ++i)
        {
            // Get the current visual attributes
            int RGBA, doubleBuffer, red, green, blue, alpha, depth, stencil, multiSampling, samples;
            glXGetConfig(m_display, &visuals[i], GLX_RGBA,               &RGBA);
            glXGetConfig(m_display, &visuals[i], GLX_DOUBLEBUFFER,       &doubleBuffer);
            glXGetConfig(m_display, &visuals[i], GLX_RED_SIZE,           &red);
            glXGetConfig(m_display, &visuals[i], GLX_GREEN_SIZE,         &green);
            glXGetConfig(m_display, &visuals[i], GLX_BLUE_SIZE,          &blue);
            glXGetConfig(m_display, &visuals[i], GLX_ALPHA_SIZE,         &alpha);
            glXGetConfig(m_display, &visuals[i], GLX_DEPTH_SIZE,         &depth);
            glXGetConfig(m_display, &visuals[i], GLX_STENCIL_SIZE,       &stencil);
            glXGetConfig(m_display, &visuals[i], GLX_SAMPLE_BUFFERS_ARB, &multiSampling);
            glXGetConfig(m_display, &visuals[i], GLX_SAMPLES_ARB,        &samples);

            // First check the mandatory parameters
            if ((RGBA == 0) || (doubleBuffer == 0))
                continue;

            // Evaluate the current configuration
            int color = red + green + blue + alpha;
            int score = evaluateFormat(bitsPerPixel, m_settings, color, depth, stencil, multiSampling ? samples : 0);

            // Keep it if it's better than the current best
            if (score < bestScore)
            {
                bestScore  = score;
                bestVisual = &visuals[i];
            }
        }

        // Make sure that we have found a visual
        if (!bestVisual)
        {
            err() << "Failed to find a suitable pixel format for the window -- cannot create OpenGL context" << std::endl;
            return;
        }

        // Create the context, using the target window's visual
        m_context = glXCreateContext(m_display, bestVisual, toShare, true);
        if (!m_context)
        {
            err() << "Failed to create an OpenGL context for this window" << std::endl;
            return;
        }

        // Change the target window's colormap so that it matches the context's one
        ::Window root = RootWindow(m_display, DefaultScreen(m_display));
        Colormap colorMap = XCreateColormap(m_display, root, bestVisual->visual, AllocNone);
        XSetWindowColormap(m_display, m_window, colorMap);

        // Update the creation settings from the chosen format
        int depth, stencil, multiSampling, samples;
        glXGetConfig(m_display, bestVisual, GLX_DEPTH_SIZE,   &depth);
        glXGetConfig(m_display, bestVisual, GLX_STENCIL_SIZE, &stencil);

        if (sfglx_ext_ARB_multisample == sfglx_LOAD_SUCCEEDED)
        {
            glXGetConfig(m_display, bestVisual, GLX_SAMPLE_BUFFERS_ARB, &multiSampling);
            glXGetConfig(m_display, bestVisual, GLX_SAMPLES_ARB,        &samples);
        }
        else
        {
            multiSampling = 0;
            samples = 0;
        }

        m_settings.depthBits         = static_cast<unsigned int>(depth);
        m_settings.stencilBits       = static_cast<unsigned int>(stencil);
        m_settings.antialiasingLevel = multiSampling ? samples : 0;

        // Free the visual info
        XFree(visuals);
    }
}

} // namespace priv

} // namespace sf
