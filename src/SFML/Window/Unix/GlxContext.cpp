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
#include <SFML/OpenGL.hpp>
#include <SFML/System/Err.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
GlxContext::GlxContext(GlxContext* shared) :
m_window    (0),
m_context   (NULL),
m_ownsWindow(true)
{
    // Open a connection with the X server
    m_display = OpenDisplay();

    // Create a dummy window (disabled and hidden)
    int screen = DefaultScreen(m_display);
    m_window = XCreateWindow(m_display,
                             RootWindow(m_display, screen),
                             0, 0,
                             1, 1,
                             0,
                             DefaultDepth(m_display, screen),
                             InputOutput,
                             DefaultVisual(m_display, screen),
                             0, NULL);

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

    // Create the hidden window
    int screen = DefaultScreen(m_display);
    m_window = XCreateWindow(m_display,
                             RootWindow(m_display, screen),
                             0, 0,
                             width, height,
                             0,
                             DefaultDepth(m_display, screen),
                             InputOutput,
                             DefaultVisual(m_display, screen),
                             0, NULL);

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
        XDestroyWindow(m_display, m_window);
        XFlush(m_display);
    }

    // Close the connection with the X server
    CloseDisplay(m_display);
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
    const GLubyte* name = reinterpret_cast<const GLubyte*>("glXSwapIntervalSGI");
    PFNGLXSWAPINTERVALSGIPROC glXSwapIntervalSGI = reinterpret_cast<PFNGLXSWAPINTERVALSGIPROC>(glXGetProcAddress(name));
    if (glXSwapIntervalSGI)
        glXSwapIntervalSGI(enabled ? 1 : 0);
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
    XVisualInfo* visualInfo = NULL;

    // Save the creation settings
    m_settings = settings;

    // Get the context to share display lists with
    GLXContext toShare = shared ? shared->m_context : NULL;

    // Create the OpenGL context -- first try context versions >= 3.0 if it is requested (they require special code)
    if (m_settings.majorVersion >= 3)
    {
        const GLubyte* name = reinterpret_cast<const GLubyte*>("glXCreateContextAttribsARB");
        PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = reinterpret_cast<PFNGLXCREATECONTEXTATTRIBSARBPROC>(glXGetProcAddress(name));
        if (glXCreateContextAttribsARB)
        {
            // Select a GLXFB config that matches the requested context settings
            int nbConfigs = 0;
            int fbAttributes[] =
            {
                GLX_DEPTH_SIZE, static_cast<int>(settings.depthBits),
                GLX_STENCIL_SIZE, static_cast<int>(settings.stencilBits),
                GLX_SAMPLE_BUFFERS, settings.antialiasingLevel > 0,
                GLX_SAMPLES, static_cast<int>(settings.antialiasingLevel),
                GLX_RED_SIZE, 8,
                GLX_GREEN_SIZE, 8,
                GLX_BLUE_SIZE, 8,
                GLX_ALPHA_SIZE, bitsPerPixel == 32 ? 8 : 0,
                GLX_DOUBLEBUFFER, True,
                GLX_X_RENDERABLE, True,
                GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                GLX_RENDER_TYPE, GLX_RGBA_BIT,
                GLX_CONFIG_CAVEAT, GLX_NONE,
                None
            };
            GLXFBConfig* configs = glXChooseFBConfig(m_display, DefaultScreen(m_display), fbAttributes, &nbConfigs);
            if (configs && nbConfigs)
            {
                while (!m_context && (m_settings.majorVersion >= 3))
                {
                    // Create the context
                    int attributes[] =
                    {
                        GLX_CONTEXT_MAJOR_VERSION_ARB, static_cast<int>(m_settings.majorVersion),
                        GLX_CONTEXT_MINOR_VERSION_ARB, static_cast<int>(m_settings.minorVersion),
                        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                        0, 0
                    };
                    m_context = glXCreateContextAttribsARB(m_display, configs[0], toShare, true, attributes);

                    if (m_context)
                    {
                        // Ok: retrieve the config's visual
                        visualInfo = glXGetVisualFromFBConfig(m_display, configs[0]);
                    }
                    else
                    {
                        // If we couldn't create the context, lower the version number and try again -- stop at 3.0
                        // Invalid version numbers will be generated by this algorithm (like 3.9), but we really don't care
                        if (m_settings.minorVersion > 0)
                        {
                            // If the minor version is not 0, we decrease it and try again
                            m_settings.minorVersion--;
                        }
                        else
                        {
                            // If the minor version is 0, we decrease the major version
                            m_settings.majorVersion--;
                            m_settings.minorVersion = 9;
                        }
                    }
                }
                XFree(configs);
            }
        }
    }

    // If the OpenGL >= 3.0 context failed or if we don't want one, create a regular OpenGL 1.x/2.x context
    if (!m_context)
    {
        // set the context version to 2.0 (arbitrary)
        m_settings.majorVersion = 2;
        m_settings.minorVersion = 0;

        // Retrieve the attributes of the target window
        XWindowAttributes windowAttributes;
        if (XGetWindowAttributes(m_display, m_window, &windowAttributes) == 0)
        {
            err() << "Failed to get the window attributes" << std::endl;
            return;
        }

        // Get its visual
        XVisualInfo tpl;
        tpl.screen   = DefaultScreen(m_display);
        tpl.visualid = XVisualIDFromVisual(windowAttributes.visual);
        int nbVisuals = 0;
        visualInfo = XGetVisualInfo(m_display, VisualIDMask | VisualScreenMask, &tpl, &nbVisuals);

        // Create the context, using the target window's visual
        m_context = glXCreateContext(m_display, visualInfo, toShare, true);
        if (!m_context)
        {
            err() << "Failed to create an OpenGL context for this window" << std::endl;
            return;
        }
    }

    // Update the creation settings from the chosen format
    int depth, stencil, multiSampling, samples;
    glXGetConfig(m_display, visualInfo, GLX_DEPTH_SIZE,         &depth);
    glXGetConfig(m_display, visualInfo, GLX_STENCIL_SIZE,       &stencil);
    glXGetConfig(m_display, visualInfo, GLX_SAMPLE_BUFFERS_ARB, &multiSampling);
    glXGetConfig(m_display, visualInfo, GLX_SAMPLES_ARB,        &samples);
    m_settings.depthBits         = static_cast<unsigned int>(depth);
    m_settings.stencilBits       = static_cast<unsigned int>(stencil);
    m_settings.antialiasingLevel = multiSampling ? samples : 0;

    // Free the visual info
    XFree(visualInfo);
}

} // namespace priv

} // namespace sf
