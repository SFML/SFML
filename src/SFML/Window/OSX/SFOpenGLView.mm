////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2012 Marco Antognini (antognini.marco@gmail.com), 
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
#include <SFML/Window/OSX/HIDInputManager.hpp> // For localizedKeys and nonLocalizedKeys
#include <SFML/System/Err.hpp>

#import <SFML/Window/OSX/SFOpenGLView.h>
#import <SFML/Window/OSX/SFSilentResponder.h>
#import <SFML/Window/OSX/SFKeyboardModifiersHelper.h>


////////////////////////////////////////////////////////////
/// Returns true if `event` represents a representable character.
///
/// The event is assumed to be a key down event.
/// False is returned if the event is either escape or a non text unicode.
///
////////////////////////////////////////////////////////////
BOOL isValidTextUnicode(NSEvent* event);


////////////////////////////////////////////////////////////
/// SFOpenGLView class : Privates Methods Declaration
///
////////////////////////////////////////////////////////////
@interface SFOpenGLView ()

////////////////////////////////////////////////////////////
/// Handle view resized event.
/// 
////////////////////////////////////////////////////////////
-(void)frameDidChange:(NSNotification *)notification;

////////////////////////////////////////////////////////////
/// Establish if the mouse is inside or outside the OpenGL view.
/// 
////////////////////////////////////////////////////////////
-(BOOL)isMouseInside;

////////////////////////////////////////////////////////////
/// Update the mouse state (in or out) and fire an event
/// if its state has changed.
/// 
////////////////////////////////////////////////////////////
-(void)updateMouseState;

////////////////////////////////////////////////////////////
/// Convert the NSEvent mouse button type to SFML type.
///
/// Returns ButtonCount if the button is unknown
/// 
////////////////////////////////////////////////////////////
-(sf::Mouse::Button)mouseButtonFromEvent:(NSEvent *)event;

////////////////////////////////////////////////////////////
/// Convert a key down/up NSEvent into an SFML key event.
/// Based on localizedKeys and nonLocalizedKeys function.
///
/// Return sf::Keyboard::Unknown as Code if the key is unknown.
///
////////////////////////////////////////////////////////////
+(sf::Event::KeyEvent)convertNSKeyEventToSFMLEvent:(NSEvent *)anEvent;

@end

@implementation SFOpenGLView

#pragma mark
#pragma mark SFOpenGLView's methods

////////////////////////////////////////////////////////
-(id)initWithFrame:(NSRect)frameRect
{
    if ((self = [super initWithFrame:frameRect])) {
        [self setRequesterTo:0];
        [self enableKeyRepeat];
        m_realSize = NSZeroSize;
        
        // Register for mouse move event
        m_mouseIsIn = [self isMouseInside];
        NSUInteger opts = (NSTrackingActiveAlways | NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved | NSTrackingEnabledDuringMouseDrag);
        m_trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds]
                                                      options:opts
                                                        owner:self
                                                     userInfo:nil];
        [self addTrackingArea:m_trackingArea];
        
        // Register for resize event
        NSNotificationCenter* center = [NSNotificationCenter defaultCenter];
        [center addObserver:self
                   selector:@selector(frameDidChange:)
                       name:NSViewFrameDidChangeNotification
                     object:self];

        // Create a hidden text view for parsing key down event properly
        m_silentResponder = [[SFSilentResponder alloc] init];
        m_hiddenTextView = [[NSTextView alloc] initWithFrame:NSZeroRect];
        [m_hiddenTextView setNextResponder:m_silentResponder];
    }

    return self;
}


////////////////////////////////////////////////////////
-(void)enterFullscreen
{
    // Remove the tracking area first,
    // just to be sure we don't add it twice!
    [self removeTrackingArea:m_trackingArea];
    [self addTrackingArea:m_trackingArea];

    // Fire an mouse entered event if needed
    if (!m_mouseIsIn && m_requester != 0) {
        m_requester->mouseMovedIn();
    }

    // Update status
    m_mouseIsIn = YES;
}


////////////////////////////////////////////////////////
-(void)exitFullscreen
{
    [self removeTrackingArea:m_trackingArea];

    // Fire an mouse left event if needed
    if (m_mouseIsIn && m_requester != 0) {
        m_requester->mouseMovedOut();
    }

    // Update status
    m_mouseIsIn = NO;
}


////////////////////////////////////////////////////////
-(void)setRequesterTo:(sf::priv::WindowImplCocoa *)requester
{
    m_requester = requester;
}


////////////////////////////////////////////////////////
-(void)setRealSize:(NSSize)newSize
{
    m_realSize = newSize;
}


////////////////////////////////////////////////////////
-(NSPoint)computeGlobalPositionOfRelativePoint:(NSPoint)point
{   
    // Recompute the mouse pos if required.
    if (!NSEqualSizes(m_realSize, NSZeroSize)) {
        point.x = point.x / m_realSize.width  * [self frame].size.width;
        point.y = point.y / m_realSize.height * [self frame].size.height;
    }

    // Note : -[NSWindow convertBaseToScreen:] is deprecated on 10.7
    //        but the recommended -[NSWindow convertRectToScreen] is not
    //        available until 10.7.
    //
    //        So we stick with the old one for now.

    
    // Flip SFML coordinates to match window coordinates
    point.y = [self frame].size.height - point.y;
    
    // Get the position of (x, y) in the coordinate system of the window.
    point = [self convertPoint:point toView:self];
    point = [self convertPoint:point toView:nil]; // nil means window
    
    // Convert it to screen coordinates
    point = [[self window] convertBaseToScreen:point];
    
    // Flip screen coodinates to match CGDisplayMoveCursorToPoint referential.
    float const screenHeight = [[[self window] screen] frame].size.height;
    point.y = screenHeight - point.y;

    return point;
}


////////////////////////////////////////////////////////
-(void)enableKeyRepeat
{
    m_useKeyRepeat = YES;
}


////////////////////////////////////////////////////////
-(void)disableKeyRepeat
{
    m_useKeyRepeat = NO;
}


////////////////////////////////////////////////////////
-(void)frameDidChange:(NSNotification *)notification
{
    // Update mouse internal state.
    [self updateMouseState];
    
    // Update the OGL view to fit the new size.
    [self update];
    
    // Send an event
    if (m_requester == 0) return;
    
    // The new size
    NSSize newSize = [self frame].size;
    m_requester->windowResized(newSize.width, newSize.height);
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


////////////////////////////////////////////////////////
-(void)updateMouseState
{
    BOOL mouseWasIn = m_mouseIsIn;
    m_mouseIsIn = [self isMouseInside];
    
    // Send event if needed.
    if (mouseWasIn && !m_mouseIsIn) {
        [self mouseExited:nil];
    } else if (!mouseWasIn && m_mouseIsIn) {
        [self mouseEntered:nil];
    }
}


#pragma mark
#pragma mark Subclassing methods


////////////////////////////////////////////////////////
-(void)dealloc
{
    // Unregister
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [self removeTrackingArea:m_trackingArea];

    // Release attributes
    [m_hiddenTextView release];
    [m_silentResponder release];
    [m_trackingArea release];

    [self setRequesterTo:0];

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
-(void)mouseDown:(NSEvent *)theEvent 
{
    // Forward to...
    [self otherMouseDown:theEvent];
    
    // Transmit to non-SFML responder
    [[self nextResponder] mouseDown:theEvent];
}


////////////////////////////////////////////////////////
-(void)mouseUp:(NSEvent *)theEvent
{
    // Forward to...
    [self otherMouseUp:theEvent];
    
    // Transmit to non-SFML responder
    [[self nextResponder] mouseUp:theEvent];
}


////////////////////////////////////////////////////////
-(void)mouseMoved:(NSEvent *)theEvent
{
    // Forward to...
    [self otherMouseDragged:theEvent];
    
    // Transmit to non-SFML responder
    [[self nextResponder] mouseMoved:theEvent];
}


////////////////////////////////////////////////////////
-(void)scrollWheel:(NSEvent *)theEvent
{
    if (m_requester != 0) {
        NSPoint loc = [self cursorPositionFromEvent:theEvent];
        
        m_requester->mouseWheelScrolledAt([theEvent deltaY], loc.x, loc.y);
    }
    
    // Transmit to non-SFML responder
    [[self nextResponder] scrollWheel:theEvent];
}


////////////////////////////////////////////////////////
-(void)mouseEntered:(NSEvent *)theEvent
{
    // There are two cases when we need to fire an event:
    // a) the event is nil, meaning that the method was
    //    called from our code (e.g. updateMouseState)
    // b) the mouse was outside the view.
    BOOL shouldFire = (theEvent == nil || m_mouseIsIn == NO);

    // Update status
    m_mouseIsIn = YES;

    if (m_requester == 0) return;

    // Fire (or not) an event
    if (shouldFire) {
        m_requester->mouseMovedIn();
    }
}


////////////////////////////////////////////////////////
-(void)mouseExited:(NSEvent *)theEvent
{
    // Similarly to mouseEntered:
    BOOL shouldFire = (theEvent == nil || m_mouseIsIn == YES);

    // Update status
    m_mouseIsIn = NO;

    if (m_requester == 0) return;

    // Fire (or not) an event
    if (shouldFire) {
        m_requester->mouseMovedOut();
    }
}


////////////////////////////////////////////////////////
-(void)rightMouseDown:(NSEvent *)theEvent
{
    // Forward to...
    [self otherMouseDown:theEvent];
    
    // Transmit to non-SFML responder
    [[self nextResponder] rightMouseDown:theEvent];
}


////////////////////////////////////////////////////////
-(void)rightMouseUp:(NSEvent *)theEvent
{
    // Forward to...
    [self otherMouseUp:theEvent];
    
    // Transmit to non-SFML responder
    [[self nextResponder] rightMouseUp:theEvent];
}


////////////////////////////////////////////////////////
-(void)otherMouseDown:(NSEvent *)theEvent
{
    sf::Mouse::Button button = [self mouseButtonFromEvent:theEvent];
    
    if (m_requester != 0) {
        NSPoint loc = [self cursorPositionFromEvent:theEvent];
        
        if (button != sf::Mouse::ButtonCount) {
            m_requester->mouseDownAt(button, loc.x, loc.y);
        }
    }
    
    // If the event is not forwarded by mouseDown or rightMouseDown...
    if (button != sf::Mouse::Left && button != sf::Mouse::Right) {
        // ... transmit to non-SFML responder
        [[self nextResponder] otherMouseDown:theEvent];
    }
}


////////////////////////////////////////////////////////
-(void)otherMouseUp:(NSEvent *)theEvent
{
    sf::Mouse::Button button = [self mouseButtonFromEvent:theEvent];
    
    if (m_requester != 0) {
        NSPoint loc = [self cursorPositionFromEvent:theEvent];
        
        if (button != sf::Mouse::ButtonCount) {
            m_requester->mouseUpAt(button, loc.x, loc.y);
        }
    }
    
    // If the event is not forwarded by mouseUp or rightMouseUp...
    if (button != sf::Mouse::Left && button != sf::Mouse::Right) {
        // ... transmit to non-SFML responder
        [[self nextResponder] otherMouseUp:theEvent];
    }
}


////////////////////////////////////////////////////////
-(void)rightMouseDragged:(NSEvent *)theEvent
{
    // Forward to...
    [self otherMouseDragged:theEvent];
    
    // Transmit to non-SFML responder
    [[self nextResponder] rightMouseDragged:theEvent];
}


////////////////////////////////////////////////////////
-(void)mouseDragged:(NSEvent *)theEvent
{
    // Forward to...
    [self otherMouseDragged:theEvent];
    
    // Transmit to non-SFML responder
    [[self nextResponder] mouseDragged:theEvent];
}


////////////////////////////////////////////////////////
-(void)otherMouseDragged:(NSEvent *)theEvent
{
    if (m_requester != 0) {
        NSPoint loc = [self cursorPositionFromEvent:theEvent];

        // Make sure the point is inside the view.
        // (mouseEntered: and mouseExited: are not immediately called
        //  when the mouse is dragged. That would be too easy!)
        [self updateMouseState];
        if (m_mouseIsIn) {
            m_requester->mouseMovedAt(loc.x, loc.y);
        }
    }
    
    // If the event is not forwarded by mouseDragged or rightMouseDragged...
    sf::Mouse::Button button = [self mouseButtonFromEvent:theEvent];
    if (button != sf::Mouse::Left && button != sf::Mouse::Right) {
        // ... transmit to non-SFML responder
        [[self nextResponder] otherMouseUp:theEvent];
    }
}


////////////////////////////////////////////////////////
-(NSPoint)cursorPositionFromEvent:(NSEvent *)eventOrNil
{
    NSPoint loc;
    // If no event given then get current mouse pos.
    if (eventOrNil == nil) {
        NSPoint rawPos = [[self window] mouseLocationOutsideOfEventStream];
        loc = [self convertPoint:rawPos fromView:nil];
    } else {
        loc = [self convertPoint:[eventOrNil locationInWindow] fromView:nil];
    }
    
    // Don't forget to change to SFML coord system.
    float h = [self frame].size.height;
    loc.y = h - loc.y;
    
    // Recompute the mouse pos if required.
    if (!NSEqualSizes(m_realSize, NSZeroSize)) {
        loc.x = loc.x * m_realSize.width  / [self frame].size.width;
        loc.y = loc.y * m_realSize.height / [self frame].size.height;
    }
    
    return loc;
}


////////////////////////////////////////////////////////
-(sf::Mouse::Button)mouseButtonFromEvent:(NSEvent *)event
{
    switch ([event buttonNumber]) {
        case 0:     return sf::Mouse::Left;
        case 1:     return sf::Mouse::Right;
        case 2:     return sf::Mouse::Middle;
        case 3:     return sf::Mouse::XButton1;
        case 4:     return sf::Mouse::XButton2;
        default:    return sf::Mouse::ButtonCount; // Never happens! (hopefully)
    }
}


#pragma mark
#pragma mark Key-event methods


////////////////////////////////////////////////////////
-(void)keyDown:(NSEvent *)theEvent
{
    // Transmit to non-SFML responder
    [[self nextResponder] keyDown:theEvent];
    
    if (m_requester == 0) return;
    
    // Handle key down event
    if (m_useKeyRepeat || ![theEvent isARepeat]) {
        sf::Event::KeyEvent key = [SFOpenGLView convertNSKeyEventToSFMLEvent:theEvent];
        
        if (key.code != sf::Keyboard::Unknown) { // The key is recognized.
            m_requester->keyDown(key);
        }
    }


    // Handle text entred event:
    // Ignore event if we don't want repeated keystrokes
    if (m_useKeyRepeat || ![theEvent isARepeat]) {
        // Ignore escape key and other non text keycode (See NSEvent.h)
        // because they produce a sound alert.
        if (isValidTextUnicode(theEvent)) {
            // Send the event to the hidden text view for processing
            [m_hiddenTextView interpretKeyEvents:[NSArray arrayWithObject:theEvent]];
        }

        // Carefully handle backspace and delete..
        // Note: the event is intentionally sent to the hidden view
        //       even if we do something more specific below. This way
        //       key combination are correctly interpreted.

        unsigned short keycode = [theEvent keyCode];

        // Backspace
        if (keycode == 0x33) {
            // Send the correct unicode value (i.e. 8) instead of 127 (which is 'delete')
            m_requester->textEntered(8);
        }

        // Delete
        else if (keycode == 0x75 || keycode == NSDeleteFunctionKey) {
            // Instead of the value 63272 we send 127.
            m_requester->textEntered(127);
        }

        // Otherwise, let's see what our hidden field has computed
        else {
            NSString* string = [m_hiddenTextView string];

            // Send each character to SFML event requester
            for (NSUInteger index = 0; index < [string length]; ++index) {
                m_requester->textEntered([string characterAtIndex:index]);
            }

            // Empty our hidden cache
            [m_hiddenTextView setString:@""];
        }
    }
}


////////////////////////////////////////////////////////
-(void)keyUp:(NSEvent *)theEvent
{
    // Transmit to non-SFML responder
    [[self nextResponder] keyUp:theEvent];
 
    if (m_requester == 0) return;
    
    sf::Event::KeyEvent key = [SFOpenGLView convertNSKeyEventToSFMLEvent:theEvent];
    
    if (key.code != sf::Keyboard::Unknown) { // The key is recognized.
        m_requester->keyUp(key);
    }
}


////////////////////////////////////////////////////////
-(void)flagsChanged:(NSEvent *)theEvent
{
    // Transmit to non-SFML responder
    [[self nextResponder] flagsChanged:theEvent];
 
    if (m_requester == 0) return;
    
    NSUInteger modifiers = [theEvent modifierFlags];
    handleModifiersChanged(modifiers, *m_requester);
}


////////////////////////////////////////////////////////
+(sf::Event::KeyEvent)convertNSKeyEventToSFMLEvent:(NSEvent *)anEvent
{
    // Key code
    sf::Keyboard::Key key = sf::Keyboard::Unknown;
    
    // First we look if the key down is from a list of characters
    // that depend on keyboard localization.
    NSString* string = [anEvent charactersIgnoringModifiers];
    if ([string length] > 0) {
        key = sf::priv::HIDInputManager::localizedKeys([string characterAtIndex:0]);
    }
    
    // If the key is not a localized one, we try to find a corresponding code
    // through virtual key code.
    if (key == sf::Keyboard::Unknown) {
        key = sf::priv::HIDInputManager::nonLocalizedKeys([anEvent keyCode]);
    }
    
//#ifdef SFML_DEBUG // Don't bother the final customers with annoying messages.
//    if (key.code == sf::Keyboard::Unknown) { // The key is unknown.
//        sf::err() << "This is an unknow key. Virtual key code is 0x"
//                  << std::hex
//                  << [anEvent keyCode]
//                  << "."
//                  << std::endl;
//    }
//#endif
    
    return keyEventWithModifiers([anEvent modifierFlags], key);
}

@end


#pragma mark - C-like functions

BOOL isValidTextUnicode(NSEvent* event)
{
    if ([event keyCode] == 0x35) { // Escape
        return false;
    } else if ([[event characters] length] > 0) {
        unichar code = [[event characters] characterAtIndex:0];
        return code < 0xF700 || code > 0xF8FF;
    } else {
        return true;
    }
}

