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
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/OSX/DisplayImpl.hpp>
#include <SFML/Window/OSX/cg_sf_conversion.hpp>
#include <SFML/System/Err.hpp>
#include <algorithm>
#include <functional>

////////////////////////////////////////////////////////////
// Private Data
////////////////////////////////////////////////////////////
namespace
{

struct find_NativeDisplayMode
: public std::unary_function<sf::priv::DisplayImpl::NativeDisplayMode, bool>
{
    sf::VideoMode mode;
    
    bool operator () (sf::priv::DisplayImpl::NativeDisplayMode const& value)
    {
        return mode == value.mode;
    }
};
    
} // namespace


namespace sf
{
namespace priv
{

////////////////////////////////////////////////////////////
DisplayImpl::DisplayImpl(CGDirectDisplayID display)
{
    m_display = display;
    NativeDisplayMode displayMode;
    find_NativeDisplayMode findDisplayMode;

#if MAC_OS_X_VERSION_MAX_ALLOWED < 1060
    
    // Retrieve array of dictionaries representing display modes.
    CFArrayRef displayModes = CGDisplayAvailableModes(CGMainDisplayID());
    
    if (displayModes == NULL) {
        sf::err() << "Couldn't get VideoMode for main display." << std::endl;
    }
    
    // Loop on each mode and convert it into a sf::VideoMode object.
    CFIndex const modesCount = CFArrayGetCount(displayModes);
    for (CFIndex i = 0; i < modesCount; i++) {
        CFDictionaryRef dictionary = (CFDictionaryRef)CFArrayGetValueAtIndex(displayModes, i);
        
        VideoMode mode = convertCGModeToSFMode(dictionary);

        displayMode.mode = convertCGModeToSFMode(cgmode);
        displayMode.ref = CGDisplayModeRetain(cgmode);
        
        findDisplayMode.mode = mode;
        
        // If not yet listed we add it to our modes array.
        if (std::find_if(m_modes.begin(), m_modes.end(), findDisplayMode) == m_modes.end()) {
            m_modes.push_back(displayMode);
        }
    }
    
#else // MAC_OS_X_VERSION_MAX_ALLOWED >= 1060
    
    // Retrieve all modes available for main screen only.
    CFArrayRef cgmodes = CGDisplayCopyAllDisplayModes(CGMainDisplayID(), NULL);
    
    if (cgmodes == NULL) {
        sf::err() << "Couldn't get VideoMode for main display." << std::endl;
    }
    
    // Loop on each mode and convert it into a sf::VideoMode object.
    CFIndex const modesCount = CFArrayGetCount(cgmodes);
    for (CFIndex i = 0; i < modesCount; i++) {
        CGDisplayModeRef cgmode = (CGDisplayModeRef)CFArrayGetValueAtIndex(cgmodes, i);
        
        VideoMode mode = convertCGModeToSFMode(cgmode);
        
        displayMode.mode = convertCGModeToSFMode(cgmode);
        displayMode.ref = CGDisplayModeRetain(cgmode);

        findDisplayMode.mode = mode;
        
        // If not yet listed we add it to our modes array.
        if (std::find_if(m_modes.begin(), m_modes.end(), findDisplayMode) == m_modes.end()) {
            m_modes.push_back(displayMode);
        }
    }
    
    // Clean up memory.
    CFRelease(cgmodes);
    
#endif
}

////////////////////////////////////////////////////////////
DisplayImpl::~DisplayImpl()
{
    std::vector<NativeDisplayMode>::iterator it;
    
    for ( it = m_modes.begin();
         it != m_modes.end();
         it++)
    {
        CGDisplayModeRelease(it->ref);
        it->ref = NULL;
    }
}

////////////////////////////////////////////////////////////
std::vector<VideoMode> DisplayImpl::getModes() {
    std::vector<VideoMode> modes;
    std::vector<NativeDisplayMode>::iterator it;

    for ( it = m_modes.begin();
         it != m_modes.end();
         it++)
    {
        modes.push_back(it->mode);
    }
    
    return modes;
}

////////////////////////////////////////////////////////////
bool DisplayImpl::setMode(VideoMode const& mode) {
    find_NativeDisplayMode findMode;
    std::vector<NativeDisplayMode>::iterator it;
    CGError error;
    
    findMode.mode = mode;
    it = std::find_if(m_modes.begin(), m_modes.end(), findMode);
    
    if (it == m_modes.end()) {
        sf::err() << "Mode unavailable.";
        return false;
    }
    
    CGDisplayConfigRef config;
    
    CGBeginDisplayConfiguration(&config);
    
    error = CGConfigureDisplayWithDisplayMode(config, m_display, it->ref, NULL);

    if (error != kCGErrorSuccess) {
        sf::err() << "Failed to configure display.";
        return false;
    }
    
    error = CGCompleteDisplayConfiguration(config, kCGConfigureForAppOnly);
    
    if (error != kCGErrorSuccess) {
        sf::err() << "Failed to set video mode.";
        return false;
    }
    
    return true;
}

} // namespace priv
    
} // namespace sf
