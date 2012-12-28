////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2012 Marco Antognini (antognini.marco@gmail.com), 
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
#include <SFML/Window/OSX/cg_sf_conversion.hpp>
#include <SFML/System/Err.hpp>

namespace sf
{
namespace priv
{

////////////////////////////////////////////////////////////
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1060
size_t modeBitsPerPixel(CGDisplayModeRef mode)
{
    size_t bpp = 0; // no match
    
    // Compare encoding.
    CFStringRef pixEnc = CGDisplayModeCopyPixelEncoding(mode);
    if(CFStringCompare(pixEnc, CFSTR(IO32BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo) {
        
        bpp = 32;
        
    } else if(CFStringCompare(pixEnc, CFSTR(IO16BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo) {
        
        bpp = 16;
        
    } else if(CFStringCompare(pixEnc, CFSTR(IO8BitIndexedPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo) {
        
        bpp = 8;
        
    }
    
    // Clean up memory.
    CFRelease(pixEnc);
    
    return bpp;
}
#endif



////////////////////////////////////////////////////////////
size_t displayBitsPerPixel(CGDirectDisplayID displayId)
{
#if MAC_OS_X_VERSION_MAX_ALLOWED < 1060
    
    return CGDisplayBitsPerPixel(displayId);
    
#else // MAC_OS_X_VERSION_MAX_ALLOWED >= 1060
    
    // Get the display mode.
    CGDisplayModeRef mode = CGDisplayCopyDisplayMode(displayId);
    
    // Get bpp for the mode.
    size_t const bpp = modeBitsPerPixel(mode);
    
    // Clean up Memory.
    CGDisplayModeRelease(mode);
    
    return bpp;
    
#endif
}


////////////////////////////////////////////////////////////
#if MAC_OS_X_VERSION_MAX_ALLOWED < 1060

VideoMode convertCGModeToSFMode(CFDictionaryRef dictionary)
{
    VideoMode sfmode;
    
    CFNumberRef cfnumber = (CFNumberRef)CFDictionaryGetValue(dictionary, kCGDisplayWidth);
    CFNumberGetValue(cfnumber, kCFNumberIntType, &(sfmode.width));
    
    cfnumber = (CFNumberRef)CFDictionaryGetValue(dictionary, kCGDisplayHeight);
    CFNumberGetValue(cfnumber, kCFNumberIntType, &(sfmode.height));
    
    cfnumber = (CFNumberRef)CFDictionaryGetValue(dictionary, kCGDisplayBitsPerPixel);
    CFNumberGetValue(cfnumber, kCFNumberIntType, &(sfmode.bitsPerPixel));
    
    return sfmode;
}

#else // MAC_OS_X_VERSION_MAX_ALLOWED >= 1060

VideoMode convertCGModeToSFMode(CGDisplayModeRef cgmode)
{
    return VideoMode(CGDisplayModeGetWidth(cgmode),
                     CGDisplayModeGetHeight(cgmode),
                     modeBitsPerPixel(cgmode));
}

#endif
    
////////////////////////////////////////////////////////////
#if MAC_OS_X_VERSION_MAX_ALLOWED < 1060

CFDictionaryRef convertSFModeToCGMode(VideoMode sfmode)
{
    // If sfmode is in VideoMode::GetFullscreenModes
    // then this should be an exact match (see NULL parameter doc).
    return CGDisplayBestModeForParameters(CGMainDisplayID(),
                                          sfmode.bitsPerPixel,
                                          sfmode.width,
                                          sfmode.height,
                                          NULL);
}

#else // MAC_OS_X_VERSION_MAX_ALLOWED >= 1060

CGDisplayModeRef convertSFModeToCGMode(VideoMode sfmode)
{
    // Starting with 10.6 we should query the display all the modes and
    // search for the best one.
    
    // Will return NULL if sfmode is not in VideoMode::GetFullscreenModes.
    CGDisplayModeRef cgbestMode = NULL;
    
    // Retrieve all modes available for main screen only.
    CFArrayRef cgmodes = CGDisplayCopyAllDisplayModes(CGMainDisplayID(), NULL);
    
    if (cgmodes == NULL) { // Should not happen but anyway...
        sf::err() << "Couldn't get VideoMode for main display.";
        return NULL;
    }
    
    // Loop on each mode and convert it into a sf::VideoMode object.
    CFIndex const modesCount = CFArrayGetCount(cgmodes);
    for (CFIndex i = 0; i < modesCount; i++) {
        CGDisplayModeRef cgmode = (CGDisplayModeRef)CFArrayGetValueAtIndex(cgmodes, i);
        
        VideoMode mode = convertCGModeToSFMode(cgmode);
        
        if (mode == sfmode) {
            cgbestMode = cgmode;
        }
    }
    
    // Clean up memory.
    CFRelease(cgmodes);
    
    if (cgbestMode == NULL) {
        sf::err()
        << "Couldn't convert the given sf:VideoMode into a CGDisplayMode."
        << std::endl;
    }
    
    return cgbestMode;
}

#endif

} // namespace priv
} // namespace sf
