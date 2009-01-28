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
#import <SFML/Window/Cocoa/AppController.h>
#import <SFML/Window/Cocoa/WindowController.h>
#import <SFML/Window/Cocoa/WindowImplCocoa.hpp>
#import <SFML/System.hpp>
#import <ApplicationServices/ApplicationServices.h>
#import <iostream>


// AppController singleton
static AppController *shared = nil;


/* setAppleMenu disappeared from the headers in 10.4 */
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4
@interface NSApplication (SFML)
- (void)setAppleMenu:(NSMenu *)menu;
@end
#endif

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


@implementation AppController

- (id)init
{
	self = [super init];
	
	if (self != nil) {
		windows = new std::vector <sf::priv::WindowImplCocoa *>;
		cleaner = new sf::Clock;
	}
	
	return self;
}


- (void)dealloc
{
	delete windows;
	delete cleaner;
	[super dealloc];
}


////////////////////////////////////////////////////////////
/// Return the shared AppController object. Makes one if needed
////////////////////////////////////////////////////////////
+ (AppController *)sharedController
{
	if (nil == shared) {
		shared = [massert([AppController alloc]) init];
	}
	
	return shared;
}


////////////////////////////////////////////////////////////
/// Reallocate main pool to release autoreleased objects
////////////////////////////////////////////////////////////
- (void)resetPool
{
	[mainPool release];
	
	mainPool = [massert([NSAutoreleasePool alloc]) init];
}


////////////////////////////////////////////////////////////
/// Register our application and launch it if needed
////////////////////////////////////////////////////////////
- (void)runApplication
{
	if ([NSApp isRunning])
		return;
	
	// We want our application to appear in the Dock and be able
	// to get focus
	ProcessSerialNumber psn;
	
    if (!GetCurrentProcess(&psn)) {
        TransformProcessType(&psn, kProcessTransformToForegroundApplication);
        SetFrontProcess(&psn);
    }
	
    if (NSApp == nil) {
        massert([NSApplication sharedApplication]);
    }
	
	if ([NSApp mainMenu] == nil) {
		[self makeMenuBar];
	}
	
	[NSApp finishLaunching];
    [NSApp setRunning:YES];
	[NSApp setDelegate:self];
	
	desktopMode = sf::VideoMode::GetDesktopMode();
}


////////////////////////////////////////////////////////////
/// Terminate the current running application
////////////////////////////////////////////////////////////
- (void)quitApplication:(id)sender
{
	// Close all windows
	// SFML user has to detect when all windows are closed
	NSWindow *current = nil;
	sf::priv::WindowImplCocoa *priv = NULL;
	
	while (windows->size()) {
		priv = windows->at(0);
		current = static_cast <NSWindow *> (priv->CocoaWindow());
		[current close];
		windows->erase(windows->begin());
	}
}


////////////////////////////////////////////////////////////
/// Returns the first full screen window found or nil
////////////////////////////////////////////////////////////
- (SFWindow *)fullscreenWindow
{
	SFWindow *window = nil;
	std::vector<sf::priv::WindowImplCocoa *>::size_type sz = windows->size();
	std::vector<sf::priv::WindowImplCocoa *>::size_type idx;
	
	for (idx = 0; idx < sz; idx++) {
		sf::priv::WindowImplCocoa *win = windows->at(idx);
		if (win && win->IsFullscreen()) {
			window = static_cast <SFWindow *> (win->CocoaWindow());
			break;
		}
	}
	
	return window;
}


////////////////////////////////////////////////////////////
/// Hide all the fullscreen windows and switch to desktop display mode
////////////////////////////////////////////////////////////
- (void)applicationWillHide:(NSNotification *)aNotification
{
	if ([self isUsingFullscreen]) {
		prevMode = sf::VideoMode::GetDesktopMode();
		
		CFDictionaryRef displayMode = CGDisplayBestModeForParameters (kCGDirectMainDisplay,
																	  desktopMode.BitsPerPixel,
																	  desktopMode.Width,
																	  desktopMode.Height,
																	  NULL);
		
		CGDisplayFadeReservationToken token = kCGDisplayFadeReservationInvalidToken;
		
		// Fade to black screen
		[SharedAppController doFadeOperation:FillScreen time:0.2f sync:true token:&token];
		
		// Make all the full screen SFML windows unvisible
		std::vector<sf::priv::WindowImplCocoa *>::size_type sz = windows->size();
		std::vector<sf::priv::WindowImplCocoa *>::size_type idx;
		
		for (idx = 0; idx < sz; idx++) {
			sf::priv::WindowImplCocoa *win = windows->at(idx);
			
			if (win->IsFullscreen()) {
				[static_cast <SFWindow *> (win->CocoaWindow()) setAlphaValue:0.0f];
			}
		}
		
		// Switch to the wished display mode
		CGDisplaySwitchToMode(kCGDirectMainDisplay, displayMode);
		
		// Fade to normal screen
		[SharedAppController doFadeOperation:CleanScreen time:0.5f sync:false token:&token];
	}
}


////////////////////////////////////////////////////////////
/// Unhide all the fullscreen windows and switch to full screen display mode
////////////////////////////////////////////////////////////
- (void)applicationWillUnhide:(NSNotification *)aNotification
{
	if ([self isUsingFullscreen]) {
		CFDictionaryRef displayMode = CGDisplayBestModeForParameters (kCGDirectMainDisplay,
																	  prevMode.BitsPerPixel,
																	  prevMode.Width,
																	  prevMode.Height,
																	  NULL);
		
		CGDisplayFadeReservationToken token = kCGDisplayFadeReservationInvalidToken;
		
		// Fade to a black screen
		[SharedAppController doFadeOperation:FillScreen time:0.5f sync:true token:&token];
		[NSMenu setMenuBarVisible:NO];
		
		// Switch to the wished display mode
		CGDisplaySwitchToMode(kCGDirectMainDisplay, displayMode);
		
		// Make all the SFML windows visible
		std::vector<sf::priv::WindowImplCocoa *>::size_type sz = windows->size();
		std::vector<sf::priv::WindowImplCocoa *>::size_type idx;
		
		for (idx = 0; idx < sz; idx++) {
			sf::priv::WindowImplCocoa *win = windows->at(idx);
			
			if (win->IsFullscreen()) {
				[static_cast <SFWindow *> (win->CocoaWindow()) setAlphaValue:1.0f];
				[static_cast <SFWindow *> (win->CocoaWindow()) center];
			}
		}
		
		// Fade to normal screen
		[SharedAppController doFadeOperation:CleanScreen time:0.5f sync:false token:&token];
	}
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
	[NSApp setMainMenu:[massert([NSMenu alloc]) init]];
	
	// Create the application menu
	appleMenu = [massert([NSMenu alloc]) initWithTitle:@""];
	
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
	menuItem = static_cast <NSMenuItem *> ([appleMenu addItemWithTitle:@"Hide Others"
																action:@selector(hideOtherApplications:)
														 keyEquivalent:@"h"]);
	[menuItem setKeyEquivalentModifierMask:(NSAlternateKeyMask|NSCommandKeyMask)];
	
	// + 'Show all' menu item
	[appleMenu addItemWithTitle:@"Show All"
						 action:@selector(unhideAllApplications:)
				  keyEquivalent:@""];
	
	[appleMenu addItem:[NSMenuItem separatorItem]];
	
	// + 'Quit' menu item
	title = [@"Quit " stringByAppendingString:appName];
	quitMenuItem = [[massert([NSMenuItem alloc])
					 initWithTitle:title 
					 action:@selector(quitApplication:)
					 keyEquivalent:@"q"] autorelease];
	[quitMenuItem setTarget:self];
	[appleMenu addItem:quitMenuItem];
	
	// Put the menu into the menubar
	menuItem = [massert([NSMenuItem alloc])
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
	fileMenu = [massert([NSMenu alloc])
				initWithTitle:@"File"];
	
	// + 'Close' menu item
	menuItem = [massert([NSMenuItem alloc]) 
				initWithTitle:@"Close"
				action:@selector(performClose:)
				keyEquivalent:@"w"];
	[fileMenu addItem:menuItem];
	[menuItem release];
	
	// + 'File' menu item (head)
	menuItem = [massert([NSMenuItem alloc])
				initWithTitle:@"File"
				action:nil
				keyEquivalent:@""];
	[menuItem setSubmenu:fileMenu];
	[[NSApp mainMenu] addItem:menuItem];
	[menuItem release];
	
	// 'Window' menu
	windowMenu = [massert([NSMenu alloc])
				  initWithTitle:@"Window"];
	
	// + 'Minimize' menu item
	menuItem = [massert([NSMenuItem alloc])
				initWithTitle:@"Minimize"
				action:@selector(performMiniaturize:)
				keyEquivalent:@"m"];
	[windowMenu addItem:menuItem];
	[menuItem release];
	
	// + 'Window' menu item (head)
	menuItem = [massert([NSMenuItem alloc])
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
/// Delegate method in order to prevent usual -terminate:
////////////////////////////////////////////////////////////
- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
{
	[self quitApplication:nil];
	return NSTerminateCancel;
}


////////////////////////////////////////////////////////////
/// Get the events and put them into an array for each window
////////////////////////////////////////////////////////////
- (void)processEvents
{
	// Release the main autorelease pool every second
	if (cleaner->GetElapsedTime() > 1.0f) {
		cleaner->Reset();
		[self resetPool];
	}
	
	NSEvent *event = nil;
	
	while (nil != (event = [NSApp nextEventMatchingMask:NSAnyEventMask
											  untilDate:nil
												 inMode:NSEventTrackingRunLoopMode
												dequeue:YES])) {
		NSWindow *keyWindow = [NSApp keyWindow];
		
		if (keyWindow == nil) {
			// Is there a fullscreen WindowImpl object ?
			[NSApp sendEvent:event];
		} else {
			
			std::vector<sf::priv::WindowImplCocoa *>::size_type cnt = windows->size();
			std::vector<sf::priv::WindowImplCocoa *>::size_type idx;
			
			// is the key window a SFML window ?
			for (idx = 0;idx < cnt; idx++) {
				sf::priv::WindowImplCocoa *ptr = windows->at(idx);;
				
				if (ptr->CocoaWindow() == keyWindow) {
					// yup, it is
					ptr->HandleEvent(static_cast <void *> (event));
					break;
				}
			}
			
			// nop, it isn't
			if (idx == cnt) {
				[NSApp sendEvent:event];
			}
		}
	}
}


////////////////////////////////////////////////////////////
/// Add the 'windowImplObj' object to the list of known windows
////////////////////////////////////////////////////////////
- (void)registerWindow:(sf::priv::WindowImplCocoa *)windowImplObj
{
	
	if (windowImplObj != NULL) {
		std::vector<sf::priv::WindowImplCocoa *>::size_type sz = windows->size();
		std::vector<sf::priv::WindowImplCocoa *>::size_type idx;
		
		for (idx = 0; idx < sz; idx++) {
			if (windows->at(idx) == windowImplObj) {
				break;
			}
		}
		
		
		// Register window only if not already registered
		if (sz == idx) {
			windows->push_back(windowImplObj);
		}
	}
}


////////////////////////////////////////////////////////////
/// Remove the 'windowImplObj' object from the list of known windows
////////////////////////////////////////////////////////////
- (void)unregisterWindow:(sf::priv::WindowImplCocoa *)windowImplObj
{
	if (windowImplObj != NULL) {
		std::vector<sf::priv::WindowImplCocoa *>::size_type sz = windows->size();
		std::vector<sf::priv::WindowImplCocoa *>::size_type idx;
		
		for (idx = 0; idx < sz; idx++) {
			if (windows->at(idx) == windowImplObj) {
				break;
			}
		}
		
		if (idx < sz) {
			windows->erase(windows->begin() + idx);
		}
	}
}


////////////////////////////////////////////////////////////
/// Return true is one of the registered window is a full screen one
////////////////////////////////////////////////////////////
- (bool)isUsingFullscreen
{
	bool isUsing = false;
	std::vector<sf::priv::WindowImplCocoa *>::size_type sz = windows->size();
	std::vector<sf::priv::WindowImplCocoa *>::size_type idx;
	
	for (idx = 0; idx < sz; idx++) {
		sf::priv::WindowImplCocoa *win = windows->at(idx);
		if (win && win->IsFullscreen()) {
			isUsing = true;
			break;
		}
	}
	
	return isUsing;
}


////////////////////////////////////////////////////////////
/// Perform fade operation where 'operation' is one of { FillScreen, CleanScreen}
/// and 'time' is the time during which you wish the operation to be performed.
/// Set 'sync' to true if you do not want the method to end before the end
/// of the fade operation. Pass the last used token or a new one if you are
/// using this method for the first time. This lets the method release some
/// resources when doing CleanScreen operation.
////////////////////////////////////////////////////////////
- (void) doFadeOperation:(int)operation time:(float)time sync:(bool)sync token:(CGDisplayFadeReservationToken *)prevToken
{
	CGDisplayFadeReservationToken token = kCGDisplayFadeReservationInvalidToken;
	if (prevToken)
		token = *prevToken;
	
	CGError result = 0, capture = 0;
	
	if (operation == FillScreen) {
		// Get access for the fade operation
		result = CGAcquireDisplayFadeReservation((int)(3 + time), &token);
		
		if (!result) {
			// Capture display but do not fill the screen with black
			// so that we can see the fade operation
			capture = CGDisplayCaptureWithOptions(kCGDirectMainDisplay, kCGCaptureNoFill);
			
			if (!capture) {
				// Do the increasing fade operation
				CGDisplayFade(token, time,
							  kCGDisplayBlendNormal,
							  kCGDisplayBlendSolidColor,
							  0.0, 0.0, 0.0, sync);
				
				// Now, release the non black-filling capture
				CGDisplayRelease(kCGDirectMainDisplay);
				
				// And capture with filling
				// so that we don't see the switching in the meantime
				CGDisplayCaptureWithOptions(kCGDirectMainDisplay, kCGCaptureNoOptions);
			}
			
			if (prevToken)
				*prevToken = token;
		}
	} else if (operation == CleanScreen) {
		// Get access for the fade operation
		if (token == kCGDisplayFadeReservationInvalidToken)
			result = CGAcquireDisplayFadeReservation((int)(3 + time), &token);
		
		if (!result) {
			if (!capture) {
				// Release the black-filling capture
				CGDisplayRelease(kCGDirectMainDisplay);
				
				// Capture the display but do not fill with black (still for the fade operation)
				CGDisplayCaptureWithOptions(kCGDirectMainDisplay, kCGCaptureNoFill);
				
				// Do the decreasing fading
				CGDisplayFade(token, time,
							  kCGDisplayBlendSolidColor,
							  kCGDisplayBlendNormal,
							  0.0, 0.0, 0.0, sync);
				
				// Release the fade operation token
				CGReleaseDisplayFadeReservation(token);
				
				// Invalidate the given token
				if (prevToken)
					*prevToken = kCGDisplayFadeReservationInvalidToken;
			}
			
			// Release the captured display
			CGDisplayRelease(kCGDirectMainDisplay);
		}
	}
}

@end


template <typename T>
T *massert(T *ptr)
{
	if (NULL == ptr) {
		throw std::bad_alloc();
	}
	
	return ptr;
}
