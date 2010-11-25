

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
/// Everything is done via a class that implement this protocol.
/// There are two of these classes : 
///
/// SFViewController and SFWindowController
///
/// The requester is a WindowImplCocoa. It's used to send back
/// event via these functions :
///
/// WindowClosed, WindowResized, WindowLostFocus, WindowGainedFocus
///
/// MouseDownAt, MouseUpAt, MouseMovedAt, MouseWheelScrolledAt,
/// MouseMovedIn, MouseMovedOut
///
/// KeyDown, KeyUp, TextEntred
///
/// Note : Joystick are not bound to a view or window 
/// thus they're not managed by a class implementing this protocol.
////////////////////////////////////////////////////////////
@protocol WindowImplDelegateProtocol

/// Set the WindowImpl who requested this delegate
/// (This would be a ctor in C++ or Java where we can prohibit the
/// construction of an object.)
-(void)setRequesterTo:(sf::priv::WindowImplCocoa*)requester;

/// Return the main view or window.
-(sf::WindowHandle)getSystemHandle;

/// Hide or show the mouse cursor.
-(void)hideMouseCursor;
-(void)showMouseCursor;

/// Move the mouse cursor to (x,y) (SFML Coordinates).
-(void)setCursorPositionToX:(unsigned int)x Y:(unsigned int)y;

/// Move the window (not the view if we handle not a window) (SFML Coordinates).
-(void)setWindowPositionToX:(unsigned int)x Y:(unsigned int)y;

/// Resize the window/view.
-(void)resizeTo:(unsigned int)width by:(unsigned int)height;

/// Set the title (does nothing if we manage a view).
-(void)changeTitle:(NSString*)title;

/// Hide or show the window (does nothing if we manage a view).
-(void)hideWindow;
-(void)showWindow;

/// Close the window (does nothing if we manage a view).
-(void)closeWindow;

/// Enable or disable key repeat.
-(void)enableKeyRepeat;
-(void)disableKeyRepeat;

/// Set an icon to the application.
-(void)setIconTo:(unsigned int)width by:(unsigned int)height with:(sf::Uint8 const*)pixels;

/// Fetch new event
-(void)processEventWithBlockingMode:(BOOL)block;

/// Apply a given context to an OpenGL view.
-(void)applyContext:(NSOpenGLContext*)context;

@end
