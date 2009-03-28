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
#include <SFML/Window/Linux/ContextGLX.hpp>
#include <SFML/Window/WindowImpl.hpp>
#include <GL/gl.h>
#include <SFML/Window/glext/glxext.h>
#include <SFML/Window/glext/glext.h>
#include <iostream>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Create a new context, not associated to a window
////////////////////////////////////////////////////////////
ContextGLX::ContextGLX(ContextGLX* Shared) :
myWindow    (NULL),
myContext   (NULL)/*,
myOwnsWindow(true)*/
{
    // Create the rendering context
    XVisualInfo Visual;
    CreateContext(Shared, VideoMode::GetDesktopMode().BitsPerPixel, ContextSettings(0, 0, 0));

    // Create a new color map with the chosen visual
    /*Colormap ColMap = XCreateColormap(ourDisplay, RootWindow(ourDisplay, ourScreen), Visual.visual, AllocNone);

    // Define the window attributes
    XSetWindowAttributes Attributes;
    Attributes.colormap = ColMap;

    // Create a dummy window (disabled and hidden)
    myWindow = XCreateWindow(ourDisplay,
                             RootWindow(ourDisplay, ourScreen),
                             0, 0,
                             myWidth, myHeight,
                             0,
                             Visual.depth,
                             InputOutput,
                             Visual.visual,
                             CWColormap, &Attributes);*/
}


////////////////////////////////////////////////////////////
/// Create a new context attached to a window
////////////////////////////////////////////////////////////
ContextGLX::ContextGLX(ContextWGL* Shared, const WindowImpl* Owner, unsigned int BitsPerPixel, const ContextSettings& Settings) :
myWindow    (NULL),
myContext   (NULL),/*
myOwnsWindow(false)*/
{
    // Get the owner window and its device context
    myWindow = static_cast<Window>(Owner->GetHandle());

    // Create the context
    if (myWindow)
        CreateContext(Shared, BitsPerPixel, Settings);
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
ContextGLX::~ContextGLX()
{
    // Destroy the OpenGL context
    if (myContext)
    {
        if (glXGetCurrentContext() == myContext)
            glXMakeCurrent(myDisplay.GetDisplay(), None, NULL);
        glXDestroyContext(myContext);
    }

    // Destroy the window if we own it
    /*if (myWindow && myOwnsWindow)
    {
        XDestroyWindow(myDisplay.GetDisplay(), myWindow);
        XFlush(myDisplay.GetDisplay());
    }*/
}


////////////////////////////////////////////////////////////
/// \see Context::MakeCurrent
////////////////////////////////////////////////////////////
bool ContextGLX::MakeCurrent(bool Active)
{
    if (Active)
    {
        if (myWindow && myContext)
        {
            if (glXGetCurrentContext() != myContext)
                return glXMakeCurrent(myDisplay.GetDisplay(), myWindow, myContext) != 0;
            else
                return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (glXGetCurrentContext() == myContext)
            return wglMakeCurrent(myDisplay.GetDisplay(), None, NULL) != 0;
        else
            return true;
    }
}


////////////////////////////////////////////////////////////
/// \see Context::Display
////////////////////////////////////////////////////////////
void ContextGLX::Display()
{
    if (myWindow)
        glXSwapBuffers(myDisplay.GetDisplay(), myWindow);
}


////////////////////////////////////////////////////////////
/// \see Context::UseVerticalSync
////////////////////////////////////////////////////////////
void ContextGLX::UseVerticalSync(bool Enabled)
{
    const GLubyte* ProcAddress = reinterpret_cast<const GLubyte*>("glXSwapIntervalSGI");
    PFNGLXSWAPINTERVALSGIPROC glXSwapIntervalSGI = reinterpret_cast<PFNGLXSWAPINTERVALSGIPROC>(glXGetProcAddress(ProcAddress));
    if (glXSwapIntervalSGI)
        glXSwapIntervalSGI(Enabled ? 1 : 0);
}


////////////////////////////////////////////////////////////
/// Check if a context is active on the current thread
////////////////////////////////////////////////////////////
bool ContextGLX::IsContextActive()
{
    return glXGetCurrentContext() != NULL;
}


////////////////////////////////////////////////////////////
/// Create the context
////////////////////////////////////////////////////////////
void ContextGLX::CreateContext(ContextGLX* Shared, unsigned int BitsPerPixel, const ContextSettings& Settings)
{
    XVisualInfo Template;
    Template.screen = DefaultScreen(myDisplayRef.GetDisplay());
    if (myWindow)
    {
        // Get the attributes of the target window
        XWindowAttributes WindowAttributes;
        if (XGetWindowAttributes(myDisplayRef.GetDisplay(), myWindow, &WindowAttributes) == 0)
        {
            std::cerr << "Failed to get the window attributes" << std::endl;
            return;
        }

        // Setup the visual infos to match
        Template.depth    = WindowAttributes.depth;
        Template.visualid = XVisualIDFromVisual(WindowAttributes.visual);
    }

    // Get all the visuals matching the template
    int NbVisuals = 0;
    XVisualInfo* Visuals = XGetVisualInfo(myDisplay.GetDisplay(), VisualDepthMask | VisualIDMask | VisualScreenMask, &Template, &NbVisuals);
    if (!Visuals || (NbVisuals == 0))
    {
        if (Visuals)
            XFree(Visuals);
        std::cerr << "There is no valid visual for the selected screen" << std::endl;
        return;
    }

    // Find the best visual
    int          BestScore  = 0xFFFF;
    XVisualInfo* BestVisual = NULL;
    while (!BestVisual)
    {
        for (int i = 0; i < NbVisuals; ++i)
        {
            // Get the current visual attributes
            int RGBA, DoubleBuffer, Red, Green, Blue, Alpha, Depth, Stencil, MultiSampling, Samples;
            glXGetConfig(ourDisplay, &Visuals[i], GLX_RGBA,               &RGBA);
            glXGetConfig(ourDisplay, &Visuals[i], GLX_DOUBLEBUFFER,       &DoubleBuffer); 
            glXGetConfig(ourDisplay, &Visuals[i], GLX_RED_SIZE,           &Red);
            glXGetConfig(ourDisplay, &Visuals[i], GLX_GREEN_SIZE,         &Green); 
            glXGetConfig(ourDisplay, &Visuals[i], GLX_BLUE_SIZE,          &Blue); 
            glXGetConfig(ourDisplay, &Visuals[i], GLX_ALPHA_SIZE,         &Alpha); 
            glXGetConfig(ourDisplay, &Visuals[i], GLX_DEPTH_SIZE,         &Depth);        
            glXGetConfig(ourDisplay, &Visuals[i], GLX_STENCIL_SIZE,       &Stencil);
            glXGetConfig(ourDisplay, &Visuals[i], GLX_SAMPLE_BUFFERS_ARB, &MultiSampling);        
            glXGetConfig(ourDisplay, &Visuals[i], GLX_SAMPLES_ARB,        &Samples);

            // First check the mandatory parameters
            if ((RGBA == 0) || (DoubleBuffer == 0))
                continue;

            // Evaluate the current configuration
            int Color = Red + Green + Blue + Alpha;
            int Score = EvaluateFormat(BitsPerPixel, mySettings, Color, Depth, Stencil, MultiSampling ? Samples : 0);

            // Keep it if it's better than the current best
            if (Score < BestScore)
            {
                BestScore  = Score;
                BestVisual = &Visuals[i];
            }
        }

        // If no visual has been found, try a lower level of antialiasing
        if (!BestVisual)
        {
            if (Params.AntialiasingLevel > 2)
            {
                std::cerr << "Failed to find a pixel format supporting "
                          << Params.AntialiasingLevel << " antialiasing levels ; trying with 2 levels" << std::endl;
                Params.AntialiasingLevel = 2;
            }
            else if (Params.AntialiasingLevel > 0)
            {
                std::cerr << "Failed to find a pixel format supporting antialiasing ; antialiasing will be disabled" << std::endl;
                Params.AntialiasingLevel = 0;
            }
            else
            {
                std::cerr << "Failed to find a suitable pixel format for the window -- cannot create OpenGL context" << std::endl;
                return;
            }
        }
    }

    // Get the context to share display lists with
    GLXContext ToShare = Shared ? Shared->myContext : NULL;

    // Create the context
    myContext = glXCreateContext(myDisplay.GetDisplay(), BestVisual, ToShare, true);
    if (!myContext)
    {
        std::cerr << "Failed to create an OpenGL context for this window" << std::endl;
        return;
    }

    // Update the creation settings from the chosen format
    int Depth, Stencil;
    glXGetConfig(myDisplay.GetDisplay(), BestVisual, GLX_DEPTH_SIZE,   &Depth);
    glXGetConfig(myDisplay.GetDisplay(), BestVisual, GLX_STENCIL_SIZE, &Stencil);
    mySettings.DepthBits   = static_cast<unsigned int>(Depth);
    mySettings.StencilBits = static_cast<unsigned int>(Stencil);

    // Change the target window's colormap so that it matches the context's one
    if (myWindow)
    {
        ::Window Root = RootWindow(myDisplay.GetDisplay(), DefaultScreen(myDisplay.GetDisplay()))
        Colormap ColMap = XCreateColormap(myDisplay.GetDisplay(), Root, BestVisual->visual, AllocNone);
        XSetWindowColormap(myDisplay.GetDisplay(), myWindow, ColMap);
    }

    // Free the temporary visuals array
    XFree(Visuals);
}

} // namespace priv

} // namespace sf
