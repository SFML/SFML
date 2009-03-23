////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Window/Cocoa/VideoModeSupport.hpp>
#include <ApplicationServices/ApplicationServices.h>
#include <algorithm>

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Get supported video modes
////////////////////////////////////////////////////////////
void VideoModeSupport::GetSupportedVideoModes(std::vector<VideoMode>& Modes)
{
	// Ceylo -- using same implementation as in OSXCarbon
	
    // First, clear array to fill
    Modes.clear();
	
    // Enumerate all available video modes for primary display adapter
    CFArrayRef DisplayModes = CGDisplayAvailableModes( kCGDirectMainDisplay );
    CFIndex DisplayModeCount = CFArrayGetCount( DisplayModes );
    CFDictionaryRef CurrentMode;
	
    for (int Count = 0; Count < DisplayModeCount; ++Count)
    {
        CurrentMode = (CFDictionaryRef)CFArrayGetValueAtIndex( DisplayModes, Count );
		
		VideoMode Mode;
		
        CFNumberGetValue((CFNumberRef)CFDictionaryGetValue(CurrentMode, kCGDisplayWidth), kCFNumberIntType, &(Mode.Width));
        CFNumberGetValue((CFNumberRef)CFDictionaryGetValue(CurrentMode, kCGDisplayHeight), kCFNumberIntType, &(Mode.Height));
        CFNumberGetValue((CFNumberRef)CFDictionaryGetValue(CurrentMode, kCGDisplayBitsPerPixel), kCFNumberIntType, &(Mode.BitsPerPixel));
		
        // Add it only if it is not already in the array
        if (std::find(Modes.begin(), Modes.end(), Mode) == Modes.end())
            Modes.push_back(Mode);
    }
}


////////////////////////////////////////////////////////////
/// Get current desktop video mode
////////////////////////////////////////////////////////////
VideoMode VideoModeSupport::GetDesktopVideoMode()
{
	// Ceylo -- using same implementation as in OSXCarbon
	
	CFDictionaryRef CurrentVideoMode = CGDisplayCurrentMode(kCGDirectMainDisplay);
	
	VideoMode DesktopMode;
	
	
	// Get video mode width
    CFNumberGetValue((CFNumberRef)CFDictionaryGetValue(CurrentVideoMode, kCGDisplayWidth),
					 kCFNumberIntType,
					 &(DesktopMode.Width));
	
	// Get video mode height
    CFNumberGetValue((CFNumberRef)CFDictionaryGetValue(CurrentVideoMode, kCGDisplayHeight),
					 kCFNumberIntType,
					 &(DesktopMode.Height));
	
	// Get video mode depth
    CFNumberGetValue((CFNumberRef)CFDictionaryGetValue(CurrentVideoMode, kCGDisplayBitsPerPixel),
					 kCFNumberIntType,
					 &(DesktopMode.BitsPerPixel));
	
	
	return DesktopMode;
}

} // namespace priv

} // namespace sf
