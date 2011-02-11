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
/// Try to convert a character into a SFML key code.
/// Return sf::Key::Count if it doesn't match any 'localized' keys.
///
/// By 'localized' I mean keys that depend on the keyboard layout
/// and might not be the same as the US keycode in some country
/// (e.g. the keys 'Y' and 'Z' are switched on QWERTZ keyboard and
/// US keyboard layouts.)
///
////////////////////////////////////////////////////////////
sf::Key::Code LocalizedKeys(unichar ch);

////////////////////////////////////////////////////////////
/// Try to convert a keycode into a SFML key code.
/// Return sf::Key::Count if the keycode is unknown.
///
////////////////////////////////////////////////////////////
sf::Key::Code NonLocalizedKeys(unsigned short keycode);

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
/// Convert a key down/up NSEvent into an SFML key event.
/// Based on LocalizedKeys and NonLocalizedKeys function.
///
/// Return sf::Key::Count as Code if the key is unknown.
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
    if (myRequester == 0) return;
    
    NSPoint loc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    
    // Don't forget to change to SFML coord system.
    float h = [self frame].size.height;
    myRequester->MouseDownAt(sf::Mouse::Left, loc.x, h - loc.y);
}


////////////////////////////////////////////////////////
-(void)mouseUp:(NSEvent *)theEvent
{
    if (myRequester == 0) return;
    
    NSPoint loc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    
    // Don't forget to change to SFML coord system.
    float h = [self frame].size.height;
    loc.y = h - loc.y;
    
    // Recompute the mouse pos if required.
    if (!NSEqualSizes(myRealSize, NSZeroSize)) {
        loc.x = loc.x * myRealSize.width / [self frame].size.width;
        loc.y = loc.y * myRealSize.height / [self frame].size.height;
    }
    
    myRequester->MouseUpAt(sf::Mouse::Left, loc.x, loc.y);
}


////////////////////////////////////////////////////////
-(void)mouseMoved:(NSEvent *)theEvent
{
    if (myRequester == 0) return;
    
    // If the event is not useful.
    if (!myMouseIsIn) return;
    
    NSPoint loc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    
    // Don't forget to change to SFML coord system.
    float h = [self frame].size.height;
    loc.y = h - loc.y;
    
    // Recompute the mouse pos if required.
    if (!NSEqualSizes(myRealSize, NSZeroSize)) {
        loc.x = loc.x * myRealSize.width / [self frame].size.width;
        loc.y = loc.y * myRealSize.height / [self frame].size.height;
    }
    
    myRequester->MouseMovedAt(loc.x, loc.y);
}


////////////////////////////////////////////////////////
-(void)scrollWheel:(NSEvent *)theEvent
{
    if (myRequester == 0) return;
    
    NSPoint loc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    
    // Don't forget to change to SFML coord system.
    float h = [self frame].size.height;
    loc.y = h - loc.y;
    
    // Recompute the mouse pos if required.
    if (!NSEqualSizes(myRealSize, NSZeroSize)) {
        loc.x = loc.x * myRealSize.width / [self frame].size.width;
        loc.y = loc.y * myRealSize.height / [self frame].size.height;
    }
    
    myRequester->MouseWheelScrolledAt([theEvent deltaY], loc.x, loc.y);
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
    if (myRequester == 0) return;
    
    NSPoint loc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    
    // Don't forget to change to SFML coord system.
    float h = [self frame].size.height;
    loc.y = h - loc.y;
    
    // Recompute the mouse pos if required.
    if (!NSEqualSizes(myRealSize, NSZeroSize)) {
        loc.x = loc.x * myRealSize.width / [self frame].size.width;
        loc.y = loc.y * myRealSize.height / [self frame].size.height;
    }
    
    myRequester->MouseDownAt(sf::Mouse::Right, loc.x, loc.y);
}


////////////////////////////////////////////////////////
-(void)rightMouseUp:(NSEvent *)theEvent
{
    if (myRequester == 0) return;
    
    NSPoint loc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    
    // Don't forget to change to SFML coord system.
    float h = [self frame].size.height;
    loc.y = h - loc.y;
    
    // Recompute the mouse pos if required.
    if (!NSEqualSizes(myRealSize, NSZeroSize)) {
        loc.x = loc.x * myRealSize.width / [self frame].size.width;
        loc.y = loc.y * myRealSize.height / [self frame].size.height;
    }
    
    myRequester->MouseUpAt(sf::Mouse::Right, loc.x, loc.y);
}


////////////////////////////////////////////////////////
-(void)otherMouseDown:(NSEvent *)theEvent
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
    float h = [self frame].size.height;
    loc.y = h - loc.y;
    
    // Recompute the mouse pos if required.
    if (!NSEqualSizes(myRealSize, NSZeroSize)) {
        loc.x = loc.x * myRealSize.width / [self frame].size.width;
        loc.y = loc.y * myRealSize.height / [self frame].size.height;
    }
    
    myRequester->MouseDownAt(button, loc.x, loc.y);
}


////////////////////////////////////////////////////////
-(void)otherMouseUp:(NSEvent *)theEvent
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
    float h = [self frame].size.height;
    loc.y = h - loc.y;
    
    // Recompute the mouse pos if required.
    if (!NSEqualSizes(myRealSize, NSZeroSize)) {
        loc.x = loc.x * myRealSize.width / [self frame].size.width;
        loc.y = loc.y * myRealSize.height / [self frame].size.height;
    }
    
    myRequester->MouseUpAt(button, loc.x, loc.y);
}


////////////////////////////////////////////////////////
-(void)rightMouseDragged:(NSEvent *)theEvent
{
    if (myRequester == 0) return;
    
    // If the event is not useful.
    if (!myMouseIsIn) return;
    
    NSPoint loc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    
    // Don't forget to change to SFML coord system.
    float h = [self frame].size.height;
    loc.y = h - loc.y;
    
    // Recompute the mouse pos if required.
    if (!NSEqualSizes(myRealSize, NSZeroSize)) {
        loc.x = loc.x * myRealSize.width / [self frame].size.width;
        loc.y = loc.y * myRealSize.height / [self frame].size.height;
    }
    
    myRequester->MouseMovedAt(loc.x, loc.y);
}


////////////////////////////////////////////////////////
-(void)mouseDragged:(NSEvent *)theEvent
{
    if (myRequester == 0) return;
    
    // If the event is not useful.
    if (!myMouseIsIn) return;
    
    NSPoint loc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    
    // Don't forget to change to SFML coord system.
    float h = [self frame].size.height;
    loc.y = h - loc.y;
    
    // Recompute the mouse pos if required.
    if (!NSEqualSizes(myRealSize, NSZeroSize)) {
        loc.x = loc.x * myRealSize.width / [self frame].size.width;
        loc.y = loc.y * myRealSize.height / [self frame].size.height;
    }
    
    myRequester->MouseMovedAt(loc.x, loc.y);
}


////////////////////////////////////////////////////////
-(void)otherMouseDragged:(NSEvent *)theEvent
{
    if (myRequester == 0) return;
    
    // If the event is not useful.
    if (!myMouseIsIn) return;
    
    NSPoint loc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    
    // Don't forget to change to SFML coord system.
    float h = [self frame].size.height;
    loc.y = h - loc.y;
    
    // Recompute the mouse pos if required.
    if (!NSEqualSizes(myRealSize, NSZeroSize)) {
        loc.x = loc.x * myRealSize.width / [self frame].size.width;
        loc.y = loc.y * myRealSize.height / [self frame].size.height;
    }
    
    myRequester->MouseMovedAt(loc.x, loc.y);
}


#pragma mark
#pragma mark Key-event methods


////////////////////////////////////////////////////////
-(void)keyDown:(NSEvent *)theEvent
{
    if (myRequester == 0) return;
    
    if (myUseKeyRepeat || ![theEvent isARepeat]) {
        sf::Event::KeyEvent key = [SFOpenGLView convertNSKeyEventToSFMLEvent:theEvent];
        
        if (key.Code != sf::Key::Count) { // The key is recognized.
            myRequester->KeyDown(key);
        }
    }

    if (myUseKeyRepeat || ![theEvent isARepeat]) {
        // Let's see if its a valid text.
        NSText* text = [[self window] fieldEditor:YES forObject:self];
        [text interpretKeyEvents:[NSArray arrayWithObject:theEvent]];
        
        NSString* string = [text string];
        if ([string length] > 0) {
            // It's a valid TextEntered event.
            myRequester->TextEntered([string characterAtIndex:0]);
            
            [text setString:@""];
        }
    }
}


////////////////////////////////////////////////////////
-(void)keyUp:(NSEvent *)theEvent
{
    if (myRequester == 0) return;
    
    sf::Event::KeyEvent key = [SFOpenGLView convertNSKeyEventToSFMLEvent:theEvent];
    
    if (key.Code != sf::Key::Count) { // The key is recognized.
        myRequester->KeyUp(key);
    }
}


////////////////////////////////////////////////////////
-(void)flagsChanged:(NSEvent *)theEvent
{
    if (myRequester == 0) return;
    
    NSUInteger modifiers = [theEvent modifierFlags];
    
    // Setup a potential event key.
    sf::Event::KeyEvent key;
    key.Code    = sf::Key::Count;
    key.Alt     = modifiers & NSAlternateKeyMask;
    key.Control = modifiers & NSControlKeyMask;
    key.Shift   = modifiers & NSShiftKeyMask;
    
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
                
                key.Code = sf::Key::LShift;
                myRequester->KeyUp(key);
            }
            
            if (!myRightShiftWasDown) {
                // right shift pressed
                
                key.Code = sf::Key::RShift;
                myRequester->KeyDown(key);
            }
        }
        
        // Only left shift is down ?
        if (shift == NSLeftShiftKeyMask) {
            
            leftShiftIsDown = YES;
            
            if (myRightShiftWasDown) {
                // right shift released
                rightShiftIsDown = NO;
                
                key.Code = sf::Key::RShift;
                myRequester->KeyUp(key);
            }
            
            if (!myLeftShiftWasDown) {
                // left shift pressed
                
                key.Code = sf::Key::LShift;
                myRequester->KeyDown(key);
            }
        }
        
        // Or are they both down ?
        if (shift == (NSRightShiftKeyMask | NSLeftShiftKeyMask)) {
            
            rightShiftIsDown = YES;
            leftShiftIsDown = YES;
            
            if (!myRightShiftWasDown) {
                // right shift pressed
                
                key.Code = sf::Key::RShift;
                myRequester->KeyDown(key);
            }
            
            if (!myLeftShiftWasDown) {
                // left shift pressed
                
                key.Code = sf::Key::LShift;
                myRequester->KeyDown(key);
            }
        }
    } else { // No shift key down.
        
        rightShiftIsDown = NO;
        leftShiftIsDown  = NO;
        
        if (myRightShiftWasDown) {
            // right shift released
            
            key.Code = sf::Key::RShift;
            myRequester->KeyUp(key);
        }
        
        if (myLeftShiftWasDown) {
            // left shift released
            
            key.Code = sf::Key::LShift;
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
                
                key.Code = sf::Key::LSystem;
                myRequester->KeyUp(key);
            }
            
            if (!myRightCommandWasDown) {
                // right command pressed
                
                key.Code = sf::Key::RSystem;
                myRequester->KeyDown(key);
            }
        }
        
        // Only left Command is down ?
        if (command == NSLeftCommandKeyMask) {
            
            leftCommandIsDown = YES;
            
            if (myRightCommandWasDown) {
                // right command released
                rightCommandIsDown = NO;
                
                key.Code = sf::Key::RSystem;
                myRequester->KeyUp(key);
            }
            
            if (!myLeftCommandWasDown) {
                // left command pressed
                
                key.Code = sf::Key::LSystem;
                myRequester->KeyDown(key);
            }
        }
        
        // Or are they both down ?
        if (command == (NSRightCommandKeyMask | NSLeftCommandKeyMask)) {
            
            rightCommandIsDown = YES;
            leftCommandIsDown = YES;
            
            if (!myRightCommandWasDown) {
                // right command pressed
                
                key.Code = sf::Key::RSystem;
                myRequester->KeyDown(key);
            }
            
            if (!myLeftCommandWasDown) {
                // left command pressed
                
                key.Code = sf::Key::LSystem;
                myRequester->KeyDown(key);
            }
        }
    } else { // No Command key down.
        
        rightCommandIsDown = NO;
        leftCommandIsDown  = NO;
        
        if (myRightCommandWasDown) {
            // right command released
            
            key.Code = sf::Key::RSystem;
            myRequester->KeyUp(key);
        }
        
        if (myLeftCommandWasDown) {
            // left command released
            
            key.Code = sf::Key::LSystem;
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
                
                key.Code = sf::Key::LAlt;
                myRequester->KeyUp(key);
            }
            
            if (!myRightAlternateWasDown) {
                // right alt pressed
                
                key.Code = sf::Key::RAlt;
                myRequester->KeyDown(key);
            }
        }
        
        // Only left Alternate is down ?
        if (alternate == NSLeftAlternateKeyMask) {
            
            leftAlternateIsDown = YES;
            
            if (myRightAlternateWasDown) {
                // right alt released
                rightAlternateIsDown = NO;
                
                key.Code = sf::Key::RAlt;
                myRequester->KeyUp(key);
            }
            
            if (!myLeftAlternateWasDown) {
                // left alt pressed
                
                key.Code = sf::Key::LAlt;
                myRequester->KeyDown(key);
            }
        }
        
        // Or are they both down ?
        if (alternate == (NSRightAlternateKeyMask | NSLeftAlternateKeyMask)) {
            
            rightAlternateIsDown = YES;
            leftAlternateIsDown = YES;
            
            if (!myRightAlternateWasDown) {
                // right alt pressed
                
                key.Code = sf::Key::RAlt;
                myRequester->KeyDown(key);
            }
            
            if (!myLeftAlternateWasDown) {
                // left alt pressed
                
                key.Code = sf::Key::LAlt;
                myRequester->KeyDown(key);
            }
        }
    } else { // No Alternate key down.
        
        rightAlternateIsDown = NO;
        leftAlternateIsDown  = NO;
        
        if (myRightAlternateWasDown) {
            // right alt released
            
            key.Code = sf::Key::RAlt;
            myRequester->KeyUp(key);
        }
        
        if (myLeftAlternateWasDown) {
            // left alt released
            
            key.Code = sf::Key::LAlt;
            myRequester->KeyUp(key);
        }
    }
    
    // Control keys.
    if (modifiers & NSControlKeyMask) {
        // Currently only the left control key will be used in SFML (see note above).
        
        controlIsDown = YES;
        
        if (!myControlWasDown) {
            // ctrl pressed
            
            key.Code = sf::Key::LControl;
            myRequester->KeyDown(key);
        }
    } else { // No control key down.
        controlIsDown = NO;
        
        if (myControlWasDown) {
            // ctrl released
            
            key.Code = sf::Key::LControl;
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
    
    // Key code.
    key.Code = sf::Key::Count;
    // First we look if the key down is from a list of caracter that depend on keyboard localization.
    NSString* string = [anEvent charactersIgnoringModifiers];
    if ([string length] > 0) {
        key.Code = LocalizedKeys([string characterAtIndex:0]);
    }
    
    // The key is not a localized one, the other keys.
    if (key.Code == sf::Key::Count) {
        key.Code = NonLocalizedKeys([anEvent keyCode]);
    }
    
#ifdef SFML_DEBUG // We don't want to bother the final customer with anoying messages.
    if (key.Code == sf::Key::Count) { // The key is unknown.
        sf::Err()
        << "This is an unknow key. Should not happen (?). Keycode is 0x"
        << std::hex
        << [anEvent keyCode]
        << "."
        << std::endl;
    }
#endif
    
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


////////////////////////////////////////////////////////
sf::Key::Code LocalizedKeys(unichar ch)
{
    switch (ch) {
        case 'a':
        case 'A':                   return sf::Key::A;
        
        case 'b':
        case 'B':                   return sf::Key::B;
        
        case 'c':
        case 'C':                   return sf::Key::C;
        
        case 'd':
        case 'D':                   return sf::Key::D;
        
        case 'e':
        case 'E':                   return sf::Key::E;
        
        case 'f':
        case 'F':                   return sf::Key::F;
        
        case 'g':
        case 'G':                   return sf::Key::G;
        
        case 'h':
        case 'H':                   return sf::Key::H;
        
        case 'i':
        case 'I':                   return sf::Key::I;
        
        case 'j':
        case 'J':                   return sf::Key::J;
        
        case 'k':
        case 'K':                   return sf::Key::K;
        
        case 'l':
        case 'L':                   return sf::Key::L;
        
        case 'm':
        case 'M':                   return sf::Key::M;
        
        case 'n':
        case 'N':                   return sf::Key::N;
        
        case 'o':
        case 'O':                   return sf::Key::O;
        
        case 'p':
        case 'P':                   return sf::Key::P;
        
        case 'q':
        case 'Q':                   return sf::Key::Q;
        
        case 'r':
        case 'R':                   return sf::Key::R;
        
        case 's':
        case 'S':                   return sf::Key::S;
        
        case 't':
        case 'T':                   return sf::Key::T;
        
        case 'u':
        case 'U':                   return sf::Key::U;
        
        case 'v':
        case 'V':                   return sf::Key::V;
        
        case 'w':
        case 'W':                   return sf::Key::W;
        
        case 'x':
        case 'X':                   return sf::Key::X;
        
        case 'y':
        case 'Y':                   return sf::Key::Y;
        
        case 'z':
        case 'Z':                   return sf::Key::Z;
        
        // The kew is not 'localized'.
        default:                    return sf::Key::Count;
    }
}


////////////////////////////////////////////////////////
sf::Key::Code NonLocalizedKeys(unsigned short keycode)
{
    // (Some) 0x code based on http://forums.macrumors.com/showthread.php?t=780577
    // Some sf::Key are present twice.
    switch (keycode) {
        // These cases should not be used but anyway...
        case 0x00:                      return sf::Key::A;
        case 0x0b:                      return sf::Key::B;
        case 0x08:                      return sf::Key::C;
        case 0x02:                      return sf::Key::D;
        case 0x0e:                      return sf::Key::E;
        case 0x03:                      return sf::Key::F;
        case 0x05:                      return sf::Key::G;
        case 0x04:                      return sf::Key::H;
        case 0x22:                      return sf::Key::I;
        case 0x26:                      return sf::Key::J;
        case 0x28:                      return sf::Key::K;
        case 0x25:                      return sf::Key::L;
        case 0x2e:                      return sf::Key::M;
        case 0x2d:                      return sf::Key::N;
        case 0x1f:                      return sf::Key::O;
        case 0x23:                      return sf::Key::P;
        case 0x0c:                      return sf::Key::Q;
        case 0x0f:                      return sf::Key::R;
        case 0x01:                      return sf::Key::S;
        case 0x11:                      return sf::Key::T;
        case 0x20:                      return sf::Key::U;
        case 0x09:                      return sf::Key::V;
        case 0x0d:                      return sf::Key::W;
        case 0x07:                      return sf::Key::X;
        case 0x10:                      return sf::Key::Y;
        case 0x06:                      return sf::Key::Z;
            
        // These cases should not be used but anyway...
        case 0x1d:                      return sf::Key::Num0;
        case 0x12:                      return sf::Key::Num1;
        case 0x13:                      return sf::Key::Num2;
        case 0x14:                      return sf::Key::Num3;
        case 0x15:                      return sf::Key::Num4;
        case 0x17:                      return sf::Key::Num5;
        case 0x16:                      return sf::Key::Num6;
        case 0x1a:                      return sf::Key::Num7;
        case 0x1c:                      return sf::Key::Num8;
        case 0x19:                      return sf::Key::Num9;
        
        case 0x35:                      return sf::Key::Escape;
        
        // Modifier keys : never happen with keyDown/keyUp methods (?)
        case 0x3b:                      return sf::Key::LControl;
        case 0x38:                      return sf::Key::LShift;
        case 0x3a:                      return sf::Key::LAlt;
        case 0x37:                      return sf::Key::LSystem;
        case 0x3e:                      return sf::Key::RControl;
        case 0x3c:                      return sf::Key::RShift;
        case 0x3d:                      return sf::Key::RAlt;
        case 0x36:                      return sf::Key::RSystem;
        
        case NSMenuFunctionKey:         return sf::Key::Menu;
        
        case 0x21:                      return sf::Key::LBracket;
        case 0x1e:                      return sf::Key::RBracket;
        case 0x29:                      return sf::Key::SemiColon;
        case 0x2b:                      return sf::Key::Comma;
        case 0x2f:                      return sf::Key::Period;
        case 0x27:                      return sf::Key::Quote;
        case 0x2c:                      return sf::Key::Slash;
        case 0x2a:                      return sf::Key::BackSlash;

#warning sf::Key::Tilde might be in conflict with some other key.
        // 0x0a is for "Non-US Backslash" according to HID Calibrator,
        // a sample provided by Apple.
        case 0x0a:                      return sf::Key::Tilde;

        case 0x18:                      return sf::Key::Equal;
        case 0x32:                      return sf::Key::Dash;
        case 0x31:                      return sf::Key::Space;
        case 0x24:                      return sf::Key::Return;
        case 0x33:                      return sf::Key::Back;
        case 0x30:                      return sf::Key::Tab;
        
        // Duplicates (see next §).
        case 0x74:                      return sf::Key::PageUp;
        case 0x79:                      return sf::Key::PageDown;
        case 0x77:                      return sf::Key::End;
        case 0x73:                      return sf::Key::Home;
        
        case NSPageUpFunctionKey:       return sf::Key::PageUp;
        case NSPageDownFunctionKey:     return sf::Key::PageDown;
        case NSEndFunctionKey:          return sf::Key::End;
        case NSHomeFunctionKey:         return sf::Key::Home;
            
        case NSInsertFunctionKey:       return sf::Key::Insert;
        case NSDeleteFunctionKey:       return sf::Key::Delete;
        
        case 0x45:                      return sf::Key::Add;
        case 0x4e:                      return sf::Key::Subtract;
        case 0x43:                      return sf::Key::Multiply;
        case 0x4b:                      return sf::Key::Divide;
        
        // Duplicates (see next §).
        case 0x7b:                      return sf::Key::Left;
        case 0x7c:                      return sf::Key::Right;
        case 0x7e:                      return sf::Key::Up;
        case 0x7d:                      return sf::Key::Down;
        
        case NSLeftArrowFunctionKey:    return sf::Key::Left;
        case NSRightArrowFunctionKey:   return sf::Key::Right;
        case NSUpArrowFunctionKey:      return sf::Key::Up;
        case NSDownArrowFunctionKey:    return sf::Key::Down;
        
        case 0x52:                      return sf::Key::Numpad0;
        case 0x53:                      return sf::Key::Numpad1;
        case 0x54:                      return sf::Key::Numpad2;
        case 0x55:                      return sf::Key::Numpad3;
        case 0x56:                      return sf::Key::Numpad4;
        case 0x57:                      return sf::Key::Numpad5;
        case 0x58:                      return sf::Key::Numpad6;
        case 0x59:                      return sf::Key::Numpad7;
        case 0x5b:                      return sf::Key::Numpad8;
        case 0x5c:                      return sf::Key::Numpad9;
        
        // Duplicates (see next §).
        case 0x7a:                      return sf::Key::F1;
        case 0x78:                      return sf::Key::F2;
        case 0x63:                      return sf::Key::F3;
        case 0x76:                      return sf::Key::F4;
        case 0x60:                      return sf::Key::F5;
        case 0x61:                      return sf::Key::F6;
        case 0x62:                      return sf::Key::F7;
        case 0x64:                      return sf::Key::F8;
        case 0x65:                      return sf::Key::F9;
        case 0x6d:                      return sf::Key::F10;
        case 0x67:                      return sf::Key::F11;
        case 0x6f:                      return sf::Key::F12;
        case 0x69:                      return sf::Key::F13;
        case 0x6b:                      return sf::Key::F14;
        case 0x71:                      return sf::Key::F15;
        
        case NSF1FunctionKey:           return sf::Key::F1;
        case NSF2FunctionKey:           return sf::Key::F2;
        case NSF3FunctionKey:           return sf::Key::F3;
        case NSF4FunctionKey:           return sf::Key::F4;
        case NSF5FunctionKey:           return sf::Key::F5;
        case NSF6FunctionKey:           return sf::Key::F6;
        case NSF7FunctionKey:           return sf::Key::F7;
        case NSF8FunctionKey:           return sf::Key::F8;
        case NSF9FunctionKey:           return sf::Key::F9;
        case NSF10FunctionKey:          return sf::Key::F10;
        case NSF11FunctionKey:          return sf::Key::F11;
        case NSF12FunctionKey:          return sf::Key::F12;
        case NSF13FunctionKey:          return sf::Key::F13;
        case NSF14FunctionKey:          return sf::Key::F14;
        case NSF15FunctionKey:          return sf::Key::F15;
        
        case NSPauseFunctionKey:        return sf::Key::Pause;
        
#warning keycode 0x1b is not bound to any key.
        // This key is ' on CH-FR, ) on FR and - on US layouts.
        
        // An unknown key.
        default:                        return sf::Key::Count;
    }
}

