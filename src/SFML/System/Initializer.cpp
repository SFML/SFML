////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2008 Lucas Soltic (elmerod@gmail.com) and Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Config.hpp>


#ifdef SFML_SYSTEM_MACOS

#include <CoreFoundation/CoreFoundation.h>
#include <iostream>

namespace
{

////////////////////////////////////////////////////////////
/// Under Mac OS X, when launching an application from the Finder,
/// the default working directory is the user home directory ;
/// when launching from Xcode, the default one is the directory
/// containing the application. In order to produce a uniform behaviour
/// and simplify the use of resources, SFML sets the working directory to
/// the Resources folder of the application bundle.
/// The "constructor" attribute forces the function to be called
/// at library loading time.
////////////////////////////////////////////////////////////
	
void working_directory_initializer(void) __attribute__ ((constructor));
void working_directory_initializer(void)
{
	char pathBuffer[4096];
	
	// Get the application bundle
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	
	if (!mainBundle)
	{
		std::cerr << "*** SFML: error getting the application main bundle" << std::endl;
		return;
	}
	
	// Get the resource directory URL
	CFURLRef resourceDirectory = CFBundleCopyResourcesDirectoryURL(mainBundle);
	
	if (!resourceDirectory)
	{
		std::cerr << "*** SFML: error getting the resource directory of the main bundle" << std::endl;
		return;
	}
	
	// Convert it as absolute URL
	CFURLRef absoluteURL = CFURLCopyAbsoluteURL(resourceDirectory);
	
	if (!absoluteURL)
	{
		std::cerr << "*** SFML: error gettint the resource directory as an absolute URL" << std::endl;
		CFRelease(resourceDirectory);
		return;
	}
	
	// Get the POSIX style path
	CFStringRef absolutePath = CFURLCopyFileSystemPath(absoluteURL, kCFURLPOSIXPathStyle);
	
	if (!absolutePath)
	{
		std::cerr << "*** SFML: error converting the resource directory URL as a POSIX path" << std::endl;
		CFRelease(absoluteURL);
		CFRelease(resourceDirectory);
		return;
	}
	
	// Get the path as C string and set it
	if (CFStringGetCString(absolutePath, pathBuffer, 4096, kCFStringEncodingASCII))
		chdir(pathBuffer);
	else
		std::cerr << "*** SFML: error copying the resource directory path in the C buffer" << std::endl;
	
	CFRelease(absolutePath);
	CFRelease(absoluteURL);
	CFRelease(resourceDirectory);
}

} // anonymous namespace

#endif // SFML_SYSTEM_MACOS

