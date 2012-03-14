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

////////////////////////////////////////////////////////////
/// \brief Spesialized NSOpenGLView
///
/// Handle event and send them back to the requester.
///
/// In order to send correct mouse coordonate to the requester when
/// the window is in fullscreen we use m_realSize to represent the
/// back buffer size (see SFWindowController). If 'm_realSize' is
/// bound to its default value we don't recompute the mouse position
/// and assume it's correct.
///
/// As I don't have the right control keycode I cannot implement left-right
/// recognition for this key. (See SFOpenGLView.mm for more info.)
///
////////////////////////////////////////////////////////////
@interface SFOpenGLView : NSOpenGLView {
    sf::priv::WindowImplCocoa*    m_requester;
    BOOL                          m_useKeyRepeat;
    NSTrackingRectTag             m_trackingTag;
    BOOL                          m_mouseIsIn;
    NSSize                        m_realSize;
    
    /// 'modifiers' state
    BOOL m_rightShiftWasDown;
    BOOL m_leftShiftWasDown;
    BOOL m_rightCommandWasDown;
    BOOL m_leftCommandWasDown;
    BOOL m_rightAlternateWasDown;
    BOOL m_leftAlternateWasDown;
    BOOL m_controlWasDown;
}

////////////////////////////////////////////////////////////
/// Create the SFML opengl view to fit the given area.
/// 
////////////////////////////////////////////////////////////
-(id)initWithFrame:(NSRect)frameRect;

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
/// Move the mouse cursor to (x,y) (SFML Coordinates).
/// 
////////////////////////////////////////////////////////////
-(void)setCursorPositionToX:(unsigned int)x Y:(unsigned int)y;

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
