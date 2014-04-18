////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Marco Antognini (antognini.marco@gmail.com),
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
size_t modeBitsPerPixel(CGDisplayModeRef mode)
{
    size_t bpp = 0; // no match

    // Compare encoding.
    CFStringRef pixEnc = CGDisplayModeCopyPixelEncoding(mode);
    if(CFStringCompare(pixEnc, CFSTR(IO32BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
        bpp = 32;
    else if(CFStringCompare(pixEnc, CFSTR(IO16BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
        bpp = 16;
    else if(CFStringCompare(pixEnc, CFSTR(IO8BitIndexedPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
        bpp = 8;

    // Clean up memory.
    CFRelease(pixEnc);

    return bpp;
}


////////////////////////////////////////////////////////////
size_t displayBitsPerPixel(CGDirectDisplayID displayId)
{
    // Get the display mode.
    CGDisplayModeRef mode = CGDisplayCopyDisplayMode(displayId);

    // Get bpp for the mode.
    const size_t bpp = modeBitsPerPixel(mode);

    // Clean up Memory.
    CGDisplayModeRelease(mode);

    return bpp;
}


////////////////////////////////////////////////////////////
VideoMode convertCGModeToSFMode(CGDisplayModeRef cgmode)
{
    return VideoMode(CGDisplayModeGetWidth(cgmode),
                     CGDisplayModeGetHeight(cgmode),
                     modeBitsPerPixel(cgmode));
}


////////////////////////////////////////////////////////////
CGDisplayModeRef convertSFModeToCGMode(VideoMode sfmode)
{
    // Starting with 10.6 we should query the display all the modes and
    // search for the best one.

    // Will return NULL if sfmode is not in VideoMode::GetFullscreenModes.
    CGDisplayModeRef cgbestMode = NULL;

    // Retrieve all modes available for main screen only.
    CFArrayRef cgmodes = CGDisplayCopyAllDisplayModes(CGMainDisplayID(), NULL);

    if (cgmodes == NULL) // Should not happen but anyway...
    {
        sf::err() << "Couldn't get VideoMode for main display.";
        return NULL;
    }

    // Loop on each mode and convert it into a sf::VideoMode object.
    const CFIndex modesCount = CFArrayGetCount(cgmodes);
    for (CFIndex i = 0; i < modesCount; i++)
    {
        CGDisplayModeRef cgmode = (CGDisplayModeRef)CFArrayGetValueAtIndex(cgmodes, i);

        VideoMode mode = convertCGModeToSFMode(cgmode);

        if (mode == sfmode)
            cgbestMode = cgmode;
    }

    // Clean up memory.
    CFRelease(cgmodes);

    if (cgbestMode == NULL)
        sf::err() << "Couldn't convert the given sf:VideoMode into a CGDisplayMode."
                  << std::endl;

    return cgbestMode;
}

} // namespace priv
} // namespace sf
