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
#import <SFML/Window/Cocoa/AppController.h>
#import <SFML/Window/Cocoa/GLKit.h>
#import <SFML/System.hpp>
#import <ApplicationServices/ApplicationServices.h>
#import <iostream>



/* setAppleMenu disappeared from the headers in 10.4 */
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4
@interface NSApplication (SFML)
- (void)setAppleMenu:(NSMenu *)menu;
@end
#endif

#define ENABLE_FADE_OPERATIONS 1

@implementation NSApplication (SFML)

- (void)setRunning:(BOOL)flag
{
	// Note: _running is a short, not a BOOL
	if (flag)
		_running = 1;
	else
		_running = 0;
}

@end


@implementation sfPrivAppController


////////////////////////////////////////////////////////////
/// Return an initialized AppController instance
/// Save the desktop mode
/// Make the main autorelease pool
/// Set the application observer
////////////////////////////////////////////////////////////
- (id)init
{
	self = [super init];
	
	if (self != nil) {
		myOwningEventLoop = NO;
		
		// Save the desktop mode
		myDesktopMode = sf::VideoMode::GetDesktopMode();
		myPrevMode = myDesktopMode;
		
		// Make the app autorelease pool
		myMainPool = [[NSAutoreleasePool alloc] init];
		
		if (!myMainPool) {
			[self release];
			throw std::bad_alloc();
		}
		
		// Don't go on if the user handles the app
		if (![NSApp isRunning])
		{
			// Force our application to appear in the Dock and make it able
			// to get focus (even when it's a raw executable)
			ProcessSerialNumber psn;
			
			if (!GetCurrentProcess(&psn)) {
				TransformProcessType(&psn, kProcessTransformToForegroundApplication);
				SetFrontProcess(&psn);
			}
			
			// Make the app
			if (![NSApplication sharedApplication]) {
				[self release];
				throw std::bad_alloc();
			}
			
			[self setNotifications];
			
			if ([NSApp mainMenu] == nil) {
				[self makeMenuBar];
			}
		}
	}
	
	return self;
}


////////////////////////////////////////////////////////////
/// Clean the controller
////////////////////////////////////////////////////////////
- (void)dealloc
{
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	[myMainPool release];
	[super dealloc];
}


////////////////////////////////////////////////////////////
/// Return the shared AppController instance. Make one if needed.
////////////////////////////////////////////////////////////
+ (sfPrivAppController *)sharedController
{
	// AppController singleton object
	static sfPrivAppController *shared = [[sfPrivAppController alloc] init];
	return shared;
}


////////////////////////////////////////////////////////////
/// Returns the primay computer's screen
////////////////////////////////////////////////////////////
+ (CGDirectDisplayID)primaryScreen
{
	static BOOL firstTime = YES;
	static CGDirectDisplayID screen = kCGNullDirectDisplay;
	
	if (firstTime) {
		CGDisplayCount numScr;
		CGDisplayErr err = CGGetDisplaysWithPoint(CGPointMake(0, 0), 1, &screen, &numScr);
		
		if (err != kCGErrorSuccess || numScr < 1) {
			std::cerr << "Unable to get primary screen (error code " << err 
			<< " ). Using the main screen.";
			screen = CGMainDisplayID();
		}
		firstTime = NO;
	}
	
	return screen;
}


////////////////////////////////////////////////////////////
/// Reset notifictions about application focus
////////////////////////////////////////////////////////////
- (void)setNotifications
{
	NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
	[nc addObserver:self
		   selector:@selector(applicationWillDeactivate:)
			   name:NSApplicationWillResignActiveNotification
			 object:NSApp];
	[nc addObserver:self
		   selector:@selector(applicationWillActivate:)
			   name:NSApplicationWillBecomeActiveNotification
			 object:NSApp];

	[nc addObserver:self
		   selector:@selector(applicationWillTerminate:)
			   name:NSApplicationWillTerminateNotification
			 object:NSApp];
}


////////////////////////////////////////////////////////////
/// Hide all the fullscreen windows and switch back to the desktop display mode
////////////////////////////////////////////////////////////
- (void)applicationWillDeactivate:(NSNotification *)aNotification
{
	// Note: not using fading because it produces reactivation issues
	if (myFullscreenWrapper) {
		myPrevMode = sf::VideoMode::GetDesktopMode();
		
		CFDictionaryRef displayMode = CGDisplayBestModeForParameters ([sfPrivAppController primaryScreen],
																	  myDesktopMode.BitsPerPixel,
																	  myDesktopMode.Width,
																	  myDesktopMode.Height,
																	  NULL);
		
		// Make the full screen window unvisible
		[[myFullscreenWrapper window] setAlphaValue:0.0f];
		
		// Switch to the wished display mode
		CGDisplaySwitchToMode([sfPrivAppController primaryScreen], displayMode);
	}
}


////////////////////////////////////////////////////////////
/// Unhide all the fullscreen windows and switch to full screen display mode
////////////////////////////////////////////////////////////
- (void)applicationWillActivate:(NSNotification *)aNotification
{
	if (myFullscreenWrapper) {
		CFDictionaryRef displayMode = CGDisplayBestModeForParameters ([sfPrivAppController primaryScreen],
																	  myPrevMode.BitsPerPixel,
																	  myPrevMode.Width,
																	  myPrevMode.Height,
																	  NULL);
		[NSMenu setMenuBarVisible:NO];
		
		// Switch to the wished display mode
		CGDisplaySwitchToMode([sfPrivAppController primaryScreen], displayMode);
		
		// Show the fullscreen window if existing
		if (myFullscreenWrapper)
		{
			[[myFullscreenWrapper window] setAlphaValue:1.0f];
			[[myFullscreenWrapper window] center];
			[[myFullscreenWrapper window] makeKeyAndOrderFront:self];
		}
	}
}


- (void)applicationWillTerminate:(NSNotification *)aNotification
{
	if (myFullscreenWrapper)
		[self setFullscreenWindow:nil mode:NULL];
}

////////////////////////////////////////////////////////////
/// Make menu bar
////////////////////////////////////////////////////////////
- (void)makeMenuBar
{
	// Source taken from SDL 1.3
	NSString *appName = nil;
	NSString *title = nil;
	NSMenu *appleMenu = nil;
	NSMenu *fileMenu = nil;
	NSMenu *windowMenu = nil;
	NSMenuItem *menuItem = nil;
	NSMenuItem *quitMenuItem = nil;
	
	// Determine the application name
	appName = [[[NSBundle mainBundle] infoDictionary] objectForKey: @"CFBundleName"];
	
	if (![appName length])
		appName = [[NSProcessInfo processInfo] processName];
	
	
	// Create the main menu bar
	[NSApp setMainMenu:[[NSMenu alloc] init]];
	
	// Create the application menu
	appleMenu = [[NSMenu alloc] initWithTitle:@""];
	
	// Put menu items
	// + 'About' menu item
	title = [@"About " stringByAppendingString:appName];
	[appleMenu addItemWithTitle:title
						 action:@selector(orderFrontStandardAboutPanel:)
				  keyEquivalent:@""];
	
	[appleMenu addItem:[NSMenuItem separatorItem]];
	
	// + 'Hide' menu item
	title = [@"Hide " stringByAppendingString:appName];
	[appleMenu addItemWithTitle:title
						 action:@selector(hide:)
				  keyEquivalent:@"h"];
	
	// + 'Hide other' menu item
	menuItem = (NSMenuItem *)[appleMenu addItemWithTitle:@"Hide Others"
												  action:@selector(hideOtherApplications:)
										   keyEquivalent:@"h"];
	[menuItem setKeyEquivalentModifierMask:NSAlternateKeyMask | NSCommandKeyMask];
	
	// + 'Show all' menu item
	[appleMenu addItemWithTitle:@"Show All"
						 action:@selector(unhideAllApplications:)
				  keyEquivalent:@""];
	
	[appleMenu addItem:[NSMenuItem separatorItem]];
	
	// + 'Quit' menu item
	title = [@"Quit " stringByAppendingString:appName];
	quitMenuItem = [[[NSMenuItem alloc]
					 initWithTitle:title 
					 action:@selector(terminate:)
					 keyEquivalent:@"q"] autorelease];
	//[quitMenuItem setTarget:self];
	[appleMenu addItem:quitMenuItem];
	
	// Put the menu into the menubar
	menuItem = [[NSMenuItem alloc]
				initWithTitle:@""
				action:nil
				keyEquivalent:@""];
	[menuItem setSubmenu:appleMenu];
	[[NSApp mainMenu] addItem:menuItem];
	[menuItem release];
	
	// Tell the application object that this is now the application menu
	[NSApp setAppleMenu:appleMenu];
	[appleMenu release];
	
	// 'File' menu
	fileMenu = [[NSMenu alloc]
				initWithTitle:@"File"];
	
	// + 'Close' menu item
	menuItem = [[NSMenuItem alloc]
				initWithTitle:@"Close"
				action:@selector(performClose:)
				keyEquivalent:@"w"];
	[fileMenu addItem:menuItem];
	[menuItem release];
	
	// + 'File' menu item (head)
	menuItem = [[NSMenuItem alloc]
				initWithTitle:@"File"
				action:nil
				keyEquivalent:@""];
	[menuItem setSubmenu:fileMenu];
	[[NSApp mainMenu] addItem:menuItem];
	[menuItem release];
	
	// 'Window' menu
	windowMenu = [[NSMenu alloc]
				  initWithTitle:@"Window"];
	
	// + 'Minimize' menu item
	menuItem = [[NSMenuItem alloc]
				initWithTitle:@"Minimize"
				action:@selector(performMiniaturize:)
				keyEquivalent:@"m"];
	[windowMenu addItem:menuItem];
	[menuItem release];
	
	// + 'Window' menu item (head)
	menuItem = [[NSMenuItem alloc]
				initWithTitle:@"Window"
				action:nil keyEquivalent:@""];
	[menuItem setSubmenu:windowMenu];
	[[NSApp mainMenu] addItem:menuItem];
	[menuItem release];
	
	// Tell the application object that this is now the window menu
	[NSApp setWindowsMenu:windowMenu];
	[windowMenu release];
}


////////////////////////////////////////////////////////////
/// Process all the events and send them to the application
/// No event is processed if the AppController instance is
/// not the owner of the event loop (ie: user made his own loop)
////////////////////////////////////////////////////////////
- (void)processEvents
{
	// Check there is a run loop
	if (![NSApp isRunning])
	{
		// Get the ownershipt of event handling if not and run
		[NSApp finishLaunching];
		[NSApp setRunning:YES];
		myOwningEventLoop = YES;
	}
	
	NSEvent *event = nil;
	
	if (myOwningEventLoop)
	{
		// Clean the autorelease pool
		[myMainPool release];
		myMainPool = [[NSAutoreleasePool alloc] init];
		
		// Minimal event loop
		while (nil != (event = [NSApp nextEventMatchingMask:NSAnyEventMask
												  untilDate:nil
													 inMode:NSDefaultRunLoopMode
													dequeue:YES]))
		{
			[NSApp sendEvent:event];
		}
	}
}


////////////////////////////////////////////////////////////
/// Set @window as the current fullscreen window
/// Change the screen resolution if needed according to @window and @fullscreenMode
////////////////////////////////////////////////////////////
- (void)setFullscreenWindow:(sfPrivWindow *)aWindow mode:(sf::VideoMode *)fullscreenMode
{
	// If we have a fullscreen window and want to remove it
	if (aWindow == nil && myFullscreenWrapper)
	{
		// Get the CoreGraphics display mode according to the desktop mode
		CFDictionaryRef displayMode = CGDisplayBestModeForParameters ([sfPrivAppController primaryScreen],
																	  myDesktopMode.BitsPerPixel,
																	  myDesktopMode.Width,
																	  myDesktopMode.Height,
																	  NULL);
		
#if ENABLE_FADE_OPERATIONS
		// Fade to black screen
		[self doFadeOperation:FillScreen time:0.2f sync:true];
#endif
		
		// Switch to the desktop display mode
		CGDisplaySwitchToMode([sfPrivAppController primaryScreen], displayMode);
		
		// Close the window
		[[myFullscreenWrapper window] close];
		
		// Show the menu bar
		[NSMenu setMenuBarVisible:YES];
		
#if ENABLE_FADE_OPERATIONS
		// Fade to normal screen
		[self doFadeOperation:CleanScreen time:0.5f sync:true];
#endif
		
		// Release the saved window wrapper
		myFullscreenWrapper = nil;
	}
	else if (aWindow)
	{
		// else if we want to SET fullscreen
		assert(fullscreenMode != NULL);
		
		// Get the CoreGraphics display mode according to the given sf mode
		CFDictionaryRef displayMode = CGDisplayBestModeForParameters ([sfPrivAppController primaryScreen],
																	  fullscreenMode->BitsPerPixel,
																	  fullscreenMode->Width,
																	  fullscreenMode->Height,
																	  NULL);
		
#if ENABLE_FADE_OPERATIONS
		// Fade to a black screen
		[self doFadeOperation:FillScreen time:0.5f sync:true];
#endif
		
		if (!myFullscreenWrapper)
		{
			// Hide the main menu bar
			[NSMenu setMenuBarVisible:NO];
		}
		
		if (myPrevMode != *fullscreenMode)
		{
			// Switch to the wished display mode
			myPrevMode = *fullscreenMode;
			CGDisplaySwitchToMode([sfPrivAppController primaryScreen], displayMode);
		}
		
		if (myFullscreenWrapper)
		{
			[[myFullscreenWrapper window] close];
		}
		
		// Open and center the window
		[[aWindow window] makeKeyAndOrderFront:nil];
		[[aWindow window] center];
		
#if ENABLE_FADE_OPERATIONS
		// Fade to normal screen
		[self doFadeOperation:CleanScreen time:0.2f sync:false];
#endif
		
		// Save the fullscreen wrapper
		myFullscreenWrapper = aWindow;
	}
	else
	{
		std::cerr << "Inconcistency error for arguments given to -[sfPrivAppController setFullscreenWindow:mode:]" << std::endl;
	}
}


////////////////////////////////////////////////////////////
/// Perform fade operation where 'operation' is one of {FillScreen, CleanScreen}
/// and 'time' is the time during which you wish the operation to be performed.
/// Set 'sync' to true if you do not want the method to end before the end
/// of the fade operation. Pass the last used token or a new one if you are
/// using this method for the first time. This lets the method release some
/// resources when doing CleanScreen operation.
////////////////////////////////////////////////////////////
- (void) doFadeOperation:(int)operation time:(float)time sync:(bool)sync
{
	static CGDisplayFadeReservationToken prevToken = kCGDisplayFadeReservationInvalidToken;
	CGDisplayFadeReservationToken token = prevToken;
	
	CGError result = 0, capture = 0;
	
	if (operation == FillScreen) {
		// Get access for the fade operation
		result = CGAcquireDisplayFadeReservation((int)(3 + time), &token);
		
		if (!result) {
			// Capture display but do not fill the screen with black
			// so that we can see the fade operation
			capture = CGDisplayCaptureWithOptions([sfPrivAppController primaryScreen], kCGCaptureNoFill);
			
			if (!capture) {
				// Do the increasing fade operation
				CGDisplayFade(token, time,
							  kCGDisplayBlendNormal,
							  kCGDisplayBlendSolidColor,
							  0.0f, 0.0f, 0.0f, sync);
				
				// Now, release the non black-filling capture
				CGDisplayRelease([sfPrivAppController primaryScreen]);
				
				// And capture with filling
				// so that we don't see the switching in the meantime
				CGDisplayCaptureWithOptions([sfPrivAppController primaryScreen], kCGCaptureNoOptions);
			}
			
			prevToken = token;
		}
	} else if (operation == CleanScreen) {
		// Get access for the fade operation
		if (token == kCGDisplayFadeReservationInvalidToken)
			result = CGAcquireDisplayFadeReservation((int)(3 + time), &token);
		
		if (!result) {
			if (!capture) {
				// Release the black-filling capture
				CGDisplayRelease([sfPrivAppController primaryScreen]);
				
				// Capture the display but do not fill with black (still for the fade operation)
				CGDisplayCaptureWithOptions([sfPrivAppController primaryScreen], kCGCaptureNoFill);
				
				// Do the decreasing fading
				CGDisplayFade(token, time,
							  kCGDisplayBlendSolidColor,
							  kCGDisplayBlendNormal,
							  0.0f, 0.0f, 0.0f, sync);
				
				// Release the fade operation token
				CGReleaseDisplayFadeReservation(token);
				
				// Invalidate the given token
				prevToken = kCGDisplayFadeReservationInvalidToken;
			}
			
			// Release the captured display
			CGDisplayRelease([sfPrivAppController primaryScreen]);
		}
	}
}


////////////////////////////////////////////////////////////
/// Return the desktop video mode (made at the instance initialization)
////////////////////////////////////////////////////////////
- (const sf::VideoMode&)desktopMode
{
	return myDesktopMode;
}

@end

