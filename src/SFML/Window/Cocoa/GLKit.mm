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
#import <SFML/System/Sleep.hpp>
#import <OpenGL/gl.h>
#import <iostream>


////////////////////////////////////////////////////////////
/// Window independant OpenGL context class
////////////////////////////////////////////////////////////
@implementation GLContext

static GLContext *sharedCtx = nil;

////////////////////////////////////////////////////////////
/// Return the shared OpenGL context instance (making one if needed)
////////////////////////////////////////////////////////////
+ (id)sharedContext
{
	if (sharedCtx == nil)
	{
		// Make a new context with the default parameters
		sf::WindowSettings params(0, 0, 0);
		sharedCtx = [[GLContext alloc] initWithAttributes:params sharedContext:nil];
	}
	
	return sharedCtx;
}

- (void)dealloc
{
	[mySharedContext release];
	[super dealloc];
}

////////////////////////////////////////////////////////////
/// Make a new OpenGL context according to the @attribs settings
/// and the shared context @context
////////////////////////////////////////////////////////////
- (id)initWithAttributes:(sf::WindowSettings&)attribs sharedContext:(GLContext *)context
{
	// Note about antialiasing and other context attributes :
	// OpenGL context sharing does not allow the shared contexts to use different attributes.
	// The point is that the default shared global OpenGL context uses default parameters.
	// That means that all the other context *should* use the same paramaters.
	// Fortunately some values parameters for some parameters are compatible, but some are not
	// among which : the antialising level.
	//
	// I've no way to fix this for now.
	
	if (attribs.AntialiasingLevel)
		std::cerr << "Warning: antialiasing settings are inhibited under Mac OS X for technical reasons" << std::endl;
	
	NSOpenGLPixelFormat *myPixelFormat = nil;
	unsigned idx = 0;
	
	// Attributes list
	NSOpenGLPixelFormatAttribute ctxtAttribs[15] = {(NSOpenGLPixelFormatAttribute) 0};
	
	// Accelerated, double buffered
	ctxtAttribs[idx++] = NSOpenGLPFAClosestPolicy;
	ctxtAttribs[idx++] = NSOpenGLPFADoubleBuffer;
	ctxtAttribs[idx++] = NSOpenGLPFAAccelerated;
	
	// windowed context (even fullscreen mode uses a window)
	ctxtAttribs[idx++] = NSOpenGLPFAWindow;
	
	// Color size ; usually 32 bits per pixel
	ctxtAttribs[idx++] = NSOpenGLPFAColorSize;
	ctxtAttribs[idx++] = (NSOpenGLPixelFormatAttribute) sf::VideoMode::GetDesktopMode().BitsPerPixel;
	
	// Z-buffer size
	ctxtAttribs[idx++] = NSOpenGLPFADepthSize;
	ctxtAttribs[idx++] = (NSOpenGLPixelFormatAttribute) attribs.DepthBits;
	
	// Stencil bits (I don't really know what's that...)
	ctxtAttribs[idx++] = NSOpenGLPFAStencilSize;
	ctxtAttribs[idx++] = (NSOpenGLPixelFormatAttribute) attribs.StencilBits;
	
	myPixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:ctxtAttribs];
	
	if (myPixelFormat) {
		self = [super initWithFormat:myPixelFormat
									shareContext:context];
		
		mySharedContext = [context retain];
		
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
	}
	
	return self;
}

@end


////////////////////////////////////////////////////////////
/// Customized Cocoa OpenGL view
////////////////////////////////////////////////////////////
@implementation GLView

////////////////////////////////////////////////////////////
/// Make a new view according the the rect @frame,
/// the video mode @mode, the window settings @settings
/// and the sf window delegate @delegate
/// @delegate must not be null
////////////////////////////////////////////////////////////
- (id)initWithFrame:(NSRect)frame
			   mode:(const sf::VideoMode&)mode
		   settings:(sf::WindowSettings&)settings
		   delegate:(sf::priv::WindowImplCocoa *)delegate
{
	assert(delegate != NULL);
	
	// make the view
	self = [super initWithFrame:frame pixelFormat:nil];
	
	if (self)
	{
		// enabled auto-resizing
		[self setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
		
		// make the OpenGL context
		myGLContext = [[GLContext alloc] initWithAttributes:settings sharedContext:sharedCtx];
		
		// We need to update the OpenGL view when it's resized
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		[nc addObserver:self
			   selector:@selector(viewFrameDidChange:)
				   name:NSViewFrameDidChangeNotification
				 object:self];
		
		// Save the delegate
		myDelegate = delegate;
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
	
	NSText *field = [[self window] fieldEditor:YES forObject:nil];
	[field interpretKeyEvents:[NSArray arrayWithObject:theEvent]];
	[field setString:@""];
	
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
/// Cocoa window implementation to let fullscreen windows
/// catch key events
////////////////////////////////////////////////////////////
@implementation GLWindow

- (BOOL)canBecomeKeyWindow
{
	return YES;
}

- (BOOL)canBecomeMainWindow
{
	return YES;
}

@end


////////////////////////////////////////////////////////////
/// WindowWrapper class : handles both imported and self-built windows
////////////////////////////////////////////////////////////
@implementation WindowWrapper

////////////////////////////////////////////////////////////
/// Make a new window wrapper according to the window settings @attribs,
/// the video mode @mode, the window style @style, the window title @title
/// and the sf window implementation delegate @delegate
////////////////////////////////////////////////////////////
- (id)initWithSettings:(sf::WindowSettings&)params
			 videoMode:(sf::VideoMode&)mode
				 style:(unsigned long)style
				 title:(NSString *)title
			  delegate:(sf::priv::WindowImplCocoa *)delegate
{
	return [self initWithWindow:nil
					   settings:params
					  videoMode:mode
						  style:style
						  title:title
					   delegate:delegate];
}


////////////////////////////////////////////////////////////
/// Make a new window wrapper by importing @window and according to
/// the window settings @params and the sf window implementation delegate
/// @delegate
/// @window and @delegate must not be null
////////////////////////////////////////////////////////////
- (id)initWithWindow:(NSWindow *)window
			settings:(sf::WindowSettings&)params
			delegate:(sf::priv::WindowImplCocoa *)delegate
{
	sf::VideoMode mode([[myWindow contentView] frame].size.width, [[myWindow contentView] frame].size.height);
	return [self initWithWindow:window
					   settings:params
					  videoMode:mode
						  style:0
						  title:nil
					   delegate:delegate];
}


////////////////////////////////////////////////////////////
/// Make a new window wrapper by importing @window if it's not null and according to
/// the window settings @params and the sf window implementation delegate
/// @delegate; or by creating a new window if @window is null. In this case @title
/// must therefore not be null and @params must be valid.
/// @delegate must never be null 
////////////////////////////////////////////////////////////
- (id)initWithWindow:(NSWindow *)window
			settings:(sf::WindowSettings&)params
		   videoMode:(sf::VideoMode&)mode
			   style:(unsigned long)style
			   title:(NSString *)title
			delegate:(sf::priv::WindowImplCocoa *)delegate
{
	assert(delegate != NULL);
	
	self = [super init];
	
	if (self)
	{
		if (window) {
			myWindow = (GLWindow *)[window retain];
		} else {
			assert(title != nil);
			
			NSRect frame = NSMakeRect (0.0f, 0.0f, (float) mode.Width, (float) mode.Height);
			unsigned int mask = 0;
			
			if (style & sf::Style::Fullscreen) {
				myIsFullscreen = true;
				
				// Check display mode and put new values in 'mode' if needed
				boolean_t exact = true;
				
				CFDictionaryRef properties = CGDisplayBestModeForParameters(kCGDirectMainDisplay, mode.BitsPerPixel,
																			mode.Width, mode.Height, &exact);
				
				if (!properties) {
					std::cerr << "Unable to get a display mode with the given parameters" << std::endl;
					[self autorelease];
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
			
			// We grab options from WindowStyle and add them to our window mask
			if (style & sf::Style::None || style & sf::Style::Fullscreen) {
				mask |= NSBorderlessWindowMask;
				
				
				
			} else {
				if (style & sf::Style::Titlebar) {
					mask |= NSTitledWindowMask;
					mask |= NSMiniaturizableWindowMask;
				}
				
				if (style & sf::Style::Resize) {
					mask |= NSTitledWindowMask;
					mask |= NSMiniaturizableWindowMask;
					mask |= NSResizableWindowMask;
				}
				
				if (style & sf::Style::Close) {
					mask |= NSTitledWindowMask;
					mask |= NSClosableWindowMask;
					mask |= NSMiniaturizableWindowMask;
				}
			}
			
			// Now we make the window with the values we got
			// Note: defer flag set to NO to be able to use OpenGL in our window
			myWindow = [[GLWindow alloc] initWithContentRect:frame
												   styleMask:mask
													 backing:NSBackingStoreBuffered
													   defer:NO];
			
			if (myWindow) {
				// We set title and window position
				[myWindow setTitle:title];
				[myWindow center];
			} else {
				std::cerr << "Unable to create the Cocoa window" << std::endl;
				[self autorelease];
				return nil;
			}
		}
		
		// Make the OpenGL view
		myView = [[GLView alloc] initWithFrame:[[myWindow contentView] frame]
										  mode:mode
									  settings:params
									  delegate:delegate];
		
		if (myView)	{
			// Finish setting up the view and window
			// Add the view to our window and tell it to the view
			[[myWindow contentView] addSubview:myView];
			[myView finishInitialization];
			
			NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
			
			// We want to know when our window got the focus
			[nc addObserver:self
				   selector:@selector(windowDidBecomeMain:)
					   name:NSWindowDidBecomeMainNotification
					 object:myWindow];
			
			// We want to know when our window lost the focus
			[nc addObserver:self
				   selector:@selector(windowDidResignMain:)
					   name:NSWindowDidResignMainNotification
					 object:myWindow];
			
			// We want to know when the user closes the window
			[nc addObserver:self
				   selector:@selector(windowWillClose:)
					   name:NSWindowWillCloseNotification
					 object:myWindow];
			
			// I want to re-center the window if it's a full screen one and moved by Spaces
			[nc addObserver:self
				   selector:@selector(windowDidMove:)
					   name:NSWindowDidMoveNotification
					 object:myWindow];
			
			// Needed not to make application crash when releasing the window in our destructor
			// (I prefer to take control of everything :P)
			[myWindow setReleasedWhenClosed:NO];
			[myWindow setAcceptsMouseMovedEvents:YES];
			
		} else {
			std::cerr << "Unable to create the OpenGL view" << std::endl;
			[self autorelease];
			return nil;
		}
		
		if (myIsFullscreen) {
			myFullscreenMode = mode;
			
			// Using this because full screen window was not always
			// in front of the other application windows when unhiding app
			[myWindow setLevel:NSFloatingWindowLevel];
		}
	}
	
	return self;
}


////////////////////////////////////////////////////////////
/// Clean the window wrapper
////////////////////////////////////////////////////////////
- (void)dealloc
{
	
	// Remove the notification observer
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	
	// Close the window
	[self show:false];
	
	// Release the window and view
	[myView release];
	[myWindow release];
	
	[super dealloc];
}


////////////////////////////////////////////////////////////
/// Return a reference to the internal Cocoa window
////////////////////////////////////////////////////////////
- (NSWindow *)window
{
	return myWindow;
}


////////////////////////////////////////////////////////////
/// Return a reference to the internal Cocoa OpenGL view
////////////////////////////////////////////////////////////
- (GLView *)glView
{
	return myView;
}


////////////////////////////////////////////////////////////
/// Forward call to set the window position on screen
////////////////////////////////////////////////////////////
- (void)setPosition:(NSPoint)pos
{
	assert(myWindow != nil);
	
	if (!myIsFullscreen) {
		// Flip Y and set window position
		pos.y = [[myWindow screen] frame].size.height - pos.y;
		[myWindow setFrameTopLeftPoint:pos];
	}
}


////////////////////////////////////////////////////////////
/// Forward call to set the window size
////////////////////////////////////////////////////////////
- (void)setSize:(NSSize)size
{
	assert(myWindow != nil);
	
	if (!myIsFullscreen) {
		[myWindow setFrame:NSMakeRect([myWindow frame].origin.x,
									  [myWindow frame].origin.y,
									  size.width, size.height)
				   display:YES];
	}
}


////////////////////////////////////////////////////////////
/// Return the mouse location relative to the internal window
////////////////////////////////////////////////////////////
- (NSPoint)mouseLocation
{
	assert(myWindow != nil);
	
	NSPoint	relativeLocation = [myWindow convertScreenToBase:[NSEvent mouseLocation]];
	relativeLocation.y = [[self glView] frame].size.height - relativeLocation.y;
	return relativeLocation;
}


////////////////////////////////////////////////////////////
/// Return whether the mouse is on our window
////////////////////////////////////////////////////////////
- (BOOL)mouseInside
{
	assert(myWindow != nil);
	assert(myView != nil);
	
	BOOL flag = NO;
	
	if ([myWindow isVisible]) {
		NSPoint relativeToWindow = [myWindow mouseLocationOutsideOfEventStream];
		NSPoint relativeToView = [myView convertPoint:relativeToWindow fromView:nil];
		
		if (NSPointInRect (relativeToView, [myView bounds]))
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
	assert(myWindow != nil);
	
	if (flag && ![myWindow isVisible]) {
		// Wanna open the closed window
		
		if (myIsFullscreen) {
			[SharedAppController setFullscreenWindow:self mode:&myFullscreenMode];
		} else {
			// Show the window
			[myWindow makeKeyAndOrderFront:nil];
		}
	} else if (!flag && [myWindow isVisible]) {
		// Wanna close the opened window
		
		if (myIsFullscreen) {
			[SharedAppController setFullscreenWindow:nil mode:NULL];
		} else {
			// Close the window
			[myWindow close];
		}
	}	
}


////////////////////////////////////////////////////////////
/// Forward call to en/disable the OpenGL view vertical synchronization
////////////////////////////////////////////////////////////
- (void)enableVerticalSync:(bool)flag
{
	assert(myView != nil);
	[myView enableVerticalSync:flag];
}


////////////////////////////////////////////////////////////
/// Forward 'setActive' call the the OpenGL view
////////////////////////////////////////////////////////////
- (void)setActive:(bool)flag
{
	assert(myView != nil);
	[myView setActive:flag];
}


////////////////////////////////////////////////////////////
/// Forward call to flush the OpenGL view
////////////////////////////////////////////////////////////
- (void)flushBuffer
{
	assert(myView != nil);
	[myView flushBuffer];
}


////////////////////////////////////////////////////////////
/// Notification method receiver when the window gains focus
////////////////////////////////////////////////////////////
- (void)windowDidBecomeMain:(NSNotification *)notification
{
	sf::Event ev;
	ev.Type = sf::Event::GainedFocus;
	
	[myView pushEvent:ev];
}


////////////////////////////////////////////////////////////
/// Notification method receiver when the window loses focus
////////////////////////////////////////////////////////////
- (void)windowDidResignMain:(NSNotification *)notification
{
	sf::Event ev;
	ev.Type = sf::Event::LostFocus;
	
	[myView pushEvent:ev];
}


////////////////////////////////////////////////////////////
/// Notification method receiver when the window closes
////////////////////////////////////////////////////////////
- (void)windowWillClose:(NSNotification *)notification
{
	sf::Event ev;
	ev.Type = sf::Event::Closed;
	
	[myView pushEvent:ev];
}


////////////////////////////////////////////////////////////
/// Notification method receiver when the window finish moving
////////////////////////////////////////////////////////////
- (void)windowDidMove:(NSNotification *)notification
{
	NSWindow *sender = [notification object];
	
	if (!([sender styleMask] & NSTitledWindowMask))
		[sender center];
}

@end

