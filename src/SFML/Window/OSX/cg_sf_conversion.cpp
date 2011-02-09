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
#include <SFML/Window/OSX/cg_sf_conversion.hpp>
#include <SFML/System/Err.hpp>

namespace sf
{
namespace priv
{

////////////////////////////////////////////////////////////
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1060
size_t ModeBitsPerPixel(CGDisplayModeRef mode)
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
size_t DisplayBitsPerPixel(CGDirectDisplayID displayId)
{
#if MAC_OS_X_VERSION_MAX_ALLOWED < 1060
    
    return CGDisplayBitsPerPixel(displayId);
    
#else // MAC_OS_X_VERSION_MAX_ALLOWED >= 1060
    
    // Get the display mode.
    CGDisplayModeRef mode = CGDisplayCopyDisplayMode(displayId);
    
    // Get bpp for the mode.
    size_t const bpp = ModeBitsPerPixel(mode);
    
    // Clean up Memory.
    CGDisplayModeRelease(mode);
    
    return bpp;
    
#endif
}


////////////////////////////////////////////////////////////
#if MAC_OS_X_VERSION_MAX_ALLOWED < 1060

VideoMode ConvertCGModeToSFMode(CFDictionaryRef dictionary)
{
    VideoMode sfmode;
    
    CFNumberRef cfnumber = (CFNumberRef)CFDictionaryGetValue(CurrentMode, kCGDisplayWidth);
    CFNumberGetValue(cfnumber, kCFNumberIntType, &(mode.Width));
    
    cfnumber = (CFNumberRef)CFDictionaryGetValue(CurrentMode, kCGDisplayHeight);
    CFNumberGetValue(cfnumber, kCFNumberIntType, &(mode.Height));
    
    cfnumber = (CFNumberRef)CFDictionaryGetValue(CurrentMode, kCGDisplayBitsPerPixel);
    CFNumberGetValue(cfnumber, kCFNumberIntType, &(mode.BitsPerPixel));
    
    return sfmode;
}

#else // MAC_OS_X_VERSION_MAX_ALLOWED >= 1060

VideoMode ConvertCGModeToSFMode(CGDisplayModeRef cgmode)
{
    return VideoMode(CGDisplayModeGetWidth(cgmode),
                     CGDisplayModeGetHeight(cgmode),
                     ModeBitsPerPixel(cgmode));
}

#endif
    
////////////////////////////////////////////////////////////
#if MAC_OS_X_VERSION_MAX_ALLOWED < 1060

CFDictionaryRef ConvertSFModeToCGMode(VideoMode sfmode)
{
    // If sfmode is in VideoMode::GetFullscreenModes
    // then this should be an exact match (see NULL parameter doc).
    return CGDisplayBestModeForParameters(CGMainDisplayID(),
                                          sfmode.BitsPerPixel,
                                          sfmode.Width,
                                          sfmode.Height
                                          NULL);
}

#else // MAC_OS_X_VERSION_MAX_ALLOWED >= 1060

CGDisplayModeRef ConvertSFModeToCGMode(VideoMode sfmode)
{
    // Starting with 10.6 we should query the display all the modes and
    // search for the best one.
    
    // Will return NULL if sfmode is not in VideoMode::GetFullscreenModes.
    CGDisplayModeRef cgbestMode = NULL;
    
    // Retrieve all modes available for main screen only.
    CFArrayRef cgmodes = CGDisplayCopyAllDisplayModes(CGMainDisplayID(), NULL);
    
    if (cgmodes == NULL) { // Should not happen but anyway...
        sf::Err() << "Couldn't get VideoMode for main display.";
        return NULL;
    }
    
    // Loop on each mode and convert it into a sf::VideoMode object.
    CFIndex const modesCount = CFArrayGetCount(cgmodes);
    for (CFIndex i = 0; i < modesCount; i++) {
        CGDisplayModeRef cgmode = (CGDisplayModeRef)CFArrayGetValueAtIndex(cgmodes, i);
        
        VideoMode mode = ConvertCGModeToSFMode(cgmode);
        
        if (mode == sfmode) {
            cgbestMode = cgmode;
        }
    }
    
    // Clean up memory.
    CFRelease(cgmodes);
    
    if (cgbestMode == NULL) {
        sf::Err()
        << "Couldn't convert the given sf:VideoMode into a CGDisplayMode."
        << std::endl;
    }
    
    return cgbestMode;
}

#endif

} // namespace priv
} // namespace sf
