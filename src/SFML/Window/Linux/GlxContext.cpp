////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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
#define GLX_GLXEXT_LEGACY // so that our local glxext.h is used instead of the system one
#include <SFML/Window/Linux/GlxContext.hpp>
#include <SFML/Window/Linux/WindowImplX11.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window/glext/glxext.h>
#include <SFML/System/Err.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
GlxContext::GlxContext(GlxContext* shared) :
myWindow    (0),
myContext   (NULL),
myOwnsWindow(true)
{
    // Open a connection with the X server
    myDisplay = XOpenDisplay(NULL);

    // Create a dummy window (disabled and hidden)
    int screen = DefaultScreen(myDisplay);
    myWindow = XCreateWindow(myDisplay,
                             RootWindow(myDisplay, screen),
                             0, 0,
                             1, 1,
                             0,
                             DefaultDepth(myDisplay, screen),
                             InputOutput,
                             DefaultVisual(myDisplay, screen),
                             0, NULL);

    // Create the context
    CreateContext(shared, VideoMode::GetDesktopMode().BitsPerPixel, ContextSettings(0, 0, 0));

    // Activate the context
    SetActive(true);
}


////////////////////////////////////////////////////////////
GlxContext::GlxContext(GlxContext* shared, const WindowImpl* owner, unsigned int bitsPerPixel, const ContextSettings& settings) :
myWindow    (0),
myContext   (NULL),
myOwnsWindow(false)
{
    // Use the same context as the owner window (important!)
    myDisplay = static_cast<const WindowImplX11*>(owner)->GetDisplay();

    // Get the owner window and its device context
    myWindow = static_cast<Window>(owner->GetSystemHandle());

    // Create the context
    if (myWindow)
        CreateContext(shared, bitsPerPixel, settings);

    // Activate the context
    SetActive(true);
}


////////////////////////////////////////////////////////////
GlxContext::~GlxContext()
{
    // Destroy the context
    if (myContext)
    {
        if (glXGetCurrentContext() == myContext)
            glXMakeCurrent(myDisplay, None, NULL);
        glXDestroyContext(myDisplay, myContext);
    }
    
    // Destroy the window if we own it
    if (myWindow && myOwnsWindow)
    {
        XDestroyWindow(myDisplay, myWindow);
        XFlush(myDisplay);
    }
    
    // Close the connection with the X server
    if (myOwnsWindow)
    {
        XCloseDisplay(myDisplay);
    }
}


////////////////////////////////////////////////////////////
bool GlxContext::MakeCurrent()
{
    return myContext && glXMakeCurrent(myDisplay, myWindow, myContext);
}


////////////////////////////////////////////////////////////
void GlxContext::Display()
{
    if (myWindow)
        glXSwapBuffers(myDisplay, myWindow);
}


////////////////////////////////////////////////////////////
void GlxContext::EnableVerticalSync(bool enabled)
{
    const GLubyte* name = reinterpret_cast<const GLubyte*>("glXSwapIntervalSGI");
    PFNGLXSWAPINTERVALSGIPROC glXSwapIntervalSGI = reinterpret_cast<PFNGLXSWAPINTERVALSGIPROC>(glXGetProcAddress(name));
    if (glXSwapIntervalSGI)
        glXSwapIntervalSGI(enabled ? 1 : 0);
}


////////////////////////////////////////////////////////////
void GlxContext::CreateContext(GlxContext* shared, unsigned int bitsPerPixel, const ContextSettings& settings)
{
    // Save the creation settings
    mySettings = settings;

    // Get the attributes of the target window
    XWindowAttributes windowAttributes;
    if (XGetWindowAttributes(myDisplay, myWindow, &windowAttributes) == 0)
    {
        Err() << "Failed to get the window attributes" << std::endl;
        return;
    }

    // Setup the visual infos to match
    XVisualInfo tpl;
    tpl.depth    = windowAttributes.depth;
    tpl.visualid = XVisualIDFromVisual(windowAttributes.visual);
    tpl.screen   = DefaultScreen(myDisplay);

    // Get all the visuals matching the template
    int nbVisuals = 0;
    XVisualInfo* visuals = XGetVisualInfo(myDisplay, VisualDepthMask | VisualIDMask | VisualScreenMask, &tpl, &nbVisuals);
    if (!visuals || (nbVisuals == 0))
    {
        if (visuals)
            XFree(visuals);
        Err() << "There is no valid visual for the selected screen" << std::endl;
        return;
    }

    // Find the best visual
    int          bestScore  = 0xFFFF;
    XVisualInfo* bestVisual = NULL;
    for (int i = 0; i < nbVisuals; ++i)
    {
        // Get the current visual attributes
        int RGBA, doubleBuffer, red, green, blue, alpha, depth, stencil, multiSampling, samples;
        glXGetConfig(myDisplay, &visuals[i], GLX_RGBA,               &RGBA);
        glXGetConfig(myDisplay, &visuals[i], GLX_DOUBLEBUFFER,       &doubleBuffer); 
        glXGetConfig(myDisplay, &visuals[i], GLX_RED_SIZE,           &red);
        glXGetConfig(myDisplay, &visuals[i], GLX_GREEN_SIZE,         &green); 
        glXGetConfig(myDisplay, &visuals[i], GLX_BLUE_SIZE,          &blue); 
        glXGetConfig(myDisplay, &visuals[i], GLX_ALPHA_SIZE,         &alpha); 
        glXGetConfig(myDisplay, &visuals[i], GLX_DEPTH_SIZE,         &depth);        
        glXGetConfig(myDisplay, &visuals[i], GLX_STENCIL_SIZE,       &stencil);
        glXGetConfig(myDisplay, &visuals[i], GLX_SAMPLE_BUFFERS_ARB, &multiSampling);        
        glXGetConfig(myDisplay, &visuals[i], GLX_SAMPLES_ARB,        &samples);

        // First check the mandatory parameters
        if ((RGBA == 0) || (doubleBuffer == 0))
            continue;

        // Evaluate the current configuration
        int color = red + green + blue + alpha;
        int score = EvaluateFormat(bitsPerPixel, mySettings, color, depth, stencil, multiSampling ? samples : 0);

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
        Err() << "Failed to find a suitable pixel format for the window -- cannot create OpenGL context" << std::endl;
        return;
    }

    // Get the context to share display lists with
    GLXContext toShare = shared ? shared->myContext : NULL;

    // Create the OpenGL context -- first try context versions >= 3.0 if it is requested (they require special code)
    while (!myContext && (mySettings.MajorVersion >= 3))
    {
        const GLubyte* name = reinterpret_cast<const GLubyte*>("glXCreateContextAttribsARB");
        PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = reinterpret_cast<PFNGLXCREATECONTEXTATTRIBSARBPROC>(glXGetProcAddress(name));
        if (glXCreateContextAttribsARB)
        {
            int nbConfigs = 0;
            GLXFBConfig* configs = glXChooseFBConfig(myDisplay, DefaultScreen(myDisplay), NULL, &nbConfigs);
            if (configs && nbConfigs)
            {
                // Create the context
                int attributes[] =
                {
                    GLX_CONTEXT_MAJOR_VERSION_ARB, mySettings.MajorVersion,
                    GLX_CONTEXT_MINOR_VERSION_ARB, mySettings.MinorVersion,
                    GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                    0, 0
                };
                myContext = glXCreateContextAttribsARB(myDisplay, configs[0], toShare, true, attributes);
            }

            if (configs)
                XFree(configs);
        }

        // If we couldn't create the context, lower the version number and try again -- stop at 3.0
        // Invalid version numbers will be generated by this algorithm (like 3.9), but we really don't care
        if (!myContext)
        {
            if (mySettings.MinorVersion > 0)
            {
                // If the minor version is not 0, we decrease it and try again
                mySettings.MinorVersion--;
            }
            else
            {
                // If the minor version is 0, we decrease the major version
                mySettings.MajorVersion--;
                mySettings.MinorVersion = 9;
            }
        }
    }

    // If the OpenGL >= 3.0 context failed or if we don't want one, create a regular OpenGL 1.x/2.x context
    if (!myContext)
    {
        // set the context version to 2.0 (arbitrary)
        mySettings.MajorVersion = 2;
        mySettings.MinorVersion = 0;

        myContext = glXCreateContext(myDisplay, bestVisual, toShare, true);
        if (!myContext)
        {
            Err() << "Failed to create an OpenGL context for this window" << std::endl;
            return;
        }
    }

    // Update the creation settings from the chosen format
    int depth, stencil, multiSampling, samples;
    glXGetConfig(myDisplay, bestVisual, GLX_DEPTH_SIZE,         &depth);
    glXGetConfig(myDisplay, bestVisual, GLX_STENCIL_SIZE,       &stencil);
    glXGetConfig(myDisplay, bestVisual, GLX_SAMPLE_BUFFERS_ARB, &multiSampling);        
    glXGetConfig(myDisplay, bestVisual, GLX_SAMPLES_ARB,        &samples);
    mySettings.DepthBits         = static_cast<unsigned int>(depth);
    mySettings.StencilBits       = static_cast<unsigned int>(stencil);
    mySettings.AntialiasingLevel = multiSampling ? samples : 0;

    // Change the target window's colormap so that it matches the context's one
    ::Window root = RootWindow(myDisplay, DefaultScreen(myDisplay));
    Colormap colorMap = XCreateColormap(myDisplay, root, bestVisual->visual, AllocNone);
    XSetWindowColormap(myDisplay, myWindow, colorMap);

    // Free the temporary visuals array
    XFree(visuals);
}

} // namespace priv

} // namespace sf
