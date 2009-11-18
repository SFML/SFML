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
#include <SFML/System/Lock.hpp>
#include <SFML/System/Mutex.hpp>
#include <iostream>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
ContextWGL::ContextWGL(ContextWGL* shared) :
myWindow       (NULL),
myDeviceContext(NULL),
myContext      (NULL),
myOwnsWindow   (true)
{
    // TODO : try to create a bitmap in memory instead of a dummy window

    // Create a dummy window (disabled and hidden)
    myWindow = CreateWindowA("STATIC", "", WS_POPUP | WS_DISABLED, 0, 0, 1, 1, NULL, NULL, GetModuleHandle(NULL), NULL);
    ShowWindow(myWindow, SW_HIDE);
    myDeviceContext = GetDC(myWindow);

    // Create the context
    if (myDeviceContext)
        CreateContext(shared, VideoMode::GetDesktopMode().BitsPerPixel, ContextSettings(0, 0, 0));

    // Activate the context
    SetActive(true);
}


////////////////////////////////////////////////////////////
ContextWGL::ContextWGL(ContextWGL* shared, const WindowImpl* owner, unsigned int bitsPerPixel, const ContextSettings& settings) :
myWindow       (NULL),
myDeviceContext(NULL),
myContext      (NULL),
myOwnsWindow   (false)
{
    // Get the owner window and its device context
    myWindow = static_cast<HWND>(owner->GetHandle());
    myDeviceContext = GetDC(myWindow);

    // Create the context
    if (myDeviceContext)
        CreateContext(shared, bitsPerPixel, settings);

    // Activate the context
    SetActive(true);
}


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
    if (myWindow && myDeviceContext)
        ReleaseDC(myWindow, myDeviceContext);

    // Destroy the window if we own it
    if (myWindow && myOwnsWindow)
        DestroyWindow(myWindow);
}


////////////////////////////////////////////////////////////
bool ContextWGL::MakeCurrent()
{
    if (myDeviceContext && myContext)
    {
        if (wglGetCurrentContext() != myContext)
            return wglMakeCurrent(myDeviceContext, myContext) != 0;
        else
            return true;
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
void ContextWGL::Display()
{
    if (myDeviceContext && myContext)
        SwapBuffers(myDeviceContext);
}


////////////////////////////////////////////////////////////
void ContextWGL::UseVerticalSync(bool enabled)
{
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
    if (wglSwapIntervalEXT)
        wglSwapIntervalEXT(enabled ? 1 : 0);
}


////////////////////////////////////////////////////////////
bool ContextWGL::IsContextActive()
{
    return wglGetCurrentContext() != NULL;
}


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
	    bool  isValid = wglChoosePixelFormatARB(myDeviceContext, intAttributes, floatAttributes, sizeof(formats) / sizeof(*formats), formats, &nbFormats) != 0;
        if (!isValid || (nbFormats == 0))
        {
            if (mySettings.AntialiasingLevel > 2)
            {
                // No format matching our needs : reduce the multisampling level
                std::cerr << "Failed to find a pixel format supporting "
                          << mySettings.AntialiasingLevel << " antialiasing levels ; trying with 2 levels" << std::endl;

                mySettings.AntialiasingLevel = intAttributes[11] = 2;
	            isValid = wglChoosePixelFormatARB(myDeviceContext, intAttributes, floatAttributes, sizeof(formats) / sizeof(*formats), formats, &nbFormats) != 0;
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
                DescribePixelFormat(myDeviceContext, formats[i], sizeof(attributes), &attributes);

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
        bestFormat = ChoosePixelFormat(myDeviceContext, &descriptor);
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
    DescribePixelFormat(myDeviceContext, bestFormat, sizeof(actualFormat), &actualFormat);
    mySettings.DepthBits   = actualFormat.cDepthBits;
    mySettings.StencilBits = actualFormat.cStencilBits;

    // Set the chosen pixel format
    if (!SetPixelFormat(myDeviceContext, bestFormat, &actualFormat))
    {
        std::cerr << "Failed to set pixel format for device context -- cannot create OpenGL context" << std::endl;
        return;
    }

    // Get the context to share display lists with
    HGLRC sharedContext = shared ? shared->myContext : NULL;

    // Create the OpenGL context -- first try an OpenGL 3.0 context if it is requested
    if (settings.MajorVersion >= 3)
    {
        PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
        if (wglCreateContextAttribsARB)
        {
            int attributes[] =
            {
                WGL_CONTEXT_MAJOR_VERSION_ARB, settings.MajorVersion,
                WGL_CONTEXT_MINOR_VERSION_ARB, settings.MinorVersion,
                0, 0
            };
            myContext = wglCreateContextAttribsARB(myDeviceContext, sharedContext, attributes);
        }

        // If we couldn't create an OpenGL 3 context, adjust the settings
        if (!myContext)
        {
            mySettings.MajorVersion = 2;
            mySettings.MinorVersion = 0;
        }
    }

    // If the OpenGL 3.0 context failed or if we don't want one, create a regular OpenGL 1.x/2.x context
    if (!myContext)
    {
        myContext = wglCreateContext(myDeviceContext);
        if (!myContext)
        {
            std::cerr << "Failed to create an OpenGL context for this window" << std::endl;
            return;
        }

        // Share this context with others
        if (sharedContext)
        {
            // wglShareLists doesn't seem to be thread-safe
            static Mutex mutex;
            Lock lock(mutex);

            if (!wglShareLists(sharedContext, myContext))
                std::cerr << "Failed to share the OpenGL context" << std::endl;
        }
    }
}

} // namespace priv

} // namespace sf
