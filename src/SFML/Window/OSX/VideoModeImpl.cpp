////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2010 Marco Antognini (antognini.marco@gmail.com), 
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
#include <SFML/System/Err.hpp>
#include <algorithm>

#import <ApplicationServices/ApplicationServices.h>

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Get bpp for all OS X version
/// 
/// This function use only non-deprecated way to get the
/// display bits per pixel information. It depends
/// on these macros :
///
/// - USE_OS_X_VERSION_10_4
/// - USE_OS_X_VERSION_10_6
///
#warning This may be improved by removing these dummy macros.
/// Maybe MAC_OS_X_VERSION_MAX_ALLOWED ?
////////////////////////////////////////////////////////////
size_t DisplayBitsPerPixel(CGDirectDisplayID displayId)
{
#if MAC_OS_X_VERSION_MAX_ALLOWED < 1060
	
	return CGDisplayBitsPerPixel(displayId);

#else // MAC_OS_X_VERSION_MAX_ALLOWED >= 1060
	
	CGDisplayModeRef mode = CGDisplayCopyDisplayMode(displayId);

	CFStringRef pixEnc = CGDisplayModeCopyPixelEncoding(mode);
	if(CFStringCompare(pixEnc, CFSTR(IO32BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
		return 32;
	else if(CFStringCompare(pixEnc, CFSTR(IO16BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
		return 16;
	else if(CFStringCompare(pixEnc, CFSTR(IO8BitIndexedPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
		return 8;

	return 0; // no match
	
#endif
}

	
////////////////////////////////////////////////////////////
std::vector<VideoMode> VideoModeImpl::GetFullscreenModes()
{
	std::vector<VideoMode> modes;
	
	CGDisplayCount count = 0;
	int err = CGGetActiveDisplayList(0, NULL, &count);
	
	if (err != 0) {
		sf::Err() << "Error when retrieving displays count";
		return modes;
	}
	
	CGDirectDisplayID* displays = new CGDirectDisplayID[count];
	err = CGGetActiveDisplayList(count, displays, &count);
	
	if (err != 0) {
		sf::Err() << "Error when retrieving displays array";
		return modes;
	}
	
	for (int i = 0; i < count; ++i) {		
		VideoMode mode(CGDisplayPixelsWide(displays[i]), 
									 CGDisplayPixelsHigh(displays[i]), 
									 DisplayBitsPerPixel(displays[i]));
		
		// Add it only if it isn't already in the array.
		if (std::find(modes.begin(), modes.end(), mode) == modes.end())
			modes.push_back(mode);
	}
	
	delete[] displays;
	
	return modes;
}


////////////////////////////////////////////////////////////
VideoMode VideoModeImpl::GetDesktopMode()
{
	CGDirectDisplayID display = CGMainDisplayID();
	return VideoMode(CGDisplayPixelsWide(display), 
									 CGDisplayPixelsHigh(display), 
									 DisplayBitsPerPixel(display));
}

} // namespace priv

} // namespace sf
