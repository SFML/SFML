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
#import <SFML/Window/OSX/SFViewController.h>
#import <SFML/Window/OSX/SFOpenGLView.h>
#include <SFML/System/Err.hpp>
#include <SFML/Window/OSX/WindowImplCocoa.hpp>

@implementation SFViewController


////////////////////////////////////////////////////////
-(id)initWithView:(NSView *)view
{
    if ((self = [super init])) {
        myRequester = 0;
        
        // Retain the view for our own use.
        myView = [view retain];
        
        if (myView == nil) {
            
            sf::Err() 
            << "No view was given to initWithWindow:."
            << std::endl;
            
            return self;
        }
        
        // Create the view.
        NSRect frame = [myView frame];
        frame.origin.x = 0;
        frame.origin.y = 0;
        myOGLView = [[SFOpenGLView alloc] initWithFrame:frame];
        
        if (myOGLView == nil) {
            
            sf::Err()
            << "Could not create an instance of NSOpenGLView "
            << "in (SFViewController -initWithView:)."
            << std::endl;
            
            return self;
        }
        
        // Set the (OGL) view to the view as its "content" view.
        [myView addSubview:myOGLView];
    }
    
    return self;
}


////////////////////////////////////////////////////////
-(void)dealloc
{
    [self closeWindow];
    
    [myView release];
    [myOGLView release];
    
    [super dealloc];
}


////////////////////////////////////////////////////////
-(void)setRequesterTo:(sf::priv::WindowImplCocoa *)requester
{
    // Forward to the view.
    [myOGLView setRequesterTo:requester];
    myRequester = requester;
}


////////////////////////////////////////////////////////
-(sf::WindowHandle)getSystemHandle
{
    return myView;
}


////////////////////////////////////////////////////////
-(void)changeTitle:(NSString *)title
{
    sf::Err() << "Cannot change the title of the view." << std::endl;
}


////////////////////////////////////////////////////////
-(void)enableKeyRepeat
{
    [myOGLView enableKeyRepeat];
}


////////////////////////////////////////////////////////
-(void)disableKeyRepeat
{
    [myOGLView disableKeyRepeat];
}


////////////////////////////////////////////////////////
-(void)hideMouseCursor
{
    [NSCursor hide];
}


////////////////////////////////////////////////////////
-(void)showMouseCursor
{
    [NSCursor unhide];
}


////////////////////////////////////////////////////////
-(void)hideWindow
{
    [myView setHidden:YES];
}


////////////////////////////////////////////////////////
-(void)showWindow
{
    [myView setHidden:NO];
}


////////////////////////////////////////////////////////
-(void)closeWindow
{
    sf::Err() << "Cannot close the view." << std::endl;
    [self setRequesterTo:0];
}


////////////////////////////////////////////////////////
-(void)setCursorPositionToX:(unsigned int)x Y:(unsigned int)y
{
    if (myRequester == 0) return;
    
    // Create a SFML event.
    myRequester->MouseMovedAt(x, y);
    
    // Flip for SFML window coordinate system
    y = NSHeight([[myView window] frame]) - y;
    
    // Adjust for view reference instead of window
    y -= NSHeight([[myView window] frame]) - NSHeight([myOGLView frame]);
    
    // Convert to screen coordinates
    NSPoint screenCoord = [[myView window] convertBaseToScreen:NSMakePoint(x, y)];
    
    // Flip screen coodinates
    float const screenHeight = NSHeight([[[myView window] screen] frame]);
    screenCoord.y = screenHeight - screenCoord.y;
    
    CGDirectDisplayID screenNumber = (CGDirectDisplayID)[[[[[myView window] screen] deviceDescription] valueForKey:@"NSScreenNumber"] intValue];
    
    // Place the cursor.
    CGDisplayMoveCursorToPoint(screenNumber, CGPointMake(screenCoord.x, screenCoord.y));
    /*
     CGDisplayMoveCursorToPoint -- Discussion :
     
     No events are generated as a result of this move. 
     Points that lie outside the desktop are clipped to the desktop.
     */
}


////////////////////////////////////////////////////////.
-(void)setWindowPositionToX:(unsigned int)x Y:(unsigned int)y
{
    sf::Err() << "Cannot move the view." << std::endl;
}


////////////////////////////////////////////////////////
-(void)resizeTo:(unsigned int)width by:(unsigned int)height
{
    NSRect frame = NSMakeRect([myView frame].origin.x,
                              [myView frame].origin.y,
                              width,
                              height);
    
    [myView setFrame:frame];
}


////////////////////////////////////////////////////////
-(void)setIconTo:(unsigned int)width
              by:(unsigned int)height 
            with:(sf::Uint8 const *)pixels
{
    sf::Err() << "Cannot set an icon to the view." << std::endl;
}


////////////////////////////////////////////////////////
-(void)processEvent
{
    sf::Err() << "Cannot process event from the view." << std::endl;
}


////////////////////////////////////////////////////////
-(void)applyContext:(NSOpenGLContext *)context
{
    [myOGLView setOpenGLContext:context];
    [context setView:myOGLView];
}


@end
