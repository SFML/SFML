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

#import <SFML/Window/OSX/SFOpenGLView.h>
#import <SFML/Window/OSX/SFOpenGLView+mouse_priv.h>


////////////////////////////////////////////////////////////
/// In this file, we implement mouse handling for SFOpenGLView
///
////////////////////////////////////////////////////////////

@implementation SFOpenGLView (mouse)

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

    // Send event if needed.
    if (mouseWasIn && !m_mouseIsIn)
        m_requester->mouseMovedOut();
    else if (!mouseWasIn && m_mouseIsIn)
        m_requester->mouseMovedIn();
}



////////////////////////////////////////////////////////
-(void)mouseDown:(NSEvent*)theEvent
{
    [self handleMouseDown:theEvent];

    // Transmit to non-SFML responder
    [[self nextResponder] mouseDown:theEvent];
}


////////////////////////////////////////////////////////
-(void)rightMouseDown:(NSEvent*)theEvent
{
    [self handleMouseDown:theEvent];

    // Transmit to non-SFML responder
    [[self nextResponder] rightMouseDown:theEvent];
}


////////////////////////////////////////////////////////
-(void)otherMouseDown:(NSEvent*)theEvent
{
    [self handleMouseDown:theEvent];

    // Transmit to non-SFML responder
    [[self nextResponder] otherMouseDown:theEvent];
}


////////////////////////////////////////////////////////
-(void)handleMouseDown:(NSEvent*)theEvent
{
    sf::Mouse::Button button = [SFOpenGLView mouseButtonFromEvent:theEvent];

    if (m_requester != 0)
    {
        NSPoint loc = [self cursorPositionFromEvent:theEvent];

        if (button != sf::Mouse::ButtonCount)
            m_requester->mouseDownAt(button, loc.x, loc.y);
    }
}


////////////////////////////////////////////////////////
-(void)mouseUp:(NSEvent*)theEvent
{
    [self handleMouseUp:theEvent];

    // Transmit to non-SFML responder
    [[self nextResponder] mouseUp:theEvent];
}


////////////////////////////////////////////////////////
-(void)rightMouseUp:(NSEvent*)theEvent
{
    [self handleMouseUp:theEvent];

    // Transmit to non-SFML responder
    [[self nextResponder] rightMouseUp:theEvent];
}


////////////////////////////////////////////////////////
-(void)otherMouseUp:(NSEvent*)theEvent
{
    [self handleMouseUp:theEvent];

    // Transmit to non-SFML responder
    [[self nextResponder] otherMouseUp:theEvent];
}


////////////////////////////////////////////////////////////
-(void)handleMouseUp:(NSEvent*)theEvent
{
    sf::Mouse::Button button = [SFOpenGLView mouseButtonFromEvent:theEvent];

    if (m_requester != 0)
    {
        NSPoint loc = [self cursorPositionFromEvent:theEvent];

        if (button != sf::Mouse::ButtonCount)
            m_requester->mouseUpAt(button, loc.x, loc.y);
    }
}


////////////////////////////////////////////////////////
-(void)mouseMoved:(NSEvent*)theEvent
{
    [self handleMouseMove:theEvent];

    // Transmit to non-SFML responder
    [[self nextResponder] mouseMoved:theEvent];
}


////////////////////////////////////////////////////////
-(void)rightMouseDragged:(NSEvent*)theEvent
{
    [self handleMouseMove:theEvent];

    // Transmit to non-SFML responder
    [[self nextResponder] rightMouseDragged:theEvent];
}


////////////////////////////////////////////////////////
-(void)mouseDragged:(NSEvent*)theEvent
{
    [self handleMouseMove:theEvent];

    // Transmit to non-SFML responder
    [[self nextResponder] mouseDragged:theEvent];
}


////////////////////////////////////////////////////////
-(void)otherMouseDragged:(NSEvent*)theEvent
{
    [self handleMouseMove:theEvent];

    // Transmit to non-SFML responder
    [[self nextResponder] otherMouseUp:theEvent];
}


////////////////////////////////////////////////////////
-(void)handleMouseMove:(NSEvent*)theEvent
{
    if (m_requester != 0)
    {
        NSPoint loc = [self cursorPositionFromEvent:theEvent];

        // Make sure the point is inside the view.
        // (mouseEntered: and mouseExited: are not immediately called
        //  when the mouse is dragged. That would be too easy!)
        [self updateMouseState];
        if (m_mouseIsIn)
            m_requester->mouseMovedAt(loc.x, loc.y);
    }
}


////////////////////////////////////////////////////////
-(void)scrollWheel:(NSEvent*)theEvent
{
    if (m_requester != 0)
    {
        NSPoint loc = [self cursorPositionFromEvent:theEvent];
        m_requester->mouseWheelScrolledAt([theEvent deltaX], [theEvent deltaY], loc.x, loc.y);
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
-(NSPoint)cursorPositionFromEvent:(NSEvent*)eventOrNil
{
    NSPoint loc;
    // If no event given then get current mouse pos.
    if (eventOrNil == nil)
    {
        NSPoint rawPos = [[self window] mouseLocationOutsideOfEventStream];
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
+(sf::Mouse::Button)mouseButtonFromEvent:(NSEvent*)event
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


@end
