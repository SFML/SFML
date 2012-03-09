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
#include <SFML/System/Err.hpp>

#import <SFML/Window/OSX/SFWindowController.h>
#import <SFML/Window/OSX/SFViewController.h>
#import <SFML/Window/OSX/cpp_objc_conversion.h>
#import <SFML/Window/OSX/AutoreleasePoolWrapper.h>

namespace sf
{
namespace priv
{

#pragma mark
#pragma mark WindowImplCocoa's ctor/dtor

////////////////////////////////////////////////////////////
WindowImplCocoa::WindowImplCocoa(WindowHandle handle)
: myShowCursor(true)
{
    // Ask for a pool.
    RetainPool();
    
    // Treat the handle as it real type
    id nsHandle = (id)handle;
    if ([nsHandle isKindOfClass:[NSWindow class]]) {
        
        // We have a window.
        myDelegate = [[SFWindowController alloc] initWithWindow:nsHandle];
    
    } else if ([nsHandle isKindOfClass:[NSView class]]) {
        
        // We have a view.
        myDelegate = [[SFViewController alloc] initWithView:nsHandle];
        
    } else {
        
        sf::Err()
            << "Cannot import this Window Handle because it is neither "
            << "a <NSWindow*> nor <NSView*> object "
            << "(or any of their subclasses). You gave a <" 
            << [[nsHandle className] UTF8String]
            << "> object."
            << std::endl;
        return;            
        
    }
     
    // NO :
    // [myDelegate setRequesterTo:this];
    // because we don't handle event.
}
    
    
////////////////////////////////////////////////////////////
WindowImplCocoa::WindowImplCocoa(VideoMode mode, 
                                 const std::string& title, 
                                 unsigned long style)
: myShowCursor(true)
{
    // Transform the app process.
    SetUpProcess();
    
    // Ask for a pool.
    RetainPool();
    
    myDelegate = [[SFWindowController alloc] initWithMode:mode andStyle:style];
    [myDelegate changeTitle:stringToNSString(title)];
    [myDelegate setRequesterTo:this];
}

    
////////////////////////////////////////////////////////////
WindowImplCocoa::~WindowImplCocoa()
{
    [myDelegate closeWindow];
    
    [myDelegate release];
    
    ReleasePool();
    
    DrainPool(); // Make sure everything was freed
    // This solve some issue when sf::Window::Create is called for the
    // second time (nothing was render until the function was called again)
}
    
    
////////////////////////////////////////////////////////////
void WindowImplCocoa::ApplyContext(NSOpenGLContextRef context) const
{
    [myDelegate applyContext:context];
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::SetUpProcess(void)
{
    static bool isTheProcessSetAsApplication = false;
    
    if (!isTheProcessSetAsApplication) {
        // Do it only once !
        isTheProcessSetAsApplication = true;
        
        // Set the process as a normal application so it can get focus.
        ProcessSerialNumber psn;
        if (!GetCurrentProcess(&psn)) {
            TransformProcessType(&psn, kProcessTransformToForegroundApplication);
            SetFrontProcess(&psn);
        }
        
        // Tell the application to stop bouncing in the Dock.
        [[NSApplication sharedApplication] finishLaunching];
        // NOTE : This last call won't harm anything even if SFML window was
        // created with an external handle.
    }
}


#pragma mark
#pragma mark WindowImplCocoa's window-event methods
    
    
////////////////////////////////////////////////////////////
void WindowImplCocoa::WindowClosed(void)
{            
    Event event;
    event.Type = Event::Closed;
    
    PushEvent(event);
}
    
    
////////////////////////////////////////////////////////////
void WindowImplCocoa::WindowResized(unsigned int width, unsigned int height)
{
    Event event;
    event.Type = Event::Resized;
    event.Size.Width  = width;
    event.Size.Height = height;
    
    PushEvent(event);
}

    
////////////////////////////////////////////////////////////
void WindowImplCocoa::WindowLostFocus(void)
{
    if (!myShowCursor) {
        [myDelegate showMouseCursor]; // Make sur the cursor is visible
    }
    
    Event event;
    event.Type = Event::LostFocus;
    
    PushEvent(event);
}

    
////////////////////////////////////////////////////////////
void WindowImplCocoa::WindowGainedFocus(void)
{
    if (!myShowCursor) {
        [myDelegate hideMouseCursor]; // Restore user's setting
    }
    
    Event event;
    event.Type = Event::GainedFocus;
    
    PushEvent(event);
}
    
#pragma mark
#pragma mark WindowImplCocoa's mouse-event methods
    
    
////////////////////////////////////////////////////////////
void WindowImplCocoa::MouseDownAt(Mouse::Button button, int x, int y)
{
    Event event;
    event.Type = Event::MouseButtonPressed;
    event.MouseButton.Button = button;
    event.MouseButton.X = x;
    event.MouseButton.Y = y;
    
    PushEvent(event);
}

    
////////////////////////////////////////////////////////////
void WindowImplCocoa::MouseUpAt(Mouse::Button button, int x, int y)
{
    Event event;
    event.Type = Event::MouseButtonReleased;
    event.MouseButton.Button = button;
    event.MouseButton.X = x;
    event.MouseButton.Y = y;
    
    PushEvent(event);
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::MouseMovedAt(int x, int y)
{
    Event event;
    event.Type = Event::MouseMoved;
    event.MouseMove.X = x;
    event.MouseMove.Y = y;
    
    PushEvent(event);
}

////////////////////////////////////////////////////////////
void WindowImplCocoa::MouseWheelScrolledAt(float delta, int x, int y)
{
    Event event;
    event.Type = Event::MouseWheelMoved;
    event.MouseWheel.Delta = delta;
    event.MouseWheel.X = x;
    event.MouseWheel.Y = y;
    
    PushEvent(event);
}

////////////////////////////////////////////////////////////
void WindowImplCocoa::MouseMovedIn(void)
{
    if (!myShowCursor) {
        [myDelegate hideMouseCursor]; // Restore user's setting
    }
    
    Event event;
    event.Type = Event::MouseEntered;
    
    PushEvent(event);
}
    
////////////////////////////////////////////////////////////
void WindowImplCocoa::MouseMovedOut(void)
{
    if (!myShowCursor) {
        [myDelegate showMouseCursor]; // Make sur the cursor is visible
    }
    
    Event event;
    event.Type = Event::MouseLeft;
    
    PushEvent(event);
}
    
    
#pragma mark
#pragma mark WindowImplCocoa's key-event methods
    
    
////////////////////////////////////////////////////////////
void WindowImplCocoa::KeyDown(Event::KeyEvent key)
{
    Event event;
    event.Type = Event::KeyPressed;
    event.Key = key;
    
    PushEvent(event);
}

    
////////////////////////////////////////////////////////////
void WindowImplCocoa::KeyUp(Event::KeyEvent key)
{
    Event event;
    event.Type = Event::KeyReleased;
    event.Key = key;
    
    PushEvent(event);
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::TextEntered(unichar charcode)
{
    Event event;
    event.Type = Event::TextEntered;
    event.Text.Unicode = charcode;
    
    PushEvent(event);
}

    
#pragma mark
#pragma mark WindowImplCocoa's event-related methods

////////////////////////////////////////////////////////////
void WindowImplCocoa::ProcessEvents()
{
    [myDelegate processEvent];
}
    
#pragma mark
#pragma mark WindowImplCocoa's private methods

////////////////////////////////////////////////////////////
WindowHandle WindowImplCocoa::GetSystemHandle() const
{
    return [myDelegate getSystemHandle];
}

    
////////////////////////////////////////////////////////////
Vector2i WindowImplCocoa::GetPosition() const
{
    NSPoint pos = [myDelegate position];
    return Vector2i(pos.x, pos.y);
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::SetPosition(const Vector2i& position)
{
    [myDelegate setWindowPositionToX:position.x Y:position.y];
}


////////////////////////////////////////////////////////////
Vector2u WindowImplCocoa::GetSize() const
{
    NSSize size = [myDelegate size];
    return Vector2u(size.width, size.height);
}

    
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetSize(const Vector2u& size)
{    
    [myDelegate resizeTo:size.x by:size.y];
}

    
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetTitle(const std::string& title)
{
    [myDelegate changeTitle:stringToNSString(title)];
}

    
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
    [myDelegate setIconTo:width by:height with:pixels];
}

    
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetVisible(bool visible)
{
    if (visible) {
        [myDelegate showWindow];
    } else {
        [myDelegate hideWindow];
    }
}

    
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetMouseCursorVisible(bool visible)
{
    myShowCursor = visible;
    
    if (myShowCursor) {
        [myDelegate showMouseCursor];
    } else {
        [myDelegate hideMouseCursor];
    }
}

    
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetKeyRepeatEnabled(bool enabled)
{
    if (enabled) {
        [myDelegate enableKeyRepeat];
    } else {
        [myDelegate disableKeyRepeat];
    }
}

    
} // namespace priv
    
} // namespace sf
