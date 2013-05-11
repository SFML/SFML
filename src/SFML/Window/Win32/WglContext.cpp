////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Window/WindowImpl.hpp> // included first to avoid a warning about macro redefinition
#include <SFML/Window/Win32/WglContext.hpp>
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
m_window       (NULL),
m_deviceContext(NULL),
m_context      (NULL),
m_ownsWindow   (true)
{
    // Creating a dummy window is mandatory: we could create a memory DC but then
    // its pixel format wouldn't match the regular contexts' format, and thus
    // wglShareLists would always fail. Too bad...

    // Create a dummy window (disabled and hidden)
    m_window = CreateWindowA("STATIC", "", WS_POPUP | WS_DISABLED, 0, 0, 1, 1, NULL, NULL, GetModuleHandle(NULL), NULL);
    ShowWindow(m_window, SW_HIDE);
    m_deviceContext = GetDC(m_window);

    // Create the context
    if (m_deviceContext)
        createContext(shared, VideoMode::getDesktopMode().bitsPerPixel, ContextSettings());
}


////////////////////////////////////////////////////////////
WglContext::WglContext(WglContext* shared, const ContextSettings& settings, const WindowImpl* owner, unsigned int bitsPerPixel) :
m_window       (NULL),
m_deviceContext(NULL),
m_context      (NULL),
m_ownsWindow   (false)
{
    // Get the owner window and its device context
    m_window = owner->getSystemHandle();
    m_deviceContext = GetDC(m_window);

    // Create the context
    if (m_deviceContext)
        createContext(shared, bitsPerPixel, settings);
}


////////////////////////////////////////////////////////////
WglContext::WglContext(WglContext* shared, const ContextSettings& settings, unsigned int width, unsigned int height) :
m_window       (NULL),
m_deviceContext(NULL),
m_context      (NULL),
m_ownsWindow   (true)
{
    // The target of the context is a hidden window.
    // We can't create a memory DC (the resulting context wouldn't be compatible
    // with other contexts), and we don't add the extra complexity of P-Buffers;
    // we can still support them in the future if this solution is not good enough.

    // Create the hidden window
    m_window = CreateWindowA("STATIC", "", WS_POPUP | WS_DISABLED, 0, 0, width, height, NULL, NULL, GetModuleHandle(NULL), NULL);
    ShowWindow(m_window, SW_HIDE);
    m_deviceContext = GetDC(m_window);

    // Create the context
    if (m_deviceContext)
        createContext(shared, VideoMode::getDesktopMode().bitsPerPixel, settings);
}


////////////////////////////////////////////////////////////
WglContext::~WglContext()
{
    // Destroy the OpenGL context
    if (m_context)
    {
        if (wglGetCurrentContext() == m_context)
            wglMakeCurrent(NULL, NULL);
        wglDeleteContext(m_context);
    }

    // Destroy the device context
    if (m_deviceContext)
        ReleaseDC(m_window, m_deviceContext);

    // Destroy the window if we own it
    if (m_window && m_ownsWindow)
        DestroyWindow(m_window);
}


////////////////////////////////////////////////////////////
bool WglContext::makeCurrent()
{
    return m_deviceContext && m_context && wglMakeCurrent(m_deviceContext, m_context);
}


////////////////////////////////////////////////////////////
void WglContext::display()
{
    if (m_deviceContext && m_context)
        SwapBuffers(m_deviceContext);
}


////////////////////////////////////////////////////////////
void WglContext::setVerticalSyncEnabled(bool enabled)
{
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
    if (wglSwapIntervalEXT)
        wglSwapIntervalEXT(enabled ? 1 : 0);
}


////////////////////////////////////////////////////////////
void WglContext::createContext(WglContext* shared, unsigned int bitsPerPixel, const ContextSettings& settings)
{
    // Save the creation settings
    m_settings = settings;

    // Let's find a suitable pixel format -- first try with antialiasing
    int bestFormat = 0;
    if (m_settings.antialiasingLevel > 0)
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
                WGL_SAMPLE_BUFFERS_ARB, (m_settings.antialiasingLevel ? GL_TRUE : GL_FALSE),
		        WGL_SAMPLES_ARB,        static_cast<int>(m_settings.antialiasingLevel),
		        0,                      0
            };

            // Let's check how many formats are supporting our requirements
            int   formats[128];
	        UINT  nbFormats;
	        float floatAttributes[] = {0, 0};
	        bool  isValid = wglChoosePixelFormatARB(m_deviceContext, intAttributes, floatAttributes, sizeof(formats) / sizeof(*formats), formats, &nbFormats) != 0;
            while ((!isValid || (nbFormats == 0)) && m_settings.antialiasingLevel > 0)
            {
                // Decrease the antialiasing level until we find a valid one
                m_settings.antialiasingLevel--;
                intAttributes[11] = m_settings.antialiasingLevel;
                isValid = wglChoosePixelFormatARB(m_deviceContext, intAttributes, floatAttributes, sizeof(formats) / sizeof(*formats), formats, &nbFormats) != 0;
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
                    DescribePixelFormat(m_deviceContext, formats[i], sizeof(attributes), &attributes);

                    // Evaluate the current configuration
                    int color = attributes.cRedBits + attributes.cGreenBits + attributes.cBlueBits + attributes.cAlphaBits;
                    int score = evaluateFormat(bitsPerPixel, m_settings, color, attributes.cDepthBits, attributes.cStencilBits, m_settings.antialiasingLevel);

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
            err() << "Antialiasing is not supported ; it will be disabled" << std::endl;
            m_settings.antialiasingLevel = 0;
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
        descriptor.cDepthBits   = static_cast<BYTE>(m_settings.depthBits);
        descriptor.cStencilBits = static_cast<BYTE>(m_settings.stencilBits);
        descriptor.cAlphaBits   = bitsPerPixel == 32 ? 8 : 0;

        // Get the pixel format that best matches our requirements
        bestFormat = ChoosePixelFormat(m_deviceContext, &descriptor);
        if (bestFormat == 0)
        {
            err() << "Failed to find a suitable pixel format for device context -- cannot create OpenGL context" << std::endl;
            return;
        }
    }

    // Extract the depth and stencil bits from the chosen format
    PIXELFORMATDESCRIPTOR actualFormat;
    actualFormat.nSize    = sizeof(actualFormat);
    actualFormat.nVersion = 1;
    DescribePixelFormat(m_deviceContext, bestFormat, sizeof(actualFormat), &actualFormat);
    m_settings.depthBits   = actualFormat.cDepthBits;
    m_settings.stencilBits = actualFormat.cStencilBits;

    // Set the chosen pixel format
    if (!SetPixelFormat(m_deviceContext, bestFormat, &actualFormat))
    {
        err() << "Failed to set pixel format for device context -- cannot create OpenGL context" << std::endl;
        return;
    }

    // Get the context to share display lists with
    HGLRC sharedContext = shared ? shared->m_context : NULL;

    // Create the OpenGL context -- first try context versions >= 3.0 if it is requested (they require special code)
    while (!m_context && (m_settings.majorVersion >= 3))
    {
        PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
        if (wglCreateContextAttribsARB)
        {
            int attributes[] =
            {
                WGL_CONTEXT_MAJOR_VERSION_ARB, static_cast<int>(m_settings.majorVersion),
                WGL_CONTEXT_MINOR_VERSION_ARB, static_cast<int>(m_settings.minorVersion),
                WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                0, 0
            };
            m_context = wglCreateContextAttribsARB(m_deviceContext, sharedContext, attributes);
        }

        // If we couldn't create the context, lower the version number and try again -- stop at 3.0
        // Invalid version numbers will be generated by this algorithm (like 3.9), but we really don't care
        if (!m_context)
        {
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

    // If the OpenGL >= 3.0 context failed or if we don't want one, create a regular OpenGL 1.x/2.x context
    if (!m_context)
    {
        // set the context version to 2.0 (arbitrary)
        m_settings.majorVersion = 2;
        m_settings.minorVersion = 0;

        m_context = wglCreateContext(m_deviceContext);
        if (!m_context)
        {
            err() << "Failed to create an OpenGL context for this window" << std::endl;
            return;
        }

        // Share this context with others
        if (sharedContext)
        {
            // wglShareLists doesn't seem to be thread-safe
            static Mutex mutex;
            Lock lock(mutex);

            if (!wglShareLists(sharedContext, m_context))
                err() << "Failed to share the OpenGL context" << std::endl;
        }
    }
}

} // namespace priv

} // namespace sf
