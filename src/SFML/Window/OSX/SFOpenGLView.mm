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
#include <SFML/Window/OSX/WindowImplCocoa.hpp>

#import <SFML/Window/OSX/SFOpenGLView.h>

@implementation SFOpenGLView

#pragma mark
#pragma mark SFOpenGLView's methods

////////////////////////////////////////////////////////
-(id)initWithFrame:(NSRect)frameRect
{
	if (self = [super initWithFrame:frameRect]) {
		[self setRequesterTo:0];
		[self enableKeyRepeat];
		
		// Register for mouse-move event
		myMouseIsIn = [self isMouseInside];
		myTrackingTag = [self addTrackingRect:[self frame] 
																		owner:self 
																 userData:nil 
														 assumeInside:myMouseIsIn];
		
		// Register for resize event
		NSNotificationCenter* center = [NSNotificationCenter defaultCenter];
		[center addObserver:self 
							 selector:@selector(frameDidChange:)
									 name:NSViewFrameDidChangeNotification
								 object:nil];
	}

	return self;
}


////////////////////////////////////////////////////////
-(void)setRequesterTo:(sf::priv::WindowImplCocoa*)requester
{
	myRequester = requester;
}


////////////////////////////////////////////////////////
-(void)enableKeyRepeat
{
	myUseKeyRepeat = YES;
}


////////////////////////////////////////////////////////
-(void)disableKeyRepeat
{
	myUseKeyRepeat = NO;
}


////////////////////////////////////////////////////////
-(void)frameDidChange:(NSNotification*)notification
{
	// Adapt tracking area for mouse mouse event.
	[self removeTrackingRect:myTrackingTag];
	myTrackingTag = [self addTrackingRect:[self frame] 
																	owner:self 
															 userData:nil 
													 assumeInside:myMouseIsIn];
	
	// Update the OGL view to fit the new size.
	[self update];
	
	// Send an event
	if (myRequester == 0) return;
	
	// The new size
	NSSize newSize = [self frame].size;
	myRequester->WindowResized(newSize.width, newSize.height);
}


////////////////////////////////////////////////////////
-(BOOL)isMouseInside
{
	NSPoint relativeToWindow = [[self window] mouseLocationOutsideOfEventStream];
	NSPoint relativeToView = [self convertPoint:relativeToWindow fromView:nil];
	
	if (NSPointInRect(relativeToView, [self frame])) {
		return YES;
	}
	
	return NO;
}


#pragma mark
#pragma mark Subclassing methods


////////////////////////////////////////////////////////
-(void)dealloc
{
	// Unregister
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	[self removeTrackingRect:myTrackingTag];
	
	[super dealloc];
}


////////////////////////////////////////////////////////
-(BOOL)acceptsFirstResponder 
{ 
	// Accepts key event.
	return YES;
}


////////////////////////////////////////////////////////
-(BOOL)canBecomeKeyView
{
	// Accepts key event.
	return YES;
}


#pragma mark
#pragma mark Mouse-event methods


////////////////////////////////////////////////////////
-(void)mouseDown:(NSEvent*)theEvent 
{
	if (myRequester == 0) return;
	
	NSPoint loc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	// Don't forget to change to SFML coord system.
	float h = [[theEvent window] frame].size.height;
	myRequester->MouseDownAt(sf::Mouse::Left, loc.x, h - loc.y);
}


////////////////////////////////////////////////////////
-(void)mouseUp:(NSEvent*)theEvent
{
	if (myRequester == 0) return;
	
	NSPoint loc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	// Don't forget to change to SFML coord system.
	float h = [[theEvent window] frame].size.height;
	myRequester->MouseUpAt(sf::Mouse::Left, loc.x, h - loc.y);
}


////////////////////////////////////////////////////////
-(void)mouseMoved:(NSEvent*)theEvent
{
	if (myRequester == 0) return;
	
	// If the event is not useful.
	if (!myMouseIsIn) return;
	
	NSPoint loc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	// Don't forget to change to SFML coord system.
	float h = [[theEvent window] frame].size.height;
	myRequester->MouseMovedAt(loc.x, h - loc.y);
}


////////////////////////////////////////////////////////
-(void)rightMouseDown:(NSEvent*)theEvent
{
	if (myRequester == 0) return;
	
	NSPoint loc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	// Don't forget to change to SFML coord system.
	float h = [[theEvent window] frame].size.height;
	myRequester->MouseDownAt(sf::Mouse::Right, loc.x, h - loc.y);
}


////////////////////////////////////////////////////////
-(void)rightMouseUp:(NSEvent*)theEvent
{
	if (myRequester == 0) return;
	
	NSPoint loc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	// Don't forget to change to SFML coord system.
	float h = [[theEvent window] frame].size.height;
	myRequester->MouseUpAt(sf::Mouse::Right, loc.x, h - loc.y);
}


////////////////////////////////////////////////////////
-(void)otherMouseDown:(NSEvent*)theEvent
{
	if (myRequester == 0) return;
	
	NSPoint loc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	sf::Mouse::Button button;
	switch ([theEvent buttonNumber]) {
		case 2:
			button = sf::Mouse::Middle;
			break;
		case 3:
			button = sf::Mouse::XButton1;
			break;
		case 4:
			button = sf::Mouse::XButton2;
			break;
		default:
			break;
	}
	// Don't forget to change to SFML coord system.
	float h = [[theEvent window] frame].size.height;
	myRequester->MouseDownAt(button, loc.x, h - loc.y);
}


////////////////////////////////////////////////////////
-(void)otherMouseUp:(NSEvent*)theEvent
{
	if (myRequester == 0) return;
	
	NSPoint loc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	sf::Mouse::Button button;
	switch ([theEvent buttonNumber]) {
		case 2:
			button = sf::Mouse::Middle;
			break;
		case 3:
			button = sf::Mouse::XButton1;
			break;
		case 4:
			button = sf::Mouse::XButton2;
			break;
		default:
			break;
	}
	// Don't forget to change to SFML coord system.
	float h = [[theEvent window] frame].size.height;
	myRequester->MouseUpAt(button, loc.x, h - loc.y);
}


////////////////////////////////////////////////////////
-(void)scrollWheel:(NSEvent*)theEvent
{
	if (myRequester == 0) return;
	
	NSPoint loc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	// Don't forget to change to SFML coord system.
	float h = [[theEvent window] frame].size.height;
	myRequester->MouseWheelScrolledAt([theEvent deltaY], loc.x, h - loc.y);
}


////////////////////////////////////////////////////////
-(void)mouseEntered:(NSEvent*)theEvent
{
	myMouseIsIn = YES;
	
	if (myRequester == 0) return;
	
	myRequester->MouseMovedIn();
}


////////////////////////////////////////////////////////
-(void)mouseExited:(NSEvent*)theEvent
{
	myMouseIsIn = NO;
	
	if (myRequester == 0) return;
	
	myRequester->MouseMovedOut();
}


#pragma mark
#pragma mark Key-event methods


////////////////////////////////////////////////////////
-(void)keyDown:(NSEvent*)theEvent
{
	if (myRequester == 0) return;
	
	if (myUseKeyRepeat || ![theEvent isARepeat])
		myRequester->KeyDown([theEvent keyCode], [theEvent modifierFlags]);
	
	if ([[theEvent characters] length] > 0)
		myRequester->TextEntred([[theEvent characters] characterAtIndex:0]);
}


////////////////////////////////////////////////////////
-(void)keyUp:(NSEvent*)theEvent
{
	if (myRequester == 0) return;
	
	myRequester->KeyUp([theEvent keyCode], [theEvent modifierFlags]);
}

@end
