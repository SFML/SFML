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
#include <SFML/Window/Win32/ContextWGL.hpp>
#include <SFML/Window/WindowImpl.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window/glext/wglext.h>
#include <iostream>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Create a new context, not associated to a window
////////////////////////////////////////////////////////////
ContextWGL::ContextWGL(ContextWGL* shared) :
myWindow    (NULL),
myDC        (NULL),
myContext   (NULL),
myOwnsWindow(true)
{
    // TODO : try to create a bitmap in memory instead of a dummy window

    // Create a dummy window (disabled and hidden)
    myWindow = CreateWindowA("STATIC", "", WS_POPUP | WS_DISABLED, 0, 0, 1, 1, NULL, NULL, GetModuleHandle(NULL), NULL);
    ShowWindow(myWindow, SW_HIDE);
    myDC = GetDC(myWindow);

    // Create the context
    if (myDC)
        CreateContext(shared, VideoMode::GetDesktopMode().BitsPerPixel, ContextSettings(0, 0, 0));

    // Activate the context
    if (shared)
        SetActive(true);
}


////////////////////////////////////////////////////////////
/// Create a new context attached to a window
////////////////////////////////////////////////////////////
ContextWGL::ContextWGL(ContextWGL* shared, const WindowImpl* owner, unsigned int bitsPerPixel, const ContextSettings& settings) :
myWindow    (NULL),
myDC        (NULL),
myContext   (NULL),
myOwnsWindow(false)
{
    // Get the owner window and its device context
    myWindow = static_cast<HWND>(owner->GetHandle());
    myDC = GetDC(myWindow);

    // Create the context
    if (myDC)
        CreateContext(shared, bitsPerPixel, settings);

    // Activate the context
    if (shared)
        SetActive(true);
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
ContextWGL::~ContextWGL()
{
    // Destroy the OpenGL context
    if (myContext)
    {
        if (wglGetCurrentContext() == myContext)
            wglMakeCurrent(NULL, NULL);
        wglDeleteContext(myContext);
    }

    // Release the DC
    if (myWindow && myDC)
        ReleaseDC(myWindow, myDC);

    // Destroy the window if we own it
    if (myWindow && myOwnsWindow)
        DestroyWindow(myWindow);
}


////////////////////////////////////////////////////////////
/// \see Context::MakeCurrent
////////////////////////////////////////////////////////////
bool ContextWGL::MakeCurrent(bool active)
{
    if (active)
    {
        if (myDC && myContext)
        {
            if (wglGetCurrentContext() != myContext)
                return wglMakeCurrent(myDC, myContext) != 0;
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
        if (wglGetCurrentContext() == myContext)
            return wglMakeCurrent(NULL, NULL) != 0;
        else
            return true;
    }
}


////////////////////////////////////////////////////////////
/// \see Context::Display
////////////////////////////////////////////////////////////
void ContextWGL::Display()
{
    if (myDC && myContext)
        SwapBuffers(myDC);
}


////////////////////////////////////////////////////////////
/// \see Context::UseVerticalSync
////////////////////////////////////////////////////////////
void ContextWGL::UseVerticalSync(bool enabled)
{
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
    if (wglSwapIntervalEXT)
        wglSwapIntervalEXT(enabled ? 1 : 0);
}


////////////////////////////////////////////////////////////
/// Check if a context is active on the current thread
////////////////////////////////////////////////////////////
bool ContextWGL::IsContextActive()
{
    return wglGetCurrentContext() != NULL;
}


////////////////////////////////////////////////////////////
/// Create the context
////////////////////////////////////////////////////////////
void ContextWGL::CreateContext(ContextWGL* shared, unsigned int bitsPerPixel, const ContextSettings& settings)
{
    // Save the creation settings
    mySettings = settings;

    // Let's find a suitable pixel format -- first try with antialiasing
    int bestFormat = 0;
    if (mySettings.AntialiasingLevel > 0)
    {
        // Get the wglChoosePixelFormatARB function (it is an extension)
        PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));

        // Define the basic attributes we want for our window
        int intAttributes[] =
        {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		    WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		    WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
		    WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
            WGL_SAMPLE_BUFFERS_ARB, (mySettings.AntialiasingLevel ? GL_TRUE : GL_FALSE),
		    WGL_SAMPLES_ARB,        mySettings.AntialiasingLevel,
		    0,                      0
        };

        // Let's check how many formats are supporting our requirements
        int   formats[128];
	    UINT  nbFormats;
	    float floatAttributes[] = {0, 0};
	    bool  isValid = wglChoosePixelFormatARB(myDC, intAttributes, floatAttributes, sizeof(formats) / sizeof(*formats), formats, &nbFormats) != 0;
        if (!isValid || (nbFormats == 0))
        {
            if (mySettings.AntialiasingLevel > 2)
            {
                // No format matching our needs : reduce the multisampling level
                std::cerr << "Failed to find a pixel format supporting "
                          << mySettings.AntialiasingLevel << " antialiasing levels ; trying with 2 levels" << std::endl;

                mySettings.AntialiasingLevel = intAttributes[1] = 2;
	            isValid = wglChoosePixelFormatARB(myDC, intAttributes, floatAttributes, sizeof(formats) / sizeof(*formats), formats, &nbFormats) != 0;
            }

            if (!isValid || (nbFormats == 0))
            {
                // Cannot find any pixel format supporting multisampling ; disabling antialiasing
                std::cerr << "Failed to find a pixel format supporting antialiasing ; antialiasing will be disabled" << std::endl;
                mySettings.AntialiasingLevel = 0;
            }
        }

        // Get the best format among the returned ones
        if (isValid && (nbFormats > 0))
        {
            int bestScore = 0xFFFF;
            for (UINT i = 0; i < nbFormats; ++i)
            {
                // Get the current format's attributes
                PIXELFORMATDESCRIPTOR attributes;
                attributes.nSize    = sizeof(attributes);
                attributes.nVersion = 1;
                DescribePixelFormat(myDC, formats[i], sizeof(attributes), &attributes);

                // Evaluate the current configuration
                int color = attributes.cRedBits + attributes.cGreenBits + attributes.cBlueBits + attributes.cAlphaBits;
                int score = EvaluateFormat(bitsPerPixel, mySettings, color, attributes.cDepthBits, attributes.cStencilBits, mySettings.AntialiasingLevel);

                // Keep it if it's better than the current best
                if (score < bestScore)
                {
                    bestScore  = score;
                    bestFormat = formats[i];
                }
            }
        }
    }

    // Find a pixel format with no antialiasing, if not needed or not supported
    if (bestFormat == 0)
    {
        // Setup a pixel format descriptor from the rendering settings
        PIXELFORMATDESCRIPTOR descriptor;
        ZeroMemory(&descriptor, sizeof(descriptor));
        descriptor.nSize        = sizeof(descriptor);
        descriptor.nVersion     = 1;
        descriptor.iLayerType   = PFD_MAIN_PLANE;
        descriptor.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        descriptor.iPixelType   = PFD_TYPE_RGBA;
        descriptor.cColorBits   = static_cast<BYTE>(bitsPerPixel);
        descriptor.cDepthBits   = static_cast<BYTE>(mySettings.DepthBits);
        descriptor.cStencilBits = static_cast<BYTE>(mySettings.StencilBits);

        // Get the pixel format that best matches our requirements
        bestFormat = ChoosePixelFormat(myDC, &descriptor);
        if (bestFormat == 0)
        {
            std::cerr << "Failed to find a suitable pixel format for device context -- cannot create OpenGL context" << std::endl;
            return;
        }
    }

    // Extract the depth and stencil bits from the chosen format
    PIXELFORMATDESCRIPTOR actualFormat;
    actualFormat.nSize    = sizeof(actualFormat);
    actualFormat.nVersion = 1;
    DescribePixelFormat(myDC, bestFormat, sizeof(actualFormat), &actualFormat);
    mySettings.DepthBits   = actualFormat.cDepthBits;
    mySettings.StencilBits = actualFormat.cStencilBits;

    // Set the chosen pixel format
    if (!SetPixelFormat(myDC, bestFormat, &actualFormat))
    {
        std::cerr << "Failed to set pixel format for device context -- cannot create OpenGL context" << std::endl;
        return;
    }

    // Get the context to share display lists with
    HGLRC sharedContext = shared ? shared->myContext : NULL;

    // Create the OpenGL context -- first try an OpenGL 3.0 context if it is supported
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
    if (wglCreateContextAttribsARB)
    {
        int attributes[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
            WGL_CONTEXT_MINOR_VERSION_ARB, 0,
            0, 0
        };
        myContext = wglCreateContextAttribsARB(myDC, sharedContext, attributes);
    }

    // If the OpenGL 3.0 context failed, create a regular OpenGL 1.x context
    if (!myContext)
    {
        myContext = wglCreateContext(myDC);
        if (!myContext)
        {
            std::cerr << "Failed to create an OpenGL context for this window" << std::endl;
            return;
        }

        // Share this context with others
        if (sharedContext)
        {
            if (!wglShareLists(sharedContext, myContext))
                std::cerr << "Failed to share the OpenGL context" << std::endl;
        }
    }
}

} // namespace priv

} // namespace sf
