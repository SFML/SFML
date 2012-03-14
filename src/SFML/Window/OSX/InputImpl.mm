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
#include <SFML/Window/OSX/InputImpl.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/System/Err.hpp>
#include <SFML/Window/OSX/HIDInputManager.hpp>

#import <AppKit/AppKit.h>
#import <SFML/Window/OSX/SFOpenGLView.h>

////////////////////////////////////////////////////////////
/// In order to keep track of the keyboard's state and mouse buttons' state
/// we use the HID manager. Mouse position is handled differently.
///
/// NB : we probably could use 
/// NSEvent +addGlobalMonitorForEventsMatchingMask:handler: for mouse only.
///
////////////////////////////////////////////////////////////

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Extract the dedicated SFOpenGLView from the SFML window
/// 
/// \param window a SFML window
/// \return nil if something went wrong or a SFOpenGLView*.
///
////////////////////////////////////////////////////////////    
SFOpenGLView* getSFOpenGLViewFromSFMLWindow(const Window& window)
{
    id nsHandle = (id)window.getSystemHandle();
    
    // Get our SFOpenGLView from ...
    SFOpenGLView* view = nil;
    
    if ([nsHandle isKindOfClass:[NSWindow class]]) {
        // If system handle is a window then from its content view.
        view = [nsHandle contentView];
        
        // Subview doesn't match ?
        if (![view isKindOfClass:[SFOpenGLView class]]) {
            sf::err() << "The content view is not a valid SFOpenGLView" 
                      << std::endl;
            view = nil;
        }
        
    } else if ([nsHandle isKindOfClass:[NSView class]]) {
        // If system handle is a view then from a subview of kind SFOpenGLView.
        NSArray* subviews = [nsHandle subviews];
        for (NSView* subview in subviews) {
            if ([subview isKindOfClass:[SFOpenGLView class]]) {
                view = (SFOpenGLView *)subview;
                break;
            }
        }
        
        // No matching subview ?
        if (view == nil) {
            sf::err() << "Cannot find a valid SFOpenGLView subview." << std::endl;

        }
        
    } else {
        if (nsHandle != 0) {
            sf::err() << "The system handle is neither a <NSWindow*> nor <NSView*>"
                      << "object. This shouldn't happen."
                      << std::endl;
        } else {
            // This probably means the SFML window was previously closed.
        }

    }
    
    return view;
}

////////////////////////////////////////////////////////////
bool InputImpl::isKeyPressed(Keyboard::Key key)
{
    return HIDInputManager::getInstance().isKeyPressed(key);
}


////////////////////////////////////////////////////////////
bool InputImpl::isMouseButtonPressed(Mouse::Button button)
{
    return HIDInputManager::getInstance().isMouseButtonPressed(button);
}


////////////////////////////////////////////////////////////
Vector2i InputImpl::getMousePosition()
{
    // Reverse Y axis to match SFML coord.
    NSPoint pos = [NSEvent mouseLocation];
    pos.y = sf::VideoMode::getDesktopMode().height - pos.y;

    return Vector2i(pos.x, pos.y);
}


////////////////////////////////////////////////////////////
Vector2i InputImpl::getMousePosition(const Window& relativeTo)
{
    SFOpenGLView* view = getSFOpenGLViewFromSFMLWindow(relativeTo);
    
    // No view ?
    if (view == nil) {
        return Vector2i();
    }
    
    // Use -cursorPositionFromEvent: with nil.
    NSPoint pos = [view cursorPositionFromEvent:nil];
    
    return Vector2i(pos.x, pos.y);
}


////////////////////////////////////////////////////////////
void InputImpl::setMousePosition(const Vector2i& position)
{
    // Here we don't need to reverse the coordinates.
    CGPoint pos = CGPointMake(position.x, position.y);
    
    // Place the cursor.
    CGEventRef event = CGEventCreateMouseEvent(NULL, 
                                               kCGEventMouseMoved, 
                                               pos, 
                                               /*we don't care about this : */0);
    CGEventPost(kCGHIDEventTap, event);
    CFRelease(event);
    // This is a workaround to deprecated CGSetLocalEventsSuppressionInterval.
}


////////////////////////////////////////////////////////////
void InputImpl::setMousePosition(const Vector2i& position, const Window& relativeTo)
{
    SFOpenGLView* view = getSFOpenGLViewFromSFMLWindow(relativeTo);
    
    // No view ?
    if (view == nil) {
        return;
    }
    
    // Use -setCursorPositionToX:Y:.
    [view setCursorPositionToX:position.x Y:position.y];
}

} // namespace priv

} // namespace sf
