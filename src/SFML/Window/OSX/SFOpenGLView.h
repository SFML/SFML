////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Marco Antognini (antognini.marco@gmail.com),
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
#import <AppKit/AppKit.h>

namespace sf {
    namespace priv {
        class WindowImplCocoa;
    }
}

@class SFSilentResponder;

////////////////////////////////////////////////////////////
/// \brief Specialized NSOpenGLView
///
/// Handle event and send them back to the requester.
///
/// NSTrackingArea is used to keep track of mouse events. We also
/// need to be able to ignore mouse event when exiting fullscreen.
///
/// Modifiers keys (cmd, ctrl, alt, shift) are handled by this class
/// but the actual logic is done in SFKeyboardModifiersHelper.(h|mm).
///
/// For some (mystic) reason, a window can receive a
/// NSWindowDidBecomeKeyNotification after having received a
/// NSWindowWillCloseNotification. m_willClose is there to
/// make sure no invalid actions are taken after the window
/// is known to closing.
///
/// The mouse is trapped if the window is active (key) and:
///   + the window is fullscreen or
///   + the user requested it (sf::Window::setCursorGrabbed)
///
/// When the cursor is grabbed, we make sure it doesn't go out
/// of the view. This is more easy said than done, sadly. Cocoa
/// doesn't provide an all-in-one function for that. So we need
/// to track mouse move events (as usual) and project any move
/// that goes outside the view back in it.
/// CGAssociateMouseAndMouseCursorPosition and CGDisplayMoveCursorToPoint
/// are used to reposition the cursor when needed.
///
/// But we need one additional trick to prevent a fast mouse move
/// followed by a click to activate another window or application.
/// We need to track mouse down events in other applications
/// with +[NSEvent addGlobalMonitorForEventsMatchingMask:handler:]
/// and refocus the window if a undesirable click happened.
///
/// We need two timers to know when we should refocus or not the window.
/// The first one is reset when we get a NSWindowDidResignKeyNotification
/// notification. The second one is reset when a click outside the window
/// is detected. We need these two because the two events can
/// happen in any order. Then, when handling each of these two events
/// we check if the *other* timer was reset recently (pure magic number
/// involved!). In that case we refocus the app.
///
/// We need all that because we cannot drop an event that is not
/// sent to our specific application with Cocoa API.
///
/// The window will disappear for a brief moment (<< 1 sec) but I
/// haven't found any other way. Setting the window level high
/// enough (1) to be always above other window when it's active
/// (and restore the level when it's not the key window), doesn't
/// help for a few reasons: a) the window will lose the focus for
/// a brief moment too; b) there is no guaranteed order for mouse
/// down event and NSWindowDidResignKeyNotification notification;
/// c) thus we don't know if the user used cmd+tab to switch to
/// another app or if he inadvertently clicked outside the window;
/// c') hence we don't know when the window level should be restored
/// to its default value.
///
/// (1) with [window setLevel:CGWindowLevelForKey(kCGOverlayWindowLevelKey)];
///
/// An better alternative for games that need grabbing would be
/// to hide the cursor and use relative motion. This would
/// involve much less corner case for the implementation and thus
/// a much better user experience.
///
////////////////////////////////////////////////////////////
@interface SFOpenGLView : NSOpenGLView
{
    sf::priv::WindowImplCocoa*    m_requester;      ///< View's requester
    BOOL                          m_useKeyRepeat;   ///< Key repeat setting
    BOOL                          m_mouseIsIn;      ///< Mouse positional state
    NSTrackingArea*               m_trackingArea;   ///< Mouse tracking area
    BOOL                          m_fullscreen;     ///< Indicate whether the window is fullscreen or not
    CGFloat                       m_scaleFactor;    ///< Display scale factor (e.g. 1x for classic display, 2x for retina)
    BOOL                          m_cursorGrabbed;  ///< Is the mouse cursor trapped?
    id                            m_monitor;        ///< Event monitor used to catch mouseDown: (more details above)
    BOOL                          m_willClose;      ///< When set to YES, the window is about to close

    // Hidden text view used to convert key event to actual chars.
    // We use a silent responder to prevent sound alerts.
    SFSilentResponder*  m_silentResponder;
    NSTextView*         m_hiddenTextView;
}

////////////////////////////////////////////////////////////
/// \brief Create the SFML OpenGL view
///
/// NB: -initWithFrame: is also implemented to default isFullscreen to NO
/// in case SFOpenGLView is created with the standard message.
///
/// To finish the initialization -finishInit should be called too.
///
/// \param frameRect dimension of the view
/// \param isFullscreen fullscreen flag
///
/// \return an initialized view
///
////////////////////////////////////////////////////////////
-(id)initWithFrame:(NSRect)frameRect fullscreen:(BOOL)isFullscreen;

////////////////////////////////////////////////////////////
/// \brief Finish the creation of the SFML OpenGL view
///
/// This method should be called after the view was added to a window
///
////////////////////////////////////////////////////////////
-(void)finishInit;

////////////////////////////////////////////////////////////
/// Clips or releases the mouse cursor
///
/// Generate a MouseEntered event when it makes sense.
///
/// \param grabbed YES to grab, NO to release
///
////////////////////////////////////////////////////////////
-(void)setCursorGrabbed:(BOOL)grabbed;

////////////////////////////////////////////////////////////
/// \brief Apply the given requester to the view
///
/// \param requester new 'requester' of the view
///
////////////////////////////////////////////////////////////
-(void)setRequesterTo:(sf::priv::WindowImplCocoa*)requester;

////////////////////////////////////////////////////////////
/// \brief Compute the position in global coordinate
///
/// \param point a point in SFML coordinate
///
/// \return the global coordinates of the point
///
////////////////////////////////////////////////////////////
-(NSPoint)computeGlobalPositionOfRelativePoint:(NSPoint)point;

////////////////////////////////////////////////////////////
/// \brief Enable key repeat
///
////////////////////////////////////////////////////////////
-(void)enableKeyRepeat;

////////////////////////////////////////////////////////////
/// \brief Disable key repeat
///
////////////////////////////////////////////////////////////
-(void)disableKeyRepeat;

////////////////////////////////////////////////////////////
/// \brief Get the display scale factor
///
/// \return e.g. 1.0 for classic display, 2.0 for retina display
///
////////////////////////////////////////////////////////////
-(CGFloat)displayScaleFactor;

////////////////////////////////////////////////////////////
/// \brief Compute the position of the cursor
///
/// \param eventOrNil if nil the cursor position is the current one
///
/// \return the mouse position in SFML coord system
///
////////////////////////////////////////////////////////////
-(NSPoint)cursorPositionFromEvent:(NSEvent*)eventOrNil;

////////////////////////////////////////////////////////////
/// \brief Determine where the mouse is
///
/// \return true when the mouse is inside the OpenGL view, false otherwise
///
////////////////////////////////////////////////////////////
-(BOOL)isMouseInside;

@end
