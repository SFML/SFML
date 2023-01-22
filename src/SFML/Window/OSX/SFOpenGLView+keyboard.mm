////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Marco Antognini (antognini.marco@gmail.com),
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
#include <SFML/Window/OSX/HIDInputManager.hpp> // For localizedKeys and nonLocalizedKeys
#import <SFML/Window/OSX/SFKeyboardModifiersHelper.h>
#import <SFML/Window/OSX/SFOpenGLView+keyboard_priv.h>
#include <SFML/Window/OSX/WindowImplCocoa.hpp>

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

////////////////////////////////////////////////////////////
/// In this file, we implement keyboard handling for SFOpenGLView
///
////////////////////////////////////////////////////////////


@implementation SFOpenGLView (keyboard)


////////////////////////////////////////////////////////
- (BOOL)acceptsFirstResponder
{
    // Accepts key event.
    return YES;
}


////////////////////////////////////////////////////////
- (BOOL)canBecomeKeyView
{
    // Accepts key event.
    return YES;
}


////////////////////////////////////////////////////////
- (void)enableKeyRepeat
{
    m_useKeyRepeat = YES;
}


////////////////////////////////////////////////////////
- (void)disableKeyRepeat
{
    m_useKeyRepeat = NO;
}


////////////////////////////////////////////////////////
- (void)keyDown:(NSEvent*)theEvent
{
    // Transmit to non-SFML responder
    [[self nextResponder] keyDown:theEvent];

    if (m_requester == nil)
        return;

    // Handle key down event
    if (m_useKeyRepeat || ![theEvent isARepeat])
    {
        sf::Event::KeyEvent key = [SFOpenGLView convertNSKeyEventToSFMLEvent:theEvent];

        if ((key.code != sf::Keyboard::Unknown) || (key.scancode != sf::Keyboard::Scan::Unknown))
            m_requester->keyDown(key);
    }


    // Handle text entered event:
    // Ignore event if we don't want repeated keystrokes
    if (m_useKeyRepeat || ![theEvent isARepeat])
    {
        // Ignore escape key and other non text keycode (See NSEvent.h)
        // because they produce a sound alert.
        if ([SFOpenGLView isValidTextUnicode:theEvent])
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
- (void)sfKeyUp:(NSEvent*)theEvent
{
    // For some mystic reasons, key released events don't work the same way
    // as key pressed events... We somewhat hijack the event chain of response
    // in -[SFApplication sendEvent:] and resume this chain with the next
    // responder.
    // This is workaround to make sure key released events are fired in
    // fullscreen window too.

    // Transmit to non-SFML responder
    [[self nextResponder] keyUp:theEvent];

    if (m_requester == nil)
        return;

    sf::Event::KeyEvent key = [SFOpenGLView convertNSKeyEventToSFMLEvent:theEvent];

    if ((key.code != sf::Keyboard::Unknown) || (key.scancode != sf::Keyboard::Scan::Unknown))
        m_requester->keyUp(key);
}


////////////////////////////////////////////////////////
- (void)flagsChanged:(NSEvent*)theEvent
{
    // Transmit to non-SFML responder
    [[self nextResponder] flagsChanged:theEvent];

    if (m_requester == nil)
        return;

    NSUInteger modifiers = [theEvent modifierFlags];
    handleModifiersChanged(modifiers, *m_requester);
}


////////////////////////////////////////////////////////
+ (sf::Event::KeyEvent)convertNSKeyEventToSFMLEvent:(NSEvent*)event
{
    // The scancode always depends on the hardware keyboard, not some OS setting.
    sf::Keyboard::Scancode code = sf::priv::HIDInputManager::nonLocalizedKey([event keyCode]);

    // Get the corresponding key under the current keyboard layout.
    sf::Keyboard::Key key = sf::Keyboard::localize(code);

    return keyEventWithModifiers([event modifierFlags], key, code);
}


////////////////////////////////////////////////////////
+ (BOOL)isValidTextUnicode:(NSEvent*)event
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

@end
