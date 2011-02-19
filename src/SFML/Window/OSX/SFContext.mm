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

/*
 * DISCUSSION :
 * ============
 * 
 * [1] (2010_07)    
 *     should AA-related NSOpenGLPixelFormatAttribute not be in the array 
 *     if AA is not enable (settings.AntialiasingLevel == 0) ?
 *     => will not be present in attributs array if 0.
 *
 * [2] (2010_07)    
 *     how many buffer should be used for AA ?
 *     => «1» was choosen.
 */

namespace sf
{
namespace priv
{

////////////////////////////////////////////////////////////
SFContext::SFContext(SFContext* shared)
{
    myPool = [[NSAutoreleasePool alloc] init];
    
    // Create the context
    CreateContext(shared, 0, ContextSettings(0, 0, 0));
    
    // Activate the context
    SetActive(true);
}

    
////////////////////////////////////////////////////////////
SFContext::SFContext(SFContext* shared, const WindowImpl* owner, 
                     unsigned int bitsPerPixel, const ContextSettings& settings)
{
    myPool = [[NSAutoreleasePool alloc] init];
    
    // Create the context.
    CreateContext(shared, bitsPerPixel, settings);
    
    // Apply context.
    WindowImplCocoa const * ownerCocoa = static_cast<WindowImplCocoa const *>(owner);
    ownerCocoa->ApplyContext(myContext);
    
    // Activate the context
    SetActive(true);
}

    
////////////////////////////////////////////////////////////
SFContext::~SFContext()
{
    [myContext release];
    [myPool drain]; // [A]
    
    /*
     * [A] : Produce sometimes "*** attempt to pop an unknown autorelease pool"
     */
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
    
    if (settings.AntialiasingLevel > 0) { // [1]
        attrs.push_back(NSOpenGLPFAMultisample);
        
        attrs.push_back(NSOpenGLPFASampleBuffers);
        attrs.push_back((NSOpenGLPixelFormatAttribute)1); // [2]
        
        attrs.push_back(NSOpenGLPFASamples);
        attrs.push_back((NSOpenGLPixelFormatAttribute)settings.AntialiasingLevel);
    }
    
    attrs.push_back((NSOpenGLPixelFormatAttribute)0); // end of array
    
    // Create the pixel pormat.
    NSOpenGLPixelFormat* pixFmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:&attrs[0]];
    
    if(pixFmt == nil) {
        sf::Err() << "Error. Unable to find a suitable pixel format." << std::endl;
        return;
    }
    
    // Use the shared context if one is given.
    NSOpenGLContext* sharedContext = shared != NULL ? shared->myContext : nil;
    
    // Create the context.
    myContext = [[NSOpenGLContext alloc] initWithFormat:pixFmt
                                           shareContext:sharedContext];
    
    // Free up.
    [pixFmt release];
    
#warning update settings with ogl version not yet implemented
    
    // Save the creation settings
    mySettings = settings;
}
    
} // namespace priv
    
} // namespace sf

