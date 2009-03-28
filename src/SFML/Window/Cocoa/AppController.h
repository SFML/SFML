////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Lucas Soltic (ceylow@gmail.com) and Laurent Gomila (laurent.gom@gmail.com)
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
#import <SFML/Window/VideoMode.hpp>
#import <Cocoa/Cocoa.h>


#define SharedAppController [AppController sharedController]

// Fade operations
enum {
	FillScreen,
	CleanScreen
};

@class WindowWrapper;
@interface AppController : NSObject {
	BOOL myOwningEventLoop;
	WindowWrapper *myFullscreenWrapper;
	NSAutoreleasePool *myMainPool;
	sf::VideoMode myDesktopMode;
	sf::VideoMode myPrevMode;
}

////////////////////////////////////////////////////////////
/// Return the shared AppController instance. Make one if needed.
////////////////////////////////////////////////////////////
+ (AppController *)sharedController;

////////////////////////////////////////////////////////////
/// Make the menu bar
////////////////////////////////////////////////////////////
- (void)makeMenuBar;

////////////////////////////////////////////////////////////
/// Process all the events and send them to the application
/// No event is processed if the AppController instance is
/// not the owner of the event loop (ie: user made his own loop)
////////////////////////////////////////////////////////////
- (void)processEvents;

////////////////////////////////////////////////////////////
/// Set @window as the current fullscreen window
/// Change the screen resolution if needed according to @window and @fullscreenMode
////////////////////////////////////////////////////////////
- (void)setFullscreenWindow:(WindowWrapper *)window mode:(sf::VideoMode *)fullscreenMode;

////////////////////////////////////////////////////////////
/// Perform fade operation where 'operation' is one of { FillScreen, CleanScreen}
/// and 'time' is the time during which you wish the operation to be performed.
/// Set 'sync' to true if you do not want the method to end before the end
/// of the fade operation.
////////////////////////////////////////////////////////////
- (void)doFadeOperation:(int)operation time:(float)time sync:(bool)sync;

////////////////////////////////////////////////////////////
/// Return the desktop video mode (made at the instance initialization)
////////////////////////////////////////////////////////////
- (const sf::VideoMode&)desktopMode;

@end

