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
#include <SFML/Window/WindowHandle.hpp>
#include <SFML/Config.hpp> // for sf::Uint8

#import <AppKit/AppKit.h>

namespace sf {
    namespace priv {
        class WindowImplCocoa;
    }
}

////////////////////////////////////////////////////////////
/// This protocol defines the interface of the delegate of 
/// the window implementation.
///
/// We don't create an interface here because Obj-C doesn't allow
/// multiple inheritance (SFViewController and SFWindowController
/// don't have the same parent classes). Unfortunately this means
/// we have to duplicate some code.
///
/// Everything is done via a class that implement this protocol.
/// There are two of these classes : 
///
/// SFViewController and SFWindowController
///
/// The requester is a WindowImplCocoa. It's used to send back
/// event via these functions :
///
/// windowClosed, windowResized, windowLostFocus, windowGainedFocus
///
/// mouseDownAt, mouseUpAt, mouseMovedAt, mouseWheelScrolledAt,
/// mouseMovedIn, mouseMovedOut
///
/// keyDown, keyUp, textEntered
///
/// Note : Joysticks are not bound to a view or window 
/// thus they're not managed by a class implementing this protocol.
///
////////////////////////////////////////////////////////////
@protocol WindowImplDelegateProtocol

////////////////////////////////////////////////////////////
/// Set the WindowImpl who requested this delegate
/// (This would be a ctor in C++ or Java where we can prohibit the
/// construction of an object.)
///
////////////////////////////////////////////////////////////
-(void)setRequesterTo:(sf::priv::WindowImplCocoa *)requester;

////////////////////////////////////////////////////////////
/// Return the main view or window.
///
////////////////////////////////////////////////////////////
-(sf::WindowHandle)getSystemHandle;

////////////////////////////////////////////////////////////
/// Hide or show the mouse cursor.
///
////////////////////////////////////////////////////////////
-(void)hideMouseCursor;
-(void)showMouseCursor;

////////////////////////////////////////////////////////////
/// Get window's position.
///
////////////////////////////////////////////////////////////
-(NSPoint)position;

////////////////////////////////////////////////////////////
/// Move the window (not the view if we handle not a window) (SFML Coordinates).
///
////////////////////////////////////////////////////////////
-(void)setWindowPositionToX:(int)x Y:(int)y;

////////////////////////////////////////////////////////////
/// Get window's size.
///
////////////////////////////////////////////////////////////
-(NSSize)size;

////////////////////////////////////////////////////////////
/// Resize the window/view.
///
////////////////////////////////////////////////////////////
-(void)resizeTo:(unsigned int)width by:(unsigned int)height;

////////////////////////////////////////////////////////////
/// Set the title (does nothing if we manage a view).
///
////////////////////////////////////////////////////////////
-(void)changeTitle:(NSString *)title;

////////////////////////////////////////////////////////////
/// Hide or show the window (does nothing if we manage a view).
///
////////////////////////////////////////////////////////////
-(void)hideWindow;
-(void)showWindow;

////////////////////////////////////////////////////////////
/// Close the window (does nothing if we manage a view).
///
////////////////////////////////////////////////////////////
-(void)closeWindow;

////////////////////////////////////////////////////////////
/// Enable or disable key repeat.
///
////////////////////////////////////////////////////////////
-(void)enableKeyRepeat;
-(void)disableKeyRepeat;

////////////////////////////////////////////////////////////
/// Set an icon to the application.
///
////////////////////////////////////////////////////////////
-(void)setIconTo:(unsigned int)width by:(unsigned int)height with:(sf::Uint8 const*)pixels;

////////////////////////////////////////////////////////////
/// Fetch new event
///
////////////////////////////////////////////////////////////
-(void)processEvent;

////////////////////////////////////////////////////////////
/// Apply a given context to an OpenGL view.
///
////////////////////////////////////////////////////////////
-(void)applyContext:(NSOpenGLContext *)context;

@end
