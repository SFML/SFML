////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Marco Antognini (antognini.marco@gmail.com), 
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
#include <SFML/Window/VideoModeImpl.hpp>
#include <SFML/Window/OSX/cg_sf_conversion.hpp>
#include <SFML/System/Err.hpp>
#include <algorithm>

namespace sf
{
namespace priv
{    
////////////////////////////////////////////////////////////
/// Note : 
///     Starting with 10.6, CGDisplayModeRef and CGDisplayCopyAllDisplayModes
///     should be used instead of CFDictionaryRef and CGDisplayAvailableModes.
///
////////////////////////////////////////////////////////////
std::vector<VideoMode> VideoModeImpl::getFullscreenModes()
{
#if MAC_OS_X_VERSION_MAX_ALLOWED < 1060
    
    std::vector<VideoMode> modes;
    
    // Retrieve array of dictionaries representing display modes.
    CFArrayRef displayModes = CGDisplayAvailableModes(CGMainDisplayID());
    
    if (displayModes == NULL) {
        sf::err() << "Couldn't get VideoMode for main display.";
        return modes;
    }
    
    // Loop on each mode and convert it into a sf::VideoMode object.
    CFIndex const modesCount = CFArrayGetCount(displayModes);
    for (CFIndex i = 0; i < modesCount; i++) {
        CFDictionaryRef dictionary = (CFDictionaryRef)CFArrayGetValueAtIndex(displayModes, i);
        
        VideoMode mode = convertCGModeToSFMode(dictionary);
        
        // If not yet listed we add it to our modes array.
        if (std::find(modes.begin(), modes.end(), mode) == modes.end()) {
            modes.push_back(mode);
        }
    }
    
    return modes;
    
#else // MAC_OS_X_VERSION_MAX_ALLOWED >= 1060
    
    std::vector<VideoMode> modes;
    
    // Retrieve all modes available for main screen only.
    CFArrayRef cgmodes = CGDisplayCopyAllDisplayModes(CGMainDisplayID(), NULL);
    
    if (cgmodes == NULL) {
        sf::err() << "Couldn't get VideoMode for main display.";
        return modes;
    }
    
    // Loop on each mode and convert it into a sf::VideoMode object.
    CFIndex const modesCount = CFArrayGetCount(cgmodes);
    for (CFIndex i = 0; i < modesCount; i++) {
        CGDisplayModeRef cgmode = (CGDisplayModeRef)CFArrayGetValueAtIndex(cgmodes, i);
        
        VideoMode mode = convertCGModeToSFMode(cgmode);
        
        // If not yet listed we add it to our modes array.
        if (std::find(modes.begin(), modes.end(), mode) == modes.end()) {
            modes.push_back(mode);
        }
    }
    
    // Clean up memory.
    CFRelease(cgmodes);
    
    return modes;
    
#endif
}


////////////////////////////////////////////////////////////
VideoMode VideoModeImpl::getDesktopMode()
{
    CGDirectDisplayID display = CGMainDisplayID();
    return VideoMode(CGDisplayPixelsWide(display), 
                     CGDisplayPixelsHigh(display), 
                     displayBitsPerPixel(display));
}

} // namespace priv

} // namespace sf
