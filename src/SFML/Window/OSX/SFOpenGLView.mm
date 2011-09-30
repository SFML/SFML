////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2011 Marco Antognini (antognini.marco@gmail.com), 
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
#include <SFML/Window/OSX/HIDInputManager.hpp> // For LocalizedKeys and NonLocalizedKeys
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
NSUInteger EraseMaskFromData(NSUInteger data, NSUInteger mask);

////////////////////////////////////////////////////////////
/// Erase (replace with 0) everything execept the given bits mask from the given data bits.
///
////////////////////////////////////////////////////////////
NSUInteger KeepOnlyMaskFromData(NSUInteger data, NSUInteger mask);

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
/// Converte the NSEvent mouse button type to SFML type.
///
/// Returns ButtonCount if the button is unknown
/// 
////////////////////////////////////////////////////////////
-(sf::Mouse::Button)mouseButtonFromEvent:(NSEvent *)event;

////////////////////////////////////////////////////////////
/// Convert a key down/up NSEvent into an SFML key event.
/// Based on LocalizedKeys and NonLocalizedKeys function.
///
/// Return sf::Keyboard::KeyCount as Code if the key is unknown.
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
        myRealSize = NSZeroSize;
        [self initModifiersState];
        
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
                     object:self];
    }

    return self;
}


////////////////////////////////////////////////////////
-(void)setRequesterTo:(sf::priv::WindowImplCocoa *)requester
{
    myRequester = requester;
}


////////////////////////////////////////////////////////
-(void)setRealSize:(NSSize)newSize
{
    myRealSize = newSize;
}


////////////////////////////////////////////////////////
-(void)setCursorPositionToX:(unsigned int)x Y:(unsigned int)y
{
    // Flip for SFML window coordinate system
    y = NSHeight([[self window] frame]) - y;
    
    // Adjust for view reference instead of window
    y -= NSHeight([[self window] frame]) - NSHeight([self frame]);
    
    // Convert to screen coordinates
    NSPoint screenCoord = [[self window] convertBaseToScreen:NSMakePoint(x, y)];
    
    // Flip screen coodinates
    float const screenHeight = NSHeight([[[self window] screen] frame]);
    screenCoord.y = screenHeight - screenCoord.y;
    
    // Recompute the mouse pos if required.
    if (!NSEqualSizes(myRealSize, NSZeroSize)) {
        screenCoord.x = screenCoord.x / myRealSize.width  * [self frame].size.width;
        screenCoord.y = screenCoord.y / myRealSize.height * [self frame].size.height;
    }
    
    // Place the cursor.
    CGEventRef event = CGEventCreateMouseEvent(NULL, 
                                               kCGEventMouseMoved, 
                                               CGPointMake(screenCoord.x, 
                                                           screenCoord.y), 
                                               /*we don't care about this : */0);
    CGEventPost(kCGHIDEventTap, event);
    CFRelease(event);
    // This is a workaround to deprecated CGSetLocalEventsSuppressionInterval function
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
-(void)frameDidChange:(NSNotification *)notification
{
    // Update mouse internal state.
    BOOL mouseWasIn = myMouseIsIn;
    myMouseIsIn = [self isMouseInside];
    
    // Send event if needed.
    if (mouseWasIn && !myMouseIsIn) {
        [self mouseExited:nil];
    } else if (!mouseWasIn && myMouseIsIn) {
        [self mouseEntered:nil];
    }
    
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
    if (myRequester != 0) {
        NSPoint loc = [self cursorPositionFromEvent:theEvent];
        
        myRequester->MouseWheelScrolledAt([theEvent deltaY], loc.x, loc.y);
    }
    
    // Transmit to non-SFML responder
    [[self nextResponder] scrollWheel:theEvent];
}


////////////////////////////////////////////////////////
-(void)mouseEntered:(NSEvent *)theEvent
{
    myMouseIsIn = YES;
    
    if (myRequester == 0) return;
    
    myRequester->MouseMovedIn();
}


////////////////////////////////////////////////////////
-(void)mouseExited:(NSEvent *)theEvent
{
    myMouseIsIn = NO;
    
    if (myRequester == 0) return;
    
    myRequester->MouseMovedOut();
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
    
    if (myRequester != 0) {
        NSPoint loc = [self cursorPositionFromEvent:theEvent];
        
        if (button != sf::Mouse::ButtonCount) {
            myRequester->MouseDownAt(button, loc.x, loc.y);
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
    
    if (myRequester != 0) {
        NSPoint loc = [self cursorPositionFromEvent:theEvent];
        
        if (button != sf::Mouse::ButtonCount) {
            myRequester->MouseUpAt(button, loc.x, loc.y);
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
    if (myRequester != 0) {
        // If the event is not useful.
        if (!myMouseIsIn) return;
        
        NSPoint loc = [self cursorPositionFromEvent:theEvent];
        
        myRequester->MouseMovedAt(loc.x, loc.y);
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
    if (!NSEqualSizes(myRealSize, NSZeroSize)) {
        loc.x = loc.x * myRealSize.width  / [self frame].size.width;
        loc.y = loc.y * myRealSize.height / [self frame].size.height;
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
    
    if (myRequester == 0) return;
    
    // Handle key down event
    if (myUseKeyRepeat || ![theEvent isARepeat]) {
        sf::Event::KeyEvent key = [SFOpenGLView convertNSKeyEventToSFMLEvent:theEvent];
        
        if (key.Code != sf::Keyboard::KeyCount) { // The key is recognized.
            myRequester->KeyDown(key);
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
    
    if ((myUseKeyRepeat || ![ev isARepeat]) && [[ev characters] length] > 0) {
        
        // Ignore escape key and non text keycode. (See NSEvent.h)
        // They produce a sound alert.
        unichar code = [[ev characters] characterAtIndex:0];
        if ([ev keyCode] != 0x35 && (code < 0xF700 || code > 0xF8FF)) {
            
            // Let's see if its a valid text.
            NSText* text = [[self window] fieldEditor:YES forObject:self];
            [text interpretKeyEvents:[NSArray arrayWithObject:ev]];
            
            NSString* string = [text string];
            if ([string length] > 0) {
                // It's a valid TextEntered event.
                myRequester->TextEntered([string characterAtIndex:0]);
                
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
 
    if (myRequester == 0) return;
    
    sf::Event::KeyEvent key = [SFOpenGLView convertNSKeyEventToSFMLEvent:theEvent];
    
    if (key.Code != sf::Keyboard::KeyCount) { // The key is recognized.
        myRequester->KeyUp(key);
    }
}


////////////////////////////////////////////////////////
-(void)flagsChanged:(NSEvent *)theEvent
{
    // Transmit to non-SFML responder
    [[self nextResponder] flagsChanged:theEvent];
 
    if (myRequester == 0) return;
    
    NSUInteger modifiers = [theEvent modifierFlags];
    
    // Setup a potential event key.
    sf::Event::KeyEvent key;
    key.Code    = sf::Keyboard::KeyCount;
    key.Alt     = modifiers & NSAlternateKeyMask;
    key.Control = modifiers & NSControlKeyMask;
    key.Shift   = modifiers & NSShiftKeyMask;
    key.System  = modifiers & NSCommandKeyMask;
    
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
        NSUInteger shift = KeepOnlyMaskFromData(modifiers, NSRightShiftKeyMask | NSLeftShiftKeyMask);
        
        // Only right shift is down ?
        if (shift == NSRightShiftKeyMask) {
            
            rightShiftIsDown = YES;
            
            if (myLeftShiftWasDown) {
                // left shift released
                leftShiftIsDown  = NO;
                
                key.Code = sf::Keyboard::LShift;
                myRequester->KeyUp(key);
            }
            
            if (!myRightShiftWasDown) {
                // right shift pressed
                
                key.Code = sf::Keyboard::RShift;
                myRequester->KeyDown(key);
            }
        }
        
        // Only left shift is down ?
        if (shift == NSLeftShiftKeyMask) {
            
            leftShiftIsDown = YES;
            
            if (myRightShiftWasDown) {
                // right shift released
                rightShiftIsDown = NO;
                
                key.Code = sf::Keyboard::RShift;
                myRequester->KeyUp(key);
            }
            
            if (!myLeftShiftWasDown) {
                // left shift pressed
                
                key.Code = sf::Keyboard::LShift;
                myRequester->KeyDown(key);
            }
        }
        
        // Or are they both down ?
        if (shift == (NSRightShiftKeyMask | NSLeftShiftKeyMask)) {
            
            rightShiftIsDown = YES;
            leftShiftIsDown = YES;
            
            if (!myRightShiftWasDown) {
                // right shift pressed
                
                key.Code = sf::Keyboard::RShift;
                myRequester->KeyDown(key);
            }
            
            if (!myLeftShiftWasDown) {
                // left shift pressed
                
                key.Code = sf::Keyboard::LShift;
                myRequester->KeyDown(key);
            }
        }
    } else { // No shift key down.
        
        rightShiftIsDown = NO;
        leftShiftIsDown  = NO;
        
        if (myRightShiftWasDown) {
            // right shift released
            
            key.Code = sf::Keyboard::RShift;
            myRequester->KeyUp(key);
        }
        
        if (myLeftShiftWasDown) {
            // left shift released
            
            key.Code = sf::Keyboard::LShift;
            myRequester->KeyUp(key);
        }
    }
    
    // Command keys.
    if (modifiers & NSCommandKeyMask) { // At least one command key is down.
        // Clean up modifiers to keep only 'Command' bits.
        NSUInteger command = KeepOnlyMaskFromData(modifiers, NSRightCommandKeyMask | NSLeftCommandKeyMask);

        // Only right Command is down ?
        if (command == NSRightCommandKeyMask) {
            
            rightCommandIsDown = YES;
            
            if (myLeftCommandWasDown) {
                // left command released
                leftCommandIsDown  = NO;
                
                key.Code = sf::Keyboard::LSystem;
                myRequester->KeyUp(key);
            }
            
            if (!myRightCommandWasDown) {
                // right command pressed
                
                key.Code = sf::Keyboard::RSystem;
                myRequester->KeyDown(key);
            }
        }
        
        // Only left Command is down ?
        if (command == NSLeftCommandKeyMask) {
            
            leftCommandIsDown = YES;
            
            if (myRightCommandWasDown) {
                // right command released
                rightCommandIsDown = NO;
                
                key.Code = sf::Keyboard::RSystem;
                myRequester->KeyUp(key);
            }
            
            if (!myLeftCommandWasDown) {
                // left command pressed
                
                key.Code = sf::Keyboard::LSystem;
                myRequester->KeyDown(key);
            }
        }
        
        // Or are they both down ?
        if (command == (NSRightCommandKeyMask | NSLeftCommandKeyMask)) {
            
            rightCommandIsDown = YES;
            leftCommandIsDown = YES;
            
            if (!myRightCommandWasDown) {
                // right command pressed
                
                key.Code = sf::Keyboard::RSystem;
                myRequester->KeyDown(key);
            }
            
            if (!myLeftCommandWasDown) {
                // left command pressed
                
                key.Code = sf::Keyboard::LSystem;
                myRequester->KeyDown(key);
            }
        }
    } else { // No Command key down.
        
        rightCommandIsDown = NO;
        leftCommandIsDown  = NO;
        
        if (myRightCommandWasDown) {
            // right command released
            
            key.Code = sf::Keyboard::RSystem;
            myRequester->KeyUp(key);
        }
        
        if (myLeftCommandWasDown) {
            // left command released
            
            key.Code = sf::Keyboard::LSystem;
            myRequester->KeyUp(key);
        }
    }
    
    // Alternate keys.
    if (modifiers & NSAlternateKeyMask) { // At least one alternate key is down.
        // Clean up modifiers to keep only 'Alternate' bits.
        NSUInteger alternate = KeepOnlyMaskFromData(modifiers, NSRightAlternateKeyMask | NSLeftAlternateKeyMask);
        
        // Only right Alternate is down ?
        if (alternate == NSRightAlternateKeyMask) {
            
            rightAlternateIsDown = YES;
            
            if (myLeftAlternateWasDown) {
                // left alt released
                leftAlternateIsDown  = NO;
                
                key.Code = sf::Keyboard::LAlt;
                myRequester->KeyUp(key);
            }
            
            if (!myRightAlternateWasDown) {
                // right alt pressed
                
                key.Code = sf::Keyboard::RAlt;
                myRequester->KeyDown(key);
            }
        }
        
        // Only left Alternate is down ?
        if (alternate == NSLeftAlternateKeyMask) {
            
            leftAlternateIsDown = YES;
            
            if (myRightAlternateWasDown) {
                // right alt released
                rightAlternateIsDown = NO;
                
                key.Code = sf::Keyboard::RAlt;
                myRequester->KeyUp(key);
            }
            
            if (!myLeftAlternateWasDown) {
                // left alt pressed
                
                key.Code = sf::Keyboard::LAlt;
                myRequester->KeyDown(key);
            }
        }
        
        // Or are they both down ?
        if (alternate == (NSRightAlternateKeyMask | NSLeftAlternateKeyMask)) {
            
            rightAlternateIsDown = YES;
            leftAlternateIsDown = YES;
            
            if (!myRightAlternateWasDown) {
                // right alt pressed
                
                key.Code = sf::Keyboard::RAlt;
                myRequester->KeyDown(key);
            }
            
            if (!myLeftAlternateWasDown) {
                // left alt pressed
                
                key.Code = sf::Keyboard::LAlt;
                myRequester->KeyDown(key);
            }
        }
    } else { // No Alternate key down.
        
        rightAlternateIsDown = NO;
        leftAlternateIsDown  = NO;
        
        if (myRightAlternateWasDown) {
            // right alt released
            
            key.Code = sf::Keyboard::RAlt;
            myRequester->KeyUp(key);
        }
        
        if (myLeftAlternateWasDown) {
            // left alt released
            
            key.Code = sf::Keyboard::LAlt;
            myRequester->KeyUp(key);
        }
    }
    
    // Control keys.
    if (modifiers & NSControlKeyMask) {
        // Currently only the left control key will be used in SFML (see note above).
        
        controlIsDown = YES;
        
        if (!myControlWasDown) {
            // ctrl pressed
            
            key.Code = sf::Keyboard::LControl;
            myRequester->KeyDown(key);
        }
    } else { // No control key down.
        controlIsDown = NO;
        
        if (myControlWasDown) {
            // ctrl released
            
            key.Code = sf::Keyboard::LControl;
            myRequester->KeyUp(key);
        }
    }
    
    // Update the state
    myRightShiftWasDown     = rightShiftIsDown;
    myLeftShiftWasDown      = leftShiftIsDown;
    myRightCommandWasDown   = rightCommandIsDown;
    myLeftCommandWasDown    = leftCommandIsDown;
    myRightAlternateWasDown = rightAlternateIsDown;
    myLeftAlternateWasDown  = leftAlternateIsDown;
    myControlWasDown        = controlIsDown;
}


////////////////////////////////////////////////////////
-(void)initModifiersState
{
    // Set default value to NO.
    myRightShiftWasDown       = NO;
    myLeftShiftWasDown        = NO;
    myRightCommandWasDown     = NO;
    myLeftCommandWasDown      = NO;
    myRightAlternateWasDown   = NO;
    myLeftAlternateWasDown    = NO;
    myControlWasDown          = NO;
    
    NSUInteger modifiers = [[NSApp currentEvent] modifierFlags];
    modifiers = EraseMaskFromData(modifiers, 0x100); // We erase something useless that might be present.
    
    // Shift keys.
    if (modifiers & NSShiftKeyMask) { // At least one shift key is down.
        // Clean up modifiers to keep only 'shift' bits.
        NSUInteger shift = KeepOnlyMaskFromData(modifiers, NSRightShiftKeyMask | NSLeftShiftKeyMask);
        
        // Only right shift is down ?
        if (shift == NSRightShiftKeyMask) {
            
            myRightShiftWasDown = YES;
        }
        
        // Only left shift is down ?
        if (shift == NSLeftShiftKeyMask) {
            
            myLeftShiftWasDown = YES;
        }
        
        // Or are they both down ?
        if (shift == (NSRightShiftKeyMask | NSLeftShiftKeyMask)) {
            
            myRightShiftWasDown = YES;
            myLeftShiftWasDown = YES;
        }
    }
    
    // Command keys.
    if (modifiers & NSCommandKeyMask) { // At least one command key is down.
        // Clean up modifiers to keep only 'Command' bits.
        NSUInteger command = KeepOnlyMaskFromData(modifiers, NSRightCommandKeyMask | NSLeftCommandKeyMask);
        
        // Only right Command is down ?
        if (command == NSRightCommandKeyMask) {
            
            myRightCommandWasDown = YES;
        }
        
        // Only left Command is down ?
        if (command == NSLeftCommandKeyMask) {
            
            myLeftCommandWasDown = YES;
        }
        
        // Or are they both down ?
        if (command == (NSRightCommandKeyMask | NSLeftCommandKeyMask)) {
            
            myRightCommandWasDown = YES;
            myLeftCommandWasDown = YES;
        }
    }
    
    // Alternate keys.
    if (modifiers & NSAlternateKeyMask) { // At least one alternate key is down.
        // Clean up modifiers to keep only 'Alternate' bits.
        NSUInteger alternate = KeepOnlyMaskFromData(modifiers, NSRightAlternateKeyMask | NSLeftAlternateKeyMask);
        
        // Only right Alternate is down ?
        if (alternate == NSRightAlternateKeyMask) {
            
            myRightAlternateWasDown = YES;
        }
        
        // Only left Alternate is down ?
        if (alternate == NSLeftAlternateKeyMask) {
            
            myLeftAlternateWasDown = YES;
        }
        
        // Or are they both down ?
        if (alternate == (NSRightAlternateKeyMask | NSLeftAlternateKeyMask)) {
            
            myRightAlternateWasDown = YES;
            myLeftAlternateWasDown = YES;
        }

    }
    
    // Control keys.
    if (modifiers & NSControlKeyMask) {
        // Currently only the left control key will be used in SFML (see note above).
        
        myControlWasDown = YES;
    }
}


////////////////////////////////////////////////////////
+(sf::Event::KeyEvent)convertNSKeyEventToSFMLEvent:(NSEvent *)anEvent
{
    sf::Event::KeyEvent key;
    
    // Modifiers.
    NSUInteger modifierFlags = [anEvent modifierFlags];
    key.Alt     = modifierFlags & NSAlternateKeyMask;
    key.Control = modifierFlags & NSControlKeyMask;
    key.Shift   = modifierFlags & NSShiftKeyMask;
    key.System  = modifierFlags & NSCommandKeyMask;
    
    // Key code.
    key.Code = sf::Keyboard::KeyCount;
    
    // First we look if the key down is from a list of caracter 
    // that depend on keyboard localization.
    NSString* string = [anEvent charactersIgnoringModifiers];
    if ([string length] > 0) {
        key.Code = sf::priv::HIDInputManager::LocalizedKeys([string characterAtIndex:0]);
    }
    
    // The key is not a localized one, so we try to find a corresponding code
    // through virtual key code.
    if (key.Code == sf::Keyboard::KeyCount) {
        key.Code = sf::priv::HIDInputManager::NonLocalizedKeys([anEvent keyCode]);
    }
    
//#ifdef SFML_DEBUG // Don't bother the final customers with annoying messages.
//    if (key.Code == sf::Keyboard::KeyCount) { // The key is unknown.
//        sf::Err() << "This is an unknow key. Virtual key code is 0x"
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
NSUInteger EraseMaskFromData(NSUInteger data, NSUInteger mask)
{
    return (data | mask) ^ mask;
}


////////////////////////////////////////////////////////
NSUInteger KeepOnlyMaskFromData(NSUInteger data, NSUInteger mask)
{
    NSUInteger negative = NSUIntegerMax ^ mask;
    return EraseMaskFromData(data, negative);
}


