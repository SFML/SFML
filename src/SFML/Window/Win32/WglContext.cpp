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
#include <SFML/Window/WindowImpl.hpp> // included first to avoid a warning about macro redefinition
#include <SFML/Window/Win32/WglContext.hpp>
#include <SFML/Window/Win32/WglExtensions.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Err.hpp>
#include <sstream>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
void ensureExtensionsInit(HDC deviceContext)
{
    static bool initialized = false;
    if (!initialized)
    {
        int loaded = sfwgl_LoadFunctions(deviceContext);
        if (loaded == sfwgl_LOAD_FAILED)
        {
            err() << "Failed to initialize WglExtensions" << std::endl;
        }
        else
        {
            initialized = true;
        }
    }
}


////////////////////////////////////////////////////////////
String getErrorString(DWORD errorCode)
{
    std::basic_ostringstream<TCHAR, std::char_traits<TCHAR> > ss;
    TCHAR errBuff[256];
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, 0, errBuff, sizeof(errBuff), NULL);
    ss << errBuff;
    String errMsg(ss.str());

    return errMsg;
}


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
void* WglContext::getFunction(const char* name)
{
    void* address = reinterpret_cast<void*>(wglGetProcAddress(reinterpret_cast<LPCSTR>(name)));

    if (address)
    {
        // Test whether the returned value is a valid error code
        ptrdiff_t errorCode = reinterpret_cast<ptrdiff_t>(address);

        if ((errorCode != -1) && (errorCode != 1) && (errorCode != 2) && (errorCode != 3))
            return address;
    }

    static HMODULE module = NULL;

    if (!module)
        module = GetModuleHandleA("OpenGL32.dll");

    if (module)
        return reinterpret_cast<void*>(GetProcAddress(module, reinterpret_cast<LPCSTR>(name)));

    return 0;
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
    // Make sure that extensions are initialized
    ensureExtensionsInit(m_deviceContext);

    if (sfwgl_ext_EXT_swap_control == sfwgl_LOAD_SUCCEEDED)
        wglSwapIntervalEXT(enabled ? 1 : 0);
    else
    {
        // wglSwapIntervalEXT not supported
        err() << "Setting vertical sync not supported" << std::endl;
    }
}


////////////////////////////////////////////////////////////
void WglContext::createContext(WglContext* shared, unsigned int bitsPerPixel, const ContextSettings& settings)
{
    // Save the creation settings
    m_settings = settings;

    // Make sure that extensions are initialized if this is not the shared context
    // The shared context is the context used to initialize the extensions
    if (shared)
        ensureExtensionsInit(m_deviceContext);

    // Let's find a suitable pixel format -- first try with antialiasing
    int bestFormat = 0;
    if (m_settings.antialiasingLevel > 0)
    {
        if ((sfwgl_ext_ARB_pixel_format == sfwgl_LOAD_SUCCEEDED) && (sfwgl_ext_ARB_multisample == sfwgl_LOAD_SUCCEEDED))
        {
            // Define the basic attributes we want for our window
            int intAttributes[] =
            {
                WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
                WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
                WGL_SAMPLE_BUFFERS_ARB, (m_settings.antialiasingLevel ? 1 : 0),
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
            err() << "Failed to find a suitable pixel format for device context: " << getErrorString(GetLastError()).toAnsiString() << std::endl
                  << "Cannot create OpenGL context" << std::endl;
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
        err() << "Failed to set pixel format for device context: " << getErrorString(GetLastError()).toAnsiString() << std::endl
              << "Cannot create OpenGL context" << std::endl;
        return;
    }

    // Get the context to share display lists with
    HGLRC sharedContext = shared ? shared->m_context : NULL;

    // Create the OpenGL context -- first try using wglCreateContextAttribsARB
    while (!m_context && m_settings.majorVersion)
    {
        if (sfwgl_ext_ARB_create_context == sfwgl_LOAD_SUCCEEDED)
        {
            // Check if setting the profile is supported
            if (sfwgl_ext_ARB_create_context_profile == sfwgl_LOAD_SUCCEEDED)
            {
                int profile = (m_settings.attributeFlags & ContextSettings::Core) ? WGL_CONTEXT_CORE_PROFILE_BIT_ARB : WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
                int debug = (m_settings.attributeFlags & ContextSettings::Debug) ? WGL_CONTEXT_DEBUG_BIT_ARB : 0;

                int attributes[] =
                {
                    WGL_CONTEXT_MAJOR_VERSION_ARB, static_cast<int>(m_settings.majorVersion),
                    WGL_CONTEXT_MINOR_VERSION_ARB, static_cast<int>(m_settings.minorVersion),
                    WGL_CONTEXT_PROFILE_MASK_ARB,  profile,
                    WGL_CONTEXT_FLAGS_ARB,         debug,
                    0,                             0
                };
                m_context = wglCreateContextAttribsARB(m_deviceContext, sharedContext, attributes);
            }
            else
            {
                if ((m_settings.attributeFlags & ContextSettings::Core) || (m_settings.attributeFlags & ContextSettings::Debug))
                    err() << "Selecting a profile during context creation is not supported,"
                          << "disabling comptibility and debug" << std::endl;

                m_settings.attributeFlags = ContextSettings::Default;

                int attributes[] =
                {
                    WGL_CONTEXT_MAJOR_VERSION_ARB, static_cast<int>(m_settings.majorVersion),
                    WGL_CONTEXT_MINOR_VERSION_ARB, static_cast<int>(m_settings.minorVersion),
                    0,                             0
                };
                m_context = wglCreateContextAttribsARB(m_deviceContext, sharedContext, attributes);
            }
        }
        else
        {
            // If wglCreateContextAttribsARB is not supported, there is no need to keep trying
            break;
        }

        // If we couldn't create the context, first try disabling flags,
        // then lower the version number and try again -- stop at 0.0
        // Invalid version numbers will be generated by this algorithm (like 3.9), but we really don't care
        if (!m_context)
        {
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

    // If wglCreateContextAttribsARB failed, use wglCreateContext
    if (!m_context)
    {
        // set the context version to 2.1 (arbitrary) and disable flags
        m_settings.majorVersion = 2;
        m_settings.minorVersion = 1;
        m_settings.attributeFlags = ContextSettings::Default;

        m_context = wglCreateContext(m_deviceContext);
        if (!m_context)
        {
            err() << "Failed to create an OpenGL context for this window: " << getErrorString(GetLastError()).toAnsiString() << std::endl;
            return;
        }

        // Share this context with others
        if (sharedContext)
        {
            // wglShareLists doesn't seem to be thread-safe
            static Mutex mutex;
            Lock lock(mutex);

            if (!wglShareLists(sharedContext, m_context))
                err() << "Failed to share the OpenGL context: " << getErrorString(GetLastError()).toAnsiString() << std::endl;
        }
    }

    // Perform checks to inform the user if they are getting a context they might not have expected

    // 3.0 contexts only deprecate features, but do not remove them yet
    // 3.1 contexts remove features if ARB_compatibility is not present (we assume it isn't for simplicity)
    // 3.2+ contexts remove features only if a core profile is requested

    // If the context was created with wglCreateContext, it is guaranteed to be compatibility.
    // If a 3.2+ context was created with wglCreateContextAttribsARB, the compatibility flag
    // would have been set correctly already depending on whether ARB_create_context_profile is supported.

    // If the user requests a 3.0 context, it will be a compatibility context regardless of the requested profile.
    // If the user requests a 3.1 context and its creation was successful, the specification
    // states that it will not be a compatibility profile context regardless of the requested profile.
    if ((m_settings.majorVersion == 3) && (m_settings.minorVersion == 0))
        m_settings.attributeFlags &= ~ContextSettings::Core;
    else if ((m_settings.majorVersion == 3) && (m_settings.minorVersion == 1))
        m_settings.attributeFlags |= ContextSettings::Core;

    int version = m_settings.majorVersion * 10 + m_settings.minorVersion;
    int requestedVersion = settings.majorVersion * 10 + settings.minorVersion;

    if ((m_settings.attributeFlags    != settings.attributeFlags)    ||
        (version                      <  requestedVersion)           ||
        (m_settings.stencilBits       <  settings.stencilBits)       ||
        (m_settings.antialiasingLevel <  settings.antialiasingLevel) ||
        (m_settings.depthBits         <  settings.depthBits))
    {
        err() << "Warning: The created OpenGL context does not fully meet the settings that were requested" << std::endl;
        err() << "Requested: version = " << settings.majorVersion << "." << settings.minorVersion
              << " ; depth bits = " << settings.depthBits
              << " ; stencil bits = " << settings.stencilBits
              << " ; AA level = " << settings.antialiasingLevel
              << std::boolalpha
              << " ; core = " << ((settings.attributeFlags & ContextSettings::Core) != 0)
              << " ; debug = " << ((settings.attributeFlags & ContextSettings::Debug) != 0)
              << std::noboolalpha << std::endl;
        err() << "Created: version = " << m_settings.majorVersion << "." << m_settings.minorVersion
              << " ; depth bits = " << m_settings.depthBits
              << " ; stencil bits = " << m_settings.stencilBits
              << " ; AA level = " << m_settings.antialiasingLevel
              << std::boolalpha
              << " ; core = " << ((m_settings.attributeFlags & ContextSettings::Core) != 0)
              << " ; debug = " << ((m_settings.attributeFlags & ContextSettings::Debug) != 0)
              << std::noboolalpha << std::endl;
    }
}

} // namespace priv

} // namespace sf
