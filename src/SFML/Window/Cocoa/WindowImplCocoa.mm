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
#import <SFML/Window/Cocoa/WindowController.h>
#import <SFML/Window/Cocoa/AppController.h>
#import <SFML/Window/WindowStyle.hpp>
#import <SFML/System.hpp>
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>
#import <Cocoa/Cocoa.h>
#import <iostream>


#pragma mark Notes

// ENABLE_ANTIALIASING macro :
//
// I use this to prevent the use of antialiasing
// as OpenGL context sharing does not allow only one of the
// shared OpenGL context to be shared. That means
// antialiasing could not be used without re-compiling
// the whole sfml-window library.
//
// I've no way to fix this for now.

// ENABLE_WINDOWIMPORT macro :
//
// I use this to disable the import of Cocoa windows
// as it does not work fine for now and is not
// a high priority.





namespace sf
{
namespace priv
{

// Do something only once (useful in loops)
#define ONCE(make) \
{ static int __done = 0;\
if (!__done) {\
make;\
__done = 1;\
} }

// Has the event been used or not ? If not, it must be sent to NSApp
enum {
	UsedEvent,
	UnusedEvent
};


////////////////////////////////////////////////////////////
/// Structure containing all the members I can't directly put in the class definition
/// because I would have to hide them in a #ifdef __OBJC__ block and the object
/// allocator would allocate space for it as it would be called from a C++ code
/// that wouldn't see these members
////////////////////////////////////////////////////////////
struct objc_members {
	WindowController *controller;
	SFWindow *window;
	NSOpenGLContext *context;
	NSOpenGLView *view;
};

////////////////////////////////////////////////////////////
/// Pointer to the shared OpenGL context
////////////////////////////////////////////////////////////
static NSOpenGLContext *sharedContext = nil;


////////////////////////////////////////////////////////////
/// Private function declarations
////////////////////////////////////////////////////////////
static SFWindow *		MakeWindow(WindowSettings& params, unsigned long style, VideoMode& mode, NSString *title);
static NSOpenGLContext *MakeOpenGLContext(WindowSettings& params);
static NSOpenGLView	*	MakeOpenGLView(SFWindow *window, NSOpenGLContext *context, WindowSettings& params);
static void				ConfigureWindow(SFWindow *window, NSOpenGLView *view, WindowController *controller);
static Key::Code		KeyForVirtualCode(unsigned short vCode);
static Key::Code		KeyForUnicode(unsigned short uniCode);
static bool				IsTextEvent(NSEvent *event);
static bool				MouseInside(SFWindow *window, NSView *view);
static NSPoint			MouseLocation(SFWindow *window);


////////////////////////////////////////////////////////////
/// Default constructor
/// (creates a dummy window to provide a valid OpenGL context)
////////////////////////////////////////////////////////////
WindowImplCocoa::WindowImplCocoa() :
members(NULL),
useKeyRepeat(false),
mouseIn(false),
wheelStatus(0.0f),
fullscreen(false),
fullscreenMode(0, 0, 0),
desktopMode(0, 0, 0)
{
	Initialize();
	
    // We just want to have a valid support for an OpenGL context
	
	// So we create the OpenGL context
	WindowSettings params(0, 0, 0);
	members->context = MakeOpenGLContext(params);
	
	if (members->context != nil) {
		// Increase the reference counter for the shared OpenGL context
		sharedContext = [members->context retain];
		
		// Then we make it the current active OpenGL context
		SetActive();
		
		// And set the current working directory to
		// the Resources folder is it's a bundled app,
		// or to the directory containing the executable otherwise
		chdir([[[NSBundle mainBundle] resourcePath] UTF8String]);
		
	} else {
		std::cerr << "*** SFML: Unable to make the main shared OpenGL context" << std::endl;
	}
}


////////////////////////////////////////////////////////////
/// Create the window implementation from an existing control
////////////////////////////////////////////////////////////
WindowImplCocoa::WindowImplCocoa(WindowHandle Handle, WindowSettings& params) :
members(NULL),
useKeyRepeat(false),
mouseIn(false),
wheelStatus(0.0f),
fullscreen(false),
fullscreenMode(0, 0, 0),
desktopMode(0, 0, 0)
{
	Initialize();
	
#if ENABLE_WINDOWIMPORT
	// Register ourselves for event handling
	[[AppController sharedController] registerWindow:this];
	
	// Make a WindowController to handle notifications
	members->controller = [[WindowController controllerWithWindow:this] retain];
	
	// Use existing window
	members->window = [static_cast <SFWindow *> (Handle) retain];
	
	if (members->window != nil) {		
		// We make the OpenGL context, associate it to the OpenGL view
		// and add the view to our window
		members->context = MakeOpenGLContext(params);
		
		if (members->context != nil) {
			members->view = MakeOpenGLView(members->window, members->context, params);
			
			if (members->view != nil) {
				// initial mouse state
				mouseIn = MouseInside(members->window, members->view);
				
				// Initialize myWidth and myHeight members from base class with the window size
				myWidth = (unsigned) [members->window frame].size.width;
				myHeight = (unsigned) [members->window frame].size.height;
			} else {
				//error(__FILE__, __LINE__, "failed to make the OpenGL view for the public window");
			}
		} else {
			//error(__FILE__, __LINE__, "failed to make the OpenGL context for the public window");
		}
	} else {
		//error(__FILE__, __LINE__, "invalid imported window");
	}
#else
	std::cerr << "*** SFML: making a sf::Window from a Cocoa one is not available in this version of the SFML" << std::endl;
#endif
}


////////////////////////////////////////////////////////////
/// Create the window implementation
////////////////////////////////////////////////////////////
WindowImplCocoa::WindowImplCocoa(VideoMode Mode, const std::string& Title, unsigned long WindowStyle, WindowSettings& params) :
members(NULL),
useKeyRepeat(false),
mouseIn(false),
wheelStatus(0.0f),
fullscreen(WindowStyle & Style::Fullscreen),
fullscreenMode(0, 0, 0),
desktopMode(0, 0, 0)
{
	Initialize();
	
	// Make a WindowController to handle notifications
	members->controller = [[WindowController controllerWithWindow:this] retain];
	
    // Create a new window with given size, title and style
	// First we define some objects used for our window
	NSString *title = massert([NSString stringWithUTF8String:Title.c_str()]);
	
	// We create the window
	members->window = MakeWindow(params, WindowStyle, Mode, title);
	
	
	if (members->window != nil) {
		members->context = MakeOpenGLContext(params);
		
		if (members->context != nil) {
			// We make the OpenGL context, associate it to the OpenGL view
			// and add the view to our window
			members->view = MakeOpenGLView(members->window, members->context, params);
			
			if (members->view != nil) {
				// Set observers and some window settings
				ConfigureWindow(members->window, members->view, members->controller);
				
				// initial mouse state
				mouseIn = MouseInside(members->window, members->view);
				
				// We set the myWidth and myHeight members to the correct values
				myWidth = Mode.Width;
				myHeight = Mode.Height;
				
				if (WindowStyle & Style::Fullscreen) {
					fullscreenMode = Mode;
					
					// Using this because full screen window was not always
					// in front of the other application windows when unhiding app
					[members->window setLevel:NSFloatingWindowLevel];
				}
			} else {
				std::cerr << "*** SFML: failed to make the OpenGL view for the public window" << std::endl;
				[members->context release], members->context = nil;
				[sharedContext release];
				[members->window release], members->window = nil;
			}
		} else {
			std::cerr << "*** SFML: failed to make the OpenGL context for the public window" << std::endl;
			[members->window release], members->window = nil;
		}
	} else {
		std::cerr << "*** SFML: failed to make the public window" << std::endl;
	}
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
WindowImplCocoa::~WindowImplCocoa()
{

	// Release the notification receiver
	if (members) {
		[[NSNotificationCenter defaultCenter] removeObserver:members->controller];
		[members->controller release];
	}
	
	// Make sure the window is closed
	Show(false);
	
	// Decrement the shared context counter
	[sharedContext release];
	
	// Release the window objects
	if (members) {
		[members->context release];
		[members->view release];
		[members->window release];
	}
	
	// Free the private members struct
	delete members;
}


////////////////////////////////////////////////////////////
/// Check if there's an active context on the current thread
////////////////////////////////////////////////////////////
bool WindowImplCocoa::IsContextActive()
{
	return ([NSOpenGLContext currentContext] != NULL);
}


////////////////////////////////////////////////////////////
/// Handle a Cocoa NSEvent
////////////////////////////////////////////////////////////
void WindowImplCocoa::HandleEvent(void *eventRef)
{
	if (eventRef == nil) {
		std::cerr << "*** SFML: cannot handle a NULL event. Returning." << std::endl;
		return;
	}
	
	NSEvent *event = static_cast <NSEvent *> (eventRef);
	int eventStatus = UnusedEvent;
	
	switch ([event type]) {
		case NSKeyDown:
			eventStatus = HandleKeyDown(eventRef);
			break;
			
		case NSKeyUp:
			eventStatus = HandleKeyUp(eventRef);
			break;
			
		case NSFlagsChanged:
			eventStatus = HandleModifierKey(eventRef);
			break;
			
		case NSScrollWheel:
			eventStatus = HandleMouseWheel(eventRef);
			break;
		
		case NSLeftMouseDown:
		case NSRightMouseDown:
			eventStatus = HandleMouseDown(eventRef);
			break;
			
		case NSLeftMouseUp:
		case NSRightMouseUp:
			eventStatus = HandleMouseUp(eventRef);
			break;
			
		case NSMouseMoved:
		case NSLeftMouseDragged:
		case NSRightMouseDragged:
		case NSOtherMouseDragged:
			eventStatus = HandleMouseMove(eventRef);
			break;
			
		default:
			break;
	}
	
	if (eventStatus == UnusedEvent) {
		[NSApp sendEvent:event];
	}
}


////////////////////////////////////////////////////////////
/// Handle event sent by the default NSNotificationCenter
////////////////////////////////////////////////////////////
void WindowImplCocoa::HandleNotifiedEvent(Event& event)
{
	// Set myWidth and myHeight to correct value if
	// window size changed
	switch (event.Type) {
		case Event::Resized:
			myWidth = event.Size.Width;
			myHeight = event.Size.Height;
			break;
			
		default:
			break;
	}
	
	// And send the event
	SendEvent(event);
}


////////////////////////////////////////////////////////////
/// Handle a key down event (NSEvent)
////////////////////////////////////////////////////////////
int WindowImplCocoa::HandleKeyDown(void *eventRef)
{
	NSEvent *event = static_cast <NSEvent *> (eventRef);
	
	Event sfEvent;
	unichar chr = 0, rawchr = 0;
	unsigned long length = [[event characters] length];
	unsigned mods = [event modifierFlags];
	
	if (length) {
		chr = [[event characters] characterAtIndex:0];
		
		// Note : I got a crash (out of bounds exception) while typing so now I test...
		if ([[event charactersIgnoringModifiers] length])
			rawchr = [[event charactersIgnoringModifiers] characterAtIndex:0];
		
	}
	
	if (mods & NSCommandKeyMask) {
		// Application commands
		[NSApp sendEvent:event];
	}
	
	// User events
	
	if (!useKeyRepeat && [event isARepeat]) {
		return UsedEvent;
	}
	
#if 1
	// Is it also a text event ?
	if (IsTextEvent(event)) {
		// buffer for the UTF-8 characters
		const char *utf8Characters = [[event characters] UTF8String];
		
		// buffer for the UTF-32 characters
		Uint32 utf32Characters[2];
		
		// convert the characters
		const Uint32 *addr = Unicode::UTF8ToUTF32(utf8Characters,
												  utf8Characters + length,
												  utf32Characters);
		
		// si il y a eu des caracteres convertis ?
		if (addr > utf32Characters) {
			sfEvent.Type = Event::TextEntered;
			sfEvent.Text.Unicode = utf32Characters[0];
			
			SendEvent(sfEvent);
		}
	}
#else
	// Is it also a text event ?
	if (IsTextEvent(event)) {
		static NSMutableArray *arr = [[NSMutableArray alloc] initWithCapacity:1];
		
		sfEvent.Type = Event::TextEntered;
		sfEvent.Text.Unicode = chr;
		
		NSText *field = [members->window fieldEditor:YES forObject:nil];
		[arr addObject:event];
		[field interpretKeyEvents:arr];
		
		if ([[field string] length]) {
			unichar unichr = [[field string] characterAtIndex:0];
			sfEvent.Text.Unicode = unichr;
			SendEvent(sfEvent);
			
			unichar str[2] = {unichr, 0};
			NSLog(@"Char::%@", [NSString stringWithCharacters:str length:2]);
			
			[field setString:@""];
			[arr removeAllObjects];
		}
		
		
	}
#endif
	
	// Anyway it's also a KeyPressed event
	sfEvent.Type = Event::KeyPressed;
	
	// Get the keys
	if (Key::Code(0) == (sfEvent.Key.Code = KeyForUnicode(rawchr))) {
		sfEvent.Key.Code = KeyForVirtualCode([event keyCode]);
	}
	
	// Get the modifiers
	sfEvent.Key.Alt = mods & NSAlternateKeyMask;
	sfEvent.Key.Control = mods & NSControlKeyMask;
	sfEvent.Key.Shift = mods & NSShiftKeyMask;
	
	// Send the event
	SendEvent(sfEvent);
	
	return UsedEvent;
}


////////////////////////////////////////////////////////////
/// Handle a key up event (NSEvent)
////////////////////////////////////////////////////////////
int WindowImplCocoa::HandleKeyUp(void *eventRef)
{
	NSEvent *event = static_cast <NSEvent *> (eventRef);
	
	Event sfEvent;
	unsigned mods = [event modifierFlags];
	unichar chr = 0, rawchr = 0;
	
	if ([[event characters] length]) {
		chr = [[event characters] characterAtIndex:0];
		
		if ([[event charactersIgnoringModifiers] length])
			rawchr = [[event charactersIgnoringModifiers] characterAtIndex:0];
		
	}
	
	if (mods & NSCommandKeyMask) {
		[NSApp sendEvent:event];
	}
	
	sfEvent.Type = Event::KeyReleased;
	
	// Get the code
	if (Key::Code(0) == (sfEvent.Key.Code = KeyForUnicode(rawchr))) {
		sfEvent.Key.Code = KeyForVirtualCode([event keyCode]);
	}
	
	// Get the modifiers
	sfEvent.Key.Alt = mods & NSAlternateKeyMask;
	sfEvent.Key.Control = mods & NSControlKeyMask;
	sfEvent.Key.Shift = mods & NSShiftKeyMask;
	
	SendEvent(sfEvent);
	
	return UsedEvent;
}


////////////////////////////////////////////////////////////
/// Handle a key modifier event [Command, Option, Control, Shift]
////////////////////////////////////////////////////////////
int WindowImplCocoa::HandleModifierKey(void *eventRef)
{
	NSEvent *event = static_cast <NSEvent *> (eventRef);
	Event sfEvent;
	unsigned mods = [event modifierFlags];
	
	sfEvent.Type = Event::KeyPressed;
	sfEvent.Key.Code = KeyForVirtualCode([event keyCode]);
	
	sfEvent.Key.Alt = mods & NSAlternateKeyMask;
	sfEvent.Key.Control = mods & NSControlKeyMask;
	sfEvent.Key.Shift = mods & NSShiftKeyMask;
	
	if (!(mods & NSAlternateKeyMask) &&
		(sfEvent.Key.Code == Key::LAlt || sfEvent.Key.Code == Key::RAlt)) {
		sfEvent.Type = Event::KeyReleased;
	}
	
	if (!(mods & NSControlKeyMask) &&
		(sfEvent.Key.Code == Key::LControl || sfEvent.Key.Code == Key::RControl)) {
		sfEvent.Type = Event::KeyReleased;
	}
	
	if (!(mods & NSShiftKeyMask) &&
		(sfEvent.Key.Code == Key::LShift || sfEvent.Key.Code == Key::RShift)) {
		sfEvent.Type = Event::KeyReleased;
	}
	
	if (!(mods & NSCommandKeyMask) &&
		(sfEvent.Key.Code == Key::LSystem || sfEvent.Key.Code == Key::RSystem)) {
		sfEvent.Type = Event::KeyReleased;
	}
	
	SendEvent(sfEvent);
	
	return UnusedEvent;
}


////////////////////////////////////////////////////////////
/// Handle a mouse down event (NSEvent)
////////////////////////////////////////////////////////////
int WindowImplCocoa::HandleMouseDown(void *eventRef)
{
	NSEvent *event = static_cast <NSEvent *> (eventRef);
	Event sfEvent;
	NSPoint loc = {0, 0};
	unsigned mods = [event modifierFlags];
	
	switch ([event type]) {
		case NSLeftMouseDown:
			sfEvent.Type = Event::MouseButtonPressed;
			
			if (mods & NSControlKeyMask) {
				sfEvent.MouseButton.Button = Mouse::Right;
			} else {
				sfEvent.MouseButton.Button = Mouse::Left;
			}
			
			// Get mouse position
			loc = MouseLocation(members->window);
			
			sfEvent.MouseButton.X = (int) loc.x;
			sfEvent.MouseButton.Y = (int) loc.y;
			
			SendEvent(sfEvent);
			break;
			
		case NSRightMouseDown:
			sfEvent.Type = Event::MouseButtonPressed;
			sfEvent.MouseButton.Button = Mouse::Right;
			
			// Get mouse position
			loc = MouseLocation(members->window);
			
			sfEvent.MouseButton.X = (int) loc.x;
			sfEvent.MouseButton.Y = (int) loc.y;
			
			SendEvent(sfEvent);
			break;
			
		default:
			break;
	}
	
	return UnusedEvent;
	
}


////////////////////////////////////////////////////////////
/// Handle a mouse up event (NSEvent)
////////////////////////////////////////////////////////////
int WindowImplCocoa::HandleMouseUp(void *eventRef)
{
	NSEvent *event = static_cast <NSEvent *> (eventRef);
	Event sfEvent;
	NSPoint loc = {0, 0};
	unsigned mods = [event modifierFlags];
	
	switch ([event type]) {
		case NSLeftMouseUp:
			sfEvent.Type = Event::MouseButtonReleased;
			
			if (mods & NSControlKeyMask) {
				sfEvent.MouseButton.Button = Mouse::Right;
			} else {
				sfEvent.MouseButton.Button = Mouse::Left;
			}
			
			// Get mouse position
			loc = MouseLocation(members->window);
			
			sfEvent.MouseButton.X = (int) loc.x;
			sfEvent.MouseButton.Y = (int) loc.y;
			
			SendEvent(sfEvent);
			break;
			
		case NSRightMouseUp:
			sfEvent.Type = Event::MouseButtonReleased;
			sfEvent.MouseButton.Button = Mouse::Right;
			
			// Get mouse position
			loc = MouseLocation(members->window);
			
			sfEvent.MouseButton.X = (int) loc.x;
			sfEvent.MouseButton.Y = (int) loc.y;
			
			SendEvent(sfEvent);
			break;
			
		default:
			break;
	}
	
	return UnusedEvent;
}


////////////////////////////////////////////////////////////
/// Handle a mouse move event (NSEvent)
////////////////////////////////////////////////////////////
int WindowImplCocoa::HandleMouseMove(void *eventRef)
{
	Event sfEvent;
	NSPoint loc = {0, 0};
	
	loc = MouseLocation(members->window);
	sfEvent.Type = Event::MouseMoved;
	
	sfEvent.MouseMove.X = (int) loc.x;
	sfEvent.MouseMove.Y = (int) loc.y;
	
	SendEvent(sfEvent);
	
	// MouseEntered and MouseLeft events
	if (MouseInside(members->window, members->view) && !mouseIn) {
		sfEvent.Type = Event::MouseEntered;
		mouseIn = true;
		SendEvent(sfEvent);
	} else if (!MouseInside(members->window, members->view) && mouseIn) {
		sfEvent.Type = Event::MouseLeft;
		mouseIn = false;
		SendEvent(sfEvent);
	}
	
	return UnusedEvent;
}


////////////////////////////////////////////////////////////
/// Handle a mouse wheel event (NSEvent)
////////////////////////////////////////////////////////////
int WindowImplCocoa::HandleMouseWheel(void *eventRef)
{
	NSEvent *event = static_cast <NSEvent *> (eventRef);
	
	wheelStatus += [event deltaY];
	
	if (fabs(wheelStatus) > 1.0f) {
		Event sfEvent;
		sfEvent.Type = Event::MouseWheelMoved;
		sfEvent.MouseWheel.Delta = (int) wheelStatus;
		SendEvent(sfEvent);
		
		wheelStatus -= (int) wheelStatus;
	}
	
	return UnusedEvent;
}


////////////////////////////////////////////////////////////
/// Return a pointer to the SFWindow object
////////////////////////////////////////////////////////////
void *WindowImplCocoa::CocoaWindow(void)
{
	return static_cast <void *> (members->window);
}

////////////////////////////////////////////////////////////
/// Return whether the window is in full screen mode
////////////////////////////////////////////////////////////
bool WindowImplCocoa::IsFullscreen(void)
{
	return fullscreen;
}

////////////////////////////////////////////////////////////
/// /see sfWindowImpl::Display
////////////////////////////////////////////////////////////
void WindowImplCocoa::Display()
{
	[members->context flushBuffer];
}


////////////////////////////////////////////////////////////
/// /see sfWindowImpl::ProcessEvents
////////////////////////////////////////////////////////////
void WindowImplCocoa::ProcessEvents()
{
	if (![NSApp isRunning])
		return;
	
	[[AppController sharedController] processEvents];
}


////////////////////////////////////////////////////////////
/// /see sfWindowImpl::MakeActive
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetActive(bool Active) const
{
	if (Active) {
		if ([NSOpenGLContext currentContext] != members->context)
			[members->context makeCurrentContext];
	} else {
		if ([NSOpenGLContext currentContext] == members->context)
			[NSOpenGLContext clearCurrentContext];
	}
}


////////////////////////////////////////////////////////////
/// /see sfWindowImpl::UseVerticalSync
////////////////////////////////////////////////////////////
void WindowImplCocoa::UseVerticalSync(bool Enabled)
{
	GLint enable = (Enabled) ? 1 : 0;
	[members->context setValues:&enable forParameter:NSOpenGLCPSwapInterval];
}


////////////////////////////////////////////////////////////
/// /see sfWindowImpl::ShowMouseCursor
////////////////////////////////////////////////////////////
void WindowImplCocoa::ShowMouseCursor(bool flag)
{
	if (flag) {
		[NSCursor unhide];
	} else {
		[NSCursor hide];
	}
}


////////////////////////////////////////////////////////////
/// /see sfWindowImpl::SetCursorPosition
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetCursorPosition(unsigned int Left, unsigned int Top)
{
	NSPoint pos = NSMakePoint ((float) Left, (float) Top);
	
	if (members->window) {
		// Flip for SFML window coordinate system
		pos.y = [members->window frame].size.height - pos.y;
		
		// Adjust for view reference instead of window
		pos.y -= [members->window frame].size.height - [members->view frame].size.height;
		
		// Convert to screen coordinates
		NSPoint absolute = [members->window convertBaseToScreen:pos];
		
		// Flip screen coodinates
		absolute.y = [[NSScreen mainScreen] frame].size.height - absolute.y;
		
		// Move cursor
		CGDisplayMoveCursorToPoint(kCGDirectMainDisplay, CGPointMake(absolute.x, absolute.y));
	} else {
		std::cerr << "*** SFML: uninitialized 'members->window' member (objc_members) in "
			 << this << " (sf::priv::WindowImplCocoa)" << std::endl;
	}
}


////////////////////////////////////////////////////////////
/// /see sfWindowImpl::SetPosition
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetPosition(int Left, int Top)
{
	if (members->window) {
		if (!fullscreen) {
			// Change the window position
			Top = (int) [[members->window screen] frame].size.height - Top;
			[members->window setFrameTopLeftPoint:NSMakePoint(Left, Top)];
		}
	} else {
		std::cerr << "*** SFML: uninitialized 'members->window' member (objc_members) in "
			 << this << " (sf::priv::WindowImplCocoa)";
	}
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetSize
///
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetSize(unsigned int Width, unsigned int Height)
{
	if (members->window) {
		if (!fullscreen) {
			[members->window setFrame:NSMakeRect([members->window frame].origin.x,
												 [members->window frame].origin.y,
												 (float) Width, (float) Height)
			 display:YES];
		}
	} else {
		std::cerr << "*** SFML: uninitialized 'members->window' member (objc_members) in "
			 << this << " (sf::priv::WindowImplCocoa)" << std::endl;
	}
}


////////////////////////////////////////////////////////////
/// /see sfWindowImpl::Show
////////////////////////////////////////////////////////////
void WindowImplCocoa::Show(bool State)
{
	if (State && ![members->window isVisible]) {
		// Wanna open the closed window
		
		// Register ourselves for event handling
		[[AppController sharedController] registerWindow:this];
		
		if (fullscreen) {
			desktopMode = VideoMode::GetDesktopMode();
			
			CFDictionaryRef displayMode = CGDisplayBestModeForParameters (kCGDirectMainDisplay,
																		  fullscreenMode.BitsPerPixel,
																		  fullscreenMode.Width,
																		  fullscreenMode.Height,
																		  NULL);
			
			CGDisplayFadeReservationToken token = kCGDisplayFadeReservationInvalidToken;
			
			// Fade to a black screen
			[SharedAppController doFadeOperation:FillScreen time:0.5f sync:true token:&token];
			[NSMenu setMenuBarVisible:NO];
			
			// Switch to the wished display mode
			CGDisplaySwitchToMode(kCGDirectMainDisplay, displayMode);
			
			// Open the window
			[members->window makeKeyAndOrderFront:nil];
			[members->window center];
			
			// Fade to normal screen
			[SharedAppController doFadeOperation:CleanScreen time:0.2f sync:false token:&token];
			
		} else {
			// Show the window
			// Note: using these two lines instead of -[NSWindow makeKeyAndOrderFront:]
			// in order to prevent the standard window buttons from not displaying
			// the "mouse over" icons
			[members->window orderFront:nil];
			[members->window makeKeyWindow];
			
		}
	} else if (!State && [members->window isVisible]) {
		// Wanna close the opened window
		
		if (fullscreen) {
			CFDictionaryRef displayMode = CGDisplayBestModeForParameters (kCGDirectMainDisplay,
																		  desktopMode.BitsPerPixel,
																		  desktopMode.Width,
																		  desktopMode.Height,
																		  NULL);
			
			CGDisplayFadeReservationToken token = kCGDisplayFadeReservationInvalidToken;
			
			// Fade to black screen
			[SharedAppController doFadeOperation:FillScreen time:0.2f sync:true token:&token];
			
			// Switch to the wished display mode
			CGDisplaySwitchToMode(kCGDirectMainDisplay, displayMode);
			
			// Close the window
			[members->window close];
			[NSMenu setMenuBarVisible:YES];
			
			// Fade to normal screen
			[SharedAppController doFadeOperation:CleanScreen time:0.5f sync:false token:&token];
			
			// Do not sync but sleep so that the Dock and the Finder desktop go back to normal
			// state before the end of the fade operation
			sf::Sleep(0.5f);
		} else {
			// Close the window
			[members->window close];
			
		}
		
		// Unregister ourselves from the event handler
		[[AppController sharedController] unregisterWindow:this];
	}
}


////////////////////////////////////////////////////////////
/// /see sfWindowImpl::EnableKeyRepeat
////////////////////////////////////////////////////////////
void WindowImplCocoa::EnableKeyRepeat(bool Enabled)
{
	useKeyRepeat = Enabled;
}


////////////////////////////////////////////////////////////
/// see WindowImpl::SetIcon
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetIcon(unsigned int Width, unsigned int Height, const Uint8* Pixels)
{
	// Nothing to do
}


////////////////////////////////////////////////////////////
/// Make some allocations and initializations
////////////////////////////////////////////////////////////
void WindowImplCocoa::Initialize(void)
{
	// Allocate mem for the private objc members
	members = new objc_members;
	bzero(members, sizeof(*members));
	
	// Needed to always have an autorelease pool as soon as application is launched
	ONCE([SharedAppController resetPool]);
	
	// Register application if needed and launch it
	ONCE([SharedAppController runApplication]);
}


////////////////////////////////////////////////////////////
/// Make the window
////////////////////////////////////////////////////////////
static SFWindow *MakeWindow(WindowSettings& params, unsigned long style, VideoMode& mode, NSString *title)
{
	SFWindow *window = nil;
	
	NSRect frame = NSMakeRect (0.0f, 0.0f, (float) mode.Width, (float) mode.Height);
	unsigned int mask = 0;
	
	// We grab options from WindowStyle and add them to our window mask
	if (style & Style::None || style & Style::Fullscreen) {
		mask |= NSBorderlessWindowMask;
		
		if (style & Style::Fullscreen) {
			// Check display mode and put new values in 'mode' if needed
			boolean_t exact = true;
			CFDictionaryRef properties = CGDisplayBestModeForParameters(kCGDirectMainDisplay, mode.BitsPerPixel,
																		mode.Width, mode.Height, &exact);
			
			if (!properties) {
				std::cerr << "*** SFML: unable to get a display mode with the given parameters" << std::endl; 
				return nil;
			}
			
			if (exact == false) {
				CFNumberGetValue((CFNumberRef) CFDictionaryGetValue(properties, kCGDisplayWidth),
								 kCFNumberIntType, &mode.Width);
				
				CFNumberGetValue((CFNumberRef) CFDictionaryGetValue(properties, kCGDisplayHeight),
								 kCFNumberIntType, &mode.Height);
				
				CFNumberGetValue((CFNumberRef) CFDictionaryGetValue(properties, kCGDisplayBitsPerPixel),
								 kCFNumberIntType, &mode.BitsPerPixel);
			}
		}
		
	} else {
		if (style & Style::Titlebar) {
			mask |= NSTitledWindowMask;
			mask |= NSMiniaturizableWindowMask;
		}
		
		if (style & Style::Resize) {
			mask |= NSTitledWindowMask;
			mask |= NSMiniaturizableWindowMask;
			mask |= NSResizableWindowMask;
		}
		
		if (style & Style::Close) {
			mask |= NSTitledWindowMask;
			mask |= NSClosableWindowMask;
			mask |= NSMiniaturizableWindowMask;
		}
	}
	
	// Now we make the window with the values we got
	// Note: defer flag set to NO to be able to use OpenGL in our window
	window =[massert([SFWindow alloc]) initWithContentRect:frame
												  styleMask:mask
													backing:NSBackingStoreBuffered
													  defer:NO];
	
	if (window != nil) {
		// We set title and window position
		[window setTitle:title];
		[window center];
	}
	
	return window;
}


////////////////////////////////////////////////////////////
/// Make the OpenGL pixel format from the given attributes
////////////////////////////////////////////////////////////
static NSOpenGLContext *MakeOpenGLContext(WindowSettings& params)
{
	NSOpenGLPixelFormat *pixFormat = nil;
	NSOpenGLContext *context = nil;
	unsigned idx = 0;
#if ENABLE_ANTIALIASING
	unsigned samplesIdx = 0;
#endif
	
	// Attributes list
	NSOpenGLPixelFormatAttribute attribs[15] = {(NSOpenGLPixelFormatAttribute) 0};
	
	// Accelerated, double buffered
	attribs[idx++] = NSOpenGLPFAClosestPolicy;
	attribs[idx++] = NSOpenGLPFADoubleBuffer;
	attribs[idx++] = NSOpenGLPFAAccelerated;
	
	// windowed context
	attribs[idx++] = NSOpenGLPFAWindow;
	
	// Color size ; usually 32 bits per pixel
	attribs[idx++] = NSOpenGLPFAColorSize;
	attribs[idx++] = (NSOpenGLPixelFormatAttribute) VideoMode::GetDesktopMode().BitsPerPixel;
	
	// Z-buffer size
	attribs[idx++] = NSOpenGLPFADepthSize;
	attribs[idx++] = (NSOpenGLPixelFormatAttribute) params.DepthBits;
	
	// Stencil bits (I don't really know what's that...)
	attribs[idx++] = NSOpenGLPFAStencilSize;
	attribs[idx++] = (NSOpenGLPixelFormatAttribute) params.StencilBits;
	
#if ENABLE_ANTIALIASING
	// Antialiasing settings
	if (params.AntialiasingLevel) {
		samplesIdx = idx;
		
		attribs[idx++] = NSOpenGLPFASamples;
		attribs[idx++] = (NSOpenGLPixelFormatAttribute) params.AntialiasingLevel;
		
		attribs[idx++] = NSOpenGLPFASampleBuffers;
		attribs[idx++] = (NSOpenGLPixelFormatAttribute) GL_TRUE;
	}
#endif
	
	pixFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
	
#if ENABLE_ANTIALIASING
	// If pixel format creation fails and antialiasing level is
	// greater than 2, we set it to 2.
	if (pixFormat == nil && params.AntialiasingLevel > 2) {
		std::cerr << "Failed to find a pixel format supporting " << params.AntialiasingLevel << " antialiasing levels ; trying with 2 levels" << std::endl;
		params.AntialiasingLevel = attribs[samplesIdx + 1] = (NSOpenGLPixelFormatAttribute) 2;
		
		pixFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
	}
	
	// If pixel format creation fails and antialiasing is enabled,
	// we disable it.
	if (pixFormat == nil && params.AntialiasingLevel > 0) {
		std::cerr << "Failed to find a pixel format supporting antialiasing ; antialiasing will be disabled" << std::endl;
		attribs[samplesIdx] = (NSOpenGLPixelFormatAttribute) nil;
		
		pixFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
	}
#endif
	
	if (pixFormat) {
		context = [[NSOpenGLContext alloc] initWithFormat:pixFormat
											 shareContext:[sharedContext retain]];
		
		// Get the effective properties from our OpenGL context
		GLint tmpDepthSize = 0, tmpStencilBits = 0, tmpAntialiasingLevel = 0;
		
		if (context) {
			[pixFormat getValues:&tmpDepthSize
					forAttribute:NSOpenGLPFADepthSize
				forVirtualScreen:[context currentVirtualScreen]];
			
			[pixFormat getValues:&tmpStencilBits
					forAttribute:NSOpenGLPFAStencilSize
				forVirtualScreen:[context currentVirtualScreen]];
			
			[pixFormat getValues:&tmpAntialiasingLevel
					forAttribute:NSOpenGLPFASamples
				forVirtualScreen:[context currentVirtualScreen]];
		}
		
		
		params.DepthBits = (unsigned) tmpDepthSize;
		params.StencilBits = (unsigned) tmpStencilBits;
		params.AntialiasingLevel = (unsigned) tmpAntialiasingLevel;
		
		[pixFormat release];
	}
	
	return context;
}


static NSOpenGLView	* MakeOpenGLView(SFWindow *window, NSOpenGLContext *context, WindowSettings& params)
{
	assert(window != nil);
	assert(context != nil);
	
	NSOpenGLView *view = nil;
	
	
	// We make the NSOpenGLView
	view = [[NSOpenGLView alloc] initWithFrame:[[window contentView] bounds]
								   pixelFormat:nil];
	
	if (view) {
		// We add the NSOpenGLView to the window
		[[window contentView] addSubview:view];
		
		[view setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
		[view clearGLContext];
		[view setOpenGLContext:context];
		[context setView:view];
	}
	
	return view;
}


static void ConfigureWindow(SFWindow *window, NSOpenGLView *view, WindowController *controller)
{
	assert(window != nil);
	assert(view != nil);
	assert(controller != nil);
	
	// We need to update the OpenGL view when it changes
	NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
	[nc addObserver:controller
		   selector:@selector(viewFrameDidChange:)
			   name:NSViewFrameDidChangeNotification
			 object:view];
	
	// We want to know when our window got the focus
	[nc addObserver:controller
		   selector:@selector(windowDidBecomeMain:)
			   name:NSWindowDidBecomeMainNotification
			 object:window];
	
	// We want to know when our window lost the focus
	[nc addObserver:controller
		   selector:@selector(windowDidResignMain:)
			   name:NSWindowDidResignMainNotification
			 object:window];
	
	// We want to know when the user closes the window
	[nc addObserver:controller
		   selector:@selector(windowWillClose:)
			   name:NSWindowWillCloseNotification
			 object:window];
	
	// I want to re-center the window if it's a full screen one and moved by Spaces
	[nc addObserver:controller
		   selector:@selector(windowDidMove:)
			   name:NSWindowDidMoveNotification
			 object:window];
	
	
	// Needed not to make application crash when releasing the window in our destructor
	// (I prefer to take control of everything :P)
	[window setReleasedWhenClosed:NO];
	[window setAcceptsMouseMovedEvents:YES];
}


////////////////////////////////////////////////////////////
/// Return the SFML key corresponding to a key code
////////////////////////////////////////////////////////////
static Key::Code KeyForVirtualCode(unsigned short vCode)
{
	static struct {
		unsigned short code;
		Key::Code sfKey;
	} virtualTable[] =
	{
		{0x35, Key::Escape},
		{0x31, Key::Space},
		{0x24, Key::Return},	// main Return key
		{0x4C, Key::Return},	// pav Return key
		{0x33, Key::Back},
		{0x30, Key::Tab},
		{0x74, Key::PageUp},
		{0x79, Key::PageDown},
		{0x77, Key::End},
		{0x73, Key::Home},
		{0x72, Key::Insert},
		{0x75, Key::Delete},
		{0x45, Key::Add},
		{0x4E, Key::Subtract},
		{0x43, Key::Multiply},
		{0x4B, Key::Divide},
		
		{0x7A, Key::F1}, {0x78, Key::F2}, {0x63, Key::F3},
		{0x76, Key::F4}, {0x60, Key::F5}, {0x61, Key::F6},
		{0x62, Key::F7}, {0x64, Key::F8}, {0x65, Key::F9},
		{0x6D, Key::F10}, {0x67, Key::F11}, {0x6F, Key::F12},
		{0x69, Key::F13}, {0x6B, Key::F14}, {0x71, Key::F15},
		
		{0x7B, Key::Left},
		{0x7C, Key::Right},
		{0x7E, Key::Up},
		{0x7D, Key::Down},
		
		{0x52, Key::Numpad0}, {0x53, Key::Numpad1}, {0x54, Key::Numpad2},
		{0x55, Key::Numpad3}, {0x56, Key::Numpad4}, {0x57, Key::Numpad5},
		{0x58, Key::Numpad6}, {0x59, Key::Numpad7}, {0x5B, Key::Numpad8},
		{0x5C, Key::Numpad9},
		
		{0x1D, Key::Num0}, {0x12, Key::Num1}, {0x13, Key::Num2},
		{0x14, Key::Num3}, {0x15, Key::Num4}, {0x17, Key::Num5},
		{0x16, Key::Num6}, {0x1A, Key::Num7}, {0x1C, Key::Num8},
		{0x19, Key::Num9},
		
		{0x3B, Key::LControl},	//< Left Ctrl
		{0x3A, Key::LAlt},		//< Left Option/Alt
		{0x37, Key::LSystem},	//< Left Command
		{0x38, Key::LShift},	//< Left Shift
		{0x3E, Key::RControl},	//< Right Ctrl
		{0x3D, Key::RAlt},		//< Right Option/Alt
		{0x36, Key::RSystem},	//< Right Command
		{0x3C, Key::RShift},	//< Right Shift
		
		{0x39, Key::Code(0)}	//< Caps Lock
	};
	
	Key::Code result = Key::Code(0);
	
	for (unsigned i = 0;virtualTable[i].code;i++) {
		if (virtualTable[i].code == vCode) {
			result = virtualTable[i].sfKey;
			break;
		}
	}
	
	return result;
}


////////////////////////////////////////////////////////////
/// Return the SFML key corresponding to a unicode code
////////////////////////////////////////////////////////////
static Key::Code KeyForUnicode(unsigned short uniCode)
{
	static struct {
		unsigned short character;
		Key::Code sfKey;
	} unicodeTable[] =
	{
		{'!', Key::Code(0)}, //< No Key for this code
		{'"', Key::Code(0)}, //< No Key for this code
		{'#', Key::Code(0)}, //< No Key for this code
		{'$', Key::Code(0)}, //< No Key for this code
		{'%', Key::Code(0)}, //< No Key for this code
		{'&', Key::Code(0)}, //< No Key for this code
		{'\'', Key::Quote},
		{'(', Key::Code(0)}, //< No Key for this code
		{')', Key::Code(0)}, //< No Key for this code
		{'*', Key::Multiply},
		{'+', Key::Add},
		{',', Key::Comma},
		{'-', Key::Code(0)}, //< Handled by KeyForVirtualCode()
		{'.', Key::Period},
		{'/', Key::Code(0)}, //< Handled by KeyForVirtualCode()
		{'0', Key::Code(0)}, //< Handled by KeyForVirtualCode()
		{'1', Key::Code(0)}, //< Handled by KeyForVirtualCode()
		{'2', Key::Code(0)}, //< Handled by KeyForVirtualCode()
		{'3', Key::Code(0)}, //< Handled by KeyForVirtualCode()
		{'4', Key::Code(0)}, //< Handled by KeyForVirtualCode()
		{'5', Key::Code(0)}, //< Handled by KeyForVirtualCode()
		{'6', Key::Code(0)}, //< Handled by KeyForVirtualCode()
		{'7', Key::Code(0)}, //< Handled by KeyForVirtualCode()
		{'8', Key::Code(0)}, //< Handled by KeyForVirtualCode()
		{'9', Key::Code(0)}, //< Handled by KeyForVirtualCode()
		{':', Key::Code(0)}, //< No Key for this code
		{';', Key::SemiColon},
		{'<', Key::Code(0)}, //< No Key for this code
		{'=', Key::Equal},
		{'>', Key::Code(0)}, //< No Key for this code
		{'?', Key::Code(0)}, //< No Key for this code
		{'@', Key::Code(0)}, //< No Key for this code
		{'A', Key::A}, {'B', Key::B}, {'C', Key::C},
		{'D', Key::D}, {'E', Key::E}, {'F', Key::F},
		{'G', Key::G}, {'H', Key::H}, {'I', Key::I},
		{'J', Key::J}, {'K', Key::K}, {'L', Key::L},
		{'M', Key::M}, {'N', Key::N}, {'O', Key::O},
		{'P', Key::P}, {'Q', Key::Q}, {'R', Key::R},
		{'S', Key::S}, {'T', Key::T}, {'U', Key::U},
		{'V', Key::V}, {'W', Key::W}, {'X', Key::X},
		{'Y', Key::Y}, {'Z', Key::Z},
		{'[', Key::LBracket},
		{'\\', Key::BackSlash},
		{']', Key::RBracket},
		{'^', Key::Code(0)}, //< No Key for this code
		{'_', Key::Code(0)}, //< No Key for this code
		{'`', Key::Code(0)}, //< No Key for this code
		{'a', Key::A}, {'b', Key::B}, {'c', Key::C},
		{'d', Key::D}, {'e', Key::E}, {'f', Key::F},
		{'g', Key::G}, {'h', Key::H}, {'i', Key::I},
		{'j', Key::J}, {'k', Key::K}, {'l', Key::L},
		{'m', Key::M}, {'n', Key::N}, {'o', Key::O},
		{'p', Key::P}, {'q', Key::Q}, {'r', Key::R},
		{'s', Key::S}, {'t', Key::T}, {'u', Key::U},
		{'v', Key::V}, {'w', Key::W}, {'x', Key::X},
		{'y', Key::Y}, {'z', Key::Z},
		{'{', Key::Code(0)}, //< No Key for this code
		{'|', Key::Code(0)}, //< No Key for this code
		{'}', Key::Code(0)}, //< No Key for this code
		{'~', Key::Tilde},
		{0, Key::Code(0)}
	};
	
	Key::Code result = Key::Code(0);
	
	for (unsigned i = 0;unicodeTable[i].character;i++) {
		if (unicodeTable[i].character == uniCode) {
			result = unicodeTable[i].sfKey;
			break;
		}
	}
	
	return result;
}


////////////////////////////////////////////////////////////
/// Return whether 'ev' must be considered as a TextEntered event
////////////////////////////////////////////////////////////
static bool IsTextEvent(NSEvent *event)
{
	bool res = false;
	
	if (event && [event type] == NSKeyDown && [[event characters] length]) {
		unichar code = [[event characters] characterAtIndex:0];
		
		// Codes from 0xF700 to 0xF8FF are non text keys (see NSEvent.h)
		if (code < 0xF700 || code > 0xF8FF)
			res = true;
	}
	
	return res;
}

	
////////////////////////////////////////////////////////////
/// Return whether the mouse is on our OpenGL view
////////////////////////////////////////////////////////////
static bool MouseInside(SFWindow *window, NSView *view)
{
	bool res = false;
	
	if (window && view && [window isVisible]) {
		NSPoint relativeToWindow = [window mouseLocationOutsideOfEventStream];
		NSPoint relativeToView = [view convertPoint:relativeToWindow fromView:nil];
		
		if (NSPointInRect (relativeToView, [view bounds]))
			res = true;
	}
	
	return res;
}


////////////////////////////////////////////////////////////
/// Return the mouse location in the SFML coordinates according to 'window'
////////////////////////////////////////////////////////////
static NSPoint MouseLocation(SFWindow *window)
{
	NSPoint location = [NSEvent mouseLocation];
	NSPoint	relativeLocation = {0, 0};
	
	if (window) {
		relativeLocation = [window convertScreenToBase:location];
		relativeLocation.y = [[window contentView] frame].size.height - relativeLocation.y;
	} else {
		std::cerr << "*** SFML: tried to get mouse location from no window" << std::endl;
	}
	
	return relativeLocation;
}

} // namespace priv

} // namespace sf

