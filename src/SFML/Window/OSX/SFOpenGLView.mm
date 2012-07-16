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

////////////////////////////////////////////////////////////
/// Here are define the mask value for the 'modifiers' keys (cmd, ctrl, alt, shift)
///
/// As I don't have the right control keycode I cannot implement left-right
/// recognition for this key.
#warning Missing keycode for right control key.
/// #define NSRightControlKeyMask       0x...
/// #define NSLeftControlKeyMask        0x40101
///
////////////////////////////////////////////////////////////
#define NSRightShiftKeyMask         0x020004
#define NSLeftShiftKeyMask          0x020002
#define NSRightCommandKeyMask       0x100010
#define NSLeftCommandKeyMask        0x100008
#define NSRightAlternateKeyMask     0x080040
#define NSLeftAlternateKeyMask      0x080020

////////////////////////////////////////////////////////////
/// Erase (replace with 0) the given bits mask from the given data bits.
///
////////////////////////////////////////////////////////////
NSUInteger eraseMaskFromData(NSUInteger data, NSUInteger mask);

////////////////////////////////////////////////////////////
/// Erase (replace with 0) everything execept the given bits mask from the given data bits.
///
////////////////////////////////////////////////////////////
NSUInteger keepOnlyMaskFromData(NSUInteger data, NSUInteger mask);

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
/// Init the 'modifiers' key state.
/// 
////////////////////////////////////////////////////////////
-(void)initModifiersState;

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
        [self initModifiersState];
        
        // Register for mouse-move event
        m_mouseIsIn = [self isMouseInside];
        m_trackingTag = [self addTrackingRect:[self frame]
                                        owner:self
                                     userData:nil
                                 assumeInside:m_mouseIsIn];
        
        // Register for resize event
        NSNotificationCenter* center = [NSNotificationCenter defaultCenter];
        [center addObserver:self
                   selector:@selector(frameDidChange:)
                       name:NSViewFrameDidChangeNotification
                     object:self];
    }

    return self;
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
-(void)setCursorPositionToX:(unsigned int)x Y:(unsigned int)y
{
    if (m_requester == 0) return;
    
    // Create a SFML event.
    m_requester->mouseMovedAt(x, y);
    
    // Recompute the mouse pos if required.
    if (!NSEqualSizes(m_realSize, NSZeroSize)) {
        x = x / m_realSize.width  * [self frame].size.width;
        y = y / m_realSize.height * [self frame].size.height;
    }

    // Note : -[NSWindow convertBaseToScreen:] is deprecated on 10.7
    //        but the recommended -[NSWindow convertRectToScreen] is not
    //        available until 10.7.
    //
    //        So we stick with the old one for now.

    
    // Flip SFML coordinates to match window coordinates
    y = [self frame].size.height - y;
    
    // Get the position of (x, y) in the coordinate system of the window.
    NSPoint p = [self convertPoint:NSMakePoint(x, y) toView:self];
    p = [self convertPoint:p toView:nil]; // nil means window
    
    // Convert it to screen coordinates
    p = [[self window] convertBaseToScreen:p];
    
    // Flip screen coodinates to match CGDisplayMoveCursorToPoint referential.
    float const screenHeight = [[[self window] screen] frame].size.height;
    p.y = screenHeight - p.y;
    
    x = p.x;
    y = p.y;

    
    // Get the id of the screen
    CGDirectDisplayID screenNumber = (CGDirectDisplayID)[[[[[self window] screen] deviceDescription] valueForKey:@"NSScreenNumber"] intValue];
    
    // Place the cursor.
    CGDisplayMoveCursorToPoint(screenNumber, CGPointMake(x, y));
    /*
     * CGDisplayMoveCursorToPoint -- Discussion :
     *
     * No events are generated as a result of this move. 
     * Points that lie outside the desktop are clipped to the desktop.
     */
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
    BOOL mouseWasIn = m_mouseIsIn;
    m_mouseIsIn = [self isMouseInside];
    
    // Send event if needed.
    if (mouseWasIn && !m_mouseIsIn) {
        [self mouseExited:nil];
    } else if (!mouseWasIn && m_mouseIsIn) {
        [self mouseEntered:nil];
    }
    
    // Adapt tracking area for mouse mouse event.
    [self removeTrackingRect:m_trackingTag];
    m_trackingTag = [self addTrackingRect:[self frame]
                                    owner:self
                                 userData:nil
                             assumeInside:m_mouseIsIn];
    
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


#pragma mark
#pragma mark Subclassing methods


////////////////////////////////////////////////////////
-(void)dealloc
{
    // Unregister
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [self removeTrackingRect:m_trackingTag];
    
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
    m_mouseIsIn = YES;
    
    if (m_requester == 0) return;
    
    m_requester->mouseMovedIn();
}


////////////////////////////////////////////////////////
-(void)mouseExited:(NSEvent *)theEvent
{
    m_mouseIsIn = NO;
    
    if (m_requester == 0) return;
    
    m_requester->mouseMovedOut();
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
        // If the event is not useful.
        if (!m_mouseIsIn) return;
        
        NSPoint loc = [self cursorPositionFromEvent:theEvent];
        
        m_requester->mouseMovedAt(loc.x, loc.y);
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


    // Handle text entred event
    // We create a new event without command/ctrl modifiers 
    // to prevent the OS from sending an alert
    NSUInteger modifiers = [theEvent modifierFlags];
    
    if (modifiers & NSCommandKeyMask) modifiers = modifiers & ~NSCommandKeyMask;
    if (modifiers & NSControlKeyMask) modifiers = modifiers & ~NSControlKeyMask;
    
    NSEvent* ev = [NSEvent keyEventWithType:NSKeyDown
                                   location:[theEvent locationInWindow]
                              modifierFlags:modifiers
                                  timestamp:[theEvent timestamp]
                               windowNumber:[theEvent windowNumber]
                                    context:[theEvent context]
                                 characters:[theEvent characters]
                charactersIgnoringModifiers:[theEvent charactersIgnoringModifiers]
                                  isARepeat:[theEvent isARepeat]
                                    keyCode:[theEvent keyCode]];
    
    if ((m_useKeyRepeat || ![ev isARepeat]) && [[ev characters] length] > 0) {
        
        // Ignore escape key and non text keycode. (See NSEvent.h)
        // They produce a sound alert.
        unichar code = [[ev characters] characterAtIndex:0];
        unsigned short keycode = [ev keyCode];
        
        // Backspace and Delete unicode values are badly handled by Apple.
        // We do a small workaround here :
        
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
        
        // All other unicode values
        else if (keycode != 0x35 && (code < 0xF700 || code > 0xF8FF)) {
            
            // Let's see if its a valid text.
            NSText* text = [[self window] fieldEditor:YES forObject:self];
            [text interpretKeyEvents:[NSArray arrayWithObject:ev]];
            
            NSString* string = [text string];
            if ([string length] > 0) {
                // It's a valid TextEntered event.
                m_requester->textEntered([string characterAtIndex:0]);
                
                [text setString:@""];
            }
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
    
    // Setup a potential event key.
    sf::Event::KeyEvent key;
    key.code    = sf::Keyboard::Unknown;
    key.alt     = modifiers & NSAlternateKeyMask;
    key.control = modifiers & NSControlKeyMask;
    key.shift   = modifiers & NSShiftKeyMask;
    key.system  = modifiers & NSCommandKeyMask;
    
    // State
    BOOL rightShiftIsDown       = NO;
    BOOL leftShiftIsDown        = NO;
    BOOL rightCommandIsDown     = NO;
    BOOL leftCommandIsDown      = NO;
    BOOL rightAlternateIsDown   = NO;
    BOOL leftAlternateIsDown    = NO;
    BOOL controlIsDown          = NO;
    
    // Shift keys.
    if (modifiers & NSShiftKeyMask) { // At least one shift key is down.
        // Clean up modifiers to keep only 'shift' bits.
        NSUInteger shift = keepOnlyMaskFromData(modifiers, NSRightShiftKeyMask | NSLeftShiftKeyMask);
        
        // Only right shift is down ?
        if (shift == NSRightShiftKeyMask) {
            
            rightShiftIsDown = YES;
            
            if (m_leftShiftWasDown) {
                // left shift released
                leftShiftIsDown  = NO;
                
                key.code = sf::Keyboard::LShift;
                m_requester->keyUp(key);
            }
            
            if (!m_rightShiftWasDown) {
                // right shift pressed
                
                key.code = sf::Keyboard::RShift;
                m_requester->keyDown(key);
            }
        }
        
        // Only left shift is down ?
        if (shift == NSLeftShiftKeyMask) {
            
            leftShiftIsDown = YES;
            
            if (m_rightShiftWasDown) {
                // right shift released
                rightShiftIsDown = NO;
                
                key.code = sf::Keyboard::RShift;
                m_requester->keyUp(key);
            }
            
            if (!m_leftShiftWasDown) {
                // left shift pressed
                
                key.code = sf::Keyboard::LShift;
                m_requester->keyDown(key);
            }
        }
        
        // Or are they both down ?
        if (shift == (NSRightShiftKeyMask | NSLeftShiftKeyMask)) {
            
            rightShiftIsDown = YES;
            leftShiftIsDown = YES;
            
            if (!m_rightShiftWasDown) {
                // right shift pressed
                
                key.code = sf::Keyboard::RShift;
                m_requester->keyDown(key);
            }
            
            if (!m_leftShiftWasDown) {
                // left shift pressed
                
                key.code = sf::Keyboard::LShift;
                m_requester->keyDown(key);
            }
        }
    } else { // No shift key down.
        
        rightShiftIsDown = NO;
        leftShiftIsDown  = NO;
        
        if (m_rightShiftWasDown) {
            // right shift released
            
            key.code = sf::Keyboard::RShift;
            m_requester->keyUp(key);
        }
        
        if (m_leftShiftWasDown) {
            // left shift released
            
            key.code = sf::Keyboard::LShift;
            m_requester->keyUp(key);
        }
    }
    
    // Command keys.
    if (modifiers & NSCommandKeyMask) { // At least one command key is down.
        // Clean up modifiers to keep only 'Command' bits.
        NSUInteger command = keepOnlyMaskFromData(modifiers, NSRightCommandKeyMask | NSLeftCommandKeyMask);

        // Only right Command is down ?
        if (command == NSRightCommandKeyMask) {
            
            rightCommandIsDown = YES;
            
            if (m_leftCommandWasDown) {
                // left command released
                leftCommandIsDown  = NO;
                
                key.code = sf::Keyboard::LSystem;
                m_requester->keyUp(key);
            }
            
            if (!m_rightCommandWasDown) {
                // right command pressed
                
                key.code = sf::Keyboard::RSystem;
                m_requester->keyDown(key);
            }
        }
        
        // Only left Command is down ?
        if (command == NSLeftCommandKeyMask) {
            
            leftCommandIsDown = YES;
            
            if (m_rightCommandWasDown) {
                // right command released
                rightCommandIsDown = NO;
                
                key.code = sf::Keyboard::RSystem;
                m_requester->keyUp(key);
            }
            
            if (!m_leftCommandWasDown) {
                // left command pressed
                
                key.code = sf::Keyboard::LSystem;
                m_requester->keyDown(key);
            }
        }
        
        // Or are they both down ?
        if (command == (NSRightCommandKeyMask | NSLeftCommandKeyMask)) {
            
            rightCommandIsDown = YES;
            leftCommandIsDown = YES;
            
            if (!m_rightCommandWasDown) {
                // right command pressed
                
                key.code = sf::Keyboard::RSystem;
                m_requester->keyDown(key);
            }
            
            if (!m_leftCommandWasDown) {
                // left command pressed
                
                key.code = sf::Keyboard::LSystem;
                m_requester->keyDown(key);
            }
        }
    } else { // No Command key down.
        
        rightCommandIsDown = NO;
        leftCommandIsDown  = NO;
        
        if (m_rightCommandWasDown) {
            // right command released
            
            key.code = sf::Keyboard::RSystem;
            m_requester->keyUp(key);
        }
        
        if (m_leftCommandWasDown) {
            // left command released
            
            key.code = sf::Keyboard::LSystem;
            m_requester->keyUp(key);
        }
    }
    
    // Alternate keys.
    if (modifiers & NSAlternateKeyMask) { // At least one alternate key is down.
        // Clean up modifiers to keep only 'Alternate' bits.
        NSUInteger alternate = keepOnlyMaskFromData(modifiers, NSRightAlternateKeyMask | NSLeftAlternateKeyMask);
        
        // Only right Alternate is down ?
        if (alternate == NSRightAlternateKeyMask) {
            
            rightAlternateIsDown = YES;
            
            if (m_leftAlternateWasDown) {
                // left alt released
                leftAlternateIsDown  = NO;
                
                key.code = sf::Keyboard::LAlt;
                m_requester->keyUp(key);
            }
            
            if (!m_rightAlternateWasDown) {
                // right alt pressed
                
                key.code = sf::Keyboard::RAlt;
                m_requester->keyDown(key);
            }
        }
        
        // Only left Alternate is down ?
        if (alternate == NSLeftAlternateKeyMask) {
            
            leftAlternateIsDown = YES;
            
            if (m_rightAlternateWasDown) {
                // right alt released
                rightAlternateIsDown = NO;
                
                key.code = sf::Keyboard::RAlt;
                m_requester->keyUp(key);
            }
            
            if (!m_leftAlternateWasDown) {
                // left alt pressed
                
                key.code = sf::Keyboard::LAlt;
                m_requester->keyDown(key);
            }
        }
        
        // Or are they both down ?
        if (alternate == (NSRightAlternateKeyMask | NSLeftAlternateKeyMask)) {
            
            rightAlternateIsDown = YES;
            leftAlternateIsDown = YES;
            
            if (!m_rightAlternateWasDown) {
                // right alt pressed
                
                key.code = sf::Keyboard::RAlt;
                m_requester->keyDown(key);
            }
            
            if (!m_leftAlternateWasDown) {
                // left alt pressed
                
                key.code = sf::Keyboard::LAlt;
                m_requester->keyDown(key);
            }
        }
    } else { // No Alternate key down.
        
        rightAlternateIsDown = NO;
        leftAlternateIsDown  = NO;
        
        if (m_rightAlternateWasDown) {
            // right alt released
            
            key.code = sf::Keyboard::RAlt;
            m_requester->keyUp(key);
        }
        
        if (m_leftAlternateWasDown) {
            // left alt released
            
            key.code = sf::Keyboard::LAlt;
            m_requester->keyUp(key);
        }
    }
    
    // Control keys.
    if (modifiers & NSControlKeyMask) {
        // Currently only the left control key will be used in SFML (see note above).
        
        controlIsDown = YES;
        
        if (!m_controlWasDown) {
            // ctrl pressed
            
            key.code = sf::Keyboard::LControl;
            m_requester->keyDown(key);
        }
    } else { // No control key down.
        controlIsDown = NO;
        
        if (m_controlWasDown) {
            // ctrl released
            
            key.code = sf::Keyboard::LControl;
            m_requester->keyUp(key);
        }
    }
    
    // Update the state
    m_rightShiftWasDown     = rightShiftIsDown;
    m_leftShiftWasDown      = leftShiftIsDown;
    m_rightCommandWasDown   = rightCommandIsDown;
    m_leftCommandWasDown    = leftCommandIsDown;
    m_rightAlternateWasDown = rightAlternateIsDown;
    m_leftAlternateWasDown  = leftAlternateIsDown;
    m_controlWasDown        = controlIsDown;
}


////////////////////////////////////////////////////////
-(void)initModifiersState
{
    // Set default value to NO.
    m_rightShiftWasDown       = NO;
    m_leftShiftWasDown        = NO;
    m_rightCommandWasDown     = NO;
    m_leftCommandWasDown      = NO;
    m_rightAlternateWasDown   = NO;
    m_leftAlternateWasDown    = NO;
    m_controlWasDown          = NO;
    
    NSUInteger modifiers = [[NSApp currentEvent] modifierFlags];
    modifiers = eraseMaskFromData(modifiers, 0x100); // We erase something useless that might be present.
    
    // Shift keys.
    if (modifiers & NSShiftKeyMask) { // At least one shift key is down.
        // Clean up modifiers to keep only 'shift' bits.
        NSUInteger shift = keepOnlyMaskFromData(modifiers, NSRightShiftKeyMask | NSLeftShiftKeyMask);
        
        // Only right shift is down ?
        if (shift == NSRightShiftKeyMask) {
            
            m_rightShiftWasDown = YES;
        }
        
        // Only left shift is down ?
        if (shift == NSLeftShiftKeyMask) {
            
            m_leftShiftWasDown = YES;
        }
        
        // Or are they both down ?
        if (shift == (NSRightShiftKeyMask | NSLeftShiftKeyMask)) {
            
            m_rightShiftWasDown = YES;
            m_leftShiftWasDown = YES;
        }
    }
    
    // Command keys.
    if (modifiers & NSCommandKeyMask) { // At least one command key is down.
        // Clean up modifiers to keep only 'Command' bits.
        NSUInteger command = keepOnlyMaskFromData(modifiers, NSRightCommandKeyMask | NSLeftCommandKeyMask);
        
        // Only right Command is down ?
        if (command == NSRightCommandKeyMask) {
            
            m_rightCommandWasDown = YES;
        }
        
        // Only left Command is down ?
        if (command == NSLeftCommandKeyMask) {
            
            m_leftCommandWasDown = YES;
        }
        
        // Or are they both down ?
        if (command == (NSRightCommandKeyMask | NSLeftCommandKeyMask)) {
            
            m_rightCommandWasDown = YES;
            m_leftCommandWasDown = YES;
        }
    }
    
    // Alternate keys.
    if (modifiers & NSAlternateKeyMask) { // At least one alternate key is down.
        // Clean up modifiers to keep only 'Alternate' bits.
        NSUInteger alternate = keepOnlyMaskFromData(modifiers, NSRightAlternateKeyMask | NSLeftAlternateKeyMask);
        
        // Only right Alternate is down ?
        if (alternate == NSRightAlternateKeyMask) {
            
            m_rightAlternateWasDown = YES;
        }
        
        // Only left Alternate is down ?
        if (alternate == NSLeftAlternateKeyMask) {
            
            m_leftAlternateWasDown = YES;
        }
        
        // Or are they both down ?
        if (alternate == (NSRightAlternateKeyMask | NSLeftAlternateKeyMask)) {
            
            m_rightAlternateWasDown = YES;
            m_leftAlternateWasDown = YES;
        }

    }
    
    // Control keys.
    if (modifiers & NSControlKeyMask) {
        // Currently only the left control key will be used in SFML (see note above).
        
        m_controlWasDown = YES;
    }
}


////////////////////////////////////////////////////////
+(sf::Event::KeyEvent)convertNSKeyEventToSFMLEvent:(NSEvent *)anEvent
{
    sf::Event::KeyEvent key;
    
    // Modifiers.
    NSUInteger modifierFlags = [anEvent modifierFlags];
    key.alt     = modifierFlags & NSAlternateKeyMask;
    key.control = modifierFlags & NSControlKeyMask;
    key.shift   = modifierFlags & NSShiftKeyMask;
    key.system  = modifierFlags & NSCommandKeyMask;
    
    // Key code.
    key.code = sf::Keyboard::Unknown;
    
    // First we look if the key down is from a list of caracter 
    // that depend on keyboard localization.
    NSString* string = [anEvent charactersIgnoringModifiers];
    if ([string length] > 0) {
        key.code = sf::priv::HIDInputManager::localizedKeys([string characterAtIndex:0]);
    }
    
    // The key is not a localized one, so we try to find a corresponding code
    // through virtual key code.
    if (key.code == sf::Keyboard::Unknown) {
        key.code = sf::priv::HIDInputManager::nonLocalizedKeys([anEvent keyCode]);
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
    
    return key;
}

@end


#pragma mark - C-like functions


////////////////////////////////////////////////////////
NSUInteger eraseMaskFromData(NSUInteger data, NSUInteger mask)
{
    return (data | mask) ^ mask;
}


////////////////////////////////////////////////////////
NSUInteger keepOnlyMaskFromData(NSUInteger data, NSUInteger mask)
{
    NSUInteger negative = NSUIntegerMax ^ mask;
    return eraseMaskFromData(data, negative);
}


