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
#include <SFML/Window/OSX/WindowImplCocoa.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowHandle.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/System/Err.hpp>
#include <ApplicationServices/ApplicationServices.h>

#import <SFML/Window/OSX/SFWindowController.h>
#import <SFML/Window/OSX/SFApplication.h>
#import <SFML/Window/OSX/SFOpenGLView.h>
#import <SFML/Window/OSX/SFWindow.h>

////////////////////////////////////////////////////////////
/// SFWindowController class : Privates Methods Declaration
///
////////////////////////////////////////////////////////////
@interface SFWindowController ()

////////////////////////////////////////////////////////////
/// Retrieves the screen height.
/// 
////////////////////////////////////////////////////////////
-(float)screenHeight;

////////////////////////////////////////////////////////////
/// Retrives the title bar height.
/// 
////////////////////////////////////////////////////////////
-(float)titlebarHeight;

@end

@implementation SFWindowController

#pragma mark
#pragma mark SFWindowController's methods

////////////////////////////////////////////////////////
-(id)initWithWindow:(NSWindow *)window
{
    if ((self = [super init])) {
        myRequester = 0;
        
        // Retain the window for our own use.
        myWindow = [window retain];
        
        if (myWindow == nil) {
            
            sf::Err() 
            << "No window was given to initWithWindow:."
            << std::endl;
            
            return self;
        }
        
        // Create the view.
        myOGLView = [[SFOpenGLView alloc] initWithFrame:[[myWindow contentView] frame]];
        
        if (myOGLView == nil) {
            
            sf::Err()
            << "Could not create an instance of NSOpenGLView "
            << "in (SFWindowController -initWithMode:andStyle:)."
            << std::endl;
            
            return self;
        }
        
        // Set the view to the window as its content view.
        [myWindow setContentView:myOGLView];
    }
    
    return self;
}


////////////////////////////////////////////////////////
-(id)initWithMode:(sf::VideoMode const &)mode andStyle:(unsigned long)style
{
    // If we are not on the main thread we stop here and advice the user.
    if ([NSThread currentThread] != [NSThread mainThread]) {
        /*
         * See http://lists.apple.com/archives/cocoa-dev/2011/Feb/msg00460.html
         * for more information.
         */
        sf::Err()
        << "Cannot create a window from a worker thread. (OS X limitation)"
        << std::endl;
        
        return nil;
    }

    if ((self = [super init])) {
        myRequester = 0;
        
        // Create our window size.
        NSRect rect = NSZeroRect;
        if (style & sf::Style::Fullscreen && mode != sf::VideoMode::GetDesktopMode()) {
            // We use desktop mode to size the window
            // but we set the back buffer size to 'mode' in applyContext method.
            
            myFullscreenMode = mode;
            
            sf::VideoMode dm = sf::VideoMode::GetDesktopMode();
            rect = NSMakeRect(0, 0, dm.Width, dm.Height);
            
        } else { // no fullscreen requested.
            rect = NSMakeRect(0, 0, mode.Width, mode.Height);
        }
        
        // Convert the SFML window style to Cocoa window style.
        unsigned int nsStyle = NSBorderlessWindowMask;
        if (!(style & sf::Style::Fullscreen)) { // if fullscrean we keep our NSBorderlessWindowMask.
            
            if (style & sf::Style::Titlebar) nsStyle |= NSTitledWindowMask | NSMiniaturizableWindowMask;
            
            if (style & sf::Style::Resize)   nsStyle |= NSResizableWindowMask;
            
            if (style & sf::Style::Close)    nsStyle |= NSClosableWindowMask;
            
        }
        
        // Create the window.
        myWindow = [[SFWindow alloc] initWithContentRect:rect
                                               styleMask:nsStyle 
                                                 backing:NSBackingStoreBuffered 
                                                   defer:NO];
        /*
         "YES" produces some "invalid drawable".
         See http://www.cocoabuilder.com/archive/cocoa/152482-nsviews-and-nsopenglcontext-invalid-drawable-error.html
         
         [...]
         As best as I can figure, this is happening because the NSWindow (and
         hence my view) are not visible onscreen yet, and the system doesn't like that.
         [...]
         */
        
        if (myWindow == nil) {
            sf::Err() 
            << "Could not create an instance of NSWindow "
            << "in (SFWindowController -initWithMode:andStyle:)."
            << std::endl;
            
            return self;
        }
        
        // Apply special feature for fullscreen window.
        if (style & sf::Style::Fullscreen) {
            // We place the window above everything else.
            [myWindow setOpaque:YES];
            [myWindow setHidesOnDeactivate:YES];
            [myWindow setLevel:NSMainMenuWindowLevel+1];
            
            // And hide the menu bar
            [NSMenu setMenuBarVisible:NO];
            
            /* ---------------------------
             * | Note for future version |
             * ---------------------------
             *
             * starting with OS 10.5 NSView provides
             * a new method -enterFullScreenMode:withOptions: 
             * which could be a good alternative.
             */
        }

        // Center the window to be cool =)
        [myWindow center];
        
        // Create the view.
        myOGLView = [[SFOpenGLView alloc] initWithFrame:[[myWindow contentView] frame]];
        
        if (myOGLView == nil) {
            sf::Err()
            << "Could not create an instance of NSOpenGLView "
            << "in (SFWindowController -initWithMode:andStyle:)."
            << std::endl;
            
            return self;
        }
        
        // If a fullscreen window was requested...
        if (style & sf::Style::Fullscreen && mode != sf::VideoMode::GetDesktopMode()) {
            /// ... we set the "read size" of the view (that is the back buffer size).
            [myOGLView setRealSize:NSMakeSize(myFullscreenMode.Width, myFullscreenMode.Height)];
        }
        
        // Set the view to the window as its content view.
        [myWindow setContentView:myOGLView];
        
        // Register for event.
        [myWindow setDelegate:self];
        [myWindow setAcceptsMouseMovedEvents:YES];
        [myWindow setIgnoresMouseEvents:NO];
        
        // And some other things...
        [myWindow setAutodisplay:YES];
        [myWindow setReleasedWhenClosed:NO];
    } // if super init ok
    
    return self;
}

////////////////////////////////////////////////////////
-(void)dealloc
{
    [self closeWindow];
    
    [myWindow release];
    [myOGLView release];
    
    [super dealloc];
}


#pragma mark
#pragma mark WindowImplDelegateProtocol's methods


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
    return myWindow;
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
-(void)setCursorPositionToX:(unsigned int)x Y:(unsigned int)y
{
    if (myRequester == 0) return;
    
    // Create a SFML event.
    myRequester->MouseMovedAt(x, y);
    
    // Flip for SFML window coordinate system
    y = NSHeight([myWindow frame]) - y;
    
    // Adjust for view reference instead of window
    y -= NSHeight([myWindow frame]) - NSHeight([myOGLView frame]);
    
    // Convert to screen coordinates
    NSPoint screenCoord = [myWindow convertBaseToScreen:NSMakePoint(x, y)];
    
    // Flip screen coodinates
    float const screenHeight = NSHeight([[myWindow screen] frame]);
    screenCoord.y = screenHeight - screenCoord.y;
    
    CGDirectDisplayID screenNumber = (CGDirectDisplayID)[[[[myWindow screen] deviceDescription] valueForKey:@"NSScreenNumber"] intValue];
    
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
    NSPoint point = NSMakePoint(x, y);
    
    // Flip for SFML window coordinate system.
    point.y = [self screenHeight] - point.y;
    
    // Place the window.
    [myWindow setFrameTopLeftPoint:point];
}


////////////////////////////////////////////////////////
-(void)resizeTo:(unsigned int)width by:(unsigned int)height
{
    // Add titlebar height.
    NSRect frame = NSMakeRect([myWindow frame].origin.x,
                              [myWindow frame].origin.y,
                              width,
                              height + [self titlebarHeight]);
    
    [myWindow setFrame:frame display:YES];
}


////////////////////////////////////////////////////////
-(void)changeTitle:(NSString *)title
{
    [myWindow setTitle:title];
}


////////////////////////////////////////////////////////
-(void)hideWindow
{
    [myWindow orderOut:nil];
}


////////////////////////////////////////////////////////
-(void)showWindow
{
    [myWindow makeKeyAndOrderFront:nil];
}


////////////////////////////////////////////////////////
-(void)closeWindow
{
    [myWindow close];
    [myWindow setDelegate:nil];
    [self setRequesterTo:0];
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
-(void)setIconTo:(unsigned int)width
              by:(unsigned int)height 
            with:(sf::Uint8 const *)pixels
{
    // Create an empty image representation.
    NSBitmapImageRep* bitmap = 
    [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:0 // if 0 : only allocate memory
                                            pixelsWide:width
                                            pixelsHigh:height
                                         bitsPerSample:8 // The number of bits used to specify 
                                                         // one pixel in a single component of the data.
                                       samplesPerPixel:4 // 3 if no alpha, 4 with it
                                              hasAlpha:YES
                                              isPlanar:NO   // I don't know what it is but it works
                                        colorSpaceName:NSCalibratedRGBColorSpace
                                           bytesPerRow:0    // 0 == determine automatically
                                          bitsPerPixel:0];  // 0 == determine automatically
    
    // Load data pixels.
#if MAC_OS_X_VERSION_MAX_ALLOWED < 1050 // We may need to define NSUInteger.
#define NSUInteger unsigned int
#endif
    for (unsigned int y = 0; y < height; ++y) { 
        for (unsigned int x = 0; x < width; ++x, pixels+=4) {
            NSUInteger pixel[4] = { pixels[0], pixels[1], pixels[2], pixels[3] };
            [bitmap setPixel:pixel
                         atX:x
                           y:y];
        }
    }
    
    // Create an image from the representation.
    NSImage* icon = [[NSImage alloc] initWithSize:NSMakeSize(width, height)];
    [icon addRepresentation:bitmap];
    
    // Set app icon.
    [[NSApplication sharedApplication] setApplicationIconImage:icon];
    
    // Free up.
    [icon release];
    [bitmap release];
}


////////////////////////////////////////////////////////
-(void)processEventWithBlockingMode:(BOOL)block
{
    // If we are not on the main thread we stop here and advice the user.
    if ([NSThread currentThread] != [NSThread mainThread]) {
        /*
         * See http://lists.apple.com/archives/cocoa-dev/2011/Feb/msg00460.html
         * for more information.
         */
        sf::Err()
        << "Cannot fetch event from a worker thread. (OS X limitation)"
        << std::endl;
        
        return;
    }
    
    // If we don't have a requester we don't fetch event.
    if (myRequester != 0) {
        [SFApplication processEventWithBlockingMode:block];
    }
}


////////////////////////////////////////////////////////
-(void)applyContext:(NSOpenGLContext *)context
{
    [myOGLView setOpenGLContext:context];
    [context setView:myOGLView];
    
    // If fullscreen was requested and the mode used to create the window
    // was not the desktop mode, we change the back buffer size of the
    // context.
    if (myFullscreenMode != sf::VideoMode()) {
        CGLContextObj cgcontext = (CGLContextObj)[context CGLContextObj];
        
        GLint dim[2] = {myFullscreenMode.Width, myFullscreenMode.Height};
        
        CGLSetParameter(cgcontext, kCGLCPSurfaceBackingSize, dim);
        CGLEnable(cgcontext, kCGLCESurfaceBackingSize);
    }
}


#pragma mark
#pragma mark NSWindowDelegate's methods


////////////////////////////////////////////////////////
-(BOOL)windowShouldClose:(id)sender
{
    if (myRequester == 0) return YES;
    
    myRequester->WindowClosed();
    return NO;
}


////////////////////////////////////////////////////////
-(void)windowDidBecomeKey:(NSNotification *)notification
{
    // Send event.
    if (myRequester == 0) return;
    
    myRequester->WindowGainedFocus();
}


////////////////////////////////////////////////////////
-(void)windowDidResignKey:(NSNotification *)notification
{
    // Send event.
    if (myRequester == 0) return;
    
    myRequester->WindowLostFocus();
}


#pragma mark
#pragma mark Other methods

////////////////////////////////////////////////////////
-(float)screenHeight
{
    NSDictionary *deviceDescription = [[myWindow screen] deviceDescription];
    NSNumber *screenNumber = [deviceDescription valueForKey:@"NSScreenNumber"];
    CGDirectDisplayID screenID = (CGDirectDisplayID)[screenNumber intValue];
    CGFloat height = CGDisplayPixelsHigh(screenID);
    return height;
}


////////////////////////////////////////////////////////
-(float)titlebarHeight
{
    return NSHeight([myWindow frame]) - NSHeight([[myWindow contentView] frame]);
}

@end
