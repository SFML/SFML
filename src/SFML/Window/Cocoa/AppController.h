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
#import <SFML/Window/Cocoa/WindowImplCocoa.hpp>
#import <SFML/Window/VideoMode.hpp>
#import <SFML/System/Clock.hpp>
#import <Cocoa/Cocoa.h>
#import <vector>
#import <iostream>


#define SharedAppController [AppController sharedController]

// Fade operations
enum {
	FillScreen,
	CleanScreen
};


@interface AppController : NSObject {
	// Note: objc allocation doesn't call C++ constructor
	std::vector <sf::priv::WindowImplCocoa *> *windows;
	
	NSAutoreleasePool *mainPool;
	sf::Clock *cleaner;
	sf::VideoMode desktopMode;
	sf::VideoMode prevMode;
}

////////////////////////////////////////////////////////////
/// Return the shared AppController object. Makes one if needed
////////////////////////////////////////////////////////////
+ (AppController *)sharedController;

////////////////////////////////////////////////////////////
/// Reallocate main pool to release autoreleased objects
////////////////////////////////////////////////////////////
- (void)resetPool;

////////////////////////////////////////////////////////////
/// Register our application and launch it if needed
////////////////////////////////////////////////////////////
- (void)runApplication;

////////////////////////////////////////////////////////////
/// Terminate the current running application
////////////////////////////////////////////////////////////
- (void)quitApplication:(id)sender;

////////////////////////////////////////////////////////////
/// Make menu bar
////////////////////////////////////////////////////////////
- (void)makeMenuBar;

////////////////////////////////////////////////////////////
/// Get the events and put them into an array for each window
////////////////////////////////////////////////////////////
- (void)processEvents;

////////////////////////////////////////////////////////////
/// Add the 'windowImplObj' object to the list of known windows
////////////////////////////////////////////////////////////
- (void)registerWindow:(sf::priv::WindowImplCocoa *)windowImplObj;

////////////////////////////////////////////////////////////
/// Remove the 'windowImplObj' object from the list of known windows
////////////////////////////////////////////////////////////
- (void)unregisterWindow:(sf::priv::WindowImplCocoa *)windowImplObj;

////////////////////////////////////////////////////////////
/// Return true is one of the registered window is a full screen one
////////////////////////////////////////////////////////////
- (bool)isUsingFullscreen;

////////////////////////////////////////////////////////////
/// Perform fade operation where 'operation' is one of { FillScreen, CleanScreen}
/// and 'time' is the time during which you wish the operation to be performed.
/// Set 'sync' to true if you do not want the method to end before the end
/// of the fade operation. Pass the last used token or a new one if you are
/// using this method for the first time. This lets the method release some
/// resources when doing CleanScreen operation.
////////////////////////////////////////////////////////////
- (void)doFadeOperation:(int)operation time:(float)time sync:(bool)sync token:(CGDisplayFadeReservationToken *)prevToken;

@end


////////////////////////////////////////////////////////////
/// check that ptr is valid, otherwise print msg in
/// std::cerr and throw std::bad_alloc.
/// Must be used to check alloc results
////////////////////////////////////////////////////////////
template <typename T>
T *massert(T *ptr);

