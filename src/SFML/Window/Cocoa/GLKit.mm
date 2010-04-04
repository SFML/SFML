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
#import <SFML/Window/Cocoa/GLKit.h>
#import <SFML/Window/Cocoa/AppController.h>
#import <SFML/Window/VideoMode.hpp>
#import <SFML/Window/WindowStyle.hpp>
#import <OpenGL/gl.h>
#import <iostream>


@interface sfPrivWindow (Protected)

////////////////////////////////////////////////////////////
/// Registers a reference to the internal Cocoa window
////////////////////////////////////////////////////////////
- (void)setWindow:(NSWindow *)aWindow;

////////////////////////////////////////////////////////////
/// Registers the the OpenGL view and adds it to its parent container
////////////////////////////////////////////////////////////
- (void)putOpenGLView:(sfPrivGLView *)aView;

////////////////////////////////////////////////////////////
/// Registers a reference to the internal Cocoa OpenGL view
////////////////////////////////////////////////////////////
- (void)setView:(sfPrivGLView *)aView;

@end

@interface sfPrivOwnedWindow (Private)

////////////////////////////////////////////////////////////
/// Sets the window's fullscreen state
////////////////////////////////////////////////////////////
- (void)setFullscreen:(BOOL)aFlag;

@end


////////////////////////////////////////////////////////////
/// Window independant OpenGL context class
////////////////////////////////////////////////////////////
@implementation sfPrivGLContext


////////////////////////////////////////////////////////////
/// Return the shared OpenGL context instance (making one if needed)
////////////////////////////////////////////////////////////
+ (id)sharedContext
{
	// Make a new context with the default parameters
	sf::WindowSettings params;
	sf::VideoMode m;
	static sfPrivGLContext *sharedCtx = [[sfPrivGLContext alloc] initWithAttributes:params
																			   mode:m
																	  sharedContext:nil];
	return sharedCtx;
}

- (void)dealloc
{
	[[sfPrivGLContext sharedContext] release];
	[super dealloc];
}

////////////////////////////////////////////////////////////
/// Make a new OpenGL context according to the @attribs settings
/// and the shared context @context
////////////////////////////////////////////////////////////
- (id)initWithAttributes:(sf::WindowSettings&)attribs mode:(const sf::VideoMode&)mode sharedContext:(sfPrivGLContext *)sharedContext
{
	// Note about antialiasing and other context attributes :
	// OpenGL context sharing does not allow the shared contexts to use different attributes.
	// The point is that the default shared global OpenGL context uses default parameters.
	// That means that all the other context *should* use the same paramaters.
	// Fortunately some values parameters for some parameters are compatible, but some are not
	// among which : the antialising level.
	//
	// I've no way to fix this for now.
	
	if (attribs.AntialiasingLevel) {
		NSLog(@"Warning: antialiasing settings are inhibited on Mac OS X for technical reasons");
	}
	
	if (attribs.StencilBits != 8) {
		NSLog(@"Warning: stencil bits settings are inhibited on Mac OS X for technical reasons");
	}
	
	NSOpenGLPixelFormat *myPixelFormat = nil;
	unsigned idx = 0;
	
	// Attributes list
	NSOpenGLPixelFormatAttribute ctxtAttribs[15] = {(NSOpenGLPixelFormatAttribute) 0};
	
	// Accelerated, double buffered
	ctxtAttribs[idx++] = NSOpenGLPFAClosestPolicy;
	ctxtAttribs[idx++] = NSOpenGLPFADoubleBuffer;
	ctxtAttribs[idx++] = NSOpenGLPFAAccelerated;
	
	// Force use of first screen
	//ctxtAttribs[idx++] = NSOpenGLPFAScreenMask;
	//ctxtAttribs[idx++] = CGDisplayIDToOpenGLDisplayMask([sfPrivAppController primaryScreen]);
	
	// windowed context (even fullscreen mode uses a window)
	ctxtAttribs[idx++] = NSOpenGLPFAWindow;
	
	// Color buffer bits ; usually 32 bits per pixel
	ctxtAttribs[idx++] = NSOpenGLPFAColorSize;
	
	// well.. whatever I put here, the color size is always 24 bits on my computer..
	ctxtAttribs[idx++] = (NSOpenGLPixelFormatAttribute) mode.BitsPerPixel;
	
	// Alpha buffer size
	// FIXME: I don't really remember whether I can safely use the user video mode settings
	// to set this attribute (because of the shared context attributes conflicts)
	if (mode.BitsPerPixel > 24) {
		ctxtAttribs[idx++] = NSOpenGLPFAAlphaSize;
		ctxtAttribs[idx++] = (NSOpenGLPixelFormatAttribute) 8;
	}
	
	// Depth buffer size
	ctxtAttribs[idx++] = NSOpenGLPFADepthSize;
	ctxtAttribs[idx++] = (NSOpenGLPixelFormatAttribute) attribs.DepthBits;
	
	// Stencil buffer bits
	// Note: even with "NSOpenGLPFAClosestPolicy" set, the NSOpenGLPFAStencilSize value must be an valid and exact one
	// I've been looking for possible diffferent default stencil bits values among different Apple computers
	// but I can't again get the website I found oneaday
	// (describing all the graphic cards used by Apple, the supported extensions, etc).
	ctxtAttribs[idx++] = NSOpenGLPFAStencilSize;
	ctxtAttribs[idx++] = (NSOpenGLPixelFormatAttribute) /* attribs.StencilBits */ 8;
	
	myPixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:ctxtAttribs];
	
	if (myPixelFormat) {
		self = [super initWithFormat:myPixelFormat
						shareContext:[sharedContext retain]];
		
		// Get the effective properties from our OpenGL context
		GLint tmpDepthSize = 0, tmpStencilBits = 0, tmpAntialiasingLevel = 0;
		
		if (self) {
			[myPixelFormat getValues:&tmpDepthSize
					forAttribute:NSOpenGLPFADepthSize
				forVirtualScreen:[self currentVirtualScreen]];
			
			[myPixelFormat getValues:&tmpStencilBits
					forAttribute:NSOpenGLPFAStencilSize
				forVirtualScreen:[self currentVirtualScreen]];
			
			[myPixelFormat getValues:&tmpAntialiasingLevel
					forAttribute:NSOpenGLPFASamples
				forVirtualScreen:[self currentVirtualScreen]];
		}
		
		
		attribs.DepthBits = (unsigned) tmpDepthSize;
		attribs.StencilBits = (unsigned) tmpStencilBits;
		attribs.AntialiasingLevel = (unsigned) tmpAntialiasingLevel;
		
		[myPixelFormat release];
	} else {
		NSLog(@"NSOpenGLPixelFormat creation failed! (invalid video settings ?)");
	}
	
	return self;
}

@end


////////////////////////////////////////////////////////////
/// Customized Cocoa OpenGL view
////////////////////////////////////////////////////////////
@implementation sfPrivGLView

////////////////////////////////////////////////////////////
/// Make a new view according the the rect @frame,
/// the video mode @mode, the window settings @settings
/// and the sf window delegate @delegate
/// @delegate must not be null
////////////////////////////////////////////////////////////
- (id)initWithFrame:(NSRect)frame
			   mode:(const sf::VideoMode&)mode
		   settings:(sf::WindowSettings&)settings
{
	// make the view
	self = [super initWithFrame:frame pixelFormat:nil];
	
	if (self)
	{
		// enabled auto-resizing
		[self setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
		
		// make the OpenGL context
		myGLContext = [[sfPrivGLContext alloc] initWithAttributes:settings
															 mode:mode
											  sharedContext:[sfPrivGLContext sharedContext]];
		
		if (!myGLContext) {
			[self release];
			return nil;
		}
		
		// We need to update the OpenGL view when it's resized
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		[nc addObserver:self
			   selector:@selector(viewFrameDidChange:)
				   name:NSViewFrameDidChangeNotification
				 object:self];
	}
	
	return self;
}


////////////////////////////////////////////////////////////
/// Clean the instance
////////////////////////////////////////////////////////////
- (void)dealloc
{
	// Remove the observer and release the OpenGL context
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	[myGLContext release];
	
	[super dealloc];
}

////////////////////////////////////////////////////////////
/// Sets @aDelegate as the view delegate
////////////////////////////////////////////////////////////
- (void)setDelegate:(sf::priv::WindowImplCocoa *)aDelegate
{
	myDelegate = aDelegate;
}

////////////////////////////////////////////////////////////
/// Returns the view delegate
////////////////////////////////////////////////////////////
- (sf::priv::WindowImplCocoa *)delegate
{
	return myDelegate;
}


////////////////////////////////////////////////////////////
/// Finish view setting (after having added it to the window)
////////////////////////////////////////////////////////////
- (void)finishInitialization
{
	assert([self superview] != nil);
	assert(myGLContext != nil);
	
	// Attach the OpenGL context to our view
	[self clearGLContext];
	[self setOpenGLContext:myGLContext];
	[myGLContext setView:self];
	
	// Make our view the first responder
	[[self window] makeFirstResponder:self];
}


////////////////////////////////////////////////////////////
/// Forward call to en/disable vertical synchronization
////////////////////////////////////////////////////////////
- (void)enableVerticalSync:(bool)flag
{
	GLint enable = (flag) ? 1 : 0;
	[[self openGLContext] setValues:&enable forParameter:NSOpenGLCPSwapInterval];
}


////////////////////////////////////////////////////////////
/// Forward call to set the OpenGL context as active according to @flag
////////////////////////////////////////////////////////////
- (void)setActive:(bool)flag
{
	if (flag) {
		if ([NSOpenGLContext currentContext] != [self openGLContext])
			[[self openGLContext] makeCurrentContext];
	} else {
		if ([NSOpenGLContext currentContext] == [self openGLContext])
			[NSOpenGLContext clearCurrentContext];
	}
}


////////////////////////////////////////////////////////////
/// Forward call to flush the OpenGL context
////////////////////////////////////////////////////////////
- (void)flushBuffer
{
	[[self openGLContext] flushBuffer];
}


////////////////////////////////////////////////////////////
/// Send event to the linked window
////////////////////////////////////////////////////////////
- (void)pushEvent:(sf::Event)sfEvent
{
	assert(myDelegate != NULL);
	myDelegate->HandleNotifiedEvent(sfEvent);
}


////////////////////////////////////////////////////////////
/// Notification method receiver when OpenGL view size changes
////////////////////////////////////////////////////////////
- (void)viewFrameDidChange:(NSNotification *)notification
{
	[self update];
	
	sf::Event ev;
	ev.Type = sf::Event::Resized;
	ev.Size.Width = (unsigned) [self frame].size.width;
	ev.Size.Height = (unsigned) [self frame].size.height;
	
	[self pushEvent:ev];
}


////////////////////////////////////////////////////////////
/// Receiver method called when a key is pressed
////////////////////////////////////////////////////////////
- (void)keyDown:(NSEvent *)theEvent
{
	assert(myDelegate != NULL);
	
	if (sf::priv::WindowImplCocoa::IsTextEvent((void *)theEvent))
	{
		NSText *field = [[self window] fieldEditor:YES forObject:nil];
		[field interpretKeyEvents:[NSArray arrayWithObject:theEvent]];
		[field setString:@""];
	}
	
	myDelegate->HandleKeyDown(theEvent);
}


////////////////////////////////////////////////////////////
/// Receiver method called when a key is released
////////////////////////////////////////////////////////////
- (void)keyUp:(NSEvent *)theEvent
{
	assert(myDelegate != NULL);
	myDelegate->HandleKeyUp(theEvent);
}


////////////////////////////////////////////////////////////
/// Receiver method called when a modifier flag has changed
////////////////////////////////////////////////////////////
- (void)flagsChanged:(NSEvent *)theEvent
{
	assert(myDelegate != NULL);
	myDelegate->HandleModifierKey(theEvent);
}


////////////////////////////////////////////////////////////
/// Receiver method called when the mouse wheel has been used
////////////////////////////////////////////////////////////
- (void)scrollWheel:(NSEvent *)theEvent
{
	assert(myDelegate != NULL);
	myDelegate->HandleMouseWheel(theEvent);
}


////////////////////////////////////////////////////////////
/// Receiver method called when left mouse click is pressed
////////////////////////////////////////////////////////////
- (void)mouseDown:(NSEvent *)theEvent
{
	assert(myDelegate != NULL);
	myDelegate->HandleMouseDown(theEvent);
}


////////////////////////////////////////////////////////////
/// Receiver method called when right mouse click is pressed
////////////////////////////////////////////////////////////
- (void)rightMouseDown:(NSEvent *)theEvent
{
	assert(myDelegate != NULL);
	myDelegate->HandleMouseDown(theEvent);
}


////////////////////////////////////////////////////////////
/// Receiver method called when left mouse click is released
////////////////////////////////////////////////////////////
- (void)mouseUp:(NSEvent *)theEvent
{
	assert(myDelegate != NULL);
	myDelegate->HandleMouseUp(theEvent);
}


////////////////////////////////////////////////////////////
/// Receiver method called when right mouse click is released
////////////////////////////////////////////////////////////
- (void)rightMouseUp:(NSEvent *)theEvent
{
	assert(myDelegate != NULL);
	myDelegate->HandleMouseUp(theEvent);
}


////////////////////////////////////////////////////////////
/// Receiver method called when mouse moves
////////////////////////////////////////////////////////////
- (void)mouseMoved:(NSEvent *)theEvent
{
	assert(myDelegate != NULL);
	myDelegate->HandleMouseMove(theEvent);
}

////////////////////////////////////////////////////////////
/// Receiver method called when mouse is pressed (on left button) and moves
////////////////////////////////////////////////////////////
- (void)mouseDragged:(NSEvent *)theEvent
{
	assert(myDelegate != NULL);
	myDelegate->HandleMouseMove(theEvent);
}

////////////////////////////////////////////////////////////
/// Receiver method called when mouse is pressed (on right button) and moves
////////////////////////////////////////////////////////////
- (void)rightMouseDragged:(NSEvent *)theEvent
{
	assert(myDelegate != NULL);
	myDelegate->HandleMouseMove(theEvent);
}


////////////////////////////////////////////////////////////
/// Tells that the view can be focused
////////////////////////////////////////////////////////////
- (BOOL)acceptsFirstResponder
{
	return YES;
}


////////////////////////////////////////////////////////////
/// Tells that the view can become the key responder (ie. can catch key events)
////////////////////////////////////////////////////////////
- (BOOL)canBecomeKeyView
{
	return YES;
}

@end


////////////////////////////////////////////////////////////
/// Cocoa window category to let fullscreen windows
/// catch key events
////////////////////////////////////////////////////////////
@implementation NSWindow (GLWindow)

- (BOOL)canBecomeKeyWindow
{
	return YES;
}

- (BOOL)canBecomeMainWindow
{
	return YES;
}

@end



@implementation sfPrivWindow

- (void)dealloc
{
	NSAutoreleasePool *localPool = [[NSAutoreleasePool alloc] init];
	// Remove the notification observer
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	
	// Close the window
	[self show:false];
	
	// Release the window and view
	[[self view] removeFromSuperviewWithoutNeedingDisplay];
	
	[self setWindow:nil];
	[self setView:nil];
	[self setDelegate:nil];
	
	[super dealloc];
	[localPool release];
}

////////////////////////////////////////////////////////////
/// Registers a reference to the internal Cocoa window
////////////////////////////////////////////////////////////
- (void)setWindow:(NSWindow *)aWindow
{
	if (myWindow != aWindow)
	{
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		
		// Drop the observers on the previously set window
		if ([self window]) {
			[nc removeObserver:self 
						  name:NSWindowDidBecomeMainNotification
						object:[self window]];
			[nc removeObserver:self 
						  name:NSWindowDidResignMainNotification
						object:[self window]];
			[nc removeObserver:self 
						  name:NSWindowWillCloseNotification
						object:[self window]];
		}
		
		[myWindow release];
		myWindow = [aWindow retain];
		
		if (myWindow) {
			// Set the new observers
			// We want to know when our window got the focus
			[nc addObserver:self
				   selector:@selector(windowDidBecomeMain:)
					   name:NSWindowDidBecomeMainNotification
					 object:[self window]];
			
			// We want to know when our window lost the focus
			[nc addObserver:self
				   selector:@selector(windowDidResignMain:)
					   name:NSWindowDidResignMainNotification
					 object:[self window]];
			
			// We want to know when the user closes the window
			[nc addObserver:self
				   selector:@selector(windowWillClose:)
					   name:NSWindowWillCloseNotification
					 object:[self window]];
		}
	}
}

////////////////////////////////////////////////////////////
/// Return a reference to the internal Cocoa window
////////////////////////////////////////////////////////////
- (NSWindow *)window
{
	return [[myWindow retain] autorelease];
}


////////////////////////////////////////////////////////////
/// Registers the the OpenGL view and adds it to its parent container
////////////////////////////////////////////////////////////
- (void)putOpenGLView:(sfPrivGLView *)aView
{
	[self setView:aView];
	
	// Finish setting up the view and window
	// Add the view to our window and tell it to the view
	[[[self window] contentView] addSubview:[self view]];
	[[self view] finishInitialization];
}

////////////////////////////////////////////////////////////
/// Registers a reference to the internal Cocoa OpenGL view
////////////////////////////////////////////////////////////
- (void)setView:(sfPrivGLView *)aView
{
	if (myView != aView)
	{
		[myView release];
		myView = [aView retain];
	}
}

////////////////////////////////////////////////////////////
/// Return a reference to the internal Cocoa OpenGL view
////////////////////////////////////////////////////////////
- (sfPrivGLView *)view
{
	return [[myView retain] autorelease];
}

////////////////////////////////////////////////////////////
/// Sets @aDelegate as the window delegate
////////////////////////////////////////////////////////////
- (void)setDelegate:(sf::priv::WindowImplCocoa *)aDelegate
{
	[[self view] setDelegate:aDelegate];
}

////////////////////////////////////////////////////////////
/// Returns the window delegate
////////////////////////////////////////////////////////////
- (sf::priv::WindowImplCocoa *)delegate
{
	return [[self view] delegate];
}

////////////////////////////////////////////////////////////
/// Forward call to set the window position on screen
////////////////////////////////////////////////////////////
- (void)setPosition:(NSPoint)pos
{
	NSAssert([self window] != nil, @"expected valid window");
	
	// Flip Y and set window position
	pos.y = [[[self window] screen] frame].size.height - pos.y;
	[[self window] setFrameTopLeftPoint:pos];
}

////////////////////////////////////////////////////////////
/// Forward call to set the window size
////////////////////////////////////////////////////////////
- (void)setSize:(NSSize)size
{
	NSAssert([self window] != nil, @"expected valid window");
	
	[[self window] setFrame:NSMakeRect([[self window] frame].origin.x,
									   [[self window] frame].origin.y,
									   size.width, size.height)
					display:YES];
}

////////////////////////////////////////////////////////////
/// Return the mouse location relative to the internal window
////////////////////////////////////////////////////////////
- (NSPoint)mouseLocation
{
	NSAssert([self view] != nil, @"expected valid window");
	
	NSPoint windowPoint = [[self window] convertScreenToBase:[NSEvent mouseLocation]];
	NSPoint viewPoint = [[self view] convertPoint:windowPoint fromView:[[[self window] contentView] superview]];
	
	viewPoint.y = [[self view] frame].size.height - viewPoint.y;
	return viewPoint;
}

////////////////////////////////////////////////////////////
/// Return whether the mouse is on our window
////////////////////////////////////////////////////////////
- (BOOL)mouseInside
{
	NSAssert([self window] != nil, @"expected valid window");
	NSAssert([self view] != nil, @"expected valid OpenGL view");
	
	BOOL flag = NO;
	
	if ([[self window] isVisible]) {
		NSPoint relativeToWindow = [[self window] mouseLocationOutsideOfEventStream];
		NSPoint relativeToView = [[self view] convertPoint:relativeToWindow fromView:nil];
		
		if (NSPointInRect (relativeToView, [[self view] bounds]))
		{
			flag = YES;
		}
	}
	
	return flag;
}

////////////////////////////////////////////////////////////
/// Close or open the window
////////////////////////////////////////////////////////////
- (void)show:(bool)flag
{
	NSAssert([self window] != nil, @"expected valid window");
	
	if (flag && ![[self window] isVisible]) {
		// Wanna open the closed window
		
		// Show the window
		[[self window] makeKeyAndOrderFront:nil];
	} else if (!flag && [[self window] isVisible]) {
		// Wanna close the opened window
		
		// Close the window
		[[self window] close];
	}	
}


////////////////////////////////////////////////////////////
/// Forward call to en/disable the OpenGL view vertical synchronization
////////////////////////////////////////////////////////////
- (void)enableVerticalSync:(bool)flag
{
	NSAssert([self view] != nil, @"expected valid OpenGL view");
	[[self view] enableVerticalSync:flag];
}

////////////////////////////////////////////////////////////
/// Forward 'setActive' call the the OpenGL view
////////////////////////////////////////////////////////////
- (void)setActive:(bool)flag
{
	NSAssert([self view] != nil, @"expected valid OpenGL view");
	[[self view] setActive:flag];
}

////////////////////////////////////////////////////////////
/// Forward call to flush the OpenGL view
////////////////////////////////////////////////////////////
- (void)flushBuffer
{
	NSAssert([self view] != nil, @"expected valid OpenGL view");
	[[self view] flushBuffer];
}

////////////////////////////////////////////////////////////
/// Notification method receiver when the window gains focus
////////////////////////////////////////////////////////////
- (void)windowDidBecomeMain:(NSNotification *)notification
{
	sf::Event ev;
	ev.Type = sf::Event::GainedFocus;
	
	[[self view] pushEvent:ev];
}


////////////////////////////////////////////////////////////
/// Notification method receiver when the window loses focus
////////////////////////////////////////////////////////////
- (void)windowDidResignMain:(NSNotification *)notification
{
	sf::Event ev;
	ev.Type = sf::Event::LostFocus;
	
	[[self view] pushEvent:ev];
}


////////////////////////////////////////////////////////////
/// Notification method receiver when the window closes
////////////////////////////////////////////////////////////
- (void)windowWillClose:(NSNotification *)notification
{
	sf::Event ev;
	ev.Type = sf::Event::Closed;
	
	[[self view] pushEvent:ev];
}


@end


@implementation sfPrivOwnedWindow

- (id)initWithVideoMode:(sf::VideoMode&)aMode
			   settings:(sf::WindowSettings&)someSettings
				  style:(unsigned long)aStyle
				  title:(NSString *)aTitle
{
	self = [super init];
	if (self)
	{
		if (aStyle & sf::Style::Fullscreen) {
			[self setFullscreen:YES];
		}
		
		NSRect frame = NSMakeRect (0.0f, 0.0f, (float) aMode.Width, (float) aMode.Height);
		unsigned int mask = 0;
		
		if (aStyle & sf::Style::Fullscreen) {
			// Check display mode and put new values in 'mode' if needed
			boolean_t exact = true;
			
			CFDictionaryRef properties =
			CGDisplayBestModeForParameters([sfPrivAppController primaryScreen],
										   aMode.BitsPerPixel,
										   aMode.Width,
										   aMode.Height,
										   &exact);
			
			if (!properties) {
				std::cerr 
				<< "Unable to get a display mode with the given parameters" 
				<< std::endl;
				
				[self autorelease];
				return nil;
			}
			
			if (exact == false) {
				CFNumberGetValue((CFNumberRef) CFDictionaryGetValue(properties,
																	kCGDisplayWidth),
								 kCFNumberIntType, &aMode.Width);
				
				CFNumberGetValue((CFNumberRef) CFDictionaryGetValue(properties,
																	kCGDisplayHeight),
								 kCFNumberIntType, &aMode.Height);
				
				CFNumberGetValue((CFNumberRef) CFDictionaryGetValue(properties,
																	kCGDisplayBitsPerPixel),
								 kCFNumberIntType, &aMode.BitsPerPixel);
				
			}
		}
		
		// We grab options from WindowStyle and add them to our window mask
		if (aStyle & sf::Style::None || aStyle & sf::Style::Fullscreen) {
			mask |= NSBorderlessWindowMask;
		} else {
			if (aStyle & sf::Style::Titlebar) {
				mask |= NSTitledWindowMask;
				mask |= NSMiniaturizableWindowMask;
			}
			
			if (aStyle & sf::Style::Resize) {
				mask |= NSTitledWindowMask;
				mask |= NSMiniaturizableWindowMask;
				mask |= NSResizableWindowMask;
			}
			
			if (aStyle & sf::Style::Close) {
				mask |= NSTitledWindowMask;
				mask |= NSClosableWindowMask;
				mask |= NSMiniaturizableWindowMask;
			}
		}
		
		// Now we make the window with the values we got
		// Note: defer flag set to NO to be able to use OpenGL in our window
		NSWindow *newWindow = [[NSWindow alloc]
							   initWithContentRect:frame
							   styleMask:mask
							   backing:NSBackingStoreBuffered
							   defer:NO];
		
		if (newWindow) {
			[self setWindow:[newWindow autorelease]];
		} else {
			std::cerr << "Unable to create the Cocoa window" << std::endl;
			[self autorelease];
			return nil;
		}
		
		// We set title and window position
		[[self window] setTitle:aTitle != nil ? aTitle : @""];
		[[self window] center];
		
		// Make the OpenGL view
		sfPrivGLView *newView = [[sfPrivGLView alloc]
						   initWithFrame:[[[self window] contentView] frame]
						   mode:aMode
						   settings:someSettings];
		
		if (!newView) {
			std::cerr << "Unable to create the OpenGL view" << std::endl;
			[self autorelease];
			return nil;
		}
		
		// Put our view in the window 
		[self putOpenGLView:[newView autorelease]];
		
		// I want to re-center the window if it's a full screen one and moved by Spaces
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(windowDidMove:)
		 name:NSWindowDidMoveNotification
		 object:[self window]];
		
		// Needed not to make application crash when releasing the window in our destructor
		// (I prefer to take control of everything :P)
		[[self window] setReleasedWhenClosed:NO];
		[[self window] setAcceptsMouseMovedEvents:YES];
		
		if ([self isFullscreen]) {
			myFullscreenMode = aMode;
			
			// Using this because full screen window was not always
			// in front of the other application windows when unhiding app
			[[self window] setLevel:NSFloatingWindowLevel];
		}
	}
	return self;
}

- (void)dealloc
{
	[[NSNotificationCenter defaultCenter]
	 removeObserver:self 
	 name:NSWindowDidMoveNotification
	 object:[self window]];
	[super dealloc];
}

////////////////////////////////////////////////////////////
/// Sets the window's fullscreen state
////////////////////////////////////////////////////////////
- (void)setFullscreen:(BOOL)aFlag
{
	myIsFullscreen = aFlag;
}

////////////////////////////////////////////////////////////
/// Returns the window's fullscreen state
////////////////////////////////////////////////////////////
- (BOOL)isFullscreen
{
	return myIsFullscreen;
}

////////////////////////////////////////////////////////////
/// Notification method receiver when the window finish moving
////////////////////////////////////////////////////////////
- (void)windowDidMove:(NSNotification *)notification
{
	NSWindow *sender = [notification object];
	
	if (myIsFullscreen)
		[sender center];
}

////////////////////////////////////////////////////////////
/// Close or open the window
////////////////////////////////////////////////////////////
- (void)show:(bool)flag
{
	NSAssert([self window] != nil, @"expected valid window");
	
	if (flag && ![[self window] isVisible]) {
		// Wanna open the closed window
		
		if ([self isFullscreen]) {
			[[sfPrivAppController sharedController]
			 setFullscreenWindow:self
			 mode:&myFullscreenMode];
		} else {
			// Show the window
			[[self window] makeKeyAndOrderFront:nil];
		}
	} else if (!flag && [[self window] isVisible]) {
		// Wanna close the opened window
		
		if ([self isFullscreen]) {
			[[sfPrivAppController sharedController]
			 setFullscreenWindow:nil
			 mode:NULL];
		} else {
			// Close the window
			[[self window] close];
		}
	}	
}

////////////////////////////////////////////////////////////
/// Forward call to set the window position on screen
////////////////////////////////////////////////////////////
- (void)setPosition:(NSPoint)aPosition
{
	if (![self isFullscreen]) {
		[super setPosition:aPosition];
	}
}

////////////////////////////////////////////////////////////
/// Forward call to set the window size
////////////////////////////////////////////////////////////
- (void)setSize:(NSSize)size
{
	if (![self isFullscreen]) {
		[super setSize:size];
	}
}

@end


@implementation sfPrivImportedWindow

- (id)initWithWindow:(NSWindow *)aWindow
			settings:(sf::WindowSettings&)someSettings
{
	self = [super init];
	
	if (self) {
		[self setWindow:aWindow];
		
		// Make the OpenGL view
		sf::VideoMode mode([[[self window] contentView] frame].size.width,
						   [[[self window] contentView] frame].size.height);
		sfPrivGLView *newView = [[sfPrivGLView alloc]
						   initWithFrame:[[[self window] contentView] frame]
						   mode:mode
						   settings:someSettings];
		
		if (!newView) {
			std::cerr << "Unable to create the OpenGL view" << std::endl;
			[self autorelease];
			return nil;
		}
		
		[self putOpenGLView:[newView autorelease]];
		
		[[self window] setAcceptsMouseMovedEvents:YES];
	}
	
	return self;
}

@end


@implementation sfPrivImportedView


- (id)initWithView:(NSView *)aView
		  settings:(sf::WindowSettings&)someSettings
{	
	self = [super init];
	if (self)
	{
		parentView = [aView retain];
		[self setWindow:[parentView window]];
		
		// Make the OpenGL view
		sf::VideoMode mode([parentView bounds].size.width,
						   [parentView bounds].size.height);
		sfPrivGLView *newView = [[sfPrivGLView alloc]
						   initWithFrame:[parentView bounds]
						   mode:mode
						   settings:someSettings];
		
		if (!newView) {
			std::cerr << "Unable to create the OpenGL view" << std::endl;
			[self autorelease];
			return nil;
		}
		
		[self putOpenGLView:[newView autorelease]];
		[[self window] setAcceptsMouseMovedEvents:YES];
	}
	return self;
}

- (void)dealloc
{
	[parentView release];
	[super dealloc];
}

////////////////////////////////////////////////////////////
/// Registers the the OpenGL view and adds it to its parent container
////////////////////////////////////////////////////////////
- (void)putOpenGLView:(sfPrivGLView *)aView
{
	[self setView:aView];
	
	// Finish setting up the view and window
	NSRect originalFrame = [[self window] frame];
	NSRect tmpFrame = originalFrame;
	originalFrame.origin.x++;
	
	[[self window] setFrame:tmpFrame display:YES];
	[[self window] setFrame:originalFrame display:YES];
	
	
	// Add the view to our *parent view* and tell it to the view
	[parentView addSubview:[self view]];
	[[self view] finishInitialization];
}

////////////////////////////////////////////////////////////
/// Forward call to set the window position on screen
////////////////////////////////////////////////////////////
- (void)setPosition:(NSPoint)aPosition
{
	std::cerr
	<< "Warning: called Window::SetPosition() on a window imported from a widget. "
	<< "This method has been disabled in this case and has no effect. "
	<< "Directly use the widget if you want to move it."
	<< std::endl;
}

////////////////////////////////////////////////////////////
/// Forward call to set the window size
////////////////////////////////////////////////////////////
- (void)setSize:(NSSize)size
{
	std::cerr
	<< "Warning: called Window::SetSize() on a window imported from a widget. "
	<< "This method has been disabled in this case and has no effect. "
	<< "Directly use the widget if you want to resize it."
	<< std::endl;
}

@end

