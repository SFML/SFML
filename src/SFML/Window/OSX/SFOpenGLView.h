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
/// The SFWindowController should call -[SFOpenGLView exitFullscreen]
/// and -[SFOpenGLView enterFullscreen] when appropriate.
///
/// In order to send correct mouse coordonate to the requester when
/// the window is in fullscreen we use m_realSize to represent the
/// back buffer size (see SFWindowController). If 'm_realSize' is
/// bound to its default value we don't recompute the mouse position
/// and assume it's correct.
///
/// Modifiers keys (cmd, ctrl, alt, shift) are handled by this class
/// but the actual logic is done in SFKeyboardModifiersHelper.(h|mm).
///
////////////////////////////////////////////////////////////
@interface SFOpenGLView : NSOpenGLView {
    sf::priv::WindowImplCocoa*    m_requester;
    BOOL                          m_useKeyRepeat;
    BOOL                          m_mouseIsIn;
    NSTrackingArea*               m_trackingArea;
    NSSize                        m_realSize;

    // Hidden text view used to convert key event to actual chars.
    // We use a silent responder to prevent sound alerts.
    SFSilentResponder*  m_silentResponder;
    NSTextView*         m_hiddenTextView;
}

////////////////////////////////////////////////////////////
/// Create the SFML opengl view to fit the given area.
/// 
////////////////////////////////////////////////////////////
-(id)initWithFrame:(NSRect)frameRect;

////////////////////////////////////////////////////////////
/// Handle going in and out of fullscreen mode.
/// 
////////////////////////////////////////////////////////////
-(void)enterFullscreen;
-(void)exitFullscreen;

////////////////////////////////////////////////////////////
/// Apply the given resquester to the view.
/// 
////////////////////////////////////////////////////////////
-(void)setRequesterTo:(sf::priv::WindowImplCocoa *)requester;

////////////////////////////////////////////////////////////
/// Set the real size of view (it should be the back buffer size).
/// If not set, or set to its default value NSZeroSize, the view
/// won't recompute the mouse coordinates before sending them
/// to the requester.
/// 
////////////////////////////////////////////////////////////
-(void)setRealSize:(NSSize)newSize;

////////////////////////////////////////////////////////////
/// Compute the position in global coordinate
/// of the given point in SFML coordinate.
/// 
////////////////////////////////////////////////////////////
-(NSPoint)computeGlobalPositionOfRelativePoint:(NSPoint)point;

////////////////////////////////////////////////////////////
/// Adjust key repeat configuration.
/// 
////////////////////////////////////////////////////////////
-(void)enableKeyRepeat;
-(void)disableKeyRepeat;

////////////////////////////////////////////////////////////
/// Compute the position of the cursor.
/// 
////////////////////////////////////////////////////////////
-(NSPoint)cursorPositionFromEvent:(NSEvent *)eventOrNil;

@end
