////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2011 Marco Antognini (antognini.marco@gmail.com), 
//                         Laurent Gomila (laurent.gom@gmail.com), 
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
#include <SFML/Window/OSX/SFContext.hpp>
#include <SFML/Window/OSX/WindowImplCocoa.hpp>
#include <SFML/System/Err.hpp>

#import <SFML/Window/OSX/AutoreleasePoolWrapper.h>

namespace sf
{
namespace priv
{

////////////////////////////////////////////////////////////
SFContext::SFContext(SFContext* shared)
: myView(0), myWindow(0)
{
    // Ask for a pool.
    RetainPool();
    
    // Create the context
    CreateContext(shared,
                  VideoMode::GetDesktopMode().BitsPerPixel, 
                  ContextSettings(0, 0, 0));
}

    
////////////////////////////////////////////////////////////
SFContext::SFContext(SFContext* shared, const ContextSettings& settings,
                     const WindowImpl* owner, unsigned int bitsPerPixel)
: myView(0), myWindow(0)
{
    // Ask for a pool.
    RetainPool();
    
    // Create the context.
    CreateContext(shared, bitsPerPixel, settings);
    
    // Apply context.
    WindowImplCocoa const * ownerCocoa = static_cast<WindowImplCocoa const *>(owner);
    ownerCocoa->ApplyContext(myContext);
}


SFContext::SFContext(SFContext* shared, const ContextSettings& settings, 
                     unsigned int width, unsigned int height)
: myView(0), myWindow(0)
{
    // Ensure the process is setup in order to create a valid window.
    WindowImplCocoa::SetUpProcess();
    
    // Ask for a pool.
    RetainPool();
    
    // Create the context.
    CreateContext(shared, VideoMode::GetDesktopMode().BitsPerPixel, settings);
    
    // Create a dummy window/view pair (hidden) and asign it our context.
    myWindow = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, width, height)
                                           styleMask:NSBorderlessWindowMask
                                             backing:NSBackingStoreBuffered
                                               defer:NO]; // Don't defer it!
    myView = [[NSOpenGLView alloc] initWithFrame:NSMakeRect(0, 0, width, height)];
    [myWindow setContentView:myView];
    [myView setOpenGLContext:myContext];
    [myContext setView:myView];
}


////////////////////////////////////////////////////////////
SFContext::~SFContext()
{
    [myContext clearDrawable];
    [myContext release];
    
    [myView release]; // Might be nil but we don't care.
    [myWindow release]; // Idem.
    
    ReleasePool();
}


////////////////////////////////////////////////////////////
bool SFContext::MakeCurrent()
{
    [myContext makeCurrentContext];
    return myContext == [NSOpenGLContext currentContext]; // Should be true.
}


////////////////////////////////////////////////////////////
void SFContext::Display()
{
    [myContext flushBuffer];
}


////////////////////////////////////////////////////////////
void SFContext::EnableVerticalSync(bool enabled)
{
    // Make compiler happy
#if MAC_OS_X_VERSION_MAX_ALLOWED < 1060
    typedef int GLint;
#endif
    
    GLint swapInterval = enabled ? 1 : 0;
    
    [myContext setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];
}


////////////////////////////////////////////////////////////
void SFContext::CreateContext(SFContext* shared,
                              unsigned int bitsPerPixel, 
                              const ContextSettings& settings)
{
    // Choose the attributs of OGL context.
    std::vector<NSOpenGLPixelFormatAttribute> attrs;
    attrs.reserve(20); // max attributs (estimation).
    
    // These casts are safe. C++ is much more strict than Obj-C.
    
    attrs.push_back(NSOpenGLPFAClosestPolicy);
    attrs.push_back(NSOpenGLPFADoubleBuffer);
    
    if (bitsPerPixel > 24) {
        attrs.push_back(NSOpenGLPFAAlphaSize);
        attrs.push_back((NSOpenGLPixelFormatAttribute)8);
    }
    
    attrs.push_back(NSOpenGLPFADepthSize);
    attrs.push_back((NSOpenGLPixelFormatAttribute)settings.DepthBits);
    
    attrs.push_back(NSOpenGLPFAStencilSize);
    attrs.push_back((NSOpenGLPixelFormatAttribute)settings.StencilBits);
    
    if (settings.AntialiasingLevel > 0) {
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
        
        // Only one buffer is currently available.
        attrs.push_back(NSOpenGLPFASampleBuffers);
        attrs.push_back((NSOpenGLPixelFormatAttribute)1);
        
        // Antialiasing level
        attrs.push_back(NSOpenGLPFASamples);
        attrs.push_back((NSOpenGLPixelFormatAttribute)settings.AntialiasingLevel);
        
        // No software renderer
        attrs.push_back(NSOpenGLPFANoRecovery);
    }
    
    attrs.push_back((NSOpenGLPixelFormatAttribute)0); // end of array
    
    // Create the pixel pormat.
    NSOpenGLPixelFormat* pixFmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:&attrs[0]];
    
    if (pixFmt == nil) {
        sf::Err() << "Error. Unable to find a suitable pixel format." << std::endl;
        return;
    }
    
    // Use the shared context if one is given.
    NSOpenGLContext* sharedContext = shared != NULL ? shared->myContext : nil;
    
    // Create the context.
    myContext = [[NSOpenGLContext alloc] initWithFormat:pixFmt
                                           shareContext:sharedContext];
    
    if (myContext == nil) {
        sf::Err() << "Error. Unable to create the context." << std::endl;
    }
    
    // Free up.
    [pixFmt release];
    
    // Save the settings. (OpenGL version is updated elsewhere.)
    mySettings = settings;
}

} // namespace priv

} // namespace sf

