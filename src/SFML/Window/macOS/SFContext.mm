////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Marco Antognini (antognini.marco@gmail.com),
//                         Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/macOS/AutoreleasePoolWrapper.hpp>
#include <SFML/Window/macOS/SFContext.hpp>
#include <SFML/Window/macOS/WindowImplCocoa.hpp>

#include <SFML/System/Err.hpp>

#include <dlfcn.h>
#include <ostream>

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

namespace sf::priv
{

////////////////////////////////////////////////////////////
SFContext::SFContext(SFContext* shared)
{
    const AutoreleasePool pool;
    // Create the context
    createContext(shared,
                  VideoMode::getDesktopMode().bitsPerPixel,
                  ContextSettings{0 /* depthBits */, 0 /* stencilBits */, 0 /* antiAliasingLevel */});
}


////////////////////////////////////////////////////////////
SFContext::SFContext(SFContext* shared, const ContextSettings& settings, const WindowImpl& owner, unsigned int bitsPerPixel)
{
    const AutoreleasePool pool;
    // Create the context.
    createContext(shared, bitsPerPixel, settings);

    // Apply context.
    const auto& ownerCocoa = static_cast<const WindowImplCocoa&>(owner);
    ownerCocoa.applyContext(m_context);
}


////////////////////////////////////////////////////////////
SFContext::SFContext(SFContext* shared, const ContextSettings& settings, Vector2u size)
{
    const AutoreleasePool pool;
    // Ensure the process is setup in order to create a valid window.
    WindowImplCocoa::setUpProcess();

    // Create the context.
    createContext(shared, VideoMode::getDesktopMode().bitsPerPixel, settings);

    // Create a dummy window/view pair (hidden) and assign it our context.
    m_window = [[NSWindow alloc]
        initWithContentRect:NSMakeRect(0, 0, size.x, size.y)
                  styleMask:NSBorderlessWindowMask
                    backing:NSBackingStoreBuffered
                      defer:NO]; // Don't defer it!
    m_view   = [[NSOpenGLView alloc] initWithFrame:NSMakeRect(0, 0, size.x, size.y)];
    [m_window setContentView:m_view];
    [m_view setOpenGLContext:m_context];
    [m_context setView:m_view];
}


////////////////////////////////////////////////////////////
SFContext::~SFContext()
{
    const AutoreleasePool pool;
    // Notify unshared OpenGL resources of context destruction
    cleanupUnsharedResources();

    [m_context clearDrawable];

    if (m_context == [NSOpenGLContext currentContext])
        [NSOpenGLContext clearCurrentContext];

    [m_context release];

    [m_view release];   // Might be nil but we don't care.
    [m_window release]; // Idem.
}


////////////////////////////////////////////////////////////
GlFunctionPointer SFContext::getFunction(const char* name)
{
    const AutoreleasePool pool;
    static void*          image = nullptr;

    if (!image)
        image = dlopen("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL", RTLD_LAZY);

    return image ? reinterpret_cast<GlFunctionPointer>(reinterpret_cast<intptr_t>(dlsym(image, name))) : nil;
}


////////////////////////////////////////////////////////////
bool SFContext::makeCurrent(bool current)
{
    const AutoreleasePool pool;
    if (current)
    {
        [m_context makeCurrentContext];
        return m_context == [NSOpenGLContext currentContext]; // Should be true.
    }

    [NSOpenGLContext clearCurrentContext];
    return m_context != [NSOpenGLContext currentContext]; // Should be true.
}


////////////////////////////////////////////////////////////
void SFContext::display()
{
    const AutoreleasePool pool;
    [m_context flushBuffer];
}


////////////////////////////////////////////////////////////
void SFContext::setVerticalSyncEnabled(bool enabled)
{
    const AutoreleasePool pool;
    const GLint           swapInterval = enabled;

    [m_context setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];
}


////////////////////////////////////////////////////////////
void SFContext::createContext(SFContext* shared, unsigned int bitsPerPixel, const ContextSettings& settings)
{
    const AutoreleasePool pool;
    // Save the settings. (OpenGL version is updated elsewhere.)
    m_settings = settings;

    // Choose the attributes of OGL context.
    std::vector<NSOpenGLPixelFormatAttribute> attrs;
    attrs.reserve(20); // max attributes (estimation).

    // These casts are safe. C++ is much more strict than Obj-C.

    attrs.push_back(NSOpenGLPFAClosestPolicy);
    attrs.push_back(NSOpenGLPFADoubleBuffer);

    if (bitsPerPixel > 24)
    {
        attrs.push_back(NSOpenGLPFAAlphaSize);
        attrs.push_back(static_cast<NSOpenGLPixelFormatAttribute>(8));
    }

    attrs.push_back(NSOpenGLPFADepthSize);
    attrs.push_back(static_cast<NSOpenGLPixelFormatAttribute>(m_settings.depthBits));

    attrs.push_back(NSOpenGLPFAStencilSize);
    attrs.push_back(static_cast<NSOpenGLPixelFormatAttribute>(m_settings.stencilBits));

    if (m_settings.antiAliasingLevel > 0)
    {
        /*
         * Antialiasing techniques are described in the
         * "OpenGL Programming Guide for Mac OS X" document.
         *
         * According to this document, the specification currently allows
         * only one multisample buffer.
         *
         * The document also states that software renderers should be avoided
         * because antialisaing techniques are very slow with them.
         */

        // Prefer multisampling over supersampling
        attrs.push_back(NSOpenGLPFAMultisample);

        // Only one buffer is currently available
        attrs.push_back(NSOpenGLPFASampleBuffers);
        attrs.push_back(static_cast<NSOpenGLPixelFormatAttribute>(1));

        // Antialiasing level
        attrs.push_back(NSOpenGLPFASamples);
        attrs.push_back(static_cast<NSOpenGLPixelFormatAttribute>(m_settings.antiAliasingLevel));

        // No software renderer - only hardware renderer
        attrs.push_back(NSOpenGLPFAAccelerated);
    }

    // Support for OpenGL 3.2 on Mac OS X Lion and later:
    // The SFML Graphics module uses some OpenGL features that are deprecated in
    // OpenGL 3.0 and that are no longer available in 3.1 and 3.2+ with a core context.
    // Therefore the Graphics module won't work as expected.

    // 1.x/2.x are mapped to 2.1 since Apple only support that legacy version.
    // >=3.0 are mapped to a 3.2 core profile.
    if (m_settings.majorVersion < 3)
    {
        m_settings.attributeFlags &= ~static_cast<unsigned int>(ContextSettings::Core);
        m_settings.majorVersion = 2;
        m_settings.minorVersion = 1;
        attrs.push_back(NSOpenGLPFAOpenGLProfile);
        attrs.push_back(NSOpenGLProfileVersionLegacy);
    }
    else
    {
        if (!(m_settings.attributeFlags & ContextSettings::Core))
        {
            err() << "Warning. Compatibility profile not supported on this platform." << std::endl;
            m_settings.attributeFlags |= ContextSettings::Core;
        }
        m_settings.majorVersion = 3;
        m_settings.minorVersion = 2;
        attrs.push_back(NSOpenGLPFAOpenGLProfile);
        attrs.push_back(NSOpenGLProfileVersion3_2Core);
    }

    if (m_settings.attributeFlags & ContextSettings::Debug)
    {
        err() << "Warning. OpenGL debugging not supported on this platform." << std::endl;
        m_settings.attributeFlags &= ~static_cast<unsigned int>(ContextSettings::Debug);
    }

    attrs.push_back(static_cast<NSOpenGLPixelFormatAttribute>(0)); // end of array

    // All macOS pixel formats are sRGB capable
    m_settings.sRgbCapable = true;

    // Create the pixel format.
    NSOpenGLPixelFormat* const pixFmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs.data()];

    if (pixFmt == nil)
    {
        err() << "Error. Unable to find a suitable pixel format." << std::endl;
        return;
    }

    // Use the shared context if one is given.
    NSOpenGLContext* const sharedContext = shared != nullptr ? shared->m_context : nil;

    if (sharedContext != nil)
    {
        [NSOpenGLContext clearCurrentContext];

        if (sharedContext == [NSOpenGLContext currentContext])
        {
            err() << "Failed to deactivate shared context before sharing" << std::endl;
            return;
        }
    }

    // Create the context.
    m_context = [[NSOpenGLContext alloc] initWithFormat:pixFmt shareContext:sharedContext];

    if (m_context == nil)
    {
        err() << "Error. Unable to create the context. Retrying without shared context." << std::endl;
        m_context = [[NSOpenGLContext alloc] initWithFormat:pixFmt shareContext:nil];

        if (m_context == nil)
            err() << "Error. Unable to create the context." << std::endl;
        else
            err() << "Warning. New context created without shared context." << std::endl;
    }

    // Free up.
    [pixFmt release];
}

} // namespace sf::priv
