////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2015 Marco Antognini (antognini.marco@gmail.com),
//                         Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/System/Clock.hpp>
#include <SFML/System/Err.hpp>
#include <SFML/System/Time.hpp>
#include <algorithm>

#import <SFML/Window/OSX/SFKeyboardModifiersHelper.h>
#import <SFML/Window/OSX/SFOpenGLView.h>
#import <SFML/Window/OSX/SFSilentResponder.h>


////////////////////////////////////////////////////////////
/// \brief Check if the event represent some Unicode text
///
/// The event is assumed to be a key down event.
/// False is returned if the event is either escape or a non text Unicode.
///
/// \param event a key down event
///
/// \return true if event represents a Unicode character, false otherwise
///
////////////////////////////////////////////////////////////
BOOL isValidTextUnicode(NSEvent* event);



////////////////////////////////////////////////////////////
/// \brief Restart the timer handling last focus events
///
////////////////////////////////////////////////////////////
void resetTimeSinceLastLostFocus();

////////////////////////////////////////////////////////////
/// \brief Check if the «last focus» timer was reset recently
///
/// \return YES if the corresponding timer was reset less than 'duration' ago
///
////////////////////////////////////////////////////////////
BOOL isTimeSinceLastLostFocusLessThan(sf::Time duration);

////////////////////////////////////////////////////////////
/// \brief Restart the timer handling mouse down events outside the window
///
////////////////////////////////////////////////////////////
void resetTimeSinceLastMouseDownOutside();

////////////////////////////////////////////////////////////
/// \brief Check if the «mouse down outside the window» timer was reset recently
///
/// \return YES if the corresponding timer was reset less than 'duration' ago
///
////////////////////////////////////////////////////////////
BOOL isTimeSinceLastMouseDownOutsideLessThan(sf::Time duration);



////////////////////////////////////////////////////////////
/// SFOpenGLView class: Privates Methods Declaration
///
////////////////////////////////////////////////////////////
@interface SFOpenGLView ()

////////////////////////////////////////////////////////////
/// \brief Handle screen changed event
///
////////////////////////////////////////////////////////////
-(void)updateScaleFactor;

////////////////////////////////////////////////////////////
/// \brief Handle view resized event
///
////////////////////////////////////////////////////////////
-(void)viewDidEndLiveResize;

////////////////////////////////////////////////////////////
/// \brief Update the mouse state (in or out)
///
/// Fire an event if its state has changed.
///
////////////////////////////////////////////////////////////
-(void)updateMouseState;

////////////////////////////////////////////////////////////
/// \brief Check if the window has focus
///
/// \return YES when the window is key
///
////////////////////////////////////////////////////////////
-(BOOL)hasFocus;

////////////////////////////////////////////////////////////
/// \brief Callback for focus event
///
////////////////////////////////////////////////////////////
-(void)windowDidBecomeKey:(NSNotification*)notification;

////////////////////////////////////////////////////////////
/// \brief Callback for unfocus event
///
////////////////////////////////////////////////////////////
-(void)windowDidResignKey:(NSNotification*)notification;

////////////////////////////////////////////////////////////
/// \brief Callback for close event
///
////////////////////////////////////////////////////////////
-(void)windowWillClose:(NSNotification*)notification;

////////////////////////////////////////////////////////////
/// \brief Handle going in fullscreen mode
///
////////////////////////////////////////////////////////////
-(void)enterFullscreen;

////////////////////////////////////////////////////////////
/// \brief Handle exiting fullscreen mode
///
////////////////////////////////////////////////////////////
-(void)exitFullscreen;

////////////////////////////////////////////////////////////
/// \brief Check if the cursor is grabbed
///
/// The cursor is grabbed if the window is active (key) and
/// it is in fullscreen mode, or the user wants to grab it.
///
/// \return YES if the mouse is captured
///
////////////////////////////////////////////////////////////
-(BOOL)isCursorGrabbed;

////////////////////////////////////////////////////////////
/// \brief Move the cursor to a new position
///
/// \param destination new position in SFML coord system
///
////////////////////////////////////////////////////////////
-(void)moveCursor:(NSPoint)destination;

////////////////////////////////////////////////////////////
/// \brief Move the cursor into the view
///
/// \return the (new) cursor position in SFML coord system
///
////////////////////////////////////////////////////////////
-(NSPoint)projectCursorIntoView;

////////////////////////////////////////////////////////////
/// \brief Compute the projection of a point into the view
///
/// Both input and output are in SFML coord system.
///
/// \param point a point to project
/// \return the corresponding projected point
///
////////////////////////////////////////////////////////////
-(NSPoint)projectPointIntoView:(NSPoint)point;

////////////////////////////////////////////////////////////
/// \brief Create or destroy the current monitor
///
/// To be called when losing or gaining focus.
///
////////////////////////////////////////////////////////////
-(void)updateMonitor;

////////////////////////////////////////////////////////////
/// \brief Callback for mouse down event outside the window
///
/// Used to restore focus when the cursor is grabbed but the
/// user click outside the window.
///
/// \param event a mouse down event outside the window
///
////////////////////////////////////////////////////////////
-(void)mouseDownOutsideApp:(NSEvent*)event;

////////////////////////////////////////////////////////////
/// \brief Make the app active and set our window as key
///
////////////////////////////////////////////////////////////
-(void)refocus;

////////////////////////////////////////////////////////////
/// \brief Get the display identifier on which the view is
///
/// \return the current display identifier
///
////////////////////////////////////////////////////////////
-(CGDirectDisplayID)displayId;

////////////////////////////////////////////////////////////
/// \brief Convert the NSEvent mouse button type to SFML type
///
/// \param event a mouse button event
///
/// \return Left, Right, ..., or ButtonCount if the button is unknown
///
////////////////////////////////////////////////////////////
-(sf::Mouse::Button)mouseButtonFromEvent:(NSEvent*)event;

////////////////////////////////////////////////////////////
/// \brief Convert a key down/up NSEvent into an SFML key event
///
/// The conversion is based on localizedKeys and nonLocalizedKeys functions.
///
/// \param event a key event
///
/// \return sf::Keyboard::Unknown as Code if the key is unknown
///
////////////////////////////////////////////////////////////
+(sf::Event::KeyEvent)convertNSKeyEventToSFMLEvent:(NSEvent*)event;

@end

@implementation SFOpenGLView

#pragma mark
#pragma mark SFOpenGLView's methods

////////////////////////////////////////////////////////
-(id)initWithFrame:(NSRect)frameRect
{
    return [self initWithFrame:frameRect fullscreen:NO];
}

////////////////////////////////////////////////////////
-(id)initWithFrame:(NSRect)frameRect fullscreen:(BOOL)isFullscreen
{
    if ((self = [super initWithFrame:frameRect]))
    {
        [self setRequesterTo:0];
        [self enableKeyRepeat];

        // Register for mouse move event
        m_mouseIsIn = [self isMouseInside];
        NSUInteger opts = (NSTrackingActiveAlways | NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved | NSTrackingEnabledDuringMouseDrag);
        m_trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds]
                                                      options:opts
                                                        owner:self
                                                     userInfo:nil];
        [self addTrackingArea:m_trackingArea];

        m_fullscreen = isFullscreen;
        m_scaleFactor = 1.0; // Default value; it will be updated in finishInit
        m_cursorGrabbed = NO;
        m_monitor = nil;
        m_willClose = NO;

        // Create a hidden text view for parsing key down event properly
        m_silentResponder = [[SFSilentResponder alloc] init];
        m_hiddenTextView = [[NSTextView alloc] initWithFrame:NSZeroRect];
        [m_hiddenTextView setNextResponder:m_silentResponder];

        // Request high resolution on high DPI displays
        [self setWantsBestResolutionOpenGLSurface:YES];

        // At that point, the view isn't attached to a window. We defer the rest of
        // the initialization process to later.
    }

    return self;
}


////////////////////////////////////////////////////////
-(void)finishInit
{
    // Register for window focus events
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(windowDidBecomeKey:)
                                                 name:NSWindowDidBecomeKeyNotification
                                               object:[self window]];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(windowDidResignKey:)
                                                 name:NSWindowDidResignKeyNotification
                                               object:[self window]];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(windowDidResignKey:)
                                                 name:NSWindowWillCloseNotification
                                               object:[self window]];

    // Register for changed screen and changed screen's profile events
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(updateScaleFactor)
                                                 name:NSWindowDidChangeScreenNotification
                                               object:[self window]];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(updateScaleFactor)
                                                 name:NSWindowDidChangeScreenProfileNotification
                                               object:[self window]];

    // Now that we have a window, set up correctly the scale factor
    [self updateScaleFactor];
}


////////////////////////////////////////////////////////
-(void)setCursorGrabbed:(BOOL)grabbed
{
    // Fullscreen window are not authorized to release the cursor
    if (m_fullscreen)
        return;

    m_cursorGrabbed = grabbed;
    [self updateMonitor];

    // This check if we have focus too!
    if ([self isCursorGrabbed])
    {
        NSPoint oldPos = [self cursorPositionFromEvent:nil];
        NSPoint newPos = [self projectCursorIntoView];

        // If the cursor is moved we notify the user the mouse went in
        if (!NSEqualPoints(oldPos, newPos) && (m_requester != 0))
            m_requester->mouseMovedIn();
    }
}


////////////////////////////////////////////////////////
-(void)setRequesterTo:(sf::priv::WindowImplCocoa*)requester
{
    m_requester = requester;
}


////////////////////////////////////////////////////////
-(NSPoint)convertPointToScreen:(NSPoint)point
{
    NSRect rect = NSZeroRect;
    rect.origin = point;
    rect = [[self window] convertRectToScreen:rect];
    return rect.origin;
}


////////////////////////////////////////////////////////
-(NSPoint)computeGlobalPositionOfRelativePoint:(NSPoint)point
{
    // Flip SFML coordinates to match window coordinates
    point.y = [self frame].size.height - point.y;

    // Get the position of (x, y) in the coordinate system of the window.
    point = [self convertPoint:point toView:self];
    point = [self convertPoint:point toView:nil]; // nil means window

    // Convert it to screen coordinates
    point = [self convertPointToScreen:point];

    // Flip screen coordinates to match CGDisplayMoveCursorToPoint referential.
    const float screenHeight = [[[self window] screen] frame].size.height;
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
-(CGFloat)displayScaleFactor
{
    return m_scaleFactor;
}


////////////////////////////////////////////////////////
-(void)updateScaleFactor
{
    NSWindow* window = [self window];
    NSScreen* screen = window ? [window screen] : [NSScreen mainScreen];
    CGFloat oldScaleFactor = m_scaleFactor;
    m_scaleFactor = [screen backingScaleFactor];

    // Send a resize event if the scaling factor changed
    if ((m_scaleFactor != oldScaleFactor) && (m_requester != 0)) {
        NSSize newSize = [self frame].size;
        m_requester->windowResized(newSize.width, newSize.height);
    }
}


////////////////////////////////////////////////////////
-(void)viewDidEndLiveResize
{
    // We use viewDidEndLiveResize to notify the user ONCE
    // only, when the resizing is finished.
    // In a perfect world we would like to notify the user
    // in live that the window is being resized. However,
    // it seems impossible to forward to the user
    // NSViewFrameDidChangeNotification before the resizing
    // is done. Several notifications are emitted but they
    // are all delivered after when the work is done.

    [super viewDidEndLiveResize];

    // Update mouse internal state.
    [self updateMouseState];

    // Update the OGL view to fit the new size.
    [self update];

    // Send an event
    if (m_requester == 0)
        return;

    // The new size
    NSSize newSize = [self frame].size;
    m_requester->windowResized(newSize.width, newSize.height);
}


////////////////////////////////////////////////////////
-(BOOL)isMouseInside
{
    NSPoint relativeToWindow = [[self window] mouseLocationOutsideOfEventStream];
    NSPoint relativeToView = [self convertPoint:relativeToWindow fromView:nil];

    return NSPointInRect(relativeToView, [self bounds]);
}


////////////////////////////////////////////////////////
-(void)updateMouseState
{
    BOOL mouseWasIn = m_mouseIsIn;
    m_mouseIsIn = [self isMouseInside];

    if (m_requester == 0)
        return;

    // Special case: when the cursor is grabbed,
    // it cannot leave and thus enter the view
    if ([self isCursorGrabbed])
        return;

    // Send an event if the state has changed
    if (mouseWasIn && !m_mouseIsIn)
        m_requester->mouseMovedOut();
    else if (!mouseWasIn && m_mouseIsIn)
        m_requester->mouseMovedIn();
}


////////////////////////////////////////////////////////
-(BOOL)hasFocus
{
    return [[self window] isKeyWindow];
}


////////////////////////////////////////////////////////
-(void)windowDidBecomeKey:(NSNotification*)notification
{
    (void)notification;

    if (m_requester)
        m_requester->windowGainedFocus();

    if (m_fullscreen)
        [self enterFullscreen];

    if ([self isCursorGrabbed])
        [self projectCursorIntoView];

    [self updateMonitor];
}


////////////////////////////////////////////////////////
-(void)windowDidResignKey:(NSNotification*)notification
{
    (void)notification;

    resetTimeSinceLastLostFocus();

    // Refocus the application when the user click
    // outside the window and the cursor is grabbed.
    // See -mouseDownOutsideApp: for more details.
    if (isTimeSinceLastMouseDownOutsideLessThan(sf::milliseconds(100)))
    {
        [self refocus];
        [self projectCursorIntoView];
        return; // ignore this notification
    }

    if (m_requester)
        m_requester->windowLostFocus();

    if (m_fullscreen)
        [self exitFullscreen];

    [self updateMonitor];
}


////////////////////////////////////////////////////////
-(void)windowWillClose:(NSNotification*)notification
{
    m_willClose = YES;

    // Simulate focus loss
    [self windowDidResignKey:notification];
}


////////////////////////////////////////////////////////
-(void)enterFullscreen
{
    // Remove the tracking area first,
    // just to be sure we don't add it twice!
    [self removeTrackingArea:m_trackingArea];
    [self addTrackingArea:m_trackingArea];

    // Fire an mouse entered event if needed
    if (!m_mouseIsIn && (m_requester != 0))
        m_requester->mouseMovedIn();

    // Update status
    m_mouseIsIn = YES;
}


////////////////////////////////////////////////////////
-(void)exitFullscreen
{
    [self removeTrackingArea:m_trackingArea];

    // Fire an mouse left event if needed
    if (m_mouseIsIn && (m_requester != 0))
        m_requester->mouseMovedOut();

    // Update status
    m_mouseIsIn = NO;
}


////////////////////////////////////////////////////////
-(BOOL)isCursorGrabbed
{
    return [self hasFocus] && (m_cursorGrabbed || m_fullscreen);
}


////////////////////////////////////////////////////////
-(void)moveCursor:(NSPoint)destination
{
    // Convert the point from SFML coord system to screen coord system
    NSPoint screenLocation = [self computeGlobalPositionOfRelativePoint:destination];

    // Move the cursor without generating any event
    CGAssociateMouseAndMouseCursorPosition(NO);
    CGDisplayMoveCursorToPoint([self displayId], NSPointToCGPoint(screenLocation));
    CGAssociateMouseAndMouseCursorPosition(YES);
}


////////////////////////////////////////////////////////
-(NSPoint)projectCursorIntoView
{
    NSPoint cursor = [self cursorPositionFromEvent:nil];
    cursor = [self projectPointIntoView:cursor];
    [self moveCursor:cursor];
    return cursor;
}


////////////////////////////////////////////////////////
-(NSPoint)projectPointIntoView:(NSPoint)point
{
    NSSize size = [self frame].size;
    point.x = std::min(std::max(CGFloat(1), point.x), size.width  - 1);
    point.y = std::min(std::max(CGFloat(1), point.y), size.height - 1);
    return point;
}


////////////////////////////////////////////////////////
-(void)updateMonitor
{
    // If the cursor is grabbed, create a monitor
    if (m_cursorGrabbed && m_monitor == nil)
    {
        NSEventMask mouseDownMask = NSLeftMouseDown | NSRightMouseDown | NSOtherMouseDown;
        m_monitor = [NSEvent addGlobalMonitorForEventsMatchingMask:mouseDownMask
                                                           handler:^(NSEvent* event)
        {
            [self mouseDownOutsideApp:event];
        }];
    }

    // If the cursor is not grabbed, remove the monitor
    if (!m_cursorGrabbed && m_monitor != nil)
    {
        [NSEvent removeMonitor:m_monitor];
        m_monitor = nil;
    }
}


////////////////////////////////////////////////////////
-(void)mouseDownOutsideApp:(NSEvent*)event
{
    resetTimeSinceLastMouseDownOutside();

    // Refocus the application when the user click
    // outside the window and the cursor is grabbed.
    // But only if it was a very recent lost focus
    // otherwise it means the user is actually using
    // another app and he doesn't want to be annoyed
    // by the SFML window popping in front of everything else.
    if (isTimeSinceLastLostFocusLessThan(sf::milliseconds(100)))
    {
        [self refocus];
        [self projectCursorIntoView];

        // Send a mouse down event
        if (m_requester)
        {
            NSPoint loc = [self cursorPositionFromEvent:nil];
            m_requester->mouseDownAt([self mouseButtonFromEvent:event], loc.x, loc.y);
        }
        // BUT NO CORRESPONDING MOUSE UP IS CAUGHT! :-/
    }
}


////////////////////////////////////////////////////////
-(void)refocus
{
    [NSApp activateIgnoringOtherApps:YES];
    [[self window] makeKeyAndOrderFront:nil];
}


////////////////////////////////////////////////////////
-(CGDirectDisplayID)displayId
{
    NSScreen* screen = [[self window] screen];
    NSNumber* displayId = [[screen deviceDescription] objectForKey:@"NSScreenNumber"];
    return [displayId intValue];
}


#pragma mark
#pragma mark Subclassing methods


////////////////////////////////////////////////////////
-(void)dealloc
{
    // Unregister for window focus events
    [[NSNotificationCenter defaultCenter] removeObserver:self];

    // Unregister
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
-(void)mouseDown:(NSEvent*)theEvent
{
    // Forward to...
    [self otherMouseDown:theEvent];

    // Transmit to non-SFML responder
    [[self nextResponder] mouseDown:theEvent];
}


////////////////////////////////////////////////////////
-(void)mouseUp:(NSEvent*)theEvent
{
    // Forward to...
    [self otherMouseUp:theEvent];

    // Transmit to non-SFML responder
    [[self nextResponder] mouseUp:theEvent];
}


////////////////////////////////////////////////////////
-(void)mouseMoved:(NSEvent*)theEvent
{
    // Forward to...
    [self otherMouseDragged:theEvent];

    // Transmit to non-SFML responder
    [[self nextResponder] mouseMoved:theEvent];
}


////////////////////////////////////////////////////////
-(void)scrollWheel:(NSEvent*)theEvent
{
    if (m_requester != 0)
    {
        NSPoint loc = [self cursorPositionFromEvent:theEvent];
        m_requester->mouseWheelScrolledAt([theEvent deltaY], loc.x, loc.y);
    }

    // Transmit to non-SFML responder
    [[self nextResponder] scrollWheel:theEvent];
}


////////////////////////////////////////////////////////
-(void)mouseEntered:(NSEvent*)theEvent
{
    (void)theEvent;
    [self updateMouseState];
}


////////////////////////////////////////////////////////
-(void)mouseExited:(NSEvent*)theEvent
{
    (void)theEvent;
    [self updateMouseState];
}


////////////////////////////////////////////////////////
-(void)rightMouseDown:(NSEvent*)theEvent
{
    // Forward to...
    [self otherMouseDown:theEvent];

    // Transmit to non-SFML responder
    [[self nextResponder] rightMouseDown:theEvent];
}


////////////////////////////////////////////////////////
-(void)rightMouseUp:(NSEvent*)theEvent
{
    // Forward to...
    [self otherMouseUp:theEvent];

    // Transmit to non-SFML responder
    [[self nextResponder] rightMouseUp:theEvent];
}


////////////////////////////////////////////////////////
-(void)otherMouseDown:(NSEvent*)theEvent
{
    sf::Mouse::Button button = [self mouseButtonFromEvent:theEvent];

    if (m_requester != 0)
    {
        NSPoint loc = [self cursorPositionFromEvent:theEvent];

        if (button != sf::Mouse::ButtonCount)
            m_requester->mouseDownAt(button, loc.x, loc.y);
    }

    // If the event is not forwarded by mouseDown or rightMouseDown...
    if ((button != sf::Mouse::Left) && (button != sf::Mouse::Right))
    {
        // ... transmit to non-SFML responder
        [[self nextResponder] otherMouseDown:theEvent];
    }
}


////////////////////////////////////////////////////////
-(void)otherMouseUp:(NSEvent*)theEvent
{
    sf::Mouse::Button button = [self mouseButtonFromEvent:theEvent];

    if (m_requester != 0)
    {
        NSPoint loc = [self cursorPositionFromEvent:theEvent];

        if (button != sf::Mouse::ButtonCount)
            m_requester->mouseUpAt(button, loc.x, loc.y);
    }

    // If the event is not forwarded by mouseUp or rightMouseUp...
    if ((button != sf::Mouse::Left) && (button != sf::Mouse::Right))
    {
        // ... transmit to non-SFML responder
        [[self nextResponder] otherMouseUp:theEvent];
    }
}


////////////////////////////////////////////////////////
-(void)rightMouseDragged:(NSEvent*)theEvent
{
    // Forward to...
    [self otherMouseDragged:theEvent];

    // Transmit to non-SFML responder
    [[self nextResponder] rightMouseDragged:theEvent];
}


////////////////////////////////////////////////////////
-(void)mouseDragged:(NSEvent*)theEvent
{
    // Forward to...
    [self otherMouseDragged:theEvent];

    // Transmit to non-SFML responder
    [[self nextResponder] mouseDragged:theEvent];
}


////////////////////////////////////////////////////////
-(void)otherMouseDragged:(NSEvent*)theEvent
{
    NSPoint loc = [self cursorPositionFromEvent:theEvent];

    // If the cursor is grabbed, we need to make sure the cursor doesn't
    // go outside the view.
    if ([self isCursorGrabbed])
        loc = [self projectCursorIntoView];

    // Make sure the point is inside the view.
    // (mouseEntered: and mouseExited: are not immediately called
    //  when the mouse is dragged. That would be too easy!)
    [self updateMouseState];
    if (m_mouseIsIn && (m_requester != 0))
        m_requester->mouseMovedAt(loc.x, loc.y);

    // If the event is not forwarded by mouseDragged or rightMouseDragged...
    sf::Mouse::Button button = [self mouseButtonFromEvent:theEvent];
    if ((button != sf::Mouse::Left) && (button != sf::Mouse::Right))
    {
        // ... transmit to non-SFML responder
        [[self nextResponder] otherMouseUp:theEvent];
    }
}


////////////////////////////////////////////////////////
-(NSPoint)cursorPositionFromEvent:(NSEvent*)eventOrNil
{
    NSPoint loc;
    // If no event given then get current mouse pos.
    if (eventOrNil == nil)
    {
        NSPoint rawPos = [[self window] mouseLocationOutsideOfEventStream];
        loc = [self convertPoint:rawPos fromView:nil];
    }
    else if ([self isCursorGrabbed])
    {
        // Special case when the mouse is grabbed:
        // we need to take into account the delta since the cursor
        // is dissociated from its position.
        NSPoint rawPos = [eventOrNil locationInWindow];
        rawPos.x += [eventOrNil deltaX];
        rawPos.y -= [eventOrNil deltaY];
        loc = [self convertPoint:rawPos fromView:nil];
    }
    else
    {
        loc = [self convertPoint:[eventOrNil locationInWindow] fromView:nil];
    }

    // Don't forget to change to SFML coord system.
    float h = [self frame].size.height;
    loc.y = h - loc.y;

    return loc;
}


////////////////////////////////////////////////////////
-(sf::Mouse::Button)mouseButtonFromEvent:(NSEvent*)event
{
    switch ([event buttonNumber])
    {
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
-(void)keyDown:(NSEvent*)theEvent
{
    // Transmit to non-SFML responder
    [[self nextResponder] keyDown:theEvent];

    if (m_requester == 0)
        return;

    // Handle key down event
    if (m_useKeyRepeat || ![theEvent isARepeat])
    {
        sf::Event::KeyEvent key = [SFOpenGLView convertNSKeyEventToSFMLEvent:theEvent];

        if (key.code != sf::Keyboard::Unknown) // The key is recognized.
            m_requester->keyDown(key);
    }


    // Handle text entered event:
    // Ignore event if we don't want repeated keystrokes
    if (m_useKeyRepeat || ![theEvent isARepeat])
    {
        // Ignore escape key and other non text keycode (See NSEvent.h)
        // because they produce a sound alert.
        if (isValidTextUnicode(theEvent))
        {
            // Send the event to the hidden text view for processing
            [m_hiddenTextView interpretKeyEvents:[NSArray arrayWithObject:theEvent]];
        }

        // Carefully handle backspace and delete..
        // Note: the event is intentionally sent to the hidden view
        //       even if we do something more specific below. This way
        //       key combination are correctly interpreted.

        unsigned short keycode = [theEvent keyCode];

        // Backspace
        if (keycode == 0x33)
        {
            // Send the correct Unicode value (i.e. 8) instead of 127 (which is 'delete')
            m_requester->textEntered(8);
        }

        // Delete
        else if ((keycode == 0x75) || (keycode == NSDeleteFunctionKey))
        {
            // Instead of the value 63272 we send 127.
            m_requester->textEntered(127);
        }

        // Otherwise, let's see what our hidden field has computed
        else
        {
            NSString* string = [m_hiddenTextView string];

            // Send each character to SFML event requester
            for (NSUInteger index = 0; index < [string length]; ++index)
                m_requester->textEntered([string characterAtIndex:index]);

            // Empty our hidden cache
            [m_hiddenTextView setString:@""];
        }
    }
}


////////////////////////////////////////////////////////
-(void)sfKeyUp:(NSEvent*)theEvent
{
    // For some mystic reasons, key released events don't work the same way
    // as key pressed events... We somewhat hijack the event chain of response
    // in -[SFApplication sendEvent:] and resume this chain with the next
    // responder.
    // This is workaround to make sure key released events are fired in
    // fullscreen window too.

    // Transmit to non-SFML responder
    [[self nextResponder] keyUp:theEvent];

    if (m_requester == 0)
        return;

    sf::Event::KeyEvent key = [SFOpenGLView convertNSKeyEventToSFMLEvent:theEvent];

    if (key.code != sf::Keyboard::Unknown) // The key is recognized.
        m_requester->keyUp(key);
}


////////////////////////////////////////////////////////
-(void)flagsChanged:(NSEvent*)theEvent
{
    // Transmit to non-SFML responder
    [[self nextResponder] flagsChanged:theEvent];

    if (m_requester == 0)
        return;

    NSUInteger modifiers = [theEvent modifierFlags];
    handleModifiersChanged(modifiers, *m_requester);
}


////////////////////////////////////////////////////////
+(sf::Event::KeyEvent)convertNSKeyEventToSFMLEvent:(NSEvent*)event
{
    // Key code
    sf::Keyboard::Key key = sf::Keyboard::Unknown;

    // First we look if the key down is from a list of characters
    // that depend on keyboard localization.
    NSString* string = [event charactersIgnoringModifiers];
    if ([string length] > 0)
        key = sf::priv::HIDInputManager::localizedKeys([string characterAtIndex:0]);

    // If the key is not a localized one, we try to find a corresponding code
    // through virtual key code.
    if (key == sf::Keyboard::Unknown)
        key = sf::priv::HIDInputManager::nonLocalizedKeys([event keyCode]);

//#ifdef SFML_DEBUG // Don't bother the final customers with annoying messages.
//    if (key.code == sf::Keyboard::Unknown) { // The key is unknown.
//        sf::err() << "This is an unknown key. Virtual key code is 0x"
//                  << std::hex
//                  << [event keyCode]
//                  << "."
//                  << std::endl;
//    }
//#endif

    return keyEventWithModifiers([event modifierFlags], key);
}

@end


#pragma mark - C-like functions

////////////////////////////////////////////////////////
BOOL isValidTextUnicode(NSEvent* event)
{
    if ([event keyCode] == 0x35) // Escape
    {
        return false;
    }
    else if ([[event characters] length] > 0)
    {
        unichar code = [[event characters] characterAtIndex:0];
        return ((code < 0xF700) || (code > 0xF8FF));
    }
    else
    {
        return true;
    }
}

////////////////////////////////////////////////////////
namespace
{
    // Data for resetTimeSinceLastLostFocus and isTimeSinceLastLostFocusGreaterThan
    sf::Clock timeSinceLastLostFocus;
    // Data for resetTimeSinceLastMouseDownOutside and isTimeSinceLastMouseDownOutsideGreaterThan
    sf::Clock timeSinceLastMouseDownOutside;
}


////////////////////////////////////////////////////////
void resetTimeSinceLastLostFocus()
{
    timeSinceLastLostFocus.restart();
}


////////////////////////////////////////////////////////
BOOL isTimeSinceLastLostFocusLessThan(sf::Time duration)
{
    return timeSinceLastLostFocus.getElapsedTime() < duration;
}


////////////////////////////////////////////////////////
void resetTimeSinceLastMouseDownOutside()
{
    timeSinceLastMouseDownOutside.restart();
}


////////////////////////////////////////////////////////
BOOL isTimeSinceLastMouseDownOutsideLessThan(sf::Time duration)
{
    return timeSinceLastMouseDownOutside.getElapsedTime() < duration;
}
