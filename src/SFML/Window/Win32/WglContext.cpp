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
#include <SFML/Window/Win32/WglContext.hpp>
#include <SFML/Window/WindowImpl.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window/glext/wglext.h>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Err.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
WglContext::WglContext(WglContext* shared) :
myWindow       (NULL),
myDeviceContext(NULL),
myContext      (NULL),
myOwnsWindow   (true)
{
    // Creating a dummy window is mandatory: we could create a memory DC but then
    // its pixel format wouldn't match the regular contexts' format, and thus
    // wglShareLists would always fail. Too bad...

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
WglContext::WglContext(WglContext* shared, const WindowImpl* owner, unsigned int bitsPerPixel, const ContextSettings& settings) :
myWindow       (NULL),
myDeviceContext(NULL),
myContext      (NULL),
myOwnsWindow   (false)
{
    // Get the owner window and its device context
    myWindow = owner->GetSystemHandle();
    myDeviceContext = GetDC(myWindow);

    // Create the context
    if (myDeviceContext)
        CreateContext(shared, bitsPerPixel, settings);

    // Activate the context
    SetActive(true);
}


////////////////////////////////////////////////////////////
WglContext::~WglContext()
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
bool WglContext::MakeCurrent()
{
    return myDeviceContext && myContext && wglMakeCurrent(myDeviceContext, myContext);
}


////////////////////////////////////////////////////////////
void WglContext::Display()
{
    if (myDeviceContext && myContext)
        SwapBuffers(myDeviceContext);
}


////////////////////////////////////////////////////////////
void WglContext::EnableVerticalSync(bool enabled)
{
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
    if (wglSwapIntervalEXT)
        wglSwapIntervalEXT(enabled ? 1 : 0);
}


////////////////////////////////////////////////////////////
void WglContext::CreateContext(WglContext* shared, unsigned int bitsPerPixel, const ContextSettings& settings)
{
    // Save the creation settings
    mySettings = settings;

    // Let's find a suitable pixel format -- first try with antialiasing
    int bestFormat = 0;
    if (mySettings.AntialiasingLevel > 0)
    {
        // Get the wglChoosePixelFormatARB function (it is an extension)
        PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
        if (wglChoosePixelFormatARB)
        {
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
            while ((!isValid || (nbFormats == 0)) && mySettings.AntialiasingLevel > 0)
            {
                // Decrease the antialiasing level until we find a valid one
                mySettings.AntialiasingLevel--;
                intAttributes[11] = mySettings.AntialiasingLevel;
                isValid = wglChoosePixelFormatARB(myDeviceContext, intAttributes, floatAttributes, sizeof(formats) / sizeof(*formats), formats, &nbFormats) != 0;
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
        else
        {
            // wglChoosePixelFormatARB not supported ; disabling antialiasing
            Err() << "Antialiasing is not supported ; it will be disabled" << std::endl;
            mySettings.AntialiasingLevel = 0;
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
        descriptor.cAlphaBits   = bitsPerPixel == 32 ? 8 : 0;

        // Get the pixel format that best matches our requirements
        bestFormat = ChoosePixelFormat(myDeviceContext, &descriptor);
        if (bestFormat == 0)
        {
            Err() << "Failed to find a suitable pixel format for device context -- cannot create OpenGL context" << std::endl;
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
        Err() << "Failed to set pixel format for device context -- cannot create OpenGL context" << std::endl;
        return;
    }

    // Get the context to share display lists with
    HGLRC sharedContext = shared ? shared->myContext : NULL;

    // Create the OpenGL context -- first try context versions >= 3.0 if it is requested (they require special code)
    while (!myContext && (mySettings.MajorVersion >= 3))
    {
        PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
        if (wglCreateContextAttribsARB)
        {
            int attributes[] =
            {
                WGL_CONTEXT_MAJOR_VERSION_ARB, mySettings.MajorVersion,
                WGL_CONTEXT_MINOR_VERSION_ARB, mySettings.MinorVersion,
                WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                0, 0
            };
            myContext = wglCreateContextAttribsARB(myDeviceContext, sharedContext, attributes);
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

        myContext = wglCreateContext(myDeviceContext);
        if (!myContext)
        {
            Err() << "Failed to create an OpenGL context for this window" << std::endl;
            return;
        }

        // Share this context with others
        if (sharedContext)
        {
            // wglShareLists doesn't seem to be thread-safe
            static Mutex mutex;
            Lock lock(mutex);

            if (!wglShareLists(sharedContext, myContext))
                Err() << "Failed to share the OpenGL context" << std::endl;
        }
    }
}

} // namespace priv

} // namespace sf
