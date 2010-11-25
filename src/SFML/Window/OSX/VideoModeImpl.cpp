

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
